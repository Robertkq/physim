#ifndef PHYSIM_COMMON_H
#define PHYSIM_COMMON_H

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

constexpr uint16_t SCREEN_WIDTH = 1920;
constexpr uint16_t SCREEN_LENGTH = 1080;
constexpr float SCREEN_WIDTH_F = 1920.f;
constexpr float SCREEN_LENGTH_F = 1080.f;
constexpr float pi = 3.14159265f;

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
        Circle = 0,
        Square = 1,
        Rectangle = 2,
        Triangle = 3,
        Convex = 4
    };
}

#endif