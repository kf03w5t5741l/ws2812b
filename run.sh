mkdir -p build
cd build/
cmake -DCMAKE_TOOLCHAIN_FILE=../avr.cmake .. && cmake --build . && avrdude -V -c arduino -p ATmega328p -P COM3 -U flash:w:bin/ws2812b.hex
