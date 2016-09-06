#include "msParticleEmitter.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "time.h"

#include "ShaderProgram/msTexture.h"
#include "ShaderProgram/msShaderProgram.h"


msParticleEmitter::msParticleEmitter(msParticleEmitterSettings &settings)
{
    // Take the parameters which have been passed into this particle emitter and set the emitters
    // properties
    this->settings = settings;

    // get memory for particle data
    this->particleData = (msParticleData*)calloc(settings.maxParticles, sizeof(msParticleData));

    // Allocate the memory necessary for the particle emitter arrays
    this->particles = (msParticle*)calloc(settings.maxParticles, sizeof(msParticle));    
    
    emissionRate = settings.maxParticles / settings.particleLifeSpan;

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
    if(this->active && this->emissionRate)
    {
        GLfloat rate = 1.0f / emissionRate;
        emitCounter += delta;
        while(this->particleCount < this->settings.maxParticles && this->emitCounter > rate)
        {
            addParticle();
            this->emitCounter -= rate;
        }

        this->elapsedTime += delta;
        if(this->settings.duration != -1 && this->settings.duration < this->elapsedTime)
            stopParticleEmitter();
    }

    // Reset the particle index before updating the particles in this emitter
    int particleIndex = 0;

    // Loop through all the particles updating their location and color
    while(particleIndex < this->particleCount)
    {
        // Get the particle for the current particle index
        msParticle *currentParticle = &this->particles[particleIndex];
        msParticleData *currentParticleData = &this->particleData[particleIndex];

        // If the current particle is alive then update it
        if(currentParticle->timeToLive > 0) {

            // Calculate the new direction based on gravity
            currentParticle->direction.add(this->settings.gravity);
            currentParticleData->position.add(currentParticle->direction);

            // Reduce the life span of the particle
            currentParticle->timeToLive -= delta;

            // Update the particles color
            currentParticleData->color.r += (currentParticle->deltaColor.r * delta);
            currentParticleData->color.g += (currentParticle->deltaColor.g * delta);
            currentParticleData->color.b += (currentParticle->deltaColor.b * delta);
            currentParticleData->color.a += (currentParticle->deltaColor.a * delta);

            // Update the particle counter
            particleIndex++;
        }
        else
        {
            // As the particle is not alive anymore replace it with the last active particle 
            // in the array and reduce the count of particles by one.  This causes all active particles
            // to be packed together at the start of the array so that a particle which has run out of
            // life will only drop into this clause once
            if(particleIndex != this->particleCount-1)
            {
                this->particles[particleIndex] = this->particles[this->particleCount-1];
                this->particleData[particleIndex] = this->particleData[this->particleCount-1];
            }
            particleCount--;
        }
    }    
}

bool msParticleEmitter::addParticle()
{
    // If we have already reached the maximum number of particles then do nothing
    if(particleCount == this->settings.maxParticles)
        return GL_FALSE;

    // Take the next particle out of the particle pool we have created and initialize it
    initParticle(particleCount);

    // Increment the particle count
    particleCount++;

    // Return YES to show that a particle has been created
    return GL_TRUE; 
}

void msParticleEmitter::initParticle(int particleIndex)
{
    msParticle *particle = &this->particles[particleIndex];
    msParticleData *particleData = &this->particleData[particleIndex];

    //srand((unsigned)time( NULL ));
    // Init the position of the particle.  This is based on the source position of the particle emitter
    // plus a configured variance.  The RANDOM_MINUS_1_TO_1 macro allows the number to be both positive
    // and negative
    particleData->position.x = this->settings.position.x + this->settings.sourcePositionVariance.x * RANDOM_MINUS_1_TO_1();
    particleData->position.y = this->settings.position.y + this->settings.sourcePositionVariance.y * RANDOM_MINUS_1_TO_1();

    // Calculate the vectorSpeed using the speed and speedVariance which has been passed in
    float vectorSpeed = this->settings.speed + this->settings.speedVariance * RANDOM_MINUS_1_TO_1();

    // Init the direction of the particle.  The newAngle is calculated using the angle passed in and the
    // angle variance.
    float newAngle = (GLfloat)DEGREES_TO_RADIANS(this->settings.angle + this->settings.angleVariance * RANDOM_MINUS_1_TO_1());

    // Create a new Vector2f using the newAngle
    particle->direction = msPoint2f(cosf(newAngle), sinf(newAngle));

    // The particles direction vector is calculated by taking the vector calculated above and
    // multiplying that by the speed
    particle->direction.multiply(vectorSpeed);

    // Calculate the particle size using the particleSize and variance passed in
    particleData->size = this->settings.particleSize + this->settings.particleSizeVariance * RANDOM_MINUS_1_TO_1();

    // Calculate the particles life span using the life span and variance passed in
    particle->timeToLive = this->settings.particleLifeSpan + this->settings.particleLifeSpanVariance * RANDOM_MINUS_1_TO_1();

    // Calculate the color the particle should have when it starts its life.  All the elements
    // of the start color passed in along with the variance as used to calculate the star color
    msColor start;
    start.r = this->settings.startColor.r + this->settings.startColorVariance.r * RANDOM_MINUS_1_TO_1();
    start.g = this->settings.startColor.g + this->settings.startColorVariance.g * RANDOM_MINUS_1_TO_1();
    start.b = this->settings.startColor.b + this->settings.startColorVariance.b * RANDOM_MINUS_1_TO_1();
    start.a = this->settings.startColor.a + this->settings.startColorVariance.a * RANDOM_MINUS_1_TO_1();

    // Calculate the color the particle should be when its life is over.  This is done the same
    // way as the start color above
    msColor end;
    end.r = this->settings.finishColor.r + this->settings.finishColorVariance.r * RANDOM_MINUS_1_TO_1();
    end.g = this->settings.finishColor.g + this->settings.finishColorVariance.g * RANDOM_MINUS_1_TO_1();
    end.b = this->settings.finishColor.b + this->settings.finishColorVariance.b * RANDOM_MINUS_1_TO_1();
    end.a = this->settings.finishColor.a + this->settings.finishColorVariance.a * RANDOM_MINUS_1_TO_1();

    // Calculate the delta which is to be applied to the particles color during each cycle of its
    // life.  The delta calculation uses the life space of the particle to make sure that the 
    // particles color will transition from the start to end color during its life time.
    particleData->color = start;
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
    return (msParticleData*)calloc(this->settings.maxParticles, sizeof(msParticleData));
}

void msParticleEmitter::deleteParticleData()
{
    free(this->particleData);
}

void msParticleEmitter::unitTest()
{
    msParticleEmitterSettings settings;
    settings.position                   = msPoint2f(0.5, 0.5);
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
    settings.particleSize               = 50 * 1;
    settings.particleSizeVariance       = 3 * 1;
    settings.duration                   = 0.125f;
    settings.blendAdditive              = GL_TRUE;
    msParticleEmitter pe(settings);

    FILE *stream;
//    fopen( &stream, "c:\\bad.txt", "w" );

    int j = 0;


    while((pe.isAlive() || pe.active))
    {
        for(int i = 0; i < pe.particleCount; i ++)
        {
            msParticle *p = &pe.particles[i];
            msParticleData *pd = &pe.particleData[i];

            fprintf(stream, "particle #%d\r\n", i);
            fprintf(stream, "\tposition:\t%f, %f\r\n", pd->position.x, pd->position.y);
            fprintf(stream, "\tsize:\t\t%f\r\n", pd->size);
            fprintf(stream, "\tcolor:\t\t%f, %f, %f, %f\r\n", pd->color.a, pd->color.g, pd->color.b, pd->color.a);
            fprintf(stream, "\tdirection:\t%f, %f\r\n", p->direction.x, p->direction.y);
            fprintf(stream, "\tdcolor:\t\t%f, %f, %f, %f\r\n", p->deltaColor.a, p->deltaColor.g, p->deltaColor.b, p->deltaColor.a);
            fprintf(stream, "\ttime:\t\t%f\r\n\r\n", p->timeToLive);
        }
        fprintf(stream, "update %d\r\n\r\n", j ++);

        pe.update(0.015f);
    }

     fclose( stream );
}