#include "config_parser.h"
#include "../emu/emu.h"

void Config::init(Emu& emu)
{
	pemu = &emu;
}

void Config::OpenConfig(const std::string& cfgFilePath)
{

	FILE* fs = fopen(cfgFilePath.c_str(), "rb");
	fseek(fs, 0, SEEK_END);
	long fsize = ftell(fs);
	rewind(fs);

	char* mem = new char[fsize + 1];
	fread(mem, 1, fsize, fs);
	mem[fsize] = '\0';
	buf = mem;
	buf.erase(std::remove(buf.begin(), buf.end(), '\r'), buf.end());
	fclose(fs);
	delete[] mem;

}

void Config::ParseConfig()
{
	std::stringstream sstream(buf);
	std::string tmpstr;
	std::vector<std::string> strvec;

	std::string entry;

	while (std::getline(sstream, tmpstr))
	{
		if (tmpstr[0] == '#')
		{
			continue;
		}

		strvec.push_back(tmpstr);
	}

	bool pfound = false;
	void* ptr;

	const std::array<EntryContainer, 2> EC
	{
		EntryContainer{"General", &GeneralParse},
		EntryContainer{"Controller", &ControllerParse}
	};

	for (auto i : strvec)
	{
		// New Entry
		if (i[0] == '[')
		{
			entry.clear();
			entry += i;
			for (auto it : i)
			{
				if (it == ']')
				{
					pfound = true;
				}

			}

			if (pfound == false)
			{
				throw std::runtime_error("] not found!");
			}

			for (int it = 0; it < entry.size(); it++)
			{
				if (entry[it] == '[')
				{
					entry.erase(it, 1);
				}
				if (entry[it] == ']')
				{
					entry.erase(it, 1);
				}

			}
			continue;
		}
		for (auto it : EC)
		{
			if (it.entryname == entry)
			{
				ptr = it.pentryfn;
				((void(*)(Config & cfg, const std::string&))ptr)(*this, i);
				break;
			}
		}
	}


}

void Config::WriteToConfig(const std::string& cfgFilePath)
{
	FILE* fs = fopen(cfgFilePath.c_str(), "wb");
	std::string cfgbuf;
	cfgbuf.append("[General]\n");
	cfgbuf.append("vsync=" + BoolToStr(pemu->b_Vsync) + "\n");
	cfgbuf.append("clockspeed=" + std::to_string(pemu->clockspeed) + "\n");

	cfgbuf.append("[Controller]\n");
	constexpr std::array<char, 16> ctable = {
		'1', '2', '3', '4',
		'Q', 'W', 'E', 'R',
		'A', 'S', 'D', 'F',
		'Z', 'X', 'C', 'V'
	};

	for (int i = 0; i < 16; i++)
	{
		std::string k = "KEY_";
		k.append(1, ctable[i]);
		cfgbuf.append(k + "=" + std::to_string(pemu->keymap[i]) + "\n");
	}

	fwrite(cfgbuf.c_str(), 1, cfgbuf.size(), fs);
	fclose(fs);
}

bool StrToBool(const std::string& str)
{
	if (str == "true")
		return true;
	if (str == "false")
		return false;

	throw std::runtime_error("Conversion failed!");
}

std::string BoolToStr(bool val)
{
	if (val == true)
		return "true";
	if (val == false)
		return "false";
	throw std::runtime_error("Conversion failed!");
}

void GeneralParse(Config& cfg, const std::string& str)
{
	auto p1 = str.substr(0, str.find("="));
	auto p2 = str.substr(str.find("=") + 1);

	if (p1 == "vsync")
	{
		cfg.pemu->b_Vsync = StrToBool(p2);
	}

	if (p1 == "clockspeed")
	{
		cfg.pemu->clockspeed = atoi(p2.c_str());
	}

	if (p1 == "debug")
	{
		cfg.pemu->b_Debug = StrToBool(p2);
	}
}

void ControllerParse(Config& cfg, const std::string& str)
{
	auto p1 = str.substr(0, str.find("_"));
	auto p2 = str.substr(str.find("_") + 1, 1);
	auto p3 = str.substr(str.find("=") + 1);

	constexpr std::array<int, 16> table = {
		(int)'1', (int)'2', (int)'3', (int)'4',
		(int)'Q', (int)'W', (int)'E', (int)'R',
		(int)'A', (int)'S', (int)'D', (int)'F',
		(int)'Z', (int)'X', (int)'C', (int)'V'
	};

	if (p1 == "KEY")
	{
		auto f = std::distance(table.begin(), std::find(table.begin(), table.end(), (int)p2[0]));
		cfg.pemu->keymap[f] = atoi(p3.c_str());
	}

	if (p1 == "GAMEPAD")
	{
		auto f = std::distance(table.begin(), std::find(table.begin(), table.end(), (int)p2[0]));
		cfg.pemu->contmap[f].maploc = f;
		cfg.pemu->contmap[f].key = atoi(p3.c_str());

	}
}
