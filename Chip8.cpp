
#include "Chip8.h"

Chip8::Chip8() {

}

void Chip8::init() {
  // Initialise registers and memory once;

  pc = 0x200; // Program counter starts at this address;
  opcode = 0;
  I = 0;
  sp = 0;

  // Clear Display

  // Clear stack

  // Clear registers V0-VF

  // Clear memory

  // Load fontset
  for (int i = 0; i < 80; ++i) {
    memory[i] = fontset[i];
  }

}

bool Chip8::load(const std::string& file) {


  std::fstream reader(file, std::ios_base::binary);

  auto bufferSize = 0xFFF - 0x200;
  char data[bufferSize];
  reader.read(data, bufferSize);
  for (int i = 0; i < 0xFFF-0x200; ++i) {
    // Start filling from 0x200 (512)
    memory[i + 0x200] = data[i];
    // Assign byte by byte instead using
    // >> reader ?
  }

  // return false if file is invalid
  return true;
}

void Chip8::emulateCycle() {
  // Fetch Opcode

  // An opcode is 2 bytes long; need to fetch 2 successive bytes of the program counter
  opcode = memory[pc] << 8 | memory[pc + 1]; // Join into 2 bytes

  switch(opcode & 0xF000)
    {
    case 0x0000:
      switch (opcode & 0x000F)
        {
        case 0x0000: // 0x00E0: Clears screen

        break;
        case 0x000E: // 0x00EE: Returns from subroutine

        break;
        }
    break;

    case 0x2000:
      {
        stack[sp] = pc;
        ++sp;
        pc = opcode & 0x0FFF;
        break;
      }

    case 0x0004:
      {
        if (V[(opcode & 0x00F0) >> 4] + V[(opcode & 0x0F00) >> 8] > 0xFF) {
          // If larger than 255, set carry flag
          V[0xF] = 1;
        }
        else {
          V[0xF] = 0;
        }
        V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
        pc += 2;
        break;
      }

    case 0x0033:
      {
        memory[I] = V[(opcode & 0x0F00) >> 8] / 100; // hundreds digit
        memory[I + 1] = (V[(opcode & 0x0F00) >> 8] % 100) / 10; // tenth digit
        memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10; // ones digit
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
            if ((pixel && (0x80 >> xScan)) != 0) {
              // If the pixel is 1 and the pixel on the display is 1
              // then a collision is detected
              if (gfx[(x + xScan + ((y + yScan) * 64))] == 1) {
                V[0xF] = 1; // mark collision
              }
              gfx[x + xScan + ((y + yScan) * 64)] ^= 1;
            }
          }
        }
        drawFlag = true;
        pc += 2;
        break;
      }

    case 0xE000:
      {
        switch (opcode & 0x00FF) {
          // EX9E: Skips the next instruction
          // If the key stored in VX is pressed
        case 0x009E: {
            if (key[V[(opcode & 0x0F00) >> 8]] != 0) {
              pc += 4;
            } else {
              pc += 2;
            }
            break;
          }
          break;
        }
      }

    // Decode Opcode

    // Execute Opcode

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
}


bool Chip8::graphicsUpdated() {
  return drawFlag;
}
