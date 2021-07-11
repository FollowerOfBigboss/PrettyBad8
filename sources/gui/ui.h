#include "imgui/imgui.h"
#include "imgui/ImGuiFileDialog.h"

// #include "debugger.h"

#include "../vm/debugger.h"
#include "../gui/debugger_ui.h"
#include "../render/simple_renderer.h"

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

	extern bool ShowSettings;

	extern bool Vsync;

	void Init();
	void DrawMenuBar();
	void DrawDebuggerStuf();
	void DrawSettingsWindow(bool* open);

	void DrawOtherWindows();

	void EmuLoop();
	void EmuDraw();
}
