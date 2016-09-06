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
    int particleDataCount;

public:
    msParticleEmitterBundle(void);
    ~msParticleEmitterBundle(void);

    void removeInactiveEmitters();

    void addParticleEmitter(msParticleEmitterSettings &settings);

    void update(float delta);

    msParticleData* getParticleData();

    int getParticleCount();
};

