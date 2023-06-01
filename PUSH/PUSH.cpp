// PUSH.cpp : définit le point d'entrée de l'application.
//

#include "PUSH.h"

using namespace std;
namespace fs = std::filesystem;
using namespace glm;

GLFWwindow* window;

///////////////////////////////////////////////////////////////////////////////
//
// 
//      Structures


struct Processus {

    float x0 = 0.0;
    float x1, x2, x3, x4, x5;

    float y0 = 0.0;
    float y1, y2, y3, y4, y5;

    vector <float> pointX;
    vector <float> pointY;

    Processus() {
        createPoint();
    }

    void createPoint() {
        x1 = x0 - 0.5;
        x2 = x0 + 0.5;
        x3 = x0 - 0.3;
        x4 = x0 + 0.7;
        x5 = x0 + 0.3;

        y1 = y0 - 0.5;
        y2 = y0 + 0.5;
        y3 = y0 + 0.3;
        y4 = y0 - 0.3;
        y5 = y0 - 0.7;

        pointX = { x1, x2, x2, x1, x1, x2, x1, x1, x2, x1, x2, x2, x3, x2, x2, x3, x3, x2, x2, x2, x4, x3, x5, x0 };
        pointY = { y2, y2, y3, y2, y3, y3, y1, y4, y4, y1, y1, y4, y2, y2, y1, y2, y1, y1, y3, y4, y0, y1, y1, y5 };
    }

    void mouvForm(const double xCursor, const double yCursor) {
        x0 = xCursor; // D�placement horizontal
        y0 = yCursor; // D�placement vertical

        createPoint(); // Permet de changer tout les points
    }

    bool isPointInsideForm(const double xCursor, const double yCursor) {
        // Coordonn�es des sommets du triangle
        for (int i = 0; i < pointX.size() - 2; i = i + 3) {
            float x1 = pointX[i];
            float x2 = pointX[i + 1];
            float x3 = pointX[i + 2];
            float y1 = pointY[i];
            float y2 = pointY[i + 1];
            float y3 = pointY[i + 2];

            // Calcul des barycentres
            float denom = ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
            float b1 = ((y2 - y3) * (xCursor - x3) + (x3 - x2) * (yCursor - y3)) / denom;
            float b2 = ((y3 - y1) * (xCursor - x3) + (x1 - x3) * (yCursor - y3)) / denom;
            float b3 = 1.0f - b1 - b2;

            // V�rification si le point est � l'int�rieur du triangle
            if ((b1 >= 0.0f) && (b2 >= 0.0f) && (b3 >= 0.0f)) return true;
        }
        return false;
    }

};


///////////////////////////////////////////////////////////////////////////////
//
// 
//      Fonctions



void drawTriangle(vector<float> dataX, vector<float> dataY) {
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < dataX.size(); i++)
	{
		glColor3f(1.0f, 0.0f, 0.0f); // Rouge
		glVertex2f(dataX[i], dataY[i]);
	}
	glEnd();
}

void PositionCursor(GLFWwindow* pWindow, double& xCursor, double& yCursor, const int width, const int height)
{
    double xpos;
    double ypos;
    glfwGetCursorPos(pWindow, &xpos, &ypos);

    if (xpos < 0)
    {
        xpos = xpos / 11;
    }
    else if (xpos > 0)
    {
        xpos = xpos / (width - 11);
    }

    if (ypos < 0)
    {
        ypos = (ypos / 11);
    }
    if (ypos > 0)
    {
        ypos = (ypos / (height - 11));
    }

    xpos = (xpos - 0.5) * 2;
    ypos = -(ypos - 0.5) * 2;


    xCursor = xpos;
    yCursor = ypos;
}

void cd(const string& pCommand, string& pPath) {
    string vStartPath = pPath;
    int vEnd = pCommand.size();
    int StartWord = 0;
    char c[100];
    for (int i = 0; i < pCommand.size(); i++) {
        if (pCommand[i] == '/' || i+1 == pCommand.size() || pCommand[i] == ' ') {
            string vCommand = pCommand.substr(StartWord, i + 1 - StartWord);
            if (pPath == "."&& vCommand.substr(0,2) == "..") {
                StartWord = i + 1;
                return;
            }
            int Error = chdir(vCommand.c_str());
            if (Error == -1) {
                chdir(getcwd(c, 100));
                pPath = vStartPath;
                return;
            }
            StartWord = i+1;
            string vPath = getcwd(c, 100);
            int vPush = vPath.rfind("PUSH");
            pPath = "." + vPath.substr(vPush + 16, vPath.size() - vPush);
        }
        if (pCommand[i] == ' ') return;
    }
}

//my_popen allow us to get the output of a command(string) in a vector of string
//Only right commands will give a result
//Other will give an empty line

void my_popen(const string& pCommand, vector<string>& pOutput, string& pPath) {
    FILE* vFile;
    const int vSizeBuf = 1234;
    char vBuff[vSizeBuf];
    pOutput = vector<string>();
    char c[100];
    string vPath = "cd " + string(getcwd(c,100)) + " && " + pCommand;
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
        cd(pCommand.substr(3, pCommand.size() - 3), pPath);
    }
    pclose(vFile);
}//Code from stackoverflow

void init(string& pPath) {
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
    string path = ".";
    init(path);
    //cout << current_path.back() << "\n";
    //We have a command
    string vCommand = "cd test/test2";
    //We save the result of the command in output
    vector<string> vOutput;
    my_popen(vCommand, vOutput, path);
    //We check if the output has the good values
    for (vector<string>::iterator vIterator = vOutput.begin(); vIterator != vOutput.end(); ++vIterator)
    {
       cout << *vIterator << endl;
    }//It has
    vCommand = "cd ../../test";
    my_popen(vCommand, vOutput, path);
    char c[100];

    //taille de la fenetre
    int windowWidth = 1900;
    int windowHeight = 1060;

    //intialisation de la position du curseur, 
    double xCursor = 0;
    double yCursor = 0;

    // Initialisation de GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Cr�ation de la fen�tre
    window = glfwCreateWindow(windowWidth, windowHeight, "Triangle OpenGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    //Vraie valeur de la position du curseur
    PositionCursor(window, xCursor, yCursor, windowWidth, windowHeight);

    // Configuration de GLFW
    glfwMakeContextCurrent(window);

    // Colorisation
    glClearColor(0.969f, 0.941f, 0.941f, 0.0f);

    // Cr�e les pi�ces
    Processus forme = Processus();


    // Boucle principale
    while ((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)) {

        //met a jour la valeur de la position du curseur
        PositionCursor(window, xCursor, yCursor, windowWidth, windowHeight);

        // Effacer le contenu de la fen�tre
        glClear(GL_COLOR_BUFFER_BIT);

        // Mise � jour du triangle
        //mouvForm(forme.dataX, forme.dataY);
        //mouvTriangle(forme.x1, forme.y1);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) //regarde si le bouton gauche est appuye
        {
            if (forme.isPointInsideForm(xCursor, yCursor))
            {
                forme.mouvForm(xCursor, yCursor);
            }

        }

        // Dessiner le triangle
        drawTriangle(forme.pointX, forme.pointY);

        // �change des tampons d'affichage
        glfwSwapBuffers(window);

        // V�rification des �v�nements de la fen�tre
        glfwPollEvents();
    }

    // Terminaison de GLFW
    glfwTerminate();
    return 0;
}
