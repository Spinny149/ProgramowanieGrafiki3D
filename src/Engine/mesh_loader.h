﻿#pragma once

#include "Mesh.h"

#include <memory>
#include <string>

namespace xe
{
    std::shared_ptr<Mesh> load_mesh_from_obj(const std::string& path, const std::string& mtl_dir);
}