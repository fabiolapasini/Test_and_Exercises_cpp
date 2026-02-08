#include <chrono>
#include <iostream>
#include <regex>
#include <thread>

#include "../include/Threadpool.h"
#include "utils.cpp"

std::mutex vectorMutex;
using namespace log4cxx;
using namespace log4cxx::helpers;

//////////////////////////////////////////////////////////////////////
// reducer function
/////////////////////////////////////////////////////////////////////
void ReducerFunction(std::vector<std::string> fileNameVector,
                     const utils::FoldersInfo foldersInfo, int indexReducer,
                     LoggerPtr logger) {
  // shared map
  std::unordered_map<std::string, int> wordCounts;
  std::vector<std::thread> threadsVect;

  /* for (int i = 0; i < fileNameVector.size(); i++) {
  // each of the M reducer has to read N files, I do that in parallel
  // do not use push_back()
    threadsVect.emplace_back(utils::countWordsFromFile, fileNameVector[i],
                             path::intermediate_dir(foldersInfo),
                             std::ref(wordCounts));
  }
  // Wait for all threads to complete
  for (auto &thread : threadsVect) {
    thread.join();
  }*/

  {
    ThreadPool file_reader_pool(fileNameVector.size());
    for (int i = 0; i < fileNameVector.size(); i++) {
      auto current_file = fileNameVector[i];
      file_reader_pool.enqueue([current_file, &wordCounts, foldersInfo] {
        utils::countWordsFromFile(
            current_file, path::intermediate_dir(foldersInfo), wordCounts);
      });
    }
  }

  // Generate the output file name
  std::string outputFileName = "out-" + std::to_string(indexReducer) + ".txt";

  // Create the full path for the output file
  std::filesystem::path outputFilePath =
      path::output_dir(foldersInfo) / outputFileName;

  // Write the resulting map to the output file
  std::ofstream outputFile(outputFilePath);
  if (outputFile.is_open()) {
    for (const auto &pair : wordCounts) {
      outputFile << pair.first << ": " << pair.second << std::endl;
    }
  }

  outputFile.close();
}

//////////////////////////////////////////////////////////////////////
// mapper function
/////////////////////////////////////////////////////////////////////
void MapperFunction(std::vector<std::string> &fileNameVector,
                    const utils::Configuration config, int indexMapper,
                    int reducers, LoggerPtr logger) {
  // one or more threads have to deal with more files since
  // the number of threads is less then the number of files
  while (fileNameVector.size() > 0) {
    // I protect the access to the vector, only one thread at a time can do
    // pop()
    vectorMutex.lock();

    LOG4CXX_INFO(logger, "Thread: " << indexMapper << "  got the access");
    std::string fileName = fileNameVector.back();
    fileNameVector.pop_back();
    LOG4CXX_INFO(logger,
                 "Thread: " << indexMapper << "  deals with: " << fileName);

    vectorMutex.unlock();

    int index = 0;
    int temp = 0;
    std::string alphanumericWord;
    std::ifstream inputFile(path::input_dir(config.foldersInfo) / fileName);
    std::vector<std::shared_ptr<std::ofstream>> outputFilesPointers;
    std::vector<std::string> assignedFiles;
    if (inputFile.is_open()) {
      std::string word;
      while (!inputFile.eof()) {
        inputFile >> word;
        alphanumericWord = "";
        for (char c : word) {
          // check if it is alphanumeric
          if (utils::isAlphanumeric(c)) {
            // add the isAlphanumeric together to form the word
            alphanumericWord += c;
          }
        }

        if (!alphanumericWord.empty()) {
          // I compose the file title by applying % operation to the ascii table
          // index of the first letter
          index = alphanumericWord.at(0);
          temp = index % reducers;

          std::filesystem::path intermediateFilePath =
              path::intermediate_dir(config.foldersInfo) /
              ("mr-" + std::to_string(indexMapper) + "-" +
               std::to_string(temp) + ".txt");
          std::string intermediateFileName = intermediateFilePath.string();

          // verify id the intermediateFileName file is already be assigned to
          // an ofstream
          auto it = std::find(assignedFiles.begin(), assignedFiles.end(),
                              intermediateFileName);
          if (it != assignedFiles.end()) {
            // get the index that corrispond to intermediateFileName
            auto index = std::distance(assignedFiles.begin(), it);

            // retrieve the ofstream
            std::shared_ptr<std::ofstream> &outputFilePtr =
                outputFilesPointers[index];
            *outputFilePtr << alphanumericWord << std::endl;
          } else {
            // the intermediateFileName was not assigned yet, open a new
            // ofstream object
            std::shared_ptr<std::ofstream> interFileOffStream(
                new std::ofstream(intermediateFileName, std::ios::app));
            if (interFileOffStream->is_open()) {
              // add the intermediateFileName to a vector that keep track of the
              // assigned files, in that way i don t do more time ofstream on
              // the same file
              assignedFiles.push_back(intermediateFileName);

              // add ofstream to a vector
              outputFilesPointers.push_back(interFileOffStream);

              // add the alphanumeric word to the file
              *interFileOffStream << alphanumericWord << std::endl;
            }
          }
        }
      }
    }

    // at the close the file
    inputFile.close();
    for (const auto &file : outputFilesPointers) {
      // iterates on the vector of std::ofstream to close all
      file->close();
    }

    LOG4CXX_INFO(logger, "Thread: " << indexMapper << " ended");
  }
}

//////////////////////////////////////////////////////////////////////
// runner
/////////////////////////////////////////////////////////////////////
int runProgram(LoggerPtr logger, const utils::Configuration &config) {
  // start the timer
  auto start = std::chrono::steady_clock::now();

  // check and create directories if needed
  const auto inputDir = path::input_dir(config.foldersInfo);
  const auto interDir = path::intermediate_dir(config.foldersInfo);
  const auto outputDir = path::output_dir(config.foldersInfo);

  if (!std::filesystem::exists(inputDir)) {
    LOG4CXX_ERROR(logger, "Error: Input directory not found at: " << inputDir);
    return 1;
  }

  if (!std::filesystem::exists(interDir)) {
    std::filesystem::create_directory(interDir);
    LOG4CXX_INFO(logger, "Directory created: " << interDir);
  }

  if (!std::filesystem::exists(outputDir)) {
    std::filesystem::create_directory(outputDir);
    LOG4CXX_INFO(logger, "Directory created: " << outputDir);
  }

  // collect input files
  std::vector<std::string> inputFiles;
  for (const auto &entry : std::filesystem::directory_iterator(inputDir)) {
    if (entry.is_regular_file() && entry.path().extension() == ".txt") {
      inputFiles.push_back(entry.path().filename().string());
    }
  }

  //// number of thread based on concurrency
  // int mappers = std::thread::hardware_concurrency() > 0
  //                   ? std::thread::hardware_concurrency()
  //                   : config.mapperProducerInfo.N;
  // int reducers = std::thread::hardware_concurrency() > 0
  //                    ? std::thread::hardware_concurrency()
  //                    : config.mapperProducerInfo.M;

  {
    ThreadPool mapper_pool(config.mapperProducerInfo.N);
    for (int n = 0; n < config.mapperProducerInfo.N; ++n) {
      mapper_pool.enqueue([n, &inputFiles, &logger, &config] {
        MapperFunction(std::ref(inputFiles), std::cref(config), n,
                       config.mapperProducerInfo.M, logger);
      });
    }
  }

  // Wait until the end - join

  // launch mapper threads
  /* std::vector<std::thread> mapperThreads;
  for (int n = 0; n < config.mapperProducerInfo.N; ++n) {
    mapperThreads.emplace_back(MapperFunction, std::ref(inputFiles),
                               std::cref(config), n,
                               config.mapperProducerInfo.M, logger);
  }
  for (auto &th : mapperThreads) th.join();
  */

  // prepare intermediate files for reducers
  std::vector<std::vector<std::string>> filesForReducer(
      config.mapperProducerInfo.M);
  const std::string regexTemplate = R"((d)\.txt$)";

  for (int m = 0; m < config.mapperProducerInfo.M; ++m) {
    std::string regexStr = regexTemplate;
    regexStr.replace(regexStr.find('d'), 1, std::to_string(m));
    std::regex pattern(regexStr);

    for (const auto &entry : std::filesystem::directory_iterator(interDir)) {
      if (entry.is_regular_file() && entry.path().extension() == ".txt") {
        std::string filename = entry.path().filename().string();
        if (std::regex_search(filename, pattern)) {
          LOG4CXX_INFO(logger, "Filename: " << filename << " , reducer: " << m);
          filesForReducer[m].push_back(filename);
        }
      }
    }
  }

  // launch reducer threads
  // std::vector<std::thread> reducerThreads;
  // for (int m = 0; m < config.mapperProducerInfo.M; ++m) {
  //  reducerThreads.emplace_back(ReducerFunction, std::ref(filesForReducer[m]),
  //                              std::cref(config.foldersInfo), m, logger);
  //}
  // for (auto &th : reducerThreads)
  //  th.join();

  {
    ThreadPool reducer_pool(config.mapperProducerInfo.M);
    for (int m = 0; m < config.mapperProducerInfo.M; ++m) {
      auto current_file = filesForReducer[m];
      reducer_pool.enqueue([current_file, config, m, logger] {
        ReducerFunction(current_file, std::cref(config.foldersInfo), m, logger);
      });
    }
  }

  // stop the timer
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> duration = end - start;
  LOG4CXX_INFO(logger, "Time elapsed: " << duration.count() << " s");

  return 0;
}