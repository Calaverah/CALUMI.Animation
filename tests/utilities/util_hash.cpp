#include <filesystem>
#include <gtest/gtest.h>
#include "../../include/utilities/CALUMI_Hashes.h"
#include "../Common.h"
#include "../sfbgs/datascan/sfbgs_common.h"

#define GTEST(x) TEST(HashUtil, x)

using namespace CALUMI;

GTEST(KnownHashScan)
{
    Utilities::StringList errorList;

#ifdef DEBUG_BUILD
    EXPECT_NO_THROW(errorList = Utilities::HashRegistry::Get().testKnownHashes());
#endif

    EXPECT_EQ(errorList.size(), 0);
    if (!errorList.empty())
    {
        std::cout << _BRED("Hash Errors:") << "\n";
        for (uint32_t i = 0; i < errorList.size(); i++)
        {
            std::cout << errorList.c_str(i) << "\n";
        }
    }
    else
    {
        std::cout << _GREEN("No Hash Errors") << "\n";
    }
}

GTEST(RegisterHash)
{
    uint8_t size = Utilities::HashRegistry::Get().registeredStrings().size();

    const auto hash1 = Utilities::HashRegistry::Get().registerHash("TestxyzHash1");
    const auto hash2 = Utilities::HashRegistry::Get().registerHash("TestxyzHash2");
    const auto hash3 = Utilities::HashRegistry::Get().registerHash("TestxyzHash3");
    const auto hash4 = Utilities::HashRegistry::Get().registerHash("TestxyzHash4");
    const auto hash5 = Utilities::HashRegistry::Get().registerHash("TestxyzHash5");

    const auto outputList = Utilities::HashRegistry::Get().registeredStrings();

    EXPECT_EQ(outputList.size(), size + 5);
    std::set<std::string> registeredStrings;
    for (uint32_t i = 0; i < outputList.size(); i++)
        registeredStrings.insert(outputList.c_str(i));

    EXPECT_TRUE(registeredStrings.contains("TestxyzHash1"));
    EXPECT_TRUE(registeredStrings.contains("TestxyzHash2"));
    EXPECT_TRUE(registeredStrings.contains("TestxyzHash3"));
    EXPECT_TRUE(registeredStrings.contains("TestxyzHash4"));
    EXPECT_TRUE(registeredStrings.contains("TestxyzHash5"));

    EXPECT_EQ(hash1, Utilities::HashRegistry::Get().registerHash("TestxyzHash1"));
    EXPECT_EQ(hash2, Utilities::HashRegistry::Get().registerHash("TestxyzHash2"));
    EXPECT_EQ(hash3, Utilities::HashRegistry::Get().registerHash("TestxyzHash3"));
    EXPECT_EQ(hash4, Utilities::HashRegistry::Get().registerHash("TestxyzHash4"));
    EXPECT_EQ(hash5, Utilities::HashRegistry::Get().registerHash("TestxyzHash5"));

    EXPECT_EQ(outputList.size(), Utilities::HashRegistry::Get().registeredStrings().size());
}

GTEST(Scan_Anim_Hash)
{
    std::set<uint32_t> unkHashSet;

    if (!ShouldRunHashScan)
        GTEST_SKIP();

    const bool inputExists = std::filesystem::exists(SFBGS_Data::get().inputPath);
    ASSERT_TRUE(inputExists);

    if (!inputExists)
        GTEST_SKIP() << "File path: "<< SFBGS_Data::get().inputPath << " could not be found.";

    std::filesystem::path dataPath;

    if (std::ifstream dataDirInput(SFBGS_Data::get().inputPath); dataDirInput.is_open())
    {
        std::stringstream buffer;
        buffer << dataDirInput.rdbuf();

        const std::string content = buffer.str();

        dataDirInput.close();

        ASSERT_FALSE(content.empty());

        dataPath = content;
    }
    else
    {
        GTEST_SKIP() << "Getting filepath failed.";
    }

    if (!std::filesystem::exists(dataPath))
        GTEST_SKIP() << "Data Path: " << dataPath.string() << " Not Found";

    if (!std::filesystem::is_directory(dataPath))
        GTEST_SKIP() << "Data Path: " << dataPath.string() << " Is Not Directory";

    const std::filesystem::path meshesPath = dataPath.string() + "/meshes";

    std::cout << "\tScanning " << _YELLOW(meshesPath.string()) << " for .af files\n";

    uint64_t filesFound = 0;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(meshesPath))
    {
        if (!std::filesystem::is_regular_file(entry.path()) || entry.path().extension() != SFBGS_Data::get().afExt)
            continue;

        filesFound++;

        SFBGS::Animation anim;

        ASSERT_FALSE(anim.readFromFile(entry.path().c_str()).hasError());

        if (!anim.amendedHashSet().empty())
        {
            for (int i = 0; i < anim.amendedHashSet().size(); ++i)
            {
                if (const auto hash = anim.amendedHashSet().at(i); !Utilities::HashRegistry::Get().isKnownHash(hash))
                    unkHashSet.insert(hash);
            }
        }
    }


    std::cout << "\tUnknowns:\n";
    for (const auto& entry : unkHashSet)
    {
        std::cout << "\tUnk Hash: " << entry << "\n";
    }

    std::cout << "\tScanned " << _BMAGENTA(filesFound) << " files from " << _YELLOW(meshesPath.string()) << "\n";

    const std::filesystem::path niNodeListPath = "D:/output/ninodes_names.dat";

    if (std::ifstream niNodeStream(niNodeListPath); niNodeStream.is_open())
    {
        std::string line;
        while (std::getline(niNodeStream, line))
        {
            const auto hash = Utilities::HashRegistry::CalculateHash(line.c_str());
            if (unkHashSet.contains(hash))
            {
                std::cout << "\t\t{" << hash << "UL, \"" << line << "},\"\n";
            }
        }
    }

}