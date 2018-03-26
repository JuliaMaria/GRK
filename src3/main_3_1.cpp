#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"

#include "Box.cpp"

// ZADANIE:
// Wczytaj w funkcji init() dwa modele - sfery i statku - z plikow models/spaceship.obj i models/sphere.obj.
// Uzyj zmiennych sphereModel i shipModel, oraz funkcji obj::loadModelFromFile(const char * filepath).
// 
// Nastepnie uzyj funkcji Core::DrawModel(obj::Model*) aby wyswietlic jeden z obiektow w poczatku ukladu wspolrzednych.
// 
// (Macierze widoku i rzutowania sa juz stworzone, a kamera jest odsunieta i patrzy w strone poczatku ukladu).
//
// Stworz zmienna "uniform vec3 objectColor;" w pliku shader_2_1.frag, ktorej uzyjesz do przeslania koloru obiektu w formacie RGB do shadera.
// Zmienna wbudowana gl_FragColor w shaderze jest typu vec4 - RGBA. Do czesci RGB przypisz kolor ze zmiennej objectColor, a do A (kanal alpha) - 1.0f.
// Mozesz teraz kontrolowac kolor rysowanego obiektu, wysylajac przed rysowaniem zmienna objectColor przy uzyciu funkcji glUniform3f( int location, float v0, float v1, float v2 )

GLuint program;
Core::Shader_Loader shaderLoader;

obj::Model shipModel;
obj::Model sphereModel;


float cameraAngle = 0;
glm::vec3 cameraPos = glm::vec3(-5, 0, 0);
glm::vec3 cameraDir;

void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.1f;
	float moveSpeed = 0.1f;
	switch(key)
	{
	case 'a': cameraAngle -= angleSpeed; break;
	case 'd': cameraAngle += angleSpeed; break;
	case 'w': cameraPos += cameraDir * moveSpeed; break;
	case 's': cameraPos -= cameraDir * moveSpeed; break;
	}
}

glm::mat4 createCameraMatrix()
{
	cameraDir = glm::vec3(cosf(cameraAngle), 0.0f, sinf(cameraAngle));
	glm::vec3 up = glm::vec3(0,1,0);

	return Core::createViewMatrix(cameraPos, cameraDir, up);
}

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	glUseProgram(program);

	glm::mat4 camera = createCameraMatrix();
	glm::mat4 perspective = Core::createPerspectiveMatrix();
	glm::mat4 transformation;

	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);

	glUniform3f(glGetUniformLocation(program, "objectColor"), 0.0, 0.99, 0.12);
	Core::DrawModel(&shipModel);

	glUseProgram(0);
	glutSwapBuffers();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_2_1.vert", "shaders/shader_2_1.frag");
	shipModel = obj::loadModelFromFile("models/spaceship.obj");
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	
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
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
