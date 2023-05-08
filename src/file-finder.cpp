#include "file-finder.hpp"

file_finder::file_finder(std::string d, std::vector<std::string> s)
{
    directory = d;
    substrings = s;
}

file_finder::~file_finder()
{
}

void file_finder::print_results()
{
    std::unique_lock lock(mutex_);
    for (auto r : results)
    {
        std::cout << r << std::endl;
    }
    results.clear();
}

void file_finder::dumper()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (results.size() > 0)
        print_results();

    bool threads_finished = true;

    for (auto &t : walker_threads)
    {
        if (t.joinable())
            threads_finished = false;
    }

    if (threads_finished)
        exit(0);
}

void file_finder::start_threads()
{
    std::thread wct(&file_finder::wait_command, this);

    for (auto ss : substrings)
    {
        std::thread th(&file_finder::walk_directory, this, directory, ss);
        walker_threads.push_back(move(th));
    }

    std::thread dt(&file_finder::dumper, this);

    for (auto &t : walker_threads)
    {
        t.join();
    }

    wct.join();
    dt.join();
}

void file_finder::store_result(std::string s)
{
    std::unique_lock lock(mutex_);
    results.push_back(s);
}

void file_finder::walk_directory(std::string d, std::string s)
{
    std::filesystem::path dir{d};
    if (!std::filesystem::is_directory(dir))
    {
        std::cerr << dir << " is not a directory" << std::endl;
        std::cerr.flush();
        return;
    }

    for (auto const &dir_ent : std::filesystem::recursive_directory_iterator(dir))
    {
        if (std::filesystem::is_regular_file(dir_ent))
        {
            if (dir_ent.path().filename() == s)
            {
                store_result(dir_ent.path().string());
            }
        }
    }
}

void file_finder::wait_command()
{
    std::string command;

    while (true)
    {
        std::cerr << "> ";
        std::cin >> command;
        if (command == "dump")
        {
            print_results();
        }
        else if (command == "exit")
        {
            exit(0);
        }
        else
        {
            std::cerr << "command not found" << std::endl;
        }
    }
}