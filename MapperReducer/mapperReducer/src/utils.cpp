#include "../include/utils.h"

#include <filesystem>
#include <fstream>
#include <mutex>
#include <sstream>

std::mutex mapMutex;

// understand if a char is alphanumeric or not (to esclude commas..)
bool isAlphanumeric(char c) {
  // see ascii table
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
         (c >= '0' && c <= '9');
}

// function that read the occurencies of words
void countWordsFromFile(const std::string& filename,
                        const std::string IntermediateFiles,
                        std::unordered_map<std::string, int>& wordCounts) {
  std::string path = IntermediateFiles + "\\" + filename;
  std::ifstream file(path);
  if (file.is_open()) {
    std::string word;
    while (file >> word) {
      // lock the shared map
      std::lock_guard<std::mutex> lock(mapMutex);
      ++wordCounts[word];
    }
  }
  file.close();
}
