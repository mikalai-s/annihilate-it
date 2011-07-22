#include "msParticleEmitter.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "time.h"

#include "ShaderProgram/msTexture.h"
#include "ShaderProgram/msShaderProgram.h"


msParticleEmitter::msParticleEmitter(
									 msPoint2f inPosition,
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
									 msColor inFinishColor,
									 msColor inFinishColorVariance,
									 GLuint inMaxParticles,
									 GLfloat inParticleSize,
									 GLfloat inParticleSizeVariance,
									 GLfloat inDuration,
									 GLboolean inBlendAdditive)
{	
	// Take the parameters which have been passed into this particle emitter and set the emitters
	// properties
	sourcePosition = inPosition;
	sourcePositionVariance = inSourcePositionVariance;
	speed = inSpeed;
	speedVariance = inSpeedVariance;
	particleLifespan = inParticleLifeSpan;
	particleLifespanVariance = inParticleLifeSpanVariance;
	angle = inAngle;
	angleVariance = inAngleVariance;
	gravity = inGravity;
	startColor = inStartColor;
	startColorVariance = inStartColorVariance;
	finishColor = inFinishColor;
	finishColorVariance = inFinishColorVariance;
	maxParticles = inMaxParticles;
	particleSize = inParticleSize;
	particleSizeVariance = inParticleSizeVariance;
	emissionRate = maxParticles/particleLifespan;
	duration = inDuration;
	blendAdditive = inBlendAdditive;

	// get memory for particle data
	particleData = resolveParticleData();

	// Allocate the memory necessary for the particle emitter arrays
	particles = (msParticle*)calloc(maxParticles, sizeof(msParticle));

    // assign particle data for each particle
    for(int i = 0; i < maxParticles; i ++)
        particles[i].particleData = &particleData[i];

	// If one of the arrays cannot be allocated, then report a warning and return nil
	if(!particles)
	{
		printf("WARNING: ParticleEmitter - Not enough memory");
		if(particles)
			free(particles);		
	}	

	// By default the particle emitter is active when created
	active = GL_TRUE;

	// Set the particle count to zero
	particleCount = 0;

	// Reset the elapsed time
	elapsedTime = 0;

	emitCounter = 0;
}

msParticleEmitter::~msParticleEmitter(void)
{
	free(particles);

	deleteParticleData();
}

void msParticleEmitter::update(GLfloat delta)
{
	// If the emitter is active and the emission rate is greater than zero then emit
	// particles
	if(active && emissionRate)
	{
		GLfloat rate = 1.0f / emissionRate;
		emitCounter += delta;
		while(particleCount < maxParticles && emitCounter > rate)
		{
			addParticle();
			emitCounter -= rate;
		}

		elapsedTime += delta;
		if(duration != -1 && duration < elapsedTime)
			stopParticleEmitter();
	}

	// Reset the particle index before updating the particles in this emitter
	particleIndex = 0;

	// Loop through all the particles updating their location and color
	while(particleIndex < particleCount) {

		// Get the particle for the current particle index
		msParticle *currentParticle = &particles[particleIndex];
        msParticleData *particleData = currentParticle->particleData;

		// If the current particle is alive then update it
		if(currentParticle->timeToLive > 0) {

			// Calculate the new direction based on gravity
			currentParticle->direction.add(gravity);
			//currentParticle->direction.multiply(delta);
			particleData->position.add(currentParticle->direction);

			// Reduce the life span of the particle
			currentParticle->timeToLive -= delta;

			// Update the particles color
			particleData->color.r += (currentParticle->deltaColor.r * delta);
			particleData->color.g += (currentParticle->deltaColor.g * delta);
			particleData->color.b += (currentParticle->deltaColor.b * delta);
			particleData->color.a += (currentParticle->deltaColor.a * delta);

			// Update the particle counter
			particleIndex++;
		} else {

			// As the particle is not alive anymore replace it with the last active particle 
			// in the array and reduce the count of particles by one.  This causes all active particles
			// to be packed together at the start of the array so that a particle which has run out of
			// life will only drop into this clause once
			if(particleIndex != particleCount-1)
				particles[particleIndex] = particles[particleCount-1];
			particleCount--;
		}
	}	
}

GLboolean msParticleEmitter::addParticle()
{
	// If we have already reached the maximum number of particles then do nothing
	if(particleCount == maxParticles)
		return GL_FALSE;

	// Take the next particle out of the particle pool we have created and initialize it
	msParticle *particle = &particles[particleCount];
	initParticle(particle);

	// Increment the particle count
	particleCount++;

	// Return YES to show that a particle has been created
	return GL_TRUE; 
}

void msParticleEmitter::initParticle(msParticle* particle)
{
	//srand((unsigned)time( NULL ));
	// Init the position of the particle.  This is based on the source position of the particle emitter
	// plus a configured variance.  The RANDOM_MINUS_1_TO_1 macro allows the number to be both positive
	// and negative
	particle->particleData->position.x = sourcePosition.x + sourcePositionVariance.x * RANDOM_MINUS_1_TO_1();
	particle->particleData->position.y = sourcePosition.y + sourcePositionVariance.y * RANDOM_MINUS_1_TO_1();

    // Calculate the vectorSpeed using the speed and speedVariance which has been passed in
    float vectorSpeed = speed + speedVariance * RANDOM_MINUS_1_TO_1();

	// Init the direction of the particle.  The newAngle is calculated using the angle passed in and the
	// angle variance.
	float newAngle = (GLfloat)DEGREES_TO_RADIANS(angle + angleVariance * RANDOM_MINUS_1_TO_1());

	// Create a new Vector2f using the newAngle
	particle->direction = msPoint2f(cosf(newAngle), sinf(newAngle));

	// The particles direction vector is calculated by taking the vector calculated above and
	// multiplying that by the speed
	particle->direction.multiply(vectorSpeed);

	// Calculate the particle size using the particleSize and variance passed in
	particle->particleData->size = particleSize + particleSizeVariance * RANDOM_MINUS_1_TO_1();

	// Calculate the particles life span using the life span and variance passed in
	particle->timeToLive = particleLifespan + particleLifespanVariance * RANDOM_MINUS_1_TO_1();

	// Calculate the color the particle should have when it starts its life.  All the elements
	// of the start color passed in along with the variance as used to calculate the star color
	msColor start;
	start.r = startColor.r + startColorVariance.r * RANDOM_MINUS_1_TO_1();
	start.g = startColor.g + startColorVariance.g * RANDOM_MINUS_1_TO_1();
	start.b = startColor.b + startColorVariance.b * RANDOM_MINUS_1_TO_1();
	start.a = startColor.a + startColorVariance.a * RANDOM_MINUS_1_TO_1();

	// Calculate the color the particle should be when its life is over.  This is done the same
	// way as the start color above
	msColor end;
	end.r = finishColor.r + finishColorVariance.r * RANDOM_MINUS_1_TO_1();
	end.g = finishColor.g + finishColorVariance.g * RANDOM_MINUS_1_TO_1();
	end.b = finishColor.b + finishColorVariance.b * RANDOM_MINUS_1_TO_1();
	end.a = finishColor.a + finishColorVariance.a * RANDOM_MINUS_1_TO_1();

	// Calculate the delta which is to be applied to the particles color during each cycle of its
	// life.  The delta calculation uses the life space of the particle to make sure that the 
	// particles color will transition from the start to end color during its life time.
	particle->particleData->color = start;
	particle->deltaColor.r = (end.r - start.r) / particle->timeToLive;
	particle->deltaColor.g = (end.g - start.g) / particle->timeToLive;
	particle->deltaColor.b = (end.b - start.b) / particle->timeToLive;
	particle->deltaColor.a= (end.a - start.a) / particle->timeToLive;
}

void msParticleEmitter::stopParticleEmitter()
{
	active = GL_FALSE;
	elapsedTime = 0;
	emitCounter = 0; 
}

GLboolean msParticleEmitter::isAlive()
{
    return (particleCount > 0);
}

msParticleData* msParticleEmitter::resolveParticleData()
{
	return (msParticleData*)calloc(this->maxParticles, sizeof(msParticleData));
}

void msParticleEmitter::deleteParticleData()
{
	free(this->particleData);
}


msParticleData* msBundeledParticleEmitter::resolveParticleData()
{
	return (msParticleData*)calloc(this->maxParticles, sizeof(msParticleData));
}

void msBundeledParticleEmitter::deleteParticleData()
{
	free(this->particleData);
}