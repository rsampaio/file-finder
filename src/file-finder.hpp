#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <filesystem>
#include <chrono>
#include <shared_mutex>
#include <mutex>
#include <cstdlib>

class file_finder
{
private:
    std::vector<std::string> substrings;
    std::vector<std::string> results;
    std::vector<std::thread> walker_threads;
    std::string directory;

    std::shared_mutex mutex_;
    int sleep_before_dumping_in_seconds;

public:
    file_finder(std::string, std::vector<std::string>);
    ~file_finder();
    void store_result(std::string);
    void walk_directory(std::string, std::string);
    void wait_command();
    void print_results();
    void dumper();
    void start_threads();
};