
#include "Chip8.h"
#include <iomanip>
#include <random>

#define DEBUG true

Chip8::Chip8()
  : displayModule(m_sdlEventHandler),
    inputModule(m_sdlEventHandler),
    memory(4096, 0) {
  init();
  srand(0);
}

void Chip8::init() {
  // Initialise registers and memory once;

  pc = 0x200; // Program counter starts at this address;
  opcode = 0;
  I = 0;
  sp = 0;

  // Clear Display
  //displayModule.clearScreen();

  // TODO: use std::copy(...) instead

  // Clear registers V0-VF and stack
  for (int i = 0; i < 0xF; ++i) {
    V[i] = 0x0;
    stack[i] = 0x0;
  }

  // Clear memory
  for (int i = 0; i < 4096; ++i) {
    memory[i] = 0x0;
  }

  // Load fontset
  auto fontset = displayModule.getFontset();
  for (int i = 0; i < 80; ++i) {
    memory[i] = fontset[i];
  }

}

void Chip8::handleEvents() {
  m_sdlEventHandler.handleEvents();
}

void Chip8::updateDisplay() {
  displayModule.updateScreen();
}

bool Chip8::load(const std::string& file) {

  init();

  std::ifstream reader(file, std::ios::binary);

  if (reader.good()) {

    int i = 0;
    char c;
    std::cout << "\n0x" << std::setw(4) << std::setfill('0')
              << static_cast<int>(0) << " ";

    // (while (reader >> c)) skips 0x20, space character!
    while (!reader.eof()) {

      reader.read(&c, sizeof(char));

      std::cout << std::hex << std::setfill('0')
                << std::setw(2) << static_cast<int>(c & 0xFF) << " ";

      memory[i+pc] = c & 0xFF;
      i++;

      if (i % 0x10 == 0) {
        std::cout << "\n0x" << std::setw(4) << std::setfill('0')
                  << static_cast<int>(i) << " ";
      }

    }

    std::cout << "i: " << i << "\n";
  } else {

    std::cout << "Failed to open " << file << "\n";
    return false;
  }

  return true;
}

void Chip8::emulateCycle() {

  // An opcode is 2 bytes long; need to fetch 2 successive bytes of the program counter
  opcode = memory[pc] << 8 | memory[pc + 1]; // Join into 2 bytes

  if (DEBUG) {
  std::cout << std::setw(2) << std::setfill('0') << std::hex
            << "memory[" << pc << "] << 8 = "
            << static_cast<int>(memory[pc] << 8) << "\n"
            << "memory[" << pc << " + 1] << 8 = "
            << static_cast<int>(memory[pc + 1]) << "\n";

  std::cout << "opcode: " << std::setw(4) << std::setfill('0') << std::hex << opcode << "\n";
  std::cout << "pc: " << std::dec << pc << "\n";
      }

  auto beforeOpPc = pc;
  auto calledSubroutine = false;

  switch(opcode & 0xF000)
    {

    case 0x0000:
      switch (opcode & 0x00FF)
        {
        case 0x00E0: {
          // Clears screen
          displayModule.clearScreen();
          displayModule.setDrawFlag();
          break;
        }

        case 0x00EE: {
          // 0x00EE: Returns from subroutine

          if (DEBUG) {
            std::cout << "Stack at level " << sp << " = " << pc << "\n";
            std::cout << "stack[sp] = " << stack[sp] << "\n";
          }

          pc = stack[sp];
          --sp;

          if (DEBUG) {
            std::cout << "pc = " << pc << "\n"; 
          }

          calledSubroutine = true;
          break;
        }
        default: {
          std::cout << "Invalid opcode: " << std::setw(4)
                    << std::setfill('0') << opcode << "\n";
          break;
        }
      }

      if (calledSubroutine) {
        calledSubroutine = !calledSubroutine;
        break;
      }

    case 0x1000:
      {
        pc = opcode & 0x0FFF;
        break;
      }

    case 0x2000:
      {
        ++sp;
        stack[sp] = pc;
        std::cout << "Stack at level " << sp << " = " << pc << "\n";
        pc = opcode & 0x0FFF;
        break;
      }

    case 0x3000:
      {
        if (V[opcode & 0x0F00 >> 8] != (opcode & 0x00FF)) {
          pc += 2;
        }
        pc += 2;
        break;
      }

    case 0x4000:
      {
        if (V[opcode & 0x0F00 >> 8] == (opcode & 0x00FF)) {
          pc += 2;
        }
        pc += 2;
        break;
      }

    case 0x5000:
      {
        if (V[opcode & 0x0F00 >> 8] == V[opcode & 0x00F0 >> 4]) {
          pc += 2;
        }
        pc += 2;
        break;
      }

    case 0x6000:
      {
        V[opcode & 0x0F00 >> 8] = opcode & 0x00FF;
        pc += 2;
        break;
      }

    case 0x7000:
      {
        V[opcode & 0x0F00 >> 8] += opcode & 0x00FF;
        pc += 2;
        break;
      }

    case 0x8000:
      {
        switch (opcode & 0x000F) {

        case 0x0001: {
          V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] | V[opcode & 0x00F0 >> 4];
          pc += 2;
          break;
        }

        case 0x0002: {
          V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] & V[opcode & 0x00F0 >> 4];
          pc += 2;
          break;
        }

        case 0x0003: {
          V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] ^ V[opcode & 0x00F0 >> 4];
          pc += 2;
          break;
        }

        case 0x0004: {
          V[0xF] = 0;
          V[opcode & 0x0F00 >> 8] = (V[opcode & 0x0F00 >> 8] + V[opcode & 0x00F0 >> 4]) & 0xFF;
          if (V[opcode & 0x0F00 >> 8] + V[opcode & 0x00F0 >> 4] > 0xFF) {
            V[0xF] = 1;
          }
          pc += 2;
          break;
        }

        case 0x0005: {
          V[0xF] = 0;
          V[opcode & 0x0F00 >> 8] = V[opcode & 0x0F00 >> 8] - V[opcode & 0x00F0 >> 4];
          if (V[opcode & 0x0F00 >> 8] > V[opcode & 0x00F0 >> 4]) {
            V[0xF] = 1;
          }
          pc += 2;
          break;
        }

        case 0x0006: {
          V[0xF] = 0;
          if ((V[opcode & 0x0F00 >> 8] & 0x01) == 1) {
            V[0xF] = 1;
          }
          pc += 2;
          break;
        }

        case 0x0007: {
          V[0xF] = 0;
          V[opcode & 0x0F00 >> 8] = V[opcode & 0x00F0 >> 4] - V[opcode & 0x0F00 >> 8];
          if (V[opcode & 0x00F0 >> 4] > V[opcode & 0x0F00 >> 8]) {
            V[0xF] = 1;
          }
          pc += 2;
          break;
        }

        case 0x0008: {
          V[0xF] = 0;
          if ((V[opcode & 0x0F00 >> 8] & 0x80) == 1) {
            V[0xF] = 1;
          }
          pc += 2;
          break;
        }
        default: {
          std::cout << "Invalid opcode: " << std::setw(4)
                    << std::setfill('0') << std::hex << opcode << "\n";
          break;
        }
        }
      }

    case 0x9000:
      {
        if (V[opcode & 0x0F00] != V[opcode & 0x00F0]) {
          pc += 2;
        }
        pc += 2;
        break;
      }

    case 0xA000:
      {
        I = opcode & 0x0FFF;
        pc += 2;
        break;
      }

    case 0xB000:
      {
        pc = (opcode & 0x0FFF) + V[0x0];
        break;
      }

    case 0xC000:
      {
        V[opcode & 0x0F00 >> 8] = rand() % 0xFF + V[0x0];
        pc += 2;
        break;
      }

    case 0xD000:
      {
        unsigned short x = V[(opcode & 0x0F00) >> 8];
        unsigned short y = V[(opcode & 0x00F0) >> 4];
        unsigned short height = opcode & 0x000F;
        unsigned short pixel;

        V[0xF] = 0; // Reset VF register
        // Loop over each row
        for (int yScan = 0; yScan < height; ++yScan) {
          pixel = memory[I + yScan]; // Pixel value of memory starting at I
          // Loop over each column (8 bits of 1 row)
          for (int xScan = 0; xScan < 8; ++xScan) {
            // Check which pixels are 0, within a byte
            // by checking each one individually by shifting
            if ((pixel && (0x80 >> xScan)) == 1) {
              // If the pixel is 1 and the pixel on the display is 1
              // then a collision is detected
              if (displayModule.getGfxArray()[x + xScan + ((y + yScan) * 64)] == 1) {
                V[0xF] = 1; // mark collision
              }
              displayModule.getGfxArray()[x + xScan + ((y + yScan) * 64)] ^= 1;
            }
          }
        }
        displayModule.setDrawFlag();
        pc += 2;
        break;
      }

    case 0xE000:
      {
        switch (opcode & 0x00FF) {

        case 0x009E: {
          if (inputModule.isKeyPressed(V[(opcode & 0x0F00) >> 8])) {
            pc += 4;
          } else {
            pc += 2;
          }
          break;
        }

        case 0x00A1: {
          if (!inputModule.isKeyPressed(V[(opcode & 0x0F00) >> 8])) {
            pc += 4;
          } else {
            pc += 2;
          }
          break;
        }
        default: {
          std::cout << "Invalid opcode: " << std::setw(4)
                    << std::setfill('0') << std::hex << opcode << "\n";
          break;
        }
        }
      }

    case 0xF000:
      {
        switch (opcode & 0x00FF) {

          case 0x0007: {
            V[(opcode & 0x0F00) >> 8] = delayTimer;
            pc += 2;
          }

          case 0x000A: {
            auto value = inputModule.waitUntilKeyPress();
            V[(opcode & 0x0F00)] = value;
            pc += 2;
          }

          case 0x0015: {
            delayTimer = (opcode & 0x0F00) >> 8;
            pc += 2;
          }

          case 0x0018: {
            soundTimer = V[opcode & 0x0F00 >> 8];
            pc += 2;
            break;
          }

          case 0x001E: {
            I += V[opcode & 0x0F00 >> 8];
            pc += 2;
            break;
          }

          case 0x0029: {
            I = V[opcode & 0x0F00 >> 8] * 0x05;
            pc += 2;
            break;
          }

          case 0x0033: {
            memory[I] = V[(opcode & 0x0F00) >> 8] / 100; // hundreds digit
            memory[I + 1] = (V[(opcode & 0x0F00) >> 8] % 100) / 10; // tenth digit
            memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10; // ones digit
            pc += 2;
            break;
          }

          case 0x0055: {
            for (int i = 0; i < V[opcode & 0x0F00 >> 8]; ++i) {
              memory[I+i] = V[i];
            }
            pc += 2;
            break;
          }

          case 0x0065: {
            for (int i = 0; i < V[opcode & 0x0F00 >> 8]; ++i) {
              V[i] = memory[I+i];
            }
            pc += 2;
            break;
          }

          default: {
            std::cout << "Invalid opcode: " << std::setw(4)
                      << std::setfill('0') << std::hex << opcode << "\n";
            break;
          }
        }
      }
    }

    if (beforeOpPc == pc) {
      std::cout << "Invalid opcode: " << std::setw(4)
                << std::setfill('0') << std::hex << opcode << "\n";
    }

    // Update timers
    if (delayTimer > 0) {
      --delayTimer;
    }

    if (soundTimer > 0) {
      if (soundTimer == 1) {
        std::cout << "Beep!\n";
      }
      --soundTimer;
    }

}

