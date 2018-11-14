#include "DisplayModule.h"

DisplayModule::DisplayModule(EventHandler &eventHandler) : BaseModule(eventHandler), gfx(HEIGHT * WIDTH, 0)
{

  registerWithHandler(eventHandler);
  init();
}

void DisplayModule::registerWithHandler(EventHandler &eventHandler)
{

  auto onQuit = [this](const SDL_Event &event) {
    SDL_Quit();
  };

  eventHandler.registerCallback(onQuit, SDL_QUIT);
}

DisplayModule::~DisplayModule()
{
}

void DisplayModule::init()
{

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cout << "SDL_Init Error: " << SDL_GetError() << "\n";
  }

  m_window =
      std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>>(
          SDL_CreateWindow("Chip8",
                           0, 0, WIDTH, HEIGHT,
                           SDL_WINDOW_SHOWN |
                               SDL_WINDOW_RESIZABLE |
                               SDL_WINDOW_ALLOW_HIGHDPI),
          SDL_DestroyWindow);

  m_renderer =
      std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>>(
          SDL_CreateRenderer(m_window.get(),
                             -1,
                             SDL_RENDERER_ACCELERATED |
                                 SDL_RENDERER_PRESENTVSYNC),
          SDL_DestroyRenderer);

  SDL_RenderSetLogicalSize(m_renderer.get(), 640, 480);

  if (!m_renderer.get())
  {
    std::cerr << "SDL_CreateRendrer Error: " << SDL_GetError() << "\n";
    SDL_Quit();
  }
}

void DisplayModule::updateScreen()
{
  if (m_drawFlag)
  {
    drawScreen();
    SDL_RenderPresent(m_renderer.get());
    m_drawFlag = !m_drawFlag;
  }
}

void DisplayModule::setDrawColor(Color color)
{
  SDL_SetRenderDrawColor(m_renderer.get(), color.red, color.green, color.blue, color.alpha);
}

void DisplayModule::drawScreen()
{

  // Set colour to black before
  // screen is cleared (to black)
  setDrawColor(Color(0, 0, 0));
  SDL_RenderClear(m_renderer.get());

  // Set colour to white for drawing the pixels
  setDrawColor(Color(255, 255, 255));

  for (int i = 0; i < HEIGHT; ++i)
  {
    for (int j = 0; j < WIDTH; ++j)
    {
      if (gfx[i * WIDTH + j] == 1)
      {
        // Subtract to mirror the image, as the SDL coordinates
        // (0,0) are in the lower left corner, whereas
        // Chip8 expects it at the top left.
        SDL_RenderDrawPoint(m_renderer.get(), WIDTH - 1 - j, i);
      }
    }
  }
}

void DisplayModule::clearScreen()
{
  // Set all pixels (bytes) to 0
  std::fill(gfx.begin(), gfx.end(), 0);
  setDrawFlag();
}

std::vector<unsigned char> &DisplayModule::getGfxArray()
{
  return gfx;
}

unsigned char *DisplayModule::getFontset()
{
  return fontset;
}

void DisplayModule::setDrawFlag()
{
  m_drawFlag = true;
}
