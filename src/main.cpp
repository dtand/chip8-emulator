#include <iostream>
#include "emulator.h"

Emulator emulator;

int main(int argc, char* argv[]) {
    emulator.setup(argc, argv);
    emulator.run();
    return 0;
}