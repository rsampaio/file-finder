#include "file-finder.hpp"

int main(int argc, char const *argv[])
{
    std::vector<std::string> substr_list;
    std::string directory;

    if (argc < 3)
    {
        std::cout << "usage:" << std::endl;
        std::cout << "\tfile-finder <directory> [<substring> ...]" << std::endl;
        exit(-1);
    }

    directory = argv[1];

    for (int i = 2; i < argc; i++)
    {
        substr_list.push_back(argv[i]);
    }

    file_finder ff(directory, substr_list);
    ff.start_threads();

    return 0;
}
