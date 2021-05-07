#pragma warning(disable : 4786)

#include "particleSystem.h"
#include "modelerdraw.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <FL/gl.h>

#include <ctime>


/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem(Vec3d gravG, double dragK)
{
	// TODO
	srand(time(0));
	forces.push_back(new GravityForce(gravG));
	forces.push_back(new DragForce(dragK));
}





/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
	particles.clear();
	forces.clear();
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	// TODO
	bake_start_time = t;

	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO
	bake_end_time = t;
	resetSimulation(t);

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{

	// TODO
	bake_fps = t - current_time;
	current_time = t;

	if (!isSimulate())
		return;
	if (!isBaked(t)) {
		for (int i = 0; i < particles.size(); i++)
			particles[i].calculatePos(bake_fps);
		bakeParticles(t);
	}
	else particles = bake_map[t];
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{

	// TODO
	if (!isSimulate())
		return;
	for (int i = 0; i < particles.size(); i++)
		particles[i].drawOneParticle();
}





/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{

	// TODO
	std::pair<float, std::vector<Particle>> pair;
	pair.first = t;
	pair.second = particles;
	bake_map.insert(pair);
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{

	// TODO
	bake_map.clear();
}


bool ParticleSystem::isBaked(float t)
{
	map<float, std::vector<Particle>>::iterator i = bake_map.find(t);
	return (i != bake_map.end());
}


void ParticleSystem::spawnParticles(Vec3d pos, int particle_count)
{
	if (!isSimulate() || isBaked(current_time + bake_fps))
		return;
	for (int i = 0; i < particle_count; ++i) {
		double m = rand() % 3 + 0.2;
		Particle p = Particle(pos, m);
		for (int j = 0; j < forces.size(); j++) {
			p.addNewForce(forces[j]);
		}
		Vec3d new_speed = Vec3d(-(rand() % 10 / 10.0), 10, 10);
		p.setSpeed(new_speed);
		particles.push_back(p);
	}
}
