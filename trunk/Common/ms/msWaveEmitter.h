#pragma once

#include "ShaderProgram/msShaderProgram.h"

class msWaveEmitter
{
public:
    msWaveEmitter(msPoint3f location, msSizef size);
    ~msWaveEmitter(void);
    //void render();
    void step();
    void render( msShaderProgram * program );
    float m_radius;
    float m_power;
    msPoint3f m_location;
    msSizef m_size;

    GLboolean isAlive();
};

