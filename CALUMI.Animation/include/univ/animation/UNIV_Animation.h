//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "UNIV_AnimationEntries.h"
#include "UNIV_IAnimationPackage.h"
#include "utilities/CALUMI_Utilities.h"


namespace CALUMI::UNIV
{

	struct CALUMIANIMATION_API AnimationBlock
	{
		//TODO: UNIV Documentation
		AnimationBlock();
		~AnimationBlock();

		AnimationBlock(const AnimationBlock& input);
		AnimationBlock& operator=(const AnimationBlock& other);

		//This will get the final frame entry, not the total number of frames in the sequence
		unsigned int GetLastFrameInBlock() const;

		const char* boneName() const;
		void setBoneName(const char* name) const;

		RotationSequence& rotationSequence() const;
		bool addRotationEntry(const Rotation& input, bool overwrite = true) const;
		bool removeRotationEntry(unsigned int frame) const;
		void clearRotationEntries() const;
		uint64_t rotationEntryCount() const;

		void executeRDPReduction_Rotation(float tolerance = 0.0000863f) const;

		TranslationSequence& translationSequence() const;
		bool addTranslationEntry(const Translation& input, bool overwrite = true) const;
		bool removeTranslationEntry(unsigned int frame) const;
		void clearTranslationEntries() const;
		uint64_t translationEntryCount() const;

		void executeRDPReduction_Translation(float tolerance = 1.0f / 4000.0f) const;

		ScalarSequence& scalarSequence() const;
		bool addScalarEntry(const Scalar& input, bool overwrite = true) const;
		bool removeScalarEntry(unsigned int frame) const;
		void clearScalarEntries() const;
		uint64_t scalarEntryCount() const;

		void executeRDPReduction_Scalar(float tolerance = 1.0f / 5000.0f) const;

		PrioritySequence& prioritySequence() const;
		bool addPriorityEntry(const Priority& input, bool overwrite = true) const;
		bool removePriorityEntry(unsigned int frame) const;
		void clearPriorityEntries() const;
		uint64_t priorityEntryCount() const;

		Utilities::StringContainer toJSON(uint64_t indents = 0) const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	VECTORDEC(AnimationBlockVector, AnimationBlock)

	class CALUMIANIMATION_API Animation
	{
	public:
		Animation(const Utilities::StringContainer& title, unsigned int initialBlockCount);
		Animation();
		~Animation();

		Animation(const Animation& input);
		Animation& operator=(const Animation& input);

		const char* animationTitle() const;
		void setAnimationTitle(const char* title) const;
		void setAnimationTitle(const Utilities::StringContainer& title) const;

		AnimationBlockVector& animationBlocks() const;
		bool addAnimationBlock(const AnimationBlock& blockToAdd, bool overwrite = true) const;
		void clearAnimationBlocks() const;
		uint64_t animationBlockCount() const;
		int findAnimationBlock(const char* boneName) const;

		AnimationPackageManager& getPackageManager() const;

		//This will get the final frame entry of all sequences, not the total number of frames in each sequence
		unsigned int frameCount() const;

		Utilities::StringContainer toJSON(uint64_t indents = 0) const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	VECTORDEC(AnimationVector, Animation)

	//Ctype accessible, due to namespace being ignored in demangling, it is important to remember that only universal animation structs are exposed
	//For programs like Blender, this should be sufficient as a user can create and push data into the universal format and call for a write to file function with the preferred file type
	extern  "C" {
	CALUMIANIMATION_API Animation* CreateAnimationC(const char* animationTitle, unsigned int rigBoneCount);
	CALUMIANIMATION_API AnimationBlock* GetAnimationBlockC(const Animation* source, const char* boneName , Utilities::StringContainer* errorMessage);
	CALUMIANIMATION_API uint64_t GetAnimationBlockCountC(const Animation* source);
	CALUMIANIMATION_API const char* GetAnimationTitleC(const Animation* source);
	CALUMIANIMATION_API uint64_t GetFrameCountC(const Animation* source);
	CALUMIANIMATION_API bool DeleteAnimationC(const Animation* ptr);
	CALUMIANIMATION_API bool AddAnimBlockToAnimationC(const Animation* anim, const AnimationBlock* blockToAdd, bool overwrite, Utilities::StringContainer* errorMessage);

	CALUMIANIMATION_API AnimationBlock* CreateAnimBlockC(const char* boneName, Utilities::StringContainer* errorMessage);
	CALUMIANIMATION_API bool DeleteAnimationBlockC(const AnimationBlock* ptr);
	CALUMIANIMATION_API const char* GetAnimBlockBoneNameC(const AnimationBlock* source);
	CALUMIANIMATION_API int GetAnimBlockBoneIndexC(AnimationBlock* source);
	CALUMIANIMATION_API unsigned int GetLastFrameInAnimBlockC(const AnimationBlock* source);

	CALUMIANIMATION_API bool AddRotationSqToAnimBlockC(const AnimationBlock* block, const Rotation* rotSq, unsigned int size, bool overwrite);
	CALUMIANIMATION_API Rotation* GetRotationSqArrayC(const AnimationBlock* source);
	CALUMIANIMATION_API Rotation* GetRotationFromSqC(const AnimationBlock* source, int index, Utilities::StringContainer* errorMessage);
	CALUMIANIMATION_API uint64_t GetRotationSqSizeC(const AnimationBlock* source);

	//A good default tolerance may be 0.0000863f
	CALUMIANIMATION_API void ExecuteRDPReduction_RotationC(const AnimationBlock* source, float tolerance);

	CALUMIANIMATION_API bool AddTranslationSqToAnimBlockC(const AnimationBlock* block, const Translation* trnSq, unsigned int size, bool overwrite);
	CALUMIANIMATION_API Translation* GetTranslationSqArrayC(const AnimationBlock* source);
	CALUMIANIMATION_API Translation* GetTranslationFromSqC(const AnimationBlock* source, int index, Utilities::StringContainer* errorMessage);
	CALUMIANIMATION_API uint64_t GetTranslationSqSizeC(const AnimationBlock* source);

	//A good default tolerance may be 1.0f/4000.0f
	CALUMIANIMATION_API void ExecuteRDPReduction_TranslationC(const AnimationBlock* source, float tolerance);

	CALUMIANIMATION_API bool AddScalarSqToAnimBlockC(const AnimationBlock* block, const Scalar* sclrSq, unsigned int size, bool overwrite);
	CALUMIANIMATION_API Scalar* GetScalarSqArrayC(const AnimationBlock* source);
	CALUMIANIMATION_API Scalar* GetScalarFromSqC(const AnimationBlock* source, int index, Utilities::StringContainer* errorMessage);
	CALUMIANIMATION_API uint64_t GetScalarSqSizeC(const AnimationBlock* source);

	//A good default tolerance may be 1.0f/5000.0f
	CALUMIANIMATION_API void ExecuteRDPReduction_ScalarC(const AnimationBlock* source, float tolerance);

	CALUMIANIMATION_API bool AddPrioritySqToAnimBlockC(const AnimationBlock* block, const Priority* prtySq, unsigned int size, bool overwrite);
	CALUMIANIMATION_API Priority* GetPrioritySqArrayC(const AnimationBlock* source);
	CALUMIANIMATION_API Priority* GetPriorityFromSqC(const AnimationBlock* source, int index, Utilities::StringContainer* errorMessage);
	CALUMIANIMATION_API uint64_t GetPrioritySqSizeC(const AnimationBlock* source);
	}
}
