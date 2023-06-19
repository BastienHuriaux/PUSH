#include "puzzle.h"


vector <shared_ptr<Piece>> pieceArray;
vector <shared_ptr<Piece>> puzzleArray;

// Called when a button is pressed 
// Create a piece, clear the puzzle, execute the command
void useButton(string pButtonText)
{
	
	if (pButtonText.compare("Clear") == 0)
	{
		// Suppress all pieces in the puzzle 
		puzzleArray.clear();
		CommandOutput.clear();
		// Create a new input to begin the puzzle
		shared_ptr<Processus> inStart = make_shared<Processus>();
		inStart->x0 = -0.4;
		inStart->y0 = 0.5;
		puzzleArray.insert(puzzleArray.end(), inStart);
	}
	else if (pButtonText.compare("Execute") == 0)
	{
		// Execute the command line
		my_popen(CommandHidden, CommandOutput);
		CommandExecuted = true;
		my_popen("cat /tmp/error", CommandError);

		// Reinitialization of the command sentence and clear of all pieces in the puzzle
		for (int i = 0; i < puzzleArray.size(); i++)
		{
			puzzleArray[i]->text = "";
		}
		CommandSentence = "";
		CommandHidden = "";
		string ErrorFile;
		my_popen("rm /tmp/error", ErrorFile);
		chdir(PathCD.c_str());
	}
}

void createPieceSimiliButton(shared_ptr<Piece>& pPiece)
{
	// Different pieces are created and the right one will be inserted in the piece array 
	shared_ptr<Processus> newProcessus = make_shared<Processus>();
	shared_ptr<In> newIn = make_shared<In>();
	shared_ptr<Out> newOut = make_shared<Out>();
	shared_ptr<Error> newError = make_shared<Error>();
	shared_ptr<Tube> newTube = make_shared<Tube>();

	// Try which button is pressed and add a new piece accordingly
	if (typeid(*pPiece) == typeid(Processus))
	{
		newProcessus->x0 = -0.8;
		newProcessus->y0 = 0.86;
		pieceArray.insert(pieceArray.end(), newProcessus);
	}
	else if (typeid(*pPiece) == typeid(In))
	{
		newIn->x0 = -0.5;
		newIn->y0 = 0.86;
		pieceArray.insert(pieceArray.end(), newIn);
	}
	else if (typeid(*pPiece) == typeid(Out))
	{
		newOut->x0 = -0.2;
		newOut->y0 = 0.86;
		pieceArray.insert(pieceArray.end(), newOut);
	}
	else if (typeid(*pPiece) == typeid(Error))
	{
		newError->x0 = 0.1;
		newError->y0 = 0.86;
		pieceArray.insert(pieceArray.end(), newError);
	}
	else if (typeid(*pPiece) == typeid(Tube))
	{
		newTube->x0 = 0.4;
		newTube->y0 = 0.86;
		pieceArray.insert(pieceArray.end(), newTube);
	}

}

// Called when a left click is done inside a piece 
// Deplacement of a piece in concordance with the cursor position
void movePiece(shared_ptr<Piece>& pPiece, float pCursorX, float pCursorY)
{
	if (pPiece->aIsInGameArea)
	{
		// The piece can be moved in a limited space
		if (pCursorX > -0.90 && pCursorX < 0.40
			&& pCursorY > -0.65 && pCursorY < 0.65)
		{
			pPiece->x0 = pCursorX; 
			pPiece->y0 = pCursorY; 
			glutPostRedisplay(); 
		}
	}
	// The piece can be moved in a limited space
	else
	{
		if (pCursorX > -0.90 && pCursorX < 0.40
			&& pCursorY > -0.65 && pCursorY < 0.90)
		{
			pPiece->x0 = pCursorX;
			pPiece->y0 = pCursorY;

			if (pCursorY <= 0.65)
			{
				pPiece->aIsInGameArea = true;
				createPieceSimiliButton(pPiece);
			}
			glutPostRedisplay();
		}
	}	
}

// Called when a right click is done inside a piece
// Suppress a piece
void suppressPiece(int indice)
{
	pieceArray.erase(pieceArray.begin() + indice);
}

// Called when one piece is in contact with the puzzle 
// Assembling of the puzzle
void avengersPuzzle(shared_ptr<Piece>& pPiece, shared_ptr<Piece>& pPuzzle)
{
	// Comparison of the piece type
	if (typeid(*pPiece) == typeid(Processus))
	{
		Processus* p = dynamic_cast<Processus*>(pPiece.get());
		if (typeid(*pPuzzle) == typeid(In))
		{
			In* d = dynamic_cast<In*>(pPuzzle.get());

			// Processus piece goes next to In in puzzle
			if (p->inProcessus == true && d->outIn == true)
			{
				// Lock entries
				p->inProcessus = false;
				d->outIn = false;

				// Change which vector the piece is in
				puzzleArray.insert(puzzleArray.end(), pPiece);
				pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

				// Put the piece next to the one it is attached to
				p->x0 = d->x0 + 0.1;
				p->y0 = d->y0;
			}
		}
		else if (typeid(*pPuzzle) == typeid(Out))
		{
			Out* d = dynamic_cast<Out*>(pPuzzle.get());

			// Processus piece goes next to Out in puzzle
			if (p->outProcessus == true && d->inOut == true)
			{
				// Lock entries
				p->outProcessus = false;
				d->inOut = false;

				// Change which vector the piece is in
				puzzleArray.insert(puzzleArray.end(), pPiece);
				pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

				// Put the piece next to the one it is attached to
				p->x0 = d->x0 - 0.1;
				p->y0 = d->y0;
			}
		}
		else if (typeid(*pPuzzle) == typeid(Error))
		{
			Error* d = dynamic_cast<Error*>(pPuzzle.get());

			// Processus piece goes next to Error in puzzle 
			if (p->errorProcessus == true && d->inError == true)
			{
				// Lock entries
				p->errorProcessus = false;
				d->inError = false;

				// Change which vector the piece is in
				puzzleArray.insert(puzzleArray.end(), pPiece);
				pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

				// Put the piece next to the one it is attached to
				p->x0 = d->x0;
				p->y0 = d->y0 + 0.1;
			}
		}
		else if (typeid(*pPuzzle) == typeid(Tube))
		{
			Tube* d = dynamic_cast<Tube*>(pPuzzle.get());

			// Processus piece goes next to Tube in puzzle
			if (p->inProcessus == true && d->outTube == true)
			{
				// Lock entries
				p->inProcessus = false;
				d->outTube = false;

				// Change which vector the piece is in
				puzzleArray.insert(puzzleArray.end(), pPiece);
				pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

				// Put the piece next to the one it is attached to
				p->x0 = d->x0 + 0.1;
				p->y0 = d->y0;
			}
		}
	}
	else if (typeid(*pPiece) == typeid(In) && typeid(*pPuzzle) == typeid(Processus))
	{
		In* p = dynamic_cast<In*>(pPiece.get());
		Processus* d = dynamic_cast<Processus*>(pPuzzle.get());

		// In piece goes next to Processus in puzzle
		if (p->outIn == true && d->inProcessus == true)
		{
			// Lock entries
			p->outIn = false;
			d->inProcessus = false;

			// Change which vector the piece is in
			vector<shared_ptr<Piece>>::iterator vPosition;
			vPosition = find(puzzleArray.begin(), puzzleArray.end(), pPuzzle);

			// Add the Error in the puzzle array after the right Processus
			puzzleArray.insert(vPosition + 1, pPiece);

			// Delete the Error piece from the piece array
			pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

			// Put the piece next to the one it is attached to
			p->x0 = d->x0 - 0.1;
			p->y0 = d->y0;
		}
	}
	else if (typeid(*pPiece) == typeid(Out) && typeid(*pPuzzle) == typeid(Processus))
	{
		Out* p = dynamic_cast<Out*>(pPiece.get());
		Processus* d = dynamic_cast<Processus*>(pPuzzle.get());

		// Out piece goes next to Processus in puzzle
		if (p->inOut == true && d->outProcessus == true)
		{
			// Lock entries
			p->inOut = false;
			d->outProcessus = false;

			// Change which vector the piece is in
			puzzleArray.insert(puzzleArray.end(), pPiece);
			pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

			// Put the piece next to the one it is attached to
			p->x0 = d->x0 + 0.1;
			p->y0 = d->y0;
		}
	}
	else if (typeid(*pPiece) == typeid(Error) && typeid(*pPuzzle) == typeid(Processus))
	{
		Error* p = dynamic_cast<Error*>(pPiece.get());
		Processus* d = dynamic_cast<Processus*>(pPuzzle.get());

		// Error piece goes next to Processus in puzzle
		if (p->inError == true && d->errorProcessus == true)
		{
			// Lock entries
			p->inError = false;
			d->errorProcessus = false;

			// Get the position of the right Processus in the puzzle array
			vector<shared_ptr<Piece>>::iterator vPosition;
			vPosition = find(puzzleArray.begin(), puzzleArray.end(), pPuzzle);

			// Add the Error in the puzzle array after the right Processus
			puzzleArray.insert(vPosition + 1, pPiece);

			// Delete the Error piece from the piece array
			pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

			// Put the piece next to the one it is attached to
			p->x0 = d->x0;
			p->y0 = d->y0 - 0.1;
		}
	}
	else if (typeid(*pPiece) == typeid(Tube) && typeid(*pPuzzle) == typeid(Processus))
	{
		Tube* p = dynamic_cast<Tube*>(pPiece.get());
		Processus* d = dynamic_cast<Processus*>(pPuzzle.get());

		// Tube piece goes next to Processus in puzzle
		if (p->inTube == true && d->outProcessus == true)
		{
			// Lock entries
			p->inTube = false;
			d->outProcessus = false;

			// Change which vector the piece is in
			puzzleArray.insert(puzzleArray.end(), pPiece);
			pieceArray.erase(remove(pieceArray.begin(), pieceArray.end(), pPiece), pieceArray.end());

			// Put the piece next to the one it is attached to
			p->x0 = d->x0 + 0.1;
			p->y0 = d->y0;
		}
	}
}

// Called when the cursor is in a piece -- useful when moving the said piece --
// Dectect if a piece touches the puzzle and add it to the puzzle if possible
void collisionRectRect()
{
	// Check every piece in piece array and puzzle array
	for (int i = 0; i < pieceArray.size(); i++)
	{
		for (int j = 0; j < puzzleArray.size(); j++)
		{
			// Check if two rectangles - meaning two pieces - touch	
			if (pieceArray[i]->x0 - 0.05 < puzzleArray[j]->x0 + 0.05 && pieceArray[i]->x0 + 0.05 > puzzleArray[j]->x0 - 0.05 &&
				pieceArray[i]->y0 - 0.05 < puzzleArray[j]->y0 + 0.05 && pieceArray[i]->y0 + 0.05 > puzzleArray[j]->y0 - 0.05)
			{
				// Assembling of the puzzle
				avengersPuzzle(pieceArray[i], puzzleArray[j]);

				// Allow to not get out of the vector
				return;
			}
		}
	}
}