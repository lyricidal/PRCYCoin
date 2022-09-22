// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Copyright (c) 2020-2022 The PRivaCY Coin Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAINPARAMS_H
#define BITCOIN_CHAINPARAMS_H

#include "chainparamsbase.h"
#include "checkpoints.h"
#include "primitives/block.h"
#include "protocol.h"
#include "uint256.h"

#include <vector>

typedef unsigned char MessageStartChars[MESSAGE_START_SIZE];

struct CDNSSeedData {
    std::string name, host;
    bool supportsServiceBitsFiltering;
    CDNSSeedData(const std::string& strName, const std::string& strHost, bool supportsServiceBitsFilteringIn = false) : name(strName), host(strHost), supportsServiceBitsFiltering(supportsServiceBitsFilteringIn) {}
};

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

/**
 * CChainParams defines various tweakable parameters of a given instance of the
 * PRCY system. There are three: the main network on which people trade goods
 * and services, the public test network which gets reset from time to time and
 * a regression test mode which is intended for private networks only. It has
 * minimal difficulty to ensure that blocks can be found instantly.
 */
class CChainParams
{
public:
    enum Base58Type {
        PUBKEY_ADDRESS,
        SCRIPT_ADDRESS,
        SECRET_KEY,     // BIP16
        EXT_PUBLIC_KEY, // BIP32
        EXT_SECRET_KEY, // BIP32

        MAX_BASE58_TYPES
    };

    const uint256& HashGenesisBlock() const { return hashGenesisBlock; }
    const MessageStartChars& MessageStart() const { return pchMessageStart; }
    int GetDefaultPort() const { return nDefaultPort; }
    const uint256& ProofOfWorkLimit() const { return bnProofOfWorkLimit; }
    const uint256& ProofOfStakeLimit(const bool fV2) const { return fV2 ? bnProofOfStakeLimit_V2 : bnProofOfStakeLimit; }
    int SubsidyHalvingInterval() const { return nSubsidyHalvingInterval; }
    /** Used to check majorities for block version upgrade */
    int EnforceBlockUpgradeMajority() const { return nEnforceBlockUpgradeMajority; }
    int RejectBlockOutdatedMajority() const { return nRejectBlockOutdatedMajority; }
    int ToCheckBlockUpgradeMajority() const { return nToCheckBlockUpgradeMajority; }
    int MaxReorganizationDepth() const { return nMaxReorganizationDepth; }

    /** Used if GeneratePrcycoins is called with a negative number of threads */
    int DefaultMinerThreads() const { return nMinerThreads; }
    const CBlock& GenesisBlock() const { return genesis; }
    bool RequireRPCPassword() const { return fRequireRPCPassword; }
    /** Make miner wait to have peers to avoid wasting work */
    bool MiningRequiresPeers() const { return fMiningRequiresPeers; }
    /** Headers first syncing is disabled */
    bool HeadersFirstSyncingActive() const { return fHeadersFirstSyncingActive; };
    /** Default value for -checkmempool and -checkblockindex argument */
    bool DefaultConsistencyChecks() const { return fDefaultConsistencyChecks; }
    /** Allow mining of a min-difficulty block */
    bool AllowMinDifficultyBlocks() const { return fAllowMinDifficultyBlocks; }
    /** Skip proof-of-work check: allow mining of any difficulty block */
    bool SkipProofOfWorkCheck() const { return fSkipProofOfWorkCheck; }
    /** Make standard checks */
    bool RequireStandard() const { return fRequireStandard; }
    int64_t TargetTimespan() const { return nTargetTimespan; }
    int64_t TargetSpacing() const { return nTargetSpacing; }
    int64_t TargetTimespan(const bool fV2 = true) const { return fV2 ? nTargetTimespan_V2 : nTargetTimespan; }

    /** returns the coinbase maturity **/
    int COINBASE_MATURITY() const { return nMaturity; }

    /** returns the coinstake maturity (min depth required) **/
    int COINSTAKE_MIN_DEPTH() const { return nStakeMinDepth; }
    bool HasStakeMinAgeOrDepth(const int contextHeight, const uint32_t contextTime, const int utxoFromBlockHeight, const uint32_t utxoFromBlockTime) const;

    /** Time Protocol V2 **/
    int BlockStartTimeProtocolV2() const { return nBlockTimeProtocolV2; }
    bool IsTimeProtocolV2(const int nHeight) const { return nHeight >= BlockStartTimeProtocolV2(); }
    int TimeSlotLength() const { return nTimeSlotLength; }
    int FutureBlockTimeDrift(const int nHeight) const;
    bool IsValidBlockTimeStamp(const int64_t nTime, const int nHeight) const;

    CAmount MNCollateralAmt() const { return nMNCollateralAmt; }
    CAmount MinimumStakeAmount() const { return nMinimumStakeAmount; }
    /** The masternode count that we will allow the see-saw reward payments to be off by */
    int MasternodeCountDrift() const { return nMasternodeCountDrift; }
    /** Make miner stop after a block is found. In RPC, don't return until nGenProcLimit blocks are generated */
    bool MineBlocksOnDemand() const { return fMineBlocksOnDemand; }
    /** In the future use NetworkIDString() for RPC fields */
    bool TestnetToBeDeprecatedFieldRPC() const { return fTestnetToBeDeprecatedFieldRPC; }
    /** Return the BIP70 network string (main, test or regtest) */
    std::string NetworkIDString() const { return strNetworkID; }
    const std::vector<CDNSSeedData>& DNSSeeds() const { return vSeeds; }
    const std::vector<unsigned char>& Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
    const std::vector<SeedSpec6>& FixedSeeds() const { return vFixedSeeds; }
    virtual const Checkpoints::CCheckpointData& Checkpoints() const = 0;
    int PoolMaxTransactions() const { return nPoolMaxTransactions; }
    int64_t StartMasternodePayments() const { return nStartMasternodePayments; }
    int64_t Budget_Fee_Confirmations() const { return nBudget_Fee_Confirmations; }
    CBaseChainParams::Network NetworkID() const { return networkID; }
    bool IsRegTestNet() const { return NetworkID() == CBaseChainParams::REGTEST; }
    int ExtCoinType() const { return nExtCoinType; }
    int StealthPrefix() const { return nStealthPrefix; }
    int IntegratedPrefix() const { return nIntegratedPrefix; }

    /** Height or Time Based Activations **/
    int ModifierUpgradeBlock() const { return nModifierUpdateBlock; }
    int LAST_POW_BLOCK() const { return nLastPOWBlock; }
    int START_POA_BLOCK() const { return nStartPOABlock; }
    int SoftFork() const { return nSoftForkBlock;}
    int PoANewDiff() const { return nPoANewDiff;}
    int PoAFixTime() const { return nPoAFixTime;}
    int PoAPaddingBlock() const { return nPoAPaddingBlock;}
    int PoAPadding() const { return nPoAPadding;}
    int BIP65ActivationHeight() const { return nBIP65ActivationHeight; }
    int HardFork() const { return nHardForkBlock;}
    int HardForkRingSize() const { return nHardForkBlockRingSize;}
    int HardForkRingSize2() const { return nHardForkBlockRingSize2;}
    bool IsStakeModifierV2(const int nHeight) const { return nHeight >= nBlockStakeModifierlV2; }

    //For PoA block time
    int POA_BLOCK_TIME() const { return nPoABlockTime; }
    int MIN_NUM_POS_BLOCKS_AUDITED() const {return nMinNumPoSBlocks;}
    int MAX_NUM_POS_BLOCKS_AUDITED() const {return nMaxNumPoSBlocks;}
    int nLastPOWBlock;
    CAmount TOTAL_SUPPLY = 70000000 * COIN; //70M PRCY

protected:
    CChainParams() {}

    uint256 hashGenesisBlock;
    MessageStartChars pchMessageStart;
    //! Raw pub key bytes for the broadcast alert signing key.
    int nDefaultPort;
    int nExtCoinType;
    int nStealthPrefix;
    int nIntegratedPrefix;
    uint256 bnProofOfWorkLimit;
    uint256 bnProofOfStakeLimit;
    uint256 bnProofOfStakeLimit_V2;
    mutable int nMaxReorganizationDepth;
    int nSubsidyHalvingInterval;
    int nEnforceBlockUpgradeMajority;
    int nRejectBlockOutdatedMajority;
    int nToCheckBlockUpgradeMajority;
    int64_t nTargetSpacing;
    int64_t nTargetTimespan;
    int64_t nTargetTimespan_V2;
    int nStartPOABlock;
    int nSoftForkBlock;
    int nHardForkBlock;
    int nHardForkBlockRingSize;
    int nHardForkBlockRingSize2;
    int nPoANewDiff;
    int nPoAFixTime;
    int nPoAPaddingBlock;
    int nPoAPadding;
    int nMasternodeCountDrift;
    int nMaturity;
    int nStakeMinDepth;
    int nFutureTimeDriftPoW;
    int nFutureTimeDriftPoS;
    int nTimeSlotLength;

    int nModifierUpdateBlock;
    CAmount nMNCollateralAmt;
    CAmount nMinimumStakeAmount;
    int nMinerThreads;
    std::vector<CDNSSeedData> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
    CBaseChainParams::Network networkID;
    std::string strNetworkID;
    CBlock genesis;
    std::vector<SeedSpec6> vFixedSeeds;
    bool fRequireRPCPassword;
    bool fMiningRequiresPeers;
    bool fAllowMinDifficultyBlocks;
    bool fDefaultConsistencyChecks;
    bool fRequireStandard;
    bool fMineBlocksOnDemand;
    bool fSkipProofOfWorkCheck;
    bool fTestnetToBeDeprecatedFieldRPC;
    bool fHeadersFirstSyncingActive;
    int nPoolMaxTransactions;
    int64_t nStartMasternodePayments;
    int64_t nBudget_Fee_Confirmations;
    int nBIP65ActivationHeight;
    int nBlockTimeProtocolV2;
    int nBlockStakeModifierlV2;

    //For PoA blocks
    int nPoABlockTime;
    int nMinNumPoSBlocks;
    int nMaxNumPoSBlocks;
public:
    void ChangeMaxReorg(int num) const {
        nMaxReorganizationDepth = num;
    }
};

/**
 * Return the currently selected parameters. This won't change after app startup
 * outside of the unit tests.
 */
const CChainParams& Params();

/** Return parameters for the given network. */
CChainParams& Params(CBaseChainParams::Network network);


/** Sets the params returned by Params() to those for the given network. */
void SelectParams(CBaseChainParams::Network network);

/**
 * Looks for -regtest or -testnet and then calls SelectParams as appropriate.
 * Returns false if an invalid combination is given.
 */
bool SelectParamsFromCommandLine();

#endif // BITCOIN_CHAINPARAMS_H
