#include "msParticleEmitterBundle.h"


msParticleEmitterBundle::msParticleEmitterBundle(void)
{
    this->particleDataCount = 0;
}


msParticleEmitterBundle::~msParticleEmitterBundle(void)
{
    for(msParticleEmitterIterator i = this->emitterList.begin(); i != this->emitterList.end(); i ++)
         delete (*i);

    if(this->particleDataCount > 0)
        free(this->particleData);
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

void msParticleEmitterBundle::addParticleEmitter(msParticleEmitterSettings &settings)
{
    this->emitterList.push_back(new msParticleEmitter(settings));
}

void msParticleEmitterBundle::update(float delta)
{
    for(msParticleEmitterIterator ei = this->emitterList.begin(); ei != this->emitterList.end(); ei ++)
        (*ei)->update(delta);
}

msParticleData* msParticleEmitterBundle::getParticleData()
{
    int sizeOfMsParticleData = sizeof(msParticleData);
    // prepare memory block for particle data
    int realCount = getParticleCount();
    if(realCount > this->particleDataCount || this->particleDataCount == 0)
    {
        if(this->particleDataCount > 0)
            free(this->particleData);

        this->particleData = (msParticleData*)malloc(sizeOfMsParticleData * realCount);
        this->particleDataCount = realCount;
    }

    // copy memory
    unsigned long offset = 0;
    for(msParticleEmitterIterator i = this->emitterList.begin(); i != this->emitterList.end(); i ++)
    {
        msParticleEmitter *emitter = *i;
        unsigned long size = emitter->particleCount * sizeOfMsParticleData;
        memcpy(this->particleData + offset, emitter->particleData, size);
        offset += emitter->particleCount; // note: offset is not in bytes but in number of msParticleData structures
    }

    return this->particleData;
}

int msParticleEmitterBundle::getParticleCount()
{
    int count = 0;
    for(msParticleEmitterIterator i = this->emitterList.begin(); i != this->emitterList.end(); i ++)
        count += (*i)->particleCount;
    return count;
}
