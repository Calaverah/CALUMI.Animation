#include <gtest/gtest.h>
#include "Common.h"

int main (int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    for (int i = 1; i < argc; i++)
    {
        if (SCOMPARE(argv[i], "--scans") == 0)
        {
            ShouldRunScans = true;
            std::cout << "--Run Scans Arg Found\n";
        }

        if (SCOMPARE(argv[i], "--hash") == 0)
        {
            ShouldRunHashScan = true;
            std::cout << "--Run Hash Arg Found\n";
        }
    }
    return RUN_ALL_TESTS();
}