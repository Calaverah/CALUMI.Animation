//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CppUnitTest.h"
#include "..\CALUMI.Animation\CALUMI_Math.h"
#include "..\CALUMI.Animation\CALUMI_Animation.h"
#include <iostream>

#define ROTATIONENTRY(input, f, x,y,z,deg) {CALUMI::Math::Quaternion rotation(Vector3(x, y, z), ToRadians(deg)); uint16_t frame = f; \
											CALUMI::UNIV::Rotation entry(frame,rotation);\
										   input.AddRotationEntry(entry);}

#define TRANSLATIONENTRY(input, f, x,y,z) { CALUMI::Math::Vector3D vec(x,y,z); uint16_t frame = f; \
											CALUMI::UNIV::Translation entry(frame,vec); \
											input.AddTranslationEntry(entry); }

#define SCALARENTRY(input, f, s) { uint16_t frame = f; float scalar = s; \
								   CALUMI::UNIV::Scalar entry(frame,scalar);\
								   input.AddScalarEntry(entry); }

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CALUMI::Math;

namespace CALUMIAnimationUnitTests
{

	TEST_CLASS(CALUMI_RotationSequence)
	{
	public:

		TEST_METHOD(RDP_1)
		{
			CALUMI::UNIV::AnimationBlock input;

			//source sq input
			ROTATIONENTRY(input, 0, 1.0f, 0.0f, 0.0f, 0.0);
			ROTATIONENTRY(input, 1, 1.0f, 0.0f, 0.0f, 0.0); //should be culled
			ROTATIONENTRY(input, 2, 1.0f, 0.0f, 0.0f, 0.0); //""
			ROTATIONENTRY(input, 3, 1.0f, 0.0f, 0.0f, 0.0); //""
			ROTATIONENTRY(input, 4, 1.0f, 0.0f, 0.0f, 0.0); //""
			ROTATIONENTRY(input, 5, 1.0f, 0.0f, 0.0f, 0.0);

			auto first = input.RotationSequence().at(0);
			auto last = input.RotationSequence().at(input.RotationSequence().size() - 1);

			Logger::WriteMessage("Original Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			input.ExecuteRDPReduction_Rotation();

			Logger::WriteMessage("New Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			Assert::IsTrue(input.RotationSequence().size() == 2 && first.IsIdentical(input.RotationSequence().at(0)) && last.IsIdentical(input.RotationSequence().at(input.RotationSequence().size()-1)));
		}

		TEST_METHOD(RDP_2)
		{
			CALUMI::UNIV::AnimationBlock input;

			//source sq input
			ROTATIONENTRY(input, 0, 1.0f, 0.0f, 0.0f, 0.0);
			ROTATIONENTRY(input, 1, 1.0f, 0.0f, 0.0f, 0.0);
			ROTATIONENTRY(input, 2, 1.0f, 0.0f, 0.0f, 90.0);
			ROTATIONENTRY(input, 3, 1.0f, 0.0f, 0.0f, 0.0);
			ROTATIONENTRY(input, 4, 1.0f, 0.0f, 0.0f, 0.0); //should be culled
			ROTATIONENTRY(input, 5, 1.0f, 0.0f, 0.0f, 0.0);

			auto first = input.RotationSequence().at(0);
			auto last = input.RotationSequence().at(input.RotationSequence().size() - 1);

			Logger::WriteMessage("Original Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			input.ExecuteRDPReduction_Rotation();

			Logger::WriteMessage("New Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			Assert::IsTrue(input.RotationSequence().size() == 5 && first.IsIdentical(input.RotationSequence().at(0)) && last.IsIdentical(input.RotationSequence().at(input.RotationSequence().size() - 1)));
		}

		TEST_METHOD(RDP_3)
		{
			CALUMI::UNIV::AnimationBlock input;

			//source sq input
			ROTATIONENTRY(input, 0, 1.0f, 0.0f, 0.0f, 0.0);
			ROTATIONENTRY(input, 1, 1.0f, 0.0f, 0.0f, 90.0);  //Should NOT be culled
			ROTATIONENTRY(input, 2, 1.0f, 0.0f, 0.0f, 180.0);
			ROTATIONENTRY(input, 3, 1.0f, 0.0f, 0.0f, 120.0);  
			ROTATIONENTRY(input, 4, 1.0f, 0.0f, 0.0f, 60.0); //should be culled
			ROTATIONENTRY(input, 5, 1.0f, 0.0f, 0.0f, 0.0);

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n",i,input.RotationSequence().at(i).Frame(),input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(0).RotationQuaternion().AngularDistance(input.RotationSequence().at(2).RotationQuaternion())).c_str());
			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(2).RotationQuaternion().AngularDistance(input.RotationSequence().at(4).RotationQuaternion())).c_str());

			auto first = input.RotationSequence().at(0);
			auto last = input.RotationSequence().at(input.RotationSequence().size() - 1);

			Logger::WriteMessage("\nOriginal Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			input.ExecuteRDPReduction_Rotation();

			Logger::WriteMessage("New Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" \n");

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.RotationSequence().at(i).Frame(), input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			Assert::IsTrue(input.RotationSequence().size() == 5 && first.IsIdentical(input.RotationSequence().at(0)) && last.IsIdentical(input.RotationSequence().at(input.RotationSequence().size() - 1)));
		}

		TEST_METHOD(RDP_4)
		{
			CALUMI::UNIV::AnimationBlock input;

			//source sq input
			ROTATIONENTRY(input, 0, 1.0f, 0.0f, 0.0f, 0.0);
			ROTATIONENTRY(input, 1, 1.0f, 0.0f, 0.0f, 60.0);  //Should NOT be culled
			ROTATIONENTRY(input, 3, 1.0f, 0.0f, 0.0f, 180.0);

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.RotationSequence().at(i).Frame(), input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(0).RotationQuaternion().AngularDistance(input.RotationSequence().at(2).RotationQuaternion())).c_str());
			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(2).RotationQuaternion().AngularDistance(input.RotationSequence().at(4).RotationQuaternion())).c_str());

			auto first = input.RotationSequence().at(0);
			auto last = input.RotationSequence().at(input.RotationSequence().size() - 1);

			Logger::WriteMessage("\nOriginal Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			input.ExecuteRDPReduction_Rotation();

			Logger::WriteMessage("New Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" \n");

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.RotationSequence().at(i).Frame(), input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			Assert::IsTrue(input.RotationSequence().size() == 3 && first.IsIdentical(input.RotationSequence().at(0)) && last.IsIdentical(input.RotationSequence().at(input.RotationSequence().size() - 1)));
		}

		TEST_METHOD(RDP_5)
		{
			CALUMI::UNIV::AnimationBlock input;

			//source sq input
			ROTATIONENTRY(input, 0, 1.0f, 0.0f, 0.0f, 0.0);
			ROTATIONENTRY(input, 1, 1.0f, 0.0f, 0.0f, 89.95);  //Should be culled
			ROTATIONENTRY(input, 2, 1.0f, 0.0f, 0.0f, 179.9);

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.RotationSequence().at(i).Frame(), input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(0).RotationQuaternion().AngularDistance(input.RotationSequence().at(2).RotationQuaternion())).c_str());
			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(2).RotationQuaternion().AngularDistance(input.RotationSequence().at(4).RotationQuaternion())).c_str());

			auto first = input.RotationSequence().at(0);
			auto last = input.RotationSequence().at(input.RotationSequence().size() - 1);

			Logger::WriteMessage("\nOriginal Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			input.ExecuteRDPReduction_Rotation();

			Logger::WriteMessage("New Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" \n");

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.RotationSequence().at(i).Frame(), input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			Assert::IsTrue(input.RotationSequence().size() == 2 && first.IsIdentical(input.RotationSequence().at(0)) && last.IsIdentical(input.RotationSequence().at(input.RotationSequence().size() - 1)));
		}

		TEST_METHOD(RDP_6)
		{
			CALUMI::UNIV::AnimationBlock input;

			//source sq input
			ROTATIONENTRY(input, 0, 1.0f, 0.0f, 0.0f, 0.0);
			ROTATIONENTRY(input, 2, 1.0f, 0.0f, 0.0f, 90.0);  
			ROTATIONENTRY(input, 4, 1.0f, 0.0f, 0.0f, 180.0);
			ROTATIONENTRY(input, 6, 1.0f, 0.0f, 0.0f, 270.0);
			ROTATIONENTRY(input, 8, 1.0f, 0.0f, 0.0f, 360.0);

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.RotationSequence().at(i).Frame(), input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(0).RotationQuaternion().AngularDistance(input.RotationSequence().at(2).RotationQuaternion())).c_str());
			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(2).RotationQuaternion().AngularDistance(input.RotationSequence().at(4).RotationQuaternion())).c_str());

			auto first = input.RotationSequence().at(0);
			auto last = input.RotationSequence().at(input.RotationSequence().size() - 1);

			Logger::WriteMessage("\nOriginal Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			input.ExecuteRDPReduction_Rotation();

			Logger::WriteMessage("New Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" \n");

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.RotationSequence().at(i).Frame(), input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			Assert::IsTrue(input.RotationSequence().size() == 5 && first.IsIdentical(input.RotationSequence().at(0)) && last.IsIdentical(input.RotationSequence().at(input.RotationSequence().size() - 1)));
		}

		TEST_METHOD(RDP_7)
		{
			CALUMI::UNIV::AnimationBlock input;

			//source sq input
			ROTATIONENTRY(input, 0, 1.0f, 0.0f, 0.0f, 0.0);
			ROTATIONENTRY(input, 1, 1.0f, 0.0f, 0.0f, 45.0); //culled
			ROTATIONENTRY(input, 2, 1.0f, 0.0f, 0.0f, 90.0);
			ROTATIONENTRY(input, 3, 1.0f, 0.0f, 0.0f, 135.0); //culled
			ROTATIONENTRY(input, 4, 1.0f, 0.0f, 0.0f, 180.0);
			ROTATIONENTRY(input, 5, 1.0f, 0.0f, 0.0f, 225.0); //culled
			ROTATIONENTRY(input, 6, 1.0f, 0.0f, 0.0f, 270.0);
			ROTATIONENTRY(input, 7, 1.0f, 0.0f, 0.0f, 315.0); //culled
			ROTATIONENTRY(input, 8, 1.0f, 0.0f, 0.0f, 360.0);

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.RotationSequence().at(i).Frame(), input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(0).RotationQuaternion().AngularDistance(input.RotationSequence().at(2).RotationQuaternion())).c_str());
			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(2).RotationQuaternion().AngularDistance(input.RotationSequence().at(4).RotationQuaternion())).c_str());

			auto first = input.RotationSequence().at(0);
			auto last = input.RotationSequence().at(input.RotationSequence().size() - 1);

			Logger::WriteMessage("\nOriginal Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			input.ExecuteRDPReduction_Rotation();

			Logger::WriteMessage("New Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" \n");

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.RotationSequence().at(i).Frame(), input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			Assert::IsTrue(input.RotationSequence().size() == 5 && first.IsIdentical(input.RotationSequence().at(0)) && last.IsIdentical(input.RotationSequence().at(input.RotationSequence().size() - 1)));
		}

		TEST_METHOD(RDP_8)
		{
			CALUMI::UNIV::AnimationBlock input;

			//source sq input
			ROTATIONENTRY(input, 3, 1.0f, 0.0f, 0.0f, 0.0);
			ROTATIONENTRY(input, 4, 1.0f, 0.0f, 0.0f, 360.0 * 1 / 7); //culled
			ROTATIONENTRY(input, 5, 1.0f, 0.0f, 0.0f, 360.0 * 2 / 7); 
			ROTATIONENTRY(input, 6, 1.0f, 0.0f, 0.0f, 360.0 * 3 / 7); //culled
			ROTATIONENTRY(input, 7, 1.0f, 0.0f, 0.0f, 360.0 * 4 / 7); 
			ROTATIONENTRY(input, 8, 1.0f, 0.0f, 0.0f, 360.0 * 5 / 7); //culled
			ROTATIONENTRY(input, 9, 1.0f, 0.0f, 0.0f, 360.0 * 6 / 7); //culled
			ROTATIONENTRY(input,10, 1.0f, 0.0f, 0.0f, 360.0);
			ROTATIONENTRY(input,11, 1.0f, 0.0f, 0.0f, 360.0); //culled
			ROTATIONENTRY(input,12, 1.0f, 0.0f, 0.0f, 360.0); 
			ROTATIONENTRY(input,13, 1.0f, 0.0f, 0.0f, 361.0);
			ROTATIONENTRY(input,14, 1.0f, 0.0f, 0.0f, 360.666666667); //culled
			ROTATIONENTRY(input,15, 1.0f, 0.0f, 0.0f, 360.333333333); //culled
			ROTATIONENTRY(input,16, 1.0f, 0.0f, 0.0f, 360.0);

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.RotationSequence().at(i).Frame(), input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(0).RotationQuaternion().AngularDistance(input.RotationSequence().at(2).RotationQuaternion())).c_str());
			//Logger::WriteMessage(std::format("\nDiff {}\n\n",input.RotationSequence().at(2).RotationQuaternion().AngularDistance(input.RotationSequence().at(4).RotationQuaternion())).c_str());

			auto first = input.RotationSequence().at(0);
			auto last = input.RotationSequence().at(input.RotationSequence().size() - 1);

			Logger::WriteMessage("\nOriginal Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			input.ExecuteRDPReduction_Rotation();

			Logger::WriteMessage("New Size = ");
			Logger::WriteMessage(std::to_string(input.RotationSequence().size()).c_str());
			Logger::WriteMessage(" \n");

			for (size_t i = 0; i < input.RotationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.RotationSequence().at(i).Frame(), input.RotationSequence().at(i).RotationQuaternion().ToString().c_str()).c_str());
			}

			Assert::IsTrue(input.RotationSequence().size() == 7 && first.IsIdentical(input.RotationSequence().at(0)) && last.IsIdentical(input.RotationSequence().at(input.RotationSequence().size() - 1)));
		}
	};


	TEST_CLASS(CALUMI_TranslationSequence) {

	public:
		TEST_METHOD(RDP_1)
		{
			CALUMI::UNIV::AnimationBlock block;

			TRANSLATIONENTRY(block, 0, 0.0, 0.0, 0.0);
			TRANSLATIONENTRY(block, 1, 0.0, 0.0, 0.0);
			TRANSLATIONENTRY(block, 2, 0.0, 0.0, 0.0);
			TRANSLATIONENTRY(block, 3, 0.0, 0.0, 0.0);

			auto first = block.TranslationSequence().at(0);
			auto last = block.TranslationSequence().at(block.TranslationSequence().size() - 1);

			for (size_t i = 0; i < block.TranslationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("T{}: {}, {}\n", i, block.TranslationSequence().at(i).Frame(), block.TranslationSequence().at(i).TranslationVector().ToString().c_str()).c_str());
			}

			Logger::WriteMessage("\nOriginal Size = ");
			Logger::WriteMessage(std::to_string(block.TranslationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			block.ExecuteRDPReduction_Translation();

			Logger::WriteMessage("New Size = ");
			Logger::WriteMessage(std::to_string(block.TranslationSequence().size()).c_str());
			Logger::WriteMessage(" \n");

			for (size_t i = 0; i < block.TranslationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("T{}: {}, {}\n", i, block.TranslationSequence().at(i).Frame(), block.TranslationSequence().at(i).TranslationVector().ToString().c_str()).c_str());
			}

			Assert::IsTrue(block.TranslationSequence().size() == 2 && first.IsIdentical(block.TranslationSequence().at(0)) && last.IsIdentical(block.TranslationSequence().at(block.TranslationSequence().size()-1)));
		}

		TEST_METHOD(RDP_2)
		{
			CALUMI::UNIV::AnimationBlock block;

			TRANSLATIONENTRY(block, 0, -2.0, -0.2, -1.0);
			TRANSLATIONENTRY(block, 3, 1.0, 0.1, 0.5);
			TRANSLATIONENTRY(block, 4, 2.0, 0.2, 1.0);
			TRANSLATIONENTRY(block, 5, 3.0, 0.3, 1.5);

			auto first = block.TranslationSequence().at(0);
			auto last = block.TranslationSequence().at(block.TranslationSequence().size() - 1);

			for (size_t i = 0; i < block.TranslationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("T{}: {}, {}\n", i, block.TranslationSequence().at(i).Frame(), block.TranslationSequence().at(i).TranslationVector().ToString().c_str()).c_str());
			}

			Logger::WriteMessage("\nOriginal Size = ");
			Logger::WriteMessage(std::to_string(block.TranslationSequence().size()).c_str());
			Logger::WriteMessage(" ");

			block.ExecuteRDPReduction_Translation();

			Logger::WriteMessage("New Size = ");
			Logger::WriteMessage(std::to_string(block.TranslationSequence().size()).c_str());
			Logger::WriteMessage(" \n");

			for (size_t i = 0; i < block.TranslationSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("T{}: {}, {}\n", i, block.TranslationSequence().at(i).Frame(), block.TranslationSequence().at(i).TranslationVector().ToString().c_str()).c_str());
			}

			Assert::IsTrue(block.TranslationSequence().size() == 2 && first.IsIdentical(block.TranslationSequence().at(0)) && last.IsIdentical(block.TranslationSequence().at(block.TranslationSequence().size() - 1)));
		}
	};

	TEST_CLASS(CALUMI_ScalarSequence) {
	public:
		TEST_METHOD(RDP_1) 
		{
			CALUMI::UNIV::AnimationBlock input;

			//source sq input
			SCALARENTRY(input, 0, 1.0f);
			SCALARENTRY(input, 1, 0.5f);
			SCALARENTRY(input, 2, 0.75f);
			SCALARENTRY(input, 3, 1.0f);
			SCALARENTRY(input, 6, 1.0f);
			SCALARENTRY(input, 8, 1.0f);
			SCALARENTRY(input, 9, 1.0f);
			SCALARENTRY(input, 10, 1.0f);

			for (size_t i = 0; i < input.ScalarSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.ScalarSequence().at(i).Frame(), input.ScalarSequence().at(i).ScalarValue()).c_str());
			}

			auto first = input.ScalarSequence().at(0);
			auto last = input.ScalarSequence().at(input.ScalarSequence().size() - 1);

			Logger::WriteMessage("\nOriginal Size = ");
			Logger::WriteMessage(std::to_string(input.ScalarSequence().size()).c_str());
			Logger::WriteMessage(" ");

			input.ExecuteRDPReduction_Scalar();

			Logger::WriteMessage("New Size = ");
			Logger::WriteMessage(std::to_string(input.ScalarSequence().size()).c_str());
			Logger::WriteMessage(" \n");

			for (size_t i = 0; i < input.ScalarSequence().size(); i++)
			{
				Logger::WriteMessage(std::format("Q{}: {}, {}\n", i, input.ScalarSequence().at(i).Frame(), input.ScalarSequence().at(i).ScalarValue()).c_str());
			}

			Assert::IsTrue(input.ScalarSequence().size() == 4 && first.IsIdentical(input.ScalarSequence().at(0)) && last.IsIdentical(input.ScalarSequence().at(input.ScalarSequence().size() - 1)));
		}

	};

}
