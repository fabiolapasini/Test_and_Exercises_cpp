# Assignment

## Library

I used the Standard C++17 ISO (/std:c++17) library.

## Algorithm

### Main 

* The main funcion read all the titles of the given files, save them into a vector. 
* It creates N mapper thread and pass them the vector then it waits until all the mapper threads have finished.
* It creates a vector of vectors structure of dimension  [MxN] where M is the number of reducer threads and N is the number of mapper thread. For each of the M reducer this structure contains the N files the reducer has to work on.
*  It creates the M reducer threads, pass to each of them the corrispondent vector of files. Then it waits until the reducer threads have finished.

### Mapper

* This Mapper function is performed by N threads in paralel. In this function the vector that contains the name of the input files is passed as reference, each thread chooses one at a time the file it is going to work on, unti there are no files left. 
* Each Mapper thread opens the input, read each words separated by " " but keep only the alphanumeric char.
* Then the Mapper compose the name of the intermediate file it will save the alphanumeric words on. 
* Then the Mapper thread tries to understand if the intermediate file has already been assigned to an ofstream object. If yes, the mapper gets the reference to that ofstream object and writes on it, otherwise it creates a new ofstream object, assign to it the intermediate file and writes on it.

### Reducer

* Each of the Reducer threads has to work with the files contained in the vector that is passed as argument.
* Each Reducer creates other N thread:
  * each of them works in paraller counting the words of one of the N file. Each thread saves each word and the corresponding number of occurencies inside a shared map at which they can gain access one at a time.
* Each Reducer waits until the created threads have fiished, then it compose the name of the output file and copy the shared map inside the output file.

## Files Used

### Input files
I used the given files without modifying them 

### Intermediate Files
Those files are created by the Mapper Threads, the names of the files follow this scheme: <br>
_"mr-" + n + "-" + m_ <br>
where n is the number of the thread and m is the index of the ascii table of the fisrt char of the word % m. <br>
So the Intermediate files anumber is N x M.

### Ouput files
Each Reducer creates an output files, so there are M output files.



