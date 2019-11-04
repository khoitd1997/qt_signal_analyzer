#include "channel_data.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <cassert>
#include <cstring>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

// https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
std::string exec(const char* cmd) {
  std::array<char, 128>                    buffer;
  std::string                              result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) { throw std::runtime_error("popen() failed!"); }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) { result += buffer.data(); }
  return result;
}

class DeviceFile {
 private:
  int _fd = -1;

 public:
  DeviceFile() {
    auto deviceFilePath =
        "/dev/" + exec(
                      "ls -l /dev/serial/by-id | grep usb-khoitd1997_Signal_Analyzer | grep -o "
                      "'\\w*tty\\w*'");
    deviceFilePath.erase(remove_if(deviceFilePath.begin(), deviceFilePath.end(), isspace),
                         deviceFilePath.end());
    _fd = open(deviceFilePath.c_str(), O_RDWR | O_NOCTTY);
    if (_fd < 0) { throw std::runtime_error{"can't open device files"}; }

    struct termios options;
    tcgetattr(_fd, &options);
    options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
    options.c_oflag &= ~(ONLCR | OCRNL);
    options.c_lflag &= ~(ECHO | ECHONL | ISIG | IEXTEN | ICANON);
    tcsetattr(_fd, TCSANOW, &options);
  }
  ~DeviceFile() { close(_fd); }

  void get(ChannelDataPkt& data) {
    auto readCnt = 0;
    while (readCnt != sizeof(ChannelDataPkt)) {
      const auto ret =
          read(_fd, reinterpret_cast<uint8_t*>(&data) + readCnt, sizeof(ChannelDataPkt) - readCnt);
      if (ret == -1) { throw std::runtime_error{"read error"}; }
      readCnt += ret;
      if (readCnt > sizeof(ChannelDataPkt)) { throw std::runtime_error{"misaligned!"}; }
    }
  }
};

int main() {
  {
    DeviceFile     devFile;
    ChannelDataPkt channelData;

    devFile.get(channelData);
  }

  {
    DeviceFile     devFile;
    ChannelDataPkt channelData;
    for (;;) {
      devFile.get(channelData);
      std::cout << ", Time: " << channelData.timestamp[0] << ", value: " << channelData.adcData[0]
                << std::endl;
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