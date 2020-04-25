#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace bcgen {

bc::chain::transaction tx_from_hex(const std::string& hex_transaction,
                                   bool witness = true);
std::vector<uint8_t> decode_base16(const std::string& value);
std::string strip_witness(const std::string& hex_transaction);
std::string compute_hash(const std::string& hex_transaction, bool witness_in,
                         bool witness_out);

} // namespace bcgen
