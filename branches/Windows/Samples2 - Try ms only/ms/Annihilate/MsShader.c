/*
 *  MsShader.c
 *  AnnihilateIt
 *
 *  Created by user on 11-03-17.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MsShader.h"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
/*
int LoadShader (GLenum type, const char* source)
{
	const unsigned int shader = glCreateShader(type);
	if (shader == 0)
		return 0;
    
	glShaderSource(shader, 1, (const GLchar**)&source, NULL);
	glCompileShader(shader);
    
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		char errorMsg[2048];
		glGetShaderInfoLog(shader, sizeof(errorMsg), NULL, errorMsg);
		printf("Compile error: %s\n", errorMsg);
		glDeleteShader(shader);
		return 0;
	}
    
    return shader;
}
*/