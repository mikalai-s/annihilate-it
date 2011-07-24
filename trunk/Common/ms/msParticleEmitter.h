#pragma once

#include "msCommon.h"

class msShaderProgram;


struct msParticleData
{
    msPoint2f position;
    float size;
    msColor color;
};

struct msParticle
{
    msPoint2f direction;
    msColor deltaColor;
    float timeToLive;
};

struct msParticleEmitterSettings
{
    msPoint2f position;
    msPoint2f sourcePositionVariance;
    GLfloat speed;
    GLfloat speedVariance;
    GLfloat particleLifeSpan;
    GLfloat particleLifeSpanVariance;
    GLfloat angle;
    GLfloat angleVariance;
    msPoint2f gravity;
    msColor startColor;
    msColor startColorVariance;
    msColor finishColor;
    msColor finishColorVariance;
    GLuint maxParticles;
    GLfloat particleSize;
    GLfloat particleSizeVariance;
    GLfloat duration;
    GLboolean blendAdditive;
};


class msParticleEmitter
{
	friend class msParticleEmitterBundle;

protected:

    msParticleEmitterSettings settings;
    
	GLfloat emissionRate;
	GLfloat emitCounter;	
	GLuint verticesID;
	GLuint colorsID;		
	
	GLuint particleIndex;
	GLfloat elapsedTime;

	bool addParticle();

	void initParticle(int particleIndex);

	void stopParticleEmitter();

public:
	GLboolean active;
	GLuint particleCount;
	msParticle *particles;
    msParticleData *particleData;
	GLboolean blendAdditive;

	msParticleEmitter(msParticleEmitterSettings &settings);

	void update(GLfloat delta);

    GLboolean isAlive();

	~msParticleEmitter(void);

protected:
	virtual msParticleData* resolveParticleData();
	virtual void deleteParticleData();
};