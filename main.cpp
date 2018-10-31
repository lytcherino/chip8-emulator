#include <iostream>
#include <vector>

int main()
{
  Chip8 chip;

  // Setup render and register input callbacks
  // initGraphics();
  // initInput();

  // Initialise Chip8 emulator and load a game
  chip.init();
  chip.load("");

  for (;;) {
    // One emulation cycle
    chip.emulateCycle();

    // Update screen is draw flag is set
    if (chip.graphicsUpdated()) {
      draw();
    }

    // Store key press states (Press & Release)
    chip.setKeys();
  }

}
