mkdir -p build
cd build/
cmake -DCMAKE_TOOLCHAIN_FILE=../avr.cmake .. && cmake --build .
