file build/signal_analyzer_firmware.elf
# file cube_generated/build/cube_generated.elf
# file cube_generated/cmake_build/signal_analyzer_firmware.elf
target remote localhost:2331
monitor halt
monitor reset
load
monitor go
q