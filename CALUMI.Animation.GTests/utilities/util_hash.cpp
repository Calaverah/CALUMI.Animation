#include <gtest/gtest.h>
#include "utilities/CALUMI_Hashes.h"
#include "../Common.h"

#define GTEST(x) TEST(HashUtil, x)

using namespace CALUMI;

GTEST(KnownHashScan)
{
    Utilities::StringList errorList;

    EXPECT_NO_THROW(errorList = Utilities::HashRegistry::Get().testKnownHashes());

    EXPECT_EQ(errorList.size(), 0);
    if (!errorList.empty())
    {
        std::cout << _BRED("Hash Errors:") << std::endl;
        for (uint32_t i = 0; i < errorList.size(); i++)
        {
            std::cout << errorList.c_str(i) << std::endl;
        }
    }
    else
    {
        std::cout << _GREEN("No Hash Errors") << std::endl;
    }
}

GTEST(RegisterHash)
{
    const auto hash1 = Utilities::HashRegistry::Get().registerHash("TestxyzHash1");
    const auto hash2 = Utilities::HashRegistry::Get().registerHash("TestxyzHash2");
    const auto hash3 = Utilities::HashRegistry::Get().registerHash("TestxyzHash3");
    const auto hash4 = Utilities::HashRegistry::Get().registerHash("TestxyzHash4");
    const auto hash5 = Utilities::HashRegistry::Get().registerHash("TestxyzHash5");

    const auto outputList = Utilities::HashRegistry::Get().registeredStrings();

    EXPECT_EQ(outputList.size(), 5);
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