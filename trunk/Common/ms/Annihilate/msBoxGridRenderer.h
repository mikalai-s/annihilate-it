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




struct msShaderReferences
{
    struct BoxGrid
    {
        msShaderProgram *Program;
        
        struct Attribute
        {
            msAttribute *Position;
            msAttribute *BorderTexelLeft;
            msAttribute *BorderTexelBottom;
            msAttribute *BorderTexelRight;
            msAttribute *BorderTexelTop;
        } Attribute;
        
        struct Uniform
        {
            msUniform *BorderExternalLineTex;
            msUniform *BorderInternalLineTex;
            msUniform *BorderCornerTex;
            msUniform *Mvp;
            msUniform *LineBorder;
            msUniform *CornerBorder;
            msUniform *Color;
        } Uniform;
        
        struct Texture
        {
            msTexture *BorderExternalLineTex;
            msTexture *BorderInternalLineTex;
            msTexture *BorderCornerTex;
        } Texture;
        
        struct Framebuffer
        {
            msFrameBuffer *RenderTex;
        } Framebuffer;
        
    } BoxGrid;
    
    struct ShockWave
    {
        msShaderProgram *Program;
        
        struct Attribute
        {
            msAttribute *Position;
            msAttribute *TexCoord;
        } Attribute;
        
        struct Uniform
        {
            msUniform *Tex;
            msUniform *Radius;
            msUniform *Power;
            msUniform *Location;
        } Uniform;
        
        struct Texture
        {
        } Texture;
        
        struct Framebuffer
        {
        } Framebuffer;        

    } ShockWave;
    
    struct ParticleComplete
    {
        msShaderProgram *Program;
        
        struct Attribute
        {
            msAttribute *A2Position;
            msAttribute *A2TexCoord;
        } Attribute;
        
        struct Uniform
        {
            msUniform *U2Texture;
        } Uniform;
        
        struct Texture
        {
        } Texture;
        
        struct Framebuffer
        {

        } Framebuffer;        

    } ParticleComplete;
    
    
    struct ParticleCreate
    {
        msShaderProgram *Program;
        
        struct Attribute
        {
            msAttribute *APosition;
            msAttribute *AColor;
            msAttribute *ASize;
            
        } Attribute;
        
        struct Uniform
        {
            msUniform *UTexture;
        } Uniform;
        
        struct Texture
        {
            msTexture *UTexture;
        } Texture;
        
        struct Framebuffer
        {
            msFrameBuffer *RenderTex;
        } Framebuffer;        

    } ParticleCreate;
    
};


class msBoxGridRenderer
{
private:
    
    GLuint boxGridVertexArrayObject, shockWaveVertexArrayObject, particleCompleteVertexArrayObject, particleCreateVertexArrayObject;
    
    msShaderPrograms *m_shaders;
    
    msShaderReferences sr;

    msParticleEmitterBundle explosionsBundle;

    msWaveList m_waves;

    msSizef m_size;

    msMatrix m_projectionMatrix;
    
    msBoxGrid *m_boxGrid;
    
    GLuint particleBuffer;
    GLuint m_indexBuffer;
    GLuint m_vertexBuffer;
    GLuint m_textureOrientationBuffer;
    GLuint m_textureOrientationBuffer2;
    GLuint m_positionBuffer;
    int particleBufferSize;
private:
    void _drawBoxGrid(msSizef size);
    void _drawExplosions();
    void _drawBox(msBox *box);
    void _drawFace(msBoxFaceData *faceData);
    void _drawBoxesWithShockWave();
    void _drawBoxGrid();
    void _drawParticles(msParticleEmitterBundle &particleEmitters);

    msParticleEmitterSettings _createExplosionSettings(msPoint3f location, GLfloat ratio);
    msWaveEmitter* _createWave(msBox *box);

    void _removeInactiveEmitters();  
    void prepareShaderReferences();

public:
    msBoxGridRenderer(msShaderPrograms *shaders, msBoxGrid *boxGrid);
    ~msBoxGridRenderer();

    void draw(msSizef size);
    
    void setBoxGrid(msBoxGrid *boxgrid);
    
};

