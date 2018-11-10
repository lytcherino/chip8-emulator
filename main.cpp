#include <iostream>

#include "Chip8.h"
#include <thread>
#include <chrono>

int main()
{
  Chip8 chip;

  // Initialise Chip8 emulator and load a game
  if (!chip.load("../Pong.ch8")) {
    return 1;
  }

  while (true) {
  chip.emulateCycle();
  chip.handleEvents();
  chip.updateDisplay();

  std::this_thread::sleep_for(std::chrono::milliseconds{33});
  }

}
