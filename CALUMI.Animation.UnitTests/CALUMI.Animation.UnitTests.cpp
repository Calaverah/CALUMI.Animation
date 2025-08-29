#include "pch.h"
#include "CppUnitTest.h"
#include "..\CALUMI.Animation\CALUMI_Math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CALUMIAnimationUnitTests
{
	TEST_CLASS(CALUMIAnimationUnitTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			CALUMI::Math::Vector3 vec(1.0,1.0,1.0);
			double assertLength = 1.7320508075688772935274463415059;
			Assert::AreEqual((float)assertLength, vec.Length(), 0.00001f);

		}
	};
}
