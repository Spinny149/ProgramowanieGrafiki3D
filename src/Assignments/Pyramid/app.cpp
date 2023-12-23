//
// Created by pbialas on 25.09.2020.
//

#include "app.h"
#include <iostream>
#include <vector>
#include <tuple>
#include "Application/utils.h"

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

void SimpleShapeApplication::init() {
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
        { {GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
         {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"} });

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            -0.5f, 0.0f, -0.5f,  //0
            -0.5f, 0.0f, 0.5f,   //1
            0.5f, 0.0f, -0.5f,   //2
            0.5f, 0.0f, 0.5f,    //3

            -0.5f, 0.0f, -0.5f,   //4
            0.0f, 1.0f, 0.0f,     //5
            0.5f, 0.0f, -0.5f,    //6

            0.5f, 0.f, -0.5f,     //7
            0.f, 1.f, 0.f,        //8
            0.5f, 0.f, 0.5f,      //9

            -0.5f, 0.f, 0.5f,    //10
            0.f, 1.f, 0.f,       //11
            0.5f, 0.f, 0.5f,     //12

            -0.5f, 0.f, 0.5f,     //13
            0.f, 1.f, 0.f,        //14
            -0.5f, 0.f, -0.5f,    //15
    };

    std::vector<GLushort> indices = {

            0, 2, 1,
            1, 2, 3,
            4, 5, 6,
            7, 8, 9,
            11, 10, 12,
            13, 14, 15,
    };

    const std::vector colors = {
        // colors for the base
        1.0f, 0.0f, 0.0f,  // color (red)
        1.0f, 0.0f, 0.0f,  // color (red)
        1.0f, 0.0f, 0.0f,  // color (red)
        1.0f, 0.0f, 0.0f,  // color (red)

        0.0f, 1.0f, 0.0f,  // color (green)
        0.0f, 1.0f, 0.0f,  // color (green)
        0.0f, 1.0f, 0.0f,  // color (green)

        0.0f, 0.0f, 1.0f,  // color (blue)
        0.0f, 0.0f, 1.0f,  // color (blue)
        0.0f, 0.0f, 1.0f,  // color (blue)

        1.0f, 1.0f, 0.0f,  // color (yellow)
        1.0f, 1.0f, 0.0f,  // color (yellow)
        1.0f, 1.0f, 0.0f,  // color (yellow)

        // color for the apex
        0.5f, 0.5f, 0.5f,  //color (gray)     
        0.5f, 0.5f, 0.5f,   //color (gray)     
        0.5f, 0.5f, 0.5f   //color (gray)     

    };



    // Generating the buffer and loading the vertex data into it.
    GLuint i_buffer_handle,  u_fragment_buffer_handle, u_vertex_buffer_handle, v_buffer_handle, c_buffer_handle;

    glGenBuffers(1, &i_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //---------------------------------------------------------------------------------
    // Creating and binding the buffer for modifying pixel color
    glGenBuffers(1, &u_fragment_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, u_fragment_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_fragment_buffer_handle);

    glGenBuffers(1, &u_vertex_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, u_vertex_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_vertex_buffer_handle);
    //---------------------------------------------------------------------------------
    
    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &c_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(colors.size()) * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);        
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);

    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    // and this specifies how the data is layout in the buffer.

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, c_buffer_handle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    //---------------------------------------------------------------------------------
    {
        float strength = 0.8f;
        float color[3] = { 1.0f, 1.0f, 1.0f };
            

        glBindBuffer(GL_UNIFORM_BUFFER, u_fragment_buffer_handle);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
        glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), color);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    {
        const glm::mat4 model(1.0f);
        constexpr auto fov = 45.0f;
        const auto projection = glm::perspective(glm::radians(fov), static_cast<float>(kpWidth) / static_cast<float>(kpHeight), 0.1f, 100.0f);

        //const glm::vec3 camera_position = { -1.0f, -1.0f, 2.0f }; // bok 1 Y/B
        //const glm::vec3 camera_position = { 2.0f, -2.0f, 2.0f }; // bok 1 Y/R
        //const glm::vec3 camera_position = { -2.0f, -2.0f, -2.0f }; // bok 1 B/G
        //const glm::vec3 camera_position = { 0.0f, 8.0f, 2.0f }; //góra
        const glm::vec3 camera_position = { 0.0f, -8.0f, 2.0f }; //dół
        //const glm::vec3 camera_position = { 2.0f, -2.0f, 2.0f }; // obrót x, obrót o y;odleglosc w Z
        const glm::vec3 camera_target = { 0.0f, 0.5f, 0.0f };
        const glm::vec3 camera_up = { 0.0f, 1.0f, 0.0f };

        const auto view = lookAt(camera_position, camera_target, camera_up);
        const auto pvm = projection * view * model;

        glBindBuffer(GL_UNIFORM_BUFFER, u_vertex_buffer_handle);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), value_ptr(pvm));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

    }
    //---------------------------------------------------------------------------------

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //end of vao "recording"

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glUseProgram(program);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {

    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, nullptr);

    glBindVertexArray(0);
}
