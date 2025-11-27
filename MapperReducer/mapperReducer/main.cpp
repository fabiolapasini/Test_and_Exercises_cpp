#include <fstream>
#include <iostream>
#include <string>

#include "include/utils.h"
#include "src/mapper_reducer.cpp"

using json = nlohmann::json;
using namespace log4cxx;
using namespace log4cxx::helpers;

int main() {
  log4cxx::BasicConfigurator::configure();
  LoggerPtr logger(Logger::getLogger("MyTestLogger"));

  std::filesystem::path config_json_file = path::assets_dir / "config.json";
  std::ifstream file(config_json_file);
  if (!file.is_open()) {
    LOG4CXX_ERROR(logger, "Impossible to open config.json");
    return 1;
  }

  json j;
  file >> j;
  utils::Configuration config;
  try {
    config = j.get<utils::Configuration>();
  } catch (const json::exception &e) {
    LOG4CXX_ERROR(logger, "Error during JSON parsing: " << e.what());
    return 1;
  }

  LOG4CXX_INFO(logger, "MapperProducerInfo - N: "
                           << config.mapperProducerInfo.N
                           << " M: " << config.mapperProducerInfo.M);

  LOG4CXX_INFO(logger,
               "FoldersInfo - InputFiles: "
                   << config.foldersInfo.InputFiles << " IntermediateFiles: "
                   << config.foldersInfo.IntermediateFiles
                   << "OutputFiles: " << config.foldersInfo.OutputFiles);

  runProgram(logger, config);

  return 0;
}
