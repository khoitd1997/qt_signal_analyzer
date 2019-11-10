#include "channel_data.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <cassert>
#include <cstring>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

int main() {
  {
    SignalSourceDetector devFile;
    ChannelDataPkt       channelData;

    devFile.get(channelData);
  }

  auto   startTime  = std::chrono::steady_clock::now();
  double totalBytes = 0;
  {
    SignalSourceDetector devFile;
    ChannelDataPkt       channelData;
    for (;;) {
      devFile.get(channelData);

      // std::cout << ", Time: " << channelData.timestamp[0] << ", value: " <<
      // channelData.adcData[0] << std::endl;

      totalBytes += sizeof(ChannelDataPkt);
      auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::steady_clock::now() - startTime)
                         .count();
      std::cout << "Time: " << channelData.timestamp[kMaxSamplePerPkt - 1]
                << ", data: " << channelData.adcData0[0] << ", "
                << channelData.adcData1[kMaxSamplePerPkt - 1] << ", "
                << channelData.adcData23[kMaxSamplePerPkt - 1] << ", Elapsed: " << elapsed
                << ", Speed: " << totalBytes * 1000000 / elapsed << std::endl;

      // for (auto i = 0; i < kMaxSamplePerPkt; ++i) {
      //   auto s = channelData.samples[i];
      //   if (s.adcData != 0xefab || s.timestamp != 0x0123abcd) {
      //     std::cout << "don't match " + std::to_string(i) + " " + std::to_string(s.adcData) + " "
      //     +
      //                      std::to_string(s.timestamp)
      //               << std::endl;
      //   }
      // }
    }
  }

  return 0;
}