//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug), kpWidth(width), kpHeight(height){}

    void init() override;

    void frame() override;

private:
    GLuint vao_;
    int kpWidth;
    int kpHeight;
};