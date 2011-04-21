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
#include "msBoxAnimation.h"
#include "msBoxGrid.h"
#include "msPalette.h"
#include "../ShaderProgram/msShaderPrograms.h"
#include "../ShaderProgram/msShaderProgram.h"
#include "../msParticleEmitter.h"
#include <list>

using namespace std;

typedef list<msParticleEmitter*> msExplosionList;
typedef msExplosionList::iterator msExplosionIterator;

class msBoxGridRenderer
{
	msShaderPrograms *m_shaders;

    msExplosionList m_explosions;

    msSize m_size;

	void drawBox(msShaderProgram *m_program, msPalette *palette, msBox *box, msColor *color);
	void _drawLine(msShaderProgram *m_program, msPoint &start, msPoint &end, msColor *color);
	void drawLeftBorder(msShaderProgram *m_program, msBox *box, msColor *color);
	void drawTopBorder(msShaderProgram *m_program, msBox *box, msColor *color);
	void drawRightBorder(msShaderProgram *m_program, msBox *box, msColor *color);
	void drawBottomBorder(msShaderProgram *m_program, msBox *box, msColor *color);

    void drawExplosions();

    msParticleEmitter* _createExplosionPe(msPoint location, msSize screenSize);

    void removeInactiveExplosions();

public:
	msBoxGridRenderer(msShaderPrograms *shaders);
    ~msBoxGridRenderer();

	void draw(msBoxGrid *boxGrid, msSize size);
};

