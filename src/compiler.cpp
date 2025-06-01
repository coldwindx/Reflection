#include "compiler.h"

namespace reflect
{
    const vector<const char *> MetaCompiler::arguments = {"c++",
                                                          "-std=c++17",
                                                          "-D __clang__",
                                                          "-D __META_PARSER__"};

    MetaCompiler::MetaCompiler(const char *source_filename)
    {
        index = clang_createIndex(0, 0);
        transaction = clang_parseTranslationUnit(index, source_filename, arguments.data(), arguments.size(), nullptr, 0, CXTranslationUnit_None);
        if (!transaction)
            throw std::runtime_error("Failed to parse translation unit");
    }

    MetaCompiler::~MetaCompiler()
    {
        clang_disposeTranslationUnit(transaction);
        clang_disposeIndex(index);
    }

    unordered_map<string, vector<MetaData>> MetaCompiler::compile()
    {
        vector<CXCursor> metaCursors = getMetaCursors();
        auto metaDatas = getMetaDatas(metaCursors);
        return metaDatas;
    }

    vector<CXCursor> MetaCompiler::getMetaCursors()
    {
        CXCursor cursor = clang_getTranslationUnitCursor(transaction);
        auto visitor = [](CXCursor cursor, CXCursor parent, CXClientData data)
        {
            auto cursors = reinterpret_cast<vector<CXCursor> *>(data);
            if (clang_getCursorKind(cursor) == CXCursor_AnnotateAttr)
            {
                if (getClangString(clang_getCursorSpelling(cursor)) == "reflect-class")
                    cursors->push_back(parent);
            }
            return CXChildVisit_Recurse;
        };

        vector<CXCursor> metaCursors;
        clang_visitChildren(cursor, visitor, reinterpret_cast<CXClientData>(&metaCursors));
        return metaCursors;
    }

    unordered_map<string, vector<MetaData>> MetaCompiler::getMetaDatas(const vector<CXCursor> &metaCursors)
    {
        unordered_map<string, vector<MetaData>> metaDatas;
        for (auto &cursor : metaCursors)
        {
            auto visitor = [](CXCursor cursor, CXCursor parent, CXClientData data)
            {
                auto raw = reinterpret_cast<vector<MetaData> *>(data);
                if (clang_getCursorKind(cursor) == CXCursor_AnnotateAttr)
                {
                    MetaData meta;
                    if (clang_getCursorKind(parent) == CXCursor_FieldDecl)
                        meta.key = "field";
                    else if (clang_getCursorKind(parent) == CXCursor_CXXMethod)
                        meta.key = "method";
                    else
                        return CXChildVisit_Recurse;
                    meta.value = getClangString(clang_getCursorSpelling(parent));
                    raw->push_back(meta);
                }
                return CXChildVisit_Recurse;
            };
            vector<MetaData> data;
            clang_visitChildren(cursor, visitor, reinterpret_cast<CXClientData>(&data));
            metaDatas[getClangString(clang_getCursorSpelling(cursor))] = std::move(data);
        }
        return metaDatas;
    }

    string MetaCompiler::getClangString(const CXString &str)
    {
        string result = clang_getCString(str);
        clang_disposeString(str);
        return result;
    }
}