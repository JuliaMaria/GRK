#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"

#include "Box.cpp"

// ZADANIE: Odsun kamere od pudelka zamiast pudelko od kamery. Uzyj kodu do rysowania pudelka z poprzednich zadan.
// 
// Nalezy stworzyc odpowiednia macierz kamery (w funkcji createCameraMatrix). Do shadera nalezy wyslac macierz zawierajaca zlozenie obu transformacji (kamery i projekcji).
// Nie dokonujemy transformacji samego pudelka - ma ono znajdowac sie w poczatku ukladu wspolrzednych.

GLuint program;
Core::Shader_Loader shaderLoader;

glm::mat4 createCameraMatrix()
{
	// Uzupelnij kod tworzacy macierz kamery. Oblicz wektory fwd, up i side, oraz ustaw odpowiednio macierze cameraRotation i cameraTranslation.

	// Uzycie zmiennej time sprawia, ze kamera obraca sie po okregu o promieniu 2.
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	const float radius = 2.0f;
	glm::vec3 camPos = glm::vec3(cosf(time), 0, sinf(time)) * radius;

	// Skoro chcemy patrzec na pudelko lezace w poczatku ukladu wspolrzednych, wektor patrzenia "fwd" powinien byc caly czas przeciwny do wektora pozycji kamery.
	// Wektor "up" jest skierowany do gory. Wektor "side" jest iloczynem wektorowym poprzednich dwoch wektorow. Funkcja glm::cross(vec3 a, vec3 b) zwraca iloczyn wektorowy dwoch wektorow a i b.
	// Funkcja glm::normalize(vec3 v) zwraca wektor jednostkowy o tym samym kierunku co v.
	glm::vec3 fwd;
	fwd = -camPos;
	glm::vec3 up = { 0, 1, 0 };
	glm::vec3 side = normalize(cross(fwd, up));

	// Trzeba pamietac o minusie przy ustawianiu osi Z kamery.
	// Wynika to z tego, ze standardowa macierz perspektywiczna zaklada, ze "z przodu" jest ujemna (a nie dodatnia) czesc osi Z.
	glm::mat4 cameraRotation;
	cameraRotation[0][0] = side[0];
	cameraRotation[1][0] = side[1];
	cameraRotation[2][0] = side[2];
	cameraRotation[0][1] = up[0];
	cameraRotation[1][1] = up[1];
	cameraRotation[2][1] = up[2];
	cameraRotation[0][2] = fwd[0];
	cameraRotation[1][2] = fwd[1];
	cameraRotation[2][2] = fwd[2];

	glm::mat4 cameraTranslation;
	cameraTranslation[3][0] = -fwd[0];
	cameraTranslation[3][1] = -fwd[1];
	cameraTranslation[3][2] = -fwd[2];
	
	return cameraRotation * cameraTranslation;
}

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

	glUseProgram(program);

	glm::mat4 camera = createCameraMatrix();
	glm::mat4 perspective = createPerspectiveMatrix();

	// Oblicz odpowiednia transformacje.
	glm::mat4 transformation;
	transformation = perspective*camera*transformation;

	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);

	// Narysuj pudelko w poczatku ukladu wspolrzednych.


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
