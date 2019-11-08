#!/bin/bash
set -e
curr_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# generate cmake files from cubemx makefile
cd ${curr_dir} 
make -f cube_wrapper.mk 

mkdir -p build 
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake && cmake --build .

# flash the firmware
JLinkGDBServer -device STM32F407VG -if SWD -speed 16000& # launch gdb server
cd ${curr_dir}
gdb -x flash.gdbinit -batch