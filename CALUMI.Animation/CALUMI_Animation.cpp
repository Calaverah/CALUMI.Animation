//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_Animation.h"
#include <print>

namespace CALUMI {
	namespace UNIV {


	Animation::Animation::Animation(const std::string& title, const int initialBoneCount, int initialBlockCount = 8)
	{
		animationTitle = title;
		boneCount = initialBoneCount;
		animationBlocks.reserve(initialBlockCount);
	}

		void Animation::Animation::AddAnimationBlock(AnimationBlock& blockToAdd)
	{
		animationBlocks.push_back(blockToAdd);
	}

		unsigned int Animation::GetFrameCount()
		{
			unsigned int output = 0;
			for (AnimationBlock block : animationBlocks)
			{
				unsigned int temp = block.GetLastFrameInBlock();
				if (temp > output)
				{
					output = temp;
				}
			}
			output++; //adding one to sum up total frames in animation. If final frame is 50, the count is 51
			return output;
		}

		unsigned int AnimationBlock::GetLastFrameInBlock()
		{
			unsigned int output = 0;
			if (_rotationSequence.size() > 0) {
				if (_rotationSequence.at(_rotationSequence.size()-1).frame > output)
				{
					output = _rotationSequence.at(_rotationSequence.size()-1).frame;
				}
			}
			if (_translationSequence.size() > 0) {
				if (_translationSequence.at(_translationSequence.size()-1).frame > output)
				{
					output = _translationSequence.at(_translationSequence.size() - 1).frame;
				}
			}
			if (_scalarSequence.size() > 0) {
				if (_scalarSequence.at(_scalarSequence.size()-1).frame > output)
				{
					output = _scalarSequence.at(_scalarSequence.size() - 1).frame;
				}
			}
			if (_prioritySequence.size() > 0) {
				if (_prioritySequence.at(_prioritySequence.size()-1).frame > output)
				{
					output = _prioritySequence.at(_prioritySequence.size() - 1).frame;
				}
			}

			return output;
		}
}
	
}