#include "msWaveEmitter.h"

#define RADIUS_MIN 0.0f
#define RADIUS_MAX 5.0f
#define RADIUS_STEP 0.025f
#define POWER_STEP RADIUS_STEP / (RADIUS_MIN - RADIUS_MAX)

msWaveEmitter::msWaveEmitter(msPoint3f location, msSizef size)
{
    this->radius = RADIUS_MIN;
    this->power = 1.0f;
    this->location = location;    
    this->size = size;
}


msWaveEmitter::~msWaveEmitter(void)
{
}

void msWaveEmitter::step()
{
    this->radius += RADIUS_STEP;    
    this->power += POWER_STEP;
}

GLboolean msWaveEmitter::isAlive()
{
    return this->radius < RADIUS_MAX;
}




