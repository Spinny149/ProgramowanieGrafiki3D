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

#define STB_IMAGE_IMPLEMENTATION  1

#include "3rdParty/stb/stb_image.h"

#include <spdlog/spdlog.h>


void SimpleShapeApplication::init() {
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    load_image(std::string(ROOT_DIR) + "/Models/multicolor.png");
    //Material
    xe::ColorMaterial::init();

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
        -0.5f, 0.0f, -0.5f, 0.1910f, 0.5000f, // 0
         0.5f, 0.0f,  0.5f, 0.8090f, 0.5000f, // 1
        -0.5f, 0.0f,  0.5f, 0.5000f, 0.8090f, // 2
         0.5f, 0.0f, -0.5f, 0.5000f, 0.1910f, // 3
         0.0f, 1.0f,  0.0f, 0.0000f, 1.0000f, // 4
         0.0f, 1.0f,  0.0f, 1.0000f, 1.0000f, // 5
         0.0f, 1.0f,  0.0f, 1.0000f, 0.0000f, // 6
         0.0f, 1.0f,  0.0f, 0.0000f, 0.0000f, // 7
    };

    std::vector<GLushort> indices = {

        1, 2, 0, // Floor 1
        3, 1, 0, // Floor 2

        0, 2, 4, 
        2, 1, 5, 
        1, 3, 6, 
        3, 0, 7  
    };

    auto* pyramid = new xe::Mesh;
    pyramid->allocate_vertex_buffer(vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());

    pyramid->vertex_attrib_pointer(0, 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
    pyramid->vertex_attrib_pointer(1, 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    pyramid->allocate_index_buffer(indices.size() * sizeof(GLushort), GL_STATIC_DRAW);
    pyramid->load_indices(0, indices.size() * sizeof(GLushort), indices.data());

    pyramid->add_submesh(0, indices.size(), &color_material);

    add_submesh(pyramid);

    // Generating the buffer and loading the uniform data into it.
    // 
    //---------------------------------------------------------------------------------
    // Creating and binding the buffer for modifying pixel color
    glGenBuffers(1, &u_fragment_buffer_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, u_fragment_buffer_handle);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_fragment_buffer_handle);

    //---------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------
    set_camera(new Camera);
    set_controler(new CameraControler(camera()));

    //const glm::vec3 camera_position = { -1.0f, -1.0f, 2.0f }; // bok 1 Y/B
    const glm::vec3 camera_position = { 2.0f, 2.0f, 2.0f }; // bok 1 Y/R
    //const glm::vec3 camera_position = { -2.0f, 2.0f, -2.0f }; // bok 1 B/G
    //const glm::vec3 camera_position = { 0.0f, 8.0f, 2.0f }; //góra
    //const glm::vec3 camera_position = { 0.0f, -8.0f, 2.0f }; //dół
    //const glm::vec3 camera_position = { 2.0f, -2.0f, 2.0f }; // obrót x, obrót o y;odleglosc w Z
    const glm::vec3 camera_target = { 0.0f, 0.5f, 0.0f };
    const glm::vec3 camera_up = { 0.0f, 1.0f, 0.0f };

     
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    aspect_ = (float)w / h;
    fov_ = glm::pi<float>() / 4.0;
    near_ = 0.1f;
    far_ = 100.0f;

    camera_->perspective(fov_, aspect_, near_, far_);
    camera_->look_at(camera_position, camera_target, camera_up);
    controler_->set_camera(camera_);

    //---------------------------------------------------------------------------------

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    //auto [w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glUseProgram(xe::ColorMaterial::program()); 
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h);
    camera_->set_aspect((float)w / h);
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);
    camera()->zoom(yoffset / 30.0f);
}

void SimpleShapeApplication::cursor_position_callback(double x, double y)
{
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods)
{
    Application::mouse_button_callback(button, action, mods);

    if (controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controler_->LMB_released(x, y);
    }
}

void SimpleShapeApplication::load_image(const std::string& p_path) {
    stbi_set_flip_vertically_on_load(true);
    GLint width{}, height{}, channels{};
    const auto img = stbi_load(p_path.c_str(), &width, &height, &channels, 0);
    if (!img) {
        spdlog::warn("Could not read image from file `{}'", p_path);
    }

    GLuint texture{};
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);

    color_material.set_texture(texture);
}

void SimpleShapeApplication::add_submesh(xe::Mesh* p_mesh)
{
    meshes.emplace_back(p_mesh);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {

    auto PVM = camera_->projection() * camera_->view();
    glBindBuffer(GL_UNIFORM_BUFFER, u_fragment_buffer_handle);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Binding the VAO will setup all the required vertex buffers.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, nullptr);

    for (const auto* mesh : meshes)
    {
        mesh->draw();
    }
}
