#pragma once

#include "msShaderProgram.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

typedef std::vector<msShaderProgram*>	   msShaderProgramList;
typedef msShaderProgramList::iterator	msShaderProgramIterator;

class msShaderPrograms
{
public:
	msShaderPrograms(void);
	~msShaderPrograms(void);


	msShaderProgramList m_shaderPrograms;


	// load programs description
	bool loadFile(const char *fileName);

	msShaderProgram* getProgramByName( const char *name );

};
