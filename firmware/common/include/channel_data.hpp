#pragma once

#include <cstdint>

#include <array>

struct DataSample {
  uint32_t tickCnt;
  uint16_t adcData;
};

static constexpr auto kSampleCnt = 15;
struct ChannelData {
  uint8_t channelID;  // 0-3

  uint8_t    sampleCnt;
  DataSample samples[kSampleCnt];
};
