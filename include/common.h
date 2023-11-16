#ifndef PHYSIM_COMMON_H
#define PHYSIM_COMMON_H

#define SCREEN_WIDTH 1920
#define SCREEN_LENGTH 1080

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <memory>
#include <exception>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

namespace kq
{

    enum class objectType : int
    {
        circle = 0,
        square = 1,
        rectangle = 2,
        triangle = 3,
        convex = 4
    };
}

#endif