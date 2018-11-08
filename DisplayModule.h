#ifndef DISPLAY_MODULE_H
#define DISPLAY_MODULE_H

#include "SDL.h"

#include <iostream>
#include <map>
#include <memory>

#include "BaseModule.h"


class DisplayModule : public BaseModule {

  struct sdl_deleter
  {
    void operator()(SDL_Window *p) const { SDL_DestroyWindow(p); }
    void operator()(SDL_Renderer *p) const { SDL_DestroyRenderer(p); }
    void operator()(SDL_Texture *p) const { SDL_DestroyTexture(p); }
  };

  struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

    Color (uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) {
      this->red = red;
      this->green = green;
      this->blue = blue;
      this->alpha = alpha;
    }
  };

public:

  DisplayModule(EventHandler& eventHandler);
  ~DisplayModule();

  void clearScreen();
  void updateScreen();
  void setDrawFlag();

  void registerWithHandler(EventHandler& eventHandler) override;

  unsigned char* getGfxArray();
  unsigned char* getFontset();

private:

  void init();
  void drawScreen();
  void setDrawColor(Color color);

  std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> m_window;
  std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>> m_renderer;

  bool m_drawFlag;

  static constexpr int GFX_SIZE = 64 * 32;
  unsigned char gfx[GFX_SIZE]; // Graphics supported was 64 x 32 pixels, monochrome

  unsigned char fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

};



#endif
