#ifndef COMMAND_H
#define COMMAND_H

#pragma once
#include <filesystem>
#include <vector>

#include "puzzle.h"

extern std::string CommandSentence;
extern std::string CommandHidden;
extern std::string CommandOutput;
extern std::string CommandError;
extern std::string PathCD;
extern bool CommandExecuted;

void my_popen(const string& pCommand, std::string& pOutput);

void writingCommand();

#endif
