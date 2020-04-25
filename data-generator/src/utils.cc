#include <stdexcept>
#include <unordered_map>

#include <bitcoin/bitcoin.hpp>

#include "utils.h"

namespace bcgen {

uint8_t decode_base16(char value) {
    if (value >= '0' && value <= '9') {
        return value - '0';
    } else if (value >= 'a' && value <= 'f') {
        return value - 'a' + 10;
    } else if (value >= 'A' && value <= 'F') {
        return value - 'A' + 10;
    } else {
        throw std::runtime_error("invalid hex value");
    }
}

std::vector<uint8_t> decode_base16(const std::string& value) {
    if (value.size() % 2 != 0) {
        throw std::runtime_error("hex representation should have an even size");
    }
    std::vector<uint8_t> result;
    for (size_t i = 0; i < value.size(); i += 2) {
        uint8_t n = 16 * decode_base16(value[i]) + decode_base16(value[i + 1]);
        result.push_back(n);
    }

    return result;
}

bc::chain::transaction tx_from_hex(const std::string& hex_transaction,
                                   bool witness) {
    auto tx_bytes = decode_base16(hex_transaction);
    bc::chain::transaction tx;
    tx.from_data(tx_bytes, true, witness);
    return tx;
}

std::string compute_hash(const std::string& hex_transaction, bool witness_in,
                         bool witness_out) {
    auto tx = tx_from_hex(hex_transaction, witness_in);
    auto result = tx.hash(witness_out);
    std::reverse(result.begin(), result.end()); // reverse endianness
    return bc::encode_base16(result);
}

std::string strip_witness(const std::string& hex_transaction) {
    auto tx = tx_from_hex(hex_transaction, true);
    auto result = tx.to_data(true, false);
    return bc::encode_base16(result);
}

} // namespace bcgen
