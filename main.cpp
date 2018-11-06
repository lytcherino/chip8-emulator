#include <iostream>

#include "Chip8.h"
#include "SDL.h"
#include <thread>
#include <chrono>

int main()
{
  Chip8 chip;

  // Initialise Chip8 emulator and load a game
  chip.init();
  chip.load("../Tank.ch8");

  for (int i = 0; i < 200; ++i) {
    chip.emulateCycle();
    chip.handleEvents();
    chip.updateDisplay();
    // Store key press states (Press & Release)
    //chip.setKeys();

    std::this_thread::sleep_for(std::chrono::milliseconds{10});
  }

}
