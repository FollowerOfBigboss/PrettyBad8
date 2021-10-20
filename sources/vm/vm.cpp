#include "vm.h"
#include <cstring>

VM::VM()
{
	this->reset();
}

void VM::init()
{
	this->reset();
}

bool VM::loadrom(const std::string& rompath)
{
	LastLoadedRomPath = rompath;

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
}

void VM::reset_and_loadrom()
{
	this->reset();
	this->loadrom(LastLoadedRomPath);
}

void VM::run(int freq)
{
	int neededcycles = 0;
	neededcycles = hztocycles(freq);


	for (int i = 0; i < neededcycles; i++)
	{
		this->cycle();
	}
}

void VM::shutdown()
{
	this->reset();
}

void instructions::CLS_00E0(VM* vm)
{
	memset(vm->gfx, 0, sizeof(vm->gfx));
	vm->PC += 2;
}

void instructions::RET_00EE(VM* vm)
{
	vm->SP--;
	vm->PC = vm->stack[vm->SP];
	vm->stack[vm->SP] = static_cast<uint16_t>(0);
	vm->PC += 2;
}

void instructions::JP_1000(VM* vm)
{
	vm->PC = vm->opcode & 0x0FFF;
}

void instructions::CALL_2000(VM* vm)
{
	vm->stack[vm->SP] = vm->PC;
	vm->SP++;
	vm->PC = (vm->opcode & 0x0FFF);
}

void instructions::SE_3000(VM* vm)
{
	if (vm->V[(vm->opcode & 0x0F00) >> 8] == (vm->opcode & 0x00FF))
	{
		vm->PC += 4;
	}
	else
	{
		vm->PC += 2;
	}
}

void instructions::SNE_4000(VM* vm)
{
	if (vm->V[(vm->opcode & 0x0F00) >> 8] != (vm->opcode & 0x00FF))
	{
		vm->PC += 4;
	}
	else
	{
		vm->PC += 2;
	}
}

void instructions::SE_5000(VM* vm)
{
	if (vm->V[(vm->opcode & 0x0F00) >> 8] == vm->V[(vm->opcode & 0x00F0) >> 4])
	{
		vm->PC += 4;
	}
	else
	{
		vm->PC += 2;
	}
}

void instructions::LD_6000(VM* vm)
{
	vm->V[(vm->opcode & 0x0F00) >> 8] = (vm->opcode & 0x00FF);
	vm->PC += 2;
}

void instructions::ADD_7000(VM* vm)
{
	vm->V[(vm->opcode & 0x0F00) >> 8] += (vm->opcode & 0x00FF);
	vm->PC += 2;
}

void instructions::LD_8000(VM* vm)
{
	vm->V[(vm->opcode & 0x0F00) >> 8] = vm->V[(vm->opcode & 0x00F0) >> 4];
	vm->PC += 2;
}

void instructions::OR_8001(VM* vm)
{
	vm->V[(vm->opcode & 0x0F00) >> 8] |= vm->V[(vm->opcode & 0x00F0) >> 4];
	vm->PC += 2;
}

void instructions::AND_8002(VM* vm)
{
	vm->V[(vm->opcode & 0x0F00) >> 8] &= vm->V[(vm->opcode & 0x00F0) >> 4];
	vm->PC += 2;
}

void instructions::XOR_8003(VM* vm)
{
	vm->V[(vm->opcode & 0x0F00) >> 8] ^= vm->V[(vm->opcode & 0x00F0) >> 4];
	vm->PC += 2;
}

void instructions::ADD_8004(VM* vm)
{
	uint16_t tmp = vm->V[(vm->opcode & 0x0F00) >> 8] + vm->V[(vm->opcode & 0x00F0) >> 4];
	vm->V[(vm->opcode & 0x0F00) >> 8] = static_cast<uint8_t>(tmp);
	vm->V[15] = (tmp >> 8);
	// printf("tmp is %i | V[15] is %i\n", tmp, vm->V[15]);
	vm->PC += 2;
}

void instructions::SUB_8005(VM* vm)
{

	uint16_t tmp = vm->V[(vm->opcode & 0x0F00) >> 8] - vm->V[(vm->opcode & 0x00F0) >> 4];
	vm->V[(vm->opcode & 0x0F00) >> 8] = static_cast<uint8_t>(tmp);
	vm->V[15] = (tmp >> 8) + 1;	
	vm->PC += 2;
}

void instructions::SHR_8006(VM* vm)
{
	vm->V[0xF] = vm->V[(vm->opcode & 0x0F00) >> 8] & 0x1;
	vm->V[(vm->opcode & 0x0F00) >> 8] >>= 1;
	vm->PC += 2;
}

void instructions::SUBN_8007(VM* vm)
{
	uint16_t tmp = vm->V[(vm->opcode & 0x00F0) >> 4] - vm->V[(vm->opcode & 0x0F00) >> 8];
	vm->V[(vm->opcode & 0x0F00) >> 8] = static_cast<uint8_t>(tmp);
	vm->V[15] = (tmp >> 8) + 1;
	vm->PC += 2;
}

void instructions::SHL_800E(VM* vm)
{
	vm->V[0xF] = vm->V[(vm->opcode & 0x0F00) >> 8] >> 7;
	vm->V[(vm->opcode & 0x0F00) >> 8] <<= 1;
	vm->PC += 2;
}

void instructions::SNE_9000(VM* vm)
{
	if (vm->V[(vm->opcode & 0x0F00) >> 8] != vm->V[(vm->opcode & 0x00F0) >> 4])
	{
		vm->PC += 4;
	}
	else
	{
		vm->PC += 2;
	}
}

void instructions::LD_A000(VM* vm)
{
	vm->I = (vm->opcode & 0x0FFF);
	vm->PC += 2;
}

void instructions::JP_B000(VM* vm)
{
	vm->PC = (vm->opcode & 0x0FFF) + vm->V[0];
}

void instructions::RND_C000(VM* vm)
{
	vm->V[(vm->opcode & 0x0F00) >> 8] = (rand() % (0xFF + 1)) & (vm->opcode & 0x00FF);
	vm->PC += 2;
}

void instructions::DRW_D000(VM* vm)
{
	// Taken from vision8

	uint8_t* q;
	uint8_t n, x, x2, y, collision;
	uint16_t p;
	x = vm->V[(vm->opcode & 0x0F00) >> 8] & 63;
	y = vm->V[(vm->opcode & 0x00F0) >> 4] & 31;
	p = vm->I;
	q = vm->gfx + y * 64;
	n = vm->opcode & 0x0f;
	if (n + y > 32) n = 32 - y;
	for (collision = 1; n; --n, q += 64)
	{
		for (y = vm->memory[p++], x2 = x; y; y <<= 1, x2 = (x2 + 1) & 63)
			if (y & 0x80) collision &= (q[x2] ^= 1);
	}
	vm->V[15] = collision ^ 1;
	vm->PC += 2;
}

void instructions::SKP_E09E(VM* vm)
{
	if (vm->Key[vm->V[(vm->opcode & 0x0F00) >> 8]] == 1)
	{
		vm->PC += 4;
	}
	else
	{
		vm->PC += 2;
	}
}

void instructions::SKNP_E0A1(VM* vm)
{
	if (vm->Key[vm->V[(vm->opcode & 0x0F00) >> 8]] == 0)
	{
		vm->PC += 4;
	}
	else
	{
		vm->PC += 2;
	}
}

void instructions::LD_F007(VM* vm)
{
	vm->V[(vm->opcode & 0x0F00) >> 8] = vm->DT;
	vm->PC += 2;
}

void instructions::LD_F00A(VM* vm)
{
	bool key_pressed = false;

	for (int i = 0; i < 16; ++i)
	{
		if (vm->Key[i] != 0)
		{
			vm->V[(vm->opcode & 0x0F00) >> 8] = i;
			key_pressed = true;
		}
	}

	if (!key_pressed)
		return;

	vm->PC += 2;
}

void instructions::LD_F015(VM* vm)
{
	vm->DT = vm->V[(vm->opcode & 0x0F00) >> 8];
	vm->PC += 2;
}

void instructions::LD_F018(VM* vm)
{
	vm->ST = vm->V[(vm->opcode & 0x0F00) >> 8];
	vm->PC += 2;
}

void instructions::ADD_F01E(VM* vm)
{
	vm->I += vm->V[(vm->opcode & 0x0F00) >> 8];
	vm->PC += 2;
}

void instructions::LD_F029(VM* vm)
{
	vm->I = vm->V[(vm->opcode & 0x0F00) >> 8] * 0x5;
	vm->PC += 2;
}

void instructions::LD_F033(VM* vm)
{
	vm->memory[vm->I] = vm->V[(vm->opcode & 0x0F00) >> 8] / 100;
	vm->memory[vm->I + 1] = (vm->V[(vm->opcode & 0x0F00) >> 8] / 10) % 10;
	vm->memory[vm->I + 2] = vm->V[(vm->opcode & 0x0F00) >> 8] % 10;
	vm->PC += 2;
}

void instructions::LD_F055(VM* vm)
{
	for (int i = 0; i <= ((vm->opcode & 0x0F00) >> 8); ++i)
	{
		vm->memory[vm->I + i] = vm->V[i];
	}
	vm->PC += 2;
}


void instructions::LD_F065(VM* vm)
{
	for (int i = 0; i <= ((vm->opcode & 0x0F00) >> 8); ++i)
	{
		vm->V[i] = vm->memory[vm->I + i];
	}

	vm->PC += 2;
}