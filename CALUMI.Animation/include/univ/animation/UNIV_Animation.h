//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "UNIV_AnimationEntries.h"
#include "UNIV_IAnimationPackage.h"
#include "utilities/CALUMI_Utilities.h"


namespace CALUMI::UNIV
{
	/**
	 * A sequence of frame based entries that modify a bone, including translation, rotation, scale, and
	 * additive priority
	 */
	class CALUMIANIMATION_API AnimationBlock
	{
	public:

		/// @name Initialization
		/// @{
		AnimationBlock();
		~AnimationBlock();

		/**
		 * @param input
		 */
		AnimationBlock(const AnimationBlock& input);
		/// @}
		/// @name Operators
		/// @{
		/**
		 * @param other
		 * @return
		 */
		AnimationBlock& operator=(const AnimationBlock& other);
		/// @}

		/// @name Data
		/// @{

		/**
		 *
		 * @return final frame entry by value
		 */
		unsigned int lastFrameInBlock() const;
		/**
		 * @return
		 */
		const char* boneName() const;
		/**
		 * @param name
		 */
		void setBoneName(const char* name) const;
		/// @}
		/// @name Rotation
		/// @{

		/**
		 * @return
		 */
		RotationSequence& rotationSequence() const;
		/**
		 * @param input
		 * @param overwrite
		 * @return
		 */
		bool addRotationEntry(const RotationFrame& input, bool overwrite = true) const;
		/**
		 * @param frame
		 * @return
		 */
		bool removeRotationEntry(unsigned int frame) const;
		/**
		 * @brief
		 */
		void clearRotationEntries() const;
		/**
		 * @return
		 */
		uint64_t rotationEntryCount() const;

		/**
		 * @param tolerance
		 */
		void executeRDPReduction_Rotation(float tolerance = 0.0000863f) const;

		/// @}
		/// @name Translation
		/// @{

		/**
		 * @return
		 */
		TranslationSequence& translationSequence() const;
		/**
		 * @param input
		 * @param overwrite
		 * @return
		 */
		bool addTranslationEntry(const TranslationFrame& input, bool overwrite = true) const;
		/**
		 * @param frame
		 * @return
		 */
		bool removeTranslationEntry(unsigned int frame) const;
		/**
		 * @brief
		 */
		void clearTranslationEntries() const;
		/**
		 * @return
		 */
		uint64_t translationEntryCount() const;
		/**
		 * @param tolerance
		 */
		void executeRDPReduction_Translation(float tolerance = 1.0f / 4000.0f) const;

		/// @}
		/// @name Scale
		/// @{

		/**
		 * @return
		 */
		ScalarSequence& scalarSequence() const;
		/**
		 * @param input
		 * @param overwrite
		 * @return
		 */
		bool addScalarEntry(const ScalarFrame& input, bool overwrite = true) const;
		/**
		 * @param frame
		 * @return
		 */
		bool removeScalarEntry(unsigned int frame) const;
		/**
		 * @brief
		 */
		void clearScalarEntries() const;
		/**
		 * @return
		 */
		uint64_t scalarEntryCount() const;

		/**
		 * @param tolerance
		 */
		void executeRDPReduction_Scalar(float tolerance = 1.0f / 5000.0f) const;

		/// @}
		/// @name Additive Priority
		/// @{

		/**
		 * @return
		 */
		PrioritySequence& prioritySequence() const;
		/**
		 * @param input
		 * @param overwrite
		 * @return
		 */
		bool addPriorityEntry(const PriorityFrame& input, bool overwrite = true) const;
		/**
		 * @param frame
		 * @return
		 */
		bool removePriorityEntry(unsigned int frame) const;
		/**
		 * @brief
		 */
		void clearPriorityEntries() const;
		/**
		 * @return
		 */
		uint64_t priorityEntryCount() const;

		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};

	VECTORDEC(AnimationBlockVector, AnimationBlock)

	class CALUMIANIMATION_API Animation
	{
	public:
		/// @name Initialization
		/// @{
		Animation();
		/**
		 * @param title
		 * @param initialBlockCount
		 */
		Animation(const Utilities::StringContainer& title, unsigned int initialBlockCount);
		/**
		 * @param input
		 */
		Animation(const Animation& input);
		~Animation();
		/// @}
		/// @name Operators
		/// @{
		/**
		 * @return
		 */
		Animation& operator=(const Animation& input);
		/// @}
		/// @name Title
		/// @{
		/**
		 * @return
		 */
		const char* animationTitle() const;
		/**
		 * @param title
		 */
		void setAnimationTitle(const char* title) const;
		/**
		 * @param title
		 */
		void setAnimationTitle(const Utilities::StringContainer& title) const;
		/// @}
		/// @name Animation Blocks
		/// @{
		/**
		 * @return
		 */
		AnimationBlockVector& animationBlocks() const;
		/**
		 * @param blockToAdd
		 * @param overwrite
		 * @return
		 */
		bool addAnimationBlock(const AnimationBlock& blockToAdd, bool overwrite = true) const;
		/**
		 * @brief
		 */
		void clearAnimationBlocks() const;
		/**
		 * @return
		 */
		uint64_t animationBlockCount() const;
		/**
		 * @param boneName
		 * @return
		 */
		int findAnimationBlock(const char* boneName) const;
		/**
		 * @details Will return the final frame of all included sequences + 1
		 * @return Total frame count of all sequences. IE if the final frame of the entire block is 50 then the count
		 * will be 51
		 */
		unsigned int frameCount() const;
		/// @}
		/// @name Packages
		/// @{
		AnimationPackageManager& packageManager() const;
		/// @}

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
 * @param rigBoneCount Expected count of bones. Only sets the initial vector reservation and slightly improves
 * performance, but doesn't limit the amount of bones that can be included
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteAnimationC
 * @return
 */
CALUMIANIMATION_API CALUMI::UNIV::Animation* CreateAnimationC(const char* animationTitle,
                                                              unsigned int rigBoneCount);
/**
 *
 * @param source
 * @param boneName
 * @return Block ptr or nullptr if error has occurred
 */
CALUMIANIMATION_API CALUMI::UNIV::AnimationBlock* GetAnimationBlockC(const CALUMI::UNIV::Animation* source,
																	 const char* boneName);
/**
 *
 * @param source
 * @return Count or 0 if error has occurred
 */
CALUMIANIMATION_API uint64_t GetAnimationBlockCountC(const CALUMI::UNIV::Animation* source);
/**
 *
 * @param source
 * @return Title of the animation, or nullptr if an error has occurred
 */
CALUMIANIMATION_API const char* GetAnimationTitleC(const CALUMI::UNIV::Animation* source);
/**
 *
 * @param source
 * @return Total number of frames on this animation, if frame 50 is the final frame then this will return as 51
 */
CALUMIANIMATION_API uint64_t GetFrameCountC(const CALUMI::UNIV::Animation* source);
/**
 *
 * @param ptr Ptr Ref, ptr will be set to nullptr on completion
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Deleted Successfully
 */
CALUMIANIMATION_API int DeleteAnimationC(const CALUMI::UNIV::Animation** ptr);
/**
 *
 * @param anim
 * @param blockToAdd
 * @param overwrite
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Block Added\n 1 = Block Was Not Added
 */
CALUMIANIMATION_API int AddAnimBlockToAnimationC(const CALUMI::UNIV::Animation* anim,
                                                  const CALUMI::UNIV::AnimationBlock* blockToAdd,
                                                  bool overwrite);
/**
 *
 * @param boneName
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteAnimationBlockC
 * @return
 */
CALUMIANIMATION_API CALUMI::UNIV::AnimationBlock* CreateAnimBlockC(const char* boneName);

/**
 *
 * @param ptr
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Deleted Successfully
 */
CALUMIANIMATION_API int DeleteAnimationBlockC(const CALUMI::UNIV::AnimationBlock** ptr);
/**
 *
 * @param source
 * @return Name or nullptr if error has occurred
 */
CALUMIANIMATION_API const char* GetAnimBlockBoneNameC(const CALUMI::UNIV::AnimationBlock* source);
/**
 *
 * @param source
 * @return The final frame in the block, not the final count
 */
CALUMIANIMATION_API unsigned int GetLastFrameInAnimBlockC(const CALUMI::UNIV::AnimationBlock* source);
/**
 *
 * @param block
 * @param rotSq
 * @param size
 * @param overwrite
 * @return Number of entries added or simply 0 if an error has occurred
 */
CALUMIANIMATION_API unsigned int AddRotationSqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block,
														   const CALUMI::UNIV::RotationFrame* rotSq,
														   unsigned int size,
														   bool overwrite);
/**
 *
 * @param source
 * @return Ptr to the sequence array or nullptr if error has occurred. Use sequence size to safely iterate through
 * the array
 */
CALUMIANIMATION_API CALUMI::UNIV::RotationFrame* GetRotationSqArrayC(const CALUMI::UNIV::AnimationBlock* source);
/**
 *
 * @param source
 * @param index
 * @return Ptr to the entry or nullptr if an error has occurred
 */
CALUMIANIMATION_API CALUMI::UNIV::RotationFrame* GetRotationFromSqC(const CALUMI::UNIV::AnimationBlock* source, int index);
/**
 *
 * @param source
 * @return Sequence count or simply 0 if an error has occurred
 */
CALUMIANIMATION_API uint64_t GetRotationSqSizeC(const CALUMI::UNIV::AnimationBlock* source);
/**
 * @note A good default tolerance may be 0.0000863f
 * @param source
 * @param tolerance
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Operation Completed Successfully
 */
CALUMIANIMATION_API int ExecuteRDPReduction_RotationC(const CALUMI::UNIV::AnimationBlock* source, float tolerance);
/**
 *
 * @param block
 * @param trnSq
 * @param size
 * @param overwrite
 * @return Number of entries added or simply 0 if an error has occurred
 */
CALUMIANIMATION_API unsigned int AddTranslationSqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block,
															  const CALUMI::UNIV::TranslationFrame* trnSq,
															  unsigned int size,
															  bool overwrite);
/**
 * @param source
 * @return Ptr to the sequence array or nullptr if error has occurred. Use sequence size to safely iterate through
 * the array
 */
CALUMIANIMATION_API CALUMI::UNIV::TranslationFrame* GetTranslationSqArrayC(const CALUMI::UNIV::AnimationBlock* source);
/**
 * @param source
 * @param index
 * @return Ptr to the entry or nullptr if an error has occurred
 */
CALUMIANIMATION_API CALUMI::UNIV::TranslationFrame* GetTranslationFromSqC(const CALUMI::UNIV::AnimationBlock* source,
																	 int index);
/**
 * @param source
 * @return Sequence count or simply 0 if an error has occurred
 */
CALUMIANIMATION_API uint64_t GetTranslationSqSizeC(const CALUMI::UNIV::AnimationBlock* source);
/**
 * @note A good default tolerance may be 1.0/4000.0
 * @param source
 * @param tolerance
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Operation Completed Successfully
 */
CALUMIANIMATION_API int ExecuteRDPReduction_TranslationC(const CALUMI::UNIV::AnimationBlock* source,
                                                          float tolerance);
/**
 *
 * @param block
 * @param sclrSq
 * @param size
 * @param overwrite
 * @return Number of entries added or simply 0 if an error has occurred
 */
CALUMIANIMATION_API unsigned int AddScalarSqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block,
                                                const CALUMI::UNIV::ScalarFrame* sclrSq, unsigned int size, bool overwrite);
/**
 *
 * @param source
 * @return Ptr to the sequence array or nullptr if error has occurred. Use sequence size to safely iterate through
 * the array
 */
CALUMIANIMATION_API CALUMI::UNIV::ScalarFrame* GetScalarSqArrayC(const CALUMI::UNIV::AnimationBlock* source);
/**
 *
 * @param source
 * @param index
 * @return Ptr to the entry or nullptr if an error has occurred
 */
CALUMIANIMATION_API CALUMI::UNIV::ScalarFrame* GetScalarFromSqC(const CALUMI::UNIV::AnimationBlock* source, int index);
/**
 *
 * @param source
 * @return Sequence count or simply 0 if an error has occurred
 */
CALUMIANIMATION_API uint64_t GetScalarSqSizeC(const CALUMI::UNIV::AnimationBlock* source);
/**
 * @note A good default tolerance may be 1.0/5000.0
 * @param source
 * @param tolerance
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Operation Completed Successfully
 */
CALUMIANIMATION_API int ExecuteRDPReduction_ScalarC(const CALUMI::UNIV::AnimationBlock* source, float tolerance);
/**
 *
 * @param block
 * @param prtySq
 * @param size
 * @param overwrite
 * @return Number of entries added or simply 0 if an error has occurred
 */
CALUMIANIMATION_API bool AddPrioritySqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block,
                                                   const CALUMI::UNIV::PriorityFrame* prtySq,
                                                   unsigned int size,
                                                   bool overwrite);
/**
 *
 * @param source
 * @return Ptr to the sequence array or nullptr if error has occurred. Use sequence size to safely iterate through
 * the array
 */
CALUMIANIMATION_API CALUMI::UNIV::PriorityFrame* GetPrioritySqArrayC(const CALUMI::UNIV::AnimationBlock* source);
/**
 * @param source
 * @param index
 * @return Ptr to the entry or nullptr if an error has occurred
 */
CALUMIANIMATION_API CALUMI::UNIV::PriorityFrame* GetPriorityFromSqC(const CALUMI::UNIV::AnimationBlock* source, int index);
/**
 *
 * @param source
 * @return Sequence count or simply 0 if an error has occurred
 */
CALUMIANIMATION_API uint64_t GetPrioritySqSizeC(const CALUMI::UNIV::AnimationBlock* source);
}

/// @}
/// @}
