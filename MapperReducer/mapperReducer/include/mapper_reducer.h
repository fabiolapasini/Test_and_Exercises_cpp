#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

void ReducerFunction(std::vector<std::string>& fileNameVector, int indexReducer,
                     std::filesystem::path outputFiesDir);

void MapperFunction(std::vector<std::string>& fileNameVector, int indexMapper);