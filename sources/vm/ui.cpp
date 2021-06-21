#include "ui.h"

namespace EmuUi
{
	Debugger EDebugger;
	DebuggerUi DbgUi;
	VM vm;

	CRenderQuads drquads;

	bool ShowCpuDebugger = false;
	bool ShowGraphicsDebugger = false;
	bool ShowStackView = false;
	bool ShowKeyView = false;
	bool ShouldDrawMenuBar = true;
	bool RomLoaded = false;
	bool DrawFile = false;
}


void EmuUi::Init()
{
	drquads.init();
	EDebugger.attach(&vm);
	DbgUi.attach(&EDebugger);
}

void EmuUi::DrawMenuBar()
{
	if (ShouldDrawMenuBar)
	{

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Open", 0, &DrawFile);

				if (ImGui::MenuItem("Close", 0))
				{
					
					if (RomLoaded == true)
					{
						RomLoaded = false;
						vm.reset();
					}

				}

				ImGui::EndMenu();
			}


			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::EndMenu();
			}

// 			if (ImGui::BeginMenu("Debug"))
// 			{
// 				ImGui::MenuItem("Debugger", 0, &ShowCpuDebugger);
// 				ImGui::MenuItem("Graphics Debugger", 0, &ShowGraphicsDebugger);
// 				ImGui::MenuItem("Stack View", 0, &ShowStackView);
// 				ImGui::MenuItem("Key View", 0, &ShowKeyView);
// 				ImGui::EndMenu();
// 			}


			ImGui::EndMainMenuBar();
		}

	}

}

void EmuUi::DrawDebuggerStuf()
{
// 	if (ShowGraphicsDebugger)
//	{
//		EDebugger.DrawGraphicsDebugger();
//	}
// 
// 	if (ShowStackView)
// 	{
// 		EDebugger.GetStackInformation();
// 		EDebugger.DrawStack();
// 	}
// 
// 	if (ShowKeyView)
//	{
//		EDebugger.GetKeyInformation();
//		EDebugger.DrawKey();
//	}
// 
// 
// 	if (ShowCpuDebugger)
// 	{
// 		ImGui::Begin("Debugger");
// 		EDebugger.HandleAndDrawDebuggerInput();
// 		EDebugger.GetRegisterInformations();
// 		EDebugger.DrawRegisters();
// 
// 		EDebugger.DrawDissassembly();
// 		EDebugger.ApplyChangedInformation();
// 		ImGui::End();
// 	}

	DbgUi.draw();
}

void EmuUi::EmuLoop()
{
	if (RomLoaded)
	{
		EDebugger.run();
	// 	// if (ShowCpuDebugger == true && RomLoaded == true)
	// 	if (RomLoaded == true)
	// 	{
	// 		EDebugger.run();
	// 	}
	// 	else
	// 	{
	// 		vm.run(500);
	// 	}
	}
}

void EmuUi::EmuDraw()
{
	if (DrawFile)
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".ch8,.*", ".");

		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

				vm.init();
				vm.loadrom(filePathName);
				RomLoaded = true;
				EDebugger.set_status(DebuggerStatus::debugger_running);
			}

			ImGuiFileDialog::Instance()->Close();
			DrawFile = false;
		}
	}

	DrawMenuBar();
	DrawDebuggerStuf();

	EmuLoop();
	drquads.update(EmuUi::vm);
}
