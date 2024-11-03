#include <filesystem>
#include <fstream>
#include <mutex>
#include <sstream>

#include "../include/utils.h"

std::mutex mapMutex;

// conversion functions for MapperProducerInfo
void to_json(json& j, const MapperProducerInfo& mpi) {
  j = json{{"N", mpi.N}, {"M", mpi.M}};
}

void from_json(const json& j, MapperProducerInfo& mpi) {
  j.at("N").get_to(mpi.N);
  j.at("M").get_to(mpi.M);
}

// conversion functions for FoldersInfo
void to_json(json& j, const FoldersInfo& fi) {
  j = json{{"InputFiles", fi.InputFiles},
           {"IntermediateFiles", fi.IntermediateFiles},
           {"OutputFiles", fi.OutputFiles}};
}

void from_json(const json& j, FoldersInfo& fi) {
  j.at("InputFiles").get_to(fi.InputFiles);
  j.at("IntermediateFiles").get_to(fi.IntermediateFiles);
  j.at("OutputFiles").get_to(fi.OutputFiles);
}

// conversion functions for Configuration
void to_json(json& j, const Configuration& config) {
  j = json{{"MapperProducerInfo", config.mapperProducerInfo},
           {"FoldersInfo", config.foldersInfo}};
}

void from_json(const json& j, Configuration& config) {
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
