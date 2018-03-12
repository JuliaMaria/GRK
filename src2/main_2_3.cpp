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

glm::mat4 createPerspectiveMatrix()
{
	const float zNear = 0.1f, zFar = 10.0f;
	const float frustumScale = 1.0f;
	glm::mat4 perspective;
	perspective[0][0] = frustumScale;
	perspective[1][1] = frustumScale;
	perspective[2][2] = (zFar + zNear) / (zNear - zFar);
	perspective[3][2] = (2 * zFar * zNear) / (zNear - zFar);
	perspective[2][3] = -1;
	perspective[3][3] = 0;

	return perspective;
}

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	// ZADANIE: Dodaj drugie, mniejsze pudelko do sceny (uzyj skalowania). Spowoduj, zeby obracalo sie wokol pierwszego pudelka.
	// (Wyobraz sobie, ze pierwsze pudelko to Ziemia obracajaca sie wokol Slonca, a drugie to Ksiezyc obracajacy sie wokol Ziemi).
	// 
	// Sprawdz co sie stanie gdy z-bufor jest wylaczony.

	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	glUseProgram(program);

	glm::mat4 transformation;
	transformation[3][0] = 0.5;
	transformation[3][1] = 0.5;
	transformation[3][2] = -2.0;

	glm::mat4 perspective = createPerspectiveMatrix();

	glm::mat4 rotation;

	rotation[0][0] = cos(time);
	rotation[0][1] = sin(time);
	rotation[1][0] = -sin(time);
	rotation[1][1] = cos(time);

	transformation = perspective*rotation*transformation;


	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);





	Core::VertexData vertexData;
	vertexData.NumActiveAttribs = 2;
	vertexData.Attribs[0].Pointer = boxPositions;
	vertexData.Attribs[0].Size = 4;
	vertexData.Attribs[1].Pointer = boxColors;
	vertexData.Attribs[1].Size = 4;
	vertexData.NumVertices = 36;

	Core::DrawVertexArray(vertexData);


	glm::mat4 transformation2;
	transformation2[3][0] = 4.0;
	transformation2[3][1] = 4.0;
	transformation2[3][2] = -5.0;

	glm::mat4 scaling2;
	scaling2[0][0] = 0.5;
	scaling2[1][1] = 0.5;

	transformation2 = transformation2*scaling2;


	glm::mat4 rotation2;

	rotation2[0][0] = cos(time);
	rotation2[0][1] = sin(time);
	rotation2[1][0] = -sin(time);
	rotation2[1][1] = cos(time);

	transformation2 = rotation2*transformation2;

	transformation2 = transformation*transformation2;


	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation2);

	Core::DrawVertexArray(vertexData);


	glUseProgram(0);
	glutSwapBuffers();
}

void init()
{
	// Ponizsza linijka wlacza Z-bufor. Zmien glEnable na glDisable aby go wylaczyc.
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
