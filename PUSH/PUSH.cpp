// PUSH.cpp : définit le point d'entrée de l'application.
//

#include "PUSH.hpp"

using namespace std;
namespace fs = std::filesystem;

//Global arrays' initialization

Zone ZonesArray[5];
Button ButtonsArray[7];
vector <shared_ptr<Piece>> pieceArray;
vector <shared_ptr<Piece>> puzzleArray;
bool LeftClickState = false;
string CommandSentence = "";
int main_window;
GLUI* glui;
bool djlazdjalkzjd = false;
GLUI_EditText* EditText;
GLUI_Panel* pPanel;

///////////////////////////////////////////////////////////////////////////////
//	
// 
//      Fonctions

void drawShape(vector<vector<float>> pCoords, float pRed, float pGreen, float pBlue, int pDrawingType)
{
	glBegin(pDrawingType);
	for (int i = 0; i < pCoords[0].size(); i++)
	{
		glColor3f(pRed, pGreen, pBlue);// Color
		glVertex2f(pCoords[0][i], pCoords[1][i]);// Vertices
	}
	glEnd();
}

void drawTriangles(vector<vector<float>> pCoords)
{
	for (int i = 0; i < pCoords[0].size() - 2; i += 3)
	{
		vector<vector<float>> vCurrentTriangle = { {pCoords[0][i], pCoords[0][i + 1], pCoords[0][i + 2]},
												   {pCoords[1][i], pCoords[1][i + 1], pCoords[1][i + 2]} };
		drawShape(vCurrentTriangle, 0, 0, 1, GL_TRIANGLES);
	}
}

void drawWriting(float x, float y, float r, float g, float b, string string)
{
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	int len, i;
	len = (int)size(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void useButton(string pButtonText)
{
	shared_ptr<Processus> newProcessus = make_shared<Processus>();
	shared_ptr<In> newIn = make_shared<In>();
	shared_ptr<Out> newOut = make_shared<Out>();
	shared_ptr<Error> newError = make_shared<Error>();
	shared_ptr<Tube> newTube = make_shared<Tube>();

	if (pButtonText.compare("Processus") == 0)
	{
		pieceArray.insert(pieceArray.end(), newProcessus);
	}
	else if (pButtonText.compare("In") == 0)
	{
		pieceArray.insert(pieceArray.end(), newIn);
	}
	else if (pButtonText.compare("Out") == 0)
	{
		pieceArray.insert(pieceArray.end(), newOut);
	}
	else if (pButtonText.compare("Error") == 0)
	{
		pieceArray.insert(pieceArray.end(), newError);
	}
	else if (pButtonText.compare("Tube") == 0)
	{
		pieceArray.insert(pieceArray.end(), newTube);
	}
	else if (pButtonText.compare("Delete") == 0)
	{
		puzzleArray.clear();
		shared_ptr<In> inStart = make_shared<In>();
		inStart->x0 = -0.8;
		inStart->y0 = 0.5;
		puzzleArray.insert(puzzleArray.end(), inStart);
	}
	else if (pButtonText.compare("Execute") == 0)
	{
		glutLeaveMainLoop();
	}
}

void movePiece(shared_ptr<Piece>& pPiece, float pCursorX, float pCursorY)
{
	if (pCursorX > -0.90 && pCursorX < 0.50
		&& pCursorY > -0.75 && pCursorY < 0.75)
	{
		pPiece->x0 = pCursorX;
		pPiece->y0 = pCursorY;
		glutPostRedisplay();
	}
}

void suppressPiece(int indice)
{
	pieceArray.erase(pieceArray.begin() + indice);
}

void writingCommand()
{
	CommandSentence = "";
	for (int i = 0; i < puzzleArray.size(); i++)
	{
		CommandSentence += puzzleArray[i]->text;
	}
}

void avengersPuzzle(shared_ptr<Piece>& pPiece, shared_ptr<Piece>& pPuzzle)
{
	// SE RENSEIGNER SUR CETTE MERDE

	// On compare les types des pièces
	if (typeid(*pPiece) == typeid(Processus))
	{
		Processus* p = dynamic_cast<Processus*>(pPiece.get());
		if (typeid(*pPuzzle) == typeid(In))
		{
			In* d = dynamic_cast<In*>(pPuzzle.get());
			if (p->inProcessus == true && d->outIn == true)
			{
				// On bloque les entrées
				p->inProcessus = false;
				d->outIn = false;


				//+ p->texte;

				// On change la pièce de vector
				puzzleArray.insert(puzzleArray.end(), pPiece);
				pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

				// On déplace la pièce
				p->x0 = d->x0 + 0.1;
				p->y0 = d->y0;
			}
		}
		else if (typeid(*pPuzzle) == typeid(Out))
		{
			Out* d = dynamic_cast<Out*>(pPuzzle.get());
			if (p->outProcessus == true && d->inOut == true)
			{
				// On bloque les entrées
				p->outProcessus = false;
				d->inOut = false;

				//CA NARRIVERAJAMAAIS

				// On change la pièce de vector
				puzzleArray.insert(puzzleArray.end(), pPiece);
				pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

				// On déplace la pièce
				p->x0 = d->x0 - 0.1;
				p->y0 = d->y0;
			}
		}
		else if (typeid(*pPuzzle) == typeid(Error))
		{
			Error* d = dynamic_cast<Error*>(pPuzzle.get());
			if (p->errorProcessus == true && d->inError == true)
			{
				// On bloque les entrées
				p->errorProcessus = false;
				d->inError = false;

				//CA N ARRIVERA JAMAIS

				// On change la pièce de vector
				puzzleArray.insert(puzzleArray.end(), pPiece);
				pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

				// On déplace la pièce
				p->x0 = d->x0;
				p->y0 = d->y0 + 0.1;
			}
		}
		else if (typeid(*pPuzzle) == typeid(Tube))
		{
			Tube* d = dynamic_cast<Tube*>(pPuzzle.get());
			if (p->inProcessus == true && d->outTube == true)
			{
				// On bloque les entrées
				p->inProcessus = false;
				d->outTube = false;

				// On ajoute au mot total une pipe
				//pTotalWord += p->texte;;


				// On change la pièce de vector
				puzzleArray.insert(puzzleArray.end(), pPiece);
				pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

				// On déplace la pièce
				p->x0 = d->x0 + 0.1;
				p->y0 = d->y0;
			}
		}
	}
	else if (typeid(*pPiece) == typeid(In) && typeid(*pPuzzle) == typeid(Processus))
	{
		In* p = dynamic_cast<In*>(pPiece.get());
		Processus* d = dynamic_cast<Processus*>(pPuzzle.get());
		if (p->outIn == true && d->inProcessus == true)
		{
			// On bloque les entrées
			p->outIn = false;
			d->inProcessus = false;

			//Ca N ARRIVERA JAMAIS

			// On change la pièce de vector
			puzzleArray.insert(puzzleArray.end(), pPiece);
			pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

			// On déplace la pièce
			p->x0 = d->x0 - 0.1;
			p->y0 = d->y0;
		}
	}
	else if (typeid(*pPiece) == typeid(Out) && typeid(*pPuzzle) == typeid(Processus))
	{
		Out* p = dynamic_cast<Out*>(pPiece.get());
		Processus* d = dynamic_cast<Processus*>(pPuzzle.get());
		if (p->inOut == true && d->outProcessus == true)
		{
			// On bloque les entrées
			p->inOut = false;
			d->outProcessus = false;

			string txt = p->text;
			p->text = " RedirectionVersSortie" + txt;

			// On change la pièce de vector
			puzzleArray.insert(puzzleArray.end(), pPiece);
			pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

			// On déplace la pièce
			p->x0 = d->x0 + 0.1;
			p->y0 = d->y0;
		}
	}
	else if (typeid(*pPiece) == typeid(Error) && typeid(*pPuzzle) == typeid(Processus))
	{
		Error* p = dynamic_cast<Error*>(pPiece.get());
		Processus* d = dynamic_cast<Processus*>(pPuzzle.get());
		if (p->inError == true && d->errorProcessus == true)
		{
			// On bloque les entrées
			p->inError = false;
			d->errorProcessus = false;

			// On obtient la position de pPiece
			vector<shared_ptr<Piece>>::iterator vPosition;
			vPosition = find(puzzleArray.begin(), puzzleArray.end(), pPuzzle);
			// On ajoute l'erreur apres le processus dans la liste du puzzle
			puzzleArray.insert(vPosition + 1, pPiece);

			// On supprime l'erreur de la liser de piece
			pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

			string txt = p->text;
			p->text = " RedirectionVersSortieErreur" + txt;
			/*
			if (d->outProcessus)
			{
				pTotalWord += "RedirectionVersSortieErreur" + p->texte;
			}
			else
			{
				string vNewCommandLine;
				for (int vCharacterCommand = 0; vCharacterCommand < (int)size(pTotalWord) - (int)size(d->texte); vCharacterCommand++)
				{
					string vSearchedWord = "";
					for (int i = 0; i < (int)size(d->texte); i++)
					{
						vSearchedWord += pTotalWord[i];
					}
					if (vSearchedWord.compare(d->texte) == 0)
					{
						for (int i = 0; i < vCharacterCommand; i++)
						{
							vNewCommandLine[i] = pTotalWord[i];
						}

						vNewCommandLine += "RedirectionVersSortieErreur" + p->texte;

						for (int i = vCharacterCommand; i < (int)size(pTotalWord); i++)
						{
							vNewCommandLine[i + (int)size(d->texte) + 28] = pTotalWord[i]; // 28 = RedirectionVersSortieErreur.size()
						}

						pTotalWord = vNewCommandLine;
					}
				}
				//ensuite, executer la commande
			}

			// On change la pièce de vector
			pPuzzleArray.insert(pPuzzleArray.end(), pPiece);
			pPieceArray.erase(remove(pPieceArray.begin(), pPieceArray.end(), pPiece), pPieceArray.end());
			*/

			// On déplace la pièce
			p->x0 = d->x0;
			p->y0 = d->y0 - 0.1;
		}
	}
	else if (typeid(*pPiece) == typeid(Tube) && typeid(*pPuzzle) == typeid(Processus))
	{
		Tube* p = dynamic_cast<Tube*>(pPiece.get());
		Processus* d = dynamic_cast<Processus*>(pPuzzle.get());
		if (p->inTube == true && d->outProcessus == true)
		{
			// On bloque les entrées
			p->inTube = false;
			d->outProcessus = false;

			//string txt = p->texte;

			// On change la pièce de vector
			puzzleArray.insert(puzzleArray.end(), pPiece);
			pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

			// On déplace la pièce
			p->x0 = d->x0 + 0.1;
			p->y0 = d->y0;
		}
	}
}

void collisionRectRect()
{
	for (int i = 0; i < pieceArray.size(); i++)
	{
		for (int j = 0; j < puzzleArray.size(); j++)
		{
			// Vérification que les 2 rectangles se touchent
			if (pieceArray[i]->x0 - 0.05 < puzzleArray[j]->x0 + 0.05 && pieceArray[i]->x0 + 0.05 > puzzleArray[j]->x0 - 0.05 &&
				pieceArray[i]->y0 - 0.05 < puzzleArray[j]->y0 + 0.05 && pieceArray[i]->y0 + 0.05 > puzzleArray[j]->y0 - 0.05)
			{
				avengersPuzzle(pieceArray[i], puzzleArray[j]);
				return; // Permet de ne pas sortir du vector
			}
		}
	}
}

/*void my_popen(const string& pCommand, vector<string>& pOutput) {
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
}//Code from stackoverflow*/


///////////////////////////////////////////////////////////////////////////////
//	
// 
//      Main loop's fonctions

void glutDisplay()// WHat the screen displays
{
	// reset buffer's content
	glClear(GL_COLOR_BUFFER_BIT);
	// background and environment content



	//  Zones
	for (Zone CurrentZone : ZonesArray)
	{
		drawShape(CurrentZone.aCornersList, 1, 1, 1, GL_POLYGON);// Drawing interior
		drawShape(CurrentZone.aCornersList, 0, 0, 0, GL_LINE_LOOP);// Drawing exterior
	}

	// Draw Buttons
	for (Button CurrentButton : ButtonsArray)
	{
		drawShape(CurrentButton.aCornersList, 0.5, 0.25, 0.5, GL_POLYGON);// Drawing interior
		drawShape(CurrentButton.aCornersList, 0, 0, 0, GL_LINE_LOOP);// Drawing exterior
		drawWriting(CurrentButton.aPosX + 0.03, CurrentButton.aPosY + 0.03, 1, 1, 1, CurrentButton.aText);
	}

	// Draw Pieces
	for (int i = 0; i < pieceArray.size(); i++)
	{
		pieceArray[i]->createPoint();
		drawTriangles(pieceArray[i]->pointXY);
		drawShape(pieceArray[i]->contourXY, 0, 0, 0, GL_LINE_LOOP);
	}

	for (int i = 0; i < puzzleArray.size(); i++)
	{
		puzzleArray[i]->createPoint();
		drawTriangles(puzzleArray[i]->pointXY);
		drawShape(puzzleArray[i]->contourXY, 0, 0, 0, GL_LINE_LOOP);
	}
	writingCommand();
	drawWriting(-0.9, -0.88, 0, 0, 0, CommandSentence);

	// swap current screen and buffer
	glutSwapBuffers();
}

void glutMotion(int x, int y)
{
	float vX = (2 * (float)x / glutGet(GLUT_SCREEN_WIDTH)) - 1;
	float vY = -((2 * (float)y / glutGet(GLUT_SCREEN_HEIGHT)) - 1);

	for (int i = 0; i < pieceArray.size(); i++)
	{
		if (pieceArray[i]->isPointInsideForm(vX, vY))
		{
			movePiece(pieceArray[i], vX, vY);
			collisionRectRect();
			glutPostRedisplay();
			return;
		}
	}
}

void hidecaca(int test)
{
	glui->hide();
}

void control(int id)
{
	puzzleArray[id]->text = EditText->get_text();
	glui->close();
	glutPostRedisplay();
	cout << puzzleArray[id]->text << endl;
}

void glutMouse(int button, int state, int x, int y)
{
	float vX = (2 * (float)x / glutGet(GLUT_SCREEN_WIDTH)) - 1;
	float vY = -((2 * (float)y / glutGet(GLUT_SCREEN_HEIGHT)) - 1);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		for (Button CurrentButton : ButtonsArray)
		{
			if (CurrentButton.buttonPressed(vX, vY)) { useButton(CurrentButton.aText); return; }
		}
		cout << puzzleArray[0]->isPointInsideForm(vX, vY) << endl;

		for (int i = 0; i < puzzleArray.size(); i++) {
			if (puzzleArray[i]->isPointInsideForm(vX, vY) && button == GLUT_LEFT_BUTTON
				&& state == GLUT_DOWN)
			{
				glui = GLUI_Master.create_glui("GLUI");
				new GLUI_StaticText(glui, "Bite");
				EditText = new GLUI_EditText(glui, "Entrez une commande : ");
				new GLUI_Button(glui, "Valider", i, control);
				glui->set_main_gfx_window(main_window);
			}
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = 0; i < pieceArray.size(); i++)
		{
			if (pieceArray[i]->isPointInsideForm(vX, vY)) { suppressPiece(i); return; }
		}
	}
	glutPostRedisplay();
}

void glutIdle()
{
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();

	//if (djlazdjalkzjd) glui->sync_live();
}

void glutKeyboard(unsigned char key, int x, int y)// Callbacks of the keyboard
{
	if (key == 'f') {
		djlazdjalkzjd = true;
		new GLUI_StaticText(pPanel, "Entrez une commande : ");
		EditText = new GLUI_EditText(pPanel, "");
		EditText->set_w(410);
		glui->show();
	}
}

int main(int argc, char** argv)
{
	// Variables' initialization
	// Environment's zones
	Zone ZoneCommandCompleted = Zone(-0.95, -0.95, 1.43, 0.18);
	Zone ZonePiecesCreation = Zone(-0.95, 0.77, 1.9, 0.18);
	Zone ZonePuzzle = Zone(-0.95, -0.72, 1.43, 1.44);
	Zone ZoneOutputDisplay = Zone(0.53, -0.09, 0.42, 0.81);
	Zone ZoneErrorDisplay = Zone(0.53, -0.95, 0.42, 0.81);

	ZonesArray[0] = ZoneCommandCompleted;
	ZonesArray[1] = ZonePiecesCreation;
	ZonesArray[2] = ZonePuzzle;
	ZonesArray[3] = ZoneOutputDisplay;
	ZonesArray[4] = ZoneErrorDisplay;

	// Environment's buttons
	Button ButtonProcessus = Button(-0.90, 0.81, "Processus");
	Button ButtonIn = Button(-0.58, 0.81, "In");
	Button ButtonOut = Button(-0.26, 0.81, "Out");
	Button ButtonError = Button(0.06, 0.81, "Error");
	Button ButtonTube = Button(0.38, 0.81, "Tube");
	Button ButtonDelete = Button(0.7, 0.81, "Delete");

	Button ButtonExecute = Button(0.23, -0.91, "Execute");

	ButtonsArray[0] = ButtonProcessus;
	ButtonsArray[1] = ButtonIn;
	ButtonsArray[2] = ButtonOut;
	ButtonsArray[3] = ButtonError;
	ButtonsArray[4] = ButtonTube;
	ButtonsArray[5] = ButtonDelete;
	ButtonsArray[6] = ButtonExecute;

	// Initialisation de la premiere piece
	shared_ptr<In> inStart = make_shared<In>();
	inStart->x0 = -0.8;
	inStart->y0 = 0.5;
	puzzleArray.insert(puzzleArray.end(), inStart);

	// Main loop
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1920, 1080);
	main_window = glutCreateWindow("PUSH");
	//glutFullScreen();
	glClearColor(0.5, 0.25, 0.5, 0);
	glutKeyboardFunc(glutKeyboard);
	glutMouseFunc(glutMouse);
	glutMotionFunc(glutMotion);
	glutDisplayFunc(glutDisplay);



	glutMainLoop();

	return 0;
}