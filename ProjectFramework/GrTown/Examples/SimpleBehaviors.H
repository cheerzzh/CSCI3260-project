#pragma once
/// GraphicsTown2006 
// written by Michael   - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - new texture manager, improvements for behaviors

// simple example behaviors - something to learn from

#include "../Behavior.H"

////////////////////////////////////////////////////////////////////////
// a simple behavior: spin around the center point
// velocity (roughly) is radians per timestep (millisecond)
//
// the blasts away the rotation part of the object's transform matrix,
// but doesn't effect the translation part
class Spin : public Behavior {
public:
  Spin(GrObject* owner, float vel = .005);
  
  float vel;					// we need to remember this

  virtual void simulateUntil(unsigned long);
};

////////////////////////////////////////////////////////////////////////
// these examples directly update the object's matrix
// - its a bit harder to do anything complicated this way
//
// always makes things go forward - you probably want to change its
// direction using another behavior
class ForwardAlways : public Behavior {
public:
  ForwardAlways(GrObject* owner, float vel = 10);
  float vel;
  virtual void simulateUntil(unsigned long);
};
class TurnAlways : public Behavior {
public:
  TurnAlways(GrObject* owner, float vel = .1);
  float vel;
  virtual void simulateUntil(unsigned long);
};

// emit firework
class emitFireWork :public Behavior{
public:
	emitFireWork(GrObject* owner, int phase = 0);
	int phase, last;
	virtual void simulateUntil(unsigned long);
};

// random float
class RandomFloat :public Behavior{
public:
	RandomFloat(GrObject* owner,int vel = 10);
	int vel;
	int dx, dz;  //control the direction of movement in x-z plane
	int vx, vz; //velocity in x, z, ensure constant speed in each 'section'
	unsigned long sectionStartTime;	
	virtual void simulateUntil(unsigned long);
};

