# `file-finder` Design

The file-finder is a multi-thread program that searches a directory tree for filenames containing substrings. 

## Functional Requirements

The program accepts a directory to search and a list of substrings to match with filenames as parameters and while running regularly displays the results as the search is in progress until the search is complete.

The program accepts user input on the standard input and execute the following commands:
1. `dump` - display the results collected up to this point and keep searching
2. `exit` - exit the program

The program will create threads for each substring to recursively iterate over all directories under the one indicated in the first parameter and match filenames with a specific substring and add to a list of results.

The program will create a separated thread to regurlarly display the results added to the list and clear the list.

The program will exit when the search is complete and the result printed out completely.

## Non-Functional Requirements

### Scalability and performance

The program requires enough memory to hold a number of results, the number will be configurable using environment variables to allow functional testing.


### Cross-platform compatibility

The program assumes a POSIX operating system like Linux or other Unix derivatives

## High-Level Design

Since this program uses a multi-thread model I decided to use C++ for the development given its abstractions for dealing with threads, shared data structure and filesystem operations.

The program requires a few functions, a `main` function that initializes the data structures and locks, parses the arguments, validates the required parameters are present and for each substring spawns a thread to search for filenames that match.

The `search` thread will traverse recursively the directory indicated in the first parmeter and for every filename found the thread will match with it's substring, if match is true acquire a lock, add the result to the data container and release the lock.

A separated thread will be watching the data container and will output the list of results, aquire a lock and clear the data container to continue to be used by search threads.

## Details

### User Input Parsing

In C/C++ programs the main program receives an integer `argc` that indicates the number of arguments passed and `argv` which is a pointer to a list of strings. `argv` includes the program name so the first position of the array is the binary name, the program will consider `argv[1]` as the directory to find files and requires at least 1 substring to search. Given the requirement the program should receive an `argc` equal or larger than 3 to be a valid execution.

This parsing and assignment will be done by the function `main` in this program which will then initialize a `file_finder` class that will manage threads and locking and also watch the standard input for commands to `dump` the result to the output or exit the program.

### Directory Traversal

One of the methods of the `file_finder` class is the `walk_dir` method which uses the C++ STL library `filesystem` class `recursive_directory_iterator` to traverse a directory and return a `directory_entry` that can be checked to distinguish files over directories or symlinks and has the filename in case files that can be matched against the substring the thread is looking for.

### Search criteria

With the `directory_entry` the thread can use `string::find` to check if the substring is present in the filename and if so add to the data structurue holding the results calling the method `store_result`

### Results

To display the results the program will have a dedicated function `store_result` to contain the logic to aquire an exclusive lock, write the new result to `std::vector<string>` and release the lock.

Another aspect of the results is the ability of the `dumper` to aquire an exclusive lock to read the list, output each item preventing the addition of new results and emptying the list.

A separated thread will be created to regularly dump the contents of the result data structure, this thread will flush the contents of the `std::vector` while holding the lock to prevent new additions to the list and reseting it at the end. This thread will aquire an `exclusive_lock` before performing the 3 steps.

### Watch for commands

The method `wait_command` will read input from `stdin` until a new line is received, if the input is `dump` it will execute the `dump` method, if the input is `exit` it will terminate the program.

### Thread management

C++ has abstractions to manage threads and locking mechanisms to share data between threads. This program will use `std::thread` and `std::thread::shared_mutex` to control read/write access using a combination of `shared_lock` and `exclusive_lock` for reads and write controls

## Testing

### Unit tests

The description suggests not spending time on unit tests and instead describing ways to test the program

### System tests

The program will have configurable delays to be able to simulate real world use cases where the scan starts, a partial set os results is dumped and more files can be added for the scanning to continue. The validation will ingest the output of the program in different times and validate the presence of certain filenames.

## Conclusion

The `file-finder` program will use C++ STL abstractions to execute a substring search in filenames using multiple threads and proper locking and will periodically output the result and reset the container while the scanner continues to match filenames.

