#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"

#include "Box.cpp"

GLuint program;
Core::Shader_Loader shaderLoader;


void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	// ZADANIE: W zalaczonym wyzej pliku Box.cpp znajduja sie tablice pozycji i kolorow wierzcholkow prostopadloscianu. Przesun go o wektor (0.5, 0.5, -0.2) i wyswietl go.
	// 
	// Do shadera uzytego w tym zadaniu nalezy przeslac nie tylko pozycje, ale rowniez kolory wierzcholkow.
	// Jednak nasza funkcja "void DrawVertexArray(const float *, int, int)" umozliwia wyslanie tylko jednego atrybutu wierzcholkow.
	// 
	// W pliku Render_Utils.h znajdziesz deklaracje i opis nowej funkcji "void DrawVertexArray(const VertexData &)", ktora pozwala podac wiele atrybutow.
	// (Pozycje powinny zostac wyslane w atrybucie 0, a kolory w atrybucie 1.)

	glUseProgram(program);

	glm::mat4 transformation;
	transformation[3][0] = 0.5;
	transformation[3][1] = 0.5;
	transformation[3][2] = -0.2;

	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);

	Core::VertexData vertexData;
	vertexData.NumActiveAttribs = 2;
	vertexData.Attribs[0].Pointer = boxPositions;
	vertexData.Attribs[0].Size = 4;
	vertexData.Attribs[1].Pointer = boxColors;
	vertexData.Attribs[1].Size = 4;
	vertexData.NumVertices = 36;

	Core::DrawVertexArray(vertexData);
	
	



	glUseProgram(0);
	glutSwapBuffers();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_2_1.vert", "shaders/shader_2_1.frag");
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
