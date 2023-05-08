# file-finder

A finder for files, will find substrings in filenames

## Compile

```
make
```

## Run

```
./file-finder <directory> [<substring>...]
```

## Implementation Notes

- Since the directory iterator is not allocating any resource or opening any file descriptor I'm relying on the C++ abstraction to clean up resources
- Similar for the thread termination, since all threads are killed upon program termination I'm avoiding fetching the native_thread_handler and calling pthread_cancel myself and relying on the C++ STL abstraction for that
