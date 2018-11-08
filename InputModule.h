#ifndef INPUT_MODULE_H
#define INPUT_MODULE_H

#include <vector>
#include <iostream>
#include <utility>

#include "SDL.h"

#include "BaseModule.h"

class InputModule : public BaseModule {

public:

  InputModule(EventHandler& eventHandler);

  int waitUntilKeyPress();
  bool isKeyPressed(unsigned int index);

  void registerWithHandler(EventHandler& eventHandler) override;

private:

  void validateKey(SDL_Keycode key, bool state);
  void notifyKeyPress(unsigned int keyValue, bool state);

  /*
  std::vector<unsigned char> keyInputs = {
    '1', '2', '3', '4', 'Q', 'W', 'E', 'R',
    'A', 'S', 'D', 'F', 'Z', 'X', 'C', 'V'
  };
  */

  // true, position down (pressed), false (position up) not pressed
  std::vector<std::pair<SDL_Keycode, bool>> keyState = {
    {SDLK_1, false}, {SDLK_2, false}, {SDLK_3, false}, {SDLK_4, false},
    {SDLK_q, false}, {SDLK_w, false}, {SDLK_e, false}, {SDLK_r, false},
    {SDLK_a, false}, {SDLK_s, false}, {SDLK_d, false}, {SDLK_f, false},
    {SDLK_z, false}, {SDLK_x, false}, {SDLK_c, false}, {SDLK_v, false}
  };

};


#endif
