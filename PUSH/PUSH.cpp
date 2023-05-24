// PUSH.cpp : définit le point d'entrée de l'application.
//

#include "PUSH.h"

using namespace std;


//my_popen allow us to get the output of a command(string) in a vector of string
//Only right commands will give a result
//Other will give an empty line

void my_popen(const string& pCommand, vector<string>& pOutput) {
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

    pclose(vFile);
}//Code from stackoverflow



int main()
{
    //LOL
	cout << "Hello CMake." << endl;

    //We have a command
    string vCommand = "ls";
    //We save the result of the command in output
    vector<string> vOutput;
    my_popen(vCommand, vOutput);

    //We check if the output has the good values
    for (vector<string>::iterator vIterator = vOutput.begin(); vIterator != vOutput.end(); ++vIterator)
    {
        cout << *vIterator << endl;
    }//It has


	return 0;
}
