#include "random.h"

namespace bcgen {
void RandomGenerator::seed(uint64_t seed) {
    m_rbe.seed(seed);
    m_dre.seed(seed);
}

KeyPair RandomGenerator::random_keypair(uint16_t network) {
    auto random_secret = random_hash_digest();
    const bc::wallet::ec_private private_key(random_secret, network, true);
    const bc::ec_compressed public_key = private_key.to_public().point();
    return { private_key, public_key };
}

bc::hash_digest RandomGenerator::random_hash_digest() {
    return generate_random_bytes<bc::hash_size>();
}
} // namespace bcgen
