#pragma once

#include <bitcoin/bitcoin.hpp>
#include <tuple>

#include "random.h"
#include "types.h"

namespace bcgen {

struct P2WPKHOptions {
    std::string input_amount = "0.995";
    std::string output_amount = "0.993";
    size_t outputs_count = 1;
    size_t inputs_count = 1;
    bool include_witness = true;
    uint64_t seed = 0;
};


class TransactionGenerator {
public:
    explicit TransactionGenerator(uint64_t seed);
    bc::chain::output create_p2wpkh_output(uint64_t amount,
                                           const bc::ec_compressed& address);

    std::vector<bc::chain::output>
    create_p2wpkh_outputs(const std::string& total_amount, size_t count);

    bc::chain::input create_p2wpkh_input(const bc::hash_digest& prev_tx_hash,
                                         uint32_t index);

    void generate_p2wpkh_inputs(const std::string& total_amount,
                                bc::chain::transaction& tx, size_t count);

    void add_p2wpkh_input_witness(bc::chain::transaction& tx,
                                  uint32_t input_index, const KeyPair& keypair,
                                  uint64_t prev_amount);

    std::string create_raw_p2wpkh(const P2WPKHOptions& options);

private:
    RandomGenerator m_random;
};

} // namespace bcgen
