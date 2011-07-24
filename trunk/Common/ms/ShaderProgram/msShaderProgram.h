#pragma once

#include "../msGL.h"
#include <vector>
#include <string>
#include "msUniform.h"
#include "msAttribute.h"
#include "msTexture.h"
#include "msFrameBuffer.h"
#include "../msCommon.h"

using namespace std;

class msAttribute;
class msUniform;
class msTexture;
class msFrameBuffer;

typedef std::vector<msUniform*> msUniformList;
typedef msUniformList::iterator msUniformIterator;

typedef std::vector<msAttribute*> msAttributeList;
typedef msAttributeList::iterator msAttributeIterator;

typedef std::vector<msTexture*> msTextureList;
typedef msTextureList::iterator msTextureIterator;

typedef std::vector<msFrameBuffer*> msFrameBufferList;
typedef msFrameBufferList::iterator msFrameBufferIterator;

class msShaderProgram
{
    const char *m_name;
    GLuint m_handle;
    GLuint m_vertexShaderHandle;
    GLuint m_fragmentShaderHandle;

    msUniformList m_uniforms;
    msAttributeList m_attributes;
    msTextureList m_textures;
    msFrameBufferList m_frameBuffers;

    bool loadShader( string &fileName, GLuint shaderHandle );

public:
    msShaderProgram(string &name, string &vertexShaderFileName, string &fragmentShaderFileName);
    ~msShaderProgram(void);

    GLuint getHandle();
    const char *getName();

    bool link();

    void linkAllDescriptors();

    void addUniform( msUniform *uniform );
    void addAttribute( msAttribute *attribute );
    void addTexture( msTexture *texture );
    void addFrameBuffer(msFrameBuffer *frameBuffer);

    msTexture* getTexture(const char *name);
    msUniform* getUniform(const char *name);
    msAttribute* getAttribute(const char *name);
    msFrameBuffer* getFrameBuffer(const char *name);

    void use();
    void notifyScreenChanged( GLint width, GLint height );
    void notifySizeChanged( GLint width, GLint height );
};
