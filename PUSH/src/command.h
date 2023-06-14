#ifndef COMMAND_H
#define COMMAND_H

#pragma once
#include <filesystem>
#include <vector>

#include "puzzle.h"

extern string CommandSentence;
extern std::vector<std::string> CommandOutput;
extern bool CommandExecuted;


void my_popen(const string& pCommand, vector<string>& pOutput);

void writingCommand();

#endif
