#include "channel_data.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <cassert>
#include <cstring>

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#include "libusb.h"

int main() {
  const std::string devFileName{"/dev/ttyACM1"};

  int fd = open(devFileName.c_str(), O_RDWR | O_NOCTTY);
  if (fd < 0) { throw std::runtime_error{"can't open files"}; }
  struct termios options;
  tcgetattr(fd, &options);
  options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
  options.c_oflag &= ~(ONLCR | OCRNL);
  options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  tcsetattr(fd, TCSANOW, &options);

  ChannelData channel1Data;
  std::cout << "Data size: " << sizeof(ChannelData) << std::endl;
  sleep(2);
  for (;;) {
    // sleep(1);
    int byteCnt = 0;
    if (ioctl(fd, FIONREAD, &byteCnt) == -1) { throw std::runtime_error{"ioctl error"}; }
    // if (byteCnt >= sizeof(ChannelData)) {
    const auto dataSize = sizeof(ChannelData);
    if (byteCnt >= dataSize) {
      auto readCnt = read(fd, reinterpret_cast<void*>(&channel1Data), dataSize);
      if (readCnt == -1) { throw std::runtime_error{"read error"}; }

      if (readCnt == dataSize) {
        std::cout << "Data is: " << (int)channel1Data.sampleCnt
                  << ", first member: " << channel1Data.samples[0].adcData << std::endl;
      }
      std::cout << "Read Cnt: " << readCnt << ", byte Cnt:" << byteCnt << std::endl;
      // for (auto i = 0; i < dataSize; ++i) { printf("%c", buf[i]); }
      // printf("\n");
    }
    sleep(1);
  }

  close(fd);

  return 0;
}