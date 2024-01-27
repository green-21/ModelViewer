#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <vector>



#include "Image.h"

class FileIO {
public:
    static Image ReadImage(const std::string filename);
};