#include "command.h"

using namespace std;
namespace fs = std::filesystem;
string CommandSentence = "";
string CommandHidden = "";
string CommandOutput = "";
string CommandError = "";
bool CommandExecuted = false;

// Called when the button "Execute" is pushed
// Execute the command line and get the standard output
void my_popen(const string& pCommand, string& pOutput)
{
	FILE* vFile;
	const int vSizeBuf = 1234;
	char vBuff[vSizeBuf];
	if ((vFile = popen(pCommand.c_str(), "r")) == NULL)
	{
		cout << "error, File NULL" << endl;
	}

	pOutput = "";

	while (fgets(vBuff, sizeof(vBuff), vFile))
	{
		pOutput += vBuff;
	}
	pclose(vFile);
}//Code from stackoverflow

// Always called 
// Display the command line written by the user
void writingCommand()
{
	// Reinitialization of the command line
	CommandSentence = "";
	CommandHidden = "";
	// Write every string of every piece in the puzzle
	for (int i = 0; i < puzzleArray.size(); i++)
	{
		// Add redirection when needed
		if (typeid(*puzzleArray[i]) == typeid(Out))
		{
			CommandSentence += " 1> " + puzzleArray[i]->text + " ";
			CommandHidden += " 1> " + puzzleArray[i]->text + " ";
		}
		else if (typeid(*puzzleArray[i]) == typeid(Error))
		{
			CommandSentence += " 2> " + puzzleArray[i]->text + " ";
			CommandHidden += " 2> " + puzzleArray[i]->text + " ";
		}
		else if (typeid(*puzzleArray[i]) == typeid(Tube))
		{
			CommandSentence += " | ";
			CommandHidden += " | ";
		}
		else
		{
			CommandSentence += puzzleArray[i]->text + " ";
			CommandHidden += puzzleArray[i]->text + " ";
			if (typeid(*puzzleArray[i]) == typeid(Processus)) {
				Processus* p = dynamic_cast<Processus*>(puzzleArray[i].get());
				if (p->errorProcessus) {
					CommandHidden += " 2>> /tmp/error";
				}
			}
		}
	}
}