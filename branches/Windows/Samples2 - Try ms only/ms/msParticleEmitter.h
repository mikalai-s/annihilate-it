#pragma once

#include "msCommon.h"

#include "uniform.h"

class msShaderProgram;

class msParticleEmitter
{
private:
	//Image *texture;

	
	Vector2f sourcePositionVariance;
	GLfloat angle;
	GLfloat angleVariance;
	GLfloat speed;
	GLfloat speedVariance;
	Vector2f gravity;
	GLfloat particleLifespan;
	GLfloat particleLifespanVariance;
	Color4f startColor;
	Color4f startColorVariance;
	Color4f finishColor;
	Color4f finishColorVariance;
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

	void initParticle(Particle* particle);

	void stopParticleEmitter();

public:
	Vector2f sourcePosition;
	GLfloat duration;
	GLboolean active;
		GLuint particleCount;
		Particle *particles;
		PointSprite *vertices;
		Color4f *colors;
		GLboolean blendAdditive;

	msParticleEmitter(
		char *inTextureName,
		Vector2f inPosition ,
		Vector2f inSourcePositionVariance,
		GLfloat inSpeed,
		GLfloat inSpeedVariance,
		GLfloat inParticleLifeSpan,
		GLfloat inParticleLifeSpanVariance,
		GLfloat inAngle,
		GLfloat inAngleVariance,
		Vector2f inGravity,
		Color4f inStartColor,
		Color4f inStartColorVariance,
		Color4f inFinishColor ,
		Color4f inFinishColorVariance,
		GLuint inMaxParticles ,
		GLfloat inParticleSize,
		GLfloat inParticleSizeVariance,
		GLfloat inDuration,
		GLboolean inBlendAdditive);

	

	void renderParticles(msShaderProgram *particleProgram);
	

	void update(GLfloat delta);

	~msParticleEmitter(void);
};
