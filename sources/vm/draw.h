#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include <GLAD/glad.h>
#include "vm.h"


class CDQuads
{
public:
    CDQuads()
    {
        GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshader, 1, &vsh, 0);
        glCompileShader(vshader);

        GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshader, 1, &fsh, 0);
        glCompileShader(fshader);

        GLuint gprogram = glCreateProgram();
        glAttachShader(gprogram, vshader);
        glAttachShader(gprogram, fshader);
        glLinkProgram(gprogram);

        int xsize = 0, ysize = 0;
        for (int j = 0; j < 32; j++)
        {
            xsize = 0;
            for (int i = 0; i < 64; i++)
            {

                glm::vec4 calculatedPos0 = glm::vec4(glm::vec3(vertices[0] + xsize, vertices[1] + ysize, vertices[2]), 1.0f);
                glm::vec4 calculatedPos1 = glm::vec4(glm::vec3(vertices[3] + xsize, vertices[4] + ysize, vertices[5]), 1.0f);
                glm::vec4 calculatedPos2 = glm::vec4(glm::vec3(vertices[6] + xsize, vertices[7] + ysize, vertices[8]), 1.0f);
                glm::vec4 calculatedPos3 = glm::vec4(glm::vec3(vertices[9] + xsize, vertices[10] + ysize, vertices[11]), 1.0f);

                verticies.push_back(calculatedPos0.x);
                verticies.push_back(calculatedPos0.y);
                verticies.push_back(calculatedPos0.z);


                verticies.push_back(calculatedPos1.x);
                verticies.push_back(calculatedPos1.y);
                verticies.push_back(calculatedPos1.z);



                verticies.push_back(calculatedPos2.x);
                verticies.push_back(calculatedPos2.y);
                verticies.push_back(calculatedPos2.z);



                verticies.push_back(calculatedPos3.x);
                verticies.push_back(calculatedPos3.y);
                verticies.push_back(calculatedPos3.z);

                verticies.push_back(calculatedPos0.x);
                verticies.push_back(calculatedPos0.y);
                verticies.push_back(calculatedPos0.z);

                verticies.push_back(calculatedPos2.x);
                verticies.push_back(calculatedPos2.y);
                verticies.push_back(calculatedPos2.z);





                xsize += 1.0;
            }
            ysize += 1.0;
        }


        for (int i = 0; i < verticies.size(); i++)
        {
            colours.push_back(0.0f);
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_DYNAMIC_DRAW);

        glGenBuffers(1, &colours_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
        glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(float), &colours[0], GL_DYNAMIC_DRAW);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        glUseProgram(gprogram);
        GLint uniloc = glGetUniformLocation(gprogram, "projection");
        glUniformMatrix4fv(uniloc, 1, GL_FALSE, glm::value_ptr(proj));
    }

    void update(VM& VM_CHIP)
    {
        int counter = 0;
        for (int i = 0; i < colours.size(); i += 18)
        {

            if (VM_CHIP.gfx[counter] == 0)
            {
                colours[i] = 0.0f;
                colours[i + 1] = 0.0f;
                colours[i + 2] = 0.0f;
                
                colours[i + 3] = 0.0f;
                colours[i + 4] = 0.0f;
                colours[i + 5] = 0.0f;
                
                colours[i + 6] = 0.0f;
                colours[i + 7] = 0.0f;
                colours[i + 8] = 0.0f;
                
                colours[i + 9] = 0.0f;
                colours[i + 10] = 0.0f;
                colours[i + 11] = 0.0f;
                
                colours[i + 12] = 0.0f;
                colours[i + 13] = 0.0f;
                colours[i + 14] = 0.0f;
               
                colours[i + 15] = 0.0f;
                colours[i + 16] = 0.0f;
                colours[i + 17] = 0.0f;


            }
            else
            {
                colours[i] = 1.0f;
                colours[i + 1] = 1.0f;
                colours[i + 2] = 1.0f;
                
                colours[i + 3] = 1.0f;
                colours[i + 4] = 1.0f;
                colours[i + 5] = 1.0f;
                
                colours[i + 6] = 1.0f;
                colours[i + 7] = 1.0f;
                colours[i + 8] = 1.0f;
                
                colours[i + 9] = 1.0f;
                colours[i + 10] = 1.0f;
                colours[i + 11] = 1.0f;
                
                colours[i + 12] = 1.0f;
                colours[i + 13] = 1.0f;
                colours[i + 14] = 1.0f;
                
                colours[i + 15] = 1.0f;
                colours[i + 16] = 1.0f;
                colours[i + 17] = 1.0f;



            }

            counter++;
        }

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, colours.size() * sizeof(float), &colours[0]);
        glDrawArrays(GL_TRIANGLES, 0, verticies.size() / 3);
    }

    void update(uint8_t* buffer)
    {
        int counter = 0;
        for (int i = 0; i < colours.size(); i += 18)
        {

            if (buffer[counter] == 0)
            {
                colours[i] = 0.0f;
                colours[i + 1] = 0.0f;
                colours[i + 2] = 0.0f;

                colours[i + 3] = 0.0f;
                colours[i + 4] = 0.0f;
                colours[i + 5] = 0.0f;

                colours[i + 6] = 0.0f;
                colours[i + 7] = 0.0f;
                colours[i + 8] = 0.0f;

                colours[i + 9] = 0.0f;
                colours[i + 10] = 0.0f;
                colours[i + 11] = 0.0f;

                colours[i + 12] = 0.0f;
                colours[i + 13] = 0.0f;
                colours[i + 14] = 0.0f;

                colours[i + 15] = 0.0f;
                colours[i + 16] = 0.0f;
                colours[i + 17] = 0.0f;


            }
            else
            {
                colours[i] = 1.0f;
                colours[i + 1] = 1.0f;
                colours[i + 2] = 1.0f;

                colours[i + 3] = 1.0f;
                colours[i + 4] = 1.0f;
                colours[i + 5] = 1.0f;

                colours[i + 6] = 1.0f;
                colours[i + 7] = 1.0f;
                colours[i + 8] = 1.0f;

                colours[i + 9] = 1.0f;
                colours[i + 10] = 1.0f;
                colours[i + 11] = 1.0f;

                colours[i + 12] = 1.0f;
                colours[i + 13] = 1.0f;
                colours[i + 14] = 1.0f;

                colours[i + 15] = 1.0f;
                colours[i + 16] = 1.0f;
                colours[i + 17] = 1.0f;



            }

            counter++;
        }

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, colours.size() * sizeof(float), &colours[0]);
        glDrawArrays(GL_TRIANGLES, 0, verticies.size() / 3);
    }

private:
    GLuint vshader;
    GLuint fshader;

    const char* vsh = "#version 330\n"
        "layout (location = 0) in vec3 aPos;"
        "layout(location = 1) in vec3 vcolour;"
        "out vec3 Vcolor;"
        "uniform mat4 projection;"

        "void main() { gl_Position = projection * vec4(aPos, 1.0); Vcolor = vcolour;}\0";

    const char* fsh = "#version 330 core\n"
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