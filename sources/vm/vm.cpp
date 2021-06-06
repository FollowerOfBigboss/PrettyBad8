#include "vm.h"

void VM::init()
{
	memset(this->V, 0, sizeof(this->V));
	memset(this->Key, 0, sizeof(this->Key));
	memset(this->memory, 0, sizeof(this->memory));
	memset(this->stack, 0, sizeof(this->stack));
	memset(this->gfx, 0, sizeof(this->gfx));

	this->I = 0;
	this->ST = 0;
	this->DT = 0;
	this->PC = 0x200; // 512
	this->SP = 0;

	memcpy(this->memory, &vm_font, sizeof(vm_font));
}

bool VM::loadrom(const std::string& rompath)
{
	lrom = rompath;

	FILE* rom;
	rom = fopen(rompath.c_str(), "rb");

	if (rom == nullptr) {
		return false;
	}

	fseek(rom, 0, SEEK_END);
	long romSize = ftell(rom);

	if (romSize > (4096 - 512)) {
		return false;
	}

	rewind(rom);

	void* tmpbuffer = malloc(romSize);
	if (tmpbuffer == nullptr)
	{
		return false;
	}

	memset(tmpbuffer, 0, romSize);
	fread(tmpbuffer, 1, romSize, rom);

	memcpy((this->memory + 512), tmpbuffer, romSize);

	free(tmpbuffer);
	return true;
}

void VM::cycle()
{
	this->opcode = this->memory[this->PC] << 8 | this->memory[this->PC + 1];

	switch (this->opcode & 0xF000)
	{
	case 0x0000:
		switch (this->opcode & 0x000F)
		{
			case 0x0000: instructions::CLS_00E0(this); break;
			case 0x000E: instructions::RET_00EE(this); break;
		}
		break;

	case 0x1000: instructions::JP_1000(this); break;
	case 0x2000: instructions::CALL_2000(this); break;
	case 0x3000: instructions::SE_3000(this); break;
	case 0x4000: instructions::SNE_4000(this); break;
	case 0x5000: instructions::SE_5000(this); break;
	case 0x6000: instructions::LD_6000(this); break;
	case 0x7000: instructions::ADD_7000(this); break;
	case 0x8000:
		switch (this->opcode & 0x000F)
		{
			case 0x0000: instructions::LD_8000(this); break;
			case 0x0001: instructions::OR_8001(this); break;
			case 0x0002: instructions::AND_8002(this); break;
			case 0x0003: instructions::XOR_8003(this); break;
			case 0x0004: instructions::ADD_8004(this); break;
			case 0x0005: instructions::SUB_8005(this); break;
			case 0x0006: instructions::SHR_8006(this); break;
			case 0x0007: instructions::SUBN_8007(this); break;
			case 0x000E: instructions::SHL_800E(this); break;
		}
		break;

	case 0x9000: instructions::SNE_9000(this); break;
	case 0xA000: instructions::LD_A000(this); break;
	case 0xB000: instructions::JP_B000(this); break;
	case 0xC000: instructions::RND_C000(this); break;
	case 0xD000: instructions::DRW_D000(this); break;

	case 0xE000:
		switch (this->opcode & 0x00FF)
		{
			case 0x009E: instructions::SKP_E09E(this); break;
			case 0x00A1: instructions::SKNP_E0A1(this); break;
		}
		break;

	case 0xF000:
		switch (this->opcode & 0x00FF)
		{
			case 0x0007: instructions::LD_F007(this); break;
			case 0x000A: instructions::LD_F00A(this); break;
			case 0x0015: instructions::LD_F015(this); break;
			case 0x0018: instructions::LD_F018(this); break;
			case 0x001E: instructions::ADD_F01E(this); break;
			case 0x0029: instructions::LD_F029(this); break;
			case 0x0033: instructions::LD_F033(this); break;
			case 0x0055: instructions::LD_F055(this); break;
			case 0x0065: instructions::LD_F065(this); break;
		}
		break;
	}

	if (this->DT > 0)
	{
		this->DT--;
	}

	if (this->ST > 0)
	{
		this->ST--;
	}
}

void VM::reset()
{
	memset(this->V, 0, sizeof(this->V));
	memset(this->Key, 0, sizeof(this->Key));
	memset(this->memory, 0, sizeof(this->memory));
	memset(this->stack, 0, sizeof(this->stack));
	memset(this->gfx, 0, sizeof(this->gfx));

	this->I = 0;
	this->ST = 0;
	this->DT = 0;
	this->PC = 0x200; // 512
	this->SP = 0;

	memcpy(this->memory, &vm_font, sizeof(vm_font));
	loadrom(lrom);
}


// 00E0 - CLS
//Clear the display.
void instructions::CLS_00E0(VM* VM)
{
	memset(VM->gfx, 0, sizeof(VM->gfx));
	VM->PC += 2;
}

// 00EE - RET 
// Return from a subroutine.
void instructions::RET_00EE(VM* VM)
{
	VM->SP--;
	VM->PC = VM->stack[VM->SP];
	VM->PC += 2;
}

// 1nnn - JP addr
// Jump to location nnn
void instructions::JP_1000(VM* VM)
{
	VM->PC = VM->opcode & 0x0FFF;
}

// 2nnn - CALL addr
// Call subroutine at nnn
void instructions::CALL_2000(VM* VM)
{
	VM->stack[VM->SP] = VM->PC;
	VM->SP++;
	VM->PC = VM->opcode & 0x0FFF;
}

// 3xkk - SE Vx, byte
// Skip next instruction if Vx = kk
void instructions::SE_3000(VM* VM)
{
	if (VM->V[(VM->opcode & 0x0F00) >> 8] == (VM->opcode & 0x00FF))
	{
		VM->PC += 4;
	}
	else
	{
		VM->PC += 2;
	}
}

// SNE Vx, byte
// Skip next instruction if Vx != kk
void instructions::SNE_4000(VM* VM)
{
	if (VM->V[(VM->opcode & 0x0F00) >> 8] != (VM->opcode & 0x00FF))
	{
		VM->PC += 4;
	}
	else
	{
		VM->PC += 2;
	}
}

// 5xy0 - SE Vx, Vy
// Skip next instruction if Vx = Vy
void instructions::SE_5000(VM* VM)
{
	if (VM->V[(VM->opcode & 0x0F00) >> 8] == VM->V[(VM->opcode & 0x00F0) >> 4])
	{
		VM->PC += 4;
	}
	else
	{
		VM->PC += 2;
	}
}

// 6xkk - LD Vx, byte
// Set Vx = kk
void instructions::LD_6000(VM* VM)
{
	VM->V[(VM->opcode & 0x0F00) >> 8] = VM->opcode & 0x00FF;
	VM->PC += 2;
}

// 7xkk - ADD Vx, byte
// Set Vx = Vx + kk
void instructions::ADD_7000(VM* VM)
{
	VM->V[(VM->opcode & 0x0F00) >> 8] += VM->opcode & 0x00FF;
	VM->PC += 2;
}

// 8xy0 - LD Vx, Vy
// Set Vx = Vy
void instructions::LD_8000(VM* VM)
{
	VM->V[(VM->opcode & 0x0F00) >> 8] = VM->V[(VM->opcode & 0x00F0) >> 4];
	VM->PC += 2;
}

// 8xy1 - OR Vx, Vy
// Set Vx = Vx OR Vy
void instructions::OR_8001(VM* VM)
{
	VM->V[(VM->opcode & 0x0F00) >> 8] |= VM->V[(VM->opcode & 0x00F0) >> 4];
	VM->PC += 2;
}

// 8xy2 - AND Vx, Vy
// Set Vx = Vx AND Vy
void instructions::AND_8002(VM* VM)
{
	VM->V[(VM->opcode & 0x0F00) >> 8] &= VM->V[(VM->opcode & 0x00F0) >> 4];
	VM->PC += 2;
}

// 8xy3 - XOR Vx, Vy
// Set Vx = Vx XOR Vy
void instructions::XOR_8003(VM* VM)
{
	VM->V[(VM->opcode & 0x0F00) >> 8] ^= VM->V[(VM->opcode & 0x00F0) >> 4];
	VM->PC += 2;
}

// 8xy4 - ADD Vx, Vy
// Set Vx = Vx + Vy, set VF = carry
void instructions::ADD_8004(VM* VM)
{
	if (VM->V[(VM->opcode & 0x00F0) >> 4] > (0xFF - VM->V[(VM->opcode & 0x0F00) >> 8]))
	{
		VM->V[0xF] = 1;
	}
	else
	{
		VM->V[0xF] = 0;
	}
	VM->V[(VM->opcode & 0x0F00) >> 8] += VM->V[(VM->opcode & 0x00F0) >> 4];
	VM->PC += 2;
}

// 8xy5 - SUB Vx, Vy
// Set Vx = Vx - Vy, set VF = NOT borrow
void instructions::SUB_8005(VM* VM)
{
	if (VM->V[(VM->opcode & 0x00F0) >> 4] > VM->V[(VM->opcode & 0x0F00) >> 8])
	{
		VM->V[0xF] = 0;
	}
	else
	{
		VM->V[0xF] = 1;
	}

	VM->V[(VM->opcode & 0x0F00) >> 8] -= VM->V[(VM->opcode & 0x00F0) >> 4];
	VM->PC += 2;
}

// 8xy6 - SHR Vx {, Vy}
// Set Vx = Vx SHR 1.
void instructions::SHR_8006(VM* VM)
{
	VM->V[0xF] = VM->V[(VM->opcode & 0x0F00) >> 8] & 0x1;
	VM->V[(VM->opcode & 0x0F00) >> 8] >>= 1;
	VM->PC += 2;
}

// 8xy7 - SUBN Vx, Vy
// Set Vx = Vy - Vx, set VF = NOT borrow
void instructions::SUBN_8007(VM* VM)
{
	if (VM->V[(VM->opcode & 0x0F00) >> 8] > VM->V[(VM->opcode & 0x00F0) >> 4])
		VM->V[0xF] = 0;
	else
		VM->V[0xF] = 1;
	VM->V[(VM->opcode & 0x0F00) >> 8] = VM->V[(VM->opcode & 0x00F0) >> 4] - VM->V[(VM->opcode & 0x0F00) >> 8];
	VM->PC += 2;
}

// 8xyE - SHL Vx {, Vy}
// Set Vx = Vx SHL 1
void instructions::SHL_800E(VM* VM)
{
	VM->V[0xF] = VM->V[(VM->opcode & 0x0F00) >> 8] >> 7;
	VM->V[(VM->opcode & 0x0F00) >> 8] <<= 1;
	VM->PC += 2;
}

// 9xy0 - SNE Vx, Vy
// Skip next instruction if Vx != Vy
void instructions::SNE_9000(VM* VM)
{
	if (VM->V[(VM->opcode & 0x0F00) >> 8] != VM->V[(VM->opcode & 0x00F0) >> 4])
	{
		VM->PC += 4;
	}
	else
	{
		VM->PC += 2;
	}
}

// Annn - LD I, addr
// Set I = nnn
void instructions::LD_A000(VM* VM)
{
	VM->I = VM->opcode & 0x0FFF;
	VM->PC += 2;
}

// Bnnn - JP V0, addr
// Jump to location nnn + V0
void instructions::JP_B000(VM* VM)
{
	VM->PC = (VM->opcode & 0x0FFF) + VM->V[0];
}

// Cxkk - RND Vx, byte
// Set Vx = random byte AND kk
void instructions::RND_C000(VM* VM)
{
	VM->V[(VM->opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (VM->opcode & 0x00FF);
	VM->PC += 2;

}

// Dxyn - DRW Vx, Vy, nibble
// Display n - byte sprite starting at memory location I at(Vx, Vy), set VF = collision
void instructions::DRW_D000(VM* VM)
{
	unsigned short x = VM->V[(VM->opcode & 0x0F00) >> 8];
	unsigned short y = VM->V[(VM->opcode & 0x00F0) >> 4];
	unsigned short height = VM->opcode & 0x000F;
	unsigned short pixel;

	VM->V[0xF] = 0;
	for (int yline = 0; yline < height; yline++)
	{
		pixel = VM->memory[VM->I + yline];
		for (int xline = 0; xline < 8; xline++)
		{
			/*
			 iterating left to right like this 
				********
			   /\	   /\
				|	    |
			  0x80	   0x10
			
			*/
			if ((pixel & (0x80 >> xline)) != 0)
			{
				if (VM->gfx[(x + xline + ((y + yline) * 64))] == 1)
				{
					VM->V[0xF] = 1;
				}
				VM->gfx[x + xline + ((y + yline) * 64)] ^= 1;
			}
		}
	}

	VM->PC += 2;
	VM->Draw = true;
}

// Ex9E - SKP Vx
// Skip next instruction if key with the value of Vx is pressed
void instructions::SKP_E09E(VM* VM)
{
	if (VM->Key[VM->V[(VM->opcode & 0x0F00) >> 8]] != 0)
	{
		VM->PC += 4;
	}
	else
	{
		VM->PC += 2;
	}

}

// ExA1 - SKNP Vx
// Skip next instruction if key with the value of Vx is not pressed
void instructions::SKNP_E0A1(VM* VM)
{
	if (VM->Key[VM->V[(VM->opcode & 0x0F00) >> 8]] == 0)
	{
		VM->PC += 4;
	}
	else
	{
		VM->PC += 2;
	}
}

// Fx07 - LD Vx, DT
// Set Vx = delay timer value
void instructions::LD_F007(VM* VM)
{
	VM->V[(VM->opcode & 0x0F00) >> 8] = VM->DT;
	VM->PC += 2;
}

// Fx0A - LD Vx, K
// Wait for a key press, store the value of the key in Vx
void instructions::LD_F00A(VM* VM)
{
	bool keyPress = false;

	for (int i = 0; i < 16; ++i)
	{
		if (VM->Key[i] != 0)
		{
			VM->V[(VM->opcode & 0x0F00) >> 8] = i;
			keyPress = true;
		}
	}

	// If we didn't received a keypress, skip this cycle and try again.
	if (!keyPress)
	{
		return;
	}

	VM->PC += 2;
}

// Fx15 - LD DT, Vx
// Set delay timer = Vx
void instructions::LD_F015(VM* VM)
{
	VM->DT = VM->V[(VM->opcode & 0x0F00) >> 8];
	VM->PC += 2;
}

// Fx18 - LD ST, Vx
// Set sound timer = Vx
void instructions::LD_F018(VM* VM)
{
	VM->ST = VM->V[(VM->opcode & 0x0F00) >> 8];
	VM->PC += 2;
}

// Fx1E - ADD I, Vx
// Set I = I + Vx
void instructions::ADD_F01E(VM* VM)
{
	if (VM->I + VM->V[(VM->opcode & 0x0F00) >> 8] > 0xFFF)
	{
		VM->V[0xF] = 1;
	}
	else
	{
		VM->V[0xF] = 0;
	}

	VM->I += VM->V[(VM->opcode & 0x0F00) >> 8];
	VM->PC += 2;
}

// Fx29 - LD F, Vx
//  Set I = location of sprite for digit Vx
void instructions::LD_F029(VM* VM)
{
	VM->I = VM->V[(VM->opcode & 0x0F00) >> 8] * 0x5;
	VM->PC += 2;
}

// Fx33 - LD B, Vx
// Store BCD representation of Vx in memory locations I, I + 1, and I + 2
void instructions::LD_F033(VM* VM)
{
	VM->memory[VM->I] = VM->V[(VM->opcode & 0x0F00) >> 8] / 100;
	VM->memory[VM->I + 1] = (VM->V[(VM->opcode & 0x0F00) >> 8] / 10) % 10;
	VM->memory[VM->I + 2] = (VM->V[(VM->opcode & 0x0F00) >> 8] % 100) % 10;
	VM->PC += 2;
}

// Fx55 - LD[I], Vx
// Store registers V0 through Vx in memory starting at location I
void instructions::LD_F055(VM* VM)
{
	for (int i = 0; i <= ((VM->opcode & 0x0F00) >> 8); ++i)
	{
		VM->memory[VM->I + i] = VM->V[i];
	}

	VM->I += ((VM->opcode & 0x0F00) >> 8) + 1;
	VM->PC += 2;
}

// Fx65 - LD Vx, [I]
//	Read registers V0 through Vx from memory starting at location I.
void instructions::LD_F065(VM* VM)
{
	for (int i = 0; i <= ((VM->opcode & 0x0F00) >> 8); i++)
	{
		VM->V[i] = VM->memory[VM->I + i];
	}

	VM->I += ((VM->opcode & 0x0F00) >> 8) + 1;
	VM->PC += 2;
}


/*


void Clock::Reset()
{
	lastCycleTimer = -1;
}

int Clock::CatchUpCycles(int frequency)
{
	auto now = std::chrono::system_clock::now();
	long long currentClockMs = std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count();

	if (lastCycleTimer == -1)
		lastCycleTimer = currentClockMs;

	int cycles = (int)(currentClockMs - lastCycleTimer) * frequency / 1000;

	if (cycles > 0)
		lastCycleTimer = currentClockMs;

	return cycles;
}


*/