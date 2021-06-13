#include "debugger.h"
#include "imgui/imgui_internal.h"


void Debugger::attach(VM& vmInfo)
{
    vm_ptr = &vmInfo;
}

void Debugger::deattach()
{
    vm_ptr = nullptr;

    insData.clear();
    
    memset(&TemporaryV, 0, sizeof(TemporaryV));
    TemporaryI = 0;
    TemporaryST = 0;
    TemporaryDT = 0;
    TemporaryPC = 0;
    TemporarySP = 0;
    memset(&TemporaryStack, 0, sizeof(TemporaryStack));


    Pause = false;
    BreakpointContinue = false;
    BreakPointHit = false;

    StopAfterReset = false;
}

void Debugger::SingleStep()
{
    bool IsItCallOpcode = (((vm_ptr->memory[vm_ptr->PC] << 8 | vm_ptr->memory[vm_ptr->PC + 1]) & 0xF000) == 0x2000);

    bool RunUntilReturn = true;
    int CyclesThatNeedsToBeExecuted = 0;

    int FakePC = ((vm_ptr->memory[vm_ptr->PC] << 8 | vm_ptr->memory[vm_ptr->PC + 1]) & 0x0FFF);

    if (IsItCallOpcode == true)
    {
        while (RunUntilReturn)
        {
            if (((vm_ptr->memory[FakePC] << 8 | vm_ptr->memory[FakePC + 1]) & 0xF0FF) == 0x00EE)
            {
                CyclesThatNeedsToBeExecuted++;
                RunUntilReturn = false;
                break;
            }
            FakePC += 2;
            CyclesThatNeedsToBeExecuted++;
        }
        CyclesThatNeedsToBeExecuted++;


        for (int i = 0; i < CyclesThatNeedsToBeExecuted; i++)
        {
            vm_ptr->cycle();
        }

        return;
    }

    vm_ptr->cycle();
}

void Debugger::StepInto()
{
    vm_ptr->cycle();
}

void Debugger::RunUntilBreakpoint()
{
    if (Pause == false)
    {
        int a = hztocycles(500);
        bool sBreak = false;

        for (int i = 0; i < a; i++)
        {

            for (int i = 0; i < insData.size(); i++)
            {
                if (vm_ptr->PC == std::stoi(insData[i].address) && insData[i].selected == true)
                {

                    sBreak = true;
                    BreakPointHit = true;

                    if (BreakpointContinue)
                    {
                        sBreak = false;
                    }
                    else
                    {
                        sBreak = true;
                        Pause = true;
                    }

                }

            }
            if (sBreak == false)
            {
                vm_ptr->cycle();
                BreakpointContinue = false;
            }
        }
    }
}

void Debugger::GetDissassembly()
{
    std::string tmp;
    std::string tmp2;
    tmp.resize(5);
    tmp2.resize(20);

    for (int i = 512; i < 4096; i+=2)
    {
        // Use them till c++20
        snprintf(&tmp[0], tmp.size(), "%i", i);
        snprintf(&tmp2[0], tmp2.size(), "%02x %02x", vm_ptr->memory[i], vm_ptr->memory[i + 1]);
        insData.push_back({ tmp, tmp2, DecodeInstruction(vm_ptr->memory[i] << 8 | vm_ptr->memory[i + 1]), false });
    }
}

void Debugger::GetRegisterInformations()
{
    for (int i = 0; i < 16; i++)
    {
        TemporaryV[i] = vm_ptr->V[i];
    }

    TemporaryPC = vm_ptr->PC;
    TemporaryI = vm_ptr->I;
    TemporaryST = vm_ptr->ST;
    TemporaryDT = vm_ptr->DT;
    TemporarySP = vm_ptr->SP;

}

void Debugger::ApplyChangedInformation()
{
    if (Pause == true)
    {
        for (int i = 0; i < 16; i++)
        {
            if (TemporaryV[i] != vm_ptr->V[i])
                vm_ptr->V[i] = TemporaryV[i];
        }

        if (TemporaryPC != vm_ptr->PC)
        {
            vm_ptr->PC = TemporaryPC;
        }

        if (TemporaryI != vm_ptr->I)
        {
            vm_ptr->I = TemporaryI;
        }

        if (TemporaryST != vm_ptr->ST)
        {
            vm_ptr->ST = TemporaryST;
        }

        if (TemporaryDT != vm_ptr->DT)
        {
            vm_ptr->DT = TemporaryDT;
        }

        if (TemporarySP != vm_ptr->SP)
        {
            vm_ptr->SP = TemporarySP;
        }

    }

}

void Debugger::GetKeyInformation()
{
    if (Pause == false)
    {
        for (int k = 0; k < 16; k++)
        {
            TemporaryKey[k] = vm_ptr->Key[k];
        }
    }
}

void Debugger::GetStackInformation()
{
    for (int j = 0; j < 16; j++)
    {
        TemporaryStack[j] = vm_ptr->stack[j];
    }
}

void Debugger::HandleAndDrawDebuggerInput()
{
    if (ImGui::Button("Resume"))
    {
        Pause = false;

        if (BreakpointContinue != true)
            BreakpointContinue = true;

    }

    ImGui::SameLine();
    if (ImGui::Button("Pause"))
    {
        Pause = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset"))
    {
        Pause = StopAfterReset;
        vm_ptr->reset();
    }

    ImGui::SameLine();
    ImGui::Checkbox("Stop after reset", &StopAfterReset);

    ImGui::SameLine();
    if (ImGui::Button("Single Step"))
    {
        if (Pause == false)
        {
            Pause = true;
        }
        else
        {
            SingleStep();
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Step Into"))
    {
        if (Pause == false)
        {
            Pause = true;
        }
        else
        {
            StepInto();
        }
    }

}

void Debugger::DrawRegisters()
{
    for (int i = 0; i < 16; i++)
    {
        ImGui::Text("V%i", i);
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
        ImGui::InputInt(templabelsV[i], &TemporaryV[i]);
        
        if ( !((i + 1) % 4 == 0) )
        {
            ImGui::SameLine();
        }

    }

    ImGui::Text("PC");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
    ImGui::InputInt("##TemporaryPC", &TemporaryPC);

    ImGui::SameLine();

    ImGui::Text("I");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
    ImGui::InputInt("##TemporaryI", &TemporaryI);

    ImGui::SameLine();

    ImGui::Text("ST");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
    ImGui::InputInt("##TemporaryST", &TemporaryST);

    ImGui::SameLine();

    ImGui::Text("DT");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
    ImGui::InputInt("##TemporaryDT", &TemporaryDT);

    ImGui::Text("SP");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
    ImGui::InputInt("##TemporarySP", &TemporarySP);
}

void Debugger::DrawDissassembly()
{
    
    if (ImGui::BeginTable("##disassembly", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg| ImGuiTableFlags_ScrollY ))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Address");
        ImGui::TableSetupColumn("Bytes");
        ImGui::TableSetupColumn("Instruction");
      
        ImGui::TableHeadersRow();
        
        for (int i = 0; i < insData.size(); i++)
        {
            ImGui::TableNextRow();

            if (insData[i].selected)
            {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_::ImGuiTableBgTarget_RowBg0, IM_COL32(255, 0, 0, 255));
            }

            ImGui::TableNextColumn();
            ImGui::Selectable(insData[i].address.c_str(), &insData[i].selected, ImGuiSelectableFlags_SpanAllColumns);

            ImGui::TableNextColumn();
            ImGui::Text(insData[i].bytes.c_str());

            ImGui::TableNextColumn();
            ImGui::Text(insData[i].decoded_instrucction.c_str());

            if (vm_ptr->PC == std::stoi(insData[i].address) && Pause == true)
            {
                ImGui::TableSetBgColor(ImGuiTableBgTarget_::ImGuiTableBgTarget_RowBg0, IM_COL32(50, 205, 50, 255));
            }
        }

        ImGui::EndTable();
    }
}

void Debugger::DrawGraphicsDebugger()
{
    // Will enhanced and expanded
    ImGui::Begin("Graphics debugger");
    std::string tmp;

    int i = 0;
    while (i < 64*32)
    {
        for (int j = 0; j < 64; j++)
        {
            tmp += std::to_string((int)vm_ptr->gfx[i + j]);
        }
        tmp += "\n";
        i += 64;
    }

    ImGui::Text(tmp.c_str());
    ImGui::End();
}

void Debugger::DrawStack()
{
    ImGui::Begin("Stack");
    std::string t;

    if (ImGui::BeginListBox("##stack", ImVec2(-1, -1)))
    {

        for (int n = 15; n > -1; n--)
        {

            const bool is_selected = (item_current_idx == n);
            t = std::to_string(TemporaryStack[n]);
            t += "##";
            t += std::to_string(n);

            if (ImGui::Selectable(t.c_str(), is_selected))
            {
                item_current_idx = n;
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

void Debugger::DrawKey()
{
    ImGui::Begin("Key", 0, ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);
    std::string ss;
    ImGui::SetWindowSize(ImVec2(245, 250));

    for (int i = 0; i < 16; i++)
     {

        ss = std::to_string(TemporaryKey[i]);

        if (TemporaryKey[i] == 1)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 0, 0, 255));
        }

        ImGui::Button(ss.c_str(), ImVec2(50, 50));

        if (TemporaryKey[i] == 1)
        {
            ImGui::PopStyleColor();
        }

        if ( !((i + 1) % 4 == 0) )
        {
              ImGui::SameLine();
        }

     }

    ImGui::End();
}

std::string Debugger::DecodeInstruction(uint16_t opcode)
{
    std::string decoded = "Unknown";
    decoded.resize(20);

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
        snprintf(&decoded[0], decoded.size(), "SE V%i, 0x%02x", ( (opcode & 0x0F00) >> 8), (opcode & 0x00FF));
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

    return decoded;
}


/*
*
static int MyCallback(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
    {
        memcpy(data->UserData, data->Buf, data->BufSize);
    }
    return 0;
}


std::string t;

	std::string tt[16];

	char b1[50] = { 0 };
	char b2[50] = { 0 };
	char b3[50] = { 0 };

	int VK[16] = { 0 };

    static char temp[10];
    for (int i = 0; i < 16; i++)
    {
        snprintf(temp, 10, "##V%i", i);
        ImGui::Text("V%i", i);
        ImGui::SameLine();
        ImGui::InputText(temp, &tt[i][0], tt[i].size(), ImGuiInputTextFlags_CallbackEdit, MyCallback, (void*)&tt[i][0]);
    }

*/