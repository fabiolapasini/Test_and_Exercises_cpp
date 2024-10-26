#include <iostream>
#include <filesystem>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <fstream>
#include <sstream>
#include <chrono>
#include <regex>
#include <unordered_map>

using namespace std;
std::mutex vectorMutex;
std::mutex mapMutex;

const int N = 5;
const int M = 4;
const string InputFiles = "InputFiles";
const string IntermediateFiles = "IntermediateFiles";
const string OutputFiles = "OutputFiles";



// function that read the occurencies of words
void countWordsFromFile(const std::string& filename, std::unordered_map<std::string, int>& wordCounts)
{
	string path = IntermediateFiles + "\\" + filename;
	std::ifstream file(path);
	if (file.is_open())
	{
		std::string word;
		while (file >> word)
		{
			// lock the shared map
			std::lock_guard<std::mutex> lock(mapMutex);
			++wordCounts[word];
		}
	}

	file.close();
}


// reducer function
void ReducerFunction(std::vector<string>& fileNameVector, int indexReducer)
{
	// shared map
	std::unordered_map<std::string, int> wordCounts;
	std::vector<thread> threadsVect;

	for (int i = 0; i < fileNameVector.size(); i++)
	{
		// each of the M reducer has to read N files, I do that in parallel
		// do not se push_back()
		threadsVect.emplace_back(countWordsFromFile, fileNameVector[i], std::ref(wordCounts));
	}

	// Wait for all threads to complete
	for (auto& thread : threadsVect)
	{
		thread.join();
	}

	// give th title to the output file
	string outputFileName = "out-" + std::to_string(indexReducer) + ".txt";

	// write the resulting map in an output file
	std::ofstream outputFile(OutputFiles + "\\" + outputFileName);
	if (outputFile.is_open())
	{
		for (const auto& pair : wordCounts)
		{
			outputFile << pair.first << ": " << pair.second << std::endl;
		}
	}

	outputFile.close();
}


// understand if a char is alphanumeric or not (to esclude commas..)
bool isAlphanumeric(char c)
{
	// see ascii table
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}


// mapper function
void MapperFunction(std::vector<string>& fileNameVector, int indexMapper)
{
	// one or more threads have to deal with more files since  
	// the number of threads is less then the number of files
	while (fileNameVector.size() > 0)
	{
		// I protect the access to the vector, only one thread at a time can do pop()
		vectorMutex.lock();

		std::cout << "Thread: " << indexMapper << "  got the access " << std::endl;
		string fileName = fileNameVector.back();
		fileNameVector.pop_back();
		std::cout << "Thread: " << indexMapper << "  deals with: " << fileName << std::endl;

		vectorMutex.unlock();

		int index = 0;
		int temp = 0;
		string intermediateFileName;
		string alphanumericWord;
		std::ifstream inputFile(InputFiles + "\\" + fileName);
		std::vector<std::shared_ptr<std::ofstream>> outputFilesPointers;
		std::vector<std::string> assignedFiles;
		if (inputFile.is_open())
		{
			std::string word;
			while (!inputFile.eof())
			{
				inputFile >> word;
				alphanumericWord = "";
				for (char c : word)
				{
					// check if it is alphanumeric
					if (isAlphanumeric(c))
					{
						// add the isAlphanumeric together to form the word
						alphanumericWord += c;
					}
				}

				if (!alphanumericWord.empty())
				{
					// I compose the file title by applying % operation to the ascii table index of the first letter
					index = alphanumericWord.at(0);
					temp = index % M;
					intermediateFileName = IntermediateFiles + "\\" + "mr-" + std::to_string(indexMapper) + "-" + std::to_string(temp) + ".txt";

					// verify id the intermediateFileName file is already be assigned to an ofstream
					auto it = std::find(assignedFiles.begin(), assignedFiles.end(), intermediateFileName);
					if (it != assignedFiles.end())
					{
						// get the index that corrispond to intermediateFileName
						int index = std::distance(assignedFiles.begin(), it);

						// retrieve the ofstream
						std::shared_ptr<std::ofstream>& outputFilePtr = outputFilesPointers[index];
						*outputFilePtr << alphanumericWord << std::endl;
					}
					else
					{
						// the intermediateFileName was not assigned yet, open a new ofstream object
						std::shared_ptr<std::ofstream> interFileOffStream(new std::ofstream(intermediateFileName, std::ios::app));
						if (interFileOffStream->is_open())
						{
							// add the intermediateFileName to a vector that keep track of the assigned
							// files, in that way i don t do more time ofstream on the same file
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
		for (const auto& file : outputFilesPointers)
		{
			// iterates on the vector of std::ofstream to close all
			file->close();
		}

		std::cout << "Thread: " << indexMapper << " ended" << endl;
	}
}



int main()
{
	// start the timer
	// std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	// get the absolute path where to find the input files
	std::filesystem::path directory = std::filesystem::current_path() / InputFiles;

	// vector of inputs files
	std::vector<string> inputsFilesVector;
	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		// if the file is ok and has extension .txt
		if (entry.is_regular_file() && entry.path().extension() == ".txt")
		{
			// read the name of the file and add them to a vector
			string fileName = entry.path().filename().string();
			inputsFilesVector.push_back(fileName);
		}
	}

	// vector of threads
	std::vector<thread> mapperThreadsVector;
	for (int n = 0; n < N; n++)
	{
		// n reducer-thread that will create respectively N threads that read the files
		mapperThreadsVector.push_back(thread(MapperFunction, std::ref(inputsFilesVector), n));
	}

	// wait until all mappers have finished
	for (auto& th : mapperThreadsVector)
	{
		th.join();
	}

	// get the absolute path where to find the intermediate files
	std::filesystem::path interFiesDir = std::filesystem::current_path() / IntermediateFiles;

	string fileName;
	// vector of temp files
	std::vector<string> temp;
	std::string regexString(R"((d)\.txt$)");
	// vector of vector intermediate files
	std::vector<std::vector<string>> filesForReducer;
	for (int m = 0; m < M; m++)
	{
		/*
		I do a regex_search to divide the intermediate files.
		I search for all the file names which the last number of the title id equal to m (reducer index)
		*/
		std::string currentIndexRegex = regexString;
		// I replace (d)\.txt with (0)\.txt, (1)\.txt ...
		currentIndexRegex.replace(currentIndexRegex.find("d"), 1, std::to_string(m));
		std::regex pattern(currentIndexRegex);

		// clear the temp vector
		temp.clear();

		for (const auto& entry : std::filesystem::directory_iterator(interFiesDir))
		{
			// if the file is ok and has extension .txt
			if (entry.is_regular_file() && entry.path().extension() == ".txt")
			{
				// read the name of the file
				fileName = entry.path().filename().string();

				// match the last number of the title with 
				std::smatch match;
				if (std::regex_search(fileName, match, pattern))
				{
					// std::cout << "Filename: " << fileName << " , m: " << m << std::endl;
					temp.push_back(fileName);
				}
			}
		}
		filesForReducer.push_back(temp);
	}


	// vector of reducer threads
	std::vector<thread> reducerThreadsVector;
	for (int m = 0; m < M; m++)
	{
		// create M reducer threads
		reducerThreadsVector.push_back(thread(ReducerFunction, std::ref(filesForReducer[m]), m));
	}

	// wait until all reducers have finished
	for (auto& th : reducerThreadsVector)
	{
		th.join();
	}

	/*// stop the timer
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	// calculate the duration
	std::chrono::duration<double> duration = end - start;
	std::cout << "Tempo trascorso: " << duration.count() << " secondi" << std::endl;*/

	return 0;

}