#include "imgui/imgui.h"
#include "imgui/ImGuiFileDialog.h"

// #include "debugger.h"

#include "debugger_n.h"
#include "../gui/debugger_ui.h"
#include "draw.h"

namespace EmuUi
{
	extern DebuggerUi DbgUi;
	extern Debugger EDebugger;
	extern VM vm;

	extern bool ShowCpuDebugger;
	extern bool ShowGraphicsDebugger;
	extern bool ShowStackView;
	extern bool ShowKeyView;
	extern bool ShouldDrawMenuBar;

	extern bool DrawFile;
	extern bool RomLoaded;
	extern CRenderQuads drquads;

	void Init();
	void DrawMenuBar();
	void DrawDebuggerStuf();
	void EmuLoop();

	void EmuDraw();
}
