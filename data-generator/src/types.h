#pragma once

#include <bitcoin/bitcoin.hpp>

namespace bcgen {
struct KeyPair {
    bc::wallet::ec_private private_key;
    bc::ec_compressed public_key;
};
} // namespace bcgen
