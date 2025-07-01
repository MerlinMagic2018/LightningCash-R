// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Copyright (c) 2019-2025 Antoine Brûlé
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>
#include <arith_uint256.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>
#include <base58.h> // LightningCashr: Needed for DecodeDestination()

#include <assert.h>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "LNC Reborn";
    const CScript genesisOutputScript = CScript() << ParseHex("04d603f5d711578999d92907b6d8c020d5900a025be2dd9b79c0a026def0e2f62101f14319b0f4f783204fd79dc4976a2596885d256c6a7a8cc8c38d67a5e49249") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 8400000;
        consensus.BIP16Height = 0; // enforce BIP16 at start !
        consensus.BIP34Height = 71000000; // never happens
        consensus.BIP34Hash = uint256S("fa09d204a83a768ed5a7c8d441fa62f2043abf420cff1226c7b4329aeb9d51cf");
        consensus.BIP65Height = 91868400; // never happens
        consensus.BIP66Height = 81187900; // never happens
        consensus.powLimit = uint256S("00000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
	consensus.powLimit2 = uint256S("0007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4440;
        consensus.nPowTargetSpacing = 60; // target of 1 minute per block
        consensus.nPowTargetSpacing2 = 20; // target of 10 seconds per block
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 222; // 75% of 256
        consensus.nMinerConfirmationWindow = 296; // ( nPowTargetTimespan / nPowTargetSpacing ) * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008
	

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1681749846 + 5259486;  // June 17, 2023
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1681749846 + 31536000 + 5259486;  // June 16, 2024

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE; // active from the start
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // LightningCashr: Hive: Deployment
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE].bit = 7;
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE; // active from the start
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // LightningCashr: Hive 1.1: Deployment
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_1].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_1].nStartTime = 1695443728;  // September 22, 2023 
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_1].nTimeout = 1695443728 + 31536000 + 5259486;  // September 22, 2024

        // LightningCashr: Hive 1.2: Deployment
 //       consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_2].bit = 10;
 //       consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_2].nStartTime = 1566002000;  // active from the start
 //       consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_2].nTimeout = 1566002000 + 31536000;  // active from the start

        // LightningCashr fields
        consensus.powForkTime = 1694548330;                 // Time of PoW hash method change
        consensus.lastScryptBlock = 0;                // Height of last scrypt block
        consensus.powLimitSHA = uint256S("00000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");   // Initial hash target at fork ( for scrypt hashing !!! )
        consensus.slowStartBlocks = 0;                   // Scale post-fork block reward up over this many blocks
//        consensus.premineAmount =  550000;                   // Premine amount (1% of max supply)
//        std::vector<unsigned char> vch = ParseHex("76a914c9f3305556963e2976ccf3348b89a6cc736b6a4e88ac");
//        consensus.premineOutputScript = CScript(vch.begin(), vch.end());	// Output script for premine block (CLagBwF71hqPEKasbmugxBqmyWGFf6ZjMc)
        consensus.totalMoneySupplyHeight = 6215968;         // Height at which TMS is reached, do not issue rewards past this point... will have to be recalculated...

        // LightningCashr: Hive: Consensus Fields
        consensus.minBeeCost = 10000;                       // Minimum cost of a bee, used when no more block rewards
        consensus.beeCostFactor = 2500;                     // Bee cost is block_reward/beeCostFactor
        consensus.beeCreationAddress = "CReateLitecoinCashWorkerBeeXYs19YQ";        // Unspendable address for bee creation
        consensus.hiveCommunityAddress = "CeckYLfkWnViDxKE1R5vLZaarygLrNgUwa";      // Community fund address
        consensus.hiveCommunityAddress2 = "CH5B3qVbJVxVCL5qwmXayEo97Z1Ux3pqSB";
        consensus.communityContribFactor = 10;              // Optionally, donate bct_value/maxCommunityContribFactor to community fund
        consensus.communityContribFactor2 = 2;              // Optionally, donate bct_value/maxCommunityContribFactor to community fund
        consensus.beeGestationBlocks = 48 * 24;               // The number of blocks for a new bee to mature 48*51
        consensus.beeLifespanBlocks = 48 * 24 * 14;              // The number of blocks a bee lives for after maturation
        consensus.powLimitHive = uint256S("0ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");  // Highest (easiest) bee hash target
        consensus.powLimitHive2 = uint256S("7ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");  // Highest (easiest) bee hash target
        consensus.minHiveCheckBlock = 1;              // Don't bother checking below this height for Hive blocks (not used for consensus/validation checks, just efficiency when looking for potential BCTs)
        consensus.hiveTargetAdjustAggression = 30;          // Snap speed for bee hash target adjustment EMA
        consensus.hiveBlockSpacingTarget = 2;               // Target Hive block frequency (1 out of this many blocks should be Hivemined)
        consensus.hiveNonceMarker = 192;                    // Nonce marker for hivemined blocks

        // LightningCashr: Hive 1.1-related consensus fields
        consensus.minK = 1;                                 // Minimum chainwork scale for Hive blocks (see Hive whitepaper section 5)
        consensus.maxK = 8;                                 // Maximum chainwork scale for Hive blocks (see Hive whitepaper section 5)
        consensus.maxK2 = 7;                                 // Maximum chainwork scale for Hive blocks (see Hive whitepaper section 5)
        consensus.maxHiveDiff = 0.001;                      // Hive difficulty at which max chainwork bonus is awarded
        consensus.maxHiveDiff2 = 0.006;                      // Hive difficulty at which max chainwork bonus is awarded
        consensus.maxKPow = 5;                              // Maximum chainwork scale for PoW blocks
        consensus.powSplit1 = 0.000833;                        // Below this Hive difficulty threshold, PoW block chainwork bonus is halved
        consensus.powSplit2 = 0.000416;                       // Below this Hive difficulty threshold, PoW block chainwork bonus is halved again
        consensus.powSplit12 = 0.005;                        // Below this Hive difficulty threshold, PoW block chainwork bonus is halved
        consensus.powSplit22 = 0.0025;                       // Below this Hive difficulty threshold, PoW block chainwork bonus is halved again
        consensus.maxConsecutiveHiveBlocks = 2;             // Maximum hive blocks that can occur consecutively before a PoW block is required
        consensus.hiveDifficultyWindow = 24;                // How many blocks the SMA averages over in hive difficulty adjust

	consensus.variableBeecost = true;	// need to check 
	consensus.variableForkBlock = 5000000;   // need to check 
	consensus.isTestnet = false;
	consensus.ratioForkBlock = 5000000;
        consensus.beeLifespanBlocks2 = 48*24*21;
        consensus.beeLifespanBlocks3 = 48*24*21;
	consensus.remvariableForkBlock = 5000000;
        
        // The best chain should have at least this much work.

         consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000089fe179b775b");  // LightningCashr: block 5362791
        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x9e881790e1bb01f8f43fc8f3c49f1ca2f257ddf9f7042800aaa512e475727528"); // LightningCashr :  block 5362791



        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */ 
        pchMessageStart[0] = 0xea;
        pchMessageStart[1] = 0xaf;
        pchMessageStart[2] = 0x47;
        pchMessageStart[3] = 0x48;
        nDefaultPort = 9113;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1694548330, 1500284949, 0x1e0ffff0, 1, 1.25 * COIN * COIN_SCALE);			
			
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x35f373b6fed4342ef20327917f756dc5fe76b4f2c111ac0a1c52ba408764709b"));
        assert(genesis.hashMerkleRoot == uint256S("0x4caf91337a748b3d946f7f145bc180da5c9281d6323113c0535d77b808526e5a"));

        // Note that of those with the service bits flag, most only support a subset of possible options

        //vSeeds.clear();
        //vSeeds.emplace_back("seeds.lncr.com");


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,28);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1,176);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "lncr";

	vFixedSeeds.clear();
        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
        //vFixedSeeds.clear(); // no seeds yet ----> now yes
        //vSeeds.clear();

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
               {  0, uint256S("0x35f373b6fed4342ef20327917f756dc5fe76b4f2c111ac0a1c52ba408764709b")},
               {  1305391, uint256S("0x99bd061f8c1b4a5b18ab76c93b353007d9026822df35c7201b08dde8cc42e5d3")},
               {  3657000, uint256S("0x966f34fec23d771d6a0793396650e64fb1f991c06fbfa046c5928d50884f409b")},
               {  5362791, uint256S("0x9e881790e1bb01f8f43fc8f3c49f1ca2f257ddf9f7042800aaa512e475727528")},
            }
        };

        chainTxData = ChainTxData{
            // Data at genesis block.
            1751354596, // * UNIX timestamp of last known number of transactions
            5626136,   // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.09991706136773344        // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 1050000;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 14600000;
        consensus.BIP34Hash = uint256S("000000042bcd56d6ea0509230b76fe850f0a40a9110f7dba979fd5d707e47c8a"); // Block hash at block 146
        consensus.BIP65Height = 14600000;
        consensus.BIP66Height = 14600000;
        consensus.powLimit = uint256S("00000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimit2 = uint256S("0007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3840; //
        consensus.nPowTargetSpacing = 60;
        consensus.nPowTargetSpacing2 = 25;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 192; // Require 75% of last 40 blocks to activate rulechanges
        consensus.nMinerConfirmationWindow = 256;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1683303184; // // active from the start
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1683303184 + 31536000; // // June 17, 2023

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1683303184; // active from the start
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1683303184 + 31536000; // Start + 1 year

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE; //1535587200; // August 30, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT; //1535587200 + 31536000; // Start + 1 year

        // LightningCashr: Hive: Deployment
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE].bit = 7;
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE; // active from the start
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // LightningCashr: Hive 1.1: Deployment
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_1].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_1].nStartTime = 1683303184;  // active from the start
        consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_1].nTimeout = 1683303184 + 31536000;  // active from the start

        // LightningCashr: Hive 1.2: Deployment
 //       consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_2].bit = 10;
 //       consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_2].nStartTime = 1565998948;  // active from the start
 //       consensus.vDeployments[Consensus::DEPLOYMENT_HIVE_1_2].nTimeout = 1565998948 + 31536000;  // active from the start

        // LightningCashr fields
        consensus.powForkTime = 1694548330;                 // Time of PoW hash method change (block 100)
        consensus.lastScryptBlock = 0;                    // Height of last scrypt block
        consensus.powLimitSHA = uint256S("00000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");   // Initial hash target at fork
        consensus.slowStartBlocks = 125;                     // Scale post-fork block reward up over this many blocks
 //       consensus.premineAmount = 2100000;                   // Premine amount (less than 1% of issued currency at fork time)
 //       std::vector<unsigned char> vch = ParseHex("76a91424af51d38b740a6dc2868dfd70fc16d76901e1e088ac"); // havent changed it yet but whatever...
 //       consensus.premineOutputScript = CScript(vch.begin(), vch.end());	// Output script for premine block (tAGaQ7rk3NE7etu3bU6yAJF3KSKeDhTHry)
        consensus.totalMoneySupplyHeight = 6215968;         // Height at which TMS is reached, do not issue rewards past this point (Note, not accurate value for testnet)

        // LightningCashr: Hive: Consensus Fields
        consensus.minBeeCost = 10000;                       // Minimum cost of a bee, used when no more block rewards
        consensus.beeCostFactor = 2500;                     // Bee cost is block_reward/beeCostFactor
        consensus.beeCreationAddress = "tEstNetCreateLCCWorkerBeeXXXYq6T3r";        // Unspendable address for bee creation
        consensus.hiveCommunityAddress = "t9ctP2rDfvnqUr9kmo2nb1LEDpu1Lc5sQn";      // Community fund address
        consensus.communityContribFactor = 10;              // Optionally, donate bct_value/maxCommunityContribFactor to community fund
        consensus.beeGestationBlocks = 48*24;               // The number of blocks for a new bee to mature 24 times faster for testnet
        consensus.beeLifespanBlocks = 48*24*14;             // The number of blocks a bee lives for after maturation 24 times faster for testnet
        consensus.powLimitHive = uint256S("0ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");  // Highest (easiest) bee hash target
        consensus.powLimitHive2 = uint256S("7ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");  // Highest (easiest) bee hash target
        consensus.minHiveCheckBlock = 1;                  // Don't bother checking below this height for Hive blocks (not used for consensus/validation checks, just efficiency when looking for potential BCTs)
        consensus.hiveTargetAdjustAggression = 30;          // Snap speed for bee hash target adjustment EMA
        consensus.hiveBlockSpacingTarget = 2;               // Target Hive block frequency (1 out of this many blocks should be Hivemined)
        consensus.hiveNonceMarker = 192;                    // Nonce marker for hivemined blocks

        // LightningCashr: Hive 1.1-related consensus fields
        consensus.minK = 1;                                 // Minimum chainwork scale for Hive blocks (see Hive whitepaper section 5)
        consensus.maxK = 8;                                 // Maximum chainwork scale for Hive blocks (see Hive whitepaper section 5)
        consensus.maxHiveDiff = 0.002;                      // Hive difficulty at which max chainwork bonus is awarded
        consensus.maxKPow = 5;                              // Maximum chainwork scale for PoW blocks
        consensus.powSplit1 = 0.001;                        // Below this Hive difficulty threshold, PoW block chainwork bonus is halved
        consensus.powSplit2 = 0.0005;                       // Below this Hive difficulty threshold, PoW block chainwork bonus is halved again
        consensus.maxConsecutiveHiveBlocks = 2;             // Maximum hive blocks that can occur consecutively before a PoW block is required
        consensus.hiveDifficultyWindow = 24;                // How many blocks the SMA averages over in hive difficulty adjust

	consensus.variableBeecost = true;
	consensus.variableForkBlock = 5000000; // never happens
	consensus.isTestnet = true;
	consensus.ratioForkBlock = 5000000; // never happens
        consensus.beeLifespanBlocks2 = 48*24*21;
        consensus.beeLifespanBlocks3 = 48*24*21;
	consensus.remvariableForkBlock = 5000000; // never happens

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");  // LightningCashr

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x35f373b6fed4342ef20327917f756dc5fe76b4f2c111ac0a1c52ba408764709b"); // LightningCashr: 0

	pchMessageStart[0] = 0x3a;
        pchMessageStart[1] = 0x8b;
        pchMessageStart[2] = 0x5c;
        pchMessageStart[3] = 0x2d;
        nDefaultPort = 59113;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1694548330, 1500284949, 0x1e0ffff0, 1, 1.25 * COIN * COIN_SCALE);		
	
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x35f373b6fed4342ef20327917f756dc5fe76b4f2c111ac0a1c52ba408764709b"));
        assert(genesis.hashMerkleRoot == uint256S("0x4caf91337a748b3d946f7f145bc180da5c9281d6323113c0535d77b808526e5a"));

        vFixedSeeds.clear();
        //vSeeds.emplace_back("testseeds.litecoinca.sh");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,127);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tlncr";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));
	//vFixedSeeds.clear(); // No seeds yet... ---->  and now yes !!!!
        //vSeeds.clear();	

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
            {
               {  0, uint256S("0x35f373b6fed4342ef20327917f756dc5fe76b4f2c111ac0a1c52ba408764709b")},
            }
        };

        chainTxData = ChainTxData{
            1694548330,
            0,
            0.0
        };
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // LightningCashr fields
        consensus.powForkTime = 1551819029;                 // Time of PoW hash method change (block 100)
        consensus.lastScryptBlock = 0;                    // Height of last scrypt block
        consensus.powLimitSHA = uint256S("000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");   // Initial hash target at fork
        consensus.slowStartBlocks = 2000;                     // Scale post-fork block reward up over this many blocks
 //       consensus.premineAmount = 2100000;                   // Premine amount (less than 1% of issued currency at fork time)
 //       std::vector<unsigned char> vch = ParseHex("76a91424af51d38b740a6dc2868dfd70fc16d76901e1e088ac");
 //       consensus.premineOutputScript = CScript(vch.begin(), vch.end());	// Output script for premine block (tAGaQ7rk3NE7etu3bU6yAJF3KSKeDhTHry)
        consensus.totalMoneySupplyHeight = 6215968;         // Height at which TMS is reached, do not issue rewards past this point (Note, not accurate value for testnet)
        consensus.hiveNonceMarker = 192;                    // Nonce marker for hivemined blocks
	consensus.remvariableForkBlock = 118956;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

	pchMessageStart[0] = 0xd9;
        pchMessageStart[1] = 0xf6;
        pchMessageStart[2] = 0xcc;
        pchMessageStart[3] = 0xea;
        nDefaultPort = 59444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1551819029, 1, 0x207fffff, 1, 50 * COIN * COIN_SCALE);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x238a0bf5a26d0bb55bf257a6b180a49f1422c2270857bf53cbc49f8316eb88fd"));
        assert(genesis.hashMerkleRoot == uint256S("0xfe90e5f71db801cab3064947169305a13c8107e645f9387e211fd73f266a581a"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("238a0bf5a26d0bb55bf257a6b180a49f1422c2270857bf53cbc49f8316eb88fd")}
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rlncr";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
