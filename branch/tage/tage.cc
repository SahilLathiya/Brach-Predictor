#include "tage.hpp"

#include <map>

#include <ooo_cpu.h>

// TAGE defines start here.
constexpr std::size_t NUM_BANKS = 4;
constexpr std::size_t BIMODAL_SIZE = 8198;
constexpr std::size_t LEN_BIMODAL = 2;
constexpr std::size_t LEN_GLOBAL = 14;
constexpr std::size_t LEN_TAG = 10;
constexpr std::size_t LEN_COUNTS = 3;
constexpr std::size_t MIN_HISTORY_LEN = 4;
constexpr std::size_t MAX_HISTORY_LEN = 244;
// TAGE defines end here.

using MyTage = Tage<NUM_BANKS, BIMODAL_SIZE, LEN_BIMODAL, LEN_GLOBAL, LEN_TAG, LEN_COUNTS, MIN_HISTORY_LEN, MAX_HISTORY_LEN>;

namespace
{
std::map<O3_CPU*, MyTage> tage;
}

void O3_CPU::initialize_branch_predictor() { tage[this].tage_init(); }

uint8_t O3_CPU::predict_branch(uint64_t ip) { return tage[this].tage_predict(ip); }

void O3_CPU::last_branch_result(uint64_t ip, uint64_t branch_target, uint8_t taken, uint8_t branch_type) { tage[this].tage_train(ip, taken); }
