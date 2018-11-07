#ifndef CHIP8_H
#define CHIP8_H

#include <iostream>
#include <fstream>

#include "DisplayModule.h"
#include "InputModule.h"

class Chip8
{
public:

  Chip8();

  void init();
  void emulateCycle();
  void handleEvents();
  void updateDisplay();
  bool load(const std::string& file);

private:

  EventHandler m_sdlEventHandler;

  DisplayModule displayModule;
  InputModule inputModule;

  unsigned short opcode;

  // System memory map:
  // 0x200 - 0xFFF - Program ROM and working RAM
  // 0x050 - 0x0A0 - Pixel fonts
  // 0x000 - 0x1FF - Chip Interpreter
  //unsigned char memory[4096]; // 4K memory
  std::vector<unsigned char> memory; // 4K memory

  unsigned char V[16]; // 15, 8-bit registers, V1 ... VF

  unsigned short I; // Index register
  unsigned short pc; // Program counter;

  unsigned short stack[16]; // Stack with 16 levels
  unsigned short sp; // Stack pointer, points to current level used in stack

  // 60Hz timers
  unsigned char delayTimer;
  unsigned char soundTimer; // System buzzes when sound timer is 0

};
#endif
