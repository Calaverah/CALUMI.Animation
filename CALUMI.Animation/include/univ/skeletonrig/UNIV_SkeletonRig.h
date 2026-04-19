//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "UNIV_BoneTypes.h"
#include "math/Math.h"
#include "UNIV_IRigPackage.h"
#include <cstdint>

#include "interfaces/ILineage.h"

namespace CALUMI::UNIV
{
	class SkeletonRig;

	//TODO: Bone Doc and Rig Doc

	/**
	 * @brief Universal method of storing skeleton limb data
	 */
	class CALUMIANIMATION_API SkeletonBone : public ILineage
	{
	private:

		/** @name Initialization*/
		/// @{

		explicit SkeletonBone(const ILineage* parent, const char* name);
		// SkeletonBone(const SkeletonBone& other);
	public:
		~SkeletonBone() override;

		/// @}
		/** @name Operators*/
		/// @{
	public:
		// SkeletonBone& operator=(const SkeletonBone& other);

		/// @}
		/// @name ILineage
		/// @{

		/**
		 * @brief Since the rig itself cannot be a child of another bone or rig,
		 * this will return nullptr, signaling that it is the root of the structure
		 */
		[[nodiscard]] const ILineage* parent() const override;
		[[nodiscard]] const SkeletonRig& parentRig() const;

		[[nodiscard]] SkeletonBone* addChildBone(const char* name, const Math::Transform& transform) const;
		[[nodiscard]] SkeletonBone* addChildBone(const char* name, const Math::Vector3& position, const Math::Quaternion& rotation) const;
		[[nodiscard]] SkeletonBone* childBone(const char* name, bool includeDescendents = true) const;
		[[nodiscard]] SkeletonBone* childBone(unsigned int index) const;
		/**
		 *
		 * @return Number of direct children belonging to this bone
		 */
		[[nodiscard]] unsigned int childBoneCount() const;
		/**
		 *
		 * @return Total number of bones belonging to this bone, including children of its direct children
		 */
		[[nodiscard]] unsigned int boneCount() const;
		/**
		 * @brief Returns the bones that are of the given type belonging to this bone
		 * @param type Bone type to search for
		 * @return
		 */
		[[nodiscard]] unsigned int boneTypeCount(BoneType type) const;
		/// @}

	public:

		/** @name Transform*/
		/// @{

		/**
		 * @brief The transform for this object. Expressed position and rotation
		 */
		[[nodiscard]] const Math::Transform& localTransform() const;
		[[nodiscard]] Math::Transform globalTransform() const;
		void setLocalTransform(const Math::Vector3& position, const Math::Quaternion& rotation) const;

		/// @}

	public:

		/** @name Identification*/
		/// @{

		/**
		 * @brief Identifier for this bone
		 * @return name in a string container
		 */
		[[nodiscard]] const char* name() const;
		[[nodiscard]] bool isValidName(const char* name) const;
		bool setName(const char* name) const;

		/// @}
		/** @name Properties*/
		/// @{

		/**
		 * @brief Sets the unique property of this bone
		 * @param boneType Type to set
		 * @param resetExisting If this bone is already set to the given type, will reset to this type's default values
		 * @return Whether the operation was a success
		 */
		[[nodiscard]] bool setBoneTypeProperty(BoneType boneType, bool resetExisting = false) const;
		/**
		 * @brief Gets the unique bone property
		 * @return Ptr to the bone property type and its values
		 */
		[[nodiscard]] const BoneTypeProperties* boneTypeProperty() const;
		/**
		 * @brief Reset the bone properties values
		 * @param boneType Type of bone properties to set this bone to
		 * @return Whether the operation was a success
		 */
		bool resetBoneTypeProperty(BoneType boneType = BoneType::Default) const; // NOLINT(*-use-nodiscard)

	public:
		//TODO: docs
		bool setParentBone(const SkeletonBone& newParent) const; // NOLINT(*-use-nodiscard)

		/**
		 *
		 * @param boneCandidate The potential ancestor of this bone
		 * @return If the boneCandidate is this bone's ancestor (parent, grandparent, or some form of upstream lineage)
		 */
		[[nodiscard]] bool isAncestor(const SkeletonBone& boneCandidate) const;

		/// @}
		/** @name Serialization*/
		/// @{

		/**
		 * @brief Serialization
		 * @param indents Spaces for formatting
		 * @return The strContainer of the serialized struct
		 */
		[[nodiscard]] Utilities::StringContainer toJSON(uint64_t indents) const;

		/// @}

	private:
		struct Impl;
		Impl* pImpl;

	private:
		friend class SkeletonRig;
	};

	/**
	 * @brief Universal method of representing a skeleton/rig/armature.
	 * 
	 * @details The Universal Skeleton Rig aims to keep its data game agnostic with special packages and properties to tie game specific data to the rig. The goal is to create a flexible system for users when updates or additional games are supported.
	 */
	class CALUMIANIMATION_API SkeletonRig : public ILineage
	{
	public:
		/** @name Initialization*/
		/// @{

		SkeletonRig();
		explicit SkeletonRig(const char* _rigName);
		explicit SkeletonRig(const Utilities::StringContainer& _rigName);
		SkeletonRig(const SkeletonRig& input);
		~SkeletonRig() override;

		/// @}

	public:
		/** @name Properties*/
		/// @{

		/**
		 * @brief
		 * @return C String of the rig's name
		 */
		[[nodiscard]] const char* name() const;
		/**
		 * @param name Name to set, unique is advised
		 */
		bool setName(const char* name) const;

		/**
		 * @brief
		 * @return Reference to this rig's package manager
		 */
		[[nodiscard]] RigPackageManager& getPackageManager() const;

		/// @}

	public:
		/** @name Bone Entries */
		/// @{

		/**
		 * @brief Returns a pointer to a bone if one exists under the given name
		 * @param boneName
		 * @return Nullptr if none exists
		 */
		[[nodiscard]] SkeletonBone* bone(const char* boneName) const;
		/**
		 * @brief Returns the bones that are of the given type belonging to this rig
		 * @param type Bone type to search for
		 * @return
		 */
		[[nodiscard]] unsigned int boneTypeCount(BoneType type) const;
		/**
		 * @brief Returns the total amount of bones in this rig
		 */
		[[nodiscard]] unsigned int boneCount() const;
		/**
		 * @return
		 */
		[[nodiscard]] SkeletonBone& root() const;
		/**
		 * @return Full list of bones in this rig, with the only guarantee being that parents proceed children
		 */
		[[nodiscard]] Utilities::StringList boneList() const;

		/// @}

	public:
		/** @name Serialization */
		/// @{

		/**
		 * @brief Serialization
		 * @param indents Spaces for formatting
		 * @return The strContainer of the serialized struct
		 */
		[[nodiscard]] Utilities::StringContainer toJSON(uint64_t indents) const;

		/// @}
		/** @name Static Members
		*/
		/// @{

		/**
		 * @brief It is unlikely to even need this many bones
		 */
		static constexpr unsigned int MaxBoneCount = 512;

		/// @}
	private:
		struct Impl;
		Impl* pImpl;
	};
}

	/**
	 * @addtogroup extern_c
	 * @{
	 * @defgroup c_rig SkeletonRig
	 * @brief Provides C Style API for the UNIV SkeletonRig and its data
	 * @{
	 */
	/// @defgroup c_univ_rig Universal
	/// @{
	extern  "C" {
		/**
		 * @brief Creates a brand new Skeleton Rig
		 
		 * @param _rigName Identifier for the rig, best if unique
		 * @return A dynamically allocated, universal, skeleton rig ptr
		 */
		CALUMIANIMATION_API CALUMI::UNIV::SkeletonRig* CreateSkeletonRigC(const char* _rigName);
		/**
		 * @brief Deletes the dynamically allocated rig that was created, this is the preferred way to delete a rig

		 * @param ptr Ptr to the rig to delete
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool DeleteSkeletonRigC(const CALUMI::UNIV::SkeletonRig* ptr);
		/**
		 * @brief Adds a bone to the given universal skeleton rig
		 
		 * @param rig Universal skeleton rig this bone will be parented to
		 * @param rotationX X component of a unit quaternion
		 * @param rotationY Y component of a unit quaternion
		 * @param rotationZ Z component of a unit quaternion
		 * @param rotationW Imaginary component of a unit quaternion
		 * @param positionX X component of the bone's position
		 * @param positionY Y component of the bone's position
		 * @param positionZ Z component of the bone's position
		 * @param boneName Name of the bone, must be unique
		 * @param parentName Name of this bone's parent
		 * @param usingLocalValues Whether the given transform is relative to the origin (false) or the bone's parent (true)
		 * @param errorMessage Ptr to a string container, if not null a status message is given here
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API CALUMI::UNIV::SkeletonBone* AddBoneToSkeletonRigC(
			const CALUMI::UNIV::SkeletonRig* rig,
			float rotationX, float rotationY, float rotationZ, float rotationW,
			float positionX, float positionY, float positionZ,
			const char* boneName,
			const char* parentName,
			bool usingLocalValues,
			CALUMI::Utilities::StringContainer* errorMessage
		);
		/**
		 * @brief Adds a bone to the given universal skeleton rig
		 
		 * @param rig Universal skeleton rig this bone will be parented to
		 * @param rotationX X rotation in radians
		 * @param rotationY Y rotation in radians
		 * @param rotationZ Z rotation in radians
		 * @param order Euler order to apply for quaternion calculation, XYZ = 0, see Math::Quaternion for the full enum and values
		 * @param positionX X component of the bone's position
		 * @param positionY Y component of the bone's position
		 * @param positionZ Z component of the bone's position
		 * @param boneName Name of the bone, must be unique
		 * @param parentName Name of this bone's parent
		 * @param usingLocalValues Whether the given transform is relative to the origin (false) or the bone's parent (true)
		 * @param errorMessage Ptr to a string container, if not null a status message is given here
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool AddBoneToSkeletonRigWithEulerC(
			const CALUMI::UNIV::SkeletonRig* rig,
			float rotationX, float rotationY, float rotationZ, uint8_t order,
			float positionX, float positionY, float positionZ,
			const char* boneName,
			const char* parentName,
			bool usingLocalValues,
			CALUMI::Utilities::StringContainer* errorMessage
		);

		/**
		 * @brief Sets bone property type
		 
		 * @param bone Ptr to bone receiving the new classification
		 * @param boneType unsigned int form of the bone type enum, see Univ::BoneType
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool SetBoneTypeC(const CALUMI::UNIV::SkeletonBone* bone, uint32_t boneType);
		/**
		 * @brief Removes a bone of the given name, from the rig
		 * @param rig 
		 * @param boneName 
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool RemoveBoneC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName);

		/**
		 *
		 * @param rig Skeleton rig in which this bone resides
		 * @param oldName Name of the bone that will be renamed
		 * @param newName New name of the bone (must be unique and valid)
		 * @return -1 = invalid rig\n 0 = Success\n 1 = Bone not found\n 2 = New name is invalid\n 3 = New name is same
		 * as old\n 4 = Name could not be set
		 */
		CALUMIANIMATION_API int RenameBoneC(const CALUMI::UNIV::SkeletonRig* rig, const char* oldName, const char* newName);
		/**
		 * @brief Normally the root of the rig is set to the 0th index of the bone entry vector, setting this will override that selection, if desired
		 * @param rig 
		 * @param boneName Bone to mark as the root override
		 * @param keepRelative
		 * @param keepChildrenRelative
		 * @return 
		 */
		CALUMIANIMATION_API bool SetRootC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName, bool keepRelative, bool keepChildrenRelative);
		/**
		 * @brief Sets the given bone to a new index, if one exists
		 * @param rig The rig that holds the vector containing the bone
		 * @param boneName The bone to move
		 * @param newIndex The index will be clamped to the bounds of the vector
		 * @return The index the bone is now set to, if the operation failed, -1 is returned
		 */
		CALUMIANIMATION_API int SetBoneIndexC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName, int newIndex);
		/**
		 * @brief Sets bone property type
		 
		 * @param bone Ptr to bone receiving the new classification
		 * @param boneType string form of the bone type enum, see Univ::BoneType
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool SetBoneTypeFromStringC(const CALUMI::UNIV::SkeletonBone* bone, const char* boneType);
		/**
		 * @brief
		 
		 * @param bone Ptr to bone
		 * @return The unsigned int/uint32_t form of the bone type enum
		 */
		CALUMIANIMATION_API uint32_t GetBoneTypeC(const CALUMI::UNIV::SkeletonBone* bone);
		/**
		 * @brief
		 
		 * @param bone Ptr to bone
		 * @return The string form of the bone type enum
		 */
		CALUMIANIMATION_API const char* GetBoneTypeAsStringC(const CALUMI::UNIV::SkeletonBone* bone);
		/**
		 * @brief Set's the twist properties of this bone
		 
		 * @param bone Ptr to bone to set values for
		 * @param reassign True if the type should be set if not already set as twist type
		 * @param twistDriver
		 * @param twistDriverWeight 
		 * @param errorMessage Error message container
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool SetTwistBonePropertiesC(const CALUMI::UNIV::SkeletonBone* bone, bool reassign, const char* twistDriver, float twistDriverWeight, CALUMI::Utilities::StringContainer* errorMessage);
		/**
		 * @brief 
		 
		 * @param bone 
		 * @param errorMessage
		 * @return Index of the bone that dynamically drives this bone in game, empty string if none exists or is not set
		 */
		CALUMIANIMATION_API const char* GetTwistBoneDriverC(const CALUMI::UNIV::SkeletonBone* bone, CALUMI::Utilities::StringContainer* errorMessage);
		/**
		 * @brief 
		 
		 * @param bone 
		 * @param errorMessage
		 * @return Drive parameter of the twist bone interaction, each game may have a different amount of weightedness for this value
		 */
		CALUMIANIMATION_API float GetTwistBoneDriverWeightC(const CALUMI::UNIV::SkeletonBone* bone, CALUMI::Utilities::StringContainer* errorMessage);

		
		/**
		 * @brief Creates an exclusive mirror between two bones
		 
		 * @param rig Ptr to rig in which the pair resides
		 * @param bone1 bone name
		 * @param bone2 bone name
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool CreateBoneMirrorPairC(const CALUMI::UNIV::SkeletonRig* rig, const char* bone1, const char* bone2);
		/**
		 * @brief Unsets the given bone from having a mirrored bone
		 * @param boneName
		 * @param rig
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool ResetBoneMirrorC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName);
		/**
		 * @brief Resets all bones in a given rig to have no mirror
		 
		 * @param rig Ptr to skeleton rig
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool ResetAllBoneMirrorsC(const CALUMI::UNIV::SkeletonRig* rig);
		/**
		 * @brief Scans all bones in the given skeleton rig to verify the mirrors are exclusive or are set to default
		 * @param rig Ptr to rig
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool VerifyExclusiveBoneMirrorsC(const CALUMI::UNIV::SkeletonRig* rig);
		/**
		 * @brief
		 * @param source Ptr to skeleton rig to check
		 * @return The total number of bones belonging to this rig
		 */
		CALUMIANIMATION_API unsigned int GetSkeletonRigBoneCountC(const CALUMI::UNIV::SkeletonRig* source);
		/**
		 * @brief Animated bones that are not driven dynamically in game
		 * @param source Ptr to rig 
		 * @return The total number of animated bones that driven by animation's data prior to runtime
		 */
		CALUMIANIMATION_API unsigned int GetSkeletonRigAnimatedBoneCountC(const CALUMI::UNIV::SkeletonRig* source);
		/**
		 * @param source Ptr to rig
		 * @return The total number of non-animated bones that are driven during runtime (aka Twist bones)
		 */
		CALUMIANIMATION_API unsigned int GetSkeletonRigNonAnimatedBoneCountC(const CALUMI::UNIV::SkeletonRig* source);
		/**
		 * @brief 
		 
		 * @param source Ptr to the skeleton to check
		 * @return Name of skeleton rig
		 */
		CALUMIANIMATION_API const char* GetSkeletonRigNameC(const CALUMI::UNIV::SkeletonRig* source);
		/**
		 * @brief Gets the ptr of a skeleton bone
		 
		 * @param source Ptr to rig
		 * @param boneName
		 * @return Ptr, if one exists, to the skeleton bone at the given index
		 */
		CALUMIANIMATION_API const CALUMI::UNIV::SkeletonBone* GetSkeletonBoneC(const CALUMI::UNIV::SkeletonRig* source, const char* boneName);
		/**
		 * @brief 
		 
		 * @param source Ptr to the bone in question
		 * @return C String of the bone's name
		 */
		CALUMIANIMATION_API const char* GetSkeletonBoneNameC(const CALUMI::UNIV::SkeletonBone* source);
		/**
		 * @brief 
		 
		 * @param source Ptr to a skeleton bone
		 * @return The name of the source bone's parent, if none exist, will return as empty
		 */
		CALUMIANIMATION_API const CALUMI::UNIV::SkeletonBone* GetSkeletonBoneParentC(const CALUMI::UNIV::SkeletonBone* source);
		/**
		 * @brief Returns the rotation of this bone, in global coordinates
		 
		 * @param source Ptr to the bone in question
		 * @return Quaternion representation of the rotation
		 */
		CALUMIANIMATION_API const CALUMI::Math::Quaternion* GetSkeletonBoneRotationC(const CALUMI::UNIV::SkeletonBone* source);
		/**
		 * @brief Returns a heap allocated quaternion of this bone's rotation relative to its parent, if one exists
		 * @param rig 
		 * @param boneName 
		 * @return Nullptr if no bone is found, global rotation if no parent is found
		 */
		CALUMIANIMATION_API CALUMI::Math::Quaternion* GetRelativeSkeletonBoneRotationC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName);
		/**
		 * @brief Returns the position of this bone, in global coordinates		 
		 * @param source Ptr to the bone in question
		 * @return Vector3 (float) representation of the position
		 */
		CALUMIANIMATION_API const CALUMI::Math::Vector3* GetSkeletonBonePositionC(const CALUMI::UNIV::SkeletonBone* source);
		/**
		 * @brief * @brief Returns a heap allocated vector3 of this bone's position relative to its parent, if one exists
		 * @param rig 
		 * @param boneName 
		 * @return Nullptr if no bone is found, global position if no parent is found
		 */
		CALUMIANIMATION_API CALUMI::Math::Vector3* GetRelativeSkeletonBonePositionC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName);
		/**
		 * @brief Checks whether all bone names in this rig are unique
		 * @param source Ptr to the skeleton rig in question
		 * @return Whether all names in the rig are unique and valid
		 */
		CALUMIANIMATION_API bool ValidateSkeletonRigNamesC(const CALUMI::UNIV::SkeletonRig* source);
		/**
		 * @brief Returns the index of the bone in the rig's bone entry vector
		 * @param rig 
		 * @param boneName 
		 * @return -1 if no bone is found
		 */
		CALUMIANIMATION_API int FindBoneInRigC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName);
		/**
		 * @brief Returns the index of the given bone's parent
		 * @param rig 
		 * @param boneName 
		 * @return -1 if no bone, or parent bone, is found
		 */
		CALUMIANIMATION_API int FindBoneParentInRigC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName);
	}

	/**
	 * @}
	 * @}
	 * @}
	 */

