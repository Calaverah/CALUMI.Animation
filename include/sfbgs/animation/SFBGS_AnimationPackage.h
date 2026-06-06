//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include "univ/animation/UNIV_Animation.h"
#include "univ/animation/UNIV_IAnimationPackage.h"
#include "sfbgs/skeletonrig/SFBGS_RigPackage.h"

namespace CALUMI::UNIV
{
	class AnimationBlock;
    class Animation;
}

namespace CALUMI::SFBGS
{
	/**
	 * @relates SFBGS_AnimationPackage
	 */
	inline constexpr auto SFBGS_ANIM_PACKAGE = "SFBGS_ANIM_PACKAGE";

	/// @addtogroup anim_packages
	/// @{
	/**
	 * @brief A game specific package of data that can be applied directly to an animation
	 */
	class CALUMIANIMATION_API SFBGS_AnimationPackage : public UNIV::IAnimationPackage
	{
	public:
		/// @name Initialization
		/// @{
		SFBGS_AnimationPackage();
		SFBGS_AnimationPackage(const SFBGS_AnimationPackage& other);
		~SFBGS_AnimationPackage() override;
		/// @}

		/// @name Amended Animation Blocks
		/// @{

		static constexpr uint16_t MaxAmendedBlockCount = 0xFFU;

		/**
		 * @brief The amount of amended animation curves that apply to non skeleton.rig components, AnimObjects for instance
		 * @return Count
		 */
		[[nodiscard]] uint16_t amendedBlockCount() const;

		/**
		 * @brief Set's the precision to use when compressing this set of AnimationBlocks, will override the SkeletonRig that houses this package
		 * @details It is unlikely that this is used like this in game, but it's provided as an option just in case or for researching/experimentation
		 * @param precision Starfield PrecisionSet to use instead of the SkeletonRig
		 */
		void overrideRigPrecision(const PrecisionSet& precision = PrecisionSet::DefaultPrecision()) const;
		/**
			 * @brief Returns the PrecisionSet that will be used if set to override the rig's precision
			 * @return
			 */
		[[nodiscard]] PrecisionSet overridePrecisionSet() const;
		/**
			 * @brief
			 * @return Whether this animation block set will be compressed using rig precision or its own precision
			 */
		[[nodiscard]] bool usesRigForPrecision() const;
		/**
			 * @brief Set's this set of AnimationBlocks to use the SkeletonRig's precision values, as is default
			 */
		void resetPrecision() const;

		/**
			 * @brief Appends the AmendedBlock list with the given block using the Block's BoneName as the hashing key.
			 * @param block Animation Block entry with unique bone name to be used as the hashing key
			 * @param overwrite Determines if the block will overwrite another if the same hash key is found
			 * @return Whether the operation was successful
			 */
		bool addAmendedBlock(const UNIV::AnimationBlock& block, bool overwrite = true) const; // NOLINT(*-use-nodiscard)
		/**
			 * @brief Appends the AmendedBlock list with the given block using the hashing key given as an argument.
			 * @param hash
			 * @param block
			 * @param overwrite Determines if the block will overwrite another if the same hash key is found
			 * @return
			 */
		bool addAmendedBlock(uint32_t hash, const UNIV::AnimationBlock& block, bool overwrite = true) const; // NOLINT(*-use-nodiscard)
		/**
			 * @brief Removes the block with the given name
			 * @param name
			 * @return Whether the operation was successful
			 */
		bool removeAmendedBlock(const char* name) const; // NOLINT(*-use-nodiscard)
		/**
			 * @brief Removes the block with the given hash
			 * @param hash
			 * @return Whether the operation was successful
			 */
		bool removeAmendedBlock(uint32_t hash) const; // NOLINT(*-use-nodiscard)
		/**
			 * @brief Removes the block at the given index
			 * @param index
			 * @return Whether the operation was successful
			 */
		bool removeAmendedBlockUsingIndex(int index) const; // NOLINT(*-use-nodiscard)
		/**
			 * @brief
			 * @param hash Hash key found from the string
			 * @return Whether the amended block exists
			 */
		[[nodiscard]] bool hasAmendedBlock(uint32_t hash) const;
		/**
			 * @brief
			 * @param name String used to generate the hash key
			 * @return Whether the amended block exists
			 */
		[[nodiscard]] bool hasAmendedBlock(const char* name) const;
		/**
			 * @brief Searches for the block via hash
			 * @param hash
			 * @return -1 if the index does not exist
			 */
		[[nodiscard]] int findAmendedBlock(uint32_t hash) const;
		/**
			 * @brief Searches for the block via name
			 * @param name
			 * @return The index of the hashed block that matches the given string
			 */
		[[nodiscard]] int findAmendedBlock(const char* name) const;
		/**
			 * @brief
			 * @param index
			 * @return
			 */
		[[nodiscard]] UNIV::AnimationBlock* amendedBlock(int index) const;

		/**
			 * @brief Returns the hash associated with the block at the given index
			 * @param index
			 * @return Hash key, 0xFFFFFFFF if index is out of bounds
			 */
		[[nodiscard]] uint32_t amendedBlockHash(int index) const;


		/// @}
		/// @name Operators
		/// @{
		SFBGS_AnimationPackage& operator=(const SFBGS_AnimationPackage& other);
		bool operator==(const IPackage& other) const override;
		/// @}
		/// @name Json
		/// @{
		/**
		 * @return
		 */
		[[nodiscard]] Utilities::JsonObject toJson() const override;
		/**
		 * @param data
		 */
		void fromJson(const Utilities::JsonObject& data) override;
		/// @}

		/**
			 * @brief A way to get a string describing the package.
			 * @return A string of the package type, SFBGS_ANIM_PACKAGE
			 */
		[[nodiscard]] const char* packageType() const override;

	private:
		struct Impl;
		Impl* pImpl;

	protected:
		[[nodiscard]] SFBGS_AnimationPackage* clone() const override;
	public:
	/// @name Package Handling
	/// @{

	/**
	 * @brief Adds a Starfield animation package to the given animation
	 * @param animation
	 * @param overwrite If true, will reset an existing Starfield package, if one already exists
	 * @return Whether the operation was successful
	 */
	static bool AddPackage(const UNIV::Animation& animation, bool overwrite = true);
	/**
	 * @param animation
	 * @return Whether a Starfield package was removed successfully or not
	 */
	static bool RemovePackage(const UNIV::Animation& animation);

	/**
	 *
	 * @param animation
	 * @return The SFBGS Animation Package on this Animation. If none exists, one will be created.
	 */
	[[nodiscard]] static SFBGS_AnimationPackage& GetPackage(const UNIV::Animation& animation);

	};
	/// @}
	/// @}
}

	/**
	 *
	 *
	 * @addtogroup c_anim_packages
	 * @{
	 * @defgroup c_sfbgs_anim_package Starfield
	 * @{
	 */
	extern "C"
	{
	/**
	 * @brief Adds a new Starfield Animation Package to the given animation
	 * @param animation UNIV Animation
	 * @param overwrite Will reset the existing package to it's initial state if found
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Package Added\n 1 = Package Was Not Added
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_AddPackageToAnimationC(const CALUMI::UNIV::Animation* animation, bool
		overwrite);
	/**
	 * @brief Removes a Starfield Animation Package from the given animation
	 * @param animation CALUMI::UNIV Animation
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Package Removed\n 1 = Package Was Not Removed
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_RemovePackageFromAnimationC(const CALUMI::UNIV::Animation* animation);

	/**
	 * @brief Searches for an amended block with a hash generated with the given name
	 * @param animation
	 * @param name Used to generate hash, empty strings are allowed
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = False\n 1 = True
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_HasAmendedBlockC(const CALUMI::UNIV::Animation* animation, const
		char* name);
	/**
	 * @brief Searches for an amended block with a given hash
	 * @param animation
	 * @param hash Generated hash
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = False\n 1 = True
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_HasAmendedBlockHashC(const CALUMI::UNIV::Animation* animation,
		uint32_t hash);

	/**
	 * @brief
	 * @param animation
	 * @return Size of amended block vector or -1 if there is an error
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_GetAmendedBlockCountC(const CALUMI::UNIV::Animation* animation);

	/**
	 * @brief Sets the amended animation block to use its own *Default* precision values when compressing. Not recommended.
	 * @param animation
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_OverrideRigWithDefaultPrecisionC(const CALUMI::UNIV::Animation* animation);
	/**
	 * @brief Sets the amended animation block to use its own *First Person* precision values when compressing. Not recommended.
	 * @param animation
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_OverrideRigWith1stPersonPrecisionC(const CALUMI::UNIV::Animation*
		animation);
	/**
	 * @brief Sets the amended animation block to use its own *Ship* precision values when compressing. Not recommended.
	 * @param animation
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_OverrideRigWithShipPrecisionC(const CALUMI::UNIV::Animation*
		animation);
	/**
	 * @brief Sets the amended animation block to use its own *Custom* precision values when compressing. Not recommended.
	 * @param animation
	 * @param custom1
	 * @param custom2
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_OverrideRigWithCustomPrecisionC(const CALUMI::UNIV::Animation*
		animation, float custom1, float custom2);

	/**
	 * @brief If set to override the rig's precision values, the block will use the returned value for High precision
	 * @param animation
	 * @return High precision value to use if set to override, NaN if there is an error
	 */
	CALUMIANIMATION_API float SFBGSAnimationPackage_GetOverridePrecisionHighC(const CALUMI::UNIV::Animation* animation);
	/**
	 * @brief If set to override the rig's precision values, the block will use the returned value for Low precision
	 * @param animation
	 * @return Low precision value to use if set to override, NaN if there is an error
	 */
	CALUMIANIMATION_API float SFBGSAnimationPackage_GetOverridePrecisionLowC(const CALUMI::UNIV::Animation* animation);

	/**
	 * @brief A convenient method for determining the name of the precision set.
	 * @param animation
	 * @return Will return "Follows Rig" if the package is not set to override, or nullptr if there is an error
	 */
	CALUMIANIMATION_API const char* SFBGSAnimationPackage_GetPrecisionSet(const CALUMI::UNIV::Animation* animation);
	/**
	 * @brief
	 * @param animation
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = False\n 1 = True
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_UsesRigPrecision(const CALUMI::UNIV::Animation* animation);
	/**
	 * @brief Resets the package to follow the rig's precision set
	 * @param animation
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Reset
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_ResetPrecision(const CALUMI::UNIV::Animation* animation);

	/**
	 * @brief Adds an amended block to the animation to represent a non rig based animation block
	 * @param animation
	 * @param block Ptr to block to add, once added, ptr will then point to the package-owned block
	 * @param overwrite If a block with the same name generated hash is found, setting this to true will overwrite it
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Addition\n 1 = Block Was Not Added
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_AddAmendedBlockWithNameC(const CALUMI::UNIV::Animation* animation,
		const CALUMI::UNIV::AnimationBlock* block, bool overwrite);
	/**
	 * @brief Adds an amended block to the animation to represent a non rig based animation block
	 * @param animation
	 * @param hash Hash used to store the animation block during export
	 * @param block Ptr to block to add, once added, ptr will then point to the package-owned block
	 * @param overwrite If a block with the same hash is found, setting this to true will overwrite it
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Addition\n 1 = Block Was Not Added
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_AddAmendedBlockWithHashC(const CALUMI::UNIV::Animation* animation,
		uint32_t hash, const CALUMI::UNIV::AnimationBlock* block, bool overwrite);

	/**
	 * @brief Will remove an animation block with a matching hash generated from the given name
	 * @param animation
	 * @param name Generates hash
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Removal\n 1 = Block Was Not Removed
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_RemoveAmendedBlockWithNameC(const CALUMI::UNIV::Animation* animation, const char* name);
	/**
	 * @brief Will remove an animation block with a matching hash
	 * @param animation
	 * @param hash Hash used to store block
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Removal\n 1 = Block Was Not Removed
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_RemoveAmendedBlockWithHashC(const CALUMI::UNIV::Animation* animation,
		uint32_t hash);
	/**
	 * @brief Will remove an animation block with at the given index
	 * @param animation
	 * @param index Index of block
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Removal\n 1 = Block Was Not Removed
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_RemoveAmendedBlockWithIndexC(const CALUMI::UNIV::Animation* animation, uint16_t index);
	/**
	 * @brief
	 * @param animation
	 * @param name
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = False\n 1 = True
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_HasAmendedBlockWithNameC(const CALUMI::UNIV::Animation* animation,
		const char* name);
	/**
	 * @brief
	 * @param animation
	 * @param hash
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = False\n 1 = True
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_HasAmendedBlockWithHashC(const CALUMI::UNIV::Animation* animation,
		uint32_t hash);
	/**
	 * @brief Finds an amended block and returns the index
	 * @param animation
	 * @param name Used to generate hash
	 * @return Error Code:\n -1 = Invalid Ptr\n n = Index
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_FindAmendedBlockWithNameC(const CALUMI::UNIV::Animation* animation, const char* name);
	/**
	 * @brief Finds an amended block and returns the index
	 * @param animation
	 * @param hash Generated hash used to store block
	 * @return Error Code:\n -1 = Invalid Ptr\n n = Index
	 */
	CALUMIANIMATION_API int SFBGSAnimationPackage_FindAmendedBlockWithHashC(const CALUMI::UNIV::Animation* animation, uint32_t hash);
	}

/// @}
/// @}

