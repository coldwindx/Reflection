#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "clang-c/Index.h"

using std::string;
using std::unordered_map;
using std::vector;
namespace reflect
{
    struct MetaData
    {
        string key;
        string value;
    };

    class MetaCompiler
    {
    public:
        MetaCompiler(const char *source_filename);
        ~MetaCompiler();
        unordered_map<string, vector<MetaData>> compile();

    protected:
        vector<CXCursor> getMetaCursors();
        unordered_map<string, vector<MetaData>> getMetaDatas(const vector<CXCursor> &metaCursors);
        static string getClangString(const CXString &str);

    private:
        static const vector<const char *> arguments;
        CXIndex index;
        CXTranslationUnit transaction;
    };
}