// Copyright (c) 2011-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <wallet/wallet.h>
#include <wallet/fees.h>

#include <qt/hivedialog.h>
#include <qt/forms/ui_hivedialog.h>
#include <qt/clientmodel.h>
#include <qt/sendcoinsdialog.h>

#include <qt/addressbookpage.h>
#include <qt/addresstablemodel.h>
#include <qt/bitcoinunits.h>
#include <qt/optionsmodel.h>
#include <qt/platformstyle.h>
#include <qt/receiverequestdialog.h>
#include <qt/hivetablemodel.h>
#include <qt/walletmodel.h>
#include <qt/tinypie.h>
#include <qt/qcustomplot.h>

#include <qt/optionsdialog.h> // LightningCashr: Hive: Mining optimisations

#include <QAction>
#include <QCursor>
#include <QMessageBox>
#include <QScrollBar>
#include <QTextDocument>

#include <util.h>
#include <validation.h>

HiveDialog::HiveDialog(const PlatformStyle *_platformStyle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HiveDialog),
    columnResizingFixer(0),
    model(0),
    platformStyle(_platformStyle)
{
    ui->setupUi(this);

    if (!_platformStyle->getImagesOnButtons())
        ui->createBeesButton->setIcon(QIcon());
    else
        ui->createBeesButton->setIcon(_platformStyle->SingleColorIcon(":/icons/bee"));
    
    beeCost = totalCost = rewardsPaid = cost = profit = 0;
    immature = mature = dead = blocksFound = 0;
    lastGlobalCheckHeight = 0;
    potentialRewards = 0;
    currentBalance = 0;
    beePopIndex = 0;

    ui->globalHiveSummaryError->hide();

    ui->beePopIndexPie->foregroundCol = Qt::red;

    // Swap cols for hive weight pie
    QColor temp = ui->hiveWeightPie->foregroundCol;
    ui->hiveWeightPie->foregroundCol = ui->hiveWeightPie->backgroundCol;
    ui->hiveWeightPie->backgroundCol = temp;
    ui->hiveWeightPie->borderCol = palette().color(backgroundRole());

    initGraph();
    ui->beePopGraph->hide();
}

void HiveDialog::setClientModel(ClientModel *_clientModel) {
    this->clientModel = _clientModel;

    if (_clientModel) {
	if ((chainActive.Tip()->nHeight) >= nSpeedFork) {
		//LogPrintf("OK \n");
		connect(_clientModel, SIGNAL(numBlocksChanged(int,QDateTime,double,bool)), this, SLOT(updateData3()));
		connect(_clientModel, SIGNAL(numConnectionsChanged(int)), this, SLOT(updateData3()));    // TODO: This may be too expensive to call here, and the only point is to update the hive status icon.
	}
	if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
		//LogPrintf("OK \n");
		connect(_clientModel, SIGNAL(numBlocksChanged(int,QDateTime,double,bool)), this, SLOT(updateData3()));
		connect(_clientModel, SIGNAL(numConnectionsChanged(int)), this, SLOT(updateData3()));    // TODO: This may be too expensive to call here, and the only point is to update the hive status icon.
	}
	if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
		//LogPrintf("OK \n");
		connect(_clientModel, SIGNAL(numBlocksChanged(int,QDateTime,double,bool)), this, SLOT(updateData2()));
		connect(_clientModel, SIGNAL(numConnectionsChanged(int)), this, SLOT(updateData2()));    // TODO: This may be too expensive to call here, and the only point is to update the hive status icon.
	}
	if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().variableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
		//LogPrintf("NOT OK \n");
		connect(_clientModel, SIGNAL(numBlocksChanged(int,QDateTime,double,bool)), this, SLOT(updateData()));
		connect(_clientModel, SIGNAL(numConnectionsChanged(int)), this, SLOT(updateData()));    // TODO: This may be too expensive to call here, and the only point is to update the hive status icon.
	}
	

    }
}

void HiveDialog::setModel(WalletModel *_model) {
    this->model = _model;

    if(_model && _model->getOptionsModel())
    {
        _model->getHiveTableModel()->sort(HiveTableModel::Created, Qt::DescendingOrder);
        connect(_model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));
        updateDisplayUnit();

        setBalance(_model->getBalance(), _model->getUnconfirmedBalance(), _model->getImmatureBalance(), _model->getWatchBalance(), _model->getWatchUnconfirmedBalance(), _model->getWatchImmatureBalance());
        connect(_model, SIGNAL(balanceChanged(CAmount,CAmount,CAmount,CAmount,CAmount,CAmount)), this, SLOT(setBalance(CAmount,CAmount,CAmount,CAmount,CAmount,CAmount)));
        
        if (_model->getEncryptionStatus() != WalletModel::Locked)
            ui->releaseSwarmButton->hide();
        connect(_model, SIGNAL(encryptionStatusChanged(int)), this, SLOT(setEncryptionStatus(int)));

        QTableView* tableView = ui->currentHiveView;

        tableView->verticalHeader()->hide();
        tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView->setModel(_model->getHiveTableModel());
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setSelectionMode(QAbstractItemView::ContiguousSelection);
        tableView->setColumnWidth(HiveTableModel::Created, CREATED_COLUMN_WIDTH);
        tableView->setColumnWidth(HiveTableModel::Count, COUNT_COLUMN_WIDTH);
        tableView->setColumnWidth(HiveTableModel::Status, STATUS_COLUMN_WIDTH);
        tableView->setColumnWidth(HiveTableModel::EstimatedTime, TIME_COLUMN_WIDTH);
        tableView->setColumnWidth(HiveTableModel::Cost, COST_COLUMN_WIDTH);
        tableView->setColumnWidth(HiveTableModel::Rewards, REWARDS_COLUMN_WIDTH);
        //tableView->setColumnWidth(HiveTableModel::Profit, PROFIT_COLUMN_WIDTH);

        // Last 2 columns are set by the columnResizingFixer, when the table geometry is ready.
        //columnResizingFixer = new GUIUtil::TableViewLastColumnResizingFixer(tableView, PROFIT_COLUMN_WIDTH, HIVE_COL_MIN_WIDTH, this);
        columnResizingFixer = new GUIUtil::TableViewLastColumnResizingFixer(tableView, REWARDS_COLUMN_WIDTH, HIVE_COL_MIN_WIDTH, this);

        // Populate initial data
	if ((chainActive.Tip()->nHeight) >= nSpeedFork) { 
		//LogPrintf("OK \n");
        	updateData3(true);
	}
	if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) { 
		//LogPrintf("OK \n");
        	updateData3(true);
	}
	if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) { 
		//LogPrintf("OK \n");
        	updateData2(true);
	}
	if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().variableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
		//LogPrintf("NOT OK \n");
		updateData(true);
	}
    }
}

HiveDialog::~HiveDialog() {
    delete ui;
}

void HiveDialog::setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance, const CAmount& watchOnlyBalance, const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance) {
    currentBalance = balance;
    setAmountField(ui->currentBalance, currentBalance);
}

void HiveDialog::setEncryptionStatus(int status) {
    switch(status) {
        case WalletModel::Unencrypted:
        case WalletModel::Unlocked:
            ui->releaseSwarmButton->hide();
            break;
        case WalletModel::Locked:
            ui->releaseSwarmButton->show();
            break;
    }
    if ((chainActive.Tip()->nHeight) >= nSpeedFork) {
	//LogPrintf("OK \n");
    	updateData3();
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("OK \n");
    	updateData3();
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("OK \n");
    	updateData2();
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().variableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("NOT OK \n");
    	updateData();
    }
}

void HiveDialog::setAmountField(QLabel *field, CAmount value) {
    field->setText(
        BitcoinUnits::format(model->getOptionsModel()->getDisplayUnit(), value)
        + " "
        + BitcoinUnits::shortName(model->getOptionsModel()->getDisplayUnit())
    );
}

QString HiveDialog::formatLargeNoLocale(int i) {
    QString i_str = QString::number(i);

    // Use SI-style thin space separators as these are locale independent and can't be confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int i_size = i_str.size();
    for (int i = 3; i < i_size; i += 3)
        i_str.insert(i_size - i, thin_sp);

    return i_str;
}


void HiveDialog::updateData(bool forceGlobalSummaryUpdate) {
    if(IsInitialBlockDownload() || chainActive.Height() == 0) {
        ui->globalHiveSummary->hide();
        ui->globalHiveSummaryError->show();
        return;
    }
    
    const Consensus::Params& consensusParams = Params().GetConsensus();

    if(model && model->getHiveTableModel()) {
        model->getHiveTableModel()->updateBCTs(ui->includeDeadBeesCheckbox->isChecked());
        model->getHiveTableModel()->getSummaryValues(immature, mature, dead, blocksFound, cost, rewardsPaid, profit);
        
        // Update labels
        setAmountField(ui->rewardsPaidLabel, rewardsPaid);
        setAmountField(ui->costLabel, cost);
        setAmountField(ui->profitLabel, profit);
        ui->matureLabel->setText(formatLargeNoLocale(mature));
        ui->immatureLabel->setText(formatLargeNoLocale(immature));
        ui->blocksFoundLabel->setText(QString::number(blocksFound));

        if(dead == 0) {
            ui->deadLabel->hide();
            ui->deadTitleLabel->hide();
            ui->deadLabelSpacer->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        } else {
            ui->deadLabel->setText(formatLargeNoLocale(dead));
            ui->deadLabel->show();
            ui->deadTitleLabel->show();
            ui->deadLabelSpacer->changeSize(ui->immatureLabelSpacer->geometry().width(), 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        }

        // Set icon and tooltip for tray icon
        QString tooltip, icon;
        if (clientModel && clientModel->getNumConnections() == 0) {
            tooltip = "LightningCashr Coin is not connected";
            icon = ":/icons/hivestatus_disabled";
        } else if (!model->isHiveEnabled()) {
            tooltip = "The Hive is not enabled on the network";
            icon = ":/icons/hivestatus_disabled";
        } else {
            if (mature + immature == 0) {
                tooltip = "No live bees currently in wallet";
                icon = ":/icons/hivestatus_clear";
            } else if (mature == 0) {
                tooltip = "Only immature bees currently in wallet";
                icon = ":/icons/hivestatus_orange";
            } else {
                if (model->getEncryptionStatus() == WalletModel::Locked) {
                    tooltip = "WARNING: Bees mature but not mining because wallet is locked";
                    icon = ":/icons/hivestatus_red";
                } else {
                    tooltip = "Bees mature and mining";
                    icon = ":/icons/hivestatus_green";
                }
            }
        }
        // Now update bitcoingui
        Q_EMIT hiveStatusIconChanged(icon, tooltip);
    }

    beeCost = GetBeeCost(chainActive.Tip()->nHeight, consensusParams);
    setAmountField(ui->beeCostLabel, beeCost);
    updateTotalCostDisplay();

    if (forceGlobalSummaryUpdate || chainActive.Tip()->nHeight >= lastGlobalCheckHeight + 10) { // Don't update global summary every block
        int globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs;
	if ((chainActive.Tip()->nHeight) >= nSpeedFork) {
		//LogPrintf("OK \n");
		if (!GetNetworkHiveInfo(globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs, potentialRewards, consensusParams, true)) {
		    ui->globalHiveSummary->hide();
		    ui->globalHiveSummaryError->show();
		} else {
		    ui->globalHiveSummaryError->hide();
		    ui->globalHiveSummary->show();
		    if (globalImmatureBees == 0)
		        ui->globalImmatureLabel->setText("0");
		    else
		        ui->globalImmatureLabel->setText(formatLargeNoLocale(globalImmatureBees) + " (" + QString::number(globalImmatureBCTs) + " transactions)");

		    if (globalMatureBees == 0)
		        ui->globalMatureLabel->setText("0");
		    else
		        ui->globalMatureLabel->setText(formatLargeNoLocale(globalMatureBees) + " (" + QString::number(globalMatureBCTs) + " transactions)");

		    updateGraph();
		}
	}
	if ((consensusParams.variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (consensusParams.ratioForkBlock)) && (((chainActive.Tip()->nHeight) - 1) >= (consensusParams.remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
		//LogPrintf("OK \n");
		if (!GetNetworkHiveInfo4(globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs, potentialRewards, consensusParams, true)) {
		    ui->globalHiveSummary->hide();
		    ui->globalHiveSummaryError->show();
		} else {
		    ui->globalHiveSummaryError->hide();
		    ui->globalHiveSummary->show();
		    if (globalImmatureBees == 0)
		        ui->globalImmatureLabel->setText("0");
		    else
		        ui->globalImmatureLabel->setText(formatLargeNoLocale(globalImmatureBees) + " (" + QString::number(globalImmatureBCTs) + " transactions)");

		    if (globalMatureBees == 0)
		        ui->globalMatureLabel->setText("0");
		    else
		        ui->globalMatureLabel->setText(formatLargeNoLocale(globalMatureBees) + " (" + QString::number(globalMatureBCTs) + " transactions)");

		    updateGraph();
		}
	}
        if ((consensusParams.variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (consensusParams.ratioForkBlock)) && (((chainActive.Tip()->nHeight) - 1) < (consensusParams.remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
		//LogPrintf("OK \n");
		if (!GetNetworkHiveInfo3(globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs, potentialRewards, consensusParams, true)) {
		    ui->globalHiveSummary->hide();
		    ui->globalHiveSummaryError->show();
		} else {
		    ui->globalHiveSummaryError->hide();
		    ui->globalHiveSummary->show();
		    if (globalImmatureBees == 0)
		        ui->globalImmatureLabel->setText("0");
		    else
		        ui->globalImmatureLabel->setText(formatLargeNoLocale(globalImmatureBees) + " (" + QString::number(globalImmatureBCTs) + " transactions)");

		    if (globalMatureBees == 0)
		        ui->globalMatureLabel->setText("0");
		    else
		        ui->globalMatureLabel->setText(formatLargeNoLocale(globalMatureBees) + " (" + QString::number(globalMatureBCTs) + " transactions)");

		    updateGraph();
		}
	}
	if ((consensusParams.variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (consensusParams.variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) < (consensusParams.ratioForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
		//LogPrintf("OK \n");
		if (!GetNetworkHiveInfo2(globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs, potentialRewards, consensusParams, true)) {
		    ui->globalHiveSummary->hide();
		    ui->globalHiveSummaryError->show();
		} else {
		    ui->globalHiveSummaryError->hide();
		    ui->globalHiveSummary->show();
		    if (globalImmatureBees == 0)
		        ui->globalImmatureLabel->setText("0");
		    else
		        ui->globalImmatureLabel->setText(formatLargeNoLocale(globalImmatureBees) + " (" + QString::number(globalImmatureBCTs) + " transactions)");

		    if (globalMatureBees == 0)
		        ui->globalMatureLabel->setText("0");
		    else
		        ui->globalMatureLabel->setText(formatLargeNoLocale(globalMatureBees) + " (" + QString::number(globalMatureBCTs) + " transactions)");

		    updateGraph();
		}
	}
	if ((consensusParams.variableBeecost) && (((chainActive.Tip()->nHeight) - 1) < (consensusParams.variableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
		//LogPrintf("NOT OK \n");
		if (!GetNetworkHiveInfo(globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs, potentialRewards, consensusParams, true)) {
		    ui->globalHiveSummary->hide();
		    ui->globalHiveSummaryError->show();
		} else {
		    ui->globalHiveSummaryError->hide();
		    ui->globalHiveSummary->show();
		    if (globalImmatureBees == 0)
		        ui->globalImmatureLabel->setText("0");
		    else
		        ui->globalImmatureLabel->setText(formatLargeNoLocale(globalImmatureBees) + " (" + QString::number(globalImmatureBCTs) + " transactions)");

		    if (globalMatureBees == 0)
		        ui->globalMatureLabel->setText("0");
		    else
		        ui->globalMatureLabel->setText(formatLargeNoLocale(globalMatureBees) + " (" + QString::number(globalMatureBCTs) + " transactions)");

		    updateGraph();
		}
	}

        setAmountField(ui->potentialRewardsLabel, potentialRewards);

        double hiveWeight = mature / (double)globalMatureBees;
        ui->localHiveWeightLabel->setText((mature == 0 || globalMatureBees == 0) ? "0" : QString::number(hiveWeight, 'f', 3));
        ui->hiveWeightPie->setValue(hiveWeight);

        beePopIndex = ((beeCost * globalMatureBees) / (double)potentialRewards) * 100.0;
        if (beePopIndex > 200) beePopIndex = 200;
        ui->beePopIndexLabel->setText(QString::number(floor(beePopIndex)));
        ui->beePopIndexPie->setValue(beePopIndex / 100);
        
        lastGlobalCheckHeight = chainActive.Tip()->nHeight;
    }

    ui->blocksTillGlobalRefresh->setText(QString::number(10 - (chainActive.Tip()->nHeight - lastGlobalCheckHeight)));
}


void HiveDialog::updateData2(bool forceGlobalSummaryUpdate) {
    if(IsInitialBlockDownload() || chainActive.Height() == 0) {
        ui->globalHiveSummary->hide();
        ui->globalHiveSummaryError->show();
        return;
    }
    
    const Consensus::Params& consensusParams = Params().GetConsensus();

    if(model && model->getHiveTableModel()) {
        model->getHiveTableModel()->updateBCTs(ui->includeDeadBeesCheckbox->isChecked());
        model->getHiveTableModel()->getSummaryValues(immature, mature, dead, blocksFound, cost, rewardsPaid, profit);
        
        // Update labels
        setAmountField(ui->rewardsPaidLabel, rewardsPaid);
        setAmountField(ui->costLabel, cost);
        setAmountField(ui->profitLabel, profit);
        ui->matureLabel->setText(formatLargeNoLocale(mature));
        ui->immatureLabel->setText(formatLargeNoLocale(immature));
        ui->blocksFoundLabel->setText(QString::number(blocksFound));

        if(dead == 0) {
            ui->deadLabel->hide();
            ui->deadTitleLabel->hide();
            ui->deadLabelSpacer->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        } else {
            ui->deadLabel->setText(formatLargeNoLocale(dead));
            ui->deadLabel->show();
            ui->deadTitleLabel->show();
            ui->deadLabelSpacer->changeSize(ui->immatureLabelSpacer->geometry().width(), 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        }

        // Set icon and tooltip for tray icon
        QString tooltip, icon;
        if (clientModel && clientModel->getNumConnections() == 0) {
            tooltip = "LightningCashr Coin is not connected";
            icon = ":/icons/hivestatus_disabled";
        } else if (!model->isHiveEnabled()) {
            tooltip = "The Hive is not enabled on the network";
            icon = ":/icons/hivestatus_disabled";
        } else {
            if (mature + immature == 0) {
                tooltip = "No live bees currently in wallet";
                icon = ":/icons/hivestatus_clear";
            } else if (mature == 0) {
                tooltip = "Only immature bees currently in wallet";
                icon = ":/icons/hivestatus_orange";
            } else {
                if (model->getEncryptionStatus() == WalletModel::Locked) {
                    tooltip = "WARNING: Bees mature but not mining because wallet is locked";
                    icon = ":/icons/hivestatus_red";
                } else {
                    tooltip = "Bees mature and mining";
                    icon = ":/icons/hivestatus_green";
                }
            }
        }
        // Now update bitcoingui
        Q_EMIT hiveStatusIconChanged(icon, tooltip);
    }
    int HeightX = (chainActive.Height() - 1);
    //CBlockIndex* osti = chainActive.Genesis();
    //int mauditcaca = chainActive.GetBlockTime();
    int vadonchier = wototo; // matureBees
    
    int superX = threshold; // # of bees for global index at 90
    //LogPrintf("is %i <= %i ??? if so, low cost !! \n", vadonchier, superX);
    
    if (vadonchier <= superX) // if maturebees is under 90...
	beeCost = 0.0004*(GetBlockSubsidy(HeightX, consensusParams));
    else                   // memory is impair
	beeCost = 0.0008*(GetBlockSubsidy(HeightX, consensusParams));
    //beeCost = GetBeeCost(chainActive.Tip()->nHeight, consensusParams); // PROBLEM
    //beeCost = 0.0004*(GetBlockSubsidy(HeightX, consensusParams));
    //LogPrintf("beeCost in Hivedialog.cpp = %i \n", beeCost);
    setAmountField(ui->beeCostLabel, beeCost);
    updateTotalCostDisplay();

    if (forceGlobalSummaryUpdate || chainActive.Tip()->nHeight >= (lastGlobalCheckHeight + 10)) { // Don't update global summary every block
        int globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs;
	//LogPrintf("thematurebees in hivedialog = %i \n", thematurebees);
	//LogPrintf("deadmatureBees = %i \n", deadmatureBees);
	//globalMatureBees = (globalMatureBees - deadmatureBees);
	int flute = thematurebees;
	//LogPrintf("thematurebees - deadBees = %i (flute in hivedialog.cpp and coucou in pow.cpp) \n", flute);
	if ((consensusParams.variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (consensusParams.variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) >= (consensusParams.ratioForkBlock)) && (((chainActive.Tip()->nHeight) - 1) >= (consensusParams.remvariableForkBlock))) {
		//LogPrintf("OK \n");

		if (!GetNetworkHiveInfo4(globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs, potentialRewards, consensusParams, true)) {
		    ui->globalHiveSummary->hide();
		    ui->globalHiveSummaryError->show();
		} else {
		    ui->globalHiveSummaryError->hide();
		    ui->globalHiveSummary->show();
		    if (globalImmatureBees == 0)
		        ui->globalImmatureLabel->setText("0");
		    else
		        ui->globalImmatureLabel->setText(formatLargeNoLocale(globalImmatureBees) + " (" + QString::number(globalImmatureBCTs) + " transactions)");

		    if (flute == 0)
		        ui->globalMatureLabel->setText("0");
		    else
		        ui->globalMatureLabel->setText(formatLargeNoLocale(flute) + " (" + QString::number(globalMatureBCTs) + " transactions)");

		    updateGraph();
		}

	}
        if ((consensusParams.variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (consensusParams.variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) >= (consensusParams.ratioForkBlock)) && (((chainActive.Tip()->nHeight) - 1) < (consensusParams.remvariableForkBlock))) {
		//LogPrintf("OK \n");

		if (!GetNetworkHiveInfo3(globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs, potentialRewards, consensusParams, true)) {
		    ui->globalHiveSummary->hide();
		    ui->globalHiveSummaryError->show();
		} else {
		    ui->globalHiveSummaryError->hide();
		    ui->globalHiveSummary->show();
		    if (globalImmatureBees == 0)
		        ui->globalImmatureLabel->setText("0");
		    else
		        ui->globalImmatureLabel->setText(formatLargeNoLocale(globalImmatureBees) + " (" + QString::number(globalImmatureBCTs) + " transactions)");

		    if (flute == 0)
		        ui->globalMatureLabel->setText("0");
		    else
		        ui->globalMatureLabel->setText(formatLargeNoLocale(flute) + " (" + QString::number(globalMatureBCTs) + " transactions)");

		    updateGraph();
		}

	}
	if ((consensusParams.variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (consensusParams.variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) < (consensusParams.ratioForkBlock))) {
		//LogPrintf("OK \n");

		if (!GetNetworkHiveInfo2(globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs, potentialRewards, consensusParams, true)) {
		    ui->globalHiveSummary->hide();
		    ui->globalHiveSummaryError->show();
		} else {
		    ui->globalHiveSummaryError->hide();
		    ui->globalHiveSummary->show();
		    if (globalImmatureBees == 0)
		        ui->globalImmatureLabel->setText("0");
		    else
		        ui->globalImmatureLabel->setText(formatLargeNoLocale(globalImmatureBees) + " (" + QString::number(globalImmatureBCTs) + " transactions)");

		    if (flute == 0)
		        ui->globalMatureLabel->setText("0");
		    else
		        ui->globalMatureLabel->setText(formatLargeNoLocale(flute) + " (" + QString::number(globalMatureBCTs) + " transactions)");

		    updateGraph();
		}

	}
	if ((consensusParams.variableBeecost) && (((chainActive.Tip()->nHeight) - 1) < (consensusParams.variableForkBlock))) {
		//LogPrintf("NOT OK \n");
		if (!GetNetworkHiveInfo(globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs, potentialRewards, consensusParams, true)) {
		    ui->globalHiveSummary->hide();
		    ui->globalHiveSummaryError->show();
		} else {
		    ui->globalHiveSummaryError->hide();
		    ui->globalHiveSummary->show();
		    if (globalImmatureBees == 0)
		        ui->globalImmatureLabel->setText("0");
		    else
		        ui->globalImmatureLabel->setText(formatLargeNoLocale(globalImmatureBees) + " (" + QString::number(globalImmatureBCTs) + " transactions)");

		    if (flute == 0)
		        ui->globalMatureLabel->setText("0");
		    else
		        ui->globalMatureLabel->setText(formatLargeNoLocale(flute) + " (" + QString::number(globalMatureBCTs) + " transactions)");

		    updateGraph();
		}
	}

        setAmountField(ui->potentialRewardsLabel, potentialRewards);

        double hiveWeight = mature / (double)flute;
        ui->localHiveWeightLabel->setText((mature == 0 || flute == 0) ? "0" : QString::number(hiveWeight, 'f', 3));
        ui->hiveWeightPie->setValue(hiveWeight);
	/*LogPrintf("memory = %i \n", memory);
	if ((!(memory % 2)) || (memory = 0))
       		beePopIndex = (((0.0004*(GetBlockSubsidy(HeightX, consensusParams))) * flute) / (double)potentialRewards) * 100.0; // PROBLEM ---> want it to always be calculated according to base bee cost....
	else
		beePopIndex = (((0.0008*(GetBlockSubsidy(HeightX, consensusParams))) * flute) / (double)potentialRewards) * 100.0;
	LogPrintf("beePopIndex = %i \n", beePopIndex);
	//beePopIndex = (((0.0004*(GetBlockSubsidy(HeightX, consensusParams))) * flute) / (double)potentialRewards) * 100.0; // PROBLEM ---> want it to always be calculated according to base bee cost....*/

	//beePopIndex = (flute*5) / (double)potentialRewards) * 100.0; // total global mature bees X ( 1 / basecost )

	// (consensusParams.beeLifespanBlocks * GetBlockSubsidy(pindexLast->nHeight, consensusParams)) / consensusParams.hiveBlockSpacingTarget;

	CAmount npotentialRewards = (consensusParams.beeLifespanBlocks * GetBlockSubsidy(chainActive.Tip()->nHeight, consensusParams)) / consensusParams.hiveBlockSpacingTarget; // to show Global Index based on NORMAL totalBeeLifespan


	beePopIndex = (((0.0004*(GetBlockSubsidy(HeightX, consensusParams))) * flute) / (double)npotentialRewards) * 100.0; // low price times thematurebees.... on 
        LogPrintf("beePopIndex = %i \n", beePopIndex);
        if (beePopIndex > 200) beePopIndex = 200;
        ui->beePopIndexLabel->setText(QString::number(floor(beePopIndex)));
        ui->beePopIndexPie->setValue(beePopIndex / 100);
        
        lastGlobalCheckHeight = chainActive.Tip()->nHeight;
    }

    ui->blocksTillGlobalRefresh->setText(QString::number(10 - (chainActive.Tip()->nHeight - lastGlobalCheckHeight)));
}

void HiveDialog::updateData3(bool forceGlobalSummaryUpdate) {
    if(IsInitialBlockDownload() || chainActive.Height() == 0) {
        ui->globalHiveSummary->hide();
        ui->globalHiveSummaryError->show();
        return;
    }
    
    const Consensus::Params& consensusParams = Params().GetConsensus();

    if(model && model->getHiveTableModel()) {
        model->getHiveTableModel()->updateBCTs(ui->includeDeadBeesCheckbox->isChecked());
        model->getHiveTableModel()->getSummaryValues(immature, mature, dead, blocksFound, cost, rewardsPaid, profit);
        
        // Update labels
        setAmountField(ui->rewardsPaidLabel, rewardsPaid);
        setAmountField(ui->costLabel, cost);
        setAmountField(ui->profitLabel, profit);
        ui->matureLabel->setText(formatLargeNoLocale(mature));
        ui->immatureLabel->setText(formatLargeNoLocale(immature));
        ui->blocksFoundLabel->setText(QString::number(blocksFound));

        if(dead == 0) {
            ui->deadLabel->hide();
            ui->deadTitleLabel->hide();
            ui->deadLabelSpacer->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        } else {
            ui->deadLabel->setText(formatLargeNoLocale(dead));
            ui->deadLabel->show();
            ui->deadTitleLabel->show();
            ui->deadLabelSpacer->changeSize(ui->immatureLabelSpacer->geometry().width(), 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        }

        // Set icon and tooltip for tray icon
        QString tooltip, icon;
        if (clientModel && clientModel->getNumConnections() == 0) {
            tooltip = "LightningCashr Coin is not connected";
            icon = ":/icons/hivestatus_disabled";
        } else if (!model->isHiveEnabled()) {
            tooltip = "The Hive is not enabled on the network";
            icon = ":/icons/hivestatus_disabled";
        } else {
            if (mature + immature == 0) {
                tooltip = "No live bees currently in wallet";
                icon = ":/icons/hivestatus_clear";
            } else if (mature == 0) {
                tooltip = "Only immature bees currently in wallet";
                icon = ":/icons/hivestatus_orange";
            } else {
                if (model->getEncryptionStatus() == WalletModel::Locked) {
                    tooltip = "WARNING: Bees mature but not mining because wallet is locked";
                    icon = ":/icons/hivestatus_red";
                } else {
                    tooltip = "Bees mature and mining";
                    icon = ":/icons/hivestatus_green";
                }
            }
        }
        // Now update bitcoingui
        Q_EMIT hiveStatusIconChanged(icon, tooltip);
    }

    beeCost = GetBeeCost(chainActive.Tip()->nHeight, consensusParams);
    setAmountField(ui->beeCostLabel, beeCost);
    updateTotalCostDisplay();

    if (forceGlobalSummaryUpdate || chainActive.Tip()->nHeight >= lastGlobalCheckHeight + 10) { // Don't update global summary every block
        int globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs;
        if (!GetNetworkHiveInfo4(globalImmatureBees, globalImmatureBCTs, globalMatureBees, globalMatureBCTs, potentialRewards, consensusParams, true)) {
            ui->globalHiveSummary->hide();
            ui->globalHiveSummaryError->show();
        } else {
            ui->globalHiveSummaryError->hide();
            ui->globalHiveSummary->show();
            if (globalImmatureBees == 0)
                ui->globalImmatureLabel->setText("0");
            else
                ui->globalImmatureLabel->setText(formatLargeNoLocale(globalImmatureBees) + " (" + QString::number(globalImmatureBCTs) + " transactions)");

            if (globalMatureBees == 0)
                ui->globalMatureLabel->setText("0");
            else
                ui->globalMatureLabel->setText(formatLargeNoLocale(globalMatureBees) + " (" + QString::number(globalMatureBCTs) + " transactions)");

            updateGraph();
        }

        setAmountField(ui->potentialRewardsLabel, potentialRewards);

        double hiveWeight = mature / (double)globalMatureBees;
        ui->localHiveWeightLabel->setText((mature == 0 || globalMatureBees == 0) ? "0" : QString::number(hiveWeight, 'f', 3));
        ui->hiveWeightPie->setValue(hiveWeight);

        beePopIndex = ((beeCost * globalMatureBees) / (double)potentialRewards) * 100.0;
        if (beePopIndex > 200) beePopIndex = 200;
        ui->beePopIndexLabel->setText(QString::number(floor(beePopIndex)));
        ui->beePopIndexPie->setValue(beePopIndex / 100);
        
        lastGlobalCheckHeight = chainActive.Tip()->nHeight;
    }

    ui->blocksTillGlobalRefresh->setText(QString::number(10 - (chainActive.Tip()->nHeight - lastGlobalCheckHeight)));
}

void HiveDialog::updateDisplayUnit() {
    if(model && model->getOptionsModel()) {
        setAmountField(ui->beeCostLabel, beeCost);
        setAmountField(ui->rewardsPaidLabel, rewardsPaid);
        setAmountField(ui->costLabel, cost);
        setAmountField(ui->profitLabel, profit);
        setAmountField(ui->potentialRewardsLabel, potentialRewards);
        setAmountField(ui->currentBalance, currentBalance);
        setAmountField(ui->totalCostLabel, totalCost);
    }

    updateTotalCostDisplay();
}

void HiveDialog::updateTotalCostDisplay() {    
    totalCost = beeCost * ui->beeCountSpinner->value();

    if(model && model->getOptionsModel()) {
        setAmountField(ui->totalCostLabel, totalCost);
        
        if (totalCost > model->getBalance())
            ui->beeCountSpinner->setStyleSheet("QSpinBox{background:#FF8080;}");
        else
            ui->beeCountSpinner->setStyleSheet("QSpinBox{background:white;}");
    }
}

void HiveDialog::on_beeCountSpinner_valueChanged(int i) {
    updateTotalCostDisplay();
}

void HiveDialog::on_includeDeadBeesCheckbox_stateChanged() {
    if ((chainActive.Tip()->nHeight) >= nSpeedFork) {
	//LogPrintf("OK \n");
    	updateData3();
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("OK \n");
    	updateData3();
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock))  && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("OK \n");
    	updateData2();
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().variableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("NOT OK \n");
    	updateData();
    }
}

void HiveDialog::on_showAdvancedStatsCheckbox_stateChanged() {
    if(ui->showAdvancedStatsCheckbox->isChecked())
        ui->beePopGraph->show();
    else
        ui->beePopGraph->hide();
}

void HiveDialog::on_retryGlobalSummaryButton_clicked() {
    if ((chainActive.Tip()->nHeight) >= nSpeedFork) {
	//LogPrintf("OK \n");
    	updateData3(true);
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("OK \n");
    	updateData3(true);
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("OK \n");
    	updateData2(true);
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().variableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("NOT OK \n");
    	updateData(true);
    }
}

void HiveDialog::on_refreshGlobalSummaryButton_clicked() {
    if ((chainActive.Tip()->nHeight) >= nSpeedFork) {
	//LogPrintf("OK \n");
    	updateData3(true);
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("OK \n");
    	updateData3(true);
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("OK \n");
    	updateData2(true);
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().variableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("NOT OK \n");
    	updateData(true);
    }
}

void HiveDialog::on_releaseSwarmButton_clicked() {
    if(model)
        model->requestUnlock(true);
}

void HiveDialog::on_createBeesButton_clicked() {
    if ((chainActive.Tip()->nHeight) >= nSpeedFork) {
	//LogPrintf("OK \n");
    	updateData3(true);
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("OK \n");
    	updateData3(true);
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) >= (Params().GetConsensus().variableForkBlock)) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().remvariableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("OK \n");
    	updateData2(true);
    }
    if ((Params().GetConsensus().variableBeecost) && (((chainActive.Tip()->nHeight) - 1) < (Params().GetConsensus().variableForkBlock)) && ((chainActive.Tip()->nHeight) < nSpeedFork)) {
	//LogPrintf("NOT OK \n");
    	updateData(true);
    }
    
    if (model) {
        if (totalCost > model->getBalance()) {
            QMessageBox::critical(this, tr("Error"), tr("Insufficient balance to create bees."));
            return;
        }
		WalletModel::UnlockContext ctx(model->requestUnlock());
		if(!ctx.isValid())
			return;     // Unlock wallet was cancelled
        model->createBees(ui->beeCountSpinner->value(), ui->donateCommunityFundCheckbox->isChecked(), this, beePopIndex);
    }
}

// LightningCashr: Hive: Mining optimisations: Shortcut to Hive mining options
void HiveDialog::on_showHiveOptionsButton_clicked() {
    if(!clientModel || !clientModel->getOptionsModel())
        return;

    OptionsDialog dlg(this, model->isWalletEnabled());
    dlg.setModel(clientModel->getOptionsModel());
    dlg.exec();
}

void HiveDialog::initGraph() {
    ui->beePopGraph->addGraph();
    ui->beePopGraph->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->beePopGraph->graph(0)->setPen(QPen(Qt::blue));
    QColor color(42, 67, 182);
    color.setAlphaF(0.35);
    ui->beePopGraph->graph(0)->setBrush(QBrush(color));

    ui->beePopGraph->addGraph();
    ui->beePopGraph->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->beePopGraph->graph(1)->setPen(QPen(Qt::black));
    QColor color1(42, 182, 67);
    color1.setAlphaF(0.35);
    ui->beePopGraph->graph(1)->setBrush(QBrush(color1));

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssMeetTickCount);
    dateTicker->setTickCount(8);
    dateTicker->setDateTimeFormat("ddd d MMM");
    ui->beePopGraph->xAxis->setTicker(dateTicker);

    ui->beePopGraph->yAxis->setLabel("Bees");

    giTicker = QSharedPointer<QCPAxisTickerGI>(new QCPAxisTickerGI);
    ui->beePopGraph->yAxis2->setTicker(giTicker);
    ui->beePopGraph->yAxis2->setLabel("Global index");
    ui->beePopGraph->yAxis2->setVisible(true);

    ui->beePopGraph->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->beePopGraph->xAxis2->setTickLabelFont(QFont(QFont().family(), 8));
    ui->beePopGraph->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->beePopGraph->yAxis2->setTickLabelFont(QFont(QFont().family(), 8));

    connect(ui->beePopGraph->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->beePopGraph->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->beePopGraph->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->beePopGraph->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->beePopGraph, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onMouseMove(QMouseEvent*)));

    globalMarkerLine = new QCPItemLine(ui->beePopGraph);
    globalMarkerLine->setPen(QPen(Qt::blue, 1, Qt::DashLine));
    
    graphTracerImmature = new QCPItemTracer(ui->beePopGraph);
    graphTracerImmature->setGraph(ui->beePopGraph->graph(0));
    graphTracerMature = new QCPItemTracer(ui->beePopGraph);
    graphTracerMature->setGraph(ui->beePopGraph->graph(1));

    graphMouseoverText = new QCPItemText(ui->beePopGraph);
}

void HiveDialog::updateGraph() {
    const Consensus::Params& consensusParams = Params().GetConsensus();

    ui->beePopGraph->graph()->data()->clear();
    double now = QDateTime::currentDateTime().toTime_t();
    int totalLifespan;
    if (chainActive.Height() >= nAdjustFork)
        totalLifespan = consensusParams.beeGestationBlocks + consensusParams.beeLifespanBlocks;
    if ((chainActive.Height() >= nSpeedFork) && (chainActive.Height() < nAdjustFork))
        totalLifespan = consensusParams.beeGestationBlocks + consensusParams.beeLifespanBlocks3;
    if ((chainActive.Height() >= consensusParams.ratioForkBlock) && (chainActive.Height() < nSpeedFork))
        totalLifespan = consensusParams.beeGestationBlocks + consensusParams.beeLifespanBlocks2;
    if (chainActive.Height() < consensusParams.ratioForkBlock)
        totalLifespan = consensusParams.beeGestationBlocks + consensusParams.beeLifespanBlocks;
    
    
    
    QVector<QCPGraphData> dataMature(totalLifespan);
    QVector<QCPGraphData> dataImmature(totalLifespan);
    for (int i = 0; i < totalLifespan; i++) { // PROBLEM
        dataImmature[i].key = now + consensusParams.nPowTargetSpacing2 / 2 * i;
        dataImmature[i].value = (double)beePopGraph[i].immaturePop;

        dataMature[i].key = dataImmature[i].key;
        dataMature[i].value = (double)beePopGraph[i].maturePop;
    }
    ui->beePopGraph->graph(0)->data()->set(dataImmature);
    ui->beePopGraph->graph(1)->data()->set(dataMature);
    
    int HeightXosti = (chainActive.Height() - 1);


    int beeCostStable = 0.0004*(GetBlockSubsidy(chainActive.Tip()->nHeight, consensusParams)); 

    double global100 = (double)potentialRewards / beeCostStable;
    globalMarkerLine->start->setCoords(now, global100);
    globalMarkerLine->end->setCoords(now + consensusParams.nPowTargetSpacing2 / 2 * totalLifespan, global100);
    giTicker->global100 = global100;
    ui->beePopGraph->rescaleAxes();
    ui->beePopGraph->replot();
}

void HiveDialog::onMouseMove(QMouseEvent *event) {
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(sender());

    int x = (int)customPlot->xAxis->pixelToCoord(event->pos().x());
    int y = (int)customPlot->yAxis->pixelToCoord(event->pos().y());

    graphTracerImmature->setGraphKey(x);
    graphTracerMature->setGraphKey(x);
    int beeCountImmature = (int)graphTracerImmature->position->value();
    int beeCountMature = (int)graphTracerMature->position->value();      

    QDateTime xDateTime = QDateTime::fromTime_t(x);
    int global100 = (int)((double)potentialRewards / beeCost);
    QColor traceColMature = beeCountMature >= global100 ? Qt::red : Qt::black;
    QColor traceColImmature = beeCountImmature >= global100 ? Qt::red : Qt::black;

    graphTracerImmature->setPen(QPen(traceColImmature, 1, Qt::DashLine));    
    graphTracerMature->setPen(QPen(traceColMature, 1, Qt::DashLine));

    graphMouseoverText->setText(xDateTime.toString("ddd d MMM") + " " + xDateTime.time().toString() + ":\n" + formatLargeNoLocale(beeCountMature) + " mature bees\n" + formatLargeNoLocale(beeCountImmature) + " immature bees");
    graphMouseoverText->setColor(traceColMature);
    graphMouseoverText->position->setCoords(QPointF(x, y));
    QPointF pixelPos = graphMouseoverText->position->pixelPosition();

    int xoffs, yoffs;
    if (ui->beePopGraph->height() > 150) {
        graphMouseoverText->setFont(QFont(font().family(), 10));
        xoffs = 80;
        yoffs = 30;
    } else {
        graphMouseoverText->setFont(QFont(font().family(), 8));
        xoffs = 70;
        yoffs = 20;
    }

    if (pixelPos.y() > ui->beePopGraph->height() / 2)
        pixelPos.setY(pixelPos.y() - yoffs);
    else
        pixelPos.setY(pixelPos.y() + yoffs);

    if (pixelPos.x() > ui->beePopGraph->width() / 2)
        pixelPos.setX(pixelPos.x() - xoffs);
    else
        pixelPos.setX(pixelPos.x() + xoffs);

    
    graphMouseoverText->position->setPixelPosition(pixelPos);

    customPlot->replot();
}

void HiveDialog::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    columnResizingFixer->stretchColumnWidth(HiveTableModel::Rewards);
}
