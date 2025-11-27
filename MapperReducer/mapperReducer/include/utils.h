#pragma once

#include <log4cxx/asyncappender.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/propertyconfigurator.h>

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using json = nlohmann::json;

namespace utils {

struct MapperProducerInfo {
  int N;
  int M;
};

struct FoldersInfo {
  std::string InputFiles;
  std::string IntermediateFiles;
  std::string OutputFiles;
};

struct Configuration {
  MapperProducerInfo mapperProducerInfo;
  FoldersInfo foldersInfo;
};

// Conversion functions for MapperProducerInfo
void to_json(json &j, const MapperProducerInfo &mpi);
void from_json(const json &j, MapperProducerInfo &mpi);

// Conversion functions for FoldersInfo
void to_json(json &j, const FoldersInfo &fi);
void from_json(const json &j, FoldersInfo &fi);

// Conversion functions for Configuration
void to_json(json &j, const Configuration &config);
void from_json(const json &j, Configuration &config);

bool isAlphanumeric(char c);

void countWordsFromFile(const std::string &filename,
                        std::filesystem::path IntermediateFiles,
                        std::unordered_map<std::string, int> &wordCounts);

} // namespace utils

namespace path {

static const std::filesystem::path assets_dir =
    std::filesystem::current_path() / "Assets";

inline std::filesystem::path input_dir(const utils::FoldersInfo &fi) {
  return assets_dir / fi.InputFiles;
}

inline std::filesystem::path intermediate_dir(const utils::FoldersInfo &fi) {
  return assets_dir / fi.IntermediateFiles;
}

inline std::filesystem::path output_dir(const utils::FoldersInfo &fi) {
  return assets_dir / fi.OutputFiles;
}

} // namespace path
