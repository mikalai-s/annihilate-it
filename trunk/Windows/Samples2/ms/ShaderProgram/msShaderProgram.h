#pragma once

#include "GLES2/gl2.h"
#include <vector>
#include "msUniform.h"
#include "msAttribute.h"
#include "msTexture.h"

class msAttribute;
class msUniform;
class msTexture;

typedef std::vector<msUniform> msUniformList;
typedef msUniformList::iterator msUniformIterator;

typedef std::vector<msAttribute> msAttributeList;
typedef msAttributeList::iterator msAttributeIterator;

typedef std::vector<msTexture> msTextureList;
typedef msTextureList::iterator msTextureIterator;

class msShaderProgram
{
	char *m_name;
	GLuint m_handle;
	GLuint m_vertexShaderHandle;
	GLuint m_fragmentShaderHandle;

	msUniformList m_uniforms;
	msAttributeList m_attributes;
	msTextureList m_textures;

	bool loadShader( const char* fileName, GLuint shaderHandle );

public:
	msShaderProgram(const char* name, const char *vertexShaderFileName, const char *fragmentShaderFileName);
	~msShaderProgram(void);

	GLuint getHandle();
	char *getName();

	bool link();

	void addUniform( msUniform *uniform );
	void addAttribute( msAttribute *attribute );
	void addTexture( msTexture *texture );
	void updateAllUniforms();
};
