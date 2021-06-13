#include "imgui/imgui.h"

#include "debugger.h"

namespace EmuUi
{
	extern Debugger EDebugger;
	extern VM vm;

	extern bool ShowDebugger;
	extern bool ShowGraphicsDebugger;
	extern bool ShowStackView;
	extern bool ShowKeyView;

	extern bool ShouldDrawMenuBar;

	void DrawMenuBar();
	void DrawDebuggerStuf();
	void EmuLoop();
}
