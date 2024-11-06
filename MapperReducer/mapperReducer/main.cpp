#include <log4cxx/asyncappender.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/propertyconfigurator.h>
// #include <log4cxx/rollingfileappender.h>

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
aggiungere un file path per leggere direttamente il percorso
tipo assets, intermediate files etc...

aggiugnere logging e mettere tutti i print li dentro

aggiungere precommit, si posso fare le pipeline tipo con bamboo?
*/

int main() {
  log4cxx::BasicConfigurator::configure();
  LoggerPtr logger(Logger::getLogger("MyTestLogger"));

  LOG4CXX_INFO(logger, "Questo è un messaggio informativo");
  LOG4CXX_DEBUG(logger, "Questo è un messaggio di debug");
  LOG4CXX_WARN(logger, "Questo è un messaggio di warning");
  LOG4CXX_ERROR(logger, "Questo è un messaggio di errore");
  LOG4CXX_FATAL(logger, "Questo è un messaggio fatale");

  std::filesystem::path config_json_file =
      std::filesystem::current_path() / "Assets" / "config.json";
  std::ifstream file(config_json_file);
  if (!file.is_open()) {
    std::cerr << "Impossible to open config.json" << std::endl;
    return 1;
  }

  json j;
  file >> j;
  Configuration config;
  try {
    config = j.get<Configuration>();
  } catch (const json::exception& e) {
    std::cerr << "Errore durante il parsing del JSON: " << e.what()
              << std::endl;
    return 1;
  }

  /*
  std::cout << "MapperProducerInfo:" << std::endl;
  std::cout << "N: " << config.mapperProducerInfo.N << std::endl;
  std::cout << "M: " << config.mapperProducerInfo.M << std::endl;

  std::cout << std::endl  << "FoldersInfo:" << std::endl;
  std::cout << "InputFiles: " << config.foldersInfo.InputFiles << std::endl;
  std::cout << "IntermediateFiles: " << config.foldersInfo.IntermediateFiles
            << std::endl;
  std::cout << "OutputFiles: " << config.foldersInfo.OutputFiles << std::endl;
  */

  runProgram(config);

  return 0;
}
