#include "simple_renderer.h"


void CRenderQuads::init()
{
    vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vsh, 0);
    glCompileShader(vshader);

    fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fsh, 0);
    glCompileShader(fshader);


    gprogram = glCreateProgram();
    glAttachShader(gprogram, vshader);
    glAttachShader(gprogram, fshader);
    glLinkProgram(gprogram);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glUseProgram(gprogram);
    GLint uniloc = glGetUniformLocation(gprogram, "projection");
    xloc = glGetUniformLocation(gprogram, "xloc");
    yloc = glGetUniformLocation(gprogram, "yloc");

    proj = glm::ortho(0.0f, 64.0f, 32.0f, 0.0f);
    glUniformMatrix4fv(uniloc, 1, GL_FALSE, glm::value_ptr(proj));
}

void CRenderQuads::update(VM& vm)
{
    int xsize = 0, ysize = 0;
    for (int j = 0; j < 32; j++)
    {
        xsize = 0;
        for (int i = 0; i < 64; i++)
        {
            glUseProgram(gprogram);
            glUniform1i(xloc, xsize);
            glUniform1i(yloc, ysize);

            
            int calc = (j * 64) + i;

            if (vm.gfx[calc] == 1)
            {
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }

            xsize += 1.0;
        }
        ysize += 1.0;
    }
}
