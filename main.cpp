#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <fstream>
#include <clang-c/Index.h>
#include <reflect.h>

using namespace std;
using namespace reflect;

#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    Generator generator(
        "/home/zhulin/workspace/Reactor/include/controller/HelloController.h",
        // "/home/zhulin/workspace/Reflection/source.hpp",
                        "/home/zhulin/workspace/Reflection/generated/./address.generated.h");
    return 0;
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " <source_path> <target_path>" << endl;
        exit(EXIT_FAILURE);
    }
    printf("Reflect C++ Meta-Compiler\n");
    printf("Source path: %s\n", argv[1]);
    printf("Target path: %s\n", argv[2]);

    string workspace = argv[1];
    string target = argv[2];

    auto dfs = [&](auto &&dfs, const string &source, const string &target, string &dir, void *visitor) -> void
    {
        struct dirent **names;
        int n = scandir((workspace + "/" + dir).c_str(), &names, NULL, alphasort);
        if (n < 0)
            throw std::runtime_error("scandir error");

        for (int m = 0; m < n; ++m)
        {
            if (0 == strcmp(names[m]->d_name, ".") || 0 == strcmp(names[m]->d_name, ".."))
                continue;

            string subpath = dir + "/" + names[m]->d_name;

            // Check if the entry is a directory
            if (names[m]->d_type == DT_DIR)
            {
                dfs(dfs, workspace, target, subpath, visitor); // Recursive call for directories
            }
            if (names[m]->d_type == DT_REG)
            {
                string generated_name = Generator::replaceSrcName(subpath, ".h");
                if (generated_name.size() == 0)
                    generated_name = Generator::replaceSrcName(subpath.c_str(), ".hpp");
                if (0 < generated_name.size())
                {
                    Generator generator((workspace + "/" + subpath).c_str(), (target + "/" + generated_name).c_str());
                }
            }
        }
        free(names);
    };

    string dir = ".";
    dfs(dfs, workspace, target, dir, nullptr);
    return 0;
}