#pragma once

#include <climits>
#include <functional>
#include <random>

#include "types.h"

namespace bcgen {

using random_bytes_engine =
    std::independent_bits_engine<std::default_random_engine, CHAR_BIT, uint8_t>;

class RandomGenerator {
public:
    template <size_t Size>
    std::array<uint8_t, Size> generate_random_bytes();

    void seed(uint64_t seed);
    KeyPair random_keypair(uint16_t network = bc::wallet::ec_private::mainnet);
    bc::hash_digest random_hash_digest();

    template <typename IntType>
    IntType random_int(IntType min, IntType max);

private:
    random_bytes_engine m_rbe;
    std::default_random_engine m_dre;
};

template <size_t Size>
std::array<uint8_t, Size> RandomGenerator::generate_random_bytes() {
    std::array<uint8_t, Size> data;
    std::generate(data.begin(), data.end(), m_rbe);
    return data;
}

template <typename IntType>
IntType RandomGenerator::random_int(IntType min, IntType max) {
    std::uniform_int_distribution<IntType> distribution(min, max);
    return distribution(m_dre);
}

} // namespace bcgen
