#include "msShaderPrograms.h"

using namespace std;

msShaderPrograms::msShaderPrograms(void)
{
}

msShaderPrograms::~msShaderPrograms(void)
{
}

inline char* copyString(string src)
{
	char *dest = new char [src.length() + 1];
	strcpy_s( dest, src.length() + 1, src.c_str());
	return dest;
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

		getline( fin, lineString );
		stringstream line( lineString );

		++currentLine;

		line >> sItem;

		if ( sItem == "program" )
		{
			string sKey, vertShader, fragShader;
			line >> sKey >> vertShader >> fragShader;

			// Create the one program we are going to use.
			recentProgram = new msShaderProgram(copyString(sKey), vertShader.c_str(), fragShader.c_str());
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

			recentProgram->addUniform(new msUniform(copyString(sKey), fVal));
		}
		else if (sItem == "uniform_vec2")
		{
			string sKey;
			GLfloat vVal[2];
			line >> sKey >> vVal[0] >> vVal[1];

			recentProgram->addUniform(new msUniform(copyString(sKey), vVal));
		}
		else if (sItem == "uniform_vec3")
		{
			string sKey;
			GLfloat vVal[3];
			line >> sKey >> vVal[0] >> vVal[1] >> vVal[2];

			recentProgram->addUniform(new msUniform(copyString(sKey), vVal));
		}
		else if ( sItem == "uniform_vec4" )
		{
			string sKey;
			GLfloat vVal[4];
			line >> sKey >> vVal[0] >> vVal[1] >> vVal[2] >> vVal[3]; 

			recentProgram->addUniform(new msUniform(copyString(sKey), vVal));
		}
		else if ( sItem == "uniform_mat4" )
		{
			string sKey;
			line >> sKey;

			GLfloat matrix[4][4];

			for (int i=0; i < 4; ++i)
				line >> matrix[i][0] >> matrix[i][1] >> matrix[i][2] >> matrix[i][3]; 

			recentProgram->addUniform(new msUniform(copyString(sKey), matrix));
		}
		else if (sItem == "attribute")
		{
			string sKey;
			GLuint location;

			line >> sKey >> location;

			recentProgram->addAttribute(new msAttribute(copyString(sKey), location));
		}
		else if (sItem == "texture")
		{
			string sKey, sVal;
			GLuint texUnit;
			line >> sKey >> texUnit >> sVal;

			recentProgram->addTexture(new msTexture(copyString(sKey), texUnit, sVal.c_str()));
		}
		else if (sItem == "nullTexture")
		{
			GLuint texUnit;
			string sKey;
			line >> sKey >> texUnit;

			recentProgram->addTexture(new msTexture(copyString(sKey), texUnit));
		}
		else if (sItem == "colorTexFbo")
		{
			GLuint texUnit;
			string sKey;
			line >> sKey >> texUnit;

			char *name = copyString(sKey);

			recentProgram->addFrameBuffer(new msFrameBuffer(name, new msTexture(name, texUnit)));
		}		
		else if ( sItem.empty() || sItem[0]=='/' )
		{
			// Either an empty line or a comment
		}
		else
		{
			cout << "Unsupported keyword in uniform file." << endl;
		}
	}
	return true;
}

msShaderProgram* msShaderPrograms::getProgramByName( const char *name )
{
	for (msShaderProgramIterator iterator = m_shaderPrograms.begin(); iterator != m_shaderPrograms.end() ; ++iterator)
	{
		msShaderProgram* current = *iterator;
		if(strcmp(current->getName(), name) == 0)
			return current;
	}	
	return NULL;
}


