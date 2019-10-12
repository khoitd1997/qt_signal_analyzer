# Signal Analyzer Firmware

STM32F407VG firmware for the signal analyzer

## How to build

To build, you need ```stm32cubemx```, use ```cube_generated/cube_generated.ioc``` as the base project and adjust the output path to be ```cube_generated```

NOTE: Delete the makefile inside ```cube_generated``` before generating code

The following settings should be enabled on cubemx before generating the code:

- Do not generate main function

After code generation, run ```build_and_flash.sh``` to build firmware and flash it
