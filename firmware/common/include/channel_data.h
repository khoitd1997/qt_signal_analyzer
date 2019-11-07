#pragma once

// both the hal generated code and c++ code need this
#define kMaxSamplePerPkt 300

#ifdef __cplusplus

#include <stdint.h>

struct ChannelDataPkt {
  // uint32_t channelID;  // 0-3

  uint16_t adcData0[kMaxSamplePerPkt + 2]      = {0};
  uint16_t adcData1[kMaxSamplePerPkt + 2]      = {0};
  uint16_t adcData23[kMaxSamplePerPkt * 2 + 2] = {0};
  uint32_t timestamp[kMaxSamplePerPkt + 2]     = {0};
};

#endif