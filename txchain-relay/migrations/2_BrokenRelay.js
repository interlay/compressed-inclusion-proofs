var BrokenRelay = artifacts.require("./BrokenRelay.sol");
var Utils = artifacts.require("./Utils.sol")

module.exports = function (deployer, network) {
    if (network == "development") {
        deployer.deploy(Utils);
        deployer.link(Utils, BrokenRelay);
        deployer.deploy(BrokenRelay);
    } else if (network == "ropsten") {
        deployer.deploy(BrokenRelay);
    } else if (network == "main") {
        deployer.deploy(BrokenRelay);
    }
};
