const TxChecker = artifacts.require('./TxChecker.sol')

module.exports = function (deployer) {
  deployer.deploy(TxChecker)
}
