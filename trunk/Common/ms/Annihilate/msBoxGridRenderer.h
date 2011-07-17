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
#include "../msWaveEmitter.h"
#include "msSpline.h"
#include <list>
#include "../msMatrix.h"

using namespace std;

typedef list<msParticleEmitter*> msExplosionList;
typedef msExplosionList::iterator msExplosionIterator;

typedef list<msWaveEmitter*> msWaveList;
typedef msWaveList::iterator msWaveIterator;

class msBoxGridRenderer
{
	msShaderPrograms *m_shaders;

    msExplosionList m_explosions;

	msWaveList m_waves;

    msSizef m_size;

    msMatrix m_projectionMatrix;
    
    msBoxGrid *m_boxGrid;
    
    GLuint m_indexBuffer;
    GLuint m_vertexBuffer;
    GLuint m_textureOrientationBuffer;
    GLuint m_positionBuffer;

    

	void _drawFigure(msBoxList &boxes, msBox *box, msPalette *palette, msShaderProgram *m_program);
	void _drawLine(msShaderProgram *m_program, msPointf &start, msPointf &end, msColor *color);
	void drawLeftBorder(msShaderProgram *m_program, msBox *box, msColor *color);
	void drawTopBorder(msShaderProgram *m_program, msBox *box, msColor *color);
	void drawRightBorder(msShaderProgram *m_program, msBox *box, msColor *color);
	void drawBottomBorder(msShaderProgram *m_program, msBox *box, msColor *color);

    void drawBoxGrid(msShaderProgram *program, msSizef size);

    void drawExplosions();

	void drawBox(msShaderProgram *m_program, msPalette *palette, msBox *box, msBoxFaceData *faceData, bool front);


    void drawBoxesWithShockWave();

    void drawBoxGrid();

    msParticleEmitter* _createExplosionPe(msPointf location, msSizef screenSize);

	msWaveEmitter* _createWave(msBox *box);

    void removeInactiveEmitters();

    void showExplosions();
    
	GLboolean m_animate;

    GLint c;

	void _getBoxSplinePoints( msBoxList & boxes, msBox * box, msSpline &spl, char direction);

	// % part of screen that is used for menu bar
	float _menuBarHeight;

public:
	msBoxGridRenderer(msShaderPrograms *shaders, msBoxGrid *boxGrid);
    ~msBoxGridRenderer();

    void draw(msSizef size);
};

