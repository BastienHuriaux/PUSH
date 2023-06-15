// PUSH.cpp : définit le point d'entrée de l'application.
//
#include "PUSH.hpp"

int main(int argc, char** argv)
{
	init();

	// Main loop
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1920, 1080);
	main_window = glutCreateWindow("PUSH");
	glutMouseFunc(glutMouse);
	glutMotionFunc(glutMotion);
	glutPassiveMotionFunc(glutPassiveMotion);
	glutDisplayFunc(glutDisplay);

	glutMainLoop();

	return 0;
}