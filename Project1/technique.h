#include <list>
#include <glew/glew.h>

class Technique
{
public:

    Technique();

    virtual ~Technique();

    virtual bool Init();

    void Enable();

protected:

    bool AddShader(GLenum ShaderType, const char* pFilename);

    bool Finalize();

    GLuint GetUniformLocation(const char* pUniformName);

    GLuint m_shaderProg;

private:

    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList m_shaderObjList;
};