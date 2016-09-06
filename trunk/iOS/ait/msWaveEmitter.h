#pragma once

#include "ShaderProgram/msShaderProgram.h"

class msWaveEmitter
{
public:
    msWaveEmitter(msPoint3f location, msSizef size);
    ~msWaveEmitter(void);
    //void render();
    void step();
    float radius;
    float power;
    msPoint3f location;
    msSizef size;

    GLboolean isAlive();
};

