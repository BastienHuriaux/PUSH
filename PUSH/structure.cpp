// Include STD
#include <iostream>
#include <vector>
using namespace std;


///////////////////////////////////////////////////////////////////////////////
//
// 
//      Structures et Enum

enum class Type { Processus, In, Out, Error, Tube };

struct Piece
{
	// Donn�es pour la Piece
	float x0 = 0.0;
	float x1, x2, x3, x4, x5;

	float y0 = 0.0;
	float y1, y2, y3, y4, y5;

	vector <vector<float>> pointXY;
	vector <vector<float>> contourXY;

	Type type;

	bool inProcessus = false, in = false, inTube = false,
		outProcessus = false, out = false, outTube = false, error = false;


	// Constructeur
	Piece(Type pType)
	{
		type = pType;

		// Param�tre des entr�es et sorties
		if (type == Type::Processus)
		{
			inProcessus = true, outProcessus = true, error = true;
		}
		if (type == Type::In)
		{
			in = true;
		}
		if (type == Type::Out)
		{
			out = true;
		}
		if (type == Type::Error)
		{
			error = true;
		}
		if (type == Type::Tube)
		{
			inTube = true, outTube = true;
		}

		// Cr�ations des pi�ces
		createPoint();
	}

	void createPoint()
	{
		x1 = x0 - 0.05;
		x2 = x0 + 0.05;
		x3 = x0 - 0.03;
		x4 = x0 + 0.03;
		x5 = x0 + 0.07;

		y1 = y0 - 0.05;
		y2 = y0 + 0.05;
		y3 = y0 - 0.03;
		y4 = y0 + 0.03;
		y5 = y0 - 0.07;

		if (type == Type::Processus)
		{
			pointXY = { { x1, x2, x2, x1, x1, x2, x1, x1, x2, x1, x2, x2, x3, x2, x2, x3, x3, x2, x2, x2, x5, x3, x4, x0 },
						{ y2, y2, y4, y2, y4, y4, y1, y3, y3, y1, y1, y3, y2, y2, y1, y2, y1, y1, y4, y3, y0, y1, y1, y5 } };
			contourXY = { { x1, x2, x2, x5, x2, x2, x4, x0, x3, x1, x1, x3, x3, x1 },
						  { y2, y2, y4, y0, y3, y1, y1, y5, y1, y1, y3, y3, y4, y4 } };
		}
		if (type == Type::In)
		{
			pointXY = { { x1, x2, x2, x1, x1, x2, x2, x2, x5, x5, x2, x5 },
						{ y2, y2, y1, y2, y1, y1, y4, y3, y4, y4, y3, y3 } };
			contourXY = { { x1, x2, x2, x5, x5, x2, x2, x1 },
						  { y2, y2, y4, y4, y3, y3, y1, y1 } };
		}
		if (type == Type::Out)
		{
			pointXY = { { x1, x2, x2, x1, x1, x2, x1, x2, x1, x1, x2, x2, x3, x2, x2, x3, x2, x3, x1, x3, x3, x1, x3, x3 },
						{ y1, y3, y1, y3, y1, y3, y2, y2, y4, y4, y2, y4, y2, y2, y1, y2, y1, y1, y4, y4, y0, y3, y3, y0 } };
			contourXY = { { x1, x2, x2, x1, x1, x3, x1 },
						  { y2, y2, y1, y1, y3, y0, y4 } };
		}
		if (type == Type::Error)
		{
			pointXY = { { x1, x1, x3, x1, x3, x3, x2, x2, x4, x2, x4, x4, x1, x2, x1, x2, x2, x1, x4, x0, x4, x3, x0, x3 },
						{ y1, y2, y2, y1, y2, y1, y2, y1, y1, y2, y2, y1, y1, y1, y4, y4, y1, y4, y2, y4, y4, y2, y4, y4 } };
			contourXY = { { x1, x3, x0, x4, x2, x2, x1 },
						  { y2, y2, y4, y2, y2, y1, y1 } };
		}
		if (type == Type::Tube)
		{
			pointXY = { { x1, x2, x2, x1, x1, x2, x1, x1, x2, x1, x2, x2, x2, x2, x3, x3, x2, x3, x1, x3, x3, x1, x3, x3, x2, x2, x5, x5, x2, x5 },
						{ y2, y2, y4, y2, y4, y4, y1, y3, y3, y1, y1, y3, y4, y3, y4, y3, y3, y4, y3, y0, y3, y4, y0, y4, y4, y3, y4, y4, y3, y3 } };
			contourXY = { { x1, x2, x2, x5, x5, x2, x2, x1, x1, x3, x1 },
						  { y2, y2, y4, y4, y3, y3, y1, y1, y3, y0, y4 } };
		}
	}

	bool isPointInsideForm(const double xCursor, const double yCursor)
	{
		// Coordonn�es des sommets du triangle
		for (int i = 0; i < pointXY[0].size() - 2; i = i + 3)
		{
			float x1 = pointXY[0][i];
			float x2 = pointXY[0][i + 1];
			float x3 = pointXY[0][i + 2];
			float y1 = pointXY[1][i];
			float y2 = pointXY[1][i + 1];
			float y3 = pointXY[1][i + 2];

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


struct Bouton
{
	// Position du centre du bouton
	float posX;
	float posY;

	// Position pour les quatre coins du bouton
	float x1, x2;
	float y1, y2;

	// Longueur et hauteur du bouton
	float longueur;
	float hauteur;

	//Contours du bouton
	vector <vector<float>> contourXY;

	string texte = ""; // texte dans le bouton

	Bouton(float pPosX, float pPosY, float pLongueur, float pLargeur, string pTexte)
	{
		posX = pPosX;
		posY = pPosY;
		longueur = pLongueur;
		hauteur = pLargeur;
		texte = pTexte;
		createBouton();
	}

	void createBouton()
	{
		x1 = posX - longueur / 2;
		x2 = posX + longueur / 2;
		y1 = posY - hauteur / 2;
		y2 = posY + hauteur / 2;

		contourXY = { {x1, x2, x2, x1}, {y2, y2, y1, y1} };
	}

	// permet de savoir si on clique sur le bouton
	bool EstDansLeBouton(const double xCursor, const double yCursor)
	{
		if (xCursor > x1 && xCursor < x2 && yCursor > y1 && yCursor < y2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};


struct Zone
{
	// Position du centre de la zone
	float posX;
	float posY;

	// Position pour les quatre coins de la zone
	float x1, x2;
	float y1, y2;

	// Longueur et hauteur de la zone
	float longueur;
	float hauteur;

	// Type de zone (1 a 6) -- ATTENTION IL MANQUE LA VERIFICATION !!!!!!! --
	int type;

	// Contour de la zone
	vector <vector<float>> contourXY;

	Zone(float pPosX, float pPosY, float pLongueur, float pLargeur, int pType)
	{
		posX = pPosX;
		posY = pPosY;
		longueur = pLongueur;
		hauteur = pLargeur;
		type = pType;
		createZone();
	}

	void createZone()
	{
		x1 = posX - longueur / 2;
		x2 = posX + longueur / 2;
		y1 = posY - hauteur / 2;
		y2 = posY + hauteur / 2;

		contourXY = { {x1, x2, x2, x1}, {y2, y2, y1, y1} };
	}
};


struct Puzzle
{
	// En developpement
	//
	//
};