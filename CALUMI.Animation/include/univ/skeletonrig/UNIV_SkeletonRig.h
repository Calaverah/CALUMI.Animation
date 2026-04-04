//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "UNIV_BoneTypes.h"
#include "math/Math.h"
#include "UNIV_IRigPackage.h"

namespace CALUMI{ namespace UNIV{
		

	/**
	 * @brief Universal method of storing skeleton limb data
	 */
	struct CALUMIANIMATION_API SkeletonBone
	{
	public:

		/** @name Initialization*/
		/// @{

		SkeletonBone();
		SkeletonBone(const SkeletonBone& other);
		~SkeletonBone();

		/// @}
		/** @name Operators*/
		/// @{
	public:
		SkeletonBone& operator=(const UNIV::SkeletonBone& other);

		/// @}
	public:

		/** @name Transform*/
		/// @{

		/**
		 * @brief #Rotation in relation to this bone's parent
		 * @return #Rotation in the form of a Quaternion
		 */
		const Math::Quaternion& LocalRotation() const;
		/**
		 * @brief #Rotation in relation to the origin of the skeleton rig
		 * @return #Rotation in the form of a Quaternion
		 */
		const Math::Quaternion& GlobalRotation() const;
		/**
		 * @brief Position in relation to this bone's parent
		 * @return Position in the form of a Vector3 (float based)
		 */
		const Math::Vector3& LocalPosition() const;
		/**
		 * @brief Position in relation to the origin of the skeleton rig
		 * @return Position in the form of a Vector3 (float base)
		 */
		const Math::Vector3& GlobalPosition() const;

	private:
		/**
		 * @brief Method for skeleton rig, or other privileged classes, to set the transform members of this bone
		 * @param local Relative to parent bone
		 * @param global Relative to rig origin
		 */
		void SetRotation(Math::Quaternion& local, Math::Quaternion& global);
		/**
		 * @brief Method for skeleton rig, or other privileged classes, to set the transform members of this bone
		 * @param local Relative to parent bone
		 * @param global Relative to rig origin
		 */
		void SetPosition(Math::Vector3& local, Math::Vector3& global);

		/// @}

	public:

		/** @name Identification*/
		/// @{

		/**
		 * @brief Identifier for this bone
		 * @return Name in a string container
		 */
		Utilities::StringContainer& Name() const;

		/// @}
		/** @name Properties*/
		/// @{

		/**
		 * @brief Sets the unique property of this bone
		 * @param boneType Type to set
		 * @param resetExisting If this bone is already set to the given type, will reset to this type's default values
		 * @return Whether the operation was a success
		 */
		bool SetBoneTypeProperty(UNIV::BoneType boneType, bool resetExisting = false);
		/**
		 * @brief Gets the unique bone property
		 * @return Ptr to the bone property type and its values
		 */
		const BoneTypeProperties* GetBoneTypeProperty();
		/**
		 * @brief Reset the bone properties values
		 * @param boneType Type of bone properties to set this bone to
		 * @return Whether the operation was a success
		 */
		bool ResetBoneTypeProperty(UNIV::BoneType boneType = UNIV::BoneType::Default);

	public:

		/**
		 * @brief Pairs this bone with another bone in the rig
		 * @param idx The bone index in which this bone will mirror
		 */
		void SetMirrorBoneIndex(int idx);
		/**
		 * @brief Resets this bone to no longer mirror any other bones
		 */
		void ResetMirrorBoneIndex();
		/**
		 * @brief Gets the bone index to which this bone is set to mirror
		 * @return The mirrored index, or -1 if none is set
		 */
		int GetMirrorBoneIndex() const;

	public:
		/**
		 * @brief Sets the index of this bone's parent
		 * @param idx The index of the parent, or -1 if this is the root bone
		 */
		void SetParentBoneIndex(int idx);
		/**
		 * @brief Reset's the parent index to the skeleton's root
		 */
		void ResetParentBoneIndex();
		/**
		 * @brief Sets this bone to have no parent
		 */
		void SetAsRootBone();
		/**
		 * @brief 
		 * @return The index of this bone's parent 
		 */
		int GetParentBoneIndex() const;

		/// @}
		/** @name Serialization*/
		/// @{

		/**
		 * @brief Serialization
		 * @param indents Spaces for formatting
		 * @return The strContainer of the serialized struct
		 */
		Utilities::StringContainer ToJSON(std::size_t indents) const;

		/// @}

	private:
		struct Impl;
		Impl* pImpl;

	private:
		friend struct SkeletonRig;
	};

	/**
	 * @brief Universal method of representing a skeleton/rig/armature.
	 * 
	 * @details The Universal Skeleton Rig aims to keep it's data game agnostic with special packages and properties to tie game specific data to the rig. The goal is to create a flexible system for users when updates or additional games are supported.
	 */
	struct CALUMIANIMATION_API SkeletonRig
	{
	public:
		/** @name Initialization*/
		/// @{

		SkeletonRig();
		SkeletonRig(const char* _rigName);
		SkeletonRig(Utilities::StringContainer& _rigName);
		SkeletonRig(const SkeletonRig& input);
		~SkeletonRig();
		
		/// @}
		/** @name Operators*/
		/// @{
		
	public:
		SkeletonRig& operator=(const SkeletonRig& other);

		/// @}

	public:
		/** @name Properties*/
		/// @{

		/**
		 * @brief 
		 * @return String Container of the rig's name
		 */
		Utilities::StringContainer& RigName() const;
		/**
		 * @brief The entry list for this rig's bones
		 * @return Vector container, currently allows full control of the listing until further implementation is added
		 */
		Utilities::VectorContainer<SkeletonBone>& BoneEntries() const;
		/**
		 * @brief 
		 * @return Reference to this rig's package manager 
		 */
		RigPackageManager& getPackageManager() const;

		/// @}
		/** @name Validation*/
		/// @{

		/**
		 * @brief Checks whether all of the bone names in this rig are unique
		 * @return Whether all names in the rig are unique and valid, includes an error message in the "Expected" container
		 */
		Utilities::ExpectedContainer< bool, Utilities::StringContainer> ValidateNames() const;
		/**
		 * @brief Checks whether all of the bones in this rig have a parent index that is lower than the respective bone's index
		 * @return Whether all parent indices in the rig are valid, includes an error message in the "Expected" container
		 */
		Utilities::ExpectedContainer< bool, Utilities::StringContainer> ValidateParentIndices();
		/**
		 * @brief Checks if each bone mirror's another exclusively, or not at all
		 * @return Whether each bone has an exclusive mirror, or no mirror at all
		 */
		bool VerifyExclusiveBoneMirrors();

		/// @}

	public:
		/** @name Bone Entries */
		/// @{
		
		/**
		 * @brief The preferred way to add bones to a universal rig definition
		 * @param rotation Relative rotational entry as Quaternion
		 * @param position Relative position as Vector3 (float)
		 * @param boneName Must be unique string
		 * @param parentName If a parent is not found, the parent will default to the root bone
		 * @param localValues Determines whether the transform is relative to parent or the rig origin
		 * @return Whether the operation was a success
		 */
		bool AddBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, Utilities::StringContainer boneName, Utilities::StringContainer parentName, bool localValues = true);
		/**
		 * @brief The preferred way to add bones to a universal rig definition
		 * @param rotation Relative rotational entry as Quaternion
		 * @param position Relative position as Vector3 (float)
		 * @param boneName Must be unique string
		 * @param parentName If a parent is not found, the parent will default to the root bone
		 * @param localValues Determines whether the transform is relative to parent or the rig origin
		 * @return Whether the operation was a success
		 */
		bool AddBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, Utilities::StringContainer boneName, int parentIndex, bool localValues = true);
		/**
		 * @brief Renames a bone in the rig, if one exists
		 * @param oldBoneName The name of the bone to rename
		 * @param newBoneName The new name for the bone
		 * @return Whether the operation was a success
		 */
		bool RenameBone(const char* oldBoneName, const char* newBoneName);
		/**
		 * @brief Renames a bone in the rig, if one exists
		 * @param boneIndex The index of the bone to rename
		 * @param newBoneName The new name for the bone
		 * @return Whether the operation was a success
		 */
		bool RenameBone(std::size_t boneIndex, const char* newBoneName);
		/**
		 * @brief Pairs two bones to mirror eachother
		 * @param i1 Index of the first bone
		 * @param i2 Index of the second bone
		 * @return Whether the operation was a success
		 */
		bool CreateBoneMirrorPair(int i1, int i2);
		/**
		 * @brief Resets all bones in the rig to have no mirrored entry
		 * @return Whether the operation was a success
		 */
		bool ResetAllBoneMirrors();

		/// @}

	public:
		/** @name Bone Indices */
		/// @{

		/**
		 * @brief Returns the bones that are animation driven, rather than dynamically controlled
		 * @return The number of animated bones (ie not in-game twist)
		 */
		std::size_t GetAnimatedBoneCount();
		/**
		 * @brief Returns the total amount of bones, whether they are driven by curves or dynamically in game
		 * @return The total number of bones on this rig
		 */
		std::size_t GetBoneCount() const;
		/**
		 * @brief Finds the 
		 * @param boneName 
		 * @return index of the bone, or -1 if doesn't exist
		 */
		int GetBoneIndex(Utilities::StringContainer boneName) const;

		/// @}

	public:
		/** @name Serialization */
		/// @{

		/**
		 * @brief Serialization
		 * @param indents Spaces for formatting
		 * @return The strContainer of the serialized struct
		 */
		Utilities::StringContainer ToJSON(std::size_t indents) const;
		
		/// @}
		/** @name Static Members
		*/
		/// @{
		
		/**
		 * @brief It is unlikely to even need this many bones
		 */
		static const unsigned int MaxBoneCount = 512;
		
		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 * @addtogroup extern_c
	 * @{
	 * @defgroup c_univ_rig Universal SkeletonRig "C" API
	 * @brief Provides C Style API for the UNIV SkeletonRig and its data
	 * @{
	 */
	extern  "C" {
		/**
		 * @brief Creates a brand new Skeleton Rig
		 
		 * @param _rigName Identifier for the rig, best if unique
		 * @return A dynamically allocated, universal, skeleton rig ptr
		 */
		CALUMIANIMATION_API SkeletonRig* CreateSkeletonRigC(const char* _rigName);
		/**
		 * @brief Deletes the dynamically allocated rig that was created, this is the preferred way to delete a rig

		 * @param ptr Ptr to the rig to delete
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool DeleteSkeletonRigC(SkeletonRig* ptr);
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
		 * @param parentIndex Index of this bone's parent
		 * @param usingLocalValues Whether the given transform is relative to the origin (false) or the bone's parent (true)
		 * @param errorMessage Ptr to a string container, if not null a status message is given here
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool AddBoneToSkeletonRigC(
			SkeletonRig* rig,
			float rotationX, float rotationY, float rotationZ, float rotationW,
			float positionX, float positionY, float positionZ,
			const char* boneName,
			int parentIndex,
			bool usingLocalValues,
			Utilities::StringContainer* errorMessage
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
		 * @param parentIndex Index of this bone's parent
		 * @param usingLocalValues Whether the given transform is relative to the origin (false) or the bone's parent (true)
		 * @param errorMessage Ptr to a string container, if not null a status message is given here
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool AddBoneToSkeletonRigWithEulerC(
			SkeletonRig* rig,
			float rotationX, float rotationY, float rotationZ, uint8_t order,
			float positionX, float positionY, float positionZ,
			const char* boneName,
			int parentIndex,
			bool usingLocalValues,
			Utilities::StringContainer* errorMessage
		);

		/**
		 * @brief Sets bone property type
		 
		 * @param bone Ptr to bone recieving the new classification
		 * @param boneType unsigned int form of the bone type enum, see Univ::BoneType
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool SetBoneTypeC(SkeletonBone* bone, uint32_t boneType);
		/**
		 * @brief Sets bone property type
		 
		 * @param bone Ptr to bone recieving the new classification
		 * @param boneType string form of the bone type enum, see Univ::BoneType
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool SetBoneTypeFromStringC(SkeletonBone* bone, const char* boneStr);
		/**
		 * @brief
		 
		 * @param bone Ptr to bone
		 * @return The unsigned int/uint32_t form of the bone type enum
		 */
		CALUMIANIMATION_API uint32_t GetBoneTypeC(SkeletonBone* bone);
		/**
		 * @brief
		 
		 * @param bone Ptr to bone
		 * @return The string form of the bone type enum
		 */
		CALUMIANIMATION_API const char* GetBoneTypeAsStringC(SkeletonBone* bone);
		/**
		 * @brief Set's the twist properties of this bone
		 
		 * @param bone Ptr to bone to set values for
		 * @param reassign True if the type should be set if not already set as twist type
		 * @param twistDriverIndex 
		 * @param twistDriverWeight 
		 * @param errorMessage Error message container
		 * @return Wether the operation was successful
		 */
		CALUMIANIMATION_API bool SetTwistBonePropertiesC(SkeletonBone* bone, bool reassign, int32_t twistDriverIndex, float twistDriverWeight, Utilities::StringContainer* errorMessage);
		/**
		 * @brief 
		 
		 * @param bone 
		 * @param errorMessage 
		 * @return Index of the bone that dynamically drives this bone, in game, -1 if none exists or is not set
		 */
		CALUMIANIMATION_API int GetTwistBoneDriverIndexC(SkeletonBone* bone, Utilities::StringContainer* errorMessage);
		/**
		 * @brief 
		 
		 * @param bone 
		 * @param errorMessage 
		 * @return Drive parameter of the twist bone interaction, each game may have a different amount of weightedness for this value
		 */
		CALUMIANIMATION_API float GetTwistBoneDriverWeightC(SkeletonBone* bone, Utilities::StringContainer* errorMessage);

		/**
		 * @brief Sets the mirror index for this bone
		 
		 * @param bone Ptr to bone
		 * @param index Index of bone to mirror
		 * @return The index to which this bone is now mirrored
		 */
		CALUMIANIMATION_API int SetMirrorIndexC(SkeletonBone* bone, int index);
		/**
		 * @brief Get this bone's mirrored index
		 
		 * @param bone Ptr to bone to find mirror
		 * @return Index of the mirrored bone, if none exists then returns -1
		 */
		CALUMIANIMATION_API int GetMirrorIndexC(SkeletonBone* bone);
		/**
		 * @brief Creates an exclusive mirror between two bones
		 
		 * @param rig Ptr to rig in which the pair resides
		 * @param index1 bone index
		 * @param index2 bone index
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool CreateBoneMirrorPairC(SkeletonRig* rig, int index1, int index2);
		/**
		 * @brief Resets all of the bones in a given rig to have no mirror
		 
		 * @param rig Ptr to skeleton rig
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool ResetAllBoneMirrorsC(SkeletonRig* rig);
		/**
		 * @brief Scans all bones in the given skeleton rig to verify the mirrors are exclusive or are set to default
		 
		 * @param rig Ptr to rig
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool VerifyExclusiveBoneMirrorsC(SkeletonRig* rig);
		/**
		 * @brief 
		 
		 * @param source Ptr to skeleton rig to check
		 * @return The total number of bones belonging to this rig
		 */
		CALUMIANIMATION_API std::size_t GetSkeletonRigBoneCountC(SkeletonRig* source);
		/**
		 * @brief Animated bones that are not driven dynamically in game
		 
		 * @param source Ptr to rig 
		 * @return The total number of animated bones that driven by animation's data prior to runtime
		 */
		CALUMIANIMATION_API std::size_t GetSkeletonRigAnimatedBoneCountC(SkeletonRig* source);
		/**
		 * @brief 
		 
		 * @param source Ptr to the skeleton to check
		 * @return Name of skeleton rig
		 */
		CALUMIANIMATION_API const char* GetSkeletonRigNameC(SkeletonRig* source);
		/**
		 * @brief Gets the ptr of a skeleton bone
		 
		 * @param source Ptr to rig
		 * @param index Index of the skeleton bone in question
		 * @param errorMessage optional error message container
		 * @return Ptr, if one exists, to the skeleton bone at the given index
		 */
		CALUMIANIMATION_API SkeletonBone* GetSkeletonBoneC(SkeletonRig* source, int index, Utilities::StringContainer* errorMessage);
		/**
		 * @brief 
		 
		 * @param source Ptr to the bone in question
		 * @return C String of the bone's name
		 */
		CALUMIANIMATION_API const char* GetSkeletonBoneNameC(SkeletonBone* source);
		/**
		 * @brief 
		 
		 * @param source Ptr to a skeleton bone
		 * @return The index of the source bone's parent, if none exist, will return -1
		 */
		CALUMIANIMATION_API int GetSkeletonBoneParentIndexC(SkeletonBone* source);
		/**
		 * @brief Returns the rotation of this bone
		 
		 * @param source Ptr to the bone in question
		 * @param global True for rotation relative to rig origin, False for relative to bone's parent
		 * @return Quaternion representation of the rotation
		 */
		CALUMIANIMATION_API const CALUMI::Math::Quaternion* GetSkeletonBoneRotationC(SkeletonBone* source, bool global);
		/**
		 * @brief Returns the position of this bone
		 
		 * @param source Ptr to the bone in question
		 * @param global True for position relative to the rig origin, False for relative to the bone's parent
		 * @return Vector3 (float) representation of the position
		 */
		CALUMIANIMATION_API const CALUMI::Math::Vector3* GetSkeletonBonePositionC(SkeletonBone* source, bool global);
		/**
		 * @brief Checks whether all of the bone names in this rig are unique
		 
		 * @param source Ptr to the skeleton rig in question
		 * @param errorMessage Optional: Ptr to an emptry string container for error messages
		 * @return Whether all names in the rig are unique and valid
		 */
		CALUMIANIMATION_API bool ValidateSkeletonRigNamesC(SkeletonRig* source, Utilities::StringContainer* errorMessage);
		/**
		 * @brief Checks whether all of the bones in this rig have a parent index that is lower than the respective bone's index
		 
		 * @param source Ptr to the skeleton rig in question
		 * @param errorMessage Optional: Ptr to an empty string container for error messages
		 * @return Whether the parent bone indices are valid
		 */
		CALUMIANIMATION_API bool ValidateSkeletonRigParentIndicesC(SkeletonRig* source, Utilities::StringContainer* errorMessage);
	}

	/**
	* @}
	* @}
	*/
}}

#pragma warning(disable: 4661)
_VECTORTEMPLATE(CALUMI::UNIV::SkeletonBone);
_VECTORTEMPLATE(CALUMI::UNIV::IRigPackage*);
#pragma warning(default: 4661)
