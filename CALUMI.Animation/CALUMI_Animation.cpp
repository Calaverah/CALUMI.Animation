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


		Animation* CreateAnimationC(const char* animationTitle, int rigBoneCount)
		{
			Animation* outputAnimation = new Animation(animationTitle, rigBoneCount);
			return outputAnimation;
		}
		AnimationBlock* GetAnimationBlockC(Animation* source, int index, const char* errorMessage)
		{
			if (source->animationBlocks.size() <= index || index < 0)
			{
				errorMessage = "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->animationBlocks.at(index);
		}
		size_t GetAnimationBlockCountC(Animation* source)
		{
			return source->animationBlocks.size();
		}
		const char* GetAnimationTitleC(Animation* source)
		{
			return source->animationTitle.c_str();
		}
		size_t GetAnimationBoneCountC(Animation* source)
		{
			return source->boneCount;
		}
		size_t GetFrameCountC(Animation* source)
		{
			return source->GetFrameCount();
		}
		bool DeleteAnimationC(Animation* ptr)
		{
			if (ptr)
			{
				delete ptr;
				return true;
			}
			return false;
		}
		bool AddAnimBlockToAnimationC(Animation* anim, AnimationBlock* blockToAdd, const char* errorMessage)
		{
			anim->AddAnimationBlock(*blockToAdd);
			delete blockToAdd;
			errorMessage = "[CALUMI.Animation API] Animation Block Copied Into Animation Successfully. Original Block Deleted!";
			return false;
		}
		AnimationBlock* CreateAnimBlockC(const char* boneName, int boneIndex, const char* errorMessage)
		{
			if (boneName == "")
			{
				errorMessage = "[CALUMI.Animation API] Animation Block Must Have Bone Name!";
				return nullptr;
			}
			if (boneIndex < 0)
			{
				errorMessage = "[CALUMI.Animation API] Animation Block Must Have Valid Bone Index! (ix > -1)";
				return nullptr;
			}
			AnimationBlock* outputAnimationBlock = new AnimationBlock;
			outputAnimationBlock->boneIndex = boneIndex;
			outputAnimationBlock->boneName = boneName;
			errorMessage = "[CALUMI.Animation API] Animation Block Created!";
			return outputAnimationBlock;
		}
		bool DeleteAnimationBlockC(AnimationBlock* ptr)
		{
			if (ptr)
			{
				delete ptr;
				return true;
			}
			return false;
		}
		bool AddRotationSqToAnimBlockC(AnimationBlock* block, Rotation* rotSq, int size)
		{
			for (int i = 0; i < size; i++)
			{
				block->_rotationSequence.push_back(rotSq[i]);
			}
			return true;
		}
		Rotation* GetRotationSqArrayC(AnimationBlock* source)
		{
			if(source->_rotationSequence.empty())
			return nullptr;

			return source->_rotationSequence.data();
		}
		Rotation* GetRotationFromSqC(AnimationBlock* source, int index, const char* errorMessage)
		{
			if (index >= source->_rotationSequence.size() || index < 0)
			{
				errorMessage = "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->_rotationSequence.at(index);
		}
		size_t GetRotationSqSizeC(AnimationBlock* source)
		{
			return source->_rotationSequence.size();
		}
		bool AddTranslationSqToAnimBlockC(AnimationBlock* block, Translation* trnSq, int size)
		{
			for (int i = 0; i < size; i++)
			{
				block->_translationSequence.push_back(trnSq[i]);
			}
			return true;
		}
		Translation* GetTranslationSqArrayC(AnimationBlock* source)
		{
			if (source->_translationSequence.empty())
				return nullptr;

			return source->_translationSequence.data();
		}
		Translation* GetTranslationFromSqC(AnimationBlock* source, int index, const char* errorMessage)
		{
			if (index >= source->_translationSequence.size() || index < 0)
			{
				errorMessage = "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->_translationSequence.at(index);
		}
		size_t GetTranslationSqSizeC(AnimationBlock* source)
		{
			return source->_translationSequence.size();
		}
		bool AddScalarSqToAnimBlockC(AnimationBlock* block, Scalar* sclrSq, int size)
		{
			for (int i = 0; i < size; i++)
			{
				block->_scalarSequence.push_back(sclrSq[i]);
			}
			return true;
		}
		Scalar* GetScalarSqArrayC(AnimationBlock* source)
		{
			if(source->_scalarSequence.empty())
			return nullptr;

			return source->_scalarSequence.data();
		}
		Scalar* GetScalarFromSqC(AnimationBlock* source, int index, const char* errorMessage)
		{
			if (index >= source->_scalarSequence.size() || index < 0)
			{
				errorMessage = "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->_scalarSequence.at(index);
		}
		size_t GetScalarSqSizeC(AnimationBlock* source)
		{
			return source->_scalarSequence.size();
		}
		bool AddPrioritySqToAnimBlockC(AnimationBlock* block, Priority* prtySq, int size)
		{
			for (int i = 0; i < size; i++)
			{
				block->_prioritySequence.push_back(prtySq[i]);
			}
			return true;
		}
		Priority* GetPrioritySqArrayC(AnimationBlock* source)
		{
			if(source->_prioritySequence.empty())
			return nullptr;

			return source->_prioritySequence.data();
		}
		Priority* GetPriorityFromSqC(AnimationBlock* source, int index, const char* errorMessage)
		{
			if (index >= source->_prioritySequence.size() || index < 0)
			{
				errorMessage = "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->_prioritySequence.at(index);
		}
		size_t GetPrioritySqSizeC(AnimationBlock* source)
		{
			return source->_prioritySequence.size();
		}

	}
	
}