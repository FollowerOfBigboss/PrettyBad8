#ifndef EMU_RENDER
#define EMU_RENDER

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include <GLAD/glad.h>
#include "vm.h"


class CRenderQuads
{
public:
    void init();
    void update(VM& vm);
    void update(uint8_t* buffer);

private:
    GLuint vshader;
    GLuint fshader;

    const char* vsh = 
        "#version 330\n"
        "layout (location = 0) in vec3 aPos;"
        "layout(location = 1) in vec3 vcolour;"
        "out vec3 Vcolor;"
        "uniform mat4 projection;"
        "void main() { gl_Position = projection * vec4(aPos, 1.0); Vcolor = vcolour;}\0";

    const char* fsh = 
        "#version 330 core\n"
        "in vec3 Vcolor;"
        "out vec4 FragColor;\n"
        "void main() { FragColor = vec4(Vcolor, 0.0f); }\0";

    const GLfloat vertices[12] = {
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f
    };

    const glm::mat4 proj = glm::ortho(0.0f, 64.0f, 32.0f, 0.0f);

    std::vector<GLfloat> verticies;
    std::vector <GLfloat> colours;
    
    GLuint VBO, VAO;
    GLuint colours_vbo;

};

#endif