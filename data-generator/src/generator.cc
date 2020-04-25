#include <bitcoin/bitcoin.hpp>
#include <iostream>
#include <string>

#include "generator.h"

namespace bcgen {

TransactionGenerator::TransactionGenerator(uint64_t seed) {
    m_random.seed(seed);
}

bc::chain::output
TransactionGenerator::create_p2wpkh_output(uint64_t amount,
                                           const bc::ec_compressed& address) {
    // P2WPKH output.
    // 0 [20-byte hash160(public key)]
    bc::machine::operation::list p2wpkh_operations;
    p2wpkh_operations.push_back(
        bc::machine::operation(bc::machine::opcode::push_size_0));
    p2wpkh_operations.push_back(
        bc::machine::operation(bc::to_chunk(bc::bitcoin_short_hash(address))));
    return bc::chain::output(amount, p2wpkh_operations);
}

std::vector<bc::chain::output>
TransactionGenerator::create_p2wpkh_outputs(const std::string& total_amount,
                                            size_t count) {
    uint64_t output_amount;
    bc::decode_base10(output_amount, total_amount, bc::btc_decimal_places);
    std::vector<bc::chain::output> outputs;
    uint64_t amount = output_amount / count;
    for (size_t i = 0; i < count; i++) {
        auto keypair = m_random.random_keypair();
        outputs.push_back(create_p2wpkh_output(amount, keypair.public_key));
    }
    return outputs;
}

bc::chain::input
TransactionGenerator::create_p2wpkh_input(const bc::hash_digest& prev_tx_hash,
                                          uint32_t index) {
    bc::chain::output_point uxto_to_spend(prev_tx_hash, index);
    bc::chain::input p2wpkh_input;
    p2wpkh_input.set_previous_output(uxto_to_spend);
    p2wpkh_input.set_sequence(bc::max_input_sequence);
    return p2wpkh_input;
}

void TransactionGenerator::add_p2wpkh_input_witness(bc::chain::transaction& tx,
                                                    uint32_t input_index,
                                                    const KeyPair& keypair,
                                                    uint64_t prev_amount) {
    // Script code.
    bc::chain::script p2wpkh_script_code =
        bc::chain::script::to_pay_key_hash_pattern(
            bc::bitcoin_short_hash(keypair.public_key));

    bc::endorsement signature;
    bc::chain::script::create_endorsement(
        signature, keypair.private_key, p2wpkh_script_code, tx, input_index,
        bc::machine::sighash_algorithm::all, bc::machine::script_version::zero,
        prev_amount);

    bc::data_stack witness_stack;
    witness_stack.push_back(signature);
    witness_stack.push_back(bc::to_chunk(keypair.public_key));
    bc::chain::witness p2wpkh_witness(witness_stack);
    tx.inputs()[input_index].set_witness(p2wpkh_witness);
}

void TransactionGenerator::generate_p2wpkh_inputs(
    const std::string& total_amount, bc::chain::transaction& tx, size_t count) {
    std::vector<bc::chain::input> inputs;
    uint64_t prev_amount;
    bc::decode_base10(prev_amount, total_amount, bc::btc_decimal_places);
    auto amount = prev_amount / count;
    for (size_t i = 0; i < count; i++) {
        auto keypair = m_random.random_keypair();
        auto prev_tx = m_random.random_hash_digest();
        auto prev_index = m_random.random_int<uint8_t>(0, 100);
        auto input = create_p2wpkh_input(prev_tx, prev_index);
        tx.inputs().push_back(input);
        add_p2wpkh_input_witness(tx, i, keypair, amount);
    }
}

// std::string TransactionGenerator::create_raw_p2wpkh(
//     const std::string& input_amount, const std::string& output_amount,
//     size_t input_count, size_t output_count) {
std::string
TransactionGenerator::create_raw_p2wpkh(const P2WPKHOptions& options) {

    //**************************************************************
    // SEND FROM P2WPKH.
    //**************************************************************

    // Output script.
    //    According to destination address.
    // Input script.
    //    []
    // Script code.
    //    P2PKH(public key hash).
    // Witness.
    //    [signature] [public key]

    bc::chain::transaction tx;
    tx.set_version(2u);
    tx.set_locktime(0);

    generate_p2wpkh_inputs(options.input_amount, tx, options.inputs_count);

    auto p2wpkh_outputs =
        create_p2wpkh_outputs(options.output_amount, options.outputs_count);
    tx.set_outputs(p2wpkh_outputs);

    return bc::encode_base16(tx.to_data(true, options.include_witness));
}

} // namespace bcgen
