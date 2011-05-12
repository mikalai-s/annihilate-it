#pragma once

#include "ShaderProgram/msShaderProgram.h"

class msWaveEmitter
{
public:
	msWaveEmitter(msPoint location, msSize size);
	~msWaveEmitter(void);
	//void render();
	void step();
	void render( msShaderProgram * program );
	float m_radius;
	float m_power;
	msPoint m_location;
	msSize m_size;

	GLboolean isAlive();
};

