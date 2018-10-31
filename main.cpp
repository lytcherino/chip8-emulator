#include <iostream>
#include <vector>
#include <random>
 
#include "Chip8.h"
#include "SDL.h"

int main()
{
  Chip8 chip;

  // Setup render and register input callbacks
  // initGraphics();
  // initInput();

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_Init Error: " << SDL_GetError() << "\n";
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow("Chip8", 100,100, 640,480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  SDL_Renderer* renderer= SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    SDL_DestroyWindow(window);
    std::cerr << "SDL_CreateRendrer Error: " << SDL_GetError() << "\n";
    SDL_Quit();
    return 1;
  }

  // Initialise Chip8 emulator and load a game
  chip.init();
  chip.load("");

  for (;;) {
    // One emulation cycle
    chip.emulateCycle();

    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        return 0;
      }
    }

    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    SDL_RenderDrawPoint(renderer, rand() % 640, rand() % 480);
    SDL_RenderPresent(renderer);

    // Update screen is draw flag is set
    if (chip.graphicsUpdated()) {
      //draw();
    }

    // Store key press states (Press & Release)
    //chip.setKeys();
  }

}
