//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_AnimationEntries.h"
#include "CALUMI_Common.h"
#include <string>
#include <vector>

namespace CALUMI {namespace UNIV {
	
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

		std::string ToJSON() const;
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

		std::string ToJSON() const;
	};

	//Ctype accessable, due to namespace being ignored in demangling, it is important to remember that only universal animation structs are exposed
	//For programs like Blender, this should be sufficient as a user can create and push data into the universal format and call for a write to file function with the preffered file type
	extern  "C" {
		CALUMIANIMATION_API Animation* CreateAnimationC(const char* animationTitle, int rigBoneCount);
		CALUMIANIMATION_API AnimationBlock* GetAnimationBlockC(Animation* source, int index, const char* errorMessage);
		CALUMIANIMATION_API size_t GetAnimationBlockCountC(Animation* source);
		CALUMIANIMATION_API const char* GetAnimationTitleC(Animation* source);
		CALUMIANIMATION_API size_t GetAnimationBoneCountC(Animation* source);
		CALUMIANIMATION_API size_t GetFrameCountC(Animation* source);
		CALUMIANIMATION_API bool DeleteAnimationC(Animation* ptr);
		CALUMIANIMATION_API bool AddAnimBlockToAnimationC(Animation* anim, AnimationBlock* blockToAdd, const char* errorMessage);

		CALUMIANIMATION_API AnimationBlock* CreateAnimBlockC(const char* boneName, int boneIndex, const char* errorMessage);
		CALUMIANIMATION_API bool DeleteAnimationBlockC(AnimationBlock* ptr);
		CALUMIANIMATION_API const char* GetAnimBlockBoneNameC(AnimationBlock* source);
		CALUMIANIMATION_API int GetAnimBlockBoneIndexC(AnimationBlock* source);
		CALUMIANIMATION_API unsigned int GetLastFrameInAnimBlockC(AnimationBlock* source);
		CALUMIANIMATION_API bool AddRotationSqToAnimBlockC(AnimationBlock* block, Rotation* rotSq, int size);
		CALUMIANIMATION_API Rotation* GetRotationSqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Rotation* GetRotationFromSqC(AnimationBlock* source, int index, const char* errorMessage);
		CALUMIANIMATION_API size_t GetRotationSqSizeC(AnimationBlock* source);
		CALUMIANIMATION_API bool AddTranslationSqToAnimBlockC(AnimationBlock* block, Translation* trnSq, int size);
		CALUMIANIMATION_API Translation* GetTranslationSqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Translation* GetTranslationFromSqC(AnimationBlock* source, int index, const char* errorMessage);
		CALUMIANIMATION_API size_t GetTranslationSqSizeC(AnimationBlock* source);
		CALUMIANIMATION_API bool AddScalarSqToAnimBlockC(AnimationBlock* block, Scalar* sclrSq, int size);
		CALUMIANIMATION_API Scalar* GetScalarSqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Scalar* GetScalarFromSqC(AnimationBlock* source, int index, const char* errorMessage);
		CALUMIANIMATION_API size_t GetScalarSqSizeC(AnimationBlock* source);
		CALUMIANIMATION_API bool AddPrioritySqToAnimBlockC(AnimationBlock* block, Priority* prtySq, int size);
		CALUMIANIMATION_API Priority* GetPrioritySqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Priority* GetPriorityFromSqC(AnimationBlock* source, int index, const char* errorMessage);
		CALUMIANIMATION_API size_t GetPrioritySqSizeC(AnimationBlock* source);
	}
}}
