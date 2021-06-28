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

	void attach(VM* vmptr);
	void deattach();
	
	void SingleStep();
	void StepInto();

	void AddBreakpoint(int addr);
	void RemoveBreakpoint(int addr);

	void run();
	void reset();

	void set_value_of_register(int Register, uint16_t value);
	uint16_t get_value_of_register(int Register);

	int get_status();
	void set_status(int status);
	std::string get_status_str();

	uint8_t get_value_from_memory(uint16_t address);

	std::vector<int> BreakpointList;
	int debugger_status = DebuggerStatus::debugger_not_running;

	

private:
	VM* vm;
};

#endif
