#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <array>

struct EntryContainer
{
	std::string entryname;
	void* pentryfn;
};

bool StrToBool(const std::string& str);
struct Config;
void GeneralParse(Config& cfg, const std::string& str);
void ControllerParse(Config& cfg, const std::string& str);

struct Config
{
	Emu* pemu;
	Config(Emu& emu)
	{
		pemu = &emu;
	}

	void ParseConfig(std::string& str)
	{
		std::stringstream sstream(str);
		std::string tmpstr;
		std::vector<std::string> strvec;

		std::string entry;

		while (std::getline(sstream, tmpstr))
		{
			if (tmpstr[0] == '#')
				continue;

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
};

void GeneralParse(Config& cfg, const std::string& str)
{
	auto p1 = str.substr(0, str.find("="));
	auto p2 = str.substr(str.find("=")+1);

	if (p1 == "vsync")
	{
		cfg.pemu->Vsync = StrToBool(p2);
	}

	if (p1 == "clockspeed")
	{
		cfg.pemu->clockspeed = atoi(p2.c_str());
	}
}


bool StrToBool(const std::string& str)
{
	if (str == "true")
		return true;
	if (str == "false")
		return false;

	throw std::runtime_error("This datatype is not acceptable!");
}

void ControllerParse(Config& cfg, const std::string& str)
{
	auto p1 = str.substr(0, str.find("_"));
	auto p2 = str.substr(str.find("_") + 1, 1);
	auto p3 = str.substr(str.find("=") + 1);

	if (p1 == "KEY")
	{
		constexpr std::array<int, 16> table = {
			(int)'1', (int)'2', (int)'3', (int)'4',
			(int)'Q', (int)'W', (int)'E', (int)'R',
			(int)'A', (int)'S', (int)'D', (int)'F',
			(int)'Z', (int)'X', (int)'C', (int)'V'
		};

		auto f = std::distance(table.begin(), std::find(table.begin(), table.end(), (int)p2[0]));	
		cfg.pemu->keymap[f] = atoi(p3.c_str());

	}
}
