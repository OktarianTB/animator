#include "force.h"
#include "particle.h"

void GravityForce::applyForce(Particle* p) {
	Vec3d init_force = p->getForce();
	Vec3d new_force = init_force + p->getMass() * g;
	p->setForce(new_force);
}

void DragForce::applyForce(Particle * p) {
	Vec3d init_force = p->getForce();
	Vec3d new_force = init_force - k * p->getSpeed();
	p->setForce(new_force);
}