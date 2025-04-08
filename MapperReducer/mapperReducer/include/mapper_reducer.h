#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "utils.h"

void ReducerFunction(std::vector<std::string> &fileNameVector,
                     FoldersInfo foldersInfo, int indexReducer);

void MapperFunction(std::vector<std::string> &fileNameVector,
                    Configuration config, int indexMapper);
