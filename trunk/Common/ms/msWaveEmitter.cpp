#include "msWaveEmitter.h"

#define RADIUS_MIN 0.0f
#define RADIUS_MAX 1.0f
#define RADIUS_STEP 0.025f;

msWaveEmitter::msWaveEmitter(msPoint location, msSize size)
{
	m_radius = RADIUS_MIN;
	m_power = 1.0f;
	m_location = location;	
	m_size = size;
}


msWaveEmitter::~msWaveEmitter(void)
{
}

void msWaveEmitter::render( msShaderProgram * program )
{
	program->getUniform("radius")->set1f(m_radius);
	program->getUniform("power")->set1f(m_power);
	program->getUniform("location")->set2f(m_location.x / m_size.width, m_location.y / m_size.height);
}

void msWaveEmitter::step()
{
	m_radius += RADIUS_STEP;
	m_power -= 0.005;
}

GLboolean msWaveEmitter::isAlive()
{
	return m_radius < RADIUS_MAX;
}




