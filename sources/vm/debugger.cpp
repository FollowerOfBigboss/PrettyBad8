#include "debugger.h"

void Debugger::run(int freq)
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
		int neededcycles = 0;
		neededcycles = hztocycles(freq);

		for (int i = 0; i < neededcycles; i++)
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

std::string Debugger::get_status_str()
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

uint16_t Debugger::get_value_of_register(int Register)
{
	if (Register > 15)
	{
		switch (Register)
		{
		case Registers::I: return vm->I;
		case Registers::ST: return vm->ST;
		case Registers::DT: return vm->DT;
		case Registers::PC: return vm->PC;
		case Registers::SP: return vm->SP;
		}
	}
	else
	{
		return vm->V[Register];
	}

	return 21;
}

void Debugger::set_value_of_register(int Register, uint16_t value)
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
		vm->V[Register] = (uint8_t)value;
	}
}

void Debugger::SingleStep()
{

	bool IsItCallOpcode = (((vm->memory[vm->PC] << 8 | vm->memory[vm->PC + 1]) & 0xF000) == 0x2000);

	bool RunUntilReturn = true;
	int CyclesThatNeedsToBeExecuted = 0;

	int FakePC = ((vm->memory[vm->PC] << 8 | vm->memory[vm->PC + 1]) & 0x0FFF);

	if (IsItCallOpcode == true)
	{
		while (RunUntilReturn)
		{
			if (((vm->memory[FakePC] << 8 | vm->memory[FakePC + 1]) & 0xF0FF) == 0x00EE)
			{
				CyclesThatNeedsToBeExecuted++;
				RunUntilReturn = false;
				break;
			}
			FakePC += 2;
			CyclesThatNeedsToBeExecuted++;
		}
		CyclesThatNeedsToBeExecuted++;


		for (int i = 0; i < CyclesThatNeedsToBeExecuted; i++)
		{
			vm->cycle();
		}

		return;
	}

	vm->cycle();

	return;
}

void Debugger::AddBreakpoint(int addr)
{
	if (std::find(BreakpointList.begin(), BreakpointList.end(), addr) == BreakpointList.end())
	{
		BreakpointList.push_back(addr);
	}
}

void Debugger::RemoveBreakpoint(int addr)
{
	for (int i = 0; i < BreakpointList.size(); i++)
	{
		if (BreakpointList[i] == addr)
		{
			BreakpointList.erase(BreakpointList.begin() + i);
		}
	}
}
