 // SAMPLE_SOLUTION

#include "particle.h"
#include "force.h"
#include "modelerdraw.h"

#include <FL/gl.h>
#include <FL/glut.h>
#include <GL/glu.h>
#include <cstdio>
#include <math.h>

void Particle::drawOneParticle() {
	setDiffuseColor(0.5, 0.5, 0.5);
	glPushMatrix();
		glPointSize(5);
		glBegin(GL_POINTS);
		glVertex3f(pos[0], pos[1], pos[2]);
	glEnd();
	glPopMatrix();
	glPointSize(1);
}

void Particle::calculatePos(double dt) {
	for (int i = 0; i < appliedForces.size(); i++) {
		appliedForces[i]->applyForce(this);
	}

	Vec3d acceleration = force / mass;
	speed += acceleration * dt;
	pos += speed * dt;
}