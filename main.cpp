#include <iostream>

#include "Chip8.h"
#include "SDL.h"
#include <thread>
#include <chrono>

int main()
{
  Chip8 chip;

  // Initialise Chip8 emulator and load a game
  if (!chip.load("../Tank.ch8")) {
    return 1;
  }

  for (int i = 0; i < 250; ++i) {
    chip.emulateCycle();
    chip.handleEvents();
    chip.updateDisplay();

    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }

}
