#include "msShaderPrograms.h"

using namespace std;

msShaderPrograms::msShaderPrograms(void)
{
	m_mainFrameBuffer = new msFrameBuffer(0);
}

msShaderPrograms::~msShaderPrograms(void)
{
	for(msShaderProgramIterator i = m_shaderPrograms.begin(); i != m_shaderPrograms.end(); i ++)
		delete (*i);
	delete m_mainFrameBuffer;
}

bool msShaderPrograms::loadFile(string fileName )
{
	int currentLine = 0;
	msShaderProgram *recentProgram;

	m_shaderPrograms.clear();

	ifstream fin( fileName.c_str(), std::ios_base::binary );

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
            msMapDataFileName(vertShader);
            msMapDataFileName(fragShader);
			recentProgram = new msShaderProgram(sKey, vertShader, fragShader);
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

			recentProgram->addUniform(new msUniform(sKey, fVal));
		}
		else if (sItem == "uniform_vec2")
		{
			string sKey;
			GLfloat vVal[2];
			line >> sKey >> vVal[0] >> vVal[1];

			recentProgram->addUniform(new msUniform(sKey, vVal));
		}
		else if (sItem == "uniform_vec3")
		{
			string sKey;
			GLfloat vVal[3];
			line >> sKey >> vVal[0] >> vVal[1] >> vVal[2];

			recentProgram->addUniform(new msUniform(sKey, vVal));
		}
		else if ( sItem == "uniform_vec4" )
		{
			string sKey;
			GLfloat vVal[4];
			line >> sKey >> vVal[0] >> vVal[1] >> vVal[2] >> vVal[3]; 

			recentProgram->addUniform(new msUniform(sKey, vVal));
		}
		else if ( sItem == "uniform_mat4" )
		{
			string sKey;
			line >> sKey;

			GLfloat matrix[4][4];

			for (int i=0; i < 4; ++i)
				line >> matrix[i][0] >> matrix[i][1] >> matrix[i][2] >> matrix[i][3]; 

			recentProgram->addUniform(new msUniform(sKey, matrix));
		}
		else if (sItem == "attribute")
		{
			string sKey;
			GLuint location;

			line >> sKey >> location;

			recentProgram->addAttribute(new msAttribute(sKey, location));
		}
		else if (sItem == "texture")
		{
			string sKey, sVal;
			GLuint texUnit;
			line >> sKey >> texUnit >> sVal;

            msMapDataFileName(sVal);
			recentProgram->addTexture(new msTexture(sKey, texUnit, sVal));
		}
		else if (sItem == "nullTexture")
		{
			GLuint texUnit;
			string sKey;
			line >> sKey >> texUnit;

			recentProgram->addTexture(new msTexture(sKey, texUnit));
		}
		else if (sItem == "colorTexFbo")
		{
			GLuint texUnit;
			string sKey;
			line >> sKey >> texUnit;

			recentProgram->addFrameBuffer(new msFrameBuffer(sKey, texUnit));
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

msFrameBuffer* msShaderPrograms::getMainFrameBuffer()
{
	return m_mainFrameBuffer;
}

void msShaderPrograms::setMainFrameBuffer(GLint frameBufferId)
{
	if(m_mainFrameBuffer->m_id != frameBufferId)
	{
		delete m_mainFrameBuffer;
		m_mainFrameBuffer = new msFrameBuffer(frameBufferId);
	}
}

void msShaderPrograms::notifySizeChanged( GLint width, GLint height )
{
	for (msShaderProgramIterator i = m_shaderPrograms.begin(); i != m_shaderPrograms.end() ; i++)
		(*i)->notifySizeChanged(width, height);
}


