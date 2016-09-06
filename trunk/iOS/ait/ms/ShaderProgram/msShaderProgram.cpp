#include "msShaderProgram.h"
#include <iostream>
#include <fstream>
#include <sstream>

msShaderProgram::msShaderProgram(string &name, string &vertexShaderFileName, string &fragmentShaderFileName)
{
    this->name = copyString(name);

    this->handle = glCreateProgram();

    this->vertexShaderHandle = glCreateShader( GL_VERTEX_SHADER );
    loadShader(vertexShaderFileName, this->vertexShaderHandle);

    this->fragmentShaderHandle = glCreateShader( GL_FRAGMENT_SHADER );
    loadShader(fragmentShaderFileName, this->fragmentShaderHandle);
}


msShaderProgram::~msShaderProgram(void)
{
    delete this->name;
    for(msUniformIterator i = this->uniforms.begin(); i != this->uniforms.end(); i++)
        delete (*i);
    for(msAttributeIterator i = this->attributes.begin(); i != this->attributes.end(); i++)
        delete (*i);
    for(msTextureIterator i = this->textures.begin(); i != this->textures.end(); i++)
        delete (*i);
    for(msFrameBufferIterator i = this->frameBuffers.begin(); i != this->frameBuffers.end(); i++)
        delete (*i);

    glDeleteProgram(this->handle);
    glDeleteShader(this->vertexShaderHandle);
    glDeleteShader(this->fragmentShaderHandle);
}

GLuint msShaderProgram::getHandle()
{
    return this->handle;
}

const char* msShaderProgram::getName()
{
    return this->name;
}


bool msShaderProgram::loadShader( string &fileName, GLuint shaderHandle )
{
    char* source = NULL;    

    // Use file io to load the code of the shader.
    std::ifstream fp( fileName.c_str() , ios_base::binary );
    if( fp.fail() ) 
    {
        cout << "Failed to open shader file: " << fileName << endl;
        return false;
    }

    fp.seekg( 0, std::ios_base::end );
    const long len = fp.tellg();
    fp.seekg( 0, std::ios_base::beg );
    // The +1 is optional, depending on how you call glShaderSourceARB
    source = new char[len+1];
    fp.read(source, sizeof(char)*len);
    source[len] = NULL;


    const char* gls[1] = { source };

    // Pass our sources to OpenGL. Our sources are NULL terminated, so pass NULL for the lenghts.
    glShaderSource( shaderHandle, 1, gls, NULL );
    // OpenGL made a copy. Do not need the source anymore.
    delete[] source;
    source = 0;

    // Compile that one object.
    glCompileShader(shaderHandle);

    // Check for compilation success
    GLint compilationResult = 0;
    glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &compilationResult );

    // current implementation always succeeds.
    // The error will happen at link time.
    if ( compilationResult == 0 )
    {
        char log[1000];
        GLuint aproximatelength = 1000;
        GLsizei actualLength;
        cout << "Failed to compile shader:" << fileName << endl;
        glGetShaderInfoLog( shaderHandle, aproximatelength, &actualLength, log );
        cout << log << endl;
        return false;
    }

    // Attach them to the program.
    glAttachShader( this->handle, shaderHandle );

    return true;
}

bool msShaderProgram::link()
{
    // Link the whole program together.
    glLinkProgram( this->handle );

    // Check for link success
    GLint linkResult = 0;
    glGetProgramiv( this->handle, GL_LINK_STATUS, &linkResult );
    glUseProgram( this->handle );
    if ( linkResult == 0 )
    {
        char log[1000];
        GLuint aproximatelength = 1000; 
        GLsizei actualLength;
        glGetProgramInfoLog( this->handle, aproximatelength, &actualLength, log );
        cout << "Failed to link program object." << log << endl;
        return false;
    }
    linkAllDescriptors();
    return true;
}

void msShaderProgram::addUniform( msUniform *uniform)
{
    this->uniforms.push_back(uniform);
    uniform->setProgram(this);
}

void msShaderProgram::addAttribute( msAttribute *attribute)
{
    this->attributes.push_back(attribute);
    attribute->setProgram(this);
}

void msShaderProgram::addTexture( msTexture *texture)
{
    this->textures.push_back(texture);
    texture->setProgram(this);
}

void msShaderProgram::addFrameBuffer(msFrameBuffer *frameBuffer)
{
    this->frameBuffers.push_back(frameBuffer);
    //frameBuffer->setProgram(this);
}

msTexture* msShaderProgram::getTexture(const char *name)
{
    for(msTextureIterator i = this->textures.begin(); i != this->textures.end(); i ++)
    {
        msTexture* texture = *i;
        if(strcmp(texture->getName(), name) == 0)
            return texture;
    }
    return NULL;
}

msUniform* msShaderProgram::getUniform(const char *name)
{
    // try to find uniform in already declared uniforms
    for(msUniformIterator i = this->uniforms.begin(); i != this->uniforms.end(); i ++)
    {
        msUniform *uniform = *i;
        if(strcmp(uniform->getName(), name) == 0)
            return uniform;
    }
    // if uniform is not found - try to create new uniform from shader code to resolve its location
    string n(name);
    msUniform *uniform = new msUniform(n); // create it
    uniform->setProgram(this); // set parent
    uniform->link(); // to ensure location
    if(uniform->getLocation() >= 0)
    {
        // add this uniform into unifrom collection
        this->uniforms.push_back(uniform);
    }
    else
    {
        // delete it because there is no such uniform in shader code
        delete uniform;
    }
    return uniform;
}

msAttribute* msShaderProgram::getAttribute(const char *name)
{
    for(msAttributeIterator i = this->attributes.begin(); i != this->attributes.end(); i ++)
    {
        msAttribute *attribute = *i;
        if(strcmp(attribute->getName(), name) == 0)
            return attribute;
    }
    return NULL;
}

msFrameBuffer* msShaderProgram::getFrameBuffer(const char *name)
{
    for(msFrameBufferIterator i = this->frameBuffers.begin(); i != this->frameBuffers.end(); i ++)
    {
        msFrameBuffer *frameBuffer = *i;
        if(strcmp(frameBuffer->getName(), name) == 0)
            return frameBuffer;
    }
    return NULL;
}

void msShaderProgram::use()
{
    glUseProgram(this->handle);
}

void msShaderProgram::linkAllDescriptors()
{
    for (msAttributeIterator ai = this->attributes.begin(); ai != this->attributes.end() ; ++ai)
        (*ai)->link();

    for (msUniformIterator ai = this->uniforms.begin(); ai != this->uniforms.end() ; ++ai)
        (*ai)->link();

    for (msTextureIterator ai = this->textures.begin(); ai != this->textures.end() ; ++ai)
        (*ai)->link();
}

void msShaderProgram::notifySizeChanged( GLint width, GLint height )
{
    for(msFrameBufferIterator i = this->frameBuffers.begin(); i != this->frameBuffers.end(); i ++)
    {
        (*i)->setSize(width, height);
    }
}
