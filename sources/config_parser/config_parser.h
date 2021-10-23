#ifndef _CONFIG_
#define _CONFIG_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <array>

class Emu;


enum cfgerr : int
{
	unsuccessful = -1,
	created_new = 0,
	successful = 1,
};

struct EntryContainer
{
	std::string entryname;
	void* pentryfn;
};

struct Config
{
	Emu* pemu;
	std::string buf;

	void init(Emu& emu);
	cfgerr OpenConfig(const std::string& cfgFilePath = "emu.cfg");
	void ParseConfig();
	bool WriteToConfig(const std::string& cfgFilePath = "emu.cfg");
};

bool StrToBool(const std::string& str);
std::string BoolToStr(bool val);
void GeneralParse(Config& cfg, const std::string& str);
void ControllerParse(Config& cfg, const std::string& str);

#endif