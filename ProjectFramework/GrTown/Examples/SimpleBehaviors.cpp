// written by Michael   - originally written Fall, 2000
// updated Fall, 2005
// updated Fall, 2006 - new texture manager, improvements for behaviors

// simple example behaviors - something to learn from
#include "../GrTown_PCH.H"
#include "../DrawUtils.H"
#include "Utilities/Texture.H"
#include "../MMatrix.H"
#include "../GrObject.H"

#include "SimpleBehaviors.H"
#include "Objects.H"

//////////////////////////////////////////////////////////////////////
// a trick - since we don't care much about what the rotation is (only
// the rate), we can tie the rotation to the absolute time
Spin::Spin(GrObject* o, float v) : Behavior(o), vel(v)
{
}
void Spin::simulateUntil(unsigned long t)
{
  // remember position
  float x = owner->transform[3][0];
  float y = owner->transform[3][1];
  float z = owner->transform[3][2];
  // set rotation based on time
  rotMatrix(owner->transform, 'y', vel * static_cast<float>(t));
  // put the position back
  owner->transform[3][0] = x;
  owner->transform[3][1] = y;
  owner->transform[3][2] = z;

  lastV = t;
}

////////////////////////////////////////////////////////////////////////
ForwardAlways::ForwardAlways(GrObject* o, float v)
  : Behavior(o), vel(v)
{
}
void ForwardAlways::simulateUntil(unsigned long t)
{
  unsigned long dt = t - lastV;	// how long since last update
  float secs = ((float) dt) / 1000;	// convert ms to sec
  lastV = t;
  owner->transform[3][0] += owner->transform[2][0] * secs * vel;
  owner->transform[3][1] += owner->transform[2][1] * secs * vel;
  owner->transform[3][2] += owner->transform[2][2] * secs * vel;

  lastV = t;
}

////////////////////////////////////////////////////////////////////////
TurnAlways::TurnAlways(GrObject* o, float v)
  : Behavior(o), vel(v)
{
}
void TurnAlways::simulateUntil(unsigned long t)
{
  unsigned long dt = t - lastV;	// how long since last update
  float secs = ((float) dt) / 1000;	// convert ms to sec
  lastV = t;
  Matrix delta, newT;
  rotMatrix(delta,'Y',secs * vel);
  multMatrix(delta,owner->transform,newT);
  copyMatrix(newT,owner->transform);

  lastV = t;
}

// emit firework
emitFireWork::emitFireWork(GrObject* o, int p)
	: Behavior(o), phase(p)
{
	last = lastV;
}

void emitFireWork::simulateUntil(unsigned long t)
{
	if ((t + phase - last) > 5000) {
		((Fireworks *)owner)->start = true;
		last = t + phase;
	}
	lastV = t;
}

RandomFloat::RandomFloat(GrObject* o, int v) : Behavior(o), vel(v)
{
	dx = 1;
	dz = 1;
}
void RandomFloat::simulateUntil(unsigned long t)
{
	//original position
	float x = owner->transform[3][0];
	float y = owner->transform[3][1];
	float z = owner->transform[3][2];
	if (x > 1000) dx = -1;
	if (x < 0) dx = 1;
	if (z > 700) dz = -1;
	if (z < 0) dz = 1;

	unsigned long dt = t - lastV;	// how long since last update
	float secs = ((float)dt) / 1000;	// convert ms to sec
	//translation
	float tx = secs*(rand() % vel)*dx; 
	float tz = secs*(rand() % vel)*dz;
	//new position
	owner->transform[3][0] = x+tx;
	owner->transform[3][2] = z+tz;

	lastV = t;
}
