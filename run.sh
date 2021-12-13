cd build/
cmake -DCMAKE_TOOLCHAIN_FILE=../avr.cmake .. && cmake --build . && avrdude -V -c arduino -p ATmega328p -P /dev/tty.usbmodem14201 -U flash:w:bin/ws2812b.hex
