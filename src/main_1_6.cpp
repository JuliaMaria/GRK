#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"

GLuint program;
Core::Shader_Loader shaderLoader;

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	glUseProgram(program);
	glm::mat4 animation;


	// ZADANIE: Wyswietl czworokat, ktory jednoczesnie przesuwa sie i obraca.
	// 
	// Uzyj kodu do translacji i rotacji czworokatu z poprzednich zadan
	// 
	// Stworz taka macierz transformacji, ktora powoduje ze czworokat przesuwa sie do gory i na dol ekranu a jednoczesnie obraca sie.
	// Wyslij stworzona macierz do GPU za pomoca funkcji glUniformMatrix4fv zamiast macierzy "translation" i "rotation" z poprzednich zadan


	// Uzyj kodu do rysowania czworokatu z poprzedniego zadania

	const float vertices[] = { 0.5, 0.0, 0.0, 1.0,
		0.5, 1.0, 0.0, 1.0,
		0.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0 };

	const int indexes[] = { 0, 1, 2, 1 ,2 ,3 };
	Core::DrawVertexArrayIndexed(vertices, indexes, 6, 4);

	animation[0][0] = cos(time);
	animation[0][1] = sin(time);
	animation[1][0] = -sin(time);
	animation[1][1] = cos(time);

	animation[3][1] = sin(time);

	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&animation);

	glUseProgram(0);
	glutSwapBuffers();
}

void init()
{
	program = shaderLoader.CreateProgram("shaders/shader_1_2.vert", "shaders/shader_1_2.frag");
}

void shutdown()
{
	shaderLoader.DeleteProgram(program);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL Pierwszy Program");
	glewInit();

	init();
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
