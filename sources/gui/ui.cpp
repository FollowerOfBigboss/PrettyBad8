#include "ui.h"
#include <GLFW/glfw3.h>

void Emu::Init()
{
	ShowCpuDebugger = false;
	ShowGraphicsDebugger = false;
	ShowStackView = false;
	ShowKeyView = false;
	ShouldDrawMenuBar = true;
	RomLoaded = false;
	DrawFile = false;
	ShowSettings = false;
	Vsync = true;

	gquads.init();
	debugger.attach(&vm);
	gdebugger.attach(&debugger);

}

void Emu::DrawMenuBar()
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

			gdebugger.DrawDebuggerStatus();
			ImGui::EndMainMenuBar();
		}

	}
}


void Emu::DrawDebuggerStuf()
{
	if (ShowGraphicsDebugger)
	{
		gdebugger.DrawGraphicsDebugger(&ShowGraphicsDebugger);
	}
	
	if (ShowStackView)
	{
		gdebugger.DrawStack(&ShowStackView);
	}
	
	if (ShowKeyView)
	{
		gdebugger.DrawKey(&ShowKeyView);
	}
	
	if (ShowCpuDebugger)
	{
		gdebugger.DrawCpuDebugger(&ShowCpuDebugger);
	}	
}

void Emu::DrawSettingsWindow(bool* open)
{
	ImGui::Begin("Settings", open);
	
	if (ImGui::Checkbox("Vsync", &Vsync))
	{
		glfwSwapInterval((int)Vsync);
	}
	
	ImGui::End();
}

void Emu::DrawOtherWindows()
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
				debugger.set_status(DebuggerStatus::debugger_running);
				gdebugger.GetDisassembly();
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

void Emu::EmuLoop()
{
	if (RomLoaded && (ShowCpuDebugger == true || ShowGraphicsDebugger == true || ShowKeyView == true || ShowStackView == true))
	{
		debugger.run();
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

void Emu::EmuDraw()
{
	DrawOtherWindows();
	DrawMenuBar();
	DrawDebuggerStuf();

	EmuLoop();
	gquads.update(vm);
}
