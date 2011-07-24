/*
 *  MsOpenGL.h
 *  AnnihilateIt
 *
 *  Created by user on 11-03-05.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "../msGL.h"
#include "../msCommon.h"
#include "msBox.h"
#include "msBoxGrid.h"
#include "msPalette.h"
#include "../ShaderProgram/msShaderPrograms.h"
#include "../ShaderProgram/msShaderProgram.h"
#include "../msParticleEmitter.h"
#include "../msParticleEmitterBundle.h"
#include "../msWaveEmitter.h"
#include "msSpline.h"
#include "../msMatrix.h"


typedef list<msWaveEmitter*> msWaveList;
typedef msWaveList::iterator msWaveIterator;

class msBoxGridRenderer
{
private:
    msShaderPrograms *m_shaders;

    msParticleEmitterBundle explosionsBundle;

    msWaveList m_waves;

    msSizef m_size;

    msMatrix m_projectionMatrix;
    
    msBoxGrid *m_boxGrid;
    
    GLuint particleBuffer;
    GLuint m_indexBuffer;
    GLuint m_vertexBuffer;
    GLuint m_textureOrientationBuffer;
    GLuint m_positionBuffer;
    int particleBufferSize;
private:
    void _drawBoxGrid(msShaderProgram *program, msSizef size);
    void _drawExplosions();
    void _drawBox(msShaderProgram *m_program, msBox *box);
    void _drawFace(msShaderProgram *m_program, msBoxFaceData *faceData);
    void _drawBoxesWithShockWave();
    void _drawBoxGrid();
    void _drawParticles(msParticleEmitterBundle &particleEmitters, msShaderProgram &particleProgram);

    msParticleEmitterSettings _createExplosionSettings(msPoint3f location, GLfloat ratio);
    msWaveEmitter* _createWave(msBox *box);

    void _removeInactiveEmitters();    

public:
    msBoxGridRenderer(msShaderPrograms *shaders, msBoxGrid *boxGrid);
    ~msBoxGridRenderer();

    void draw(msSizef size);
};

