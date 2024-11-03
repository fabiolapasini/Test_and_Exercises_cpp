#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "src/mapper_reducer.cpp"
#include "include/utils.h"

using json = nlohmann::json;


int main() {
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






















// #include <nlohmann/json.hpp>

// TODO
/*
leggere i parametri da json e parsarli con quella libreria
e fare delle strutture dati adeguate

aggiugnere logging e mettere tutti i print li dentro

aggiungere precommit, si posso fare le pipeline tipo con bamboo?
*/

//int main() {
//  runProgram();
//  return 0;
//}