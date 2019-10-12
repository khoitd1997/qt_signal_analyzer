file build/signal_analyzer_firmware.elf
target remote localhost:2331
monitor halt
monitor reset
load
monitor go
q