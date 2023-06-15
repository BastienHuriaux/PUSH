#include "glut_functions.h"

///////////////////////////////////////////////////////////////////////////////
//	
// 
//      Main loop's fonctions

Zone ZonesArray[5];
Button ButtonsArray[7];
int main_window;
GLUI* glui;
GLUI_EditText* EditText;
string PieceOnCursorDescription = "";

// Function that gets the string in the glui textbox, then close this subwindow
void control(int id)
{
	puzzleArray[id]->text = EditText->get_text();
	glui->close();
	glutPostRedisplay();
}

// Called in main loop
// Callback function that defines what is displayed in the GLUT window
void glutDisplay()
{
	// Reset buffer's content
	glClear(GL_COLOR_BUFFER_BIT);

	// Define background color
	glClearColor(0.695, 0.102, 0.695, 0);

	//  Zones
	for (Zone CurrentZone : ZonesArray)
	{
		drawShape(CurrentZone.aCornersList, 0.898, 0.898, 0.898, GL_POLYGON);// Drawing interior
		drawShape(CurrentZone.aCornersList, 0, 0, 0, GL_LINE_LOOP);// Drawing exterior
	}

	// Draw Buttons
	for (Button CurrentButton : ButtonsArray)
	{
		drawShape(CurrentButton.aCornersList, 0.493, 0.102, 0.493, GL_POLYGON);// Drawing interior
		drawShape(CurrentButton.aCornersList, 0, 0, 0, GL_LINE_LOOP);// Drawing exterior
		drawShape(CurrentButton.outlineXY, 0, 0.797, 0.797, GL_LINE_LOOP); // drawing form
		drawWriting(CurrentButton.aPosX + 0.03, CurrentButton.aPosY + 0.03, 1, 1, 1, CurrentButton.aText, GLUT_BITMAP_TIMES_ROMAN_24);
	}

	// Draw Pieces
	for (int i = 0; i < pieceArray.size(); i++)
	{
		pieceArray[i]->createPoint();
		drawTriangles(pieceArray[i]->pointXY, 0.898, 0.598, 0);
		drawShape(pieceArray[i]->outlineXY, 0, 0, 0, GL_LINE_LOOP);
	}

	// Draw Puzzle
	for (int i = 0; i < puzzleArray.size(); i++)
	{
		puzzleArray[i]->createPoint();
		drawTriangles(puzzleArray[i]->pointXY, 0, 0.797, 0.797);
		drawShape(puzzleArray[i]->outlineXY, 0, 0, 0, GL_LINE_LOOP);
	}

	// Write the command
	writingCommand();
	drawWriting(-0.9, -0.83, 0, 0, 1, CommandSentence, GLUT_BITMAP_TIMES_ROMAN_24);

	// Write the standard output
	drawCommandOutput();

	// Swap current screen and buffer
	glutSwapBuffers();
}

// Callback function for mouse motion in the GLUT window
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

void glutPassiveMotion(int x, int y)
{
	float vX = (2 * (float)x / glutGet(GLUT_SCREEN_WIDTH)) - 1;
	float vY = -((2 * (float)y / glutGet(GLUT_SCREEN_HEIGHT)) - 1);
	for (int i = 0; i < pieceArray.size(); i++)
	{
		if (pieceArray[i]->isPointInsideForm(vX, vY))
		{
			glutPostRedisplay();
			return;
		}
	}
}

// Callback function for mouse click in the GLUT window
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
				&& state == GLUT_DOWN && (typeid(*puzzleArray[i]) != typeid(Tube)))
			{
				glui = GLUI_Master.create_glui("GLUI");
				new GLUI_StaticText(glui, "Entrez une commande : ");
				EditText = new GLUI_EditText(glui, "");
				EditText->set_w(410);
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

// Callback function of the keyboard in the GLUT window
void glutKeyboard(unsigned char key, int x, int y)
{
	if (key == 'f') { glutLeaveMainLoop(); }
}

void init()
{
	// Variables' initialization
	// Environment's zones
	Zone ZoneCommandCompleted = Zone(-0.95, -0.9, 1.43, 0.18);
	Zone ZonePiecesCreation = Zone(-0.95, 0.77, 1.9, 0.18);
	Zone ZonePuzzle = Zone(-0.95, -0.67, 1.43, 1.39);
	Zone ZoneOutputDisplay = Zone(0.53, -0.065, 0.42, 0.785);
	Zone ZoneErrorDisplay = Zone(0.53, -0.9, 0.42, 0.785);

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

	Button ButtonExecute = Button(0.23, -0.86, "Execute");

	ButtonsArray[0] = ButtonProcessus;
	ButtonsArray[1] = ButtonIn;
	ButtonsArray[2] = ButtonOut;
	ButtonsArray[3] = ButtonError;
	ButtonsArray[4] = ButtonTube;
	ButtonsArray[5] = ButtonDelete;
	ButtonsArray[6] = ButtonExecute;

	// Initialization of the first piece
	shared_ptr<Processus> inStart = make_shared<Processus>();
	inStart->x0 = -0.4;
	inStart->y0 = 0.5;
	puzzleArray.insert(puzzleArray.end(), inStart);
}