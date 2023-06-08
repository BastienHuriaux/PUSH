// PUSH.cpp : définit le point d'entrée de l'application.
//

#include "PUSH.hpp"

using namespace std;
namespace fs = std::filesystem;
using namespace glm;

GLFWwindow* window;


///////////////////////////////////////////////////////////////////////////////
//
// 
//      Fonctions

void output(float x, float y, float r, float g, float b, const char* string)
{
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	int len, i;
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void mouvForm(Piece& pPiece, const double xCursor, const double yCursor)
{
	if (xCursor > -0.95 && xCursor < 0.42
		&& yCursor > -0.45 && yCursor < 0.95)
	{
		pPiece.x0 = xCursor; // D�placement horizontal
		pPiece.y0 = yCursor; // D�placement vertical
	}

	pPiece.createPoint(); // Permet de changer tout les points
}

// Colorie l'interieur delimite par le contour 
// Exception si l'on met GL_LINE_LOOP dans pVariable, cela ne dessinera que les contours
void drawInterieur(vector<vector<float>> pContourXY, float pRouge, float pVert, float pBleu, int pVariable)
{
	//pVariable peut etre : GL_TRIANGLES ou GL_POLYGON pour colorier l'interieur

	glBegin(pVariable);
	for (int i = 0; i < pContourXY[0].size(); i++)
	{
		glColor3f(pRouge, pVert, pBleu);
		glVertex2f(pContourXY[0][i], pContourXY[1][i]);
	}
	glEnd();
}

// !!!!!! Cette fonction sert probablement a rien, A verifier !!!!!!
void drawContour(vector<vector<float>> pContourXY, float pRouge, float pVert, float pBleu)
{
	// GL_LINE_LOOP Permet de ne dessiner que les contours;
	drawInterieur(pContourXY, pRouge, pVert, pBleu, GL_LINE_LOOP);
}

// Dessine les triangles qui composent les pieces
void drawTriangle(vector<vector<float>> pPointXY, vector<vector<float>> pContourXY)
{
	drawInterieur(pPointXY, 0.537f, 0.553f, 0.569, GL_TRIANGLES);

	drawContour(pContourXY, 0.0f, 0.498f, 0.969f);
}

// Dessine les boutons
void drawBouton(vector<vector<float>> pContourXY)
{
	drawInterieur(pContourXY, 0.58f, 0.525f, 0.6f, GL_POLYGON);

	drawContour(pContourXY, 0.827f, 0.592f, 0.91f);
}


// Dessine les differentes zones, de 1 a 6
void drawZone(vector<vector<float>> pContourXY, int pType)
{
	// Permet de dessiner l'interieur
	float vRouge = 0.9;
	float vVert = 0.9;
	float vBleu = 0.9;

	// Differentes couleurs pour chaque type
	if (pType == 1)
	{
		vRouge = 0.471;
		vVert = 0.42;
		vBleu = 0.49;
	}
	else if (pType == 2)
	{
		vRouge = 0.812;
		vVert = 0.78;
		vBleu = 0.82;
	}
	else if (pType == 3)
	{
		vRouge = 0.176;
		vVert = 0.169;
		vBleu = 0.18;
	}
	else if (pType == 4)
	{
		vRouge = 0.251;
		vVert = 0.227;
		vBleu = 0.259;
	}
	else if (pType == 5)
	{
		vRouge = 0.157;
		vVert = 0.008;
		vBleu = 0.212;
	}
	else if (pType == 6)
	{
		vRouge = 0.157;
		vVert = 0.008;
		vBleu = 0.212;
	}

	drawInterieur(pContourXY, vRouge, vVert, vBleu, GL_POLYGON);

	drawContour(pContourXY, 0.671f, 0.027f, 0.89f);
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

	xCursor = (xpos - 0.5) * 2;
	yCursor = -(ypos - 0.5) * 2;
}


void createPiece(GLFWwindow* pWindow, vector <Piece>& pPieceArray, string pPieceType)//, bool& pProcessMove) 
{
	//PieceType peut etre Processus, In, Out, Error, Tube

	Piece newProcessus = Piece(Type::Processus);
	Piece newIn = Piece(Type::In);
	Piece newOut = Piece(Type::Out);
	Piece newError = Piece(Type::Error);
	Piece newTube = Piece(Type::Tube);

	static int oldState = GLFW_RELEASE;//0
	int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);// 1 if pressed
	if (newState == GLFW_RELEASE && oldState == GLFW_PRESS)
	{
		if (pPieceType.compare("Processus") == 0)
		{
			pPieceArray.insert(pPieceArray.end(), newProcessus);
		}
		else if (pPieceType.compare("In") == 0)
		{
			pPieceArray.insert(pPieceArray.end(), newIn);
		}
		else if (pPieceType.compare("Out") == 0)
		{
			pPieceArray.insert(pPieceArray.end(), newOut);
		}
		else if (pPieceType.compare("Error") == 0)
		{
			pPieceArray.insert(pPieceArray.end(), newError);
		}
		else if (pPieceType.compare("Tube") == 0)
		{
			pPieceArray.insert(pPieceArray.end(), newTube);
		}
	}
	oldState = newState;
}

void updatePiecePosition(GLFWwindow* window, const double& xCursor, const double& yCursor, vector <Piece>& pPieceArray)
{
	for (int i = 0; i < pPieceArray.size(); i++)
	{
		if (pPieceArray[i].isPointInsideForm(xCursor, yCursor))
		{
			mouvForm(pPieceArray[i], xCursor, yCursor);
			return; // Permet de selectionner une seule piece
		}
	}
}

void supressPiece(GLFWwindow* window, const double& xCursor, const double& yCursor, vector <Piece>& pPieceArray)
{
	for (int i = 0; i < pPieceArray.size(); i++)
	{
		if (pPieceArray[i].isPointInsideForm(xCursor, yCursor))
		{
			pPieceArray.erase(pPieceArray.begin() + i);
		}
	}
}

void drawPiece(vector <Piece> pPieceArray)
{
	for (int i = 0; i < pPieceArray.size(); i++)
	{
		drawTriangle(pPieceArray[i].pointXY, pPieceArray[i].contourXY);
	}
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


int main(int argc, char* argv[])
{
    //LOL
    cout << "Hello CMake." << endl;
    string path = ".";
    init(path);
    //cout << current_path.back() << "\n";
    //for (vector<string>::iterator vIterator = vOutput.begin(); vIterator != vOutput.end(); ++vIterator)
    //{
    //   cout << *vIterator << endl;
    //}//It has
    
    //bool processusMove = false;

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

    // Creation de la fen�tre
	window = glfwCreateWindow(windowWidth, windowHeight, "Triangle OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Initialisation de GLUT
	glutInit(&argc, argv);

	// Configuration de GLFW
	glfwMakeContextCurrent(window);

	// Colorisation
	//glClearColor(0.682f, 0.639f, 0.702f, 0.0f);
	glClearColor(0.478f, 0.239f, 0.561f, 0.0f);

	// Vector contenant les pi�ces
	vector <Piece> pieceArray;

	// Les Boutons
	Bouton B_Processus = Bouton(-0.7f, 0.92f, 0.15f, 0.1f, "Processus");
	Bouton B_In = Bouton(-0.4f, 0.92f, 0.15f, 0.1f, "In");
	Bouton B_Out = Bouton(-0.1f, 0.92f, 0.15f, 0.1f, "Out");
	Bouton B_Error = Bouton(0.2f, 0.92f, 0.15f, 0.1f, "Error");
	Bouton B_Tube = Bouton(0.5f, 0.92f, 0.15f, 0.1f, "Tube");
	vector<Bouton> boutonArray = { B_Processus, B_In, B_Out, B_Error, B_Tube };

	// Les Zones

	// La zone 2 a ete supprim�e ! 
	Zone Z_Zone3 = Zone(-0.25f, -0.6779f, 1.4999f, 0.3599f, 3); // Zone bas
	Zone Z_Zone4 = Zone(-0.25f, 0.25f, 1.499f, 1.499f, 4); // Zone centrale
	Zone Z_Zone5 = Zone(0.74999f, 0.5f, 0.5f, 0.9999f, 5); // Zone haut gauche
	Zone Z_Zone6 = Zone(0.74999f, -0.433f, 0.5f, 0.9f, 6); // Zone bas gauche
	Zone Z_Zone1Prime = Zone(0.0f, 0.92f, 1.997f, 0.158f, 1); // Zone haut

	vector<Zone> zoneArray =
	{
		Z_Zone3,
		Z_Zone4,
		Z_Zone5,
		Z_Zone6,
		Z_Zone1Prime,
	};
	/*
	Bouton B_Processus = Bouton(-0.75f, 0.73f, 0.15f, 0.1f, "Processus");
	Bouton B_In = Bouton(-0.75f, 0.4f, 0.15f, 0.1f, "In");
	Bouton B_Out = Bouton(-0.75f, 0.07f, 0.15f, 0.1f, "Out");
	Bouton B_Error = Bouton(-0.75f, -0.26f, 0.15f, 0.1f, "Error");
	Bouton B_Tube = Bouton(-0.75f, -0.59f, 0.15f, 0.1f, "Tube");
	vector<Bouton> boutonArray = { B_Processus, B_In, B_Out, B_Error, B_Tube };

	// Les Zones
	//Zone Z_Zone1 = Zone(-0.74999f, 0.5f, 0.5f, 0.9999f, 1); // Zone haut gauche
	Zone Z_Zone1 = Zone(-0.74999f, 0.0692f, 0.5f, 1.855f, 1); // Zone gauche
	//Zone Z_Zone2 = Zone(-0.74999f, -0.433f, 0.5f, 0.85f, 2); // Zone bas gauche
	Zone Z_Zone3 = Zone(0.0f, -0.6779f, 0.999f, 0.3599f, 3); // Zone centrale
	Zone Z_Zone4 = Zone(0.0f, 0.25f, 0.999f, 1.499f, 4); // Zone bas centre
	Zone Z_Zone5 = Zone(0.74999f, 0.5f, 0.5f, 0.9999f, 5); // Zone haut gauche
	Zone Z_Zone6 = Zone(0.74999f, -0.433f, 0.5f, 0.85f, 6); // Zone bas gauche

	vector<Zone> zoneArray =
	{
		Z_Zone1,
		//Z_Zone2,
		Z_Zone3,
		Z_Zone4,
		Z_Zone5,
		Z_Zone6
	};
	*/


	// Boucle principale
	while ((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0))
	{
		//met a jour la valeur de la position du curseur
		PositionCursor(window, xCursor, yCursor, windowWidth, windowHeight);

		// Effacer le contenu de la fen�tre
		glClear(GL_COLOR_BUFFER_BIT);

		// Les zones
		for (int i = 0; i < zoneArray.size(); i++)
		{
			drawZone(zoneArray[i].contourXY, zoneArray[i].type);
		}

		// Les boutons
		for (int i = 0; i < boutonArray.size(); i++)
		{
			drawBouton(boutonArray[i].contourXY);

			if (boutonArray[i].EstDansLeBouton(xCursor, yCursor))
			{
				createPiece(window, pieceArray, boutonArray[i].texte);
			}
		}

		// Mise a jour des pieces
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) //regarde si le bouton gauche est appuye 
		{
			updatePiecePosition(window, xCursor, yCursor, pieceArray);
		}
		// Suppression des pieces
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) //regarde si le bouton droit est appuye
		{
			supressPiece(window, xCursor, yCursor, pieceArray);
		}

		// Dessiner les pieces
		drawPiece(pieceArray);

		// Ecrit le texte sur les bouttons 
		char* cProc = "Processus";
		char* cIn = "In";
		char* cOut = "Out";
		char* cError = "Error";
		char* cTube = "Tube";
		output(-0.75f, 0.9f, 1, 1, 1, cProc);
		output(-0.41f, 0.9f, 1, 1, 1, cIn);
		output(-0.12f, 0.9f, 1, 1, 1, cOut);
		output(0.175f, 0.9f, 1, 1, 1, cError);
		output(0.475f, 0.9f, 1, 1, 1, cTube);


		// Echange des tampons d'affichage
		glfwSwapBuffers(window);

		// Verification des �v�nements de la fen�tre
		glfwPollEvents();
	}

	// Terminaison de GLFW
	glfwTerminate();
	return 0;
}
