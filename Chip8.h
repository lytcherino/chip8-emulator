#ifndef CHIP8_H
#define CHIP8_H

#include <iostream>
#include <fstream>

#include "DisplayModule.h"
//#include "EventHandler.h"

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

  unsigned char keyInputs[16] = {
    '1', '2', '3', '4', 'Q', 'W', 'E', 'R',
    'A', 'S', 'D', 'F', 'Z', 'X', 'C', 'V'
  };

  unsigned char key[16];


};
#endif
