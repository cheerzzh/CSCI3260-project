#pragma once

/// GraphicsTown2006 
// written by Michael   - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - new texture manager, improvements for behaviors

//
// A set of relatively simple objects to get the ideas across
// these are mainly here as examples - real objects should be more
// interesting 

#include "../GrObject.H"
#include "../DrawingState.H"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <deque>
#include "Utilities/Texture.H"


// sky
class Sky: public GrObject {
public:
	Texture* dayTexture;
	Texture* duskTexture;
	Texture* nightTexure;
public:
	virtual void draw(DrawingState * d);
	Sky();
	void attachTexture(int currentTime);

};

// mainly for debugging
class Cube : public GrObject {
public:
  Cube(float x, float y, float z, float s, float r=1, float g=.2, float b=.2);
  Color color;
  float size;
  virtual void draw(DrawingState*);
};


class TestCubes : public GrObject {
public:
  TestCubes();
  virtual void draw(DrawingState*);
};

// this object uses a shader!
class ShadedCube : public GrObject {
public:
  ShadedCube(float x, float y, float z, float s, float r=1, float g=.2, float b=.2);
  Color color;
  float size;
  virtual void draw(DrawingState*);
};

// particle system
class Particle : public GrObject {
public:
	glm::vec3 pos, speed;
	Color color;
	float size, angle, weight, birth, life;
	Particle(float life,const glm::vec3 &speed,Color &color,const glm::vec3 &pos,float weight,float size,float angle);
	virtual void draw(DrawingState*);
};

class Fountain : public GrObject {
public:
	std::deque<Particle> particles,particles_layer2;
	int max_particle_number, generate_rate, life,evolution_rate;
	glm::vec3 g,g2;
	Color color,color_layer2;
	clock_t last_generate_timer, last_evolution_timer;
	
	Fountain();
	void generate();
	virtual void draw(DrawingState*);
};

class Fireworks : public GrObject {
public:
	std::deque<Particle> particles;
	int max_particle_number, generate_rate, life, evolution_rate;
	int gen_evl;
	bool start, color1_has_reach,color2_has_reach;
	glm::vec3 g;
	Color color,end_color,end_color_2;
	clock_t last_generate_timer, last_evolution_timer;

	Fireworks(Color color1,Color color2, Color color3);
	void generate();
	virtual void draw(DrawingState*);
};

class Snow : public GrObject{
public:
	std::deque<Particle> particles;
	int max_particle_number, generate_rate, life,evolution_rate, gen_evl;
	bool start;
	int width;
	double random_shift_intensity;
	glm::vec3 g;
	Color color;
	clock_t last_generate_timer, last_evolution_timer;
	Snow(Color color,int width);

	virtual void draw(DrawingState*);
	void generate();
};

class HotAirBalloon : public GrObject {
public:
	HotAirBalloon(float r1, float g1, float b1, float r2, float g2, float b2);
	Color stripColor1, stripColor2;
	virtual void draw(DrawingState*);
};


