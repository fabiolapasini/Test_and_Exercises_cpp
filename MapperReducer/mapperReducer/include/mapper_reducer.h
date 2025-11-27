#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "utils.h"

void ReducerFunction(std::vector<std::string> &fileNameVector,
                     const utils::FoldersInfo foldersInfo, int indexReducer,
                     LoggerPtr logger);

void MapperFunction(std::vector<std::string> &fileNameVector,
                    const utils::Configuration config, int indexMapper,
                    int reducers, LoggerPtr logger);
