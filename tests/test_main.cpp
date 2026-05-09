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
            std::cout << "--Run Scans Arg Found" << std::endl;
        }
    }
    return RUN_ALL_TESTS();
}