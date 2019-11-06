#pragma once

#include <cstdint>

#include <array>

static constexpr auto kMaxSamplePerPkt = 600;
struct ChannelDataPkt {
  // uint32_t channelID;  // 0-3

  uint16_t adcData[kMaxSamplePerPkt + 2]   = {0};
  uint32_t timestamp[kMaxSamplePerPkt + 2] = {0};
};