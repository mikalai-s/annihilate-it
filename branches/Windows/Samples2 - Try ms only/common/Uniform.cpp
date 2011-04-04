//=================================================================================================================================
//
// Author: Maurice Ribble
//         3D Application Research Group
//         ATI Research, Inc.
//
// Implementation for the Uniform class.  This class loads textures, attributes, and uniforms.
//
//=================================================================================================================================
// $Id: //depot/3darg/Tools/Handheld/esTestApps/common/Uniform.cpp#1 $ 
// 
// Last check-in:  $DateTime: 2008/01/23 11:45:40 $ 
// Last edited by: $Author: dginsbur $
//=================================================================================================================================
//   (C) ATI Research, Inc. 2006 All rights reserved. 
//=================================================================================================================================

#include "uniform.h"

#include "SimpleImageLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <assert.h>

using namespace std;


//=================================================================================================================================
///
/// Loads all the shader objects.
///
/// \param fileName is the name for the file where we get objects
/// \param stateHandle is the ES handle to different types of shaders
///
/// \return bool saying whether we passed or failed
//=================================================================================================================================
bool Uniform::loadShaderObject( const char8* filename, GLuint shaderHandle )
{
   char8* source = NULL;

   {
      // Use file io to load the code of the shader.
      std::ifstream fp( filename , ios_base::binary );
      if( fp.fail() ) 
      {
         cout << "Failed to open shader file: ";
         cout << filename << endl;
         return false;
      }

      fp.seekg( 0, std::ios_base::end );
      const long len = fp.tellg();
      fp.seekg( 0, std::ios_base::beg );
      // The +1 is optional, depending on how you call glShaderSourceARB
      source = new char[len+1];
      fp.read(source, sizeof(char)*len);
      source[len] = NULL;
   }

   const char8* gls[1] = { source };

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
      cout << "Failed to compile shader:";
      cout << filename << endl;
      return false;
   }

   return true;
}



//=================================================================================================================================
///
/// Parses in text file with info about the textures/uniforms/attributes
///
/// \param fileName is the filename we are to parse
///
/// \return pass or fail boolean
//=================================================================================================================================
bool Uniform::parseUniforms(const char8* fileName)
{
   int currentLine = 0;

   m_uniforms.clear();
   m_unitID = 1;

   ifstream fin( fileName, std::ios_base::binary );

   if ( fin.fail() )
   {
      cout << "Failed to open uniform file: ";
      cout << fileName << endl;
      return false;
   }

   while ( ! fin.eof() )
   {
      UniformData *newUniform = new UniformData;
      string lineString;
      string sItem;
      bool addToVec = false;

      memset( newUniform, 0, sizeof(UniformData) );

      getline( fin, lineString );
      stringstream line( lineString );

      ++currentLine;

      line >> sItem;

      if ( sItem == "uniform_float" )
      {
         string sKey;
         float32 fVal;
         
         line >> sKey >> fVal;

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_FLOAT;
         newUniform->m_floatData[0] = fVal;
         newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str() );
         glUniform1f( newUniform->m_location, newUniform->m_floatData[0] );
      }
      else if (sItem == "uniform_vec2")
      {
         string sKey;
         float32 vVal[2];
         line >> sKey >> vVal[0] >> vVal[1];

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_FLOAT_VEC2;
         newUniform->m_floatData[0] = vVal[0];
         newUniform->m_floatData[1] = vVal[1];
         newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str() );
         glUniform2f( newUniform->m_location, newUniform->m_floatData[0], newUniform->m_floatData[1] );
      }
      else if (sItem == "uniform_vec3")
      {
         string sKey;
         float32 vVal[3];
         line >> sKey >> vVal[0] >> vVal[1] >> vVal[2];

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_FLOAT_VEC3;
         newUniform->m_floatData[0] = vVal[0];
         newUniform->m_floatData[1] = vVal[1];
         newUniform->m_floatData[2] = vVal[2];
         newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str() );
         glUniform3f( newUniform->m_location, newUniform->m_floatData[0], newUniform->m_floatData[1], newUniform->m_floatData[2] );
      }
      else if ( sItem == "uniform_vec4" )
      {
         string sKey;
         float32 vVal[4];
         line >> sKey >> vVal[0] >> vVal[1] >> vVal[2] >> vVal[3]; 

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_FLOAT_VEC4;
         newUniform->m_floatData[0] = vVal[0];
         newUniform->m_floatData[1] = vVal[1];
         newUniform->m_floatData[2] = vVal[2];
         newUniform->m_floatData[3] = vVal[3];
         newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str() );
         glUniform4f( newUniform->m_location, newUniform->m_floatData[0], newUniform->m_floatData[1],
                      newUniform->m_floatData[2], newUniform->m_floatData[3] );
      }
      else if ( sItem == "uniform_mat4" )
      {
         string sKey;
         line >> sKey;

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         for (int i=0; i<4; ++i)
         {
            float32 vVal[4];
            line >> vVal[0] >> vVal[1] >> vVal[2] >> vVal[3]; 
            newUniform->m_matrixData[i][0] = vVal[0];
            newUniform->m_matrixData[i][1] = vVal[1];
            newUniform->m_matrixData[i][2] = vVal[2];
            newUniform->m_matrixData[i][3] = vVal[3];
         }
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_FLOAT_MAT4;
         newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str() );
         glUniformMatrix4fv( newUniform->m_location, 1, GL_FALSE, &newUniform->m_matrixData[0][0] );
      }
      else if (sItem == "attribute")
      {
         string sKey;
         int32 vVal;

         line >> sKey >> vVal;
         glBindAttribLocation( m_recentProgramHandle, vVal, sKey.c_str() );		
         addToVec = false;
      }
      else if (sItem == "texture")
      {
         SimpleImageLoader imgLoader;

         ImageObject *pImgObj;
         uint32 texId;

         string sKey, sVal;
         int32 texUnit;
         line >> sKey >> texUnit >> sVal;

         pImgObj = imgLoader.LoadImageFile( sVal.c_str() );

         if ( pImgObj != NULL )
         {
            glGenTextures( 1, &texId );
            glActiveTexture( GL_TEXTURE0 + texUnit );
            glBindTexture( GL_TEXTURE_2D, texId );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

            if ( pImgObj->GetNumChannels() == 3 )
            {
               glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, pImgObj->GetWidth(), pImgObj->GetHeight(),
                             0, GL_RGB, GL_UNSIGNED_BYTE, pImgObj->GetDataPtr() );
            }
            else
            {
               assert( pImgObj->GetNumChannels() == 4 );
               glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pImgObj->GetWidth(), pImgObj->GetHeight(),
                             0, GL_RGBA, GL_UNSIGNED_BYTE, pImgObj->GetDataPtr() );
            }
            delete pImgObj;

            addToVec = true;
            newUniform->m_attachedProgram = m_recentProgramHandle;
            newUniform->m_name = new char [sKey.length()+1];
            strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
            newUniform->m_type = SAMPLE_INT;
            newUniform->m_textureHandle = texId;
            newUniform->m_textureUnit = texUnit;
            newUniform->m_location = glGetUniformLocation( m_recentProgramHandle, sKey.c_str() );			
            glUniform1i( newUniform->m_location, newUniform->m_textureUnit );
         }
         else
         {
            // Failed to load texture;
            newUniform->m_location = NULL;
            assert( 0 );
         }
      }
      else if ( sItem == "compressed_texture" )
      {
         string sCompressionType, sKey, sVal;
         int32 texUnit;
         line >> sCompressionType >> sKey >> texUnit >> sVal;

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_INT;
         newUniform->m_textureUnit = texUnit;

#ifndef USE_STANDARD_GL
         newUniform->m_textureHandle = LoadCompressedTexture( sVal.c_str(), sCompressionType.c_str(), texUnit );
#endif // USE_STANDARD_GL
         newUniform->m_location = glGetUniformLocation(m_recentProgramHandle,sKey.c_str());			
         glUniform1i( newUniform->m_location, newUniform->m_textureUnit );
      }
      else if (sItem == "genTexture")
      {
         GLuint texId;
         int32 texUnit;
         string sKey;
         line >> sKey >> texUnit;

         addToVec = true;
         newUniform->m_attachedProgram = m_recentProgramHandle;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_INT;
         newUniform->m_textureUnit = texUnit;
         glGenTextures( 1, &texId );
         newUniform->m_textureHandle = texId;
         newUniform->m_location = glGetUniformLocation(m_recentProgramHandle,sKey.c_str());			
         glUniform1i( newUniform->m_location, newUniform->m_textureUnit );
      }
      else if (sItem == "nullTexture")
      {
         GLuint texId;
         int32 texUnit;
         string sKey;
         line >> sKey >> texUnit;

         addToVec = true;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_INT;
         newUniform->m_textureUnit = texUnit;
         glGenTextures( 1, &texId );
         newUniform->m_textureHandle = texId;

         glActiveTexture( GL_TEXTURE0 + texUnit );
         glBindTexture( GL_TEXTURE_2D, texId );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      }
      else if (sItem == "colorTexFbo")
      {
         GLuint texId, fboId;
         int32 texUnit;
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
         glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      }
      else if ( sItem == "depthTexFbo" )
      {
         GLuint texId, fboId;
         int32 texUnit;
         string sKey;
         line >> sKey >> texUnit;

         addToVec = true;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_INT;
         newUniform->m_textureUnit = texUnit;
         newUniform->m_depthTex = true;
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

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }
      else if ( sItem == "program" )
      {
         string sKey, vertShader, fragShader;
         line >> sKey >> vertShader >> fragShader;

         addToVec = true;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_PROGRAM;

         // Create the one program we are going to use.
         newUniform->m_progHandle = glCreateProgram();
         m_recentProgramHandle = newUniform->m_progHandle;

         // Create one shader of object of each type.
         newUniform->m_vertShaderHandle = glCreateShader( GL_VERTEX_SHADER );
         newUniform->m_fragShaderHandle = glCreateShader( GL_FRAGMENT_SHADER );

         loadShaderObject( vertShader.c_str(), newUniform->m_vertShaderHandle );
         loadShaderObject( fragShader.c_str(), newUniform->m_fragShaderHandle );

         GLint compileVertResult = 0;
         GLint compileFragResult = 0;
         glGetShaderiv( newUniform->m_vertShaderHandle, GL_COMPILE_STATUS, &compileVertResult );
         glGetShaderiv( newUniform->m_fragShaderHandle, GL_COMPILE_STATUS, &compileFragResult );
         if ( compileVertResult == 0 || compileFragResult == 0 )
         {
            char8 *log = (char8*)malloc(1000);
            GLint length2;
            if ( compileVertResult == 0 )
            {
               cout << "Failed to compile vert shader." << endl;
               glGetShaderInfoLog( newUniform->m_vertShaderHandle, 1000, &length2, log );
               cout << log << endl;
            }
            if ( compileFragResult == 0 )
            {
               cout << "Failed to compile frag shader." << endl;
               glGetShaderInfoLog( newUniform->m_fragShaderHandle, 1000, &length2, log );
               cout << log << endl;
            }
            free(log);
         }

         // Attach them to the program.
         glAttachShader( newUniform->m_progHandle, newUniform->m_vertShaderHandle );
         glAttachShader( newUniform->m_progHandle, newUniform->m_fragShaderHandle );
      }
      else if (sItem == "binaryProgram" )
      {
         string sKey, filename;
         line >> sKey >> filename;

         addToVec = true;
         newUniform->m_name = new char [sKey.length()+1];
         strcpy_s( newUniform->m_name, sKey.length()+1, sKey.c_str());
         newUniform->m_type = SAMPLE_PROGRAM;

         char8* source = NULL;
         long len;
         {
            // Use file io to load the code of the shader.
            std::ifstream fp( filename.c_str() , ios_base::binary );
            if( fp.fail() ) 
            {
               cout << "Failed to open shader file: ";
               cout << filename << endl;
               return false;
            }

            fp.seekg( 0, std::ios_base::end );
            len = fp.tellg();
            fp.seekg( 0, std::ios_base::beg );

            source = new char[len];
            fp.read(source, sizeof(char)*len);
         }

         // Create the one program we are going to use.
         newUniform->m_progHandle = glCreateProgram();
         m_recentProgramHandle = newUniform->m_progHandle;

         // Create one shader of object of each type.
         newUniform->m_vertShaderHandle = glCreateShader( GL_VERTEX_SHADER );
         newUniform->m_fragShaderHandle = glCreateShader( GL_FRAGMENT_SHADER );

#ifndef USE_STANDARD_GL
         GLuint shaders[2] = { newUniform->m_vertShaderHandle, newUniform->m_fragShaderHandle };
         glShaderBinary( 2, shaders, GL_Z400_BINARY_AMD, source, len );         
#endif // USE_STANDARD_GL

         // Attach them to the program.
         glAttachShader( newUniform->m_progHandle, newUniform->m_vertShaderHandle );
         glAttachShader( newUniform->m_progHandle, newUniform->m_fragShaderHandle );

         delete[] source;
      }
      else if ( sItem == "link" )
      {
         string progName;
         line >> progName;

         addToVec = false;
         GLuint progHandle = getProgramHandle( progName.c_str() );

         // Link the whole program together.
         glLinkProgram( progHandle );

         // Check for link success
         GLint linkResult = 0;
         glGetProgramiv( progHandle, GL_LINK_STATUS, &linkResult );
         glUseProgram( progHandle );
         updateAllUniforms( progHandle );
         if ( linkResult == 0 )
         {
            char8 log[1000];
            GLint length;
            glGetProgramInfoLog( progHandle, 1000, &length, log );
            cout << "Failed to link program object.";
            cout << log << endl;
            return false;
         }
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
         assert(0);
      }

      if ( addToVec )	
      {
         m_uniforms.push_back(newUniform);			
      }
      else
      {
         delete newUniform;
      }
   }
   return true;
}

//=================================================================================================================================
///
/// Updates the value of a uniform
///
/// \param name - The name we gave to the uniform
/// \param vals - An array of values we want to to replace the current uniform values with
///
/// \return true=pass ... false=fail
//=================================================================================================================================
bool Uniform::updateOneUniform( const char8* name, float32* vals )
{
   bool updated = false;
   UniformDataIterator index;

   for ( index = m_uniforms.begin(); index != m_uniforms.end() ; ++index)
   {
      UniformData* current = *index;

      if ( strcmp( name, current->m_name ) == 0 )
      {
         updated = true;

         switch( current->m_type )
         {
         case SAMPLE_FLOAT:
            memcpy( current->m_floatData, vals, sizeof(float32) * 1 );
            glUniform1f( current->m_location, current->m_floatData[0] );
            break;
         case SAMPLE_FLOAT_VEC2:
            memcpy( current->m_floatData, vals, sizeof(float32) * 2 );
            glUniform2f( current->m_location, current->m_floatData[0], current->m_floatData[1] );
            break;
         case SAMPLE_FLOAT_VEC3:
            memcpy( current->m_floatData, vals, sizeof(float32) * 3 );
            glUniform3f( current->m_location, current->m_floatData[0], current->m_floatData[1], current->m_floatData[2] );
            break;
         case SAMPLE_FLOAT_VEC4:
            memcpy( current->m_floatData, vals, sizeof(float32) * 4 );
            glUniform4f( current->m_location, current->m_floatData[0], current->m_floatData[1], current->m_floatData[2], current->m_floatData[3] );
            break;
         case SAMPLE_FLOAT_MAT4:
            memcpy( current->m_matrixData, vals, sizeof(float32) * 16 );
            glUniformMatrix4fv( current->m_location, 1, GL_FALSE, &current->m_matrixData[0][0] );
            break;
         default:
            assert(0);
            break;
         }
      }

      if ( updated )
      {
         break;
      }
   }
   
   assert( updated == true ); // They probably passed in un unsupported type or invalid name
   return updated;
}

//=================================================================================================================================
///
/// Gets the program handle
///
/// \param name - The name we gave to the program
///
/// \return true=pass ... false=fail
//=================================================================================================================================
GLuint Uniform::getProgramHandle( const char8* name )
{
   bool updated = false;
   GLuint ret = 0;
   UniformDataIterator index;

   for ( index = m_uniforms.begin(); index != m_uniforms.end() ; ++index)
   {
      UniformData* current = *index;

      if ( strcmp( name, current->m_name ) == 0 )
      {
         updated = true;

         switch( current->m_type )
         {
         case SAMPLE_PROGRAM:
            ret = current->m_progHandle;
            break;
         default:
            assert(0);
            break;
         }
      }

      if ( updated )
      {
         break;
      }
   }
   
   assert( updated == true ); // They probably passed in un unsupported type or invalid name
   return ret;
}

//=================================================================================================================================
///
/// Binds a texture in GL
///
/// \param name - The name we gave to the texture
/// \param width - The width of the texture
/// \param height - The height of the texture
///
/// \return true=pass ... false=fail
//=================================================================================================================================

int Uniform::getTextureId(const char* name)
{
	UniformDataIterator index;
	for ( index = m_uniforms.begin(); index != m_uniforms.end() ; ++index)
	{
		UniformData* current = *index;

		if ( strcmp( name, current->m_name ) == 0 )
		{
			return current->m_textureHandle;
		}
	}
	return -1;
}

int Uniform::getTextureUnit(const char* name)
{
	UniformDataIterator index;
	for ( index = m_uniforms.begin(); index != m_uniforms.end() ; ++index)
	{
		UniformData* current = *index;

		if ( strcmp( name, current->m_name ) == 0 )
		{
			return current->m_textureUnit;
		}
	}
	return -1;
}

bool Uniform::bindTexture( const char8* name, int32 width, int32 height )
{
   bool updated = false;
   UniformDataIterator index;

   for ( index = m_uniforms.begin(); index != m_uniforms.end() ; ++index)
   {
      UniformData* current = *index;

      if ( strcmp( name, current->m_name ) == 0 )
      {
         if ( current->m_type == SAMPLE_INT && current->m_textureHandle )
         {
            updated = true;
         }
         break;
      }
   }
   
   if ( updated )
   {
      GLint curProg, uniformLocation;
      glGetIntegerv( GL_CURRENT_PROGRAM, &curProg );
      uniformLocation = glGetUniformLocation( curProg, (*index)->m_name );
      glUniform1i( uniformLocation, (*index)->m_textureUnit );
      glActiveTexture( GL_TEXTURE0 + (*index)->m_textureUnit );
      glBindTexture( GL_TEXTURE_2D, (*index)->m_textureHandle );

      if ((*index)->m_width != width || (*index)->m_height != height )
      {
         if ( width != 0 && height != 0 ) // If these are 0 then we just want to keep the current size
         {
            (*index)->m_width = width;
            (*index)->m_height = height;
            if ( (*index)->m_depthTex )
            {
               glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
            }
            else
            {
               glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
            }
         }
      }
   }
   else
   {
      assert( 0 ); // They probably passed in an a non-texture type or an invalid name
   }
   return updated;
}

//=================================================================================================================================
///
/// Binds an FBO in GL
///
/// \param name - The name of the fbo
/// \param width - The width of the fbo
/// \param height - The height of the fbo
///
/// \return true=pass ... false=fail
//=================================================================================================================================
bool Uniform::bindFbo( const char8* name, int32 width, int32 height )
{
   bool updated = false;
   UniformDataIterator index;

   for ( index = m_uniforms.begin(); index != m_uniforms.end() ; ++index)
   {
      UniformData* current = *index;

      if ( strcmp( name, current->m_name ) == 0 )
      {
         if ( current->m_type == SAMPLE_INT && current->m_textureHandle && current->m_fboHandle )
         {
            updated = true;
         }
         break;
      }
   }
   
   if ( updated )
   {
#ifdef USE_STANDARD_GL
      glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, (*index)->m_fboHandle );
#else
      glBindFramebuffer( GL_FRAMEBUFFER, (*index)->m_fboHandle );
#endif // USE_STANDARD_GL

      if ((*index)->m_width != width || (*index)->m_height != height )
      {
         GLint curTexture;
         glGetIntegerv( GL_TEXTURE_BINDING_2D, &curTexture );

         (*index)->m_width = width;
         (*index)->m_height = height;
         glBindTexture( GL_TEXTURE_2D, (*index)->m_textureHandle );
         if ( (*index)->m_depthTex )
         {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
         }
         else
         {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
         }

         glBindTexture( GL_TEXTURE_2D, curTexture );

#ifdef USE_STANDARD_GL
         if ( (*index)->m_depthTex )
         {
            if (!(*index)->m_colorRbHandle)
            {
               glGenRenderbuffersEXT( 1, &(*index)->m_colorRbHandle );
            }
            glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, (*index)->m_colorRbHandle );
            glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_RGB, width, height );
            glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, (*index)->m_colorRbHandle );
            glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, (*index)->m_textureHandle, 0 );
         }
         else
         {
            glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, (*index)->m_textureHandle, 0 );
         }
#else
         if ( (*index)->m_depthTex )
         {
            if(!(*index)->m_colorRbHandle)
            {
               glGenRenderbuffers( 1, &(*index)->m_colorRbHandle );
            }
            glBindRenderbuffer( GL_RENDERBUFFER, (*index)->m_colorRbHandle );
            glRenderbufferStorage( GL_RENDERBUFFER, GL_RGB, width, height );
            glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, (*index)->m_colorRbHandle );
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, (*index)->m_textureHandle, 0 );
         }
         else
         {
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (*index)->m_textureHandle, 0 );
         }
#endif // USE_STANDARD_GL
      }
   }
   else
   {
      assert( 0 ); // They probably passed in an a non-texture type or an invalid name
   }
   return updated;
}


//=================================================================================================================================
///
/// Deletes all the GL resources that we have loaded
///
/// \param name - The name we gave to the program
///
/// \return true=pass ... false=fail
//=================================================================================================================================
void Uniform::freeAllData()
{
   UniformDataIterator index;

   for ( index = m_uniforms.begin(); index != m_uniforms.end() ; ++index)
   {
      if ( (*index)->m_textureHandle )
      {
         glDeleteTextures(1,&((*index)->m_textureHandle)); 
      }
      if ( (*index)->m_vertShaderHandle )
      {
         glDeleteShader( (*index)->m_vertShaderHandle );
      }
      if ( (*index)->m_fragShaderHandle )
      {
         glDeleteShader( (*index)->m_fragShaderHandle );
      }
      if ( (*index)->m_progHandle )
      {
         glDeleteProgram( (*index)->m_progHandle );
      }

#ifdef USE_STANDARD_GL
      if ( (*index)->m_fboHandle )
      {
         glDeleteFramebuffersEXT(1, &(*index)->m_fboHandle );
      }
      if ( (*index)->m_colorRbHandle )
      {
         glDeleteRenderbuffersEXT(1, &(*index)->m_colorRbHandle );
      }
#else
      if ( (*index)->m_fboHandle )
      {
         glDeleteFramebuffers(1, &(*index)->m_fboHandle );
      }
      if ( (*index)->m_colorRbHandle )
      {
         glDeleteRenderbuffers(1, &(*index)->m_colorRbHandle );
      }
#endif // USE_STANDARD_GL
   }

   for ( uint32 i = 0; i < m_uniforms.size() ; i++ )
   {
      if ( m_uniforms[i] != NULL )
      {
         delete m_uniforms[i];
      }
   }
   m_uniforms.clear();
}

//=================================================================================================================================
///
/// Updates all the uniform data after a link
///
/// \param void
///
/// \return void
//=================================================================================================================================
void Uniform::updateAllUniforms( GLuint curProg )
{
   UniformDataIterator index;

   for ( index = m_uniforms.begin(); index != m_uniforms.end() ; ++index)
   {
      if ( curProg == (*index)->m_attachedProgram )
      {
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
      }
   }
}