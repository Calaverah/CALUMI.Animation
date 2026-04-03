//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CppUnitTest.h"
#include "../CALUMI.Animation/include/math/Math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CALUMI::Math;

namespace CALUMIAnimationUnitTests
{

	TEST_CLASS(CALUMI_MathQuaternion)
	{
	public:
		/// <summary>
		/// https://articulatedrobotics.xyz/tools/rotation-calculator/
		/// is so far the most consistent and accurate quaternion calculator
		/// please use this for tests as the current first link on google for quaternion to euler calculations is flawed
		/// </summary>
		TEST_METHOD(DefaultConstruction)
		{
			Quaternion quaternion;
			Assert::IsTrue(quaternion.getX() == 0 && quaternion.getY() == 0 && quaternion.getZ() == 0 && quaternion.getW() == 1.0);
		}
		TEST_METHOD(AxisAngleConstruction)
		{
			Quaternion quaternion1(Vector3{ 1.0,2.0,3.0 }, static_cast<float>(ToRadians(50.0)));
			Quaternion quaternion2(0.1129495, 0.225899, 0.3388484, 0.9063078);
			Assert::IsTrue(quaternion1.AreEqual(quaternion2,0.0000001f));
		}
		TEST_METHOD(DoubleAxisAngleConstruction)
		{
			Quaternion quaternion1(Vector3D{ 1.0,2.0,3.0 }, static_cast<float>(ToRadians(50.0)));
			Quaternion quaternion2(0.1129495, 0.225899, 0.3388484, 0.9063078);
			Assert::IsTrue(quaternion1.AreEqual(quaternion2, 0.0000001f));
		}
		TEST_METHOD(Identity)
		{
			Quaternion quaternion;
			Assert::IsTrue(quaternion == quaternion.Identity);
		}
		TEST_METHOD(Addition1_E)
		{
			Quaternion quaternion1(1.0f, 1.0f, 1.0f, 1.0f);
			Quaternion quaternion2(1.0f, 1.0f, 1.0f, 1.0f);
			Quaternion quaternion3(2.0f, 2.0f, 2.0f, 2.0f);
			
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
			Assert::IsTrue(quaternion3.AreEqual(quaternion1 + quaternion2, 0.0000001f));
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
			Assert::IsFalse(quaternion.AreEqual(quaternion.Conjugate(), 0.0000001f));
		}
		TEST_METHOD(Conjugate1_E)
		{
			Quaternion quaternion1(1.0, 2.0, 3.0, 4.0);
			Quaternion quaternion2(-1.0, -2.0, -3.0, 4.0);
			Assert::IsTrue(quaternion1.AreEqual(quaternion2.Conjugate(), 0.0000001f));
		}
		TEST_METHOD(Conjugate2_E)
		{
			Quaternion quaternion1(1.0, 2.0, 3.0, 4.0);
			Quaternion quaternion2 = quaternion1.Conjugate();
			Assert::IsTrue(quaternion1.AreEqual(quaternion2.Conjugate(), 0.0000001f));
		}
		TEST_METHOD(AreSameRotation1_E)
		{
			Quaternion quaternion1(1.0, 2.0, 3.0, 4.0);
			Quaternion quaternion2 = -quaternion1;
			Assert::IsTrue(quaternion1.AreSameRotation(quaternion2, 0.0000001f));
		}
		TEST_METHOD(AreSameRotation1_F)
		{
			Quaternion quaternion1(1.0, 2.0, 3.0, 4.0);
			Quaternion quaternion2 = quaternion1.Conjugate();
			Assert::IsFalse(quaternion1.AreSameRotation(quaternion2, 0.0000001f));
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
			Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
			Assert::IsTrue(quaternion2.AreEqual(quaternion1 * quaternion2, 0.0000001f));
		}
		TEST_METHOD(Multiplication3_E)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
			Assert::IsTrue(quaternion2.AreEqual(quaternion1 * quaternion2, 0.0000001f));
		}
		TEST_METHOD(Multiplication4_E)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
			Assert::IsTrue(quaternion2.AreSameRotation( quaternion1 * -quaternion2, 0.0000001f));
		}
		TEST_METHOD(Multiplication4_F)
		{
			Quaternion quaternion1(CLA_SQRT1_2, 0.0, 0.0, CLA_SQRT1_2);
			Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
			Assert::IsFalse(quaternion2.AreSameRotation(quaternion1 * -quaternion2, 0.0000001f));
		}
		TEST_METHOD(Division1_E)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
			Assert::IsTrue(quaternion1.AreEqual(quaternion2 / quaternion2, 0.0000001f));
		}
		TEST_METHOD(Division2_E)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
			Assert::IsTrue(quaternion2.AreEqual(quaternion2 / quaternion1, 0.0000001f));
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
		TEST_METHOD(AxisAngleMultiplication1_E)
		{
			Quaternion quaternion1(Vector3D(0.0, 0.0, 1.0), ToRadians(180.0)); //0,0,1,0
			Quaternion quaternion2(Vector3D(0.0, 1.0, 0.0), ToRadians(30.0)); //0,0.258819,0,0.9659258
			Quaternion expectedOut(-0.258819, 0.0, 0.965926, 0.0); //180 on Z axis
			//std::cout << (quaternion1 * quaternion2).ToString() << std::endl;
			Assert::IsTrue(expectedOut.AreSameRotation(quaternion1 * quaternion2, 0.0000001f));
		}
		TEST_METHOD(AxisAngleMultiplication2_E)
		{
			Quaternion quaternion1(Vector3D(0.0, 0.0, 1.0), ToRadians(180.0)); //0,0,1,0
			Quaternion quaternion2(Vector3D(0.0, 1.0, 0.0), ToRadians(30.0)); //0,0.258819,0,0.9659258
			Quaternion expectedOut = quaternion1 * quaternion2;
			Quaternion result;
			RotateQuaternionByAxisAngleC(&quaternion2, &result, 0.0, 0.0, 1.0, static_cast<float>(ToRadians(180.0)));
			Assert::IsTrue(expectedOut.AreSameRotation(result, 0.0000001f));
		}

		TEST_METHOD(AngleDistance1)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(1.0f, 0.0f, 0.0f, 0.0f);
			float diff = quaternion1.AngularDistance(quaternion2);

			Assert::AreEqual(static_cast<float>(ToRadians(180.0f)), diff);
		}

		TEST_METHOD(AngleDistance2)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(-1.0f, 0.0f, 0.0f, 0.0f);
			float diff = quaternion1.AngularDistance(quaternion2);

			Assert::AreEqual(static_cast<float>(ToRadians(180.0f)), diff);
		}

		TEST_METHOD(AngleDistance3)
		{
			Quaternion quaternion1;
			Quaternion quaternion2(Vector3D(1.0, 1.0, 0.5), ToRadians(90.0));
			float diff = quaternion1.AngularDistance(quaternion2);

			Assert::AreEqual(static_cast<float>(ToRadians(90.0f)), diff);
		}

		TEST_METHOD(Slerp1)
		{
			Quaternion q1;
			Quaternion q2(Vector3D(0.0, 1.0, 0.0), ToRadians(90.0));
			Quaternion expected(Vector3D(0.0, 1.0, 0.0), ToRadians(45.0));
			Quaternion result = q1.Slerp(q2, 0.5f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; Result ");
			Logger::WriteMessage(result.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(result,0.001f));
		}

		TEST_METHOD(Slerp2)
		{
			Quaternion q1;
			Quaternion q2(Vector3D(0.0, 1.0, 0.0), ToRadians(178.0));
			Quaternion expected(Vector3D(0.0, 1.0, 0.0), ToRadians(89.0));
			Quaternion result = q1.Slerp(q2, 0.5f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());
			
			Logger::WriteMessage("; Result ");
			Logger::WriteMessage(result.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(result, 0.001f));
		}

		TEST_METHOD(EulerXYZ)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::XYZ);
			Quaternion expected(0.3919038f, 0.3604234f, 0.4396797f, 0.7233174f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; Result ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}

		TEST_METHOD(EulerXZY)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::XZY);
			Quaternion expected(0.02226f, 0.200562f, 0.531976f, 0.8223632f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; Result ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}

		TEST_METHOD(EulerYXZ)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::YXZ);
			Quaternion expected(0.531976f, 0.02226f, 0.200562f, 0.8223632f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; Result ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}

		TEST_METHOD(EulerYZX)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::YZX);
			Quaternion expected(0.439680f, 0.391904f, 0.360423f, 0.723317f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; \nResult ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}

		TEST_METHOD(EulerZXY)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::ZXY);
			Quaternion expected(0.360423f, 0.439680f, 0.391904f, 0.723317f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; \nResult ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}

		TEST_METHOD(EulerZYX)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::ZYX);
			Quaternion expected(0.200562f, 0.531976f, 0.022260f, 0.822363f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; \nResult ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}

		TEST_METHOD(EulerXYX)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::XYX);
			Quaternion expected(0.527203f, 0.495722f, -0.065263f, 0.687064f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; \nResult ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}

		TEST_METHOD(EulerXZX)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::XZX);
			Quaternion expected(0.527203f, 0.065263f, 0.495722f, 0.687064f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; \nResult ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}

		TEST_METHOD(EulerYXY)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::YXY);
			Quaternion expected(0.495722f, 0.527203f, 0.065263f, 0.687064f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; \nResult ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}

		TEST_METHOD(EulerYZY)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::YZY);
			Quaternion expected(-0.065263f, 0.527203f, 0.495722f, 0.687064f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; \nResult ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}

		TEST_METHOD(EulerZXZ)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::ZXZ);
			Quaternion expected(0.495722f, -0.065263f, 0.527203f, 0.687064f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; \nResult ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}

		TEST_METHOD(EulerZYZ)
		{
			Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::ZYZ);
			Quaternion expected(0.065263f, 0.495722f, 0.527203f, 0.687064f);

			Logger::WriteMessage("Expected ");
			Logger::WriteMessage(expected.ToString().c_str());

			Logger::WriteMessage("; \nResult ");
			Logger::WriteMessage(q1.ToString().c_str());

			Assert::IsTrue(expected.AreEqual(q1, 0.001));
		}


	};

	TEST_CLASS(CALUMI_MathVector3)
	{
	public:

		TEST_METHOD(DefaultConstruction)
		{
			Vector3 vec;
			Assert::IsTrue(vec.getX() == 0 && vec.getY() == 0 && vec.getZ() == 0);
		}
		TEST_METHOD(FloatConstruction)
		{
			Vector3 vec(1.0f,1.0f,1.0f);
			Assert::IsTrue(vec.getX() == 1.0f && vec.getY() == 1.0f && vec.getZ() == 1.0f);
		}
		TEST_METHOD(FloatConstruction_2)
		{
			Vector3 vec(1.0f, 1.0f, 1.0f);
			Assert::IsTrue(vec.getX() == 1.0 && vec.getY() == 1.0 && vec.getZ() == 1.0);
		}
		TEST_METHOD(FloatConstruction_3)
		{
			Vector3 vec(1.0f, 1.0f, 1.0f);
			Quaternion quaternion1(vec, static_cast<float>(ToRadians(50.0)));
			Assert::IsTrue(vec.getX() == 1.0 && vec.getY() == 1.0 && vec.getZ() == 1.0);
			
		}
	};








}