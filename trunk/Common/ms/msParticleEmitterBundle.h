#pragma once

#include "msCommon.h"
#include "msParticleEmitter.h"


typedef list<msParticleEmitter*> msParticleEmitterList;
typedef msParticleEmitterList::iterator msParticleEmitterIterator;


class msParticleEmitterBundle
{
	// list of particle emitters
	msParticleEmitterList emitterList;

	// shared particle data
	msParticleData *particleData;

public:
	msParticleEmitterBundle(void);
	~msParticleEmitterBundle(void);

	void removeInactiveEmitters();

	void addParticleEmitter(msParticleEmitter *emitter);

	void update(float delta);

	msParticleData* getParticleData() const
	{
		return this->particleData;
	}

	int getCount()
	{
		return this->emitterList.size();
	}
};

