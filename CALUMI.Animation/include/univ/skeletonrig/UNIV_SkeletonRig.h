//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "UNIV_BoneTypes.h"
#include "math/Math.h"
#include "UNIV_IRigPackage.h"
#include <cstdint>

#include "interfaces/ILineage.h"

namespace CALUMI::UNIV{

	class SkeletonRig;

	/**
	 * @brief Universal method of storing skeleton limb data
	 */
	class CALUMIANIMATION_API SkeletonBone : public ILineage
	{
	public:

		/** @name Initialization*/
		/// @{

		explicit SkeletonBone(const ILineage& parent);
		SkeletonBone(const SkeletonBone& other);
		~SkeletonBone() override;

		/// @}
		/** @name Operators*/
		/// @{
	public:
		SkeletonBone& operator=(const SkeletonBone& other);

		/// @}
		/// @name ILineage
		/// @{

		/**
		 * @brief Since the rig itself cannot be a child of another bone or rig,
		 * this will return nullptr, signaling that it is the root of the structure
		 */
		[[nodiscard]] const ILineage* parent() const override;
		[[nodiscard]] const SkeletonRig& parentRig() const;
		/// @}

	public:

		/** @name Transform*/
		/// @{

		/**
		 * @brief #Rotation in relation to the origin of the skeleton rig
		 * @return #Rotation in the form of a Quaternion
		 */
        [[nodiscard]] const Math::Quaternion& globalRotation() const;
		/**
		 * @brief Position in relation to the origin of the skeleton rig
		 * @return Position in the form of a Vector3 (float base)
		 */
        [[nodiscard]] const Math::Vector3& globalPosition() const;

	private:
		/**
		 * @brief Method for skeleton rig, or other privileged classes, to set the transform members of this bone
		 * @param global Relative to rig origin
		 */
        void setRotation(const Math::Quaternion& global) const;
		/**
		 * @brief Method for skeleton rig, or other privileged classes, to set the transform members of this bone
		 * @param global Relative to rig origin
		 */
        void setPosition(const Math::Vector3& global) const;

		/// @}

	public:

		/** @name Identification*/
		/// @{

		/**
		 * @brief Identifier for this bone
         * @return name in a string container
		 */
        [[nodiscard]] const char* name() const;

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
		/**
		 * @brief Sets the parent for this bone, however, setting the parent directly on the bone will not affect its global position and rotation
		 * @param name Name of parent
		 * @details If no name is set, or is left blank, the parentage will default to the root object of the rig during compilation
		 */
        void setParentBone(const char* name) const;
		/**
		 * @brief 
		 * @return The name of this bone's parent, if set
		 */
        [[nodiscard]] const char* parentBone() const;

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

	VECTORDECF(SkeletonBoneVector, SkeletonBone, friend class SkeletonRig;)

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
		/// @name Operators
		/// @{

	public:
		SkeletonRig& operator=(const SkeletonRig& other);

		/// @}
		/// @name ILineage
		/// @{

		/**
		 * @brief Since the rig itself cannot be a child of another bone or rig,
		 * this will return nullptr, signaling that it is the root of the structure
		 */
		[[nodiscard]] const ILineage* parent() const override;
		/// @}

		//TODO: Consider flags for parental/relative/global during shifting operations

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
		 * @brief The entry list for this rig's bones
		 * @return Vector container
		 */
		[[nodiscard]] const SkeletonBoneVector& boneEntries() const;
		/**
		 * @brief Returns a pointer to a bone if one exists under the given name
		 * @param boneName 
		 * @return Nullptr if none exists
		 */
		SkeletonBone* bone(const char* boneName) const;
		/**
		 * @brief
		 * @return Reference to this rig's package manager
		 */
		[[nodiscard]] RigPackageManager& getPackageManager() const;

		/// @}
		/** @name Validation*/
		/// @{

		/**
		 * @brief Checks whether all bone names in this rig are unique
		 * @return Whether all names in the rig are unique and valid
		 */
		[[nodiscard]] bool validateNames() const;
		/**
		 * @brief Checks if each bone mirror's another exclusively, or not at all
		 * @return Whether each bone has an exclusive mirror, or no mirror at all
		 */
		[[nodiscard]] bool verifyExclusiveBoneMirrors() const;
		/**
		 * @brief Returns the name of the bone's mirror
		 * @param boneName 
		 * @return empty if no mirror is set, or bone doesn't exist
		 */
		const char* getBoneMirrorName(const char* boneName) const;

		/// @}

	public:
		/** @name Bone Entries */
		/// @{

		/**
		 * @brief The preferred way to add bones to a universal rig definition
		 * @param rotation Rotational entry as Quaternion
		 * @param position Position as Vector3 (float)
		 * @param boneName Must be unique string
		 * @param parentName If a parent is not found, the parent will default to the root bone
		 * @param relativeToParent If true, and a parent transform is found within the rig, the position and rotation will be treated as relative to this bone's parent. If false, or a parent isn't found, the values will be accepted as global.
		 * @return Ptr to the newly added bone, or nullptr if the operation was unsuccessful
		 */
		SkeletonBone* addBoneToRig(const Math::Quaternion& rotation, const Math::Vector3& position, const char* boneName, const char* parentName, bool relativeToParent) const;
		/**
		 * @brief Flags a bone to act as the root bone of this rig, moving it to the top of the bone entry vector.
		 * The same as setting this bone's index to 0.
		 * @param boneName
		 * @param keepRelative Will keep the relative position/rotation to its current parent, and assign it as global
		 * @param keepChildrenRelative Will keep the relative positions of its children
		 * @return Whether the bone was successfully set as the root
		 */
		bool setRoot(const char* boneName, bool keepRelative = true, bool keepChildrenRelative = true) const;
		/**
		 * @param boneName The bone to reparent
		 * @param parentName The new parent, if not found, will cancel operation
		 * @param keepRelative If true, the relative position/rotation of this bone to its current parent will be preserved under its new parent
		 * @param keepChildrenRelative
		 * @return Whether the operation is successful
		 */
		bool setBoneParent(const char* boneName, const char* parentName, bool keepRelative = true, bool keepChildrenRelative = true) const;
		/**
		 * @brief Renames a bone in the rig, if one exists
		 * @param oldBoneName The name of the bone to rename
		 * @param newBoneName The new name for the bone
		 * @return Whether the operation was a success
		 */
		bool renameBone(const char* oldBoneName, const char* newBoneName) const;
		/**
		 * @brief Pairs two bones to mirror each other
		 * @param bone1 Index of the first bone
		 * @param bone2 Index of the second bone
		 * @return Whether the operation was a success
		 */
		bool createBoneMirrorPair(const char* bone1, const char* bone2) const;
		/**
		 * @brief Resets all bones in the rig to have no mirrored entry
		 * @return Whether the operation was a success
		 */
		bool resetAllBoneMirrors() const; // NOLINT(*-use-nodiscard)
		/**
		 * @brief Removes a bone from the rig
		 * @param boneName Name of bone
		 * @return Whether a bone with this name was successfully removed
		 */
		bool removeBone(const char* boneName) const;
		/**
		 * @brief Returns the rotation of this bone
		 * @param boneName If not found, identity quaternion will be returned
		 * @param relativeToParent if true and a parent is found, the rotation will be relative to the parent
		 * @return relative rotation as quaternion, if no parent is found, global rotation will be returned
		 */
		Math::Quaternion boneRotation(const char* boneName, bool relativeToParent) const;
		/**
		 * @brief Returns the position of this bone
		 * @param boneName If not found, Vector3.Zero will be returned
		 * @param relativeToParent if true and a parent is found, the position will be relative to the parent
		 * @return relative position as vector3 (float), if no parent is found, global position will be returned 
		 */
		Math::Vector3 bonePosition(const char* boneName, bool relativeToParent) const;
		/**
		 * @brief Attempts to move a bone, if it exists, to a new index
		 * @param boneName 
		 * @param index The destination index, if it is out of bounds of the vector, then it will default to the nearest acceptable index. For example: -1 becomes 0, if the array size is 5 then an input of 5 will become 4, and so on
		 * @return The index, if any, that the bone was moved to. If no bone was found, or there was an error, will return -1
		 */
		int setBoneIndex(const char* boneName, int index) const;
		/**
		 * @brief Returns the index of the given bone name, if not found, returns -1
		 * @returns -1 if none found
		 */
		int findBone(const char* name) const;
		/**
		 * @brief Returns the index of the given bone's parent, if not found, returns -1
		 * @returns -1 if none found
		 */
		int findBoneParent(const char* name) const;

		/// @}

	public:
		/** @name Bone Indices */
		/// @{

		/**
		 * @brief Returns the bones that are animation driven, rather than dynamically controlled
		 * @return The number of animated bones (ie not in-game twist)
		 */
		[[nodiscard]] uint64_t animatedBoneCount() const;
		/**
		 * @brief Returns the total amount of bones, whether they are driven by curves or dynamically in game
		 * @return The total number of bones on this rig
		 */
		[[nodiscard]] uint64_t boneCount() const;

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
		/// @private
		void shiftChildren(const SkeletonBone& bone, const Math::Vector3& posOffset, const Math::Quaternion& rotOffset) const;
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
		CALUMIANIMATION_API bool DeleteSkeletonRigC(const SkeletonRig* ptr);
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
		CALUMIANIMATION_API SkeletonBone* AddBoneToSkeletonRigC(
			const SkeletonRig* rig,
			float rotationX, float rotationY, float rotationZ, float rotationW,
			float positionX, float positionY, float positionZ,
			const char* boneName,
			const char* parentName,
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
		 * @param parentName Name of this bone's parent
		 * @param usingLocalValues Whether the given transform is relative to the origin (false) or the bone's parent (true)
		 * @param errorMessage Ptr to a string container, if not null a status message is given here
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool AddBoneToSkeletonRigWithEulerC(
			const SkeletonRig* rig,
			float rotationX, float rotationY, float rotationZ, uint8_t order,
			float positionX, float positionY, float positionZ,
			const char* boneName,
			const char* parentName,
			bool usingLocalValues,
			Utilities::StringContainer* errorMessage
		);

		/**
		 * @brief Sets bone property type
		 
		 * @param bone Ptr to bone receiving the new classification
		 * @param boneType unsigned int form of the bone type enum, see Univ::BoneType
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool SetBoneTypeC(const SkeletonBone* bone, uint32_t boneType);
		/**
		 * @brief Removes a bone of the given name, from the rig
		 * @param rig 
		 * @param boneName 
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool RemoveBoneC(const SkeletonRig* rig, const char* boneName);
		/**
		 * @brief Renames a bone, if one exists, on the given rig
		 * @param rig 
		 * @param oldName 
		 * @param newName Must be unique, and not empty
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool RenameBoneC(const SkeletonRig* rig, const char* oldName, const char* newName);
		/**
		 * @brief Normally the root of the rig is set to the 0th index of the bone entry vector, setting this will override that selection, if desired
		 * @param rig 
		 * @param boneName Bone to mark as the root override
		 * @param keepRelative
		 * @param keepChildrenRelative
		 * @return 
		 */
		CALUMIANIMATION_API bool SetRootC(const SkeletonRig* rig, const char* boneName, bool keepRelative, bool keepChildrenRelative);
		/**
		 * @brief Sets the given bone to a new index, if one exists
		 * @param rig The rig that holds the vector containing the bone
		 * @param boneName The bone to move
		 * @param newIndex The index will be clamped to the bounds of the vector
		 * @return The index the bone is now set to, if the operation failed, -1 is returned
		 */
		CALUMIANIMATION_API int SetBoneIndexC(const SkeletonRig* rig, const char* boneName, int newIndex);
		/**
		 * @brief Sets bone property type
		 
		 * @param bone Ptr to bone receiving the new classification
		 * @param boneType string form of the bone type enum, see Univ::BoneType
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool SetBoneTypeFromStringC(const SkeletonBone* bone, const char* boneType);
		/**
		 * @brief
		 
		 * @param bone Ptr to bone
		 * @return The unsigned int/uint32_t form of the bone type enum
		 */
		CALUMIANIMATION_API uint32_t GetBoneTypeC(const SkeletonBone* bone);
		/**
		 * @brief
		 
		 * @param bone Ptr to bone
		 * @return The string form of the bone type enum
		 */
		CALUMIANIMATION_API const char* GetBoneTypeAsStringC(const SkeletonBone* bone);
		/**
		 * @brief Set's the twist properties of this bone
		 
		 * @param bone Ptr to bone to set values for
		 * @param reassign True if the type should be set if not already set as twist type
		 * @param twistDriver
		 * @param twistDriverWeight 
		 * @param errorMessage Error message container
		 * @return Whether the operation was successful
		 */
		CALUMIANIMATION_API bool SetTwistBonePropertiesC(const SkeletonBone* bone, bool reassign, const char* twistDriver, float twistDriverWeight, Utilities::StringContainer* errorMessage);
		/**
		 * @brief 
		 
		 * @param bone 
		 * @param errorMessage
		 * @return Index of the bone that dynamically drives this bone in game, empty string if none exists or is not set
		 */
		CALUMIANIMATION_API const char* GetTwistBoneDriverC(const SkeletonBone* bone, Utilities::StringContainer* errorMessage);
		/**
		 * @brief 
		 
		 * @param bone 
		 * @param errorMessage
		 * @return Drive parameter of the twist bone interaction, each game may have a different amount of weightedness for this value
		 */
		CALUMIANIMATION_API float GetTwistBoneDriverWeightC(const SkeletonBone* bone, Utilities::StringContainer* errorMessage);

		
		/**
		 * @brief Creates an exclusive mirror between two bones
		 
		 * @param rig Ptr to rig in which the pair resides
		 * @param bone1 bone name
		 * @param bone2 bone name
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool CreateBoneMirrorPairC(const SkeletonRig* rig, const char* bone1, const char* bone2);
		/**
		 * @brief Unsets the given bone from having a mirrored bone
		 * @param boneName
		 * @param rig
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool ResetBoneMirrorC(const SkeletonRig* rig, const char* boneName);
		/**
		 * @brief Resets all bones in a given rig to have no mirror
		 
		 * @param rig Ptr to skeleton rig
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool ResetAllBoneMirrorsC(const SkeletonRig* rig);
		/**
		 * @brief Scans all bones in the given skeleton rig to verify the mirrors are exclusive or are set to default
		 
		 * @param rig Ptr to rig
		 * @return Whether the operation was a success
		 */
		CALUMIANIMATION_API bool VerifyExclusiveBoneMirrorsC(const SkeletonRig* rig);
		/**
		 * @brief 
		 
		 * @param source Ptr to skeleton rig to check
		 * @return The total number of bones belonging to this rig
		 */
		CALUMIANIMATION_API uint64_t GetSkeletonRigBoneCountC(const SkeletonRig* source);
		/**
		 * @brief Animated bones that are not driven dynamically in game
		 
		 * @param source Ptr to rig 
		 * @return The total number of animated bones that driven by animation's data prior to runtime
		 */
		CALUMIANIMATION_API uint64_t GetSkeletonRigAnimatedBoneCountC(const SkeletonRig* source);
		/**
		 * @brief 
		 
		 * @param source Ptr to the skeleton to check
		 * @return Name of skeleton rig
		 */
		CALUMIANIMATION_API const char* GetSkeletonRigNameC(const SkeletonRig* source);
		/**
		 * @brief Gets the ptr of a skeleton bone
		 
		 * @param source Ptr to rig
		 * @param boneName
		 * @return Ptr, if one exists, to the skeleton bone at the given index
		 */
		CALUMIANIMATION_API SkeletonBone* GetSkeletonBoneC(const SkeletonRig* source, const char* boneName);
		/**
		 * @brief 
		 
		 * @param source Ptr to the bone in question
		 * @return C String of the bone's name
		 */
		CALUMIANIMATION_API const char* GetSkeletonBoneNameC(const SkeletonBone* source);
		/**
		 * @brief 
		 
		 * @param source Ptr to a skeleton bone
		 * @return The name of the source bone's parent, if none exist, will return as empty
		 */
		CALUMIANIMATION_API const char* GetSkeletonBoneParentC(const SkeletonBone* source);
		/**
		 * @brief Returns the rotation of this bone, in global coordinates
		 
		 * @param source Ptr to the bone in question
		 * @return Quaternion representation of the rotation
		 */
		CALUMIANIMATION_API const Math::Quaternion* GetSkeletonBoneRotationC(const SkeletonBone* source);
		/**
		 * @brief Returns a heap allocated quaternion of this bone's rotation relative to its parent, if one exists
		 * @param rig 
		 * @param boneName 
		 * @return Nullptr if no bone is found, global rotation if no parent is found
		 */
		CALUMIANIMATION_API Math::Quaternion* GetRelativeSkeletonBoneRotationC(const SkeletonRig* rig, const char* boneName);
		/**
		 * @brief Returns the position of this bone, in global coordinates		 
		 * @param source Ptr to the bone in question
		 * @return Vector3 (float) representation of the position
		 */
		CALUMIANIMATION_API const Math::Vector3* GetSkeletonBonePositionC(const SkeletonBone* source);
		/**
		 * @brief * @brief Returns a heap allocated vector3 of this bone's position relative to its parent, if one exists
		 * @param rig 
		 * @param boneName 
		 * @return Nullptr if no bone is found, global position if no parent is found
		 */
		CALUMIANIMATION_API Math::Vector3* GetRelativeSkeletonBonePositionC(const SkeletonRig* rig, const char* boneName);
		/**
		 * @brief Checks whether all bone names in this rig are unique
		 * @param source Ptr to the skeleton rig in question
		 * @return Whether all names in the rig are unique and valid
		 */
		CALUMIANIMATION_API bool ValidateSkeletonRigNamesC(const SkeletonRig* source);
		/**
		 * @brief Returns the index of the bone in the rig's bone entry vector
		 * @param rig 
		 * @param boneName 
		 * @return -1 if no bone is found
		 */
		CALUMIANIMATION_API int FindBoneInRigC(const SkeletonRig* rig, const char* boneName);
		/**
		 * @brief Returns the index of the given bone's parent
		 * @param rig 
		 * @param boneName 
		 * @return -1 if no bone, or parent bone, is found
		 */
		CALUMIANIMATION_API int FindBoneParentInRigC(const SkeletonRig* rig, const char* boneName);
	}

	/**
	* @}
	* @}
	*/
}
