// written by Michael   - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - new texture manager, improvements for behaviors

// the simple example objects - not that exciting - yours should be
// better! 

// note - because this is a precompiled header, getting the name the same
// is more important than getting the path right
#include "../GrTown_PCH.H"
#include "../DrawUtils.H"
#include "Utilities/Texture.H"
#include "../MMatrix.H"
#include "Suburbs.H"
#include <GL/glut.h>

/***********************************************************************/
// a Really simple tree - just a cone with a stem...
// use normals to make it look rounder...
SimpleTree1::SimpleTree1(float h, float r)
  : height(h), radius(r)
{
}
void SimpleTree1::draw(DrawingState*)
{
  // cone for the body
  glFrontFace(GL_CW);
  glColor3f(0.f,.6f,.3f);
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,1,0);
  glVertex3f(0,height,0);
  glNormal3f(1,0,0);
  glVertex3f(radius,height/4,0);
  glNormal3f(0,0,1);
  glVertex3f(0,height/4,radius);
  glNormal3f(-1,0,0);
  glVertex3f(-radius,height/4,0);
  glNormal3f(0,0,-1);
  glVertex3f(0,height/4,-radius);
  glNormal3f(1,0,0);
  glVertex3f(radius,height/4,0);
  glEnd();
  glFrontFace(GL_CCW);
  // stem
  glColor3f(.6f,.5f,.3f);
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glVertex3f(radius/5,0,0);
  glVertex3f(radius/5,height/4,0);
  glNormal3f(0,0,1);
  glVertex3f(0,0,radius/5);
  glVertex3f(0,height/4,radius/5);
  glNormal3f(-1,0,0);
  glVertex3f(-radius/5,0,0);
  glVertex3f(-radius/5,height/4,0);
  glNormal3f(0,0,-1);
  glVertex3f(0,0,-radius/5);
  glVertex3f(0,height/4,-radius/5);
  glNormal3f(1,0,0);
  glVertex3f(radius/5,0,0);
  glVertex3f(radius/5,height/4,0);
  glEnd();
}

/***********************************************************************/
// draw a patch of lawn - use a repeating texture
Lawn::Lawn(float xi1, float zi1, float xi2, float zi2)
  : x1(xi1), z1(zi1), x2(xi2), z2(zi2)
{
}

void Lawn::draw(DrawingState* d)
{
	// the catch here is that we need to use a polygon offset to draw
	// the lawn just above the ground...
	if (d->drGrTex)
		fetchTexture("grass.bmp",true,true);
	else
		glBindTexture(GL_TEXTURE_2D,0);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-2.,-2.);
		glNormal3f(0,1,0);
		glColor3f(1,1,1);
		glBegin(GL_POLYGON);
		glTexCoord2f(0,(z2-z1)/15.f);             glVertex3f(x1,0,z2);
		glTexCoord2f((x2-x1)/15.f, (z2-z1)/15.f); glVertex3f(x2,0,z2);
		glTexCoord2f((x2-x1)/15.f,0);             glVertex3f(x2,0,z1);
		glTexCoord2f(0,0);                        glVertex3f(x1,0,z1);
	glEnd();
	glDisable(GL_POLYGON_OFFSET_FILL);
	glBindTexture(GL_TEXTURE_2D,0);
}

/***********************************************************************/
//house1
static void drawChimney(GLint halfWidth, GLint height) {
	//front
	glColor3ub(255, 255, 255);
	polygoni(-4, -halfWidth, 0, halfWidth, halfWidth, 0, halfWidth,
		halfWidth, height, halfWidth, -halfWidth, height, halfWidth);
	//right
	polygoni(-4, halfWidth, 0, halfWidth, halfWidth, 0, -halfWidth,
		halfWidth, height, -halfWidth, halfWidth, height, halfWidth);
	//left
	polygoni(-4, -halfWidth, 0, -halfWidth, -halfWidth, 0, halfWidth,
		-halfWidth, height, halfWidth, -halfWidth, height, -halfWidth);
	//back
	polygoni(-4, halfWidth, 0, -halfWidth, -halfWidth, 0, -halfWidth,
		-halfWidth, height, -halfWidth, halfWidth, height, -halfWidth);
	//top
	polygoni(-4, -halfWidth, height, halfWidth, halfWidth, height, halfWidth,
		halfWidth, height, -halfWidth, -halfWidth, height, -halfWidth);
}

SimpleHouse1::SimpleHouse1()
{
	color(.6f,.7f,.8f);
}

void SimpleHouse1::draw(DrawingState*)
{
	glColor3fv(&color.r);
	//back
	fetchTexture("house1-front.bmp");
	glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(-20, 0, -25);
		glTexCoord2f(0, 1); glVertex3i(-20, 30, -25);
		glTexCoord2f(0.5, 1.5); glVertex3i(0, 45, -25);
		glTexCoord2f(1, 1); glVertex3i(20, 30, -25);
		glTexCoord2f(1, 0); glVertex3i(20, 0, -25);
	glEnd();
	//front
	glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0); glVertex3i(-20, 0, 25);
		glTexCoord2f(1, 0); glVertex3i(20, 0, 25);
		glTexCoord2f(1, 1); glVertex3i(20, 30, 25);
		glTexCoord2f(0.5, 1.5); glVertex3i(0, 45, 25);
		glTexCoord2f(0, 1); glVertex3i(-20, 30, 25);
	glEnd();
	//right
	fetchTexture("house1-side.bmp");
	polygoni(-4, 20, 0, 25, 20, 0, -25, 20, 30, -25, 20, 30, 25);
	//left
	polygoni(-4, -20, 0, -25, -20, 0, 25, -20, 30, 25, -20, 30, -25);
	//roof
	fetchTexture("roof.bmp");
	glColor3ub(255, 255, 255);
	polygoni( 4, 20,30,-25,  20,30, 25,   0, 45, 25,   0, 45, -25);
	polygoni(-4, -20,30,-25, -20,30, 25,   0, 45, 25,   0, 45, -25);
	glBindTexture(GL_TEXTURE_2D, 0);
	//chimney
	glTranslatef(10, 35, -15);
	drawChimney(2, 12);	
}

/***********************************************************************/
//house2
static void drawAttic(GLint halfWidth, GLint height) {
	//front
	polygoni(-5, -halfWidth, 0, halfWidth, halfWidth, 0, halfWidth,
		halfWidth, height, halfWidth, 0, height + 3, halfWidth, -halfWidth, height, halfWidth);
	//right
	polygoni(-4, halfWidth, 0, halfWidth, halfWidth, 0, -halfWidth,
		halfWidth, height, -halfWidth, halfWidth, height, halfWidth);
	//left
	polygoni(-4, -halfWidth, 0, -halfWidth, -halfWidth, 0, halfWidth,
		-halfWidth, height, halfWidth, -halfWidth, height, -halfWidth);
	//back
	polygoni(-5, halfWidth, 0, -halfWidth, -halfWidth, 0, -halfWidth,
		-halfWidth, height, -halfWidth, 0, height + 3, -halfWidth, halfWidth, height, -halfWidth);
	//top
	polygoni(-4, -halfWidth, height, halfWidth, halfWidth, height, halfWidth,
		halfWidth, height, -halfWidth, -halfWidth, height, -halfWidth);
	//roof
	fetchTexture("roof.bmp");
	glColor3ub(255, 255, 255);
	glBegin(GL_QUADS);
		//left roof
		glNormal3f(-0.7071,0.7071,0);
		glTexCoord2f(0, 0);	glVertex3i(-halfWidth, height, -halfWidth);
		glTexCoord2f(0.2, 0); glVertex3i(-halfWidth, height, halfWidth);
		glTexCoord2f(0.2, 0.2); glVertex3i(0, height + 3, halfWidth);
		glTexCoord2f(0, 0.2); glVertex3i(0, height + 3, -halfWidth);
		//right roof
		glNormal3f(0.7071, 0.7071, 0);
		glTexCoord2f(0, 0);	glVertex3i(halfWidth, height, halfWidth);
		glTexCoord2f(0.2, 0); glVertex3i(halfWidth, height, -halfWidth);
		glTexCoord2f(0.2, 0.2); glVertex3i(0, height + 3, -halfWidth);
		glTexCoord2f(0, 0.2); glVertex3i(0, height + 3, halfWidth);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

SimpleHouse2::SimpleHouse2()
{
	color(.6f,.7f,.8f);
	rotMatrix(transform,'Y',3.14159f/2.f);
}

void SimpleHouse2::draw(DrawingState*)
{
	glColor3fv(&color.r);
	fetchTexture("house2-side.bmp", true, false);
	glBegin(GL_POLYGON);
		//front
		glNormal3f(0,0,1);
		glTexCoord2f(0, 0);	glVertex3i(-20, 0, 30);
		glTexCoord2f(1, 0);	glVertex3i(20, 0, 30);
		glTexCoord2f(1, 1);	glVertex3i(20, 30, 30);
		glTexCoord2f(0.5, 1.5);	glVertex3i(0, 45, 30);
		glTexCoord2f(0, 1);	glVertex3i(-20, 30, 30);
	glEnd();
	glBegin(GL_POLYGON);
		//back
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0);	glVertex3i(20, 0, -30);
		glTexCoord2f(1, 0);	glVertex3i(-20, 0, -30);
		glTexCoord2f(1, 1);	glVertex3i(-20, 30, -30);
		glTexCoord2f(0.5, 1.5);	glVertex3i(0, 45, -30);
		glTexCoord2f(0, 1);	glVertex3i(20, 30, -30);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	//right
	fetchTexture("house2-front.bmp");
	polygoni(-4, 20, 0, 30, 20, 0, -30, 20, 30, -30, 20, 30, 30);
	//left
	polygoni(-4, -20, 0, -30, -20, 0, 30, -20, 30, 30, -20, 30, -30);
	//roof
	fetchTexture("roof.bmp");
	glColor3ub(255, 255, 255);
	polygoni( 4, 20,30,-30,  20,30, 30,   0, 45, 30,   0, 45, -30);
	polygoni(-4,-20,30,-30, -20,30, 30,   0, 45, 30,   0, 45, -30);
	glBindTexture(GL_TEXTURE_2D, 0);
	//attic
	glColor3fv(&color.r);
	glPushMatrix();
		glTranslatef(10, 33, 10);
		glRotatef(-90, 0, 1, 0);
		drawAttic(3, 6);
	glPopMatrix();
	glColor3fv(&color.r);
	glPushMatrix();
		glTranslatef(10, 33, -10);
		glRotatef(-90, 0, 1, 0);
		drawAttic(3, 6);
	glPopMatrix();	
}

/***********************************************************************/
//house 3
SimpleHouse3::SimpleHouse3()
{
	color(.6f,.7f,.8f);
}

void SimpleHouse3::draw(DrawingState*)
{
	glColor3fv(&color.r);
	//front
	fetchTexture("house2-front.bmp");
	polygoni(-4, -30, 0, 20, 30, 0, 20, 30, 30, 20, -30, 30, 20);
	//back
	polygoni(-4, 30, 0, -20, -30, 0, -20, -30, 30, -20, 30, 30, -20);
	//left
	fetchTexture("house2-side.bmp", true, false);
	polygoni(-4, -30, 0, -20 ,- 30, 0, 20, -30, 30, 20, -30, 30, -20);
	//right
	polygoni(-4, 30, 0, 20, 30, 0, -20, 30, 30, -20, 30, 30, 20);
	//roof
	fetchTexture("roof.bmp");
	glColor3ub(255,255, 255);
	glBegin(GL_QUADS);
		//front roof
		glNormal3f(0, 0.8, 0.6);
		glTexCoord2f(0, 0); glVertex3i(-30, 30, 20);
		glTexCoord2f(1, 0); glVertex3i(30, 30, 20);
		glTexCoord2f(0.8, 1); glVertex3i(18, 45, 0);
		glTexCoord2f(0.2, 1); glVertex3i(-18, 45, 0);
		//back roof
		glNormal3f(0, 0.8, -0.6);
		glTexCoord2f(0, 0); glVertex3i(30, 30, -20);
		glTexCoord2f(1, 0); glVertex3i(-30, 30, -20);
		glTexCoord2f(0.8, 1); glVertex3i(-18, 45, 0);
		glTexCoord2f(0.2, 1); glVertex3i(18, 45, 0);
	glEnd();
	GLfloat y = 4 / sqrt(41);
	GLfloat x = 5 / sqrt(41);
	glBegin(GL_TRIANGLES);
		//left roof
		glNormal3f(-x, y, 0);
		glTexCoord2f(0, 0); glVertex3i(-30, 30, -20);
		glTexCoord2f(0.8, 0); glVertex3i(-30, 30, 20);
		glTexCoord2f(0.4, 1); glVertex3i(-18, 45, 0);
		//right roof
		glNormal3f(x, y, 0);
		glTexCoord2f(0, 0); glVertex3i(30, 30, 20);
		glTexCoord2f(0.8, 0); glVertex3i(30, 30, -20);
		glTexCoord2f(0.4, 1); glVertex3i(18, 45, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	//chimney
	glTranslatef(0, 43, 0);
	glTranslatef(-14, 0, 0);
	drawChimney(2, 6);
	glTranslatef(28, 0, 0);
	drawChimney(2, 6);
}

/***********************************************************************/
//multitexture house
MultiTextureHouse::MultiTextureHouse()
{
}

static void drawWoodPole(GLfloat radius, GLfloat height) {
	GLUquadricObj *obj = gluNewQuadric();
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		gluCylinder(obj, radius, radius, height, 10, 20);
	glPopMatrix();
}

static void drawWoodFence(GLfloat width, GLfloat height, bool left, bool right) {
	GLUquadricObj *obj = gluNewQuadric();
	int poleCount = (int)((width-1) / 3)-1;
	//top
	glPushMatrix();
		glTranslatef(-0.5, height, 0);
		glRotatef(-90, 0, 0, 1);
		drawWoodPole(0.5, width);
	glPopMatrix();
	//left
	if (left) drawWoodPole(0.5, height);
	//middle
	for (int i = 0; i < poleCount; i++) {
		glTranslatef(3, 0, 0);
		drawWoodPole(0.5, height);
	}
	//right
	if (right) {
		glTranslatef(3, 0, 0);
		drawWoodPole(0.5, height);	}	
}

void MultiTextureHouse::draw(DrawingState*)
{
	GLint hallHalfWidth = 20, hallHeight = 25, length = 40, frontLen = 13, roofHeight = 16;
	
	Texture* wallTex = fetchTexture("wood.bmp",true,true);
	Texture* windowTex = fetchTexture("house1-front.bmp");
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wallTex->texName);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
		
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, windowTex->texName);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);

	////walls
	glColor3ub(255, 255, 255);
	glBegin(GL_QUADS);
		//front
		glNormal3f(0, 0, 1);
		glMultiTexCoord2fARB(GL_TEXTURE0, 2, 0);  glMultiTexCoord2fARB(GL_TEXTURE1, 1, 0); glVertex3i(hallHalfWidth, 0, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0, 2, 3);  glMultiTexCoord2fARB(GL_TEXTURE1, 1, 0.5); glVertex3i(hallHalfWidth, hallHeight, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0, 0, 3);  glMultiTexCoord2fARB(GL_TEXTURE1, 0, 0.5); glVertex3i(-hallHalfWidth, hallHeight, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0, 0, 0);  glMultiTexCoord2fARB(GL_TEXTURE1, 0, 0); glVertex3i(-hallHalfWidth, 0, 0);
	glEnd();

	windowTex = fetchTexture("house1-side.bmp");
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, windowTex->texName);

	glBegin(GL_QUADS);
		//right
		glNormal3f(1, 0, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0, 0, 0);  glMultiTexCoord2fARB(GL_TEXTURE1, 0, 0); glVertex3i(hallHalfWidth, 0, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0, 2, 0);  glMultiTexCoord2fARB(GL_TEXTURE1, 1, 0); glVertex3i(hallHalfWidth, 0, -length);
		glMultiTexCoord2fARB(GL_TEXTURE0, 2, 3);  glMultiTexCoord2fARB(GL_TEXTURE1, 1, 0.5); glVertex3i(hallHalfWidth, hallHeight, -length);
		glMultiTexCoord2fARB(GL_TEXTURE0, 0, 3);  glMultiTexCoord2fARB(GL_TEXTURE1, 0, 0.5); glVertex3i(hallHalfWidth, hallHeight, 0);
		//back
		glNormal3f(0, 0, -1);
		glMultiTexCoord2fARB(GL_TEXTURE0, 0, 0);  glMultiTexCoord2fARB(GL_TEXTURE1, 0, 0); glVertex3i(hallHalfWidth, 0, -length);
		glMultiTexCoord2fARB(GL_TEXTURE0, 2, 0);  glMultiTexCoord2fARB(GL_TEXTURE1, 1, 0); glVertex3i(-hallHalfWidth, 0, -length);
		glMultiTexCoord2fARB(GL_TEXTURE0, 2, 3);  glMultiTexCoord2fARB(GL_TEXTURE1, 1, 0.5); glVertex3i(-hallHalfWidth, hallHeight, -length);
		glMultiTexCoord2fARB(GL_TEXTURE0, 0, 3);  glMultiTexCoord2fARB(GL_TEXTURE1, 0, 0.5); glVertex3i(hallHalfWidth, hallHeight, -length);
		//left
		glNormal3f(-1, 0, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0, 2, 0);  glMultiTexCoord2fARB(GL_TEXTURE1, 1, 0);  glVertex3i(-hallHalfWidth, 0, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0, 2, 3);  glMultiTexCoord2fARB(GL_TEXTURE1, 1, 0.5); glVertex3i(-hallHalfWidth, hallHeight, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0, 0, 3);  glMultiTexCoord2fARB(GL_TEXTURE1, 0, 0.5); glVertex3i(-hallHalfWidth, hallHeight, -length);
		glMultiTexCoord2fARB(GL_TEXTURE0, 0, 0);  glMultiTexCoord2fARB(GL_TEXTURE1, 0, 0); glVertex3i(-hallHalfWidth, 0, -length);
	glEnd();
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	fetchTexture("wood.bmp", true, true);
	glBegin(GL_TRIANGLES);
		//front top
		glNormal3f(0, 0, 1);
		glTexCoord2f( 2, 0);  glVertex3i(hallHalfWidth, hallHeight, 0);
		glTexCoord2f(1, 3); glVertex3i(0, hallHeight + roofHeight, 0);
		glTexCoord2f(0, 0); glVertex3i(-hallHalfWidth, hallHeight, 0);
		//back top
		glNormal3f(0, 0, -1);
		glTexCoord2f(2, 0);  glVertex3i(-hallHalfWidth, hallHeight, -length);
		glTexCoord2f(1, 3); glVertex3i(0, hallHeight + roofHeight, -length);
		glTexCoord2f(0, 0); glVertex3i(hallHalfWidth, hallHeight, -length);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	////front parts
	glColor3ub(234, 166, 100);
	//floor
	polygoni(-4, -hallHalfWidth, 1, frontLen, hallHalfWidth, 1, frontLen, hallHalfWidth, 1, 0, -hallHalfWidth, 1, 0);
	polygoni(-4, -hallHalfWidth, 0, frontLen, hallHalfWidth, 0, frontLen, hallHalfWidth, 1, frontLen, -hallHalfWidth, 1, frontLen);
	polygoni(-4, -hallHalfWidth, 0, 0, -hallHalfWidth, 0, frontLen, -hallHalfWidth, 1, frontLen, -hallHalfWidth, 1, 0);
	polygoni(-4, hallHalfWidth, 0, frontLen, hallHalfWidth, 0, 0, hallHalfWidth, 1, 0, hallHalfWidth, 1, frontLen);
	glTranslatef(0, 1, 0);
	//front fence
	glPushMatrix();
		glTranslatef(-hallHalfWidth+1, 0, frontLen - 1);
		drawWoodPole(0.8, hallHeight+1);
		drawWoodFence(hallHalfWidth*0.8, 8, false, true);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(hallHalfWidth - 1, 0, frontLen - 1);
		drawWoodPole(0.8, hallHeight + 1);
		glTranslatef(-hallHalfWidth*0.8, 0, 0);
		drawWoodFence(hallHalfWidth*0.8, 8, true, false);
	glPopMatrix();
	//left fence
	glPushMatrix();
		glTranslatef(-hallHalfWidth + 1, 0, 0);
		glRotatef(-90, 0, 1, 0);
		drawWoodFence(frontLen-1, 8, false, false);
	glPopMatrix();
	//left fence
	glPushMatrix();
		glTranslatef(hallHalfWidth - 1, 0, frontLen-1);
		glRotatef(90, 0, 1, 0);
		drawWoodFence(frontLen-1, 8, false, false);
	glPopMatrix();

	////roof
	//layer 1
	glTranslatef(0, hallHeight-1, 0);
	polygoni(-4, -hallHalfWidth, 0, -length, -hallHalfWidth, 0, frontLen, 0, roofHeight, frontLen, 0, roofHeight, -length);
	polygoni(-4, hallHalfWidth, 0, frontLen, hallHalfWidth, 0, -length, 0, roofHeight, -length, 0, roofHeight, frontLen);
	//layer 2
	glTranslatef(0, 2, 0);
	polygoni(-4, -hallHalfWidth, 0, -length, -hallHalfWidth, 0, frontLen, 0, roofHeight, frontLen, 0, roofHeight, -length);
	polygoni(-4, hallHalfWidth, 0, frontLen, hallHalfWidth, 0, -length, 0, roofHeight, -length, 0, roofHeight, frontLen);
	glTranslatef(0, -2, 0);
	//connection
	polygoni(-4, -hallHalfWidth, 0, frontLen, 0, roofHeight, frontLen, 0, roofHeight + 2, frontLen, -hallHalfWidth, 2, frontLen);
	polygoni(-4, 0, roofHeight, frontLen, hallHalfWidth, 0, frontLen, hallHalfWidth, 2, frontLen, 0, 2 + roofHeight, frontLen);
	polygoni(-4, -hallHalfWidth, 0, -length, -hallHalfWidth, 0, frontLen, -hallHalfWidth, 2, frontLen, -hallHalfWidth, 2, -length);
	polygoni(-4, hallHalfWidth, 0, frontLen, hallHalfWidth, 0, -length, hallHalfWidth, 2, -length, hallHalfWidth, 2, frontLen);
	polygoni(4, -hallHalfWidth, 0, -length, 0, roofHeight, -length, 0, roofHeight + 2, -length, -hallHalfWidth, 2, -length);
	polygoni(4, 0, roofHeight, -length, hallHalfWidth, 0, -length, hallHalfWidth, 2, -length, 0, 2 + roofHeight, -length);
	
}


/***********************************************************************/
//town hall
TownHall::TownHall() : GrObject("Town Hall")
{
}

void TownHall::draw(DrawingState*)
{
	GLint hallHalfWidth = 35, hallHeight = 30, length = 40, roofHeight = 15, roofHalfWidth = 20;
	GLint chimneyHalfWidth = 2, chimneyHeight = 6;
	GLfloat towerRadius = 3.8, towerHeight =18;

	fetchTexture("town-hall.bmp",true,false);
	////walls
	glColor3ub(255, 255, 255);
	glBegin(GL_QUADS);
		//front
		glNormal3f(0, 0, 1);
		glTexCoord2f(9, 0); glVertex3i(hallHalfWidth, 0, 0);
		glTexCoord2f(9, 1); glVertex3i(hallHalfWidth, hallHeight, 0);
		glTexCoord2f(0, 1); glVertex3i(-hallHalfWidth, hallHeight, 0);
		glTexCoord2f(0, 0); glVertex3i(-hallHalfWidth, 0, 0);
		//right
		glNormal3f(1, 0, 0);
		glTexCoord2f(0, 0); glVertex3i(hallHalfWidth, 0, 0);
		glTexCoord2f(5, 0); glVertex3i(hallHalfWidth, 0, -length);
		glTexCoord2f(5, 1); glVertex3i(hallHalfWidth, hallHeight, -length);
		glTexCoord2f(0, 1); glVertex3i(hallHalfWidth, hallHeight, 0);
		//back
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(hallHalfWidth, 0, -length);
		glTexCoord2f(9, 0); glVertex3i(-hallHalfWidth, 0, -length);
		glTexCoord2f(9, 1); glVertex3i(-hallHalfWidth, hallHeight, -length);
		glTexCoord2f(0, 1); glVertex3i(hallHalfWidth, hallHeight, -length);
		//left
		glNormal3f(-1, 0, 0);
		glTexCoord2f(5, 0); glVertex3i(-hallHalfWidth, 0, 0);
		glTexCoord2f(5, 1); glVertex3i(-hallHalfWidth, hallHeight, 0);
		glTexCoord2f(0, 1); glVertex3i(-hallHalfWidth, hallHeight, -length);
		glTexCoord2f(0, 0); glVertex3i(-hallHalfWidth, 0, -length);
	glEnd();
	
	////roof
	glTranslatef(0, hallHeight, 0);
	fetchTexture("roof.bmp");
	glColor3ub(255, 255, 255);
	GLfloat v1[3] = { hallHalfWidth - roofHalfWidth, roofHeight, 0 };
	normalize(v1);
	GLfloat v2[3] = { -hallHalfWidth + roofHalfWidth, roofHeight, 0 };
	normalize(v2);
	GLfloat v3[3] = { 0, length / 2, roofHeight };
	normalize(v3);
	GLfloat v4[3] = { 0, length / 2, -roofHeight };
	normalize(v4);
	glBegin(GL_TRIANGLES);
		//right roof
		glNormal3f(v1[0], v1[1], v1[2]);
		glTexCoord2f(0, 0); glVertex3f(hallHalfWidth, 0, 0);
		glTexCoord2f(0.8, 0); glVertex3f(hallHalfWidth, 0, -length);
		glTexCoord2f(0.4, 1); glVertex3f(roofHalfWidth, roofHeight, -length / 2);
		//left roof
		glNormal3f(v2[0], v2[1], v2[2]);
		glTexCoord2f(0.8, 0); glVertex3f(-hallHalfWidth, 0, 0);
		glTexCoord2f(0.4, 1); glVertex3f(-roofHalfWidth, roofHeight, -length / 2);
		glTexCoord2f(0, 0); glVertex3f(-hallHalfWidth, 0, -length);
	glEnd();
	glBegin(GL_QUADS);
		//front roof
		glNormal3f(v3[0], v3[1], v3[2]);
		glTexCoord2f(0, 0); glVertex3f(-hallHalfWidth, 0, 0);
		glTexCoord2f(1, 0); glVertex3f(hallHalfWidth, 0, 0);
		glTexCoord2f(0.7857, 1); glVertex3f(roofHalfWidth, roofHeight, -length / 2);
		glTexCoord2f(0.2143, 1); glVertex3f(-roofHalfWidth, roofHeight, -length / 2);
		//back roof
		glNormal3f(v4[0], v4[1], v4[2]);
		glTexCoord2f(0, 0); glVertex3f(hallHalfWidth, 0, -length);
		glTexCoord2f(1, 0); glVertex3f(-hallHalfWidth, 0, -length);
		glTexCoord2f(0.7857, 1); glVertex3f(-roofHalfWidth, roofHeight, -length / 2);
		glTexCoord2f(0.2143, 1); glVertex3f(roofHalfWidth, roofHeight, -length / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	//chimney
	glTranslatef(0, roofHeight-1.5, 0);
	glTranslatef(-roofHalfWidth+chimneyHalfWidth*2, 0, -length/2);
	drawChimney(chimneyHalfWidth, chimneyHeight);
	glTranslatef(2*(roofHalfWidth - chimneyHalfWidth * 2), 0, 0);
	drawChimney(chimneyHalfWidth, chimneyHeight);

	////tower
	//bottom
	GLUquadricObj *obj = gluNewQuadric();
	glColor3ub(70, 90, 90);
	glTranslatef(-(roofHalfWidth - chimneyHalfWidth * 2), 0, 0);
	glPushMatrix();		
		glRotatef(-90, 1, 0, 0);
		glRotatef(-30, 0, 0, 1);
		gluCylinder(obj, towerRadius, towerRadius, towerHeight*0.33, 6, 10);
		//cover
		glTranslatef(0, 0, towerHeight*0.33);
		gluCylinder(obj, towerRadius, towerRadius*0.8, 1, 6, 10);
		//middle
		glTranslatef(0, 0, 1);
		gluCylinder(obj, towerRadius*0.8, towerRadius*0.8, towerHeight*0.45, 6, 10);
		glTranslatef(0, 0, towerHeight*0.45);
		gluCylinder(obj, towerRadius*0.8, towerRadius*0.5, 1.5, 6, 10);
		//top
		glTranslatef(0, 0, 1.5);
		gluCylinder(obj, towerRadius*0.5, 0, towerHeight*0.6, 6, 10);
		//flag pole
		glColor3ub(250, 250, 250);
		glTranslatef(0, 0, towerHeight*0.6-1.5);
		gluCylinder(obj, 0.3,0.3, 12, 10, 10);
	glPopMatrix();
	//flag
	fetchTexture("flag.bmp", true, false);
	glTranslatef(0, towerHeight*1.8, 0);
	glRotatef(30, 0, 1, 0);
	polygoni(-4, 0, 0, 0, 8, 0, 0, 8, 5, 0, 0, 5, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/***********************************************************************/
//lutheran church
LutheranChurch::LutheranChurch() : GrObject("Lutheran Church")
{
}

static void drawLuthChurchTower(GLint halfWidth, GLint height) {
	polygoni(-3, -halfWidth, 0, halfWidth, halfWidth, 0, halfWidth, 0, height, 0);
	polygoni(-3, halfWidth, 0, halfWidth, halfWidth, 0, -halfWidth, 0, height, 0);
	polygoni(-3, halfWidth, 0, -halfWidth, -halfWidth, 0, -halfWidth, 0, height, 0);
	polygoni(-3, -halfWidth, 0, -halfWidth, -halfWidth, 0, halfWidth, 0, height, 0);
}

void LutheranChurch::draw(DrawingState*)
{
	GLint towerHalfWidth =8, towerHeight =80,towerTopHeight = 30;
	GLint bodyHalfWidth = 25, bodyHeight = 30,roofHeight = 15,bodyLen=70;

	////tower
	//front
	fetchTexture("lutheran-church-tower.bmp");
	glColor3ub(255, 255, 255);
	polygoni(-4, -towerHalfWidth, 0, towerHalfWidth, towerHalfWidth, 0, towerHalfWidth, 
		towerHalfWidth, towerHeight, towerHalfWidth, -towerHalfWidth, towerHeight, towerHalfWidth);
	//right
	fetchTexture("lutheran-church-tower2.bmp");
	polygoni(-4, towerHalfWidth, 0, towerHalfWidth, towerHalfWidth, 0, -towerHalfWidth, 
		towerHalfWidth, towerHeight, -towerHalfWidth, towerHalfWidth, towerHeight, towerHalfWidth);
	//left
	polygoni(-4, -towerHalfWidth, 0, -towerHalfWidth, -towerHalfWidth, 0, towerHalfWidth,
		-towerHalfWidth, towerHeight, towerHalfWidth, -towerHalfWidth, towerHeight, -towerHalfWidth);
	//back
	polygoni(-4, towerHalfWidth, 0, -towerHalfWidth, -towerHalfWidth, 0, -towerHalfWidth,
		-towerHalfWidth, towerHeight, -towerHalfWidth, towerHalfWidth, towerHeight, -towerHalfWidth);
	glBindTexture(GL_TEXTURE_2D, 0);
	//top
	glColor3ub(242, 225, 137);
	glPushMatrix();	
		glTranslatef(0, towerHeight, 0);
		polygoni(-4, -towerHalfWidth, 0, towerHalfWidth, towerHalfWidth, 0, towerHalfWidth,
		towerHalfWidth, 0, -towerHalfWidth, -towerHalfWidth, 0, -towerHalfWidth);
		glColor3ub(70,90,90);
		drawLuthChurchTower(towerHalfWidth *0.7, towerTopHeight);
		glColor3ub(100, 120, 120);
		glPushMatrix();
			glTranslatef(-towerHalfWidth*0.8, 0, towerHalfWidth*0.8);
			drawLuthChurchTower(towerHalfWidth *0.2, 10);
			glTranslatef(towerHalfWidth*1.6, 0, 0);
			drawLuthChurchTower(towerHalfWidth *0.2, 10);
			glTranslatef(0, 0, -towerHalfWidth*1.6);
			drawLuthChurchTower(towerHalfWidth *0.2, 10);
			glTranslatef(-towerHalfWidth*1.6, 0, 0);
			drawLuthChurchTower(towerHalfWidth *0.2, 10);
		glPopMatrix();
	glPopMatrix();

	////body
	glColor3ub(255,255,255);
	glTranslatef(0, 0,-towerHalfWidth);
	fetchTexture("lutheran-church-front.bmp");
	//front
	glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0,0); glVertex3i(-bodyHalfWidth, 0, 0);
		glTexCoord2f(1, 0); glVertex3i(bodyHalfWidth, 0, 0);
		glTexCoord2f(1, 1); glVertex3i(bodyHalfWidth, bodyHeight, 0);
		glTexCoord2f(0.5, 1.5); glVertex3i(0, bodyHeight + roofHeight, 0);
		glTexCoord2f(0, 1); glVertex3i(-bodyHalfWidth, bodyHeight, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	//back
	glColor3ub(242, 225, 137);
	glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		glVertex3i(bodyHalfWidth, 0, -bodyLen);
		glVertex3i(-bodyHalfWidth, 0, -bodyLen);
		glVertex3i(-bodyHalfWidth, bodyHeight, -bodyLen);
		glVertex3i(0, bodyHeight + roofHeight, -bodyLen);
		glVertex3i(bodyHalfWidth, bodyHeight, -bodyLen);		
	glEnd();
	fetchTexture("lutheran-church-side.bmp",true, false);
	glColor3ub(255, 255, 255);
	glBegin(GL_QUADS);
		//left
		glNormal3f(-1, 0, 0);
		glTexCoord2f(4, 0); glVertex3i(-bodyHalfWidth, 0, 0);
		glTexCoord2f(4, 1); glVertex3i(-bodyHalfWidth, bodyHeight, 0);
		glTexCoord2f(0, 1); glVertex3i(-bodyHalfWidth, bodyHeight, -bodyLen);
		glTexCoord2f(0, 0); glVertex3i(-bodyHalfWidth, 0, -bodyLen);
		//right
		glNormal3f(1, 0, 0);
		glTexCoord2f(0, 0); glVertex3i(bodyHalfWidth, 0, 0);
		glTexCoord2f(4, 0); glVertex3i(bodyHalfWidth, 0, -bodyLen);
		glTexCoord2f(4, 1); glVertex3i(bodyHalfWidth, bodyHeight, -bodyLen);
		glTexCoord2f(0, 1); glVertex3i(bodyHalfWidth, bodyHeight, 0);
	glEnd();
	//additional room
	GLint tmpWidth = bodyHalfWidth*0.3;
	GLint tmpHeight = bodyHeight*0.9;
	GLint tmpRoofHeight = roofHeight*0.9;
	polygoni(-4, -tmpWidth, 0, -bodyLen - tmpWidth*2, -tmpWidth*2, 0, -bodyLen,
		-tmpWidth * 2, tmpHeight, -bodyLen, -tmpWidth, tmpHeight, -bodyLen - tmpWidth * 2);
	polygoni(-4, tmpWidth, 0, -bodyLen - tmpWidth * 2, -tmpWidth, 0, -bodyLen - tmpWidth * 2,
		-tmpWidth , tmpHeight, -bodyLen - tmpWidth * 2, tmpWidth, tmpHeight, -bodyLen - tmpWidth * 2);
	polygoni(-4, tmpWidth * 2, 0, -bodyLen, tmpWidth, 0, -bodyLen - tmpWidth * 2,
		tmpWidth, tmpHeight, -bodyLen - tmpWidth * 2, tmpWidth * 2, tmpHeight, -bodyLen, tmpWidth);
	glBindTexture(GL_TEXTURE_2D, 0);
	//roof1
	glColor3ub(82, 134, 111);
	polygoni(-4, -bodyHalfWidth, bodyHeight, -bodyLen, -bodyHalfWidth, bodyHeight, 0,
		0, bodyHeight + roofHeight, 0, 0, bodyHeight + roofHeight, -bodyLen);
	//roof2
	polygoni(-4, bodyHalfWidth, bodyHeight, 0, bodyHalfWidth, bodyHeight, -bodyLen,
		0, bodyHeight + roofHeight, -bodyLen, 0, bodyHeight + roofHeight, 0);
	//addiitonal room's roof
	polygoni(-3, -tmpWidth, tmpHeight, -bodyLen - tmpWidth * 2, -tmpWidth * 2, tmpHeight, -bodyLen,
		0, tmpHeight+tmpRoofHeight, -bodyLen);
	polygoni(-3, tmpWidth, tmpHeight, -bodyLen - tmpWidth * 2, -tmpWidth, tmpHeight, -bodyLen - tmpWidth * 2,
		0, tmpHeight + tmpRoofHeight, -bodyLen);
	polygoni(-3, tmpWidth * 2, tmpHeight, -bodyLen, tmpWidth, tmpHeight, -bodyLen - tmpWidth * 2,
		0, tmpHeight + tmpRoofHeight, -bodyLen);
}

/***********************************************************************/
//orthodox church
OrthodoxChurch::OrthodoxChurch() : GrObject("Orthodox Church")
{
}

static void drawOrthoChurchPole(GLint halfWidth, GLint height) {
	glColor3ub(255, 255, 255);
	//front
	polygoni(-4, -halfWidth, 0, halfWidth, halfWidth, 0, halfWidth,
		halfWidth, height, halfWidth, -halfWidth, height, halfWidth);
	//right
	polygoni(-4, halfWidth, 0, halfWidth, halfWidth, 0, -halfWidth,
		halfWidth, height, -halfWidth, halfWidth, height, halfWidth);
	//left
	polygoni(-4, -halfWidth, 0, -halfWidth, -halfWidth, 0, halfWidth,
		-halfWidth, height, halfWidth, -halfWidth, height, -halfWidth);
	//back
	polygoni(-4, halfWidth, 0, -halfWidth, -halfWidth, 0, -halfWidth,
		-halfWidth, height, -halfWidth, halfWidth, height, -halfWidth);
	//top
	glColor3ub(70, 70, 70);
	polygoni(-4, -halfWidth, height, halfWidth, halfWidth, height, halfWidth,
		halfWidth, height, -halfWidth, -halfWidth, height, -halfWidth);
}

static void drawOrthoChurchTower(GLfloat radius, GLfloat height) {
	fetchTexture("orthodox-church-tower.bmp");
	GLUquadricObj *obj = gluNewQuadric();
	gluQuadricTexture(obj, true);
	
	glPushMatrix();
		//pole
		glColor3ub(255, 255, 255);
		glRotatef(-90, 1, 0, 0);
		glRotatef(30, 0, 0,1);
		gluCylinder(obj, radius, radius, height, 20, 10);
		glBindTexture(GL_TEXTURE_2D, 0);
		//middle
		glTranslatef(0, 0, height);
		gluCylinder(obj, radius, radius*1.05, 1.5, 20, 10);
		glTranslatef(0, 0, 1.5);
		gluDisk(obj, 0, radius*1.05, 20, 10);
		//dome
		glColor3ub(80, 80, 80);
		glTranslatef(0, 0, 1.5);
		gluSphere(obj, radius, 20, 20);
		glTranslatef(0, 0, radius-1);
		gluCylinder(obj, radius*0.4, 0, 8, 20, 10);
		//cross
		glRotatef(-30, 0, 0, 1);
		glColor3ub(250, 220, 110);
		glTranslatef(0, 0, 7);
		gluSphere(obj, 0.5, 10, 10);
		glTranslatef(0, 0, 3);		
		glPushMatrix();
			glRotatef(90, 0, 0, 1);
			glScalef(1, 6, 1);
			glutSolidCube(0.5);
		glPopMatrix();
		glTranslatef(0, 0, -0.5);
		glRotatef(90, 1, 0, 0);
		glScalef(1, 9, 1);
		glutSolidCube(0.5);
	glPopMatrix();
}

void OrthodoxChurch::draw(DrawingState*)
{
	GLint outerHalfWidth = 30, outerHeight = 20, innerHalfWidth = 24, innerHeight = 18;

	////outer
	//walls
	fetchTexture("orthodox-church-window.bmp", true, false);
	glColor3ub(255, 255, 255);
	glTranslatef(0, 0, -outerHalfWidth);
	glBegin(GL_QUADS);
		//front
		glNormal3f(0, 0, 1);
		glTexCoord2f(3, 0);	glVertex3i(outerHalfWidth, 0, outerHalfWidth);
		glTexCoord2f(3, 1);	glVertex3i(outerHalfWidth, outerHeight, outerHalfWidth);
		glTexCoord2f(0, 1);	glVertex3i(-outerHalfWidth, outerHeight, outerHalfWidth);
		glTexCoord2f(0, 0);	glVertex3i(-outerHalfWidth, 0, outerHalfWidth);
		//right
		glNormal3f(1, 0, 0);
		glTexCoord2f(0, 0);	glVertex3i(outerHalfWidth, 0, outerHalfWidth);
		glTexCoord2f(3, 0);	glVertex3i(outerHalfWidth, 0, -outerHalfWidth);
		glTexCoord2f(3, 1);	glVertex3i(outerHalfWidth, outerHeight, -outerHalfWidth);
		glTexCoord2f(0, 1);	glVertex3i(outerHalfWidth, outerHeight, outerHalfWidth);
		//back
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(outerHalfWidth, 0, -outerHalfWidth);
		glTexCoord2f(3, 0); glVertex3i(-outerHalfWidth, 0, -outerHalfWidth);
		glTexCoord2f(3, 1); glVertex3i(-outerHalfWidth, outerHeight, -outerHalfWidth);
		glTexCoord2f(0, 1); glVertex3i(outerHalfWidth, outerHeight, -outerHalfWidth);
		//left
		glNormal3f(-1, 0, 0);
		glTexCoord2f(3, 0); glVertex3i(-outerHalfWidth, 0, outerHalfWidth);
		glTexCoord2f(3, 1); glVertex3i(-outerHalfWidth, outerHeight, outerHalfWidth);
		glTexCoord2f(0, 1); glVertex3i(-outerHalfWidth, outerHeight, -outerHalfWidth);
		glTexCoord2f(0, 0); glVertex3i(-outerHalfWidth, 0, -outerHalfWidth);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	//top
	glColor3ub(70, 70, 70);
	polygoni(-4, -outerHalfWidth, outerHeight, outerHalfWidth, outerHalfWidth, outerHeight, outerHalfWidth,
		outerHalfWidth, outerHeight, -outerHalfWidth, -outerHalfWidth, outerHeight, -outerHalfWidth);
	//poles
	glPushMatrix();
		glTranslatef(-outerHalfWidth, 0, outerHalfWidth);
		drawOrthoChurchPole(1,outerHeight);
		glTranslatef(outerHalfWidth*2, 0,0);
		drawOrthoChurchPole(1, outerHeight);
		glTranslatef(0, 0, -2 * outerHalfWidth);
		drawOrthoChurchPole(1, outerHeight);
		glTranslatef(-2*outerHalfWidth, 0,0);
		drawOrthoChurchPole(1, outerHeight);
	glPopMatrix();
	//gate
	glPushMatrix();
		glTranslatef(-outerHalfWidth*0.28, 0, outerHalfWidth +1.5);
		drawOrthoChurchPole(1.5,outerHeight*1.05);
		glTranslatef(outerHalfWidth*0.56, 0, 0);
		drawOrthoChurchPole(1.5, outerHeight*1.05);
		glTranslatef(-outerHalfWidth*0.28, 0, 0);
	glPopMatrix();

	fetchTexture("orthodox-church-gate.bmp");
	glColor3ub(255, 255, 255);
	glPushMatrix();
		glTranslatef(0, 0, outerHalfWidth + 1.5);
		glBegin(GL_QUADS);
			glNormal3f(0, 0, 1);
			glTexCoord2f(1, 0);	glVertex3f(outerHalfWidth*0.25, 0, 0);
			glTexCoord2f(1, 1);	glVertex3f(outerHalfWidth*0.25, outerHeight*1.05, 0);
			glTexCoord2f(0, 1);	glVertex3f(-outerHalfWidth*0.25, outerHeight*1.05, 0);
			glTexCoord2f(0, 0);	glVertex3f(-outerHalfWidth*0.25, 0, 0);
		glEnd();
	glPopMatrix();

	////inner
	fetchTexture("orthodox-church-window.bmp", true, true);
	glColor3ub(255, 255, 255);
	glTranslatef(0, outerHeight, 0);
	glBegin(GL_QUADS);
		//front
		glNormal3f(0, 0, 1);
		glTexCoord2f(2, 0); glVertex3i(innerHalfWidth, 0, innerHalfWidth);
		glTexCoord2f(2, 1); glVertex3i(innerHalfWidth, innerHeight, innerHalfWidth);
		glTexCoord2f(0, 1); glVertex3i(-innerHalfWidth, innerHeight, innerHalfWidth);
		glTexCoord2f(0, 0); glVertex3i(-innerHalfWidth, 0, innerHalfWidth);
		//right
		glNormal3f(1, 0, 0);
		glTexCoord2f(0, 0); glVertex3i(innerHalfWidth, 0, innerHalfWidth);
		glTexCoord2f(2, 0); glVertex3i(innerHalfWidth, 0, -innerHalfWidth);
		glTexCoord2f(2, 1); glVertex3i(innerHalfWidth, innerHeight, -innerHalfWidth);
		glTexCoord2f(0, 1); glVertex3i(innerHalfWidth, innerHeight, innerHalfWidth);
		//back
		glNormal3f(0, 0, -1);
		glTexCoord2f(0, 0); glVertex3i(innerHalfWidth, 0, -innerHalfWidth);
		glTexCoord2f(2, 0); glVertex3i(-innerHalfWidth, 0, -innerHalfWidth);
		glTexCoord2f(2, 1); glVertex3i(-innerHalfWidth, innerHeight, -innerHalfWidth);
		glTexCoord2f(0, 1); glVertex3i(innerHalfWidth, innerHeight, -innerHalfWidth);
		//left
		glNormal3f(-1, 0, 0);
		glTexCoord2f(2, 0); glVertex3i(-innerHalfWidth, 0, innerHalfWidth);
		glTexCoord2f(2, 1); glVertex3i(-innerHalfWidth, innerHeight, innerHalfWidth);
		glTexCoord2f(0, 1); glVertex3i(-innerHalfWidth, innerHeight, -innerHalfWidth);
		glTexCoord2f(0, 0); glVertex3i(-innerHalfWidth, 0, -innerHalfWidth);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	//top
	glColor3ub(70, 70, 70);
	polygoni(-4, -innerHalfWidth, innerHeight, innerHalfWidth, innerHalfWidth, innerHeight, innerHalfWidth,
		innerHalfWidth, innerHeight, -innerHalfWidth, -innerHalfWidth, innerHeight, -innerHalfWidth);
	//poles
	glPushMatrix();
		glTranslatef(-innerHalfWidth, 0, innerHalfWidth);
		drawOrthoChurchPole(1,innerHeight);
		glTranslatef(innerHalfWidth * 2, 0, 0);
		drawOrthoChurchPole(1,innerHeight);
		glTranslatef(0, 0, -2 * innerHalfWidth);
		drawOrthoChurchPole(1,innerHeight);
		glTranslatef(-2 * innerHalfWidth, 0, 0);
		drawOrthoChurchPole(1,innerHeight);
	glPopMatrix();
	
	////tower
	glTranslatef(0, innerHeight, 0);
	drawOrthoChurchTower(innerHalfWidth*0.35, 25);
	glTranslatef(-innerHalfWidth*0.7, 0, innerHalfWidth*0.7);
	drawOrthoChurchTower(innerHalfWidth*0.25, 16);
	glTranslatef(innerHalfWidth*1.4, 0, 0);
	drawOrthoChurchTower(innerHalfWidth*0.25, 16);
	glTranslatef(0, 0, -innerHalfWidth*1.4);
	drawOrthoChurchTower(innerHalfWidth*0.25, 16);
	glTranslatef(-innerHalfWidth*1.4, 0, 0);
	drawOrthoChurchTower(innerHalfWidth*0.25, 16);
}

/***********************************************************************/
//city gate
CityGate::CityGate() : GrObject("City Gate")
{
}

static void drawGityGatePole()
{
	GLUquadricObj *obj = gluNewQuadric();
	GLfloat towerHeight = 40, towerTopHeight = 15, towerRadius = 6;

	glPushMatrix();
		fetchTexture("brick.bmp");
		gluQuadricTexture(obj, true);
		//tower
		glRotatef(-90, 1, 0, 0);		
		glColor3ub(255, 255, 255);
		gluCylinder(obj, towerRadius, towerRadius, towerHeight, 20, 10);
		glBindTexture(GL_TEXTURE_2D, 0);

		//cover
		glTranslatef(0, 0, towerHeight);
		glColor3ub(200, 90, 60);
		gluDisk(obj, 0, towerRadius+0.5, 20, 10);
		glColor3ub(255, 255, 255);
		
		fetchTexture("roof.bmp");
		gluQuadricTexture(obj, true);
		gluCylinder(obj, towerRadius+0.5, 0, towerTopHeight, 20, 10);
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

static void drawGityGateBody()
{
	glPushMatrix();
		GLint halfWidth=20, bodyHeight = 23, roofHeight = 3, halfLen = 4;
		glColor3ub(255, 255, 255);
		fetchTexture("brick.bmp");
		//front
		polygoni(-4, -halfWidth, 0, halfLen, halfWidth, 0, halfLen,
			halfWidth, bodyHeight, halfLen, -halfWidth, bodyHeight, halfLen);
		//back
		polygoni(-4, halfWidth, 0, -halfLen, -halfWidth, 0, -halfLen,
			-halfWidth, bodyHeight, -halfLen, halfWidth, bodyHeight, -halfLen);
		glBindTexture(GL_TEXTURE_2D, 0);
		//right
		glColor3ub(210, 210, 210);
		polygoni(-5, halfWidth, 0, halfLen, halfWidth, 0, -halfLen,
			halfWidth, bodyHeight, -halfLen, halfWidth, bodyHeight+roofHeight,0, halfWidth, bodyHeight, halfLen);
		//left
		polygoni(-5, -halfWidth, 0, -halfLen, -halfWidth, 0, halfLen,
			-halfWidth, bodyHeight, halfLen, -halfWidth, bodyHeight+roofHeight, 0, -halfWidth, bodyHeight, -halfLen);
		//roof
		fetchTexture("roof-small.bmp");
		glColor3ub(255, 255, 255);
		glTranslatef(0, bodyHeight, 0);
		polygoni(-4, -halfWidth, 0, halfLen, halfWidth, 0, halfLen,
			halfWidth, roofHeight, 0, -halfWidth, roofHeight, 0);
		polygoni(-4, halfWidth, 0, -halfLen, -halfWidth, 0, -halfLen,
			-halfWidth, roofHeight, 0, halfWidth, roofHeight, 0);		
	glPopMatrix();
}

void CityGate::draw(DrawingState*)
{
	glTranslatef(-19, 0, 0);
	drawGityGatePole();
	glTranslatef(-19, 0, 0);
	drawGityGateBody();
	glTranslatef(57, 0, 0);
	drawGityGatePole();
	glTranslatef(19, 0, 0);
	drawGityGateBody();
}

/***********************************************************************/
//tv tower
TVTower::TVTower() : GrObject("TV Tower")
{
}

void drawTVTowerPole(GLfloat radius) {
	GLUquadricObj *obj = gluNewQuadric();
	glColor3ub(220, 220, 220);
	glPushMatrix();
		glTranslatef(2, 0, 0);
		glRotatef(30, 0, 1, 0);
		gluCylinder(obj, 0.5, 1, 1.2*radius, 20, 10);
	glPopMatrix();
}

void TVTower::draw(DrawingState* s)
{
	GLfloat baseHeight = 3, towerHeight = 90, middleHeight = 5, aerialHeight = 70, baseRadius = 25, towerRadius = 6, middleRadius = 12, aerialRadius = 2, aerialSectionCount = 10;
	
	////base
	GLUquadricObj *obj = gluNewQuadric();
	glColor3ub(180, 180, 180);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(obj, baseRadius, baseRadius, baseHeight, 20, 10);
	glTranslatef(0, 0, baseHeight);
	gluDisk(obj, 0, baseRadius, 20, 10);
		
	GLfloat mat_emissive_none[4] = { 0.0, 0.0, 0.0, 1.0 };
	int currentTime = s->timeOfDay; 
	bool lightOn = currentTime < 6 || currentTime>19;
	if (lightOn) {
		GLfloat mat_emissive[4] = { 0, 0, 0.5, 1.0 };
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emissive);
	}
	
	////tower
	gluCylinder(obj, towerRadius, towerRadius*0.5, towerHeight, 20, 30);
	
	////middle
	glTranslatef(0, 0, towerHeight);
	if (lightOn) {
		GLfloat mat_emissive[4] = { 0.5, 0, 0, 1.0 };
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emissive);
	}
	//poles
	glPushMatrix();
	for (int i = 0; i < 12; i++) {
		glRotatef(30, 0, 0, 1);
		drawTVTowerPole(middleRadius);
	}
	glPopMatrix();
	//layer1
	glTranslatef(0, 0, 8);
	glColor3ub(180, 180, 180);
	gluCylinder(obj, middleRadius*0.9, middleRadius*0.9, 2, 20, 10);
	gluDisk(obj, 0, middleRadius*0.9, 20, 10);
	glTranslatef(0, 0, 2);
	gluDisk(obj, 0, middleRadius*0.9, 20, 10);
	//layer2
	if (lightOn) {
		GLfloat mat_emissive[4] = { 0, 0.5, 0, 1.0 };
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emissive);
	}
	glColor3ub(0, 183, 238);
	gluCylinder(obj, middleRadius*0.85, middleRadius*0.9, 3.5, 20, 10);
	glTranslatef(0, 0, 3.5);
	//layer3
	glColor3ub(180, 180, 180);
	gluCylinder(obj, middleRadius, middleRadius, 2, 20, 10);
	gluDisk(obj, 0, middleRadius, 20, 10);
	glTranslatef(0, 0, 2);
	gluDisk(obj, 0, middleRadius, 20, 10);
	//layer4
	glColor3ub(220, 220, 220);
	gluCylinder(obj, middleRadius*0.7, middleRadius*0.7, 3, 20, 10);
	glTranslatef(0, 0, 3);
	gluDisk(obj, 0, middleRadius*0.7, 20, 10);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emissive_none);
	
	////aerial
	//base
	glColor3ub(255, 0, 0);
	gluCylinder(obj, middleRadius*0.4, middleRadius*0.3, 6, 20, 10);
	glTranslatef(0, 0, 6);
	gluDisk(obj,0, middleRadius*0.3,20, 10);
	//aerial main
	GLfloat aerialSectionHeight = aerialHeight / aerialSectionCount;
	for (int i = 0; i < aerialSectionCount; i++) {
		if (i % 2 == 0) glColor3ub(255, 255, 255); else glColor3ub(255, 0, 0);
		GLfloat radius1 = (aerialSectionCount - i) / aerialSectionCount*aerialRadius;
		GLfloat radius2 = (aerialSectionCount - i - 1) / aerialSectionCount*aerialRadius;
		gluCylinder(obj, radius1, radius2, aerialSectionHeight, 20, 20);
		glTranslatef(0, 0, aerialSectionHeight);
	}
}

/***********************************************************************/
//indoor market
IndoorMarket::IndoorMarket() : GrObject("Indoor Market")
{
}

static void drawMarketPole(GLint halfWidth, GLint halfLen, GLint height) {
	glColor3ub(220, 200, 190);
	//front
	polygoni(-4, -halfWidth, 0, halfLen, halfWidth, 0, halfLen,
		halfWidth, height, halfLen, -halfWidth, height, halfLen);
	//left
	polygoni(-4, -halfWidth, 0, -halfLen, -halfWidth, 0, halfLen,
		-halfWidth, height, halfLen, -halfWidth, height, -halfLen);
	//right
	polygoni(-4, halfWidth, 0, halfLen, halfWidth, 0, -halfLen,
		halfWidth, height, -halfLen, halfWidth, height, halfLen);
	//back
	polygoni(-4, halfWidth, 0, -halfLen, -halfWidth, 0, -halfLen,
		-halfWidth, height, -halfLen, halfWidth, height, -halfLen);
	//top
	polygoni(-4, -halfWidth, height, halfLen, halfWidth, height, halfLen,
		halfWidth, height, -halfLen, -halfWidth, height, -halfLen);

}

static void drawMarketConnection(GLint halfWidth, GLint height, GLint length) {
	//front
	glColor3ub(220, 200, 190);
	fetchTexture("market-side.bmp", true);
	polygoni(-4, -halfWidth, 0, -4, halfWidth, 0, -4,
		halfWidth, height, -4, -halfWidth, height, -4);
	//back
	polygoni(-4, halfWidth, 0, -length + 4, -halfWidth, 0, -length + 4,
		-halfWidth, height, -length + 4, halfWidth, height, -length + 4);
	glBindTexture(GL_TEXTURE_2D, 0);
	//top
	glColor3ub(81, 100, 110);
	polygoni(-4, -halfWidth, height, 0, halfWidth, height, 0,
		halfWidth, height, -length, -halfWidth, height, -length);
}

static void drawMarketRoom(GLint halfWidth, GLint height, GLint length, GLint roofHeight)
{
	glPushMatrix();
		////walls
		glColor3ub(220, 200, 190);
		fetchTexture("market-front.bmp");
		//front
		polygoni(-4, -halfWidth, 0, 0, halfWidth, 0, 0,
			halfWidth, height, 0, -halfWidth, height, 0);
		//back
		polygoni(-4, halfWidth, 0, -length, -halfWidth, 0, -length,
			-halfWidth, height, -length, halfWidth, height, -length);
		fetchTexture("market-side.bmp",true,false);
		glBegin(GL_QUADS);
			//right
			glNormal3f(1, 0, 0);
			glTexCoord2f(0, 0);	glVertex3i(halfWidth, 0, 0);
			glTexCoord2f(6, 0);	glVertex3i(halfWidth, 0, -length);
			glTexCoord2f(6, 1);	glVertex3i(halfWidth, height, -length);
			glTexCoord2f(0, 1);	glVertex3i(halfWidth, height, 0);
			//left
			glNormal3f(-1, 0, 0);
			glTexCoord2f(6, 0); glVertex3i(-halfWidth, 0, 0);
			glTexCoord2f(6, 1); glVertex3i(-halfWidth, height, 0);
			glTexCoord2f(0, 1); glVertex3i(-halfWidth, height, -length);
			glTexCoord2f(0, 0); glVertex3i(-halfWidth, 0, -length);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		//top
		glColor3ub(81, 100, 110);
		polygoni(-4, -halfWidth, height, 0, halfWidth, height, 0,
			halfWidth, height, -length, -halfWidth, height, -length);
		
		////curved roof
		glTranslatef(0, height, 0);
		for (int i = 0; i < 6; i++) {
			float theta1 = i / 6.0 * pi;
			float theta2 = (i + 1) / 6.0 * pi;
			glColor3ub(220, 200, 190);
			GLint x1 = halfWidth*cos(theta1), y1 = roofHeight*sin(theta1), x2 = halfWidth*cos(theta2), y2 = roofHeight*sin(theta2);
			glBegin(GL_TRIANGLES);
				glNormal3f(0, 0, 1);
				glVertex3i(0, 0, 0);
				glVertex3i(x1, y1, 0);
				glVertex3i(x2, y2, 0);
				glVertex3i(0, 0, -length+2);
				glVertex3i(x1, y1, -length + 2);
				glVertex3i(x2, y2, -length + 2);
			glEnd();
			glFrontFace(GL_CW);
			glBegin(GL_TRIANGLES);
				glNormal3f(0, 0, -1);
				glVertex3i(0, 0, -length);
				glVertex3i(x1, y1, -length);
				glVertex3i(x2, y2, -length);
				glVertex3i(0, 0, -2);
				glVertex3i(x1, y1, -2);
				glVertex3i(x2, y2, -2);
			glEnd();
			glFrontFace(GL_CCW);
			polygoni(-4, x1, y1, 0, x1, y1, -2, x2, y2, -2, x2, y2, 0);
			polygoni(-4, x1, y1, -length + 2, x1, y1, -length, x2, y2, -length, x2, y2, -length+2);
			//roof
			glColor3ub(81, 100, 110);
			x1 = halfWidth*cos(theta1)*0.95, y1 = roofHeight*sin(theta1)*0.95, x2 = halfWidth*cos(theta2)*0.95, y2 = roofHeight*sin(theta2)*0.95;
			polygoni(-4, x1, y1, -2, x1, y1, -length+2,	x2, y2, -length+2, x2, y2, -2);
		}
	glPopMatrix();
}

void IndoorMarket::draw(DrawingState*)
{
	GLint roomHalfWidth = 25, roomHeight = 15, roomLength = 80, roofHeight = 20, conHalfWidth = 5; 

	drawMarketRoom(roomHalfWidth,roomHeight,roomLength,roofHeight);
	glPushMatrix();
		glTranslatef(-roomHalfWidth-conHalfWidth, 0, 0);
		drawMarketConnection(conHalfWidth, roomHeight, roomLength);
		glTranslatef(-roomHalfWidth - conHalfWidth, 0, 0);
		drawMarketRoom(roomHalfWidth, roomHeight, roomLength, roofHeight);
		//poles
		glTranslatef(-roomHalfWidth + 6, roomHeight, -8);
		drawMarketPole(3, 3, 20);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(roomHalfWidth + conHalfWidth, 0, 0);
		drawMarketConnection(conHalfWidth, roomHeight, roomLength);
		glTranslatef(roomHalfWidth + conHalfWidth, 0, 0);
		drawMarketRoom(roomHalfWidth, roomHeight, roomLength, roofHeight);
	glPopMatrix();

}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
StreetLight::StreetLight(double p) : poleZ(p)
{
	radius = 10;
}
void StreetLight::draw(DrawingState*)
{
  glColor3f(.6f,.5f,.3f);
  glBegin(GL_QUAD_STRIP);
  glNormal3d( 1, 0,0);
  glVertex3d( 1, 0,poleZ);
  glVertex3d( 1,20,poleZ);
  glNormal3d( 0, 0,1);
  glVertex3d( 0, 0,poleZ+1);
  glVertex3d( 0,20,poleZ+1);
  glNormal3d(-1, 0,0);
  glVertex3d(-1, 0,poleZ);
  glVertex3d(-1,20,poleZ);
  glNormal3d( 0, 0,-1);
  glVertex3d( 0, 0,poleZ-1);
  glVertex3d( 0,20,poleZ-1);
  glNormal3d( 1, 0,0);
  glVertex3d( 1, 0,poleZ);
  glVertex3d( 1,20,poleZ);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3d(0,1,0);
  glVertex3d( 0,21, 0);
  glVertex3d( 0,21, poleZ);
  glNormal3d(-.707,-.707, 0);
  glVertex3d(-1,20, 0);
  glVertex3d(-1,20, poleZ);
  glNormal3d(.707,-.707, 0);
  glVertex3d( 1,20, 0);
  glVertex3d( 1,20, poleZ);
  glNormal3d(0,1,0);
  glVertex3d( 0,21, 0);
  glVertex3d( 0,21, poleZ);
	glEnd();
}

void StreetLight::drawAfter(DrawingState* d)
{
  int tod = d->timeOfDay;
  bool daytime;
  if (tod >=6 && tod <= 19)
	daytime = true;
  else 
	daytime = false;

  if (!daytime) {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
	float cone_length = 20;
	float radius = 10;
	float theta,dtheta = pi/16;
	int c;

	float norm[3];
	float sum;
	// Beam One
	glPushMatrix();
			
	glFrontFace(GL_CW);
			
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(1,1,0,.8f);
	glVertex3f(0,cone_length,0);
	for ( c=0,theta = 0.0 ; c <= 32  ; theta += dtheta, c++ ) {
	  float z = sin(theta) * radius;
	  float x = cos(theta) * radius;
	  glColor4f(1,1,0,.2f);
	
	  norm[0] = 0; norm[1] = x; norm[2] = z; sum = norm[0] + norm[1] + norm[2];
	  norm[0] /= sum; norm[1] /= sum; norm[2] /= sum;				
	  glNormal3f( norm[0],norm[1],norm[2]);
	
	  glVertex3f( x, cone_length/6, z );
	}  
			
	glEnd();

			
	glPopMatrix();

	glDisable(GL_BLEND);
	glClearColor(1,1,1,1);

  }
}

/**********************************************************************/
Sign::Sign(float sxi, float syi, float hi, char* tex, 
		   SignShape ss)
  : sx(sxi), sy(syi), h(hi), texName(tex), shape(ss)
{
}
void Sign::draw(DrawingState*)
{
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
  fetchTexture(texName);
  glBegin(GL_POLYGON);
  glNormal3f(0,0,1);
  glColor3f(1,1,1);
  drawShape(0);
  glEnd();
  // back of the sign
  glBindTexture(GL_TEXTURE_2D,0);
  glNormal3f(0,0,-1);
  drawShape(-.1f);
  // post
  glBegin(GL_POLYGON);
    glNormal3f(0,0,1);
	glVertex3f(-.1f,h,0);
	glVertex3f( .1f,h,0);
	glVertex3f( .1f,0,0);
	glVertex3f(-.1f,0,0);
  glEnd();
  glFrontFace(GL_CW);
  glBegin(GL_POLYGON);
    glNormal3f(0,0,1);
	glVertex3f(-.1f,h,-.05f);
	glVertex3f( .1f,h,-.05f);
	glVertex3f( .1f,0,-.05f);
	glVertex3f(-.1f,0,-.05f);
  glEnd();
  glFrontFace(GL_CCW);	
}
void Sign::drawShape(float z)
{
  glFrontFace(GL_CW);
  switch(shape) {
  case rectangle:
	glBegin(GL_POLYGON);
	glTexCoord2f(0.,0.); glVertex3f(-sx/2,h,z);
	glTexCoord2f(1.,0.); glVertex3f( sx/2,h,z);
	glTexCoord2f(1.,1.); glVertex3f( sx/2,h+sy,z);
	glTexCoord2f(0.,1.); glVertex3f(-sx/2,h+sy,z);
	glEnd();
	break;
  case octagon:
	glBegin(GL_POLYGON);
	glTexCoord2f(0.25f,0.f); glVertex3f(-sx/4,h,z);
	glTexCoord2f(0.75f,0.f); glVertex3f( sx/4,h,z);
	glTexCoord2f(1.f,.25f); glVertex3f( sx/2.f,h+    sy/4.f,z);
	glTexCoord2f(1.f,.75f); glVertex3f( sx/2.f,h+3.f*sy/4.f,z);
	glTexCoord2f(.75f,1); glVertex3f( sx/4,h+sy,z);
	glTexCoord2f(.25f,1); glVertex3f(-sx/4,h+sy,z);
	glTexCoord2f(0,.75f); glVertex3f(-sx/2.f,h+3.f*sy/4.f,z);
	glTexCoord2f(0,.25f); glVertex3f(-sx/2.f,h+sy/4.f,z);
	glEnd();
	break;
  case diamond:
	glBegin(GL_POLYGON);
	glTexCoord2f(0.45f, 0.f); glVertex3f(-sx/10,h,z);
	glTexCoord2f(0.55f, 0.f); glVertex3f( sx/10,h,z);
	glTexCoord2f(   1 ,.45f); glVertex3f( sx/2,h+9*sy/20.f,z);
	glTexCoord2f(   1 ,.55f); glVertex3f( sx/2,h+11*sy/20.f,z);
	glTexCoord2f(.55f, 1   ); glVertex3f( sx/10,h+sy,z);
	glTexCoord2f(.45f, 1   ); glVertex3f(-sx/10,h+sy,z);
	glTexCoord2f(   0 ,.55f); glVertex3f(-sx/2,h+11*sy/20.f,z);
	glTexCoord2f(   0 ,.45f); glVertex3f(-sx/2,h+ 9*sy/20.f,z);
	glEnd();
	break;
  };
  glFrontFace(GL_CCW);
}

// make the houses we create be from a set of colors
/////////////////////////////////////////
// stuff for all houses
static int houseColors[][3] = {
		{ 255, 255, 255 }, { 255, 204, 102 }, //white, orange
		{ 255, 247, 153 }, { 172, 213, 152 }, //yellow, green
		{ 220, 220, 220 }, { 242, 155, 118 }  //red, gray
};
int nHouseColors = 6;

////////////////////////////////////////////////////////////////////////
// draw a little suburban dream...
// a 100x200 lot...
SimpleLot::SimpleLot(int ht, int hc)  
{
  // make the things we want
  add(new Lawn(0,0,100,200),0,0,0,0);
  switch(ht % 3) {
  case 0: {
	  SimpleHouse1* h1 = new SimpleHouse1();
	  h1->color(houseColors[hc % nHouseColors]);
	  add(h1,50.,0,60.,0);
	  break; }
  case 1: {
	  SimpleHouse2* h2 = new SimpleHouse2();
	  h2->color(houseColors[hc% nHouseColors]);
	  add(h2,50.,0,60.,0);
	  break; }
  case 2: {
	  SimpleHouse3* h3 = new SimpleHouse3();
	  h3->color(houseColors[hc% nHouseColors]);
	  add(h3,50.,0,60.,0);
	  break; }
  } 
  add(new SimpleTree1(15,5), 10,0, 10,0);
  add(new SimpleTree1(15,5), 10,0,190,0);
  add(new SimpleTree1(15,5), 30,0,190,0);
  add(new SimpleTree1(15,5), 50,0,190,0);
  add(new SimpleTree1(15,5), 70,0,190,0);
}

/***********************************************************************/
// a set of houses going down the street
// the street is along the X axis from (0,0) to ( (nh+1)*100, 0)
SimpleSubdivision::SimpleSubdivision(int nh) 
{
	// street lights in front of every other house
	// alternate sides of the street
	for(int sl=0; sl<nh; sl ++) {
		add(new StreetLight(sl%2 ? 9 : -9), static_cast<float>(sl*100 + 25), 0, (float)((sl%2) ? 8 : -9));
	}
	
    // add houses - one on each side of the street
	for (int hc=0; hc<nh; hc++) {
		GrObject* g1 = new SimpleLot(rand(), rand());
		GrObject* g2 = new SimpleLot(rand(), rand());

		add(g1, (float) (hc*100)    ,0, 15, 0);
		add(g2, (float) (hc*100+100),0,-15,pi);
	}

  add(new Sign(4,4,4,"stop.png",octagon),(float)(100*nh-2),0,17,-(3.141592f/2.f));
  add(new Sign(4,4,4,"stop.png",octagon),  2,0,-17 ,(3.141592f/2.f));
}


