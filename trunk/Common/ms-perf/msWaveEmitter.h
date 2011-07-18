#pragma once

#include "ShaderProgram/msShaderProgram.h"

class msWaveEmitter
{
public:
	msWaveEmitter(msPointf location, msSizef size);
	~msWaveEmitter(void);
	//void render();
	void step();
	void render( msShaderProgram * program );
	float m_radius;
	float m_power;
	msPointf m_location;
	msSizef m_size;

	GLboolean isAlive();
};

