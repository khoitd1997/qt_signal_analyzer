#include "signalanalyzerdevice.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <memory>

SignalAnalyzerDevice::SignalAnalyzerDevice(const std::string& devicePath) {
  _fd = open(devicePath.c_str(), O_RDWR | O_NOCTTY);
  if (_fd < 0) { throw std::runtime_error{"can't open device files"}; }

  struct termios options;
  tcgetattr(_fd, &options);
  options.c_iflag &= static_cast<unsigned int>(~(INLCR | IGNCR | ICRNL | IXON | IXOFF));
  options.c_oflag &= static_cast<unsigned int>(~(ONLCR | OCRNL));
  options.c_lflag &= static_cast<unsigned int>(~(ECHO | ECHONL | ISIG | IEXTEN | ICANON));
  tcsetattr(_fd, TCSANOW, &options);
}

SignalAnalyzerDevice::~SignalAnalyzerDevice() { close(_fd); }

void SignalAnalyzerDevice::getData(ChannelDataPkt& data) {
  size_t readCnt = 0;
  while (readCnt != sizeof(ChannelDataPkt)) {
    const auto ret =
        read(_fd, reinterpret_cast<uint8_t*>(&data) + readCnt, sizeof(ChannelDataPkt) - readCnt);
    if (ret == -1) { throw std::runtime_error{"read error"}; }
    readCnt += static_cast<size_t>(ret);
    if (readCnt > sizeof(ChannelDataPkt)) { throw std::runtime_error{"misaligned!"}; }
  }
}
