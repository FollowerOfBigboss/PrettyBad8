#include "ui.h"
#include <GLFW/glfw3.h>

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
	bool ShowSettings = false;
	bool Vsync = true;
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
				ImGui::MenuItem("Settings", 0, &ShowSettings);
				ImGui::EndMenu();
			}

 			if (ImGui::BeginMenu("Debug"))
 			{
 				ImGui::MenuItem("CPU Debugger", 0, &ShowCpuDebugger);
 				ImGui::MenuItem("Graphics Debugger", 0, &ShowGraphicsDebugger);
 				ImGui::MenuItem("Stack View", 0, &ShowStackView);
 				ImGui::MenuItem("Key View", 0, &ShowKeyView);
 				ImGui::EndMenu();
 			}

			DbgUi.DrawDebuggerStatus();
			ImGui::EndMainMenuBar();
		}

	}

}

void EmuUi::DrawDebuggerStuf()
{
	if (ShowGraphicsDebugger)
	{
		DbgUi.DrawGraphicsDebugger(&ShowGraphicsDebugger);
	}

	if (ShowStackView)
 	{
		DbgUi.DrawStack(&ShowStackView);
	}

	if (ShowKeyView)
	{
		DbgUi.DrawKey(&ShowKeyView);
	}

	if (ShowCpuDebugger)
	{
		DbgUi.DrawCpuDebugger(&ShowCpuDebugger);
	}

}

void EmuUi::DrawSettingsWindow(bool* open)
{
	ImGui::Begin("Settings", open);

	if (ImGui::Checkbox("Vsync", &Vsync))
	{
		glfwSwapInterval((int)Vsync);
	}

	ImGui::End();
}

void EmuUi::DrawOtherWindows()
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
				DbgUi.GetDisassembly();
			}

			ImGuiFileDialog::Instance()->Close();
			DrawFile = false;
		}
	}

	if (ShowSettings)
	{
		DrawSettingsWindow(&ShowSettings);
	}
}

void EmuUi::EmuLoop()
{
	if ( RomLoaded && (ShowCpuDebugger == true || ShowGraphicsDebugger == true || ShowKeyView == true || ShowStackView == true) )
	{
		EDebugger.run();
	}
	else if (RomLoaded)
	{
		vm.run(500);
	}
	else
	{
		// std::cout << "Wait till rom loads" << "\n";
	}

}

void EmuUi::EmuDraw()
{

	DrawOtherWindows();
	DrawMenuBar();
	DrawDebuggerStuf();

	EmuLoop();
	drquads.update(EmuUi::vm);
}
