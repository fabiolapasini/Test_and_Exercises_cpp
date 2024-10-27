#pragma once

#include <string>
#include <unordered_map>


bool isAlphanumeric(char c);


void countWordsFromFile(const std::string& filename,
                        std::unordered_map<std::string, int>& wordCounts);