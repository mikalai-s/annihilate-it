#include "msShaderPrograms.h"

using namespace std;

msShaderPrograms::msShaderPrograms(void)
{
}

msShaderPrograms::~msShaderPrograms(void)
{
}

bool msShaderPrograms::loadFile( const char *fileName )
{
	int currentLine = 0;
	msShaderProgram *recentProgram;

	m_shaderPrograms.clear();

	ifstream fin( fileName, std::ios_base::binary );

	if ( fin.fail() )
	{
		cout << "Failed to open shader programs file: " << fileName << endl;
		return false;
	}

	while ( ! fin.eof() )
	{		
		string lineString;
		string sItem;
		bool addToVec = false;

		getline( fin, lineString );
		stringstream line( lineString );

		++currentLine;

		line >> sItem;

		if ( sItem == "program" )
		{
			string sKey, vertShader, fragShader;
			line >> sKey >> vertShader >> fragShader;

			// copy name
			char *programName = new char [sKey.length() + 1];
			strcpy_s( programName, sKey.length() + 1, sKey.c_str());

			// Create the one program we are going to use.
			recentProgram = new msShaderProgram(programName, vertShader.c_str(), fragShader.c_str());
			m_shaderPrograms.push_back(recentProgram);
		}
		else if ( sItem == "link" )
		{
			string progName;
			line >> progName;
			
			msShaderProgram* program = getProgramByName( progName.c_str() );
			if(program != NULL)
			{
				program->link();
			}
			else
			{
				cout << "Cannot link program " << progName << "because it doesn't exist" << endl;
			}
		}
		else if ( sItem == "uniform_float" )
		{
			string sKey;
			GLfloat fVal;

			line >> sKey >> fVal;

			char *name = new char [sKey.length()+1];
			strcpy_s( name, sKey.length()+1, sKey.c_str());

			recentProgram->addUniform(new msUniform(name, fVal));
		}
		else if (sItem == "uniform_vec2")
		{
			string sKey;
			GLfloat vVal[2];
			line >> sKey >> vVal[0] >> vVal[1];

			char *name = new char [sKey.length()+1];
			strcpy_s( name, sKey.length()+1, sKey.c_str());

			recentProgram->addUniform(new msUniform(name, vVal));
		}
		else if (sItem == "uniform_vec3")
		{
			string sKey;
			GLfloat vVal[3];
			line >> sKey >> vVal[0] >> vVal[1] >> vVal[2];

			char *name = new char [sKey.length()+1];
			strcpy_s( name, sKey.length()+1, sKey.c_str());

			recentProgram->addUniform(new msUniform(name, vVal));
		}
		else if ( sItem == "uniform_vec4" )
		{
			string sKey;
			GLfloat vVal[4];
			line >> sKey >> vVal[0] >> vVal[1] >> vVal[2] >> vVal[3]; 

			char *name = new char [sKey.length()+1];
			strcpy_s( name, sKey.length()+1, sKey.c_str());

			recentProgram->addUniform(new msUniform(name, vVal));
		}
		else if ( sItem == "uniform_mat4" )
		{
			string sKey;
			line >> sKey;

			GLfloat matrix[4][4];

			for (int i=0; i < 4; ++i)
				line >> matrix[i][0] >> matrix[i][1] >> matrix[i][2] >> matrix[i][3]; 

			char *name = new char [sKey.length()+1];
			strcpy_s( name, sKey.length()+1, sKey.c_str());

			recentProgram->addUniform(new msUniform(name, matrix));
		}
		else if (sItem == "attribute")
		{
			string sKey;
			GLuint location;

			line >> sKey >> location;

			char *name = new char [sKey.length()+1];
			strcpy_s( name, sKey.length()+1, sKey.c_str());

			recentProgram->addAttribute(new msAttribute(name, location));
		}
		else if (sItem == "texture")
		{
			string sKey, sVal;
			GLuint texUnit;
			line >> sKey >> texUnit >> sVal;

			char *name = new char [sKey.length()+1];
			strcpy_s( name, sKey.length()+1, sKey.c_str());

			recentProgram->addTexture(new msTexture(name, texUnit, sVal.c_str()));
		}
		else if (sItem == "nullTexture")
		{
			GLuint texUnit;
			string sKey;
			line >> sKey >> texUnit;

			char *name = new char [sKey.length()+1];
			strcpy_s( name, sKey.length()+1, sKey.c_str());

			recentProgram->addTexture(new msTexture(name, texUnit));
		}
		else if (sItem == "colorTexFbo")
		{/*
			GLuint texId, fboId;
			GLuint texUnit;
			string sKey;
			line >> sKey >> texUnit;

			addToVec = true;
			newUniform->m_name = new char [sKey.length()+1];
			strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
			newUniform->m_type = SAMPLE_INT;
			newUniform->m_textureUnit = texUnit;
			glGenTextures( 1, &texId );
#ifdef USE_STANDARD_GL
			glGenFramebuffersEXT( 1, &fboId );
#else
			glGenFramebuffers( 1, &fboId );
#endif // USE_STANDARD_GL
			newUniform->m_textureHandle = texId;
			newUniform->m_fboHandle = fboId;

			glActiveTexture( GL_TEXTURE0 + texUnit );
			glBindTexture( GL_TEXTURE_2D, texId );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );*/
		}		
		else if ( sItem.empty() || sItem[0]=='/' )
		{
			// Either an empty line or a comment
			addToVec = false;
		}
		else
		{
			addToVec = false;
			cout << "Unsupported keyword in uniform file." << endl;
//			assert(0);
		}
	}
	return true;
}

msShaderProgram* msShaderPrograms::getProgramByName( const char *name )
{
	for (msShaderProgramIterator iterator = m_shaderPrograms.begin(); iterator != m_shaderPrograms.end() ; ++iterator)
	{
		msShaderProgram* current = *iterator;
		if(current->getName() == name)
			return current;
	}	
	return NULL;
}


