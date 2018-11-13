#include <iostream>

#include "Chip8.h"
#include <thread>
#include <chrono>

#define FRAME_RATE 60

int main(int argc, char** argv)
{

  // Initialise Chip8 emulator and load a game
  Chip8 chip;
  /*
  if (!chip.load(argv[1])) {
    return 1;
  }
  */

  if (!chip.load("../BC_test.ch8")) {
    return 1;
  }

  while (true) {

    chip.emulateCycle();
    chip.handleEvents();
    chip.updateDisplay();

    std::this_thread::sleep_for(std::chrono::milliseconds{100});
  }


}
