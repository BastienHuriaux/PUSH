#include "draw.h"

// Called in the screen display
// Display a geometrical form
void drawShape(vector<vector<float>> pCoords, float pRed, float pGreen, float pBlue, int pDrawingType)
{
	glBegin(pDrawingType);
	for (int i = 0; i < pCoords[0].size(); i++)
	{
		// Color
		glColor3f(pRed, pGreen, pBlue);
		// Vertices of the geometrical form
		glVertex2f(pCoords[0][i], pCoords[1][i]);
	}
	glEnd();
}

// Called in the screen display
// Display a traingle form -- our pieces are multiples triangles --
void drawTriangles(vector<vector<float>> pCoords, float r, float g, float b)
{
	for (int i = 0; i < pCoords[0].size() - 2; i += 3)
	{
		vector<vector<float>> vCurrentTriangle =
		{ {pCoords[0][i], pCoords[0][i + 1], pCoords[0][i + 2]},
		  {pCoords[1][i], pCoords[1][i + 1], pCoords[1][i + 2]}
		};

		drawShape(vCurrentTriangle, r, g, b, GL_TRIANGLES);
	}
}

// Called when a string is written
// Display a string
// @pTypeOfPolice can be GLUT_BITMAP_HELVETICA_12 or GLUT_BITMAP_TIMES_ROMAN_24
void drawWriting(float x, float y, float r, float g, float b, string string, void* pTypeOfPolice)
{
	// Color of writing
	glColor3f(r, g, b);

	// Position of the first character of the sentence
	glRasterPos2f(x, y);

	// length of the string
	int len, i;
	len = (int)size(string);

	// Writing of each character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(pTypeOfPolice, string[i]);
	}
}


// Always called, but is functionnal only when the button "Execute" is pressed
// Write the output of the command
void drawCommandOutput(float pY, string& pCommand)
{
	// Definition of the Y axis in which the line will be displayed
	// Increases at each iteration
	float vLine = pY;

	// First char of the command to be displayed 
	int vFirstChar = 0;
	// Last char of the command to be displayed
	int vLastChar = 0;

	// Part of the command that will be displayed
	string vOutputToDisplay = "";

	if (pCommand.size()>0)
	{
		// We will display the command from the fisrt char to '\n',
		// Then from one '\n' to the other
		for (int i = 0; i < pCommand.size() - 1; i++)
		{
			if (pCommand[i] == '\n')
			{
				// Reinitialization of what will be displayed
				vLastChar = i;
				vOutputToDisplay = "";

				// Writing of a sentence
				for (int j = vFirstChar; j < vLastChar; j++)
				{
					vOutputToDisplay += pCommand[j];
					drawWriting(0.54, vLine, 0, 0, 1, vOutputToDisplay, GLUT_BITMAP_HELVETICA_12);
				}

				// Preparation for the next sentence to be displayed
				vLine -= 0.05;
				vFirstChar = i + 1;
			}
		}

		// Finally we display from the last '\n' to the end of the string
		vOutputToDisplay = "";
		for (int i = vFirstChar; i < pCommand.size(); i++)
		{
			vOutputToDisplay += pCommand[i];
		}
		drawWriting(0.54, vLine, 0, 0, 1, vOutputToDisplay, GLUT_BITMAP_HELVETICA_12);
	}
}