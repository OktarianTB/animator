#ifndef FORCE_H
#define FORCE_H

#include "vec.h"

class Particle;

class Force {
public:
	virtual void applyForce(Particle* p) = 0;
};

class GravityForce: public Force {      // F = mg
public:
	GravityForce(Vec3d gravG) : g(gravG) {}
	void applyForce(Particle* p);       // p->f += p->m * F->g
	Vec3d g;
};

class DragForce : public Force {        // F = -kv
public:
	DragForce(double dragK) : k(dragK) {}
	void applyForce(Particle* p);       // p->f -= F->k * p->v
	double k;
};
#endif	// FORCE_H