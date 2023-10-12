#pragma once

#include <map>

#include "msl/fwcounter.h"
#include "ooo_cpu.h"

template <std::size_t BIMODAL_TABLE_SIZE, std::size_t BIMODAL_PRIME, std::size_t COUNTER_BITS>
struct Bimodal {

  std::array<champsim::msl::fwcounter<COUNTER_BITS>, BIMODAL_TABLE_SIZE> bimodal_table;

  uint8_t predict(uint64_t ip)
  {
    auto hash = ip % BIMODAL_PRIME;
    auto value = bimodal_table[hash];

    return value.value() >= (value.maximum / 2);
  }

  void update(uint64_t ip, uint8_t taken)
  {
    auto hash = ip % BIMODAL_PRIME;
    bimodal_table[hash] += taken ? 1 : -1;
  }
};
