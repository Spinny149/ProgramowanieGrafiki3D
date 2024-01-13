//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Application/application.h"
#include "Application/utils.h"
#include "glad/gl.h"

#include "camera.h"
#include "camera_controler.h"

#include "Engine/Material.h"
#include "Engine/Mesh.h"

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug), color_material({ 1.0f, 1.0f, 1.0f, 1.0f }) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void set_camera(Camera* camera) { camera_ = camera; }
    Camera* camera() { return camera_; }
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void scroll_callback(double xoffset, double yoffset) override;

    void set_controler(CameraControler* controler) { controler_ = controler; }

    void cursor_position_callback(double x, double y);

    void mouse_button_callback(int button, int action, int mods);

private:
    GLuint vao_;

    float fov_;
    float aspect_;
    float near_;
    float far_;

    CameraControler* controler_;

    GLuint u_fragment_buffer_handle;

    Camera* camera_;

    xe::ColorMaterial color_material;
    void load_image(const std::string& p_path);

    std::vector<xe::Mesh*> meshes;
    void add_submesh(xe::Mesh* p_mesh);
};