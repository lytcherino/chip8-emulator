#include "InputModule.h"

#define DEBUG false

InputModule::InputModule(EventHandler& eventHandler)
  : BaseModule(eventHandler) {
  registerWithHandler(eventHandler);
}

void InputModule::registerWithHandler(EventHandler& eventHandler) {

  auto onKeyDown = [this](const SDL_Event& event) {
    if (DEBUG) {
      std::cout << "KeyDown Event: " << SDL_GetKeyName(event.key.keysym.sym) << "\n";
    }
    validateKey(event.key.keysym.sym, true);
  };

  auto onKeyUp = [this](const SDL_Event& event) {
    if (DEBUG) {
      std::cout << "KeyUp Event: " << SDL_GetKeyName(event.key.keysym.sym) << "\n";
    }
    validateKey(event.key.keysym.sym, false);
  };

  eventHandler.registerCallback(onKeyDown, SDL_KEYDOWN);
  eventHandler.registerCallback(onKeyUp, SDL_KEYUP);
}

void InputModule::validateKey(SDL_Keycode key, bool state) {
  std::cout << SDL_GetKeyName(key) << " is";

  // TODO: use a map to map directly instead of looping
  for (int i = 0; i < keyState.size(); ++i) {
    if (keyState[i].first == key) {
      keyState[i].second = state;
      std::cout << " a legal key\n";
      return;
    }
  }
  std::cout << " an illegal key\n";
}

int InputModule::waitUntilKeyPress() {

  // TODO: use condition variable to wait on mutex?
  std::cout << "Waiting for key press...\n";
  while(true) {

    eventHandler.handleEvents();

    for (int i = 0; i < keyState.size(); ++i) {
      if (keyState[i].second) {
        std::cout << "Exiting key wait state";
        return i;
      }
    }

    SDL_Delay(100);
  }

}

bool InputModule::isKeyPressed(unsigned int index) {
  return keyState[index].second;
}
