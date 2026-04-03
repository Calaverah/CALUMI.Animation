//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include "univ/animation/UNIV_IAnimationPackage.h"
#include "sfbgs/skeletonrig/SFBGS_RigPackage.h"

namespace CALUMI {

	namespace UNIV {
		struct AnimationBlock;
	}

	namespace SFBGS {

		inline constexpr const char* SFBGS_ANIM_PACKAGE = "SFBGS_ANIM_PACKAGE";

		/**
		 * @brief A game specific package of data that can be applied directly to an animation
		 */
		struct CALUMIANIMATION_API SFBGS_AnimationPackage : public UNIV::IAnimationPackage
		{

			/// @name Initialization
			/// @{
			SFBGS_AnimationPackage();
			SFBGS_AnimationPackage(const SFBGS_AnimationPackage& other);
			~SFBGS_AnimationPackage() override;
			/// @}

		public:
			/// @name Amended Animation Blocks
			/// @{
			
			inline static constexpr uint16_t MaxAmendedBlockCount = 0xFFFFU;

			/**
			 * @brief The amount of amended animation curves that apply to non skeleton.rig components, AnimObjects for instance
			 * @return Count
			 */
			uint16_t getAmendedBlockCount() const;

			/**
			 * @brief Set's the precision to use when compressing this set of AnimationBlocks, will override the SkeletonRig that houses this package
			 * @details It is unlikely that this is used like this in game, but it's provided as an option just in case or for researching/experimentation
			 * @param precision Starfield PrecisionSet to use instead of the SkeletonRig
			 */
			void overrideRigPrecision(PrecisionSet precision = PrecisionSet::DefaultPrecision());
			/**
			 * @brief Returns the PrecisionSet that will be used if set to override the rig's precision
			 * @return 
			 */
			SFBGS::PrecisionSet getOverridePrecisionSet() const;
			/**
			 * @brief 
			 * @return Whether this animation block set will be compressed using rig precision or it's own precision 
			 */
			bool usesRigForPrecision() const;
			/**
			 * @brief Set's this set of AnimationBlocks to use the SkeletonRig's precision values, as is default
			 */
			void resetPrecision();

			/**
			 * @brief Appends the AmendedBlock list with the given block using the Block's BoneName as the hashing key.
			 * @param block Animation Block entry with unique bone name to be used as the hashing key
			 * @param overwrite Determines if the block will overwrite another if the same hash key is found
			 * @return Whether the operation was successful
			 */
			bool addAmendedBlock(UNIV::AnimationBlock block, bool overwrite = true);
			/**
			 * @brief Appends the AmendedBlock list with the given block using the hashing key given as an argument.
			 * @param hash 
			 * @param block
			 * @param overwrite Determines if the block will overwrite another if the same hash key is found
			 * @return 
			 */
			bool addAmendedBlock(uint32_t hash, UNIV::AnimationBlock block, bool overwrite = true);
			/**
			 * @brief Removes the block with the given name
			 * @param name
			 * @return Whether the operation was successful
			 */
			bool removeAmendedBlock(const char* name);
			/**
			 * @brief Removes the block with the given hash
			 * @param hash 
			 * @return Whether the operation was successful
			 */
			bool removeAmendedBlock(uint32_t hash);
			/**
			 * @brief Removes the block at the given index
			 * @param index 
			 * @return Whether the operation was successful
			 */
			bool removeAmendedBlock(int index);
			/**
			 * @brief 
			 * @param hash Hash key found from the string
			 * @return Whether the amended block exists
			 */
			bool hasAmendedBlock(uint32_t hash) const;
			/**
			 * @brief 
			 * @param hash String used to generate the hash key
			 * @return Whether the amended block exists
			 */
			bool hasAmendedBlock(const char* hash) const;
			/**
			 * @brief Searches for the block via hash
			 * @param hash 
			 * @return -1 if the index does not exist
			 */
			int findAmendedBlock(uint32_t hash) const;
			/**
			 * @brief Searches for the block via name
			 * @param name 
			 * @return The index of the hashed block that matches the given string
			 */
			int findAmendedBlock(const char* name) const;
			/**
			 * @brief 
			 * @param index 
			 * @return 
			 */
			UNIV::AnimationBlock* getAmendedBlock(int index) const;

			/**
			 * @brief Returns the hash associated with the block at the given index
			 * @param index 
			 * @return Hash key, 0xFFFFFFFF if index is out of bounds
			 */
			uint32_t getAmendedBlockHash(int index) const;


			/// @}
			/// @name Operators
			/// @{
			SFBGS_AnimationPackage& operator=(const SFBGS_AnimationPackage& other);
			/// @}
		public:
			/**
			 * @brief A way to get a string describing the package.
			 * @return A string of the package type, SFBGS_ANIM_PACKAGE
			 */
			const char* getPackageType() const override;

			/**
			 * @brief Serialization of the Starfield Animation Package
			 * @param indents Amount of spaces for formatting
			 * @return The serialized struct as a StringContainer
			 */
			Utilities::StringContainer ToJSON(size_t indents) const override;

		private:
			struct Impl;
			Impl* pImpl;

			// Inherited via IAnimationPackage
			SFBGS_AnimationPackage* Clone() const override;
};

		bool CreateNewSFBGSAnimationPackage(UNIV::Animation& animation, bool overwrite = true);
		bool RemoveSFBGSAnimationPackage(UNIV::Animation& animation);

		/**
		 * 
		 *
		 * @addtogroup c_univ_anim_package
		 * @{
		 * @defgroup c_sfbgs_anim_package Starfield
		 * @{
		 */
		extern "C"
		{
			/**
			 * @brief Adds a new Starfield Animation Package to the given animation
			 * @param animation UNIV Animation
			 * @param errorMessage *optional* Error message container
			 * @param overwrite Will reset the existing package to it's initial state if found
			 * @return Whether the package was successfully added
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_AddPackageToAnimationC(UNIV::Animation* animation, Utilities::StringContainer* errorMessage, bool overwrite);
			/**
			 * @brief Removes a Starfield Animation Package from the given animation
			 * @param animation UNIV Animation
			 * @param errorMessage *optional* Error message container
			 * @return Whether the package was successfully removed
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_RemovePackageFromAnimationC(UNIV::Animation* animation, Utilities::StringContainer* errorMessage);

			/**
			 * @brief Searches for an amended block with a hash generated with the given name
			 * @param animation 
			 * @param name Used to generate hash
			 * @return Whether the amended block exists
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_HasAmendedBlockC(UNIV::Animation* animation, const char* name);
			/**
			 * @brief Searches for an amended block with a given hash
			 * @param animation 
			 * @param hash Generated hash
			 * @return Whether the amended block exists
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_HasAmendedBlockHashC(UNIV::Animation* animation, uint32_t hash);

			/**
			 * @brief 
			 * @param animation 
			 * @return Size of amended block vector 
			 */
			CALUMIANIMATION_API uint16_t SFBGSAnimationPackage_GetAmendedBlockCountC(UNIV::Animation* animation);

			/**
			 * @brief Sets the amended animation block to use it's own *Default* precision values when compressing. Not recommended.
			 * @param animation 
			 */
			CALUMIANIMATION_API void SFBGSAnimationPackage_OverrideRigWithDefaultPrecisionC(UNIV::Animation* animation);
			/**
			 * @brief Sets the amended animation block to use it's own *First Person* precision values when compressing. Not recommended.
			 * @param animation
			 */
			CALUMIANIMATION_API void SFBGSAnimationPackage_OverrideRigWith1stPersonPrecisionC(UNIV::Animation* animation);
			/**
			 * @brief Sets the amended animation block to use it's own *Ship* precision values when compressing. Not recommended.
			 * @param animation
			 */
			CALUMIANIMATION_API void SFBGSAnimationPackage_OverrideRigWithShipPrecisionC(UNIV::Animation* animation);
			/**
			 * @brief Sets the amended animation block to use it's own *Custom* precision values when compressing. Not recommended.
			 * @param animation
			 */
			CALUMIANIMATION_API void SFBGSAnimationPackage_OverrideRigWithCustomPrecisionC(UNIV::Animation* animation, float custom1, float custom2);

			/**
			 * @brief If set to override the rig's precision values, the block will use the returned value for High precision
			 * @param animation 
			 * @return High precision value to use if set to override
			 */
			CALUMIANIMATION_API float SFBGSAnimationPackage_GetOverridePrecisionHighC(UNIV::Animation* animation);
			/**
			 * @brief If set to override the rig's precision values, the block will use the returned value for Low precision
			 * @param animation 
			 * @return Low precision value to use if set to override
			 */
			CALUMIANIMATION_API float SFBGSAnimationPackage_GetOverridePrecisionLowC(UNIV::Animation* animation);

			/**
			 * @brief A convenient method for determining the name of the precision set.
			 * @param animation 
			 * @return Will return "Follows Rig" if the package is not set to override
			 */
			CALUMIANIMATION_API const char* SFBGSAnimationPackage_GetPrecisionSet(UNIV::Animation* animation);
			/**
			 * @brief 
			 * @param animation 
			 * @return Whether the precision is set to follow rig (*true*) or override (*false*) 
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_UsesRigPrecision(UNIV::Animation* animation);
			/**
			 * @brief Resets the package to follow the rig's precision set
			 * @param animation 
			 */
			CALUMIANIMATION_API void SFBGSAnimationPackage_ResetPrecision(UNIV::Animation* animation);

			/**
			 * @brief Adds an amended block to the animation to represent a non rig based animation block
			 * @param animation 
			 * @param block Ptr to block to add, once added, ptr will then point to the package-owned block
			 * @param errorMessage *optional* Error message container
			 * @param overwrite If a block with the same name generated hash is found, setting this to true will overwrite it
			 * @return 
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_AddAmendedBlockWithNameC(UNIV::Animation* animation, UNIV::AnimationBlock* block, Utilities::StringContainer* errorMessage, bool overwrite);
			/**
			 * @brief Adds an amended block to the animation to represent a non rig based animation block
			 * @param animation 
			 * @param hash Hash used to store the animation block during export
			 * @param block Ptr to block to add, once added, ptr will then point to the package-owned block
			 * @param errorMessage *optional* Error message container
			 * @param overwrite If a block with the same hash is found, setting this to true will overwrite it
			 * @return 
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_AddAmendedBlockWithHashC(UNIV::Animation* animation, uint32_t hash, UNIV::AnimationBlock* block, Utilities::StringContainer* errorMessage, bool overwrite);

			/**
			 * @brief Will remove an animation block with a matching hash generated from the given name
			 * @param animation 
			 * @param name Generates hash
			 * @param errorMessage *optional* Error message container
			 * @return Whether the block was removed
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_RemoveAmendedBlockWithNameC(UNIV::Animation* animation, const char* name, Utilities::StringContainer* errorMessage);
			/**
			 * @brief Will remove an animation block with a matching hash
			 * @param animation 
			 * @param hash Hash used to store block
			 * @param errorMessage *optional* Error message container
			 * @return 
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_RemoveAmendedBlockWithHashC(UNIV::Animation* animation, uint32_t hash, Utilities::StringContainer* errorMessage);
			/**
			 * @brief Will remove an animation block with at the given index
			 * @param animation 
			 * @param index Index of block
			 * @param errorMessage *optional* Error message container
			 * @return 
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_RemoveAmendedBlockWithIndexC(UNIV::Animation* animation, uint16_t index, Utilities::StringContainer* errorMessage);
			/**
			 * @brief 
			 * @param animation 
			 * @param name 
			 * @return 
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_HasAmendedBlockWithNameC(UNIV::Animation* animation, const char* name);
			/**
			 * @brief 
			 * @param animation 
			 * @param hash 
			 * @return 
			 */
			CALUMIANIMATION_API bool SFBGSAnimationPackage_HasAmendedBlockWithHashC(UNIV::Animation* animation, uint32_t hash);
			/**
			 * @brief Finds an amended block and returns the index
			 * @param animation 
			 * @param name Used to generate hash
			 * @return -1 if not found
			 */
			CALUMIANIMATION_API int SFBGSAnimationPackage_FindAmendedBlockWithNameC(UNIV::Animation* animation, const char* name);
			/**
			 * @brief Finds an amended block and returns the index
			 * @param animation 
			 * @param hash Generated hash used to store block
			 * @return 
			 */
			CALUMIANIMATION_API int SFBGSAnimationPackage_FindAmendedBlockWithHashC(UNIV::Animation* animation, uint32_t hash);
		}

		/// @}
		/// @}
} }