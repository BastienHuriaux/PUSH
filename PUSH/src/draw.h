#ifndef DRAW_H
#define DRAW_H

#pragma once
#include <vector>
#include <filesystem>
#include <unistd.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glut_functions.h"

void drawShape(std::vector<std::vector<float>> pCoords, float pRed, float pGreen, float pBlue, int pDrawingType);

void drawTriangles(std::vector<std::vector<float>> pCoords, float r, float g, float b);

void drawWriting(float x, float y, float r, float g, float b, std::string string, void* pTypeOfPolice);

void drawCommandOutput(float pY, std::string& pCommand);

#endif