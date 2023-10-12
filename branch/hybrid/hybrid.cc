#pragma once

#include <map>

#include "perceptron.hpp"
#include "tage.hpp"
#include "bimodal.hpp"
#include <ooo_cpu.h>

// TAGE defines start here.
constexpr std::size_t NUM_BANKS = 4;
constexpr std::size_t BIMODAL_SIZE = 8198;
constexpr std::size_t LEN_BIMODAL = 3;
constexpr std::size_t LEN_GLOBAL = 11;
constexpr std::size_t LEN_TAG = 32;
constexpr std::size_t LEN_COUNTS = 3;
constexpr std::size_t MIN_HISTORY_LEN = 7;
constexpr std::size_t MAX_HISTORY_LEN = 255;
// TAGE defines end here.

// Perceptron defines start here
constexpr std::size_t PERCEPTRON_HISTORY = 770; // history length for the global history shift register
constexpr std::size_t PERCEPTRON_BITS = 8;     // number of bits per weight
constexpr std::size_t NUM_PERCEPTRONS = 117;  // 163;

constexpr std::size_t NUM_UPDATE_ENTRIES = 1024; // size of buffer for keeping 'perceptron_state' for update
// Perceptron defines end here

// Bimodal metapredictor defines start here
constexpr std::size_t BIMODAL_TABLE_SIZE = 4096;
constexpr std::size_t BIMODAL_PRIME = 4093;
constexpr std::size_t COUNTER_BITS = 2;
// Bimodal metapredictor defines end here

using MyTage = Tage<NUM_BANKS, BIMODAL_SIZE, LEN_BIMODAL, LEN_GLOBAL, LEN_TAG, LEN_COUNTS, MIN_HISTORY_LEN, MAX_HISTORY_LEN>;
using MyPerceptron = PerceptronPred<PERCEPTRON_HISTORY, PERCEPTRON_BITS, NUM_PERCEPTRONS, NUM_UPDATE_ENTRIES>;
using MyMetaPredictor = Bimodal<BIMODAL_TABLE_SIZE, BIMODAL_PRIME, COUNTER_BITS>;

namespace
{
std::map<O3_CPU*, MyTage> tage;
std::map<O3_CPU*, MyPerceptron> perceptron_predictor;
std::map<O3_CPU*, MyMetaPredictor> metapredictor;
} // namespace

void O3_CPU::initialize_branch_predictor() { ::tage[this].tage_init(); }

uint8_t O3_CPU::predict_branch(uint64_t ip)
{
  auto tage_pred = ::tage[this].tage_predict(ip);
  auto perceptron_pred = ::perceptron_predictor[this].predict_branch(ip);

  if (::metapredictor[this].predict(ip)) {
      return tage_pred;
  } else {
      return perceptron_pred;
  }
}

void O3_CPU::last_branch_result(uint64_t ip, uint64_t branch_target, uint8_t taken, uint8_t branch_type)
{
  auto tage_pred = ::tage[this].tage_predict(ip);
  auto perceptron_pred = ::perceptron_predictor[this].predict_branch(ip);

  ::tage[this].tage_train(ip, taken);
  ::perceptron_predictor[this].last_branch_result(ip, branch_target, taken, branch_type);

  ::metapredictor[this].update(ip, tage_pred == taken);
}
