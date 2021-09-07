
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <complex>
#include "src/shader/Shader.h"
#include "src/data/buffer/VBO.h"
#include "src/data/buffer/EBO.h"
#include "src/data/VAO.h"
#include "include/stb/stb_image.h"
#include "src/data/texture/Texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "src/spatial/camera/Camera.h"


const int width = 1920;
const int height = 1080;

GLfloat vertices[] =
        { //     COORDINATES     /        COLORS          /    TEX COORDS   /        NORMALS       //
                -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
                -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
                0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	     5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
                0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	     5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

                -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
                -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
                0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	     2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

                -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
                0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	     0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
                0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	     2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

                0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
                0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
                0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

                0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
                -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
                0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
        };

// Indices for vertices order
GLuint indices[] =
        {
                0, 1, 2, // Bottom side
                0, 2, 3, // Bottom side
                4, 6, 5, // Left side
                7, 9, 8, // Non-facing side
                10, 12, 11, // Right side
                13, 15, 14 // Facing side
        };


GLfloat lightVertices[] =
        { //     COORDINATES     //
                -0.1f, -0.1f,  0.1f,
                -0.1f, -0.1f, -0.1f,
                0.1f, -0.1f, -0.1f,
                0.1f, -0.1f,  0.1f,
                -0.1f,  0.1f,  0.1f,
                -0.1f,  0.1f, -0.1f,
                0.1f,  0.1f, -0.1f,
                0.1f,  0.1f,  0.1f
        };

GLuint lightIndices[] =
        {
                0, 1, 2,
                0, 2, 3,
                0, 4, 7,
                0, 7, 3,
                3, 7, 6,
                3, 6, 2,
                2, 6, 5,
                2, 5, 1,
                1, 5, 4,
                1, 4, 0,
                4, 5, 6,
                4, 6, 7
        };


class App {

    GLFWwindow* window;
public:
    App(){

        //initialize glfw, specify version/ opengl profile
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //initialize window and some error handling, make the window active
        this->window = glfwCreateWindow(width,height, "U", nullptr, nullptr);
        if(window == nullptr){
            std::cout << "[ERROR] Cannot create window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);

        //initialize glad
        gladLoadGL();

        //tell opengl the size of render area
        glViewport(0,0,width,height);

        //specify how opengl draws polygons GL_FILL or GL_LINE
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glEnable(GL_DEPTH_TEST);

    }

    void processInput(){
        if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(this->window,true);
        }
    }


    void run(){
        //init camera
        Camera camera(width, height,{0.0f,0.0f,2.0f});



        //shader and data initialization
        auto sh = Shader("../src/shader/shaders/default.vert","../src/shader/shaders/default.frag");

        //create vao and bind it
        auto vao = VAO();

        vao.Bind();

        //create vbo and ebo for vao
        auto vbo = VBO(vertices,sizeof(vertices));
        auto ebo = EBO(indices, sizeof(indices));

        //link vbo attributes to vao
        vao.LinkAttribs(vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)nullptr);
        vao.LinkAttribs(vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*) (3 * sizeof(float))); //set it to layout 1 to access it in shader
        vao.LinkAttribs(vbo, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*) (6 * sizeof(float)));
        vao.LinkAttribs(vbo, 3, 3 , GL_FLOAT, 11 * sizeof(float), (void*) (8 * sizeof(float)));

        vao.Unbind();
        vbo.Unbind();
        ebo.Unbind();

        Shader lightShader("../src/shader/shaders/light.vert", "../src/shader/shaders/light.frag");

        auto lightVAO = VAO();
        lightVAO.Bind();
        VBO lightVBO(lightVertices, sizeof(lightVertices));
        EBO lightEBO(lightIndices, sizeof(lightIndices));

        lightVAO.LinkAttribs(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)nullptr);
        lightVAO.Unbind();
        lightVBO.Unbind();
        lightEBO.Unbind();

        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        glm::vec3 lightPos = {0.5f, 0.5f, 0.5f};
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);

        glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat4 pyramidModel = glm::mat4(1.0f);
        pyramidModel = glm::translate(pyramidModel, pyramidPos);

        lightShader.Activate();
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
        glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x,lightColor.y,lightColor.z, lightColor.w);
        sh.Activate();
        glUniformMatrix4fv(glGetUniformLocation(sh.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
        glUniform4f(glGetUniformLocation(sh.ID, "lightColor"), lightColor.x,lightColor.y,lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(sh.ID, "lightPos"), lightPos.x,lightPos.y,lightPos.z);


        //TEXTURE
        stbi_set_flip_vertically_on_load(true);
        int widthImg, heightImg, numColCh;
        unsigned char* bytes = stbi_load("../assets/textures/box.jpg", &widthImg, &heightImg, &numColCh, 0);


        auto tex = Texture("../assets/textures/box.jpg", GL_TEXTURE_2D, GL_TEXTURE0,GL_RGB, GL_UNSIGNED_BYTE);
        tex.texUnit(sh,"tex0",0.0);


        float th = 0.0f;
        while(!glfwWindowShouldClose(window)){
//            this->processInput();


            //clear the screen with specified color

            glClearColor(0.07f,0.13f,0.17f, 1.0f); //color config
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            camera.Inputs(window);
            camera.updateMatrix(45.0f, 0.1f, 100.0f);


            sh.Activate();
            glUniform3f(glGetUniformLocation(sh.ID,"camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
            camera.Matrix( sh, "camMatrix");
            //draw triangle

            tex.Bind();
            vao.Bind();
            glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(int),GL_UNSIGNED_INT,nullptr);

            lightShader.Activate();
            camera.Matrix(lightShader, "camMatrix");
            lightVAO.Bind();
            glDrawElements(GL_TRIANGLES, sizeof(lightIndices)/sizeof(int), GL_UNSIGNED_INT, nullptr);



//            glBindVertexArray(0);




            //swap front and back buffers
            glfwSwapBuffers(window);

            //make the window responsive to input
            glfwPollEvents();


        }

        vao.Delete();
        vbo.Delete();
        ebo.Delete();
        sh.Delete();
        tex.Delete();
//        glDeleteTextures(1,&texture);
        glfwDestroyWindow(window);
        glfwTerminate();

    };


};




int main() {
    auto* app = new App();
    app->run();

}
