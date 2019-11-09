#pragma once

#include <string>

#include "channel_data.h"

class SignalAnalyzerDevice {
 private:
  int _fd = -1;

  std::string execCmd(const char* cmd);

 public:
  SignalAnalyzerDevice(const std::string& devicePath);
  ~SignalAnalyzerDevice();

  void getData(ChannelDataPkt& data);
};