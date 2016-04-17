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

Particle::Particle(float _life, const glm::vec3 &_speed, Color &_color, const glm::vec3 &_pos,float _weight, float _size, float _angle) {
	this->life = _life;
	this->speed = _speed;
	this->color = _color;
	this->pos = _pos;
	this->weight = _weight;
	this->size = _size;
	this->angle = _angle;
	this->birth = clock();
}

void Particle::draw(DrawingState*) {
	glPushMatrix();glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	glScalef(size, size, size);
	glRotatef(angle, 0, 1, 0);
	glColor4f(color.r, color.g, color.b, color.a);
	glTranslatef(pos[0] / size, pos[1] / size, pos[2] / size);
	glutSolidSphere(1.0, 5, 5); // a particle instance
	glDisable(GL_BLEND); glPopMatrix();
}

glm::vec3 rand_water_speed(float direction,float height) {
	glm::vec3 v(0.0f, 1.0f, 0.0f); // inital value
	v = glm::rotate(v, ((float)rand() / RAND_MAX * 10 + 10) * direction, glm::vec3(1.0f, 0.0f, 0.0f));
	v = glm::rotate(v, (float)rand() / RAND_MAX * 360, glm::vec3(0.0f, 1.0f, 0.0f));
	//v = glm::translate(v, glm::vec3(0.0f,1.0f,0.0f));
	v *= glm::vec3(1.f, height, 1.f); // adjust initial up speed
	return v;
}

Fountain::Fountain() : GrObject("Fountain") {
	max_particle_number = 25000;
	life = 4;
	evolution_rate = 30;
	generate_rate = 130;
	// set color
	color(.4f, 0.698f, 1.0f, 1.0f);
	color_layer2(1.f, 0.398f, 0.f, 1.0f);
	
	g = glm::vec3(0.0f, -1.0f, 0.0f);
	g2 = glm::vec3(0.0f, -0.5f, 0.0f);
	last_evolution_timer = clock();
	last_generate_timer = last_evolution_timer;
}

void Fountain::generate() {
	clock_t currentTime = clock();
	// end life of old particles
	while (!particles.empty() && particles.front().birth + particles.front().life * CLOCKS_PER_SEC <= currentTime) {
		particles.pop_front();
	}

	while (!particles_layer2.empty() && particles_layer2.front().birth + particles_layer2.front().life * CLOCKS_PER_SEC <= currentTime) {
		particles_layer2.pop_front();
	}
	double t = (double)(currentTime - last_generate_timer) / CLOCKS_PER_SEC;
	for (int gen = (int)(t * generate_rate); gen > 0 && particles.size() < max_particle_number; gen--) {
		particles.push_back(Particle(life, rand_water_speed(1.5,1.1), color, glm::vec3(0.0, 0.0, 0.0), 1.0f, (float)rand() / RAND_MAX * 0.15 + 0.1, (float)rand() / RAND_MAX * 360));
		last_generate_timer = currentTime;
	}

	for (int gen = (int)(t * generate_rate); gen > 0 && particles_layer2.size() < max_particle_number; gen--) {
		particles_layer2.push_back(Particle(life, rand_water_speed(0.3,1.5), color, glm::vec3(0.0, 0.0, 0.0), 1.0f, (float)rand() / RAND_MAX * 0.15 + 0.1, (float)rand() / RAND_MAX * 360));
		last_generate_timer = currentTime;
	}

	currentTime = clock();
	t = (double)(currentTime - last_evolution_timer) / CLOCKS_PER_SEC;
	for (int evl = (int)(t * evolution_rate); evl > 0; evl--) {
		for (std::deque<Particle>::iterator it = particles.begin(); it != particles.end(); it++) { // for each generated particle
			it->pos += it->speed;
			glm::vec3 a = it->weight * g;
			a *= t;
			it->speed += a;
			it->color.a *= 0.98;
		}
		for (std::deque<Particle>::iterator it = particles_layer2.begin(); it != particles_layer2.end(); it++) { // for each generated particle
			it->pos += it->speed;
			glm::vec3 a = it->weight * g;
			a *= t;
			it->speed += a;
			it->color.a *= 0.98;
		}

		last_evolution_timer = currentTime;
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
	this->generate();
	// draw for each particles 
	for (std::deque<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
		it->draw(d);
	}
	for (std::deque<Particle>::iterator it = particles_layer2.begin(); it != particles_layer2.end(); it++) {
		it->draw(d);
	}
	
}

//======== firework =======
glm::vec3 rand_fire_speed() {
	glm::vec3 v(0.0f, 1.0f, 0.0f);
	v = glm::rotate(v, (float)rand() / RAND_MAX * 360, glm::vec3(1.0f, 0.0f, 0.0f));
	v = glm::rotate(v, (float)rand() / RAND_MAX * 360, glm::vec3(0.0f, 1.0f, 0.0f));
	return v;
}

Fireworks::Fireworks(Color _color1, Color _color2, Color _color3) : GrObject("Fireworks"), color(_color1),end_color(_color2),end_color_2(_color3) {
	max_particle_number = 10000;
	generate_rate = 100;
	life = 5;
	evolution_rate = 30;
	gen_evl = 0;
	g = glm::vec3(0.0f, -1.0f, 0.0f);

	color.a = 1.0f;
	end_color.a = 1.0f;
	end_color_2.a = 1.0f;
	//end_color = Color(1.0f, 0.0f, 0.0f);
	//end_color_2 = Color(0.0f, 0.0f, 1.0f);

	start = false;
	color1_has_reach = false;
	color2_has_reach = false;

	last_evolution_timer = clock();
	last_generate_timer = last_evolution_timer;
}

bool colors_are_similar(Color c1, Color c2, float tolerance){
	// if the difference of r,g,b component is within tolerance
	if ((abs(c1.r - c2.r) <= tolerance) && (abs(c1.g - c2.g) <= tolerance) && (abs(c1.b - c2.b) <= tolerance)){
		return true;
	}
	return false;
}

void Fireworks::generate() {
	clock_t currentTime = clock();
	while (!particles.empty() && particles.front().birth + particles.front().life * CLOCKS_PER_SEC <= currentTime) {
		particles.pop_front();
	}
	double t = (double)(currentTime - last_generate_timer) / CLOCKS_PER_SEC;
	int evl = (int)(t * evolution_rate);
	if (evl > 0) last_generate_timer = currentTime;
	for (; evl > 0 && gen_evl > 0; evl--, gen_evl--) {
		for (int gen = 0; gen < generate_rate && particles.size() < max_particle_number; gen++) {
			particles.push_back(Particle(life, rand_fire_speed(), color, glm::vec3(0.0, 0.0, 0.0), 0.05f, (float)rand() / RAND_MAX * 0.3 + 0.3, (float)rand() / RAND_MAX * 360));
		}
	}

	currentTime = clock();
	t = (double)(currentTime - last_evolution_timer) / CLOCKS_PER_SEC;
	for (int evl = (int)(t * evolution_rate); evl > 0; evl--) {
		for (std::deque<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
			it->pos += it->speed;
			glm::vec3 a = it->weight * g;
			a *= t;
			it->speed += a;

			// change the color
			float r_diff = end_color.r - color.r;
			float g_diff = end_color.g - color.g;
			float b_diff = end_color.b - color.b;

			float r_diff_1 = end_color_2.r - end_color.r;
			float g_diff_1 = end_color_2.g - end_color.g;
			float b_diff_1 = end_color_2.b - end_color.b;

			// update color if not reach target color 1
			if (!colors_are_similar(it->color, end_color, 0.001)){
				it->color.r += r_diff / 30;
				it->color.g += g_diff / 30;
				it->color.b += b_diff / 30;
			}
			else{
				//std::cout << "color1 reached"<< std::endl;
				color1_has_reach = true;
			}
			// shift to color2

			if (color1_has_reach){
				if (!colors_are_similar(it->color, end_color_2, 0.001)){
					it->color.r += r_diff_1 / 15;
					it->color.g += g_diff_1 / 15;
					it->color.b += b_diff_1 / 15;
				}
			}
			it->color.a *= 0.98;
		}
		last_evolution_timer = currentTime;
	}
}


Color getRandomColor()
{
	return Color((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
}

void Fireworks::draw(DrawingState *d) {
	if ((d->timeOfDay > 4 && d->timeOfDay < 20)) return;
	if (start) {
		gen_evl = 3;
		start = false;
		this->color1_has_reach = false;
		// generate new color
		this->color = getRandomColor();
		this->end_color = getRandomColor();
		this->end_color_2 = getRandomColor();
	}
	this->generate();
	for (std::deque<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
		it->draw(d);
	}
}

glm::vec3 rand_snow_speed(float scale) {
	 
	glm::vec3 v(0.0f, -1.0f * scale , 0.0f);
	//v = glm::rotate(v, (float)rand() / RAND_MAX * 360, glm::vec3(1.0f, 0.0f, 0.0f));
	//v = glm::rotate(v, (float)rand() / RAND_MAX * 360, glm::vec3(0.0f, 1.0f, 0.0f));
	//v = glm::rotate(v, ((float)rand() / RAND_MAX * 10 + 10) * direction, glm::vec3(1.0f, 0.0f, 0.0f));
	// rotate to simulate wind direc
	return v;
}

glm::vec3 rand_snow_position(int width){
	float pos1 = ((float)rand() / RAND_MAX  -0.5) *width;
	float pos2 = ((float)rand() / RAND_MAX  -0.5) *width;
	glm::vec3 v(pos1, 0.0f, pos2);

	return v;

}
Snow::Snow(Color _color,int _width) : GrObject("Snow"), color(_color),width(_width) {
	color.a = 1.0f;
	max_particle_number = 5000;
	generate_rate = 500;
	life = 15;
	evolution_rate = 30;
	gen_evl = 0;
	//width = 150;
	//height = 100;
	random_shift_intensity = 0.35;
	g = glm::vec3(0.0f, -1.0f, 0.0f);
	start = false;
	last_evolution_timer = clock();
	last_generate_timer = last_evolution_timer;
}


void Snow::generate() {
	clock_t currentTime = clock();
	// end life of old particles
	while (!particles.empty() && particles.front().birth + particles.front().life * CLOCKS_PER_SEC <= currentTime) {
		particles.pop_front();
	}

	double t = (double)(currentTime - last_generate_timer) / CLOCKS_PER_SEC;
	for (int gen = (int)(t * generate_rate); gen > 0 && particles.size() < max_particle_number; gen--) {
		particles.push_back(Particle(life, rand_snow_speed(1), color, rand_snow_position(this->width), 0.001f, (float)rand() / RAND_MAX * 0.15 + 0.12, (float)rand() / RAND_MAX * 360));
		last_generate_timer = currentTime;
	}

	currentTime = clock();
	t = (double)(currentTime - last_evolution_timer) / CLOCKS_PER_SEC;
	for (int evl = (int)(t * evolution_rate); evl > 0; evl--) {
		for (std::deque<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
			it->pos += it->speed;
			glm::vec3 a = it->weight * g;
			glm::vec3 random_v(((float)rand() / RAND_MAX - 0.5)* random_shift_intensity, 0.0f, ((float)rand() / RAND_MAX - 0.5)* random_shift_intensity); // add random shift
			a += random_v;
			a *= t;
			it->speed += a;
		}
		last_evolution_timer = currentTime;
	}
}


void Snow::draw(DrawingState *d) {
	if ((d->timeOfDay <= 4 || d->timeOfDay >= 20)) return;
	this->generate();
	for (std::deque<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
		it->draw(d);
	}
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
	char* skyTexureNight = "sky_night_0.png";
	char* skyTexureDusk = "sky_dawn_1.jpg";

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
	glPushMatrix();
	glScalef(10000, 10000, 10000);

	glDisable(GL_LIGHTING);
	attachTexture(d->timeOfDay);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(.25,.75);
	glVertex3f(-1, -1, -1);
	glTexCoord2f(.25,  .5);
	glVertex3f( 1, -1, -1);
	glTexCoord2f(.5,  .5);
	glVertex3f( 1,  1, -1);
	glTexCoord2f(.5, .75);
	glVertex3f(-1,1,-1);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(.25,  .5);
	glVertex3f(1, -1, -1);
	glTexCoord2f(.25, .25);
	glVertex3f(1, -1, 1);
	glTexCoord2f( .5, .25);
	glVertex3f(1, 1, 1);
	glTexCoord2f( .5,  .5);
	glVertex3f(1, 1, -1);

	glNormal3f(0, 0, -1);
	glTexCoord2f(.25,   0);
	glVertex3f(-1, -1, 1);
	glTexCoord2f( .5,   0);
	glVertex3f(-1,  1, 1);
	glTexCoord2f( .5, .25);
	glVertex3f( 1,  1, 1);
	glTexCoord2f(.25, .25);
	glVertex3f( 1, -1, 1);

	glNormal3f(1, 0, 0);
	glTexCoord2f(.25, .75);
	glVertex3f(-1, -1,  -1);
	glTexCoord2f( .5, .75);
	glVertex3f(-1, 1,  -1);
	glTexCoord2f( .5, 1);
	glVertex3f(-1,  1,  1);
	glTexCoord2f(.25, 1);
	glVertex3f(-1, -1,  1);

	glNormal3f(0, -1, 0);
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

/***********************************************************************/
//hot air balloon
HotAirBalloon::HotAirBalloon(float r1, float g1, float b1, float r2, float g2, float b2) : GrObject("Balloon")
{	
	ridable = 1;
	rideAbove = 75;
	stripColor1(r1,g1,b1);
	stripColor2(r2, g2, b2);
}

void HotAirBalloon::draw(DrawingState*) {
	glTranslatef(0, 70, 0);
	double radius = 20, height = 30;
	double verticalSliceHeight = 2;
	int horizontalSliceCount = 12;
	int verticalSliceCount1 = radius/verticalSliceHeight;
	int verticalSliceCount2 = height / verticalSliceHeight;
	GLint halfBasketWidth = 3;

	////body
	for (int i = 0; i <	horizontalSliceCount; i++) {
		double theta1 = 2 * pi / horizontalSliceCount*i;
		double theta2 = 2 * pi / horizontalSliceCount*(i + 1);
		if (i % 2 == 0) {
			glColor4fv(&stripColor1.r);
		}
		else {
			glColor4fv(&stripColor2.r);
		}
		for (int j = 0; j < verticalSliceCount1; j++) {
			double h = radius - verticalSliceHeight * j;
			double layerRadius1 = sqrt(radius * radius - h * h);
			h = radius - verticalSliceHeight * (j + 1);
			double layerRadius2 = sqrt(radius * radius - h * h);

			double y1 = -verticalSliceHeight*j;
			double y2 = -verticalSliceHeight*(j + 1);
			double pt1x = layerRadius1*cos(theta1);
			double pt1z = -layerRadius1*sin(theta1);
			double pt2x = layerRadius2*cos(theta1);
			double pt2z = -layerRadius2*sin(theta1);
			double pt3x = layerRadius2*cos(theta2);
			double pt3z = -layerRadius2*sin(theta2);
			double pt4x = layerRadius1*cos(theta2);
			double pt4z = -layerRadius1*sin(theta2);
			polygon(-4, pt1x, y1, pt1z, pt2x, y2, pt2z, pt3x, y2, pt3z, pt4x, y1, pt4z);
		}
		double y1 = -radius;
		double y2 = -radius-verticalSliceHeight;
		double pt1x = radius*cos(theta1);
		double pt1z = -radius*sin(theta1);
		double pt2x = radius*cos(theta1);
		double pt2z = -radius*sin(theta1);
		double pt3x = radius*cos(theta2);
		double pt3z = -radius*sin(theta2);
		double pt4x = radius*cos(theta2);
		double pt4z = -radius*sin(theta2);
		polygon(-4, pt1x, y1, pt1z, pt2x, y2, pt2z, pt3x, y2, pt3z, pt4x, y1, pt4z);
		for (int j = 0; j < verticalSliceCount2; j++) {
			double h = height - verticalSliceHeight * j;
			double layerRadius1 = sqrt(h/height)*radius;
			h = height - verticalSliceHeight * (j+1);
			double layerRadius2 = sqrt(h / height)*radius;

			double y1 = -verticalSliceHeight*(j + 1)-radius;
			double y2 = -verticalSliceHeight*(j + 2)-radius;
			double pt1x = layerRadius1*cos(theta1);
			double pt1z = -layerRadius1*sin(theta1);
			double pt2x = layerRadius2*cos(theta1);
			double pt2z = -layerRadius2*sin(theta1);
			double pt3x = layerRadius2*cos(theta2);
			double pt3z = -layerRadius2*sin(theta2);
			double pt4x = layerRadius1*cos(theta2);
			double pt4z = -layerRadius1*sin(theta2);
			polygon(-4, pt1x, y1, pt1z, pt2x, y2, pt2z, pt3x, y2, pt3z, pt4x, y1, pt4z);
		}
	}
	//threads
	glTranslatef(0, -radius - height, 0);
	GLUquadricObj *obj = gluNewQuadric();
	glColor3ub(255, 255, 255);
	glPushMatrix();
		glRotatef(90, 1, 0, 0);
		glTranslatef(-halfBasketWidth, -halfBasketWidth, 0);
		gluCylinder(obj, 0.2, 0.2, 10, 10, 20);
		glTranslatef(0, 2*halfBasketWidth, 0);
		gluCylinder(obj, 0.2, 0.2, 10, 10, 20);
		glTranslatef(2 * halfBasketWidth, 0, 0);
		gluCylinder(obj, 0.2, 0.2, 10, 10, 20);
		glTranslatef(0, -2*halfBasketWidth, 0);
		gluCylinder(obj, 0.2, 0.2, 10, 10, 20);
	glPopMatrix();

	////basket
	glTranslatef(0, - 10, 0);
	//front
	polygoni(-4, -halfBasketWidth, 0, halfBasketWidth, halfBasketWidth, 0, halfBasketWidth,
		halfBasketWidth, 3, halfBasketWidth, -halfBasketWidth, 3, halfBasketWidth);
	//right
	polygoni(-4, halfBasketWidth, 0, halfBasketWidth, halfBasketWidth, 0, -halfBasketWidth,
		halfBasketWidth, 3, -halfBasketWidth, halfBasketWidth, 3, halfBasketWidth);
	//left
	polygoni(-4, -halfBasketWidth, 0, -halfBasketWidth, -halfBasketWidth, 0, halfBasketWidth,
		-halfBasketWidth, 3, halfBasketWidth, -halfBasketWidth, 3, -halfBasketWidth);
	//back
	polygoni(-4, halfBasketWidth, 0, -halfBasketWidth, -halfBasketWidth, 0, -halfBasketWidth,
		-halfBasketWidth, 3, -halfBasketWidth, halfBasketWidth, 3, -halfBasketWidth);
	//bottom
	polygoni(4, -halfBasketWidth, 0, halfBasketWidth, halfBasketWidth, 0, halfBasketWidth,
		halfBasketWidth, 0, -halfBasketWidth, -halfBasketWidth, 0, -halfBasketWidth);
}

