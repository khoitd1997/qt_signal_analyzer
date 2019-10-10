output_cmake_toolchain_file := toolchain.cmake
output_cmakelist_file 			:= cube_generated/CMakeLists.txt

default: generate_cmake_toolchain generate_cube_mx_cmakelist;

include cube_generated/Makefile

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := ${CURDIR}/cube_generated

final_c_flag = $(CFLAGS)
final_c_flag_filtered_1 = $(filter-out $(C_INCLUDES),$(final_c_flag))
final_c_flag_filtered_final = $(filter-out -MF"$(@:%.o=%.d)",$(final_c_flag_filtered_1))

pattern = -Wl,-Map=build/cube_generated.map,--cref
raw_final_ld_flag := $(LDFLAGS)
final_ld_flag_filtered := $(filter-out $(pattern),$(raw_final_ld_flag))
final_ld_flag_subbed := $(subst -TSTM32F407VGTx_FLASH.ld,-T $(current_dir)/STM32F407VGTx_FLASH.ld,$(final_ld_flag_filtered))

generate_cmake_toolchain:
	@echo "" > $(output_cmake_toolchain_file)
	@echo "set(CMAKE_SYSTEM_NAME Generic)" >> $(output_cmake_toolchain_file)
	@echo "set(CMAKE_SYSTEM_PROCESSOR arm)" >> $(output_cmake_toolchain_file)

	@echo "set(CMAKE_TRY_COMPILE_TARGET_TYPE \"STATIC_LIBRARY\")" >> $(output_cmake_toolchain_file)
	@echo "set(BUILD_SHARED_LIBS OFF)" >> $(output_cmake_toolchain_file)

	@echo "find_program(CMAKE_C_COMPILER arm-none-eabi-gcc)" >> $(output_cmake_toolchain_file)
	@echo "find_program(CMAKE_CXX_COMPILER arm-none-eabi-g++)" >> $(output_cmake_toolchain_file)

	@echo "set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)" >> $(output_cmake_toolchain_file)
	@echo "set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)" >> $(output_cmake_toolchain_file)
	@echo "set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)" >> $(output_cmake_toolchain_file)
	@echo "set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)" >> $(output_cmake_toolchain_file)

	@echo "set(CMAKE_CXX_FLAGS \"$(final_c_flag_filtered_final)\")" >> $(output_cmake_toolchain_file)
	@echo "set(CMAKE_C_FLAGS \" -std=gnu11 $(final_c_flag_filtered_final)\")" >> $(output_cmake_toolchain_file)
	@echo "set(CMAKE_ASM_FLAGS \" -x assembler-with-cpp $(ASFLAGS)\")" >> $(output_cmake_toolchain_file)

	@echo "set(CMAKE_EXE_LINKER_FLAGS \"$(final_ld_flag_subbed)\")" >> $(output_cmake_toolchain_file)

generate_cube_mx_cmakelist:
	@echo "" > $(output_cmakelist_file)
	@echo "set(STM32_HAL_SRCS $(ASM_SOURCES) $(filter-out Src/main.c,$(C_SOURCES)) CACHE INTERNAL \"stm32 source_list\")" >> $(output_cmakelist_file)
	@echo "" >> $(output_cmakelist_file)
	@echo "set(STM32_HAL_INCS Src $(subst -I,,$(C_INCLUDES)) CACHE INTERNAL \"stm32 inc list\")" >> $(output_cmakelist_file)