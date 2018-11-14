#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "SDL.h"

#include <functional>
#include <vector>
#include <map>
#include <iostream>

class EventHandler
{
  using Callback = std::function<void(const SDL_Event &)>;

public:
  void registerCallback(const Callback &callback, const SDL_EventType &event)
  {
    m_eventCallbacks[event].push_back(callback);
  }

  void handleEvents()
  {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
      for (auto &&callback : m_eventCallbacks[static_cast<SDL_EventType>(event.type)])
      {
        callback(event);
      }
    }
  }

private:
  std::map<SDL_EventType, std::vector<Callback>> m_eventCallbacks;
};

#endif
