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
	float size, angle, weight;
	float birth, life;
	Particle(float life,
		const glm::vec3 &speed,
		Color &color,
		const glm::vec3 &pos = glm::vec3(0.0, 0.0, 0.0),
		float weight = 0.7,
		float size = 0.2,
		float angle = 0
		);
	virtual void draw(DrawingState*);
};

class Fountain : public GrObject {
public:
	std::deque<Particle> particles,particles_layer2;
	int max_particles, gen_rate, life;
	int evl_rate;
	glm::vec3 g,g2;
	Color color,color_layer2;
	clock_t last_gen_clock, last_evl_clock;
	Fountain();

	virtual void draw(DrawingState*);
	void gen_particles();
	void evolute();
};

class Fireworks : public GrObject {
public:
	std::deque<Particle> particles;
	int max_particles, gen_rate, life;
	int evl_rate;
	int gen_evl;
	bool start, color1_has_reach,color2_has_reach;
	glm::vec3 g;
	Color color,end_color,end_color_2;
	clock_t last_gen_clock, last_evl_clock;
	Fireworks(Color color1,Color color2, Color color3);
	virtual void draw(DrawingState*);
	void gen_particles();
	void evolute();
};

class Snow : public GrObject{
public:
	std::deque<Particle> particles;
	int max_particles, gen_rate, life,evl_rate, gen_evl;
	bool start;
	int width;
	double random_shift_intensity;
	glm::vec3 g;
	Color color;
	clock_t last_gen_clock, last_evl_clock;
	Snow(Color color,int width);

	virtual void draw(DrawingState*);
	void gen_particles();
	void evolute();
};

class HotAirBalloon : public GrObject {
public:
	HotAirBalloon(float r1, float g1, float b1, float r2, float g2, float b2);
	Color stripColor1, stripColor2;
	virtual void draw(DrawingState*);
};


