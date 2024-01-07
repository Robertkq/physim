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

inline std::string itos(int conv)
{
    if(conv == 0)
        return "0";
    std::string ret;
    bool negative = false;
    if(conv < 0)
    {
        negative = true;
        conv = conv * -1;
    }
    while(conv != 0)
    {
        ret.insert(ret.begin(), 48 + conv % 10);
        conv /= 10;
    }
    if(negative)
        ret.insert(ret.begin(), '-');
    return ret;
}

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