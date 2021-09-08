
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
#include "src/object/Mesh.h"
#include "src/object/Model.h"


const int width = 1920;
const int height = 1080;
Vertex vertices[] =
        { //               COORDINATES           /            COLORS          /           TexCoord         /       NORMALS         //
                Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
                Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
                Vertex{glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
                Vertex{glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
        };


// Indices for vertices order
GLuint indices[] =
        {
                0, 1, 2,
                0, 2, 3
        };
Vertex lightVertices[] =
        { //     COORDINATES     //
                Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
                Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
                Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
                Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
                Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
                Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
                Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
                Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
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
        //init vertices, indices, textures etc...  order is pos, normal, color, texture
        Texture textures[]
                {
                        Texture("../assets/textures/planks.png", "diffuse", 0),
                        Texture("../assets/textures/planksSpec.png", "specular", 1)
                };

        // static array -> vector
        std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
        std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
        std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
        std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
        std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
        // init shaders
        Shader sh_col("../src/shader/shaders/mesh/col/vs.glsl", "../src/shader/shaders/mesh/col/fs.glsl");
        Shader sh_tex_spec("../src/shader/shaders/mesh/tex_spec/vs.glsl","../src/shader/shaders/mesh/tex_spec/fs.glsl");
        Shader sh_tex("../src/shader/shaders/mesh/tex/vs.glsl","../src/shader/shaders/mesh/tex/fs.glsl");
        Shader sh_lamp("../src/shader/shaders/light/lamp/vs.glsl", "../src/shader/shaders/light/lamp/fs.glsl");

        //init meshes

        std::vector <Vertex> vis;
        std::vector<GLuint> is;
        Mesh::fromObjFile("../assets/objs/hammer.obj",vis,is);
        std::vector<Texture> empty = {};
        Mesh hammer(vis, is, empty);
        Mesh floor(verts, ind, tex);
        Mesh light(lightVerts, lightInd, tex);



        //light attribs
        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


        glm::vec3 lightPos = {0.5f, 0.5f, 0.0f};
        glm::mat4 lightModel = glm::mat4(1.0f);
//        lightModel = glm::rotate(lightModel, 0.4f, glm::vec3(1.0, 0.0,0.0));
//        lightModel = glm::translate(lightModel, lightPos);
        light.model = glm::translate(lightModel, lightPos);

        glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat4 pyramidModel = glm::mat4(1.0f);
        pyramidModel = glm::translate(pyramidModel, pyramidPos);


        sh_lamp.setVec4("lightColor", lightColor.x,lightColor.y,lightColor.z, lightColor.w);

        sh_tex_spec.setVec4("lightColor", lightColor.x,lightColor.y,lightColor.z, lightColor.w);
        sh_tex_spec.setVec3("lightPos", lightPos.x,lightPos.y,lightPos.z);

        sh_col.setVec4("lightColor", lightColor.x,lightColor.y,lightColor.z, lightColor.w);
        sh_col.setVec3("lightPos", lightPos.x,lightPos.y,lightPos.z);


        float th = 0.1;


        while(!glfwWindowShouldClose(window)){
//            this->processInput();
            //clear the screen with specified color

            glClearColor(0.07f,0.13f,0.17f, 1.0f); //color config
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            camera.Inputs(window);
            camera.updateMatrix(45.0f, 0.1f, 100.0f);


            th += 0.01;
//            light.model = glm::translate(glm::mat4(1.0f), lightPos);
            light.model = glm::translate(glm::mat4(1.0f), lightPos);
            light.model = glm::rotate(light.model, th, {0.0, 1.0, 1.0});
//            lightPos.x = th;
            sh_tex_spec.setVec3("lightPos", lightPos.x,lightPos.y,lightPos.z);
            sh_col.setVec3("lightPos", lightPos.x,lightPos.y,lightPos.z);
            //draw( shader to use, camera to use)
            hammer.Draw(sh_col, camera, glm::translate(glm::mat4(1.0f), {0.0, 0.0, -4.0}));
            floor.Draw(sh_tex_spec, camera, glm::mat4(1.0f));
            light.Draw(sh_lamp, camera,  glm::rotate(light.model, th, {0.0, 1.0, 1.0}));

            //swap front and back buffers
            glfwSwapBuffers(window);
            //make the window responsive to input
            glfwPollEvents();

        }


        sh_tex.Delete();
        sh_tex_spec.Delete();
        sh_lamp.Delete();
//        glDeleteTextures(1,&texture);
        glfwDestroyWindow(window);
        glfwTerminate();

    };


};




int main() {
    auto* app = new App();
    app->run();

}
