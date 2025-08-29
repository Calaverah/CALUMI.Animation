#include "pch.h"
#include "CppUnitTest.h"
#include "..\CALUMI.Animation\CALUMI_Math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CALUMI::Math;

namespace CALUMIAnimationUnitTests
{

	TEST_CLASS(CALUMI_MathQuaternion)
	{
	public:

		TEST_METHOD(DefaultConstruction)
		{
			Quaternion quaternion;
			Assert::IsTrue(quaternion.x == 0 && quaternion.y == 0 && quaternion.z == 0 && quaternion.w == 1.0);
		}
		TEST_METHOD(AxisAngleConstruction)
		{
			Quaternion quaternion1(Vector3{ 1.0,2.0,3.0 }, ToRadians(50));
			Quaternion quaternion2(0.1129495,0.225899,0.3388484,0.9063078);
			Assert::IsTrue(quaternion1.AreEqual(quaternion2,0.0000001));
		}
		TEST_METHOD(DoubleAxisAngleConstruction)
		{
			Quaternion quaternion1(Vector3D{ 1.0,2.0,3.0 }, ToRadians(50));
			Quaternion quaternion2(0.1129495, 0.225899, 0.3388484, 0.9063078);
			Assert::IsTrue(quaternion1.AreEqual(quaternion2, 0.0000001));
		}
		TEST_METHOD(Identity)
		{
			Quaternion quaternion;
			Assert::IsTrue(quaternion == quaternion.Identity);
		}
		TEST_METHOD(Addition1_E)
		{
			Quaternion quaternion1(1.0, 1.0, 1.0, 1.0);
			Quaternion quaternion2(1.0, 1.0, 1.0, 1.0);
			Quaternion quaternion3(2.0, 2.0, 2.0, 2.0);
			Assert::IsTrue(quaternion3 == quaternion1 + quaternion2);
		}
		TEST_METHOD(Addition1_N)
		{
			Quaternion quaternion1(0.0, 0.0, 1.0, 1.0);
			Quaternion quaternion2(1.0, 1.0, 1.0, 1.0);
			Quaternion quaternion3(2.0, 2.0, 2.0, 2.0);
			Assert::IsTrue(quaternion3 != quaternion1 + quaternion2);
		}
		TEST_METHOD(Addition2_E)
		{
			Quaternion quaternion1(1.0, 1.5, 1.0, 1.0);
			Quaternion quaternion2(1.01, 1.0, 0.0, 2.0);
			Quaternion quaternion3(0.8480776, 1.0622199, 0.4364358, 1.2515682);
			Assert::IsTrue(quaternion3.AreEqual(quaternion1 + quaternion2, 0.0000001));
		}
		TEST_METHOD(Addition2_N)
		{
			Quaternion quaternion1(1.0, 1.0, 1.0, 1.0);
			Quaternion quaternion2(1.0, 1.0, 1.001, 1.0);
			Quaternion quaternion3(2.0, 2.0, 2.0, 2.0);
			Assert::IsTrue(quaternion3 != quaternion1 + quaternion2);
		}
		TEST_METHOD(Equals1_F)
		{
			Quaternion quaternion1(1.0, 1.0, 1.0, 1.0);
			Quaternion quaternion2(1.0, 1.0, 1.001, 1.0);
			Quaternion quaternion3(2.0, 2.0, 2.0, 2.0);
			Assert::IsFalse(quaternion3 == quaternion1 + quaternion2);
		}
		TEST_METHOD(NotEquals1_F)
		{
			Quaternion quaternion1(1.0, 1.5, 1.0, 1.0);
			Quaternion quaternion2(1.01, 1.0, 0.0, 2.0);
			Quaternion quaternion3 = quaternion1 + quaternion2;
			Assert::IsFalse(quaternion3 != quaternion1 + quaternion2);
		}
		TEST_METHOD(Conjugate1_N)
		{
			Quaternion quaternion(1.0, 2.0, 3.0, 4.0);
			Assert::IsFalse(quaternion.AreEqual(quaternion.Conjugate(), 0.0000001));
		}
		TEST_METHOD(Conjugate1_E)
		{
			Quaternion quaternion1(1.0, 2.0, 3.0, 4.0);
			Quaternion quaternion2(-1.0, -2.0, -3.0, 4.0);
			Assert::IsTrue(quaternion1.AreEqual(quaternion2.Conjugate(), 0.0000001));
		}
		TEST_METHOD(Conjugate2_E)
		{
			Quaternion quaternion1(1.0, 2.0, 3.0, 4.0);
			Quaternion quaternion2 = quaternion1.Conjugate();
			Assert::IsTrue(quaternion1.AreEqual(quaternion2.Conjugate(), 0.0000001));
		}
		TEST_METHOD(AreSameRotation1_E)
		{
			Quaternion quaternion1(1.0, 2.0, 3.0, 4.0);
			Quaternion quaternion2 = -quaternion1;
			Assert::IsTrue(quaternion1.AreSameRotation(quaternion2, 0.0000001));
		}
		TEST_METHOD(AreSameRotation1_F)
		{
			Quaternion quaternion1(1.0, 2.0, 3.0, 4.0);
			Quaternion quaternion2 = quaternion1.Conjugate();
			Assert::IsFalse(quaternion1.AreSameRotation(quaternion2, 0.0000001));
		}
		TEST_METHOD(Multiplication1_E)
		{
			Quaternion quaternion1;
			Quaternion quaternion2;
			Assert::IsTrue(quaternion1 == quaternion1 * quaternion2);
		}
		TEST_METHOD(Multiplication2_E)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(1.0,2.0,3.0,4.0);
			Assert::IsTrue(quaternion2.AreEqual(quaternion1 * quaternion2, 0.0000001));
		}
		TEST_METHOD(Multiplication3_E)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
			Assert::IsTrue(quaternion2.AreEqual(quaternion1 * quaternion2, 0.0000001));
		}
		TEST_METHOD(Multiplication4_E)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
			Assert::IsTrue(quaternion2.AreSameRotation( quaternion1 * -quaternion2, 0.0000001));
		}
		TEST_METHOD(Multiplication4_F)
		{
			Quaternion quaternion1(M_SQRT1_2,0.0,0.0,M_SQRT1_2);
			Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
			Assert::IsFalse(quaternion2.AreSameRotation(quaternion1 * -quaternion2, 0.0000001));
		}
		TEST_METHOD(Division1_E)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
			Assert::IsTrue(quaternion1.AreEqual(quaternion2 / quaternion2, 0.0000001));
		}
		TEST_METHOD(Division2_E)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
			Assert::IsTrue(quaternion2.AreEqual(quaternion2 / quaternion1, 0.0000001));
		}
		TEST_METHOD(Length1_E)
		{
			//Length should always be 1
			Quaternion quaternion(1.0, 2.0, 3.0, 4.0);
			Assert::AreEqual(1.0f,quaternion.Length(), 0.0000001f);
		}
		TEST_METHOD(Length2_E)
		{
			Quaternion quaternion(1.0, 2.0, 3.0, 4.0, false);
			Assert::AreEqual(5.4772255750f, quaternion.Length(), 0.0000001f);
		}
		TEST_METHOD(Length2_N)
		{
			Quaternion quaternion(1.0, 2.0, 3.0, 4.0, false);
			Assert::AreNotEqual(1.0f, quaternion.Length(), 0.0000001f);
		}
		TEST_METHOD(LengthSquared1_E)
		{
			//Length should always be 1
			Quaternion quaternion(1.0, 2.0, 3.0, 4.0);
			Assert::AreEqual(1.0f, quaternion.LengthSquared(), 0.000001f);
		}
		TEST_METHOD(LengthSquared2_E)
		{
			Quaternion quaternion(1.0, 2.0, 3.0, 4.0, false);
			Assert::AreEqual(30.0f, quaternion.LengthSquared(), 0.0000001f);
		}
		TEST_METHOD(LengthSquared2_N)
		{
			Quaternion quaternion(1.0, 2.0, 3.0, 4.0, false);
			Assert::AreNotEqual(1.0f, quaternion.LengthSquared(), 0.0000001f);
		}
		TEST_METHOD(Dot1_E)
		{
			Quaternion quaternion(1.0, 2.0, 3.0, 4.0, false);
			Assert::AreEqual(quaternion.Dot(quaternion), quaternion.LengthSquared(), 0.0000001f);
		}
		TEST_METHOD(Dot2_E)
		{
			Quaternion quaternion(2.251, 2.0, 3.0, 4.0, false);
			Assert::AreEqual(quaternion.Dot(Quaternion().Identity), 4.0f, 0.0000001f);
		}
	};










}