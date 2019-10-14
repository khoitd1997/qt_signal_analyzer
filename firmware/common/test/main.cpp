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

int main() {
  auto deviceFilePath =
      "/dev/" + exec(
                    "ls -l /dev/serial/by-id | grep usb-khoitd1997_Signal_Analyzer | grep -o "
                    "'\\w*tty\\w*'");
  deviceFilePath.erase(remove_if(deviceFilePath.begin(), deviceFilePath.end(), isspace),
                       deviceFilePath.end());
  int fd = open(deviceFilePath.c_str(), O_RDWR | O_NOCTTY);
  if (fd < 0) { throw std::runtime_error{"can't open device files"}; }

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