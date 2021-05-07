// SAMPLE_SOLUTION

#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec.h"

#include <vector>

class Force;

class Particle {
public:
	// Constructor
	Particle(Vec3d pos, double mass) : pos(pos), mass(mass) {}
	
	// Getters
	Vec3d getPos() const { return pos; }
	Vec3d getSpeed() const { return speed; }
	Vec3d getForce() const { return force; }
	double getMass() const { return mass; }

	// Setters
	void setPos(Vec3d p) { pos = p; }
	void setSpeed(Vec3d s) { speed = s; }
	void setForce(Vec3d f) { force = f; }

	void drawOneParticle();

	void calculatePos(double dt);
	void addNewForce(Force* force) { appliedForces.push_back(force); }

private:
	Vec3d pos;
	Vec3d speed;
	Vec3d force;
	double mass;

	std::vector<Force*> appliedForces;
};

#endif	// PARTICLE_H