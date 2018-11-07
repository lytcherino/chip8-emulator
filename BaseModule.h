#ifndef BASE_MODULE
#define BASE_MODULE

#include "EventHandler.h"

class BaseModule {

public:

  BaseModule(EventHandler& eventHandler);

  virtual void registerWithHandler(EventHandler& eventHandler) = 0;

protected:
  EventHandler& eventHandler;
};

#endif
