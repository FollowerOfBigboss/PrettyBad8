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

	bool DrawFile = false;
}


void EmuUi::DrawMenuBar()
{
	if (ShouldDrawMenuBar)
	{

		if (ImGui::BeginMainMenuBar())
		{
			// https://github.com/aiekick/ImGuiFileDialog
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Open", 0, &DrawFile);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug"))
			{

				ImGui::MenuItem("Debugger", 0, &ShowDebugger);
				ImGui::MenuItem("Graphics Debugger", 0, &ShowGraphicsDebugger);
				ImGui::MenuItem("Stack View", 0, &ShowStackView);
				ImGui::MenuItem("Key View", 0, &ShowKeyView);

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

	if (DrawFile)
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".ch8,.*", ".");
	}

	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
		}

		ImGuiFileDialog::Instance()->Close();
	}

}

void EmuUi::EmuLoop()
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