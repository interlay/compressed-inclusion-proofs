# TxChain: compressed-inclusion-proofs


**Note: migrated to [github.com/interlay/compressed-inclusion-proofs](https://github.com/interlay/compressed-inclusion-proofs). This repository is maintained as a mirror for completeness** 

Repository with the source code for

TxChain: Efficient Cryptocurrency Light Clients viaContingent Transaction Aggregation

This repository contains

* `txchain-relay` - a Bitcoin SPV client Ethereum, implementing compressed inclusion proofs as defined in TxChain
* `data-generator` - a Bitcoin data generator

The `txchain-relay` also contains [a contract](./txchain-relay/contracts/TxChecker.sol) leveraging our proposed `TXEXISTS` instruction to efficiently verify the inclusion of previous transactions in Ethereum.

Ethereum's `TXEXISTS` PoC implementation can be found below

* geth client: https://github.com/danhper/go-ethereum/tree/tx-chain
* Solidity compiler: https://github.com/danhper/solidity/tree/add-tx-exists
