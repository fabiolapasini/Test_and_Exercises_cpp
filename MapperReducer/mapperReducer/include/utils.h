#pragma once

#include <log4cxx/asyncappender.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/propertyconfigurator.h>
// #include <log4cxx/rollingfileappender.h>

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

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

// conversion functions for MapperProducerInfo
void to_json(json &j, const MapperProducerInfo &mpi);

void from_json(const json &j, MapperProducerInfo &mpi);

// conversion functions for FoldersInfo
void to_json(json &j, const FoldersInfo &fi);

void from_json(const json &j, FoldersInfo &fi);

// conversion functions for Configuration
void to_json(json &j, const Configuration &config);

void from_json(const json &j, Configuration &config);

bool isAlphanumeric(char c);

void countWordsFromFile(const std::string &filename,
                        const std::string IntermediateFiles,
                        std::unordered_map<std::string, int> &wordCounts);