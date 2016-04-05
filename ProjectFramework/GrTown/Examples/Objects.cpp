// written by Michael   - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - new texture manager, improvements for behaviors

#include "../GrTown_PCH.H"
#include "Objects.H"
#include "../DrawUtils.H"
#include "Utilities/Texture.H"
#include "../MMatrix.H"
#include <FL/fl_ask.h>
#include <GL/glut.h>

#include "Utilities/ShaderTools.H"

Particle::Particle(float _life, const glm::vec3 &_speed, Color &_color, const glm::vec3 &_pos,
	float _weight, float _size, float _angle)
	: life(_life), speed(_speed), color(_color), pos(_pos), weight(_weight), size(_size), angle(_angle) {
	this->birth = clock();
}

void Particle::draw(DrawingState*) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	glPushMatrix();
	glColor4f(color.r, color.g, color.b, color.a);
	glRotatef(angle, 0, 1, 0);
	glScalef(size, size, size);
	glTranslatef(pos[0] / size, pos[1] / size, pos[2] / size);
	glutSolidSphere(1.0, 5, 5);
	glPopMatrix();
	glDisable(GL_BLEND);
}

glm::vec3 rand_water_speed() {
	glm::vec3 v(0.0f, 1.0f, 0.0f);
	v = glm::rotate(v, (float)rand() / RAND_MAX * 10 + 10, glm::vec3(1.0f, 0.0f, 0.0f));
	v = glm::rotate(v, (float)rand() / RAND_MAX * 360, glm::vec3(0.0f, 1.0f, 0.0f));
	return v;
}

Fountain::Fountain() : GrObject("Fountain") {
	color(.4f, 0.698f, 1.0f, 1.0f);
	max_particles = 15000;
	gen_rate = 130;
	life = 4;
	evl_rate = 30;
	g = glm::vec3(0.0f, -1.0f, 0.0f);
	last_gen_clock = last_evl_clock = clock();
}

void Fountain::gen_particles() {
	clock_t c = clock();
	while (!particles.empty() && particles.front().birth + particles.front().life * CLOCKS_PER_SEC <= c) {
		particles.pop_front();
	}
	double t = (double)(c - last_gen_clock) / CLOCKS_PER_SEC;
	for (int gen = (int)(t * gen_rate); gen > 0 && particles.size() < max_particles; gen--) {
		particles.push_back(Particle(life, rand_water_speed(), color, glm::vec3(0.0, 0.0, 0.0), 1.0f, (float)rand() / RAND_MAX * 0.2 + 0.1, (float)rand() / RAND_MAX * 360));
		last_gen_clock = c;
	}
}

void Fountain::evolute() {
	clock_t c = clock();
	double t = (double)(c - last_evl_clock) / CLOCKS_PER_SEC;
	for (int evl = (int)(t * evl_rate); evl > 0; evl--) {
		for (std::deque<Particle>::iterator it = particles.begin(); it != particles.end(); it++) { // for each generated particle
			it->pos += it->speed;
			glm::vec3 a = it->weight * g;
			a *= t;
			it->speed += a;
			it->color.a *= 0.96;
		}
		last_evl_clock = c;
	}
}

void Fountain::draw(DrawingState *d) {
	////base
	GLfloat baseRadius1 = 18, baseRadius2 =10;
	GLUquadricObj *obj = gluNewQuadric();
	glColor3ub(230, 214, 160);
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		//layer1
		gluCylinder(obj, baseRadius1, baseRadius1, 2, 30, 10);
		gluCylinder(obj, baseRadius1-1, baseRadius1-1, 2, 30, 10);
		//water area
		glTranslatef(0, 0, 1.5);
		glColor3ub(153,217,234);
		gluDisk(obj, 0, baseRadius1-1, 30, 10);
		//edge
		glColor3ub(230, 214, 160);
		glTranslatef(0, 0, 0.5);
		gluDisk(obj, baseRadius1-1, baseRadius1, 30, 10);
		glTranslatef(0, 0, -0.5);
		//layer2
		gluCylinder(obj, 4, 4, 2, 30, 10);
		glTranslatef(0, 0, 2);
		gluDisk(obj, 0, 4, 30, 10);
		gluCylinder(obj, 3, 1.5, 1.5, 30, 10);
		glTranslatef(0, 0, 1.5);
		gluCylinder(obj, 1.5, 1.5, 5, 30, 10);
		glTranslatef(0, 0, 5);
		gluDisk(obj, 0, 2, 30, 10);
		gluCylinder(obj, 2, 2, 1, 30, 10);
		glTranslatef(0, 0, 1);
		gluCylinder(obj, 2, baseRadius2, 2, 30, 10);
		glTranslatef(0, 0, 2);
		gluCylinder(obj, baseRadius2, baseRadius2, 2, 30, 10);
		//water area
		glTranslatef(0, 0, 1.5);
		glColor3ub(153, 217, 234);
		gluDisk(obj, 0, baseRadius2 - 1, 30, 10);
		//edge
		glColor3ub(230, 214, 160);
		glTranslatef(0, 0, 0.5);
		gluDisk(obj, baseRadius2 - 1, baseRadius2, 30, 10);
	glPopMatrix();
	////fountain
	glTranslatef(0, 15, 0);
	this->gen_particles();
	this->evolute();
	for (std::deque<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
		it->draw(d);
	}
	
}



SnowMan::SnowMan(float x, float y, float z, float size)
{
	transMatrix(transform, x, y, z); // set init position 
	scale = size;
}

void SnowMan::draw(DrawingState*)
{
	glPushMatrix(); // save current position
	glScalef(scale, scale, scale);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glTranslatef(0.0, 0.5, 0.0);
	GLUquadricObj * ball1 = gluNewQuadric();
	gluQuadricTexture(ball1, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, snowTexture);
	fetchTexture("snow1.bmp");
	gluSphere(ball1, 0.5, 30, 20);
	glTranslatef(0.0, 0.75 - 0.05, 0.0);
	gluSphere(ball1, 0.3, 30, 20);
	glBindTexture(GL_TEXTURE_2D, 0);
	/*
	// Body
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sandTexture);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glTranslatef(0.0, 0.5, 0.0);
	glutSolidSphere(0.5, 30, 25);
	// Head
	glTranslatef(0.0, 0.75 - 0.05, 0.0);
	glutSolidSphere(0.3, 20, 20);
	glDisable(GL_TEXTURE_2D);
	*/
	// Nose
	glColor4f(1, 0.2, 0.16, 1.0);
	glTranslatef(0.0, 0.0, 0.3);
	glutSolidCone(0.036, 0.4, 9, 6);

	// Eyes
	glColor4f(0, 0, 0, 1.0);
	glTranslatef(-0.1, 0.06, -0.02);
	glutSolidSphere(0.025, 10, 10);
	glTranslatef(0.18, 0.0, 0.0);
	glutSolidSphere(0.025, 10, 10);

	// hat
	glColor4f(1, 1, 1, 1.0);
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, hatTexture);
	fetchTexture("hat.bmp");
	glTranslatef(-0.1, 0.25, -0.3);
	glRotatef(90, -1, 0, 0);
	// glutSolidCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks);
	//glutSolidTeapot(0.2);
	//glutSolidCone(0.23, 0.5, 10, 5);
	GLUquadric *qobj = gluNewQuadric();
	gluCylinder(qobj, 0.23, 0.08, 0.5, 10, 30);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

// some very basic classes of objects...
// mainly for debugging
Cube::Cube(float x, float y, float z, float s, 
		   float r, float g, float b)
  : color(r,g,b), size(s)
{
  transMatrix(transform,x,y,z);
}
void Cube::draw(DrawingState*)
{
  glColor4fv(&color.r);
  cube(0,0,0,size);
}

/***********************************************************************/
TestCubes::TestCubes()
{
}

void TestCubes::draw(DrawingState*)
{
  glColor3f(1,0,0);
  cube( 20,0,0, 3);
  cube(-20,0,0, 3);
  cube(0, 20,0, 3);
  cube(0,-20,0, 3);
  cube(0,0, 20, 3);
  cube(0,0,-20, 3);

  glColor3f(1,1,0);
  cube(20,10,20,3);
  glColor3f(0,1,1);
  cube(-20,10,20,3);
  glColor3f(1,0,1);
  cube(-20,10,-20,3);
  glColor3f(1,1,1);
  cube(20,10,-20,3);
}

// some very basic classes of objects...
// mainly for debugging
ShadedCube::ShadedCube(float x, float y, float z, float s, 
					   float r, float g, float b)
  : color(r,g,b), size(s)
{
  transMatrix(transform,x,y,z);
}

// load the shader in the first time the cube draws (since that way we
// know we have a GL context)
// we only try to load it once
static unsigned int shadedCubeShader = 0;
static bool triedCubeShader=false;

void ShadedCube::draw(DrawingState*)
{
	if (!shadedCubeShader && !triedCubeShader) {
		triedCubeShader = true;
		char* error;
		if (!(shadedCubeShader=loadShader("ShadedCubeTest.vert","ShadedCubeTest.frag",error))) {
			std::string s = "Can't Load Cube Shader:";
			s += error;
			fl_alert(s.c_str());
		}
	}

	glUseProgram(shadedCubeShader);
	glColor4fv(&color.r);
	cube(0,0,0,size);
	glUseProgram(0);
}

//sky

Sky::Sky() : GrObject("Sky")
{
	dayTexture = 0;
	duskTexture = 0;
	nightTexure = 0;
}

void Sky::attachTexture(int currentTime)
{	
	// better to bind once
	char* skyTextureDay = "sky_day_0.png";
	//char* skyTextureDay = "sky_day_3.jpg";
	char* skyTexureNight = "sky_night_0.png";
	char* skyTexureDusk = "sky_dwan_2.jpg";

	// load 
	if (!dayTexture)
	{
		dayTexture = fetchTexture(skyTextureDay);
	}

	if (!duskTexture)
	{
		duskTexture = fetchTexture(skyTexureDusk);
	}

	if (!nightTexure)
	{
		nightTexure = fetchTexture(skyTexureNight);
	}

	dayTexture->bind();
	if (currentTime < 5) {
		glColor3f(0.9, 0.9, 0.9); 
		//fetchTexture(skyTexureNight);
		nightTexure->bind();
	}else if (currentTime < 7) {
		glColor3f(0.9, 0.9, 0.9);
		//fetchTexture(skyTexureNight);
		duskTexture->bind();
	}
	else if (currentTime < 10) {
		float amt = (((float)currentTime) - 6) / 3.0f;
		glColor3f(amt, amt, amt);
		//fetchTexture(skyTextureDay);
		dayTexture->bind();
	
	}
	else if (currentTime < 17) {
		glColor3f(1, 1, 1);
		//fetchTexture(skyTextureDay);
		dayTexture->bind();
	}
	else if (currentTime < 20) {
		float amt = (20 - (float)currentTime) / 3.0f;
		glColor3f(amt, amt, amt);
		//fetchTexture(skyTexureDusk);
		duskTexture->bind();
	}
	else {
		glColor3f(0.9, 0.9, 0.9);
		//fetchTexture(skyTexureNight);
		nightTexure->bind();
	}
}

void Sky::draw(DrawingState * d)
{
	//int currentTime = d->timeOfDay;
	glPushMatrix();
	glScalef(10000, 10000, 10000);

	glDisable(GL_LIGHTING);
	attachTexture(d->timeOfDay);

	glBegin(GL_QUADS);
	//front
	glNormal3f(0, 0, 1);
	glTexCoord2f(.25,.75);
	glVertex3f(-1, -1, -1);
	glTexCoord2f(.25,  .5);
	glVertex3f( 1, -1, -1);
	glTexCoord2f(.5,  .5);
	glVertex3f( 1,  1, -1);
	glTexCoord2f(.5, .75);
	glVertex3f(-1,1,-1);

	//right
	glNormal3f(-1, 0, 0);
	//glColor3f(0, 0, 1);
	glTexCoord2f(.25,  .5);
	glVertex3f(1, -1, -1);
	glTexCoord2f(.25, .25);
	glVertex3f(1, -1, 1);
	glTexCoord2f( .5, .25);
	glVertex3f(1, 1, 1);
	glTexCoord2f( .5,  .5);
	glVertex3f(1, 1, -1);

	//back
	glNormal3f(0, 0, -1);
	//glColor3f(0, 1, 0);
	glTexCoord2f(.25,   0);
	glVertex3f(-1, -1, 1);
	glTexCoord2f( .5,   0);
	glVertex3f(-1,  1, 1);
	glTexCoord2f( .5, .25);
	glVertex3f( 1,  1, 1);
	glTexCoord2f(.25, .25);
	glVertex3f( 1, -1, 1);

	//left
	glNormal3f(1, 0, 0);
	//glColor3f(0, 1, 1);
	glTexCoord2f(.25, .75);
	glVertex3f(-1, -1,  -1);
	glTexCoord2f( .5, .75);
	glVertex3f(-1, 1,  -1);
	glTexCoord2f( .5, 1);
	glVertex3f(-1,  1,  1);
	glTexCoord2f(.25, 1);
	glVertex3f(-1, -1,  1);
	//top
	glNormal3f(0, -1, 0);
	//glColor3f(1, 0, 0);
	glTexCoord2f( .5, .75);
	glVertex3f(-1.01, 0.99, -1.01);
	glTexCoord2f( .5,  .5);
	glVertex3f(1.01, 0.99, -1.01);
	glTexCoord2f(.75,  .5);
	glVertex3f(1.01, 0.99, 1.01);
	glTexCoord2f(.75, .75);
	glVertex3f(-1.01, 0.99, 1.01);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

