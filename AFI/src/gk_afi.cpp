#include <iostream>
#include "zpr.h"
#include "GL/glut.h"
#include <osg/Matrix>
#include <osg/Vec3>
#include <osg/Quat>

//! Macierz opisuj¹ca czajnik
osg::Matrix teapotMatrix;
//! Macierz opisuj¹ca torus
osg::Matrix torusMatrix;
//! Macierz opisuj¹ca szeœcian
osg::Matrix cubeMatrix;

//! TODO
//! Tutaj inicjalizowane s¹ pierwsze pozycje obiektów
void initObjects()
{
	teapotMatrix = osg::Matrix::translate(1, 1, 1);
	torusMatrix = osg::Matrix::translate(0.5, 1, -1);
	cubeMatrix = osg::Matrix::translate(0.5, 1, 1);
}

//! TODO
//! Tutaj aktualizaowana jest pozycja czajnika
void updateTeapot()
{
	osg::Vec3d pos = teapotMatrix.getTrans();
	static osg::Vec3d p1 = cubeMatrix.getTrans(); //punkt, wzgledem ktorego bedzie tworzona pierwsza czesc osemki
	static osg::Vec3d p2(1.5, 1, 1); //punkt, wzgledem ktorego bedzie tworzona pierwsza czesc osemki
	static int step = 0;

	if (step < 40)
	{
		teapotMatrix *= osg::Matrix::translate(-p1);
		teapotMatrix *= osg::Matrix::rotate(osg::DegreesToRadians(9.f), osg::Vec3d(0, 0, 1));
		teapotMatrix *= osg::Matrix::translate(p1);
	}
	else
	{
		teapotMatrix *= osg::Matrix::translate(-p2);
		teapotMatrix *= osg::Matrix::rotate(osg::DegreesToRadians(-9.f), osg::Vec3d(0, 0, 1));
		teapotMatrix *= osg::Matrix::translate(p2);
	}

	step = (step + 1) % 80;
}

//! TODO
//! Tutaj aktualizowana jest pozycja torusa
void updateTorus()
{
	osg::Vec3d pos = torusMatrix.getTrans();
	osg::Vec3d point = cubeMatrix.getTrans(); // punkt odniesienia
	static double angle = 270; // w stopniach
	osg::Vec3d angleVec; //modyfikator zmieniajacy promien okregu (tworzac efekt elipsy)
	const double step = 1.8; //rozmiar kroku

	angleVec = osg::Vec3d(cos(osg::DegreesToRadians(angle)), 0, 0);

	//obrot wokol wlasnej osi
	torusMatrix *= osg::Matrix::translate(-pos);
	torusMatrix *= osg::Matrix::rotate(osg::DegreesToRadians(step), osg::Vec3d(0, 0, 1));
	torusMatrix *= osg::Matrix::translate(pos);


	//obrot wokol punktu
	torusMatrix *= osg::Matrix::translate(-point - angleVec); //-angleVec - usun modyfikacje na czas obracania
	torusMatrix *= osg::Matrix::rotate(osg::DegreesToRadians(step), osg::Vec3d(0, 1, 0));
	torusMatrix *= osg::Matrix::translate(point); //nie przywracaj angleVec. zaraz zostanie dodana zaktualizowana wartosc

	//zmiana promienia
	angle = fmod(angle + step, 360);
	angleVec = osg::Vec3d(cos(osg::DegreesToRadians(angle)), 0, 0);
	torusMatrix *= osg::Matrix::translate(angleVec);
	//voila!
}

//! TODO
//! Tutaj aktualizowana jest pozycja szeœcianu
void updateCube()
{
	const float scaleStep = 1.01;
	static int step = 50;
	osg::Vec3d pos = cubeMatrix.getTrans();

	cubeMatrix *= osg::Matrix::translate(-pos);

	if (step >= 50)
		cubeMatrix *= osg::Matrix::scale(scaleStep, scaleStep, scaleStep);
	else
		cubeMatrix *= osg::Matrix::scale(1.0 / scaleStep, 1.0 / scaleStep, 1.0 / scaleStep);

	step = (step + 1) % 100;

	cubeMatrix *= osg::Matrix::translate(pos);
}

//! \param objectMatrix Macierz pisuj¹ca pozycjê 3D obiektu na scenie
void refreshObject(const osg::Matrix& objectMatrix)
{
	auto t = objectMatrix.getTrans();
	glTranslated(t.x(), t.y(), t.z());

	auto s = objectMatrix.getScale();
	glScaled(s.x(), s.y(), s.z());

	double angle, x, y, z;
	objectMatrix.getRotate().getRotate(angle, x, y, z);
	glRotated(osg::RadiansToDegrees(angle), x, y, z);
}

//! Metoda odrysowywuje osie uk³adu
void drawAxes()
{
	/* Name-stack manipulation for the purpose of
	   selection hit processing when mouse button
	   is pressed.  Names are ignored in normal
	   OpenGL rendering mode.                    */

	glPushMatrix();
	/* No name for grey sphere */

	glColor3f(0.3, 0.3, 0.3);
	glutSolidSphere(0.07, 20, 20);

	glPushMatrix();
	glPushName(1);            /* Red cone is 1 */
	glColor3f(1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glutSolidCone(0.06, 0.4, 20, 20);
	glPopName();
	glPopMatrix();

	glPushMatrix();
	glPushName(2);            /* Green cone is 2 */
	glColor3f(0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.06, 0.4, 20, 20);
	glPopName();
	glPopMatrix();

	glColor3f(0, 0, 1);         /* Blue cone is 3 */
	glPushName(3);
	glutSolidCone(0.06, 0.4, 20, 20);
	glPopName();

	glPopMatrix();
}

// Drawing (display) routine.
void drawScene()
{
	// Clear screen to background color.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Pude³ko
	glPushMatrix();
	glColor4f(1, 0, 0, 0.9);
	refreshObject(cubeMatrix);
	glutSolidCube(0.4);
	glPopMatrix();

	// Torus
	glPushMatrix();
	glColor4f(1, 0, 0, 0.9);
	refreshObject(torusMatrix);
	glutSolidTorus(0.05, 0.4, 10, 10);
	glPopMatrix();

	// Czajnik
	glPushMatrix();
	glColor4f(0, 1, 0, 0.9);
	refreshObject(teapotMatrix);
	glutSolidTeapot(0.2);
	glPopMatrix();

	// Draw orientation axis
	drawAxes();

	// Swap buffers for double buffering
	glutSwapBuffers();
}

//! Metoda realizuj¹ca obliczenia na potrzeby kolejnych klatek, generuje animacjê
void animate() {

	updateTeapot();
	updateTorus();
	updateCube();

	glutPostRedisplay();
}
//! Zmienne opisuj¹ce materia³ i œwiat³o OpenGL
const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

// Initialization routine.
void setup()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5, 0.5, 0.5, 0.5);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	// Register display routine.
	glutDisplayFunc(drawScene);
	// Register idle routine
	glutIdleFunc(animate);
	// Initialize camera manipulator
	zprInit();
	// Initialize first object positions
	initObjects();
}

// Main routine: defines window properties, creates window,
// registers callback routines and begins processing.
int main(int argc, char** argv)
{
	// Initialize GLUT.
	glutInit(&argc, argv);

	// Set display mode as double-buffered, RGB color and depth.
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// Set OpenGL window size.
	glutInitWindowSize(800, 800);

	// Set position of OpenGL window upper-left corner.
	glutInitWindowPosition(50, 50);

	// Create OpenGL window with title.
	glutCreateWindow("Laboratorium GK: AFI");

	// Initialize.
	setup();

	// Begin processing.
	glutMainLoop();

	return 0;
}