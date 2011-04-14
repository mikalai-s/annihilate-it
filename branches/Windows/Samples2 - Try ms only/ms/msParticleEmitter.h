#pragma once

#include "msCommon.h"

class msShaderProgram;

class msParticleEmitter
{
private:
	//Image *texture;

	
	msVector sourcePositionVariance;
	GLfloat angle;
	GLfloat angleVariance;
	GLfloat speed;
	GLfloat speedVariance;
	msVector gravity;
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
	msVector sourcePosition;
	GLfloat duration;
	GLboolean active;
		GLuint particleCount;
		msParticle *particles;
		msPointSprite *vertices;
		msColor *colors;
		GLboolean blendAdditive;

	msParticleEmitter(
		msVector inPosition ,
		msVector inSourcePositionVariance,
		GLfloat inSpeed,
		GLfloat inSpeedVariance,
		GLfloat inParticleLifeSpan,
		GLfloat inParticleLifeSpanVariance,
		GLfloat inAngle,
		GLfloat inAngleVariance,
		msVector inGravity,
		msColor inStartColor,
		msColor inStartColorVariance,
		msColor inFinishColor ,
		msColor inFinishColorVariance,
		GLuint inMaxParticles ,
		GLfloat inParticleSize,
		GLfloat inParticleSizeVariance,
		GLfloat inDuration,
		GLboolean inBlendAdditive);

	

	void renderParticles(msShaderProgram *particleProgram);
	

	void update(GLfloat delta);

	~msParticleEmitter(void);
};
