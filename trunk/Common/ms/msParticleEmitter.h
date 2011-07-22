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
    msParticleData *particleData;

    msPoint2f direction;
    msColor deltaColor;
    float timeToLive;
};


class msParticleEmitter
{
private:

	msPoint2f sourcePositionVariance;
	GLfloat angle;
	GLfloat angleVariance;
	GLfloat speed;
	GLfloat speedVariance;
	msPoint2f gravity;
	GLfloat particleLifespan;
	GLfloat particleLifespanVariance;
	msColor startColor;
	msColor startColorVariance;
	msColor finishColor;
	msColor finishColorVariance;
	GLfloat particleSize;
	GLfloat particleSizeVariance;
	GLuint maxParticles;

	GLfloat emissionRate;
	GLfloat emitCounter;	
	GLuint verticesID;
	GLuint colorsID;	
	
	
	GLuint particleIndex;
//	BOOL useTexture;
	GLfloat elapsedTime;
	



	GLboolean addParticle();

	void initParticle(msParticle* particle);

	void stopParticleEmitter();

public:
	msPoint2f sourcePosition;
	GLfloat duration;
	GLboolean active;
	GLuint particleCount;
	msParticle *particles;
    msParticleData *particleDatas;
	GLboolean blendAdditive;

	msParticleEmitter(
		msPoint2f inPosition ,
		msPoint2f inSourcePositionVariance,
		GLfloat inSpeed,
		GLfloat inSpeedVariance,
		GLfloat inParticleLifeSpan,
		GLfloat inParticleLifeSpanVariance,
		GLfloat inAngle,
		GLfloat inAngleVariance,
		msPoint2f inGravity,
		msColor inStartColor,
		msColor inStartColorVariance,
		msColor inFinishColor ,
		msColor inFinishColorVariance,
		GLuint inMaxParticles ,
		GLfloat inParticleSize,
		GLfloat inParticleSizeVariance,
		GLfloat inDuration,
		GLboolean inBlendAdditive);

	

	void update(GLfloat delta);

    GLboolean isAlive();

	~msParticleEmitter(void);
};
