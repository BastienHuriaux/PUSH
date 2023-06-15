#ifndef GLUT_FUNCTIONS_H
#define GLUT_FUNCTIONS_H

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glui.h>

#include "draw.h"
#include "command.h"

extern Zone ZonesArray[5];
extern Button ButtonsArray[2];
extern int main_window;
extern GLUI* glui;
extern GLUI_EditText* EditText;
extern std::string PieceOnCursorText;

void control(int id);

void glutDisplay();

void glutMotion(int x, int y);

void glutPassiveMotion(int x, int y);

void glutMouse(int button, int state, int x, int y);

void glutKeyboard(unsigned char key, int x, int y);

void init();

#endif