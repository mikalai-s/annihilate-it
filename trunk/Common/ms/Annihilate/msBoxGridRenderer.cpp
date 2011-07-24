/*
 *  MsOpenGL.c
 *  AnnihilateIt
 *
 *  Created by user on 11-03-05.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "msBoxGridRenderer.h"
#include "msSpline.h"
#include "../msMatrix.h"
#include "../msMatrixTransform.h"


msBoxGridRenderer::msBoxGridRenderer(msShaderPrograms *shaders, msBoxGrid *boxGrid)
{
    m_shaders = shaders;

    // calculate projection matrix
    float fovy = 45.0f / 180.0f * 3.1415926f;
    float z = -0.5f / tanf(fovy / 2.0f);

    msMatrixTransform transform;
    transform.translate(-0.5f, -0.5f, z)
        ->perspective(fovy, 1.0f, -1.0f, 1.0f)
        ->viewport(2.0f, 2.0f)          
        ->translate(-1.0f, -1.0f, 0.0f);
    m_projectionMatrix = *transform.getMatrix();
    
    m_boxGrid = boxGrid;

    // buffer for particles array
    glGenBuffers(1, &this->particleBuffer);
    this->particleBufferSize = 0;
    
    // buffer for vertices data 
    glGenBuffers(1, &m_vertexBuffer);
    
    // buffer for indicies
    GLushort boxIndicies[] = { 0, 1, 2, 3 };    
    glGenBuffers(1, &m_indexBuffer);
    // NOTE: there is only one element array buffer, so we bind it only once
    // be carefull if you created another one
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 4, boxIndicies, GL_STATIC_DRAW);
    
    
    // buffer for texture texcoords
    GLfloat textureOrientation[] = {
        // left
        0.f, 0.f,
        1.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,
        // bottom
        1.f, 0.f,
        1.f, 1.f,
        0.f, 0.f,
        0.f, 1.f,
        // right
        1.f, 1.f,
        0.f, 1.f,
        1.f, 0.f,
        0.f, 0.f,
        // top
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f, 
        1.f, 0.f,
    };    
    glGenBuffers(1, &m_textureOrientationBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_textureOrientationBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 32, textureOrientation, GL_STATIC_DRAW);

    
    // buffer for position attribute
    static const GLfloat vertexPositions[] = {
        -1.f, -1.f, 1.f, 1.f,
        1.f, -1.f, 1.f, 1.f,
        -1.f,  1.f, 1.f, 1.f,
        1.f,  1.f, 1.f, 1.f,
    };
    glGenBuffers(1, &m_positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, vertexPositions, GL_STATIC_DRAW);
    
    
    glViewport(0, 0, m_size.width, m_size.height);
    
    prepareShaderReferences();
    
    
    // provide some static values
    this->sr.BoxGrid.Program->use();
    this->sr.BoxGrid.Attribute.Position->enableVertexArray();
    this->sr.BoxGrid.Attribute.BorderTexelLeft->enableVertexArray();
    this->sr.BoxGrid.Attribute.BorderTexelBottom->enableVertexArray();
    this->sr.BoxGrid.Attribute.BorderTexelRight->enableVertexArray();
    this->sr.BoxGrid.Attribute.BorderTexelTop->enableVertexArray();
    this->sr.BoxGrid.Uniform.BorderExternalLineTex->set1i(this->sr.BoxGrid.Texture.BorderExternalLineTex->getUnit());
    this->sr.BoxGrid.Uniform.BorderInternalLineTex->set1i(this->sr.BoxGrid.Texture.BorderInternalLineTex->getUnit());
    this->sr.BoxGrid.Uniform.BorderCornerTex->set1i(this->sr.BoxGrid.Texture.BorderCornerTex->getUnit());

    this->sr.ShockWave.Program->use();
    this->sr.ShockWave.Attribute.Position->enableVertexArray();
    this->sr.ShockWave.Attribute.TexCoord->enableVertexArray();
    this->sr.ShockWave.Uniform.Tex->set1i(this->sr.BoxGrid.Framebuffer.RenderTex->getTexture()->getUnit());    

    this->sr.ParticleComplete.Program->use();
    this->sr.ParticleComplete.Attribute.A2Position->enableVertexArray();
    this->sr.ParticleComplete.Attribute.A2TexCoord->enableVertexArray();
    this->sr.ParticleComplete.Uniform.U2Texture->set1i(this->sr.ParticleCreate.Framebuffer.RenderTex->getTexture()->getUnit());

    this->sr.ParticleCreate.Program->use();
    this->sr.ParticleCreate.Attribute.APosition->enableVertexArray();
    this->sr.ParticleCreate.Attribute.AColor->enableVertexArray();
    this->sr.ParticleCreate.Attribute.ASize->enableVertexArray();
    this->sr.ParticleCreate.Uniform.UTexture->set1i(this->sr.ParticleCreate.Texture.UTexture->getUnit());
}

void msBoxGridRenderer::setBoxGrid(msBoxGrid *boxGrid)
{
    m_boxGrid = boxGrid;
    
    //recreate buffer for vertices because box grid has changed
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    unsigned long size = sizeof(msBoxData) * boxGrid->getRowCount() * boxGrid->getColCount();
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
    
}

void msBoxGridRenderer::prepareShaderReferences()
{
    msShaderProgram *program = m_shaders->getProgramByName("boxgrid");
    this->sr.BoxGrid.Program = program;
    this->sr.BoxGrid.Attribute.Position = program->getAttribute("position");
    this->sr.BoxGrid.Attribute.BorderTexelLeft = program->getAttribute("borderTexelLeft");
    this->sr.BoxGrid.Attribute.BorderTexelBottom = program->getAttribute("borderTexelBottom");
    this->sr.BoxGrid.Attribute.BorderTexelRight = program->getAttribute("borderTexelRight");
    this->sr.BoxGrid.Attribute.BorderTexelTop = program->getAttribute("borderTexelTop");
    this->sr.BoxGrid.Uniform.BorderExternalLineTex = program->getUniform("borderExternalLineTex");    
    this->sr.BoxGrid.Uniform.BorderInternalLineTex = program->getUniform("borderInternalLineTex");    
    this->sr.BoxGrid.Uniform.BorderCornerTex = program->getUniform("borderCornerTex");
    this->sr.BoxGrid.Uniform.Mvp = program->getUniform("mvp");
    this->sr.BoxGrid.Uniform.LineBorder = program->getUniform("lineBorder");
    this->sr.BoxGrid.Uniform.CornerBorder = program->getUniform("cornerBorder");
    this->sr.BoxGrid.Uniform.Color = program->getUniform("color");
    this->sr.BoxGrid.Texture.BorderExternalLineTex = program->getTexture("borderExternalLineTex");    
    this->sr.BoxGrid.Texture.BorderInternalLineTex = program->getTexture("borderInternalLineTex");    
    this->sr.BoxGrid.Texture.BorderCornerTex = program->getTexture("borderCornerTex");
    this->sr.BoxGrid.Framebuffer.RenderTex = program->getFrameBuffer("renderTex");
    
    program = m_shaders->getProgramByName("shockwave");
    this->sr.ShockWave.Program = program;
    this->sr.ShockWave.Attribute.Position = program->getAttribute("position");
    this->sr.ShockWave.Attribute.TexCoord = program->getAttribute("texcoord");
    this->sr.ShockWave.Uniform.Tex = program->getUniform("tex");
    this->sr.ShockWave.Uniform.Radius = program->getUniform("radius");
    this->sr.ShockWave.Uniform.Power = program->getUniform("power");
    this->sr.ShockWave.Uniform.Location = program->getUniform("location");
   
    program = m_shaders->getProgramByName("particle_complete");
    this->sr.ParticleComplete.Program = program;
    this->sr.ParticleComplete.Attribute.A2Position = program->getAttribute("a2_position");
    this->sr.ParticleComplete.Attribute.A2TexCoord = program->getAttribute("a2_texcoord");
    this->sr.ParticleComplete.Uniform.U2Texture = program->getUniform("u2_texture");
    
    program = m_shaders->getProgramByName("particle_create");
    this->sr.ParticleCreate.Program = program;
    this->sr.ParticleCreate.Attribute.APosition = program->getAttribute("a_position");
    this->sr.ParticleCreate.Attribute.AColor = program->getAttribute("a_color");
    this->sr.ParticleCreate.Attribute.ASize = program->getAttribute("a_size");
    this->sr.ParticleCreate.Uniform.UTexture = program->getUniform("u_texture");
    this->sr.ParticleCreate.Texture.UTexture = program->getTexture("u_texture");
    this->sr.ParticleCreate.Framebuffer.RenderTex = program->getFrameBuffer("renderTex");
}

msBoxGridRenderer::~msBoxGridRenderer()
{
    for(msWaveIterator wi = m_waves.begin(); wi != m_waves.end(); wi ++)
    {
        delete (*wi);
    }
    
    glDeleteBuffers(1, &this->particleBuffer);
    glDeleteBuffers(1, &m_indexBuffer);
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_textureOrientationBuffer);
    glDeleteBuffers(1, &m_positionBuffer);
}

void msBoxGridRenderer::draw(msSizef size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(msBoxData) * m_boxGrid->getRowCount() * m_boxGrid->getColCount(), m_boxGrid->m_boxVertexData);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    
    m_size = size;
    
    if(m_boxGrid == 0)
        return;
    
    _drawBoxesWithShockWave();
    
    _drawExplosions();
    
    _removeInactiveEmitters();
    
    // update all animations
    for(int y = 0; y < m_boxGrid->m_rowCount; y ++)
    {
        for(int x = 0; x < m_boxGrid->m_columnCount; x ++)
        {
            msBox *box = m_boxGrid->getItem(y, x);
            box->getAnimations()->performStep();
        }
    }
    
    m_boxGrid->getAnimations()->performStep();
}

void msBoxGridRenderer::_drawBoxGrid(msSizef size)
{    
    for(int y = 0; y < m_boxGrid->m_rowCount; y ++)
    {
        for(int x = 0; x < m_boxGrid->m_columnCount; x ++)
        {
            msBox *box = m_boxGrid->getItem(y, x);
            
            // first check for explosion and if box is required one put it into list to be used after grid rendering
            if(box->getRequiresExplosion())
            {
                GLfloat ratio = m_boxGrid->getRowCount() / size.height / 2.0f + m_boxGrid->getColCount() / size.width / 2.0f;
                msParticleEmitterSettings settings = _createExplosionSettings(box->getExplosionPoint(), ratio);
                explosionsBundle.addParticleEmitter(settings);
            }
            
            if(box->getRequiresWave())
            {
                m_waves.push_back(_createWave(box));
            }
            
            if(box->isVisible())
            {
                // set common data for both next _drawBox calls
                _drawBox(box);
            }
        }
    }
    
}

void msBoxGridRenderer::_drawBox(msBox *box)
{
    msMatrixTransform transform;
    float boxAngle = box->getAngle();
    if(boxAngle != 0.0f)
    {
        msPoint3f angleVector = box->getAngleVector();
        msPoint3f center = box->getVerticesData()->getCenter();
        transform.translate(-center.x, -center.y, -center.z)
        ->rotate(boxAngle, angleVector)
        ->translate(center.x, center.y, center.z);
    }
    
    transform.multiplyMatrix(m_projectionMatrix);
    
    this->sr.BoxGrid.Uniform.Mvp->setMatrix4fv(1, false, transform.getMatrix()->getArray());
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);    
    unsigned long offset = (unsigned long)box->getVerticesData() - (unsigned long)m_boxGrid->m_boxVertexData;
    this->sr.BoxGrid.Attribute.Position->setPointer(3, GL_FLOAT, GL_FALSE, sizeof(msPoint3f), (void*)offset);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_textureOrientationBuffer);    
    this->sr.BoxGrid.Attribute.BorderTexelLeft->setPointer(2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);
    this->sr.BoxGrid.Attribute.BorderTexelBottom->setPointer(2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)(sizeof(GLfloat) * 8));
    this->sr.BoxGrid.Attribute.BorderTexelRight->setPointer(2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)(sizeof(GLfloat) * 16));
    this->sr.BoxGrid.Attribute.BorderTexelTop->setPointer(2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)(sizeof(GLfloat) * 24));
    
    glCullFace(GL_FRONT);
    _drawFace(&box->getVerticesData()->frontFace);
    
    glCullFace(GL_BACK);
    _drawFace(&box->getVerticesData()->backFace);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void msBoxGridRenderer::_drawFace(msBoxFaceData *faceData)
{        
    this->sr.BoxGrid.Uniform.LineBorder->set4iv(1, faceData->getHasBorder());
    this->sr.BoxGrid.Uniform.CornerBorder->set4iv(1, faceData->getHasCornerBorder());

    msColor faceColor = *m_boxGrid->m_palette->getColor(faceData->getColorIndex());
    faceColor.r *= faceData->getColorDisturbance().r;
    faceColor.g *= faceData->getColorDisturbance().g;
    faceColor.b *= faceData->getColorDisturbance().b;
    faceColor.a *= faceData->getColorDisturbance().a;
    this->sr.BoxGrid.Uniform.Color->set4fv(1, (GLfloat*)&faceColor);

    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);       

    glDisable(GL_BLEND);    
    glDisable(GL_CULL_FACE);    
}


void msBoxGridRenderer::_removeInactiveEmitters()
{
    this->explosionsBundle.removeInactiveEmitters();

    msWaveList wavesToDelete;

    for(msWaveIterator wi = m_waves.begin(); wi != m_waves.end(); wi ++)
        if(!(*wi)->isAlive())
            wavesToDelete.push_back(*wi);

    for(msWaveIterator wi = wavesToDelete.begin(); wi != wavesToDelete.end(); wi ++)
    {
        delete (*wi);
        m_waves.remove(*wi);
    }
}



msParticleEmitterSettings msBoxGridRenderer::_createExplosionSettings(msPoint3f location, GLfloat ratio)
{
    float k = 0.03333333f / ratio;

    msParticleEmitterSettings settings;
    settings.position                   = msPoint2f(location.x, location.y);
    settings.sourcePositionVariance     = msPoint2f(0.031f, 0.031f);
    settings.speed                      = 0.0001f;
    settings.speedVariance              = 0.007f;
    settings.particleLifeSpan           = 0.5f;
    settings.particleLifeSpanVariance   = 0.25f;
    settings.angle                      = 0.0f;
    settings.angleVariance              = 360.0f;
    settings.gravity                    = msPoint2f(0.0f, -0.0000025f);
    settings.startColor                 = msColor(1.0f, 0.5f, 0.05f, 1.0f);
    settings.startColorVariance         = msColor(0.0f, 0.0f, 0.0f, 0.5f);
    settings.finishColor                = msColor(0.2f, 0.0f, 0.0f, 0.0f);
    settings.finishColorVariance        = msColor(0.2f, 0.0f, 0.0f, 0.0f);
    settings.maxParticles               = 200;
    settings.particleSize               = 50 * k;
    settings.particleSizeVariance       = 3 * k;
    settings.duration                   = 0.125f;
    settings.blendAdditive              = GL_TRUE;
    return settings;
}



void msBoxGridRenderer::_drawBoxesWithShockWave()
{
    // render fire into texture using particle shaders
    this->sr.BoxGrid.Program->use();

    // Switch the render target to the current FBO to update the texture map
    this->sr.BoxGrid.Framebuffer.RenderTex->bind();

    // FBO attachment is complete?
    if (this->sr.BoxGrid.Framebuffer.RenderTex->isComplete())
    {
        // Set viewport to size of texture map and erase previous image
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT );

        // render background
        _drawBoxGrid(m_size);
    }

    // Unbind the FBO so rendering will return to the backbuffer.
    m_shaders->getMainFrameBuffer()->bind();

    // usual renderer

    this->sr.ShockWave.Program->use();
    
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);    
    this->sr.ShockWave.Attribute.Position->setPointer(4, GL_FLOAT, 0, 0, (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_textureOrientationBuffer);    
    this->sr.ShockWave.Attribute.TexCoord->setPointer(2, GL_FLOAT, 0, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);    

    // wave
    for(msWaveIterator i = m_waves.begin(); i != m_waves.end(); i ++)
    {
        msWaveEmitter *we = *i;
        this->sr.ShockWave.Uniform.Radius->set1f(we->m_radius);
        this->sr.ShockWave.Uniform.Power->set1f(we->m_power);
        this->sr.ShockWave.Uniform.Location->set2f(we->m_location.x / we->m_size.width, we->m_location.y / we->m_size.height);
        
        we->step();
    }

    // draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);
}

void msBoxGridRenderer::_drawExplosions()
{
    // render fire into texture using particle shaders
    this->sr.ParticleCreate.Program->use();

    // Switch the render target to the current FBO to update the texture map
    this->sr.ParticleCreate.Framebuffer.RenderTex->bind();

    // FBO attachment is complete?
    if (this->sr.ParticleCreate.Framebuffer.RenderTex->isComplete())        
    {
        // Set viewport to size of texture map and erase previous image
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // render particles        
        _drawParticles(explosionsBundle);
        explosionsBundle.update(0.015f);
    }

    // Unbind the FBO so rendering will return to the backbuffer.
    m_shaders->getMainFrameBuffer()->bind();

    // usual renderer

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->sr.ParticleComplete.Program->use();
    
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);    
    this->sr.ParticleComplete.Attribute.A2Position->setPointer(4, GL_FLOAT, 0, 0, (void*)0 );
    
    glBindBuffer(GL_ARRAY_BUFFER, m_textureOrientationBuffer);    
    this->sr.ParticleComplete.Attribute.A2TexCoord->setPointer(2, GL_FLOAT, 0, 0, (void*)0 );
    glBindBuffer(GL_ARRAY_BUFFER, 0);    

    // draw with client side arrays (in real apps you should use cached VBOs which is much better for performance)
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);

    glDisable(GL_BLEND);
}

msWaveEmitter* msBoxGridRenderer::_createWave( msBox* box)
{
    msPoint3f location = box->getVerticesData()->getCenter();    
    location.x *= m_size.width;
    location.y = 1.0f - location.y;
    location.y *= m_size.height;

    return new msWaveEmitter(location, m_size);
}

void msBoxGridRenderer::_drawParticles(msParticleEmitterBundle &particleEmitters)
{
    int particlesCount = particleEmitters.getParticleCount();

    if(particlesCount == 0)
        return;

    if(this->particleBufferSize < particlesCount)
    {
        // if buffer size is not enough than recreate new buffer to fit the size and copy new data into it
        glBindBuffer(GL_ARRAY_BUFFER, this->particleBuffer);
        glBufferData(GL_ARRAY_BUFFER, particlesCount * sizeof(msParticleData), particleEmitters.getParticleData(), GL_DYNAMIC_DRAW);
        this->particleBufferSize = particlesCount;
    }
    else
    {
        // if buffer size is enough for particles than just copy new data into buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->particleBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(msParticleData), particleEmitters.getParticleData());
    }

    glEnable(GL_BLEND);
    glEnable ( GL_TEXTURE_2D );

    // todo: make blendAddictive as global setting for all emitters inside bundle
    //if(pe->blendAdditive)
    //{
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //}
    //else
    //{
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //}    

    // Load the vertex attributes
    this->sr.ParticleCreate.Attribute.APosition->setPointer(2, GL_FLOAT, GL_FALSE, sizeof(msParticleData), (void*)0);
    this->sr.ParticleCreate.Attribute.AColor->setPointer(4, GL_FLOAT, GL_FALSE, sizeof(msParticleData), (void*)offsetof(msParticleData, color));
    this->sr.ParticleCreate.Attribute.ASize->setPointer(1, GL_FLOAT, GL_FALSE, sizeof(msParticleData), (void*)offsetof(msParticleData, size));

    glDrawArrays( GL_POINTS, 0, particlesCount);

    glDisable ( GL_TEXTURE_2D );
    glDisable(GL_BLEND);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
