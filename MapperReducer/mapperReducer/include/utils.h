#pragma once

#include <string>
#include <unordered_map>


bool isAlphanumeric(char c);

void countWordsFromFile(const std::string& filename,
                        const std::string IntermediateFiles,
                        std::unordered_map<std::string, int>& wordCounts);