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

int main() {
  const std::string devFileName1{"/dev/ttyACM1"};
  const std::string devFileName2{"/dev/ttyACM2"};

  int fd = open(devFileName1.c_str(), O_RDWR | O_NOCTTY);
  if (fd < 0) { fd = open(devFileName2.c_str(), O_RDWR | O_NOCTTY); }
  struct termios options;
  tcgetattr(fd, &options);
  options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
  options.c_oflag &= ~(ONLCR | OCRNL);
  options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  tcsetattr(fd, TCSANOW, &options);

  ChannelDataPkt channel1Data;
  for (;;) {
    int  byteCnt = 0;
    auto readCnt = read(fd, reinterpret_cast<void*>(&channel1Data), sizeof(ChannelDataPkt));
    if (readCnt == -1) { throw std::runtime_error{"read error"}; }

    std::cout << "First member: " << channel1Data.samples[0].adcData << ", Read Cnt: " << readCnt
              << std::endl;
  }

  close(fd);

  return 0;
}