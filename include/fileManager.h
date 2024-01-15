#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace kq
{

class physicalObject;
class physim;

class fileManager {
public:
    fileManager(physim* parent) : parent(parent) {}
    physim* parent;
    bool loadcsv(const std::string& filename, std::vector<physicalObject*>& objects);
    bool savecsv(const std::string& filename, const std::vector<physicalObject*>& objects);
};

}


