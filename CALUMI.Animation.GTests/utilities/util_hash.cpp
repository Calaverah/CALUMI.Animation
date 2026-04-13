#include <gtest/gtest.h>
#include "utilities\CALUMI_Hashes.h"
#include "../Common.h"

#define GTEST(x) TEST(HashUtil, x)

using namespace CALUMI;

GTEST(KnownHashScan)
{
    Utilities::StringList errorList;

    EXPECT_NO_THROW(errorList = Utilities::HashRegistry::getInstance().testKnownHashes());

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
    const auto hash1 = Utilities::HashRegistry::getInstance().registerHash("TestxyzHash1");
    const auto hash2 = Utilities::HashRegistry::getInstance().registerHash("TestxyzHash2");
    const auto hash3 = Utilities::HashRegistry::getInstance().registerHash("TestxyzHash3");
    const auto hash4 = Utilities::HashRegistry::getInstance().registerHash("TestxyzHash4");
    const auto hash5 = Utilities::HashRegistry::getInstance().registerHash("TestxyzHash5");

    const auto outputList = Utilities::HashRegistry::getInstance().getRegisteredStrings();

    EXPECT_EQ(outputList.size(), 5);
    EXPECT_STREQ(outputList.c_str(0), "TestxyzHash1");
    EXPECT_STREQ(outputList.c_str(1), "TestxyzHash2");
    EXPECT_STREQ(outputList.c_str(2), "TestxyzHash3");
    EXPECT_STREQ(outputList.c_str(4), "TestxyzHash4");
    EXPECT_STREQ(outputList.c_str(3), "TestxyzHash5");

    EXPECT_EQ(hash1, Utilities::HashRegistry::getInstance().registerHash("TestxyzHash1"));
    EXPECT_EQ(hash2, Utilities::HashRegistry::getInstance().registerHash("TestxyzHash2"));
    EXPECT_EQ(hash3, Utilities::HashRegistry::getInstance().registerHash("TestxyzHash3"));
    EXPECT_EQ(hash4, Utilities::HashRegistry::getInstance().registerHash("TestxyzHash4"));
    EXPECT_EQ(hash5, Utilities::HashRegistry::getInstance().registerHash("TestxyzHash5"));

    EXPECT_EQ(outputList.size(), Utilities::HashRegistry::getInstance().getRegisteredStrings().size());
}