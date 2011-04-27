#pragma once

#include <string>
#include <list>

using namespace std;

#ifdef IOS_GL

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#elif defined WINDOWS_GL

#include <GLES2/gl2.h>

#endif

void msGetExecutableDir(string &path);

void msMapDataFileName(string &fileName);

