//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_AnimationEntries.h"
#include "CALUMI_Common.h"
#include "UNIV_IAnimationPackage.h"

namespace CALUMI {namespace UNIV {

	struct CALUMIANIMATION_API AnimationBlock
	{
		AnimationBlock();
		~AnimationBlock();
		
		AnimationBlock(const AnimationBlock& input);
		AnimationBlock& operator=(const AnimationBlock& input);

		//This will get the final frame entry, not the total number of frames in the sequence
		unsigned int GetLastFrameInBlock();

		int BoneIndex() const;
		void BoneIndex(int idx);

		const char* BoneName() const;
		void BoneName(const char* name);

		Utilities::VectorContainer<CALUMI::UNIV::Rotation>& RotationSequence() const;
		bool AddRotationEntry(CALUMI::UNIV::Rotation& input, bool overwrite = true);
		bool RemoveRotationEntry(unsigned int frame);
		void ClearRotationEntries();
		size_t GetRotationEntryCount() const;

		void ExecuteRDPReduction_Rotation(float tolerance = 0.0000863f);

		Utilities::VectorContainer<CALUMI::UNIV::Translation>& TranslationSequence() const;
		bool AddTranslationEntry(CALUMI::UNIV::Translation& input, bool overwrite = true);
		bool RemoveTranslationEntry(unsigned int frame);
		void ClearTranslationEntries();
		size_t GetTranslationEntryCount() const;
		
		void ExecuteRDPReduction_Translation(float tolerance = 1.0f / 4000.0f);

		Utilities::VectorContainer<CALUMI::UNIV::Scalar>& ScalarSequence() const;
		bool AddScalarEntry(CALUMI::UNIV::Scalar& input, bool overwrite = true);
		bool RemoveScalarEntry(unsigned int frame);
		void ClearScalarEntries();
		size_t GetScalarEntryCount() const;

		void ExecuteRDPReduction_Scalar(float tolerance = 1.0f / 5000.0f);

		Utilities::VectorContainer<CALUMI::UNIV::Priority>& PrioritySequence();
		bool AddPriorityEntry(CALUMI::UNIV::Priority& input, bool overwrite = true);
		bool RemovePriorityEntry(unsigned int frame);
		void ClearPriorityEntries();
		size_t GetPriorityEntryCount() const;

		Utilities::StringContainer ToJSON(const size_t indents = 0) const;

		bool operator<(const AnimationBlock& other) const;
		bool operator>(const AnimationBlock& other) const;
	private:
		struct Impl;
		Impl* pImpl;
	};

	class CALUMIANIMATION_API Animation
	{
	public:


		Animation(const Utilities::StringContainer& title, unsigned int initialBlockCount);
		Animation();
		~Animation();

		Animation(const Animation& input);
		Animation& operator=(const Animation& input);
		
		const char* AnimationTitle() const;
		void AnimationTitle(const char* title);
		void AnimationTitle(const Utilities::StringContainer& title);

		Utilities::VectorContainer<AnimationBlock>& AnimationBlocks() const;
		bool AddAnimationBlock(AnimationBlock& blockToAdd, bool overwrite = true);
		void ClearAnimationBlocks();
		size_t GetAnimationBlockCount() const;

		AnimationPackageManager& getPackageManager() const;

		//This will get the final frame entry of all sequences, not the total number of frames in each sequence
		unsigned int GetFrameCount();

		Utilities::StringContainer ToJSON(const size_t indents = 0) const;
	private:
		struct Impl;
		Impl* pImpl;
	};

	//Ctype accessible, due to namespace being ignored in demangling, it is important to remember that only universal animation structs are exposed
	//For programs like Blender, this should be sufficient as a user can create and push data into the universal format and call for a write to file function with the preferred file type
	extern  "C" {
		CALUMIANIMATION_API Animation* CreateAnimationC(const char* animationTitle, unsigned int rigBoneCount);
		CALUMIANIMATION_API AnimationBlock* GetAnimationBlockC(Animation* source, int index, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API size_t GetAnimationBlockCountC(Animation* source);
		CALUMIANIMATION_API const char* GetAnimationTitleC(Animation* source);
		CALUMIANIMATION_API size_t GetFrameCountC(Animation* source);
		CALUMIANIMATION_API bool DeleteAnimationC(Animation* ptr);
		CALUMIANIMATION_API bool AddAnimBlockToAnimationC(Animation* anim, AnimationBlock* blockToAdd, bool overwrite, Utilities::StringContainer* errorMessage);

		CALUMIANIMATION_API AnimationBlock* CreateAnimBlockC(const char* boneName, int boneIndex, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API bool DeleteAnimationBlockC(AnimationBlock* ptr);
		CALUMIANIMATION_API const char* GetAnimBlockBoneNameC(AnimationBlock* source);
		CALUMIANIMATION_API int GetAnimBlockBoneIndexC(AnimationBlock* source);
		CALUMIANIMATION_API unsigned int GetLastFrameInAnimBlockC(AnimationBlock* source);

		CALUMIANIMATION_API bool AddRotationSqToAnimBlockC(AnimationBlock* block, Rotation* rotSq, unsigned int size, bool overwrite);
		CALUMIANIMATION_API Rotation* GetRotationSqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Rotation* GetRotationFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API size_t GetRotationSqSizeC(AnimationBlock* source);

		//A good default tolerance may be 0.0000863f
		CALUMIANIMATION_API void ExecuteRDPReduction_RotationC(AnimationBlock* source, float tolerance);
		
		CALUMIANIMATION_API bool AddTranslationSqToAnimBlockC(AnimationBlock* block, Translation* trnSq, unsigned int size, bool overwrite);
		CALUMIANIMATION_API Translation* GetTranslationSqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Translation* GetTranslationFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API size_t GetTranslationSqSizeC(AnimationBlock* source);
		
		//A good default tolerance may be 1.0f/4000.0f
		CALUMIANIMATION_API void ExecuteRDPReduction_TranslationC(AnimationBlock* source, float tolerance);
		
		CALUMIANIMATION_API bool AddScalarSqToAnimBlockC(AnimationBlock* block, Scalar* sclrSq, unsigned int size, bool overwrite);
		CALUMIANIMATION_API Scalar* GetScalarSqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Scalar* GetScalarFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API size_t GetScalarSqSizeC(AnimationBlock* source);

		//A good default tolerance may be 1.0f/5000.0f
		CALUMIANIMATION_API void ExecuteRDPReduction_ScalarC(AnimationBlock* source, float tolerance);
		
		CALUMIANIMATION_API bool AddPrioritySqToAnimBlockC(AnimationBlock* block, Priority* prtySq, unsigned int size, bool overwrite);
		CALUMIANIMATION_API Priority* GetPrioritySqArrayC(AnimationBlock* source);
		CALUMIANIMATION_API Priority* GetPriorityFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API size_t GetPrioritySqSizeC(AnimationBlock* source);
	}
}}

#pragma warning(disable: 4661)
_VECTORTEMPLATE(CALUMI::UNIV::Animation);
_VECTORTEMPLATE(CALUMI::UNIV::AnimationBlock);
#pragma warning(default: 4661)