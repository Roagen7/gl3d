
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
        { //     COORDINATES     /        COLORS      /   TexCoord  //
                -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
                -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
                0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
                0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
                0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
        };

// Indices for vertices order
GLuint indices[] =
        {
                0, 1, 2,
                0, 2, 3,
                0, 1, 4,
                1, 2, 4,
                2, 3, 4,
                3, 0, 4
        };


GLfloat vertices2[] = {
        //COORDS                COLORS
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
        0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
        0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

GLuint indices2[] = {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Low
};

GLfloat vertices3[] = {
        //COORDS                                        //COLORS
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,        0.8f, 0.3f,  0.02f, // Lower left corner
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,         0.8f, 0.3f,  0.02f, // Lower right corner
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,      1.0f, 0.6f,  0.32f,// Upper corner
        -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,     0.9f, 0.45f, 0.17f,// Inner left
        0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,      0.9f, 0.45f, 0.17f,// Inner right
        0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,         0.8f, 0.3f,  0.02f // Inner down
};


GLuint indices3[] = {
        0, 3, 5, // Lower left triangle
        3, 2, 4, // Lower right triangle
        5, 4, 1 // Upper triangle
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
        vao.LinkAttribs(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)nullptr);
        vao.LinkAttribs(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*) (3 * sizeof(float))); //set it to layout 1 to access it in shader
        vao.LinkAttribs(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*) (6 * sizeof(float)));

        vao.Unbind();
        vbo.Unbind();
        ebo.Unbind();



        //TEXTURE
        stbi_set_flip_vertically_on_load(true);
        int widthImg, heightImg, numColCh;
        unsigned char* bytes = stbi_load("../assets/textures/box.jpg", &widthImg, &heightImg, &numColCh, 0);


        auto tex = Texture("../assets/textures/box.jpg", GL_TEXTURE_2D, GL_TEXTURE0,GL_RGB, GL_UNSIGNED_BYTE);
        tex.texUnit(sh,"tex0",0);
        sh.Activate();

        float th = 0.0f;
        while(!glfwWindowShouldClose(window)){
//            this->processInput();
            camera.Inputs(window);

            //clear the screen with specified color

            glClearColor(0.07f,0.13f,0.17f, 1.0f); //color config
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            sh.Activate();

            camera.Matrix(45.0f, 0.1f, 100.0f, sh, "camMatrix");
            //draw triangle

            tex.Bind();
            vao.Bind();

            glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(int),GL_UNSIGNED_INT,nullptr);
            glBindVertexArray(0);


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
