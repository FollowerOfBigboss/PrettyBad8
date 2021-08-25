#ifndef _CONFIG_
#define _CONFIG_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <array>

extern class Emu;

const struct EntryContainer
{
	std::string entryname;
	void* pentryfn;
};

struct Config
{
	Emu* pemu;
	std::string buf;

	void init(Emu& emu);
	void OpenConfig(const std::string& cfgFilePath = "emu.cfg");
	void ParseConfig();
	void WriteToConfig(const std::string& cfgFilePath = "emu.cfg");
};

bool StrToBool(const std::string& str);
std::string BoolToStr(bool val);
void GeneralParse(Config& cfg, const std::string& str);
void ControllerParse(Config& cfg, const std::string& str);

#endif