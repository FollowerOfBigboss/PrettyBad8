#include "dynarec.h"

bool AllocateDynarecMem(CodeBlock* block, uint32_t size)
{
	void* memptr = nullptr;
#	ifdef _WIN32

	memptr = VirtualAlloc(NULL, size, (MEM_COMMIT | MEM_RESERVE), PAGE_EXECUTE_READWRITE);
	if (memptr == NULL)
	{
		return false;
	}

	block->code = memptr;
	block->size = size;

	return true;
#	endif
	return false;
}

bool DeAllocateDynarecMem(CodeBlock* block)
{
#	ifdef _WIN32
	int ret = VirtualFree(block->code, 0, MEM_RELEASE);
	if (ret != 0)
	{
		return false;
	}
	return true;
#	endif
	
	return false;
}
