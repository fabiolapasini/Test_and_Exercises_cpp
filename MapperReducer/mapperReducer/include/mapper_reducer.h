#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

#include "utils.h"


void ReducerFunction(std::vector<std::string>& fileNameVector,
                     FoldersInfo foldersInfo, int indexReducer);

void MapperFunction(std::vector<std::string>& fileNameVector,
                    Configuration config, int indexMapper);
