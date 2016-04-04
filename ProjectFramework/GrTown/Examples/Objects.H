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

// snowman
class SnowMan : public GrObject{
public:
	SnowMan(float x, float y, float z, float size);
	float scale;
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
	std::deque<Particle> particles;
	int max_particles, gen_rate, life;
	int evl_rate;
	glm::vec3 g;
	Color color;
	clock_t last_gen_clock, last_evl_clock;
	Fountain();
	virtual void draw(DrawingState*);
	void gen_particles();
	void evolute();
};


