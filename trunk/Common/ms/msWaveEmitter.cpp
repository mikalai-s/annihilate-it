#include "msWaveEmitter.h"

#define RADIUS_MIN 0.0f
#define RADIUS_MAX 5.0f
#define RADIUS_STEP 0.025f
#define POWER_STEP RADIUS_STEP / (RADIUS_MIN - RADIUS_MAX)

msWaveEmitter::msWaveEmitter(msPoint3f location, msSizef size)
{
    m_radius = RADIUS_MIN;
    m_power = 1.0f;
    m_location = location;    
    m_size = size;
}


msWaveEmitter::~msWaveEmitter(void)
{
}

void msWaveEmitter::step()
{
    m_radius += RADIUS_STEP;    
    m_power += POWER_STEP;
}

GLboolean msWaveEmitter::isAlive()
{
    return m_radius < RADIUS_MAX;
}




