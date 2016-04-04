#pragma once

#include <functional>
#include <limits>

#include <glog/logging.h>
#include <ze/common/types.h>
#include <ze/common/time_conversions.h>
#include <ze/common/timer.h>

namespace ze {

template <typename Lambda>
uint64_t runTimingBenchmark(
    const Lambda& benchmark_fun, uint32_t num_iter_per_epoch, uint32_t num_epochs,
    const std::string& benchmark_name = "", bool print_results = false)
{
  // Define lambda that runs experiment num_iter_per_epoch times and measures.
  auto executeFunMultipleTimes = [=]() -> int64_t
  {
    Timer t;
    // Measurement starts.
    for (uint32_t i = 0; i < num_iter_per_epoch; ++i)
    {
      benchmark_fun();
    }
    // Measurement ends.
    return t.stopAndGetNanoseconds();
  };

  uint64_t min_time = std::numeric_limits<uint64_t>::max();
  for (uint32_t i = 0; i < num_epochs; ++i)
  {
    // Call function.
    uint64_t timing = executeFunMultipleTimes();

    // According to Andrei Alexandrescu, the best measure is to take the minimum.
    // See talk: https://www.youtube.com/watch?v=vrfYLlR8X8k
    min_time = std::min(timing, min_time);
  }

  if(print_results)
  {
    VLOG(1) << "Benchmark: " << benchmark_name << "\n"
            << "> Time for " << num_iter_per_epoch << " iterations: "
            << nanosecToMillisecTrunc(min_time) << " milliseconds\n"
            << "> Time for 1 iteration: "
            << nanosecToMillisecTrunc(min_time) / num_iter_per_epoch << " milliseconds";
  }

  return min_time;
}


} // namespace ze
