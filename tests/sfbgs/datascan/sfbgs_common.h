// ReSharper disable CppUnusedIncludeDirective
// ReSharper disable CppDFANotInitializedField
#pragma once
#include <gtest/gtest.h>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include "../../../include/AnimStarfield"
#include "../../Common.h"

#define ISKNOWNVERSION(x)  x == 5

class SFBGS_Data
{
public:
    static SFBGS_Data& get() {
        static SFBGS_Data instance;
        return instance;
    }

    SFBGS_Data(const SFBGS_Data&) = delete;
    SFBGS_Data& operator=(const SFBGS_Data&) = delete;

public:
    std::filesystem::path inputPath = "paths/sfbgs_exdatapath.txt";
    bool inputExists = false;
    std::filesystem::path dataPath;
    std::filesystem::path rigExt = ".rig";
    std::filesystem::path afExt = ".af";
    std::filesystem::path afxExt = ".afx";
    std::filesystem::path agxExt = ".agx";

private:
    SFBGS_Data() = default;
    ~SFBGS_Data() = default;
};