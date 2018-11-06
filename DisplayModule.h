#ifndef DISPLAY_MODULE_H
#define DISPLAY_MODULE_H

#include "SDL.h"

#include <iostream>
#include <map>
#include <memory>

#include "EventHandler.h"

class DisplayModule {

public:

  DisplayModule(EventHandler& eventHandler);
  ~DisplayModule();

  void clearScreen();
  void updateScreen();
  void setDrawFlag();

  void registerWithHandler(EventHandler& eventHandler);

  unsigned char* getGfxArray();

private:

  void init();
  void drawScreen();

  SDL_Window* m_window;
  SDL_Renderer* m_renderer;

  bool m_drawFlag;

  static constexpr int GFX_SIZE = 64 * 32;
  unsigned char gfx[GFX_SIZE]; // Graphics supported was 64 x 32 pixels, monochrome

};



#endif
