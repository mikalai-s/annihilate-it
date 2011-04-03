#pragma once

class ParticleEmitter
{
public:
	ParticleEmitter(void);
	~ParticleEmitter(void);
private:
	GameGlobals *myGlobals;
	Texture *texture;
	
	Vector2f sourcePosition;
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
	GLint particleCount;
	GLfloat emissionRate;
	GLfloat emitCounter;	
	GLuint verticesID;
	GLuint colorsID;	
	Particle *particles;
	PointSprite *vertices;
	Color4f *colors;
	BOOL active;
	GLint particleIndex;
	GLfloat elapsedTime;
	GLfloat duration;
	BOOL blendAdditive; 
};
