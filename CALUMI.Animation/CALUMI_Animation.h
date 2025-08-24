//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include <string>
#include <vector>
#include "CALUMI_AnimationEntries.h"

namespace CALUMI {namespace UNIV {

	extern  "C" {

		struct CALUMIANIMATION_API AnimationBlock
		{
			int boneIndex = -2;
			std::string boneName = "UNNAMED";
			std::vector<CALUMI::UNIV::Rotation> _rotationSequence;
			std::vector<CALUMI::UNIV::Translation> _translationSequence;
			std::vector<CALUMI::UNIV::Scalar> _scalarSequence;
			std::vector<CALUMI::UNIV::Priority> _prioritySequence;

			//This will get the final frame entry, not the total number of frames in the sequence
			unsigned int GetLastFrameInBlock();
		};

		class CALUMIANIMATION_API Animation
		{
		public:
			std::string animationTitle = "NO TITLE";
			int boneCount = 0;

			std::vector<AnimationBlock> animationBlocks;

			Animation(const std::string& title, const int initialBoneCount, int initialBlockCount);
			void AddAnimationBlock(AnimationBlock& blockToAdd);
			unsigned int GetFrameCount();

			Animation() = default;


		};

	}
}}
