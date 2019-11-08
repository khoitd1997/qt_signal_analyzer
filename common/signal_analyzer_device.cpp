#include "signal_analyzer_device.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <memory>

// https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
std::string SignalAnalyzerDevice::execCmd(const char* cmd) {
  std::array<char, 128>                    buffer;
  std::string                              result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) { throw std::runtime_error("popen() failed!"); }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) { result += buffer.data(); }
  return result;
}

SignalAnalyzerDevice::SignalAnalyzerDevice() {
  auto deviceFilePath =
      "/dev/" + execCmd(
                    "ls -l /dev/serial/by-id | grep usb-khoitd1997_Signal_Analyzer | grep -o "
                    "'\\w*tty\\w*'");
  deviceFilePath.erase(remove_if(deviceFilePath.begin(), deviceFilePath.end(), isspace),
                       deviceFilePath.end());
  _fd = open(deviceFilePath.c_str(), O_RDWR | O_NOCTTY);
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
