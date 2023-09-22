// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_POW_H
#define BITCOIN_POW_H

#include <consensus/params.h>

#include <stdint.h>

class CBlockHeader;
class CBlockIndex;
class uint256;
class CBlock;

struct BeePopGraphPoint {
    int immaturePop;
    int maturePop;
};

extern CAmount totalMatureBees;

extern int thematurebees;
extern int deadBees;

extern int wototo;

extern int switchHmem;
extern int switchLmem;
extern int threshold;


unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params&);
unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params&);
unsigned int GetNextHiveWorkRequired(const CBlockIndex* pindexLast, const Consensus::Params& params);                       // LightningCashr: Hive: Get the current Bee Hash Target
bool CheckHiveProof(const CBlock* pblock, const Consensus::Params& params);                                                 // LightningCashr: Hive: Check the hive proof for given block
bool CheckHiveProof2(const CBlock* pblock, const Consensus::Params& params);                                                 // LightningCashr: Hive: Check the hive proof for given block
bool CheckHiveProof3(const CBlock* pblock, const Consensus::Params& params);                                                 // LightningCashr: Hive: Check the hive proof for given block
bool GetNetworkHiveInfo(int& immatureBees, int& immatureBCTs, int& matureBees, int& matureBCTs, CAmount& potentialLifespanRewards, const Consensus::Params& consensusParams, bool recalcGraph = false); // LightningCashr: Hive: Get count of all live and gestating BCTs on the network
bool GetNetworkHiveInfo2(int& immatureBees, int& immatureBCTs, int& matureBees, int& matureBCTs, CAmount& potentialLifespanRewards, const Consensus::Params& consensusParams, bool recalcGraph = false); // LightningCashr: Hive: Get count of all live and gestating BCTs on the network
bool GetNetworkHiveInfo3(int& immatureBees, int& immatureBCTs, int& matureBees, int& matureBCTs, CAmount& potentialLifespanRewards, const Consensus::Params& consensusParams, bool recalcGraph = false); // LightningCashr: Hive: Get count of all live and gestating BCTs on the network
bool GetNetworkHiveInfo4(int& immatureBees, int& immatureBCTs, int& matureBees, int& matureBCTs, CAmount& potentialLifespanRewards, const Consensus::Params& consensusParams, bool recalcGraph = false); // LightningCashr: Hive: Get count of all live and gestating BCTs on the network

/** Check whether a block hash satisfies the proof-of-work requirement specified by nBits */
bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params&);
bool CheckProofOfWork2(uint256 hash, unsigned int nBits, const Consensus::Params&);



#endif // BITCOIN_POW_H
