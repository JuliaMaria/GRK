#include "glew.h"
#include "freeglut.h"

#include "Shader_Loader.h"
#include "Render_Utils.h"

GLuint program;
Core::Shader_Loader shaderLoader;

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	glUseProgram(program);

	// ZADANIE: Stworz czworokat z czterech wierzcholkkow i szesciu indeksow (typu const int []) i narysuj go za pomoca funkcji Core::DrawVertexArrayIndexed z Render_Utils.cpp

	const float vertices[] = { 0.5, 0.0, 0.0, 1.0,
	0.5, 1.0, 0.0, 1.0,
	0.0, 0.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0 };

	const int indexes[] = { 0, 1, 2, 1 ,2 ,3 };
	Core::DrawVertexArrayIndexed(vertices, indexes, 6, 4);

	glUseProgram(0);
	glutSwapBuffers();
}

void init()
{
	program = shaderLoader.CreateProgram("shaders/shader_1_1.vert", "shaders/shader_1_1.frag");
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
