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

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

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

private:
    GLuint vao_;

    float fov_;
    float aspect_;
    float near_;
    float far_;

    GLuint u_pvm_buffer_;

    Camera* camera_;
};