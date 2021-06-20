#ifndef _VM_
#define _VM_

#include <iostream>
#include <string>

#include <chrono>

const uint8_t vm_font[] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20, 0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10, 0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40, 0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};


class VM
{
public:
	void init();
	bool loadrom(const std::string& rompath);
	void cycle();
	void reset();
	void reset_and_loadrom();

	void run(int freq);
	void shutdown();

	uint8_t V[16];
	uint16_t I;
	uint8_t ST; // Sound timer
	uint8_t DT; // Delay timer
	uint16_t PC;
	uint8_t SP;
	uint8_t Key[16];
	uint8_t memory[4096];
	uint16_t stack[16];
	uint8_t gfx[64 * 32];
	uint16_t opcode;

	std::string LastLoadedRomPath;
};


namespace instructions
{
	void CLS_00E0(VM* VM);
	void RET_00EE(VM* VM);

	void JP_1000(VM* VM);
	void CALL_2000(VM* VM);
	void SE_3000(VM* VM);
	void SNE_4000(VM* VM);
	void SE_5000(VM* VM);
	void LD_6000(VM* VM);
	void ADD_7000(VM* VM);

	void LD_8000(VM* VM);
	void OR_8001(VM* VM);
	void AND_8002(VM* VM);
	void XOR_8003(VM* VM);
	void ADD_8004(VM* VM);
	void SUB_8005(VM* VM);
	void SHR_8006(VM* VM);
	void SUBN_8007(VM* VM);
	void SHL_800E(VM* VM);

	void SNE_9000(VM* VM);
	void LD_A000(VM* VM);
	void JP_B000(VM* VM);
	void RND_C000(VM* VM);
	void DRW_D000(VM* VM);

	void SKP_E09E(VM* VM);
	void SKNP_E0A1(VM* VM);

	void LD_F007(VM* VM);
	void LD_F00A(VM* VM);
	void LD_F015(VM* VM);
	void LD_F018(VM* VM);
	void ADD_F01E(VM* VM);
	void LD_F029(VM* VM);
	void LD_F033(VM* VM);
	void LD_F055(VM* VM);
	void LD_F065(VM* VM);
}

// Use for now better implementation will implemented at future
inline int hztocycles(int freq) { return freq / 60; }

#endif


// TODO: Implement later
// https://github.com/geaz/emu-chip-8


/*
class Clock
{
public:
	void Reset();
	int CatchUpCycles(int frequency);

private:
	long long lastCycleTimer = -1;
};

*/
