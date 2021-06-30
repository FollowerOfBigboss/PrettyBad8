#include "debugger_ui.h"


DebuggerUi::DebuggerUi()
{
	for (int i = 0; i < 1793; i++)
	{
		ininfo.push_back({ "00 00","???" });
	}

	memset(&TemporaryV, 0, sizeof(TemporaryV));
	TemporaryPC = 0;
	TemporaryI = 0;
	TemporaryST = 0;
	TemporaryDT = 0;
	TemporarySP = 0;
}


void DebuggerUi::UpdateDebuggerTemporaryValues()
{
	for (int i = 0; i < 16; i++)
	{
		TemporaryV[i] = debugger->get_value_of_register(i);
	}

	TemporaryPC = debugger->get_value_of_register(Registers::PC);
	TemporaryI = debugger->get_value_of_register(Registers::I);
	TemporaryST = debugger->get_value_of_register(Registers::ST);
	TemporaryDT = debugger->get_value_of_register(Registers::DT);
	TemporarySP = debugger->get_value_of_register(Registers::SP);
}

void DebuggerUi::ApplyChanged()
{

	int tmp;
	if (debugger->debugger_status == DebuggerStatus::debugger_pause)
	{
		for (int i = 0; i < 16; i++)
		{
			tmp = debugger->get_value_of_register(i);
			if (TemporaryV[i] != tmp)
			{
				debugger->set_value_of_register(i, TemporaryV[i]);
			}
		}


		tmp = debugger->get_value_of_register(Registers::PC);
		if (TemporaryPC != tmp)
		{
			debugger->set_value_of_register(Registers::PC, TemporaryPC);
		}

		tmp = debugger->get_value_of_register(Registers::I);
		if (TemporaryI != tmp)
		{
			debugger->set_value_of_register(Registers::I, TemporaryI);
		}

		tmp = debugger->get_value_of_register(Registers::ST);
		if (TemporaryST != tmp)
		{
			debugger->set_value_of_register(Registers::ST, TemporaryST);
		}

		tmp = debugger->get_value_of_register(Registers::DT);
		if (TemporaryDT != tmp)
		{
			debugger->set_value_of_register(Registers::DT, TemporaryDT);
		}

		tmp = debugger->get_value_of_register(Registers::SP);
		if (TemporarySP != tmp)
		{
			debugger->set_value_of_register(Registers::SP, TemporarySP);
		}
	}

}

void DebuggerUi::draw()
{
	DebuggerStatus();

	UpdateDebuggerTemporaryValues();
	DrawCpuDebugger();
	ApplyChanged();
}

void DebuggerUi::DebuggerStatus()
{
	int status = debugger->get_status();
	std::string str_status = debugger->get_status_str();
	ImVec4 ColorVec;

	switch (status)
	{

	case DebuggerStatus::debugger_running:
		ColorVec = ImVec4(0.678f, 1.000f, 0.184f, 1.0f);
		break;

	case DebuggerStatus::debugger_pause:
		ColorVec = ImVec4(1.000f, 1.000f, 1.000f, 1.0f);
		break;

	case DebuggerStatus::debugger_breakpoint_hit:
		ColorVec = ImVec4(1.000f, 0.000f, 0.000f, 1.0f);
		break;

	case DebuggerStatus::debugger_not_running:
		ColorVec = ImVec4(1.000f, 1.000f, 0.000f, 1.0f);
		break;
	}

	ImGui::TextColored(ColorVec, "Debugger Status: %s", str_status.c_str());
}

void DebuggerUi::DrawDisassembly()
{

	if (ImGui::BeginTable("##disassembly", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY))
	{
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableSetupColumn("Address");
		ImGui::TableSetupColumn("Bytes");
		ImGui::TableSetupColumn("Instruction");
		ImGui::TableHeadersRow();

		ImGuiListClipper clipper(1792, ImGui::GetTextLineHeight());

		if (track_pc == true)
		{
			ImGui::SetScrollFromPosY( ( ((debugger->get_value_of_register(Registers::PC) - 512) / 2) * ImGui::GetTextLineHeight()) - ImGui::GetScrollY(), 0.35f);
		}



		while (clipper.Step())
		{

			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				ImGui::TableNextRow();

				if (selected[i] == true)
				{
					ImGui::TableSetBgColor(ImGuiTableBgTarget_::ImGuiTableBgTarget_RowBg0, IM_COL32(255, 0, 0, 255));
				}
				if (debugger->get_value_of_register(Registers::PC) == (512 + (i * 2)))
				{
					ImGui::TableSetBgColor(ImGuiTableBgTarget_::ImGuiTableBgTarget_RowBg0, IM_COL32(50, 205, 50, 255));
				}


				ImGui::TableNextColumn();

				char littlebuf[5] = { 0 };
				ltoa(512 + (i * 2), littlebuf, 10);
				if (ImGui::Selectable(littlebuf, &selected[i], ImGuiSelectableFlags_SpanAllColumns))

						{
							if (selected[i] == false)
							{
								debugger->RemoveBreakpoint(512 + (i * 2));
							}
							else
							{
								debugger->AddBreakpoint(512 + (i * 2));
							}
						}

				ImGui::TableNextColumn();
				ImGui::Text(ininfo[i].bytes.c_str());

				ImGui::TableNextColumn();
				ImGui::Text(ininfo[i].instruction.c_str());

			}

		}

		clipper.End();

		/*
		if (ininfo.size() > 0)
		{
			for (int i = 0; i < 1793; i++)
		{
			ImGui::TableNextRow();

			if (debugger->get_value_of_register(Registers::PC) == (512+(i*2)) && track_pc == true)
			{
				ImGui::SetScrollHereY();
				ImGui::TableSetBgColor(ImGuiTableBgTarget_::ImGuiTableBgTarget_RowBg0, IM_COL32(50, 205, 50, 255));
			}

			if (selected[i] == true)
			{
				ImGui::TableSetBgColor(ImGuiTableBgTarget_::ImGuiTableBgTarget_RowBg0, IM_COL32(255, 0, 0, 255));
			}

			ImGui::TableNextColumn();

			char littlebuf[5] = { 0 };
			ltoa(512 + (i * 2), littlebuf, 10);
			if (ImGui::Selectable(littlebuf, &selected[i], ImGuiSelectableFlags_SpanAllColumns))

			{
				if (selected[i] == false)
				{
					debugger->RemoveBreakpoint(512 + (i * 2));
				}
				else
				{
					debugger->AddBreakpoint(512 + (i * 2));
				}
			}

			ImGui::TableNextColumn();
			ImGui::Text(ininfo[i].bytes.c_str());

			ImGui::TableNextColumn();
			ImGui::Text(ininfo[i].instruction.c_str());
			
		}

		}
		*/
		ImGui::EndTable();
	}
}

void DebuggerUi::DrawBreakPointList()
{
	ImGui::Begin("BreakPoint List");
	if (ImGui::BeginListBox("##BreakpointList", ImVec2(-FLT_MIN, -FLT_MIN)))
	{
		for (int n = 0; n < debugger->BreakpointList.size(); n++)
		{
			const bool is_selected = (IMcurritem == n);			
			
			char littlebuf[5];
			ltoa(debugger->BreakpointList[n], littlebuf, 10);
			
			if (ImGui::Selectable(littlebuf, is_selected))
			{
				IMcurritem = n;
			}

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}

	ImGui::End();
}

InstructionInfo DebuggerUi::GetInstructionInfo(int address)
{
	std::string decoded = "???";
	decoded.resize(20);
	
	uint16_t opcode = (debugger->get_value_from_memory(address) << 8) | (debugger->get_value_from_memory(address + 1));
	std::string tmp;
	tmp.resize(20);
	snprintf(&tmp[0], tmp.size(), "%02x %02x", debugger->get_value_from_memory(address), debugger->get_value_from_memory(address + 1));

	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x00FF)
		{
		case 0x00E0:
			decoded = "CLS";
			break;

		case 0x00EE:
			decoded = "RET";
			break;
		}
		break;

	case 0x1000:
		snprintf(&decoded[0], decoded.size(), "JP 0x%04x", (opcode & 0x0FFF));
		break;

	case 0x2000:
		snprintf(&decoded[0], decoded.size(), "CALL 0x%04x", (opcode & 0x0FFF));
		break;

	case 0x3000:
		snprintf(&decoded[0], decoded.size(), "SE V%i, 0x%02x", ((opcode & 0x0F00) >> 8), (opcode & 0x00FF));
		break;

	case 0x4000:
		snprintf(&decoded[0], decoded.size(), "SNE V%i, 0x%02x", ((opcode & 0x0F00) >> 8), (opcode & 0x00FF));
		break;

	case 0x5000:
		snprintf(&decoded[0], decoded.size(), "SE V%i, V%i", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
		break;

	case 0x6000:
		snprintf(&decoded[0], decoded.size(), "LD V%i, 0x%02x", ((opcode & 0x0F00) >> 8), (opcode & 0x00FF));
		break;

	case 0x7000:
		snprintf(&decoded[0], decoded.size(), "ADD V%i, 0x%02x", ((opcode & 0x0F00) >> 8), (opcode & 0x00FF));
		break;

	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000:
			snprintf(&decoded[0], decoded.size(), "LD V%i, V%i", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;

		case 0x0001:
			snprintf(&decoded[0], decoded.size(), "OR V%i, V%i", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;

		case 0x0002:
			snprintf(&decoded[0], decoded.size(), "AND V%i, V%i", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;

		case 0x0003:
			snprintf(&decoded[0], decoded.size(), "XOR V%i, V%i", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;

		case 0x0004:
			snprintf(&decoded[0], decoded.size(), "ADD V%i, V%i", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;

		case 0x0005:
			snprintf(&decoded[0], decoded.size(), "SUB V%i, V%i", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;

		case 0x0006:
			snprintf(&decoded[0], decoded.size(), "SHR V%i {, V%i}", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;

		case 0x0007:
			snprintf(&decoded[0], decoded.size(), "SUBN V%i, V%i}", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;

		case 0x000E:
			snprintf(&decoded[0], decoded.size(), "SHL V%i {, V%i}", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;
		}
		break;

	case 0x9000:
		snprintf(&decoded[0], decoded.size(), "SNE V%i, V%i", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
		break;

	case 0xA000:
		snprintf(&decoded[0], decoded.size(), "LD I, 0x%04x", (opcode & 0x0FFF));
		break;

	case 0xB000:
		snprintf(&decoded[0], decoded.size(), "JP V0, 0x%04x", (opcode & 0x0FFF));
		break;

	case 0xC000:
		snprintf(&decoded[0], decoded.size(), "RND V%i, 0x%02x", ((opcode & 0x0F00) >> 8), (opcode & 0x00FF));
		break;

	case 0xD000:
		snprintf(&decoded[0], decoded.size(), "DRW V%i, V%i, %i", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), (opcode & 0x000F));
		break;

	case 0xE000:
		switch (opcode & 0x00FF)
		{
		case 0x009E:
			snprintf(&decoded[0], decoded.size(), "SKP V%i", ((opcode & 0x0F00) >> 8));
			break;

		case 0x00A1:
			snprintf(&decoded[0], decoded.size(), "SKNP V%i", ((opcode & 0x0F00) >> 8));
			break;
		}
		break;

	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007:
			snprintf(&decoded[0], decoded.size(), "LD V%i, DT", ((opcode & 0x0F00) >> 8));
			break;

		case 0x000A:
			snprintf(&decoded[0], decoded.size(), "LD V%i, K", ((opcode & 0x0F00) >> 8));
			break;

		case 0x0015:
			snprintf(&decoded[0], decoded.size(), "LD DT, V%i", ((opcode & 0x0F00) >> 8));
			break;

		case 0x0018:
			snprintf(&decoded[0], decoded.size(), "LD ST, V%i", ((opcode & 0x0F00) >> 8));
			break;

		case 0x001E:
			snprintf(&decoded[0], decoded.size(), "ADD I, V%i", ((opcode & 0x0F00) >> 8));
			break;

		case 0x0029:
			snprintf(&decoded[0], decoded.size(), "LD F, V%i", ((opcode & 0x0F00) >> 8));
			break;

		case 0x0033:
			snprintf(&decoded[0], decoded.size(), "LD B, V%i", ((opcode & 0x0F00) >> 8));
			break;

		case 0x0055:
			snprintf(&decoded[0], decoded.size(), "LD [I], V%i", ((opcode & 0x0F00) >> 8));
			break;

		case 0x0065:
			snprintf(&decoded[0], decoded.size(), "LD V%i, [I]", ((opcode & 0x0F00) >> 8));
			break;
		}
		break;
	}

	return {tmp, decoded};
}

void DebuggerUi::GetDisassembly()
{
	for (int i = 0; i < 1793; i++)
	{
		ininfo[i] = GetInstructionInfo(512 + (i * 2));
	}
}

void DebuggerUi::DrawCpuDebugger()
{
	ImGui::Begin("CPU Debugger", 0, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::MenuItem("Stop After Reset", 0, &bStopAfterReset);
			ImGui::MenuItem("Track PC", 0, &track_pc);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Show Disassembler", 0, &bShowDisassembler);
			ImGui::MenuItem("Show Breakpoint List", 0, &bShowBreakpointList);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}


	ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 205, 50, 255));
	if (ImGui::Button("Resume"))
	{
		if (debugger->debugger_status != DebuggerStatus::debugger_not_running)
		{
			if (debugger->debugger_status == DebuggerStatus::debugger_breakpoint_hit)
			{
				debugger->set_status(DebuggerStatus::debugger_run_after_breakpoint_hit);
			}
			else
			{
				debugger->set_status(DebuggerStatus::debugger_running);
			}
		}
	}

	ImGui::PopStyleColor();

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 0, 0, 255));
	if (ImGui::Button("Pause"))
	{
		if (debugger->debugger_status != DebuggerStatus::debugger_not_running)
		{
			debugger->set_status(DebuggerStatus::debugger_pause);
		}
	}

	ImGui::PopStyleColor();

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		if (debugger->debugger_status != DebuggerStatus::debugger_not_running)
		{
			if (bStopAfterReset == true)
			{
				debugger->set_status(DebuggerStatus::debugger_pause);
			}

			debugger->reset();
			UpdateDebuggerTemporaryValues();
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Single Step"))
	{
		if (debugger->debugger_status != DebuggerStatus::debugger_not_running)
		{
			if (debugger->debugger_status == DebuggerStatus::debugger_running)
			{
				debugger->set_status(DebuggerStatus::debugger_pause);
			}


			if (debugger->debugger_status != DebuggerStatus::debugger_running)
			{
				debugger->SingleStep();
				UpdateDebuggerTemporaryValues();
			}
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Step Into"))
	{
		if (debugger->debugger_status == DebuggerStatus::debugger_running)
		{
			debugger->set_status(DebuggerStatus::debugger_pause);
		}

		if (debugger->debugger_status != DebuggerStatus::debugger_running)
		{
			debugger->StepInto();
			UpdateDebuggerTemporaryValues();
		}
	}

	for (int i = 0; i < 16; i++)
	{
		ImGui::Text("V%i", i);
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
		ImGui::InputInt(templabelsV[i], &TemporaryV[i], 0);

		if (!((i + 1) % 4 == 0))
		{
			ImGui::SameLine();
		}

	}

	ImGui::Text("PC");
	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
	ImGui::InputInt("##TemporaryPC", &TemporaryPC, 0);

	ImGui::SameLine();

	ImGui::Text("I");
	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
	ImGui::InputInt("##TemporaryI", &TemporaryI, 0);

	ImGui::SameLine();

	ImGui::Text("ST");
	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
	ImGui::InputInt("##TemporaryST", &TemporaryST, 0);

	ImGui::SameLine();

	ImGui::Text("DT");
	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
	ImGui::InputInt("##TemporaryDT", &TemporaryDT, 0);

	ImGui::Text("SP");
	ImGui::SameLine();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
	ImGui::InputInt("##TemporarySP", &TemporarySP, 0);


	if (bShowDisassembler == true)
	{
		DrawDisassembly();
	}

	ImGui::End();

	if (bShowBreakpointList == true)
	{
		DrawBreakPointList();
	}

}