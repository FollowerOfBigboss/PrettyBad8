#ifndef N_DBG
#define N_DBG

#include "vm.h"
#include <vector>
#include <algorithm>

enum DebuggerStatus
{
	debugger_running = 0,
	debugger_pause,
	debugger_breakpoint_hit,
	debugger_run_after_breakpoint_hit,
	debugger_not_running,

	debugger_attached, // reserved
	debugger_not_attached // reserved
};

enum Registers
{
	V0 = 0,
	V1,
	V2,
	V3,
	V4,
	V5,
	V6,
	V7,
	V8,
	V9,
	V10,
	V11,
	V12,
	V13,
	V14,
	V15,
	I,
	ST,
	DT,
	PC,
	SP
};

class Debugger
{
public:
	inline void attach(VM* vmptr) { vm = vmptr; }
	inline void deattach() { vm = nullptr; }
	
	void run();
	inline void reset() { vm->reset_and_loadrom(); }


	inline int get_status() { return debugger_status; }
	inline void set_status(int status) { debugger_status = status; }
	std::string get_status_str();

	uint16_t get_value_of_register(int Register);
	void set_value_of_register(int Register, uint16_t value);
	inline uint8_t get_value_from_memory(uint16_t address) { return vm->memory[address]; }


	void SingleStep();
	inline void StepInto() { vm->cycle(); }

	void AddBreakpoint(int addr);
	void RemoveBreakpoint(int addr);
	
	std::vector<int> BreakpointList;
	int debugger_status = DebuggerStatus::debugger_not_running;

private:
	VM* vm;
};

#endif
