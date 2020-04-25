pragma solidity ^0.6.0;

// This contract requires the txexists instruction
// https://github.com/danhper/go-ethereum/commit/edcdedb0e75af265c51cd44dc500b4971b0f16ef
// https://github.com/danhper/solidity/commit/6853078013a40a72e3a096995f539aaa830d44fb

contract TxChecker {
  function idsExist(bytes32[] memory txids) public returns (bool) {
    for(uint i = 0; i < txids.length; i++) {
      bool exists;
      bytes32 txid = txids[i];
      assembly { exists := txexists(txid) }
      if (!exists) {
        return false;
      }
    }
    return true;
  }
}
