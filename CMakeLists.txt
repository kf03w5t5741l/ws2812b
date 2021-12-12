cmake_minimum_required(VERSION 3.10)

project(
	ws2812b
    VERSION 0.1
    LANGUAGES C
)

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

add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Werror
)

# add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES avr)
    # Note: AVR custom target settings were derived from 
    # https://www.kuon.ch/post/2018-07-11-avr-cmake/
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)

    add_custom_target(strip ALL avr-strip ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.elf DEPENDS ${PROJECT_NAME})
    add_custom_target(hex ALL avr-objcopy -R .eeprom -O ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex DEPENDS strip)
    add_custom_target(eeprom avr-objcopy -j .eeprom  --set-section-flags=.eeprom="alloc,load"  --change-section-lma .eeprom=0 -O ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.eep DEPENDS strip)

    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex;${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.eeprom;${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.lst")
endif()