# Note: AVR options were derived from 
# https://www.kuon.ch/post/2018-07-11-avr-cmake/

cmake_minimum_required(VERSION 3.10)

project(
	ws2812b
    VERSION 0.1
    LANGUAGES C
)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(include)
link_directories(lib)

# set input and output directories
set(SRC_DIR src)
set(SOURCES
    ${SRC_DIR}/${PROJECT_NAME}.c
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(MCU atmega328p)
set(PROG_TYPE arduino)

add_compile_options(
    -mmcu=${MCU}
    -Os
    -Wall
    -Wextra
    -pedantic
    -Werror
)

set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

# add the executable
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)

add_custom_target(strip ALL avr-strip ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.elf DEPENDS ${PROJECT_NAME})
add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex DEPENDS strip)
add_custom_target(eeprom avr-objcopy -j .eeprom  --set-section-flags=.eeprom="alloc,load"  --change-section-lma .eeprom=0 -O ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.eep DEPENDS strip)
add_custom_target(upload avrdude -F -V -c ${PROG_TYPE} -p ${MCU} -P COM3 -U flash:w:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex DEPENDS hex)
add_custom_target(upload_eeprom avrdude -F -V -c ${PROG_TYPE} -p ${MCU} -P COM3 -U eeprom:w:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.eep DEPENDS eeprom)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex;${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.eeprom;${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.lst")
