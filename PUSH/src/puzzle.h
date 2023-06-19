#ifndef PUZZLE_H
#define PUZZLE_H

#pragma once

#include <vector>
#include <filesystem>
#include <unistd.h>

#include <GL/freeglut.h>

#include "structure.cpp"
#include "command.h"

extern vector <shared_ptr<Piece>> pieceArray;
extern vector <shared_ptr<Piece>> puzzleArray;

void useButton(string pButtonText);

void movePiece(shared_ptr<Piece>& pPiece, float pCursorX, float pCursorY);

void suppressPiece(int indice);

void avengersPuzzle(shared_ptr<Piece>& pPiece, shared_ptr<Piece>& pPuzzle);

void collisionRectRect();

#endif