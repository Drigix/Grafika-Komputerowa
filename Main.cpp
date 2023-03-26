/////////////////////////////////////////////
//
// Skeletal Animation 
//
/////////////////////////////////////////////
#include <iostream> 
#include <vector> 
#include <string> 
#include <stdio.h>
#include <glew.h>
#include <wglew.h>
#include <cstdlib>
#include <windows.h>
#include <mmsystem.h>
#include <GL/glut.h>
//using namespace std;
#pragma comment(lib,"winmm.lib")
///////////////////////////////////////////
#include "core.h"
#include "Bmp.h"
#include "ogl.h"
#include "glsl.h"
///////////////////////////////////////////
vec4f lightvec(-1, 0, -1, 0);
#include "Mesh.h"

/////////////////////////////////////////////////////////
//
// EXERCISES
//
/////////////////////////////////////////////////////////

///////////////////////////////////////////
void printBone(Mesh halo, int index, int level) {
	//TO DO 
}
///////////////////////////////////////////


void AnimateBone(Mesh& mesh, char* boneName, float phiX, float phiY, float phiZ) {
	//TO DO
}

void updateChilds(Mesh& mesh, int index) {

	MeshAnimation::TBone b = mesh.animation.bones[index];
	matrix44 m;

	// bind pose : default
	vec3f pos(b.pos[0], b.pos[1], b.pos[2]);
	m.set(b.rot[0], b.rot[1], b.rot[2], b.rot[3]);
	m.set_translation(pos);
	if (mesh.animation.bones[index].parent >= 0)
		mesh.animation.bones[index].matrix = m * mesh.animation.bones[b.parent].matrix;
	else
		mesh.animation.bones[index].matrix = m;
	//std::cout << b.name << ", ";
	loopi(0, b.childs.size())
	{
		int childIndex = b.childs[i];
		updateChilds(mesh, childIndex);
	}
}

/////////////////////////////////////////////////////////

struct ParentCoords {
	double x, y, z;
};

inline double convertRadians(double degrees) {
	return degrees / 360 * 2 * 3.14;
}

void rotate_x(Mesh& mesh, int index, int degrees) {
	vector3 pos = mesh.animation.bones[index].matrix.pos_component();
	mesh.animation.bones[index].matrix.set_translation(vector3(-pos[0], -pos[1], -pos[2]));
	mesh.animation.bones[index].matrix.rotate_x(convertRadians(degrees));
	mesh.animation.bones[index].matrix.set_translation(vector3(pos[0], pos[1], pos[2]));
}

void rotate_y(Mesh& mesh, int index, int degrees) {
	vector3 pos = mesh.animation.bones[index].matrix.pos_component();
	mesh.animation.bones[index].matrix.set_translation(vector3(-pos[0], -pos[1], -pos[2]));
	mesh.animation.bones[index].matrix.rotate_y(convertRadians(degrees));
	mesh.animation.bones[index].matrix.set_translation(vector3(pos[0], pos[1], pos[2]));
}

void rotate_z(Mesh& mesh, int index, int degrees) {
	vector3 pos = mesh.animation.bones[index].matrix.pos_component();
	mesh.animation.bones[index].matrix.set_translation(vector3(-pos[0], -pos[1], -pos[2]));
	mesh.animation.bones[index].matrix.rotate_z(convertRadians(degrees));
	mesh.animation.bones[index].matrix.set_translation(vector3(pos[0], pos[1], pos[2]));
}

void translate_x(Mesh& mesh, int index, double value) {
	mesh.animation.bones[index].matrix.translate(vector3(value, 0, 0));
}

void translate_y(Mesh& mesh, int index, double value) {
	mesh.animation.bones[index].matrix.translate(vector3(0, value, 0));
}

void translate_z(Mesh& mesh, int index, double value) {
	mesh.animation.bones[index].matrix.translate(vector3(0, 0, value));
}

enum BodyPartEnum : int {
	CORE = 0,

	RIGHT_UPPER_ARM = 1,
	RIGHT_LOWER_ARM = 2,
	RIGHT_WRIST = 3,

	LEFT_UPPER_ARM = 4,
	LEFT_LOWER_ARM = 5,
	LEFT_WRIST = 6,

	SACRUM = 7,

	RIGHT_UPPER_LEG = 8,
	RIGHT_LOWER_LEG = 9,
	RIGHT_FOOT = 10,
	RIGHT_FOOT_FINGERS = 11,

	LEFT_UPPER_LEG = 12,
	LEFT_LOWER_LEG = 13,
	LEFT_FOOT = 14,
	LEFT_FOOT_FINGERS = 15,

	HEAD = 16,
	UPPER_HEAD = 17
};



double zRotation = 0.0;
double xPosition = 0.0;
double xPosition22 = 0.0;

bool bendDirection = true;

void bendKnee(Mesh& mesh) {
	double rotationDelta = 0.00045 * 5;
	double translationDelta = 0.00001 * 5;
	double translationDelta2 = 0.000025 * 5;

	if (zRotation < 45) {
		zRotation += rotationDelta;
	}

	if (xPosition < 1) {
		xPosition += translationDelta;
	}

	if (xPosition22 < 2.5) {
		xPosition22 += translationDelta2;
	}

	if (xPosition >= 1 && zRotation >= 45 && xPosition22 >= 2.5) {
		bendDirection = false;
	}

	rotate_z(mesh, BodyPartEnum::LEFT_UPPER_LEG, zRotation);
	translate_x(mesh, BodyPartEnum::LEFT_UPPER_LEG, xPosition);

	rotate_z(mesh, BodyPartEnum::LEFT_LOWER_LEG, zRotation);
	translate_x(mesh, BodyPartEnum::LEFT_LOWER_LEG, xPosition);

	rotate_z(mesh, BodyPartEnum::LEFT_FOOT, zRotation);
	translate_x(mesh, BodyPartEnum::LEFT_FOOT, xPosition22);

	rotate_z(mesh, BodyPartEnum::LEFT_FOOT_FINGERS, zRotation);
	translate_x(mesh, BodyPartEnum::LEFT_FOOT_FINGERS, xPosition22);
}

void streightenKnee(Mesh& mesh) {
	double rotationDelta = 0.00065 * 5;
	double translationDelta = 0.00002 * 5;
	double translationDelta2 = 0.000035 * 5;

	if (zRotation > -20) {
		zRotation -= rotationDelta;
	}

	if (xPosition > -1) {
		xPosition -= translationDelta;
	}

	if (xPosition22 > -1.5) {
		xPosition22 -= translationDelta2;
	}

	if (zRotation <= -20 && xPosition <= -1 && xPosition22 <= -1.5) {
		bendDirection = true;
	}

	rotate_z(mesh, BodyPartEnum::LEFT_UPPER_LEG, zRotation);
	translate_x(mesh, BodyPartEnum::LEFT_UPPER_LEG, xPosition);

	rotate_z(mesh, BodyPartEnum::LEFT_LOWER_LEG, zRotation);
	translate_x(mesh, BodyPartEnum::LEFT_LOWER_LEG, xPosition);

	rotate_z(mesh, BodyPartEnum::LEFT_FOOT, zRotation);
	translate_x(mesh, BodyPartEnum::LEFT_FOOT, xPosition22);

	rotate_z(mesh, BodyPartEnum::LEFT_FOOT_FINGERS, zRotation);
	translate_x(mesh, BodyPartEnum::LEFT_FOOT_FINGERS, xPosition22);
}

double turnHeadSpeed = 0.01;
double turnHeadPosition = 0.0;

bool turnHeadRight = true;


void animateHead(Mesh& mesh) {

	if (turnHeadRight) {
		turnHeadPosition += turnHeadSpeed;
	}
	else {
		turnHeadPosition -= turnHeadSpeed;
	}

	if (turnHeadPosition > 45) {
		turnHeadRight = false;
	}
	else if (turnHeadPosition < -45) {
		turnHeadRight = true;
	}

	rotate_x(mesh, BodyPartEnum::HEAD, turnHeadPosition);
}

double zRotation1 = 0.0;
double zRotation2 = 0.0;

double xPosition1 = 0.0;
double xPosition2 = 0.0;

double yPosition1 = 0.0;
double yPosition2 = 0.0;

bool upDirection = true;

void upHands(Mesh& mesh) {

	double zRotation1Rate = 0.00060 * 5;
	double zRotation2Rate = 0.00045 * 5;

	double xPosition1Rate = 0.000015 * 5;
	double xPosition2Rate = 0.000020 * 5;

	double yPosition1Rate = 0.000015 * 5;
	double yPosition2Rate = 0.000025 * 5;

	if (zRotation1 >= 60 && zRotation2 >= 45 && xPosition1 >= 1.5 && xPosition2 >= 2 && yPosition1 >= 1.5 && yPosition2 >= 2.5) {
		upDirection = false;
	}

	if (zRotation1 <= 0 && zRotation2 <= 0 && xPosition1 <= 0 && xPosition2 <= 0 && yPosition1 <= 0 && yPosition2 <= 0) {
		upDirection = true;
	}

	if (upDirection) {
		if (zRotation1 < 60) {
			zRotation1 += zRotation1Rate;
		}

		if (zRotation2 < 45) {
			zRotation2 += zRotation2Rate;
		}

		if (xPosition1 < 1.5) {
			xPosition1 += xPosition1Rate;
		}

		if (xPosition2 < 2) {
			xPosition2 += xPosition2Rate;
		}

		if (yPosition1 < 1.5) {
			yPosition1 += yPosition1Rate;
		}

		if (yPosition2 < 2.5) {
			yPosition2 += yPosition2Rate;
		}
	}
	else {
		if (zRotation1 > 0) {
			zRotation1 -= zRotation1Rate;
		}

		if (zRotation2 > 0) {
			zRotation2 -= zRotation2Rate;
		}

		if (xPosition1 > 0) {
			xPosition1 -= xPosition1Rate;
		}

		if (xPosition2 > 0) {
			xPosition2 -= xPosition2Rate;
		}

		if (yPosition1 > 0) {
			yPosition1 -= yPosition1Rate;
		}

		if (yPosition2 > 0) {
			yPosition2 -= yPosition2Rate;
		}
	}

	rotate_z(mesh, BodyPartEnum::RIGHT_UPPER_ARM, zRotation1);
	translate_x(mesh, BodyPartEnum::RIGHT_UPPER_ARM, xPosition1);

	rotate_z(mesh, BodyPartEnum::RIGHT_LOWER_ARM, zRotation1);
	translate_y(mesh, BodyPartEnum::RIGHT_LOWER_ARM, yPosition1);
	translate_x(mesh, BodyPartEnum::RIGHT_LOWER_ARM, xPosition1);

	rotate_z(mesh, BodyPartEnum::RIGHT_WRIST, zRotation2);
	translate_y(mesh, BodyPartEnum::RIGHT_WRIST, yPosition2);
	translate_x(mesh, BodyPartEnum::RIGHT_WRIST, xPosition2);

	rotate_z(mesh, BodyPartEnum::LEFT_UPPER_ARM, zRotation1);
	translate_x(mesh, BodyPartEnum::LEFT_UPPER_ARM, xPosition1);

	rotate_z(mesh, BodyPartEnum::LEFT_LOWER_ARM, zRotation1);
	translate_y(mesh, BodyPartEnum::LEFT_LOWER_ARM, yPosition1);
	translate_x(mesh, BodyPartEnum::LEFT_LOWER_ARM, xPosition1);

	rotate_z(mesh, BodyPartEnum::LEFT_WRIST, zRotation2);
	translate_y(mesh, BodyPartEnum::LEFT_WRIST, yPosition2);
	translate_x(mesh, BodyPartEnum::LEFT_WRIST, xPosition2);
}

void AnimateArms(Mesh& mesh) {

	if (bendDirection) {
		bendKnee(mesh);
	}
	else {
		streightenKnee(mesh);
	}

	animateHead(mesh);
	upHands(mesh);
}


/////////////////////////////////////////////////////////
//
// END
//
/////////////////////////////////////////////////////////

void DrawScene()
{
	if (GetAsyncKeyState(VK_ESCAPE))  exit(0);

	// mouse pointer position
	POINT cursor;
	GetCursorPos(&cursor);

	// camera orientation
	float	viewangle_x = float(cursor.x - 1280 / 2) / 4.0;
	float	viewangle_y = float(cursor.y - 768 / 2) / 4.0;

	//time
	static uint t0 = timeGetTime();
	double time_elapsed = double(timeGetTime() - t0) / 1000;

	// clear and basic
	glClearDepth(1);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// projection
	int vp[4];
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glGetIntegerv(GL_VIEWPORT, vp);
	gluPerspective(90.0, float(vp[2]) / float(vp[3]), 0.01, 100);

	// modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(viewangle_y, 1, 0, 0);		// set rotation
	glRotatef(viewangle_x, 0, 1, 0);		// set rotation

	// select level of detail for rendering
	// (lods are generated automatically by the ogre xml converter )

	int lod = 0;
	if (GetAsyncKeyState(VK_F1)) lod = 1;
	if (GetAsyncKeyState(VK_F2)) lod = 2;
	if (GetAsyncKeyState(VK_F3)) lod = 3;
	if (GetAsyncKeyState(VK_F4)) lod = 4;
	if (GetAsyncKeyState(VK_F5)) lod = 5;

	// Test 1 : Halo character (animated mesh)

	static Mesh halo("../data/halo/halo.material",		//	required material file)
		"../data/halo/halo.mesh.xml",		//	required mesh file
		"../data/halo/halo.skeleton.xml");	//	optional skeleton file
//halo.skinning = false;

	loopi(0, 2)
	{
		// Set the skeleton to an animation at a given time

		//int idle = halo.animation.GetAnimationIndexOf("idle"); // <- in case we dont know the animation id
		halo.animation.SetPose(
			i / 2,			// animation id (2 animations, 0 and 1, are available)
			time_elapsed);	// time in seconds

		// F6 : example to manually set the shoulder - for shooting a weapon e.g.
		if (GetAsyncKeyState(VK_F7)) {
			printBone(halo, 0, 0);
		}
		if (GetAsyncKeyState(VK_F6))
		{
			AnimateArms(halo);
		}

		// Draw the model

		halo.Draw(
			vec3f(i * 7, -5, 7),			// position
			vec3f(0,/*time_elapsed*0.3*/ 1.57F, 0),	// rotation
			lod,							// LOD level
			(i & 1) == 1);						// draw skeleton ?
	}

	// Test 2 : moon (static mesh)

	//static Mesh moon   ("../data/moon/moon.material",	//	required material file)
	//					"../data/moon/moon.mesh.xml");	//	required mesh file
	//	moon.Draw(
	//		vec3f( 1.1,-0,-1),		  		// position
	//		vec3f(
	//			time_elapsed*0.2,			// rotation
	//			time_elapsed*0.1,
	//			time_elapsed*0.4),	
	//		lod								// LOD level
	//	);

	// Swap
	glutSwapBuffers();
}
///////////////////////////////////////////
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(1280, 768);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Skeletal Animation");
	glutDisplayFunc(DrawScene);
	glutIdleFunc(DrawScene);
	glewInit();
	wglSwapIntervalEXT(0);
	glutMainLoop();
	return 0;
}
///////////////////////////////////////////
