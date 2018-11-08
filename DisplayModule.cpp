#include "DisplayModule.h"

DisplayModule::DisplayModule(EventHandler& eventHandler) : BaseModule(eventHandler) {

  registerWithHandler(eventHandler);
  init();

}

void DisplayModule::registerWithHandler(EventHandler& eventHandler) {

  auto onQuit = [this](const SDL_Event& event) {
    SDL_Quit();
  };

  eventHandler.registerCallback(onQuit, SDL_QUIT);
}

DisplayModule::~DisplayModule() {

}

void DisplayModule::init() {

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_Init Error: " << SDL_GetError() << "\n";
  }

  m_window =
    std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>(
      SDL_CreateWindow("Chip8",
                        100, 100, 640, 480,
                        SDL_WINDOW_SHOWN |
                        SDL_WINDOW_RESIZABLE |
                        SDL_WINDOW_ALLOW_HIGHDPI
                       ),
      SDL_DestroyWindow);

  m_renderer =
    std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>>(
      SDL_CreateRenderer(m_window.get(),
                         -1,
                         SDL_RENDERER_ACCELERATED |
                         SDL_RENDERER_PRESENTVSYNC
                        ),
      SDL_DestroyRenderer);

   SDL_RenderSetLogicalSize(m_renderer.get(), 640, 480);

  if (!m_renderer.get()) {
    std::cerr << "SDL_CreateRendrer Error: " << SDL_GetError() << "\n";
    SDL_Quit();
  }
}

void DisplayModule::updateScreen() {
  if (m_drawFlag) {
    drawScreen();
    SDL_RenderPresent(m_renderer.get());
    m_drawFlag = !m_drawFlag;
  }
}

void DisplayModule::setDrawColor(Color color) {
  SDL_SetRenderDrawColor(m_renderer.get(), color.red, color.green, color.blue, color.alpha);
}

void DisplayModule::drawScreen() {

  // Set colour to black before
  // screen is cleared (to black)
  setDrawColor(Color(0,0,0));
  SDL_RenderClear(m_renderer.get());

  // Set colour to white for drawing the pixels
  setDrawColor(Color(255,255,255));

  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 64; ++j) {
      if (gfx[i * 64 + j] == 1) {
        SDL_RenderDrawPoint(m_renderer.get(), i, j);
      }
    }
  }
}

void DisplayModule::clearScreen() {
  for (int i = 0; GFX_SIZE; ++i) {
    // Set all bytes to 0
    gfx[i] = 0;
  }
  setDrawFlag();
}

unsigned char* DisplayModule::getGfxArray() {
  return gfx;
}

unsigned char* DisplayModule::getFontset() {
  return fontset;
}

void DisplayModule::setDrawFlag() {
  m_drawFlag = true;
}

