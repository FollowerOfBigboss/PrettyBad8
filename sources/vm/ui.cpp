#include "ui.h"

namespace EmuUi
{
	Debugger EDebugger;
	VM vm;

	bool ShowDebugger = false;
	bool ShowGraphicsDebugger = false;
	bool ShowStackView = false;
	bool ShowKeyView = false;
	bool ShouldDrawMenuBar = true;
	bool RomLoaded = false;
	bool DrawFile = false;
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
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug"))
			{
				if (RomLoaded == true)
				{
					ImGui::MenuItem("Debugger", 0, &ShowDebugger);
					ImGui::MenuItem("Graphics Debugger", 0, &ShowGraphicsDebugger);
					ImGui::MenuItem("Stack View", 0, &ShowStackView);
					ImGui::MenuItem("Key View", 0, &ShowKeyView);
				}
				else
				{
					ImGui::MenuItem("Debugger", 0);
					ImGui::MenuItem("Graphics Debugger", 0);
					ImGui::MenuItem("Stack View", 0);
					ImGui::MenuItem("Key View", 0);
				}


				ImGui::EndMenu();
			}


			ImGui::EndMainMenuBar();
		}

	}

}

void EmuUi::DrawDebuggerStuf()
{
	if (ShowGraphicsDebugger)
	{
		EDebugger.DrawGraphicsDebugger();
	}

	if (ShowStackView)
	{
		EDebugger.GetStackInformation();
		EDebugger.DrawStack();
	}

	if (ShowKeyView)
	{
		EDebugger.GetKeyInformation();
		EDebugger.DrawKey();
	}


	if (ShowDebugger)
	{
		ImGui::Begin("Debugger");
		EDebugger.HandleAndDrawDebuggerInput();
		EDebugger.GetRegisterInformations();
		EDebugger.DrawRegisters();
		EDebugger.DrawDissassembly();
		EDebugger.ApplyChangedInformation();
		ImGui::End();
	}
}

void EmuUi::EmuLoop()
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

				EDebugger.attach(vm);
				RomLoaded = true;
			}

			ImGuiFileDialog::Instance()->Close();
			DrawFile = false;
		}
	}


	if (RomLoaded)
	{
		if (ShowDebugger == true)
		{
			EDebugger.RunUntilBreakpoint();
		}
		else
		{
			int a = hztocycles(500);
			for (int i = 0; i < a; i++)
			{
				vm.cycle();
			}
		}
	}
}