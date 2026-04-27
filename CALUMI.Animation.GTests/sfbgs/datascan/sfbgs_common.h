// ReSharper disable CppUnusedIncludeDirective
// ReSharper disable CppDFANotInitializedField
#pragma once
#include <gtest/gtest.h>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include <AnimStarfield>
#include "../../Common.h"

#define ISKNOWNVERSION(x)  x == 5

class SFBGS_Data
{
public:
    // 1. Static method to access the single instance
    static SFBGS_Data& get() {
        // C++11 guarantees this is initialized only once and is thread-safe
        static SFBGS_Data instance;
        return instance;
    }

    // 2. Delete copy constructor and assignment operator to prevent duplicates
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
    // 3. Private constructor to prevent direct instantiation
    SFBGS_Data() = default;
    ~SFBGS_Data() = default;
};