#include "msShaderProgram.h"
#include <iostream>
#include <fstream>
#include <sstream>

msShaderProgram::msShaderProgram(string &name, string &vertexShaderFileName, string &fragmentShaderFileName)
{
	m_name = copyString(name);

	m_handle = glCreateProgram();

	m_vertexShaderHandle = glCreateShader( GL_VERTEX_SHADER );
	loadShader(vertexShaderFileName.c_str(), m_vertexShaderHandle);

	m_fragmentShaderHandle = glCreateShader( GL_FRAGMENT_SHADER );
	loadShader(fragmentShaderFileName.c_str(), m_fragmentShaderHandle);
}


msShaderProgram::~msShaderProgram(void)
{
	delete m_name;
	for(msUniformIterator i = m_uniforms.begin(); i != m_uniforms.end(); i++)
		delete (*i);
	for(msAttributeIterator i = m_attributes.begin(); i != m_attributes.end(); i++)
		delete (*i);
	for(msTextureIterator i = m_textures.begin(); i != m_textures.end(); i++)
		delete (*i);
	for(msFrameBufferIterator i = m_frameBuffers.begin(); i != m_frameBuffers.end(); i++)
		delete (*i);

	glDeleteProgram(m_handle);
	glDeleteShader(m_vertexShaderHandle);
	glDeleteShader(m_fragmentShaderHandle);
}

GLuint msShaderProgram::getHandle()
{
	return m_handle;
}

const char* msShaderProgram::getName()
{
	return m_name;
}


bool msShaderProgram::loadShader( const char* fileName, GLuint shaderHandle )
{
	char* source = NULL;


	// Use file io to load the code of the shader.
	std::ifstream fp( fileName , ios_base::binary );
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
	glAttachShader( m_handle, shaderHandle );

	return true;
}

bool msShaderProgram::link()
{
	// Link the whole program together.
	glLinkProgram( m_handle );

	// Check for link success
	GLint linkResult = 0;
	glGetProgramiv( m_handle, GL_LINK_STATUS, &linkResult );
	glUseProgram( m_handle );
	if ( linkResult == 0 )
	{
		char log[1000];
		GLuint aproximatelength = 1000; 
		GLsizei actualLength;
		glGetProgramInfoLog( m_handle, aproximatelength, &actualLength, log );
		cout << "Failed to link program object." << log << endl;
		return false;
	}
	linkAllDescriptors();
	return true;
}

void msShaderProgram::addUniform( msUniform *uniform)
{
	m_uniforms.push_back(uniform);
	uniform->setProgram(this);
}

void msShaderProgram::addAttribute( msAttribute *attribute)
{
	m_attributes.push_back(attribute);
	attribute->setProgram(this);
}

void msShaderProgram::addTexture( msTexture *texture)
{
	m_textures.push_back(texture);
	texture->setProgram(this);
}

void msShaderProgram::addFrameBuffer(msFrameBuffer *frameBuffer)
{
	m_frameBuffers.push_back(frameBuffer);
	//frameBuffer->setProgram(this);
}

msTexture* msShaderProgram::getTexture(const char *name)
{
	for(msTextureIterator i = m_textures.begin(); i != m_textures.end(); i ++)
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
	for(msUniformIterator i = m_uniforms.begin(); i != m_uniforms.end(); i ++)
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
		m_uniforms.push_back(uniform);
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
	for(msAttributeIterator i = m_attributes.begin(); i != m_attributes.end(); i ++)
	{
		msAttribute *attribute = *i;
		if(strcmp(attribute->getName(), name) == 0)
			return attribute;
	}
	return NULL;
}

msFrameBuffer* msShaderProgram::getFrameBuffer(const char *name)
{
	for(msFrameBufferIterator i = m_frameBuffers.begin(); i != m_frameBuffers.end(); i ++)
	{
		msFrameBuffer *frameBuffer = *i;
		if(strcmp(frameBuffer->getName(), name) == 0)
			return frameBuffer;
	}
	return NULL;
}

void msShaderProgram::use()
{
	glUseProgram(m_handle);
}

void msShaderProgram::linkAllDescriptors()
{
	for (msAttributeIterator ai = m_attributes.begin(); ai != m_attributes.end() ; ++ai)
		(*ai)->link();

	for (msUniformIterator ai = m_uniforms.begin(); ai != m_uniforms.end() ; ++ai)
		(*ai)->link();

	for (msTextureIterator ai = m_textures.begin(); ai != m_textures.end() ; ++ai)
		(*ai)->link();
}

void msShaderProgram::notifySizeChanged( GLint width, GLint height )
{
	for(msFrameBufferIterator i = m_frameBuffers.begin(); i != m_frameBuffers.end(); i ++)
	{
		(*i)->setSize(width, height);
	}
}
