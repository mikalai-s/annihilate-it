#include "msGL.h"


#ifdef IOS_GL

void msGetExecutableDir(char *buffer, int length)
{

}

#elif defined WINDOWS_GL

void msGetExecutableDir(char *buffer, int length)
{
    GetModuleFileName(0, buffer, length);
}

#endif

// modifies given fileName string to contains full path
void msMapDataFileName(string &fileName)
{
    char buf[MAX_PATH];
    msGetExecutableDir(buf, MAX_PATH);

    // Extract directory
    string path = buf;
    path = path.substr(0, path.rfind("\\"));

    fileName = path.append(fileName);
}


