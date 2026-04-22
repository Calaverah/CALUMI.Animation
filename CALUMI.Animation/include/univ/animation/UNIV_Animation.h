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

		[[deprecated]] [[nodiscard]] Utilities::StringContainer toJSON(uint64_t indents = 0) const;

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
}

/**
	 *
	 *
	 * @addtogroup c_anim
	 * @{
	 * @defgroup c_univ_anim Universal
	 * @{
	 */
	extern  "C" {
	/**
	 *
	 * @param animationTitle
	 * @param rigBoneCount
	 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteAnimationC
	 * @return
	 */
	CALUMIANIMATION_API CALUMI::UNIV::Animation* CreateAnimationC(const char* animationTitle,
	                                                              unsigned int rigBoneCount);
	/**
	 *
	 * @param source
	 * @param boneName
	 * @param errorMessage
	 * @return
	 */
	CALUMIANIMATION_API CALUMI::UNIV::AnimationBlock* GetAnimationBlockC(
																	const CALUMI::UNIV::Animation* source,
																	const char* boneName,
																	CALUMI::Utilities::StringContainer* errorMessage);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API uint64_t GetAnimationBlockCountC(const CALUMI::UNIV::Animation* source);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API const char* GetAnimationTitleC(const CALUMI::UNIV::Animation* source);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API uint64_t GetFrameCountC(const CALUMI::UNIV::Animation* source);
	/**
	 *
	 * @param ptr
	 * @return
	 */
	CALUMIANIMATION_API bool DeleteAnimationC(const CALUMI::UNIV::Animation* ptr);
	/**
	 *
	 * @param anim
	 * @param blockToAdd
	 * @param overwrite
	 * @param errorMessage
	 * @return
	 */
	CALUMIANIMATION_API bool AddAnimBlockToAnimationC(const CALUMI::UNIV::Animation* anim,
	                                                  const CALUMI::UNIV::AnimationBlock* blockToAdd,
	                                                  bool overwrite,
	                                                  CALUMI::Utilities::StringContainer* errorMessage);
	/**
	 *
	 * @param boneName
	 * @param errorMessage
	 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteAnimationBlockC
	 * @return
	 */
	CALUMIANIMATION_API CALUMI::UNIV::AnimationBlock* CreateAnimBlockC(
																	const char* boneName,
																	CALUMI::Utilities::StringContainer* errorMessage);
	/**
	 *
	 * @param ptr
	 * @return
	 */
	CALUMIANIMATION_API bool DeleteAnimationBlockC(const CALUMI::UNIV::AnimationBlock* ptr);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API const char* GetAnimBlockBoneNameC(const CALUMI::UNIV::AnimationBlock* source);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API int GetAnimBlockBoneIndexC(CALUMI::UNIV::AnimationBlock* source);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API unsigned int GetLastFrameInAnimBlockC(const CALUMI::UNIV::AnimationBlock* source);
	/**
	 *
	 * @param block
	 * @param rotSq
	 * @param size
	 * @param overwrite
	 * @return
	 */
	CALUMIANIMATION_API bool AddRotationSqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block,
	                                                   const CALUMI::UNIV::Rotation* rotSq,
	                                                   unsigned int size,
	                                                   bool overwrite);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API CALUMI::UNIV::Rotation* GetRotationSqArrayC(const CALUMI::UNIV::AnimationBlock* source);
	/**
	 *
	 * @param source
	 * @param index
	 * @param errorMessage
	 * @return
	 */
	CALUMIANIMATION_API CALUMI::UNIV::Rotation* GetRotationFromSqC(const CALUMI::UNIV::AnimationBlock* source,
	                                                               int index,
	                                                               CALUMI::Utilities::StringContainer* errorMessage);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API uint64_t GetRotationSqSizeC(const CALUMI::UNIV::AnimationBlock* source);
	/**
	 * @note A good default tolerance may be 0.0000863f
	 * @param source
	 * @param tolerance
	 */
	CALUMIANIMATION_API void ExecuteRDPReduction_RotationC(const CALUMI::UNIV::AnimationBlock* source, float tolerance);
	/**
	 *
	 * @param block
	 * @param trnSq
	 * @param size
	 * @param overwrite
	 * @return
	 */
	CALUMIANIMATION_API bool AddTranslationSqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block,
	                                                      const CALUMI::UNIV::Translation* trnSq,
	                                                      unsigned int size,
	                                                      bool overwrite);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API CALUMI::UNIV::Translation* GetTranslationSqArrayC(const CALUMI::UNIV::AnimationBlock* source);
	/**
	 *
	 * @param source
	 * @param index
	 * @param errorMessage
	 * @return
	 */
	CALUMIANIMATION_API CALUMI::UNIV::Translation* GetTranslationFromSqC(
																	const CALUMI::UNIV::AnimationBlock* source,
																	int index,
																	CALUMI::Utilities::StringContainer* errorMessage);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API uint64_t GetTranslationSqSizeC(const CALUMI::UNIV::AnimationBlock* source);
	/**
	 * @note A good default tolerance may be 1.0/4000.0
	 * @param source
	 * @param tolerance
	 */
	CALUMIANIMATION_API void ExecuteRDPReduction_TranslationC(const CALUMI::UNIV::AnimationBlock* source,
	                                                          float tolerance);
	/**
	 *
	 * @param block
	 * @param sclrSq
	 * @param size
	 * @param overwrite
	 * @return
	 */
	CALUMIANIMATION_API bool AddScalarSqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block,
	                                                 const CALUMI::UNIV::Scalar* sclrSq,
	                                                 unsigned int size,
	                                                 bool overwrite);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API CALUMI::UNIV::Scalar* GetScalarSqArrayC(const CALUMI::UNIV::AnimationBlock* source);
	/**
	 *
	 * @param source
	 * @param index
	 * @param errorMessage
	 * @return
	 */
	CALUMIANIMATION_API CALUMI::UNIV::Scalar* GetScalarFromSqC(const CALUMI::UNIV::AnimationBlock* source,
	                                                           int index,
	                                                           CALUMI::Utilities::StringContainer* errorMessage);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API uint64_t GetScalarSqSizeC(const CALUMI::UNIV::AnimationBlock* source);
	/**
	 * @note A good default tolerance may be 1.0/5000.0
	 * @param source
	 * @param tolerance
	 */
	CALUMIANIMATION_API void ExecuteRDPReduction_ScalarC(const CALUMI::UNIV::AnimationBlock* source, float tolerance);
	/**
	 *
	 * @param block
	 * @param prtySq
	 * @param size
	 * @param overwrite
	 * @return
	 */
	CALUMIANIMATION_API bool AddPrioritySqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block,
	                                                   const CALUMI::UNIV::Priority* prtySq,
	                                                   unsigned int size,
	                                                   bool overwrite);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API CALUMI::UNIV::Priority* GetPrioritySqArrayC(const CALUMI::UNIV::AnimationBlock* source);
	/**
	 *
	 * @param source
	 * @param index
	 * @param errorMessage
	 * @return
	 */
	CALUMIANIMATION_API CALUMI::UNIV::Priority* GetPriorityFromSqC(const CALUMI::UNIV::AnimationBlock* source,
	                                                               int index,
	                                                               CALUMI::Utilities::StringContainer* errorMessage);
	/**
	 *
	 * @param source
	 * @return
	 */
	CALUMIANIMATION_API uint64_t GetPrioritySqSizeC(const CALUMI::UNIV::AnimationBlock* source);
	}

/// @}
/// @}
