#include <filesystem>
#include <fstream>
#include <mutex>
#include <sstream>

#include "../include/utils.h"

// std::mutex mapMutex;

namespace utils {

// conversion functions for MapperProducerInfo
void to_json(json &j, const utils::MapperProducerInfo &mpi) {
  j = json{{"N", mpi.N}, {"M", mpi.M}};
}

void from_json(const json &j, utils::MapperProducerInfo &mpi) {
  j.at("N").get_to(mpi.N);
  j.at("M").get_to(mpi.M);
}

// conversion functions for FoldersInfo
void to_json(json &j, const utils::FoldersInfo &fi) {
  j = json{{"InputFiles", fi.InputFiles},
           {"IntermediateFiles", fi.IntermediateFiles},
           {"OutputFiles", fi.OutputFiles}};
}

void from_json(const json &j, utils::FoldersInfo &fi) {
  j.at("InputFiles").get_to(fi.InputFiles);
  j.at("IntermediateFiles").get_to(fi.IntermediateFiles);
  j.at("OutputFiles").get_to(fi.OutputFiles);
}

// conversion functions for Configuration
void to_json(json &j, const utils::Configuration &config) {
  j = json{{"MapperProducerInfo", config.mapperProducerInfo},
           {"FoldersInfo", config.foldersInfo}};
}

void from_json(const json &j, utils::Configuration &config) {
  j.at("MapperProducerInfo").get_to(config.mapperProducerInfo);
  j.at("FoldersInfo").get_to(config.foldersInfo);
}

// understand if a char is alphanumeric or not (to esclude commas..)
bool isAlphanumeric(char c) {
  // see ascii table
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
         (c >= '0' && c <= '9');
}

// function that read the occurencies of words
void countWordsFromFile(const std::string &filename,
                        std::filesystem::path IntermediateFiles,
                        std::unordered_map<std::string, int> &wordCounts) {
  std::filesystem::path filePath = IntermediateFiles / filename;

  std::ifstream file(filePath);
  if (!file.is_open())
    return;

  std::string word;
  while (file >> word) {
    ++wordCounts[word];
  }
}

} // namespace utils
