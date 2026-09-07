#ifndef GLRENDERER_UTIL_H
#define GLRENDERER_UTIL_H

#ifndef _WIN64
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <assert.h>
#include<glm/glm.hpp>
#include<GLFW/glfw3.h>
#include <assimp/postprocess.h>

using namespace std;

bool ReadFile(const char* fileName, string& outFile);
char* ReadBinaryFile(const char* pFileName, int& size);

void GLRendererError(const char* pFileName, GLuint line, const char* msg, ...);
void GLRendererFileError(const char* pFileName, GLuint line, const char* pFileError);

#define GLRENDERER_ERROR0(msg) GLRendererError(__FILE__, __LINE__, msg)
#define GLRENDERER_ERROR(msg, ...) GLRendererError(__FILE__, __LINE__, msg, __VA_ARGS__)
#define GLRENDERER_FILE_ERROR(FileError) GLRendererFileError(__FILE__, __LINE__, FileError);

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ZERO_MEM_VAR(var) memset(&var, 0, sizeof(var))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifdef _WIN64
#define SNPRINTF _snprintf_s
#define VSNPRINTF vsnprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#else
#define SNPRINTF snprintf
#define VSNPRINTF vsnprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#endif

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define GLExitIfError                                                          \
{                                                                               \
    GLenum Error = glGetError();                                                \
                                                                                \
    if (Error != GL_NO_ERROR) {                                                 \
        printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, Error);     \
        exit(0);                                                                \
    }                                                                           \
}

#define GLCheck(x)                             \
do {                                           \
    x;                                         \
    GLenum err = glGetError();                 \
    if (err != GL_NO_ERROR) {                  \
        printf("%s -> 0x%x\n", #x, err);       \
        __debugbreak();                        \
    }                                          \
} while(0)

#define GLCheckError() (glGetError() == GL_NO_ERROR)

long long GetCurrentTimeMillis();


#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace)

#endif 