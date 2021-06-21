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

	void attach(VM* vmptr)
	{
		vm = vmptr;
	}

	void deattach()
	{
		vm = nullptr;
	}

	void SingleStep()
	{

	}

	void StepInto()
	{

	}
	
	void AddBreakpoint(int addr)
	{
		if (std::find(BreakpointList.begin(), BreakpointList.end(), addr) == BreakpointList.end())
		{
			BreakpointList.push_back(addr);
		}
	}

	void RemoveBreakpoint(int addr)
	{
		for (int i = 0; i < BreakpointList.size(); i++)
		{
			if (BreakpointList[i] == addr)
			{
				BreakpointList.erase(BreakpointList.begin() + i);
			}
		}
	}

	void run()
	{
		// this flag set by outside
		if (debugger_status == DebuggerStatus::debugger_run_after_breakpoint_hit)
		{
			vm->cycle();

			// Expired one time only ticket
			debugger_status = DebuggerStatus::debugger_running;
		}

		if (debugger_status == DebuggerStatus::debugger_running)
		{
			int a = hztocycles(500);
		
			for (int i = 0; i < a; i++)
			{
				if (std::find(BreakpointList.begin(), BreakpointList.end(), vm->PC) != BreakpointList.end())
				{
					debugger_status = DebuggerStatus::debugger_breakpoint_hit;
				}
				else
				{
					vm->cycle();
				}
			}
		}

	}
	
	void set_value_of_register(int Register, uint16_t value)
	{
		if (Register > 15)
		{
			switch (Register)
			{
			case Registers::I: vm->I = value; break;
			case Registers::ST: vm->ST = (uint8_t)value; break;
			case Registers::DT: vm->DT = (uint8_t)value; break;
			case Registers::PC: vm->PC = value; break; 
			case Registers::SP: vm->SP = (uint8_t)value; break;
			}
		}
		else
		{
			vm->V[Register] = value;
		}
	}

	std::string get_status_str()
	{
		switch (debugger_status)
		{
			case DebuggerStatus::debugger_running: return "Debugger Running";
			case DebuggerStatus::debugger_pause: return "Debugger Pause";
			case DebuggerStatus::debugger_breakpoint_hit: return "Debugger Breakpoint Hit";
			case DebuggerStatus::debugger_run_after_breakpoint_hit: return "Debugger Run After Breakpoint Hit";
			case DebuggerStatus::debugger_not_running: return "Debugger Not Running";
			default: return "Unknown";
		}
	}
	int get_status()
	{
		return debugger_status;
	}
	void set_status(int status)
	{
		debugger_status = status;
	}

	std::vector<int> BreakpointList;
	int debugger_status = DebuggerStatus::debugger_not_running;

private:
	VM* vm;
};

#endif
