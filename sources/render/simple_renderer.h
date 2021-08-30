#ifndef EMU_RENDER
#define EMU_RENDER

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include <GLAD/glad.h>
#include "../vm/vm.h"


class CRenderQuads
{
public:
    void init();
    void update(VM& vm);

private:
    GLuint vshader;
    GLuint fshader;

    const char* vsh = "#version 330\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 projection;\n"
        "uniform int xloc;\n"
        "uniform int yloc;\n"
        "void main() { gl_Position = projection * vec4(aPos.x + xloc, aPos.y + yloc, 1.0, 1.0); }\0";

    const char* fsh = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() { FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); }\0";

    GLuint VBO, VAO, EBO;
    GLuint gprogram;
    GLint xloc;
    GLint yloc;
    glm::mat4 proj;
    
    GLfloat vertices[12] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
    };

    GLuint indicies[6] = {
        0, 1, 2,
        0, 3, 2
    };
};

#endif