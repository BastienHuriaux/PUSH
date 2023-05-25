// PUSH.cpp : définit le point d'entrée de l'application.
//

#include "PUSH.h"
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;


void cd(const string& pCommand, vector<fs::path>& pPath) {
    int StartWord = 3;
    bool change = false;
    int resize = pPath.size();
    for (int i = 3; i < pCommand.size(); i++) {
        if (pCommand[i] == '.' && pCommand[i - 1] == '.') {
            if (resize > 1) {
                resize -= 1;
                change = true;
            }
        }
        if (pCommand[i] == '/' || pCommand[i] == ' ') {
            if (fs::is_directory(pPath.back() / pCommand.substr(StartWord, i - StartWord))) {
                pPath.push_back(pPath.back() / pCommand.substr(StartWord, i - StartWord));
                StartWord = i + 1;
            }
        }
        if (i + 1 == pCommand.size()) {
            if (fs::is_directory(pPath.back() / pCommand.substr(StartWord, i - (StartWord - 1)))) {
                pPath.push_back(pPath.back() / pCommand.substr(StartWord, i - (StartWord - 1)));
            }
        }
    }
    if(change) pPath.resize(resize);
}

//my_popen allow us to get the output of a command(string) in a vector of string
//Only right commands will give a result
//Other will give an empty line

void my_popen(const string& pCommand, vector<string>& pOutput, vector<fs::path>& pPath) {
    FILE* vFile;
    const int vSizeBuf = 1234;
    char vBuff[vSizeBuf];
    pOutput = vector<string>();
    string vPath = "cd " + string(pPath.back()) + " && " + pCommand;
    if ((vFile = popen(vPath.c_str(), "r")) == NULL)
    {
        cout << "error, File NULL" << endl;
    }

    string vCurrent_string = "";

    while (fgets(vBuff, sizeof(vBuff), vFile))
    {
        vCurrent_string += vBuff;
    }

    pOutput.push_back(vCurrent_string.substr(0, vCurrent_string.size() - 1));
    if (pCommand.substr(0, 2) == "cd") {
        cd(pCommand, pPath);
    }
    pclose(vFile);
}//Code from stackoverflow

void init(vector<fs::path>& pPath) {
    vector<string> vOutput;
    const auto vWorkingDir = fs::current_path();
    if(!fs::is_directory(vWorkingDir / "Environment"))
        my_popen("mkdir Environment", vOutput, pPath);
    my_popen("cd Environment", vOutput, pPath);
}


int main()
{
    //LOL
	cout << "Hello CMake." << endl;
    fs::path initial_path = "./Environment";
    vector<fs::path> current_path = {initial_path};
    //init(current_path);

    //We have a command
    string vCommand = "cd test/test2";
    //We save the result of the command in output
    vector<string> vOutput;
    my_popen(vCommand, vOutput, current_path);
    cout << current_path.back();
    //We check if the output has the good values
    for (vector<string>::iterator vIterator = vOutput.begin(); vIterator != vOutput.end(); ++vIterator)
    {
       cout << *vIterator << endl;
    }//It has
    vCommand = "cd ../.. && ls";
    my_popen(vCommand, vOutput, current_path);
    cout << current_path.back();

	return 0;
}
