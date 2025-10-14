//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_AnimationEntries.h"
#include "CALUMI_Common.h"


namespace CALUMI {namespace UNIV {

	struct CALUMIANIMATION_API AnimationBlock
	{
		int boneIndex = -2;
		Utilities::StringContainer boneName = "UNNAMED";
		Utilities::VectorContainer<CALUMI::UNIV::Rotation> _rotationSequence;
		Utilities::VectorContainer<CALUMI::UNIV::Translation> _translationSequence;
		Utilities::VectorContainer<CALUMI::UNIV::Scalar> _scalarSequence;
		Utilities::VectorContainer<CALUMI::UNIV::Priority> _prioritySequence;

		AnimationBlock() = default;

		//This will get the final frame entry, not the total number of frames in the sequence
		unsigned int GetLastFrameInBlock();

		bool AddRotationEntry(CALUMI::UNIV::Rotation& input, bool overwrite = true);
		bool RemoveRotationEntry(unsigned int frame);
		void ClearRotationEntries();
		size_t GetRotationEntryCount() const;

		bool AddTranslationEntry(CALUMI::UNIV::Translation& input, bool overwrite = true);
		bool RemoveTranslationEntry(unsigned int frame);
		void ClearTranslationEntries();
		size_t GetTranslationEntryCount() const;

		bool AddScalarEntry(CALUMI::UNIV::Scalar& input, bool overwrite = true);
		bool RemoveScalarEntry(unsigned int frame);
		void ClearScalarEntries();
		size_t GetScalarEntryCount() const;


		bool AddPriorityEntry(CALUMI::UNIV::Priority& input, bool overwrite = true);
		bool RemovePriorityEntry(unsigned int frame);
		void ClearPriorityEntries();
		size_t GetPriorityEntryCount() const;

		
		UNIV::AnimationBlock& operator=(const AnimationBlock& other);

		Utilities::StringContainer ToJSON(const size_t indents = 0) const;

		bool operator<(const AnimationBlock& other) const;
		bool operator>(const AnimationBlock& other) const;
	};

	class CALUMIANIMATION_API Animation
	{
	public:
		Utilities::StringContainer animationTitle = "NO TITLE";
		int boneCount = 0;

		Utilities::VectorContainer<AnimationBlock> animationBlocks;

		Animation(const Utilities::StringContainer& title, const int initialBoneCount, int initialBlockCount);
		Animation() = default;

		bool AddAnimationBlock(AnimationBlock& blockToAdd, bool overwrite = true);
		void ClearAnimationBlocks();
		size_t GetAnimationBlockCount() const;

		unsigned int GetFrameCount();

		Utilities::StringContainer ToJSON(const size_t indents = 0) const;

	};

	//Ctype accessible, due to namespace being ignored in demangling, it is important to remember that only universal animation structs are exposed
	//For programs like Blender, this should be sufficient as a user can create and push data into the universal format and call for a write to file function with the preferred file type
	extern  "C" {
		CALUMIANIMATION_API Animation* CreateAnimationC(const char* animationTitle, int rigBoneCount);
		CALUMIANIMATION_API AnimationBlock* GetAnimationBlockC(Animation* source, int index, const char* errorMessage);
		CALUMIANIMATION_API size_t GetAnimationBlockCountC(Animation* source);
		CALUMIANIMATION_API const char* GetAnimationTitleC(Animation* source);
		CALUMIANIMATION_API size_t GetAnimationBoneCountC(Animation* source);
		CALUMIANIMATION_API size_t GetFrameCountC(Animation* source);
		CALUMIANIMATION_API bool DeleteAnimationC(Animation* ptr);
		CALUMIANIMATION_API bool AddAnimBlockToAnimationC(Animation* anim, AnimationBlock* blockToAdd, bool overwrite, const char* errorMessage);

		CALUMIANIMATION_API AnimationBlock* CreateAnimBlockC(const char* boneName, int boneIndex, const char* errorMessage);
		CALUMIANIMATION_API bool DeleteAnimationBlockC(AnimationBlock* ptr);
		CALUMIANIMATION_API const char* GetAnimBlockBoneNameC(AnimationBlock* source);
		CALUMIANIMATION_API int GetAnimBlockBoneIndexC(AnimationBlock* source);
		CALUMIANIMATION_API unsigned int GetLastFrameInAnimBlockC(AnimationBlock* source);
		CALUMIANIMATION_API bool AddRotationSqToAnimBlockC(AnimationBlock* block, Rotation* rotSq, int size, bool overwrite);
		CALUMIANIMATION_API Rotation* GetRotationSqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Rotation* GetRotationFromSqC(AnimationBlock* source, int index, const char* errorMessage);
		CALUMIANIMATION_API size_t GetRotationSqSizeC(AnimationBlock* source);
		CALUMIANIMATION_API bool AddTranslationSqToAnimBlockC(AnimationBlock* block, Translation* trnSq, int size, bool overwrite);
		CALUMIANIMATION_API Translation* GetTranslationSqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Translation* GetTranslationFromSqC(AnimationBlock* source, int index, const char* errorMessage);
		CALUMIANIMATION_API size_t GetTranslationSqSizeC(AnimationBlock* source);
		CALUMIANIMATION_API bool AddScalarSqToAnimBlockC(AnimationBlock* block, Scalar* sclrSq, int size, bool overwrite);
		CALUMIANIMATION_API Scalar* GetScalarSqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Scalar* GetScalarFromSqC(AnimationBlock* source, int index, const char* errorMessage);
		CALUMIANIMATION_API size_t GetScalarSqSizeC(AnimationBlock* source);
		CALUMIANIMATION_API bool AddPrioritySqToAnimBlockC(AnimationBlock* block, Priority* prtySq, int size, bool overwrite);
		CALUMIANIMATION_API Priority* GetPrioritySqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Priority* GetPriorityFromSqC(AnimationBlock* source, int index, const char* errorMessage);
		CALUMIANIMATION_API size_t GetPrioritySqSizeC(AnimationBlock* source);
	}
}}

_VECTORTEMPLATE(CALUMI::UNIV::Animation);
_VECTORTEMPLATE(CALUMI::UNIV::AnimationBlock);