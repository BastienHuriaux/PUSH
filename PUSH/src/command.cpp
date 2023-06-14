#include "command.h"

using namespace std;
namespace fs = std::filesystem;
string CommandSentence = "";
vector<string> CommandOutput = vector<string>();
bool CommandExecuted = false;

// Called when the button "Execute" is pushed
// Execute the command line and get the standard output
void my_popen(const string& pCommand, vector<string>& pOutput)
{
	FILE* vFile;
	const int vSizeBuf = 1234;
	char vBuff[vSizeBuf];
	pOutput = vector<string>();
	if ((vFile = popen(pCommand.c_str(), "r")) == NULL)
	{
		cout << "error, File NULL" << endl;
	}

	string vCurrent_string = "";

	while (fgets(vBuff, sizeof(vBuff), vFile))
	{
		vCurrent_string += vBuff;
	}
	pOutput.push_back(vCurrent_string.substr(0, vCurrent_string.size() - 1));
	//if (pCommand.substr(0, 2) == "cd") {
	//	cd(pCommand.substr(3, pCommand.size() - 3), pPath);
	//}
	pclose(vFile);
}//Code from stackoverflow

// Always called 
// Display the command line written by the user
void writingCommand()
{
	// Reinitialization of the command line
	CommandSentence = "";

	// Write every string of every piece in the puzzle
	for (int i = 0; i < puzzleArray.size(); i++)
	{
		// Add redirection when needed
		if (typeid(*puzzleArray[i]) == typeid(Out))
		{
			CommandSentence += " 1> " + puzzleArray[i]->text + " ";
		}
		else if (typeid(*puzzleArray[i]) == typeid(Error))
		{
			CommandSentence += " 2> " + puzzleArray[i]->text + " ";
		}
		else if (typeid(*puzzleArray[i]) == typeid(Tube))
		{
			CommandSentence += " | ";
		}
		else
		{
			CommandSentence += puzzleArray[i]->text + " ";
		}
	}
}