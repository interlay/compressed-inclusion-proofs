#include <CLI11.hpp>
#include <string>

#include "generator.h"
#include "utils.h"

void add_generate_tx_command(CLI::App& app, bcgen::P2WPKHOptions& options) {
    auto generate_tx = app.add_subcommand("generate-tx");
    generate_tx->add_option("-i,--input-amount", options.input_amount,
                            "Input amount of the transaction");
    generate_tx->add_option("-o,--output-amount", options.output_amount,
                            "Output amount of the transaction");
    generate_tx
        ->add_option("-m,--inputs-count", options.inputs_count,
                     "Number of inputs of the transaction")
        ->required();
    generate_tx->add_option("-n,--outputs-count", options.outputs_count,
                            "Number of outputs of the transaction");
    generate_tx->add_option("-s,--seed", options.seed, "Random seed");
    generate_tx->add_flag(
        "--witness,!--no-witness", options.include_witness,
        "Include or not the witness in the input transaction");

    generate_tx->callback([&options]() {
        bcgen::TransactionGenerator generator(options.seed);
        auto raw_transaction = generator.create_raw_p2wpkh(options);
        std::cout << raw_transaction;
    });
}

struct ComputeHashOptions {
    std::string hex_transaction;
    bool witness_in = true;
    bool witness_out = true;
};

void add_compute_hash_command(CLI::App& app, ComputeHashOptions& options) {
    auto command = app.add_subcommand("compute-hash");
    command
        ->add_option("hex", options.hex_transaction, "The transaction to parse")
        ->required();
    command->add_flag("--witness-in,!--no-witness-in", options.witness_in,
                      "Whether the input has witnesses");
    command->add_flag("--witness-out,!--no-witness-out", options.witness_out,
                      "Whether the output hash should have witnesses");
    command->callback([&options]() {
        auto hash = bcgen::compute_hash(
            options.hex_transaction, options.witness_in, options.witness_out);
        std::cout << hash << std::endl;
    });
}

void add_strip_witness_command(CLI::App& app, std::string& hex_transaction) {
    auto command = app.add_subcommand("strip-witness");
    command
        ->add_option("hex", hex_transaction, "The transaction from which to strip witness")
        ->required();
    command->callback([&hex_transaction]() {
        auto result = bcgen::strip_witness(hex_transaction);
        std::cout << result << std::endl;
    });
}


int main(int argc, char** argv) {
    CLI::App app{"Bitcoin transaction parser/generator"};
    app.require_subcommand(1);

    bcgen::P2WPKHOptions generate_tx_options;
    add_generate_tx_command(app, generate_tx_options);

    ComputeHashOptions compute_hash_options;
    add_compute_hash_command(app, compute_hash_options);

    std::string hex_transaction;
    add_strip_witness_command(app, hex_transaction);

    CLI11_PARSE(app, argc, argv);

    return 0;
}
