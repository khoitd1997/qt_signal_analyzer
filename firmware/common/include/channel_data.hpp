#pragma once

#include <cstdint>

#include <array>

struct DataSample {
  uint32_t tickCnt;
  uint16_t adcData;
};

static constexpr auto kSamplePerPkt = 15;
struct ChannelDataPkt {
  uint8_t channelID;  // 0-3

  DataSample samples[kSamplePerPkt];
};
