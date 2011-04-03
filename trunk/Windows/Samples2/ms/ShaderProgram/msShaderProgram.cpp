#include "msShaderProgram.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

msShaderProgram::msShaderProgram(const char* name, const char *vertexShaderFileName, const char *fragmentShaderFileName)
{
	m_handle = glCreateProgram();

	m_vertexShaderHandle = glCreateShader( GL_VERTEX_SHADER );
	loadShader(vertexShaderFileName, m_vertexShaderHandle);

	m_fragmentShaderHandle = glCreateShader( GL_FRAGMENT_SHADER );
	loadShader(fragmentShaderFileName, m_fragmentShaderHandle);
}


msShaderProgram::~msShaderProgram(void)
{
}

GLuint msShaderProgram::getHandle()
{
	return m_handle;
}

char* msShaderProgram::getName()
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
	updateAllUniforms( );
	if ( linkResult == 0 )
	{
		char log[1000];
		GLuint aproximatelength = 1000; 
		GLsizei actualLength;
		glGetProgramInfoLog( m_handle, aproximatelength, &actualLength, log );
		cout << "Failed to link program object." << log << endl;
		return false;
	}
	return true;
}

void msShaderProgram::addUniform( msUniform *uniform)
{
	m_uniforms.push_back(*uniform);
	uniform->setProgram(this);
}

void msShaderProgram::addAttribute( msAttribute *attribute)
{
	m_attributes.push_back(*attribute);
	attribute->setProgram(this);
}

void msShaderProgram::addTexture( msTexture *texture)
{
	m_textures.push_back(*texture);
	texture->setProgram(this);
}

void msShaderProgram::updateAllUniforms()
{
	for (msUniformIterator ai = m_uniforms.begin(); ai != m_uniforms.end() ; ++ai)
	{/*
			switch( (*index)->m_type )
			{
			case SAMPLE_FLOAT:
				glUniform1f( (*index)->m_location, (*index)->m_floatData[0] );
				break;
			case SAMPLE_FLOAT_VEC2:
				glUniform2f( (*index)->m_location, (*index)->m_floatData[0], (*index)->m_floatData[1] );
				break;
			case SAMPLE_FLOAT_VEC3:
				glUniform3f( (*index)->m_location, (*index)->m_floatData[0], (*index)->m_floatData[1], (*index)->m_floatData[2] );
				break;
			case SAMPLE_FLOAT_VEC4:
				glUniform4f( (*index)->m_location, (*index)->m_floatData[0], (*index)->m_floatData[1], (*index)->m_floatData[2], (*index)->m_floatData[3] );
				break;
			case SAMPLE_FLOAT_MAT4:
				glUniformMatrix4fv( (*index)->m_location, 1, GL_FALSE, &(*index)->m_matrixData[0][0] );
				break;
			case SAMPLE_INT:
				if ( (*index)->m_location != 0 ) // Prevents error on nullTexture which has not yet been given a location (done at bind time)
				{
					glUniform1i( (*index)->m_location, (*index)->m_textureUnit );
				}
				break;
			case SAMPLE_PROGRAM:
				break;
			default:
				assert(0);
				break;
			}
		*/
	}
}
