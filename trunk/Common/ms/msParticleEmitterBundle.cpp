#include "msParticleEmitterBundle.h"


msParticleEmitterBundle::msParticleEmitterBundle(void)
{
}


msParticleEmitterBundle::~msParticleEmitterBundle(void)
{
    for(msParticleEmitterIterator i = this->emitterList.begin(); i != this->emitterList.end(); i ++)
         delete (*i);
}

void msParticleEmitterBundle::removeInactiveEmitters()
{
	msParticleEmitterList emittersToDelete;

    for(msParticleEmitterIterator ei = this->emitterList.begin(); ei != this->emitterList.end(); ei ++)
        if(!(*ei)->isAlive())
            emittersToDelete.push_back(*ei);

    for(msParticleEmitterIterator ei = emittersToDelete.begin(); ei != emittersToDelete.end(); ei ++)
    {
        delete (*ei);
        this->emitterList.remove(*ei);
    }
}

void msParticleEmitterBundle::addParticleEmitter(msParticleEmitter *emitter)
{
	//emitter->particleData

	emitterList.push_back(emitter);
}

void msParticleEmitterBundle::update(float delta)
{
	for(msParticleEmitterIterator ei = this->emitterList.begin(); ei != this->emitterList.end(); ei ++)
		(*ei)->update(delta);
}
