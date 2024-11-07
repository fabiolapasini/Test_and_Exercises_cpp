#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "include/utils.h"
#include "src/mapper_reducer.cpp"

using json = nlohmann::json;
using namespace log4cxx;
using namespace log4cxx::helpers;

// TODO
/*
1) add a file called path.h and store there all the used paths, avoiding doing
this: std::filesystem::path intermediateFilePath =
              std::filesystem::current_path() / "Assets" /
              config.foldersInfo.IntermediateFiles /

2) logging: add the library, and the simplest usage.
=> now:
- save the log in a file
- read more abt the logging configuration,
 - what are the numbers printed by the logger, the names if th thread maybe.
2 [0x00005e3c] INFO MyTestLogger
- is it possible to extend the lib?

3) add precommit to check if the format is good, make the commit fails if it is
not

4) what abt pipilines? is possible to write pipelines to check the
results of the test and the installation process.
*/

int main() {
  log4cxx::BasicConfigurator::configure();
  LoggerPtr logger(Logger::getLogger("MyTestLogger"));

  std::filesystem::path config_json_file =
      std::filesystem::current_path() / "Assets" / "config.json";
  std::ifstream file(config_json_file);
  if (!file.is_open()) {
    LOG4CXX_ERROR(logger, "Impossible to open config.json");
    return 1;
  }

  json j;
  file >> j;
  Configuration config;
  try {
    config = j.get<Configuration>();
  } catch (const json::exception& e) {
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
