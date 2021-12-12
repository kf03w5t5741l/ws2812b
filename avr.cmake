# Note: AVR toolchain settings were derived from 
# https://www.kuon.ch/post/2018-07-11-avr-cmake/

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

set(MCU atmega328p)
set(PROG_TYPE arduino)

set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

add_compile_options(
    -mmcu=${MCU}
    -Os
)

set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})
