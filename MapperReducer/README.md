# Mapper Reducer
Parallel MapReduce Word Counter

This program implements a local, parallel MapReduce process to count the occurrences of each word across multiple text files in ASCII format. The execution involves two main steps: a mapper phase and a reducer phase.

### Mapper Phase:
Launch N mapper threads (where N is less than the number of files) to process each file independently, splitting the text into words.
Each mapper writes its output to files named mr-N-X, where N is the mapper index, and X is the “bucket number” (calculated using the first letter of each word modulo M, the number of reducers).

### Reducer Phase:
Launch M reducer threads, each of which processes all files with the same bucket number X (e.g., mr-0-X, mr-1-X, …).
Each reducer aggregates word counts from its assigned bucket files and writes the final counts to out-X files.

### Output Format:
The output is saved in out-X files, with each file containing unique word counts in the format:


### Special Considerations:
- Ignore punctuation (e.g., commas, periods) to count only alphanumeric characters. \
- Choose N and M values greater than 2.



## Execution

