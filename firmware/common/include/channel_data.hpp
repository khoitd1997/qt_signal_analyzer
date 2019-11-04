#pragma once

#include <cstdint>

#include <array>

struct DataSample {
  uint32_t timestamp;
  uint16_t adcData;
};

static constexpr auto kMaxSamplePerPkt = 280;
struct ChannelDataPkt {
  uint32_t   channelID;  // 0-3
  DataSample samples[kMaxSamplePerPkt];
};
