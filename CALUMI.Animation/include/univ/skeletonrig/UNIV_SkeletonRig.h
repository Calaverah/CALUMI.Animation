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

	/**
	 * @brief Universal method of storing skeleton limb data
	 * @note Bones are unique and must be created in a parent. To add a bone to a skeleton rig, you must add a
	 * child bone either to the rig's root or to another, already existing, child bone of the rig
	 */
	class CALUMIANIMATION_API SkeletonBone : public ILineage
	{
	private:

		/** @name Initialization*/
		/// @{

		/**
		 */
		explicit SkeletonBone(const ILineage* parent, const char* name);

	public:
		/**
		 *
		 */
		~SkeletonBone() override;

		/**
		 * @brief Static method to remove and delete bone from its parent
		 * @param bone Bone to delete
		 */
		static void DeleteBone(const SkeletonBone*& bone);

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
		/**
		 *
		 * @return Rig that owns this bone and its lineage
		 */
		[[nodiscard]] const SkeletonRig& parentRig() const;

		/**
		 *
		 * @param name
		 * @param transform
		 * @return
		 */
		[[nodiscard]] SkeletonBone* addChildBone(const char* name, const Math::Transform& transform) const;
		/**
		 *
		 * @param name
		 * @param position
		 * @param rotation
		 * @return
		 */
		[[nodiscard]] SkeletonBone* addChildBone(const char* name, const Math::Vector3& position, const Math::Quaternion& rotation) const;
		/**
		 *
		 * @param name
		 * @param includeDescendents
		 * @return
		 */
		[[nodiscard]] SkeletonBone* childBone(const char* name, bool includeDescendents = true) const;
		/**
		 * @bried index method of getting a child bone, mostly useful during for loops
		 * @param index Index of the direct child
		 * @return
		 */
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
		[[nodiscard]] unsigned int boneTypeCount(BoneTypeProperty::BoneType type) const;
		/// @}

	public:

		/** @name Transform*/
		/// @{

		/**
		 * @brief The transform for this object. Expressed position and rotation
		 */
		[[nodiscard]] const Math::Transform& localTransform() const;
		/**
		 *
		 * @return The global form of this object's transform
		 */
		[[nodiscard]] Math::Transform globalTransform() const;
		/**
		 *
		 * @param position Vector3
		 * @param rotation Quaternion
		 */
		void setLocalTransform(const Math::Vector3& position, const Math::Quaternion& rotation) const;
		/**
		 *
		 * @param transform Transform
		 */
		void setLocalTransform(const Math::Transform& transform) const;

		/// @}

	public:

		/** @name Identification*/
		/// @{

		/**
		 * @brief Identifier for this bone
		 * @return name in a string container
		 */
		[[nodiscard]] const char* name() const;
		/**
		 *
		 * @param name
		 * @return
		 */
		[[nodiscard]] bool isValidName(const char* name) const;
		/**
		 *
		 * @param name
		 * @return
		 */
		bool setName(const char* name) const;
		/**
		 *
		 * @return
		 */
		[[nodiscard]] bool isRoot() const;

		/// @}
		/** @name Properties*/
		/// @{

		/**
		 * @brief Sets the unique property of this bone
		 * @param boneType Type to set
		 * @param resetExisting If this bone is already set to the given type, will reset to this type's default values
		 * @return Whether the operation was a success
		 */
		[[nodiscard]] bool setBoneTypeProperty(BoneTypeProperty::BoneType boneType, bool resetExisting = false) const;
		/**
		 * @brief Gets the unique bone property
		 * @return Ptr to the bone property type and its values
		 */
		[[nodiscard]] const BoneTypeProperty* boneTypeProperty() const;
		/**
		 * @brief Reset the bone properties values
		 * @param boneType Type of bone properties to set this bone to
		 * @return Whether the operation was a success
		 */
		bool resetBoneTypeProperty(BoneTypeProperty::BoneType boneType = BoneTypeProperty::BoneType::Default) const; // NOLINT(*-use-nodiscard)

	public:
		/**
		 * @warning Root bones belong directly to a rig and cannot be set to a new parent
		 * @param newParent
		 * @return
		 */
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
		 * @deprecated
		 * @brief Serialization
		 * @param indents Spaces for formatting
		 * @return The strContainer of the serialized struct
		 */
		[[deprecated]] [[nodiscard]] Utilities::StringContainer toJSON(uint64_t indents) const;

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

		/**
		 */
		SkeletonRig();
		/**
		 *
		 * @param _rigName
		 */
		explicit SkeletonRig(const char* _rigName);
		/**
		 *
		 * @param _rigName
		 */
		explicit SkeletonRig(const Utilities::StringContainer& _rigName);
		/**
		 *
		 * @param input
		 */
		SkeletonRig(const SkeletonRig& input);
		/**
		 *
		 */
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
		[[nodiscard]] unsigned int boneTypeCount(BoneTypeProperty::BoneType type) const;
		/**
		 * @brief Returns the total amount of bones in this rig
		 */
		[[nodiscard]] unsigned int boneCount() const;
		/**
		 * @return
		 */
		[[nodiscard]] SkeletonBone* root() const;
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
		 * @brief It is unlikely to even need this many bones, no supported game allows this many bones to be added
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
extern  "C"
{
	/**
	 * @brief Creates a brand new Skeleton Rig
	 * @param name Identifier for the rig, best if unique
	 * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteSkeletonRigC
	 * @return A dynamically allocated, universal, skeleton rig ptr
	 */
	CALUMIANIMATION_API CALUMI::UNIV::SkeletonRig* CreateSkeletonRigC(const char* name);
	/**
	 * @brief Deletes the dynamically allocated rig that was created, this is the preferred way to delete a rig.
	 * @param ptrRef Reference to ptr of the rig to delete. This allows the ptr to safely be set to nullptr when
	 * finished.
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
	 */
	CALUMIANIMATION_API int DeleteSkeletonRigC(const CALUMI::UNIV::SkeletonRig** ptrRef);
	/**
	 * @brief Proper method to reach the root bone of a rig
	 * @param rig
	 * @note The root bone is a permanent child to the rig and cannot be deleted, however its data can be affected the same as any other bone
	 * @return Root bone that is tied to a rig
	 */
	CALUMIANIMATION_API CALUMI::UNIV::SkeletonBone* GetSkeletonRigRootC(const CALUMI::UNIV::SkeletonRig* rig);
	/**
	 * @brief Add a child bone to a given parent bone, using simple component input
	 * @param parentBone Parent of the created bone
	 * @param posX X component of the position
	 * @param posY Y component of the position
	 * @param posZ Z component of the position
	 * @param rotX X component of the rotation quaternion
	 * @param rotY Y component of the rotation quaternion
	 * @param rotZ Z component of the rotation quaternion
	 * @param rotW W component of the rotation quaternion
	 * @param boneName Unique name (within the rig) of the bone
	 * @note The object is under ownership of the given parent bone and does not need to be explicitly deleted
	 * @return Ptr to the newly created bone or nullptr if unsuccessful
	 */
	CALUMIANIMATION_API CALUMI::UNIV::SkeletonBone* AddChildBoneC(const CALUMI::UNIV::SkeletonBone* parentBone,
	                                                              float posX, float posY, float posZ,
	                                                              float rotX, float rotY, float rotZ, float rotW,
	                                                              const char* boneName);
	/**
	 * @brief Add a child bone to a given parent bone, using euler based rotation
	 * @param parentBone Parent of the created bone
	 * @param posX X component of the position
	 * @param posY Y component of the position
	 * @param posZ Z component of the position
	 * @param rad1 First rotational operation, in radians
	 * @param rad2 Second rotational operation, in radians
	 * @param rad3 Third rotational operation, in radians
	 * @param eulerOrder Integer representation of the @ref CALUMI::Math::EulerDefinition::EulerOrder "Euler Order"
	 * of choice
	 * @param boneName Unique name (within the rig) of the bone
	 * @note The object is under ownership of the given parent bone and does not need to be explicitly deleted
	 * @return Ptr to the newly created bone or nullptr if unsuccessful
	 */
	CALUMIANIMATION_API CALUMI::UNIV::SkeletonBone* AddChildBoneWithEulerC(const CALUMI::UNIV::SkeletonBone* parentBone,
	                                                                       float posX, float posY, float posZ,
	                                                                       float rad1, float rad2, float rad3,
	                                                                       int eulerOrder,
	                                                                       const char* boneName);
	/**
	 * @brief Add a child bone to a given parent bone, using a reference to a Vector3 or Quaternion
	 * @param parentBone Parent of the created bone
	 * @param position
	 * @param rotation
	 * @param boneName Unique name (within the rig) of the bone
	 * @note The object is under ownership of the given parent bone and does not need to be explicitly deleted
	 * @return Ptr to the newly created bone or nullptr if unsuccessful
	 */
	CALUMIANIMATION_API CALUMI::UNIV::SkeletonBone* AddChildBoneWithVectorC(const CALUMI::UNIV::SkeletonBone* parentBone,
	                                                                        const CALUMI::Math::Vector3* position,
	                                                                        const CALUMI::Math::Quaternion* rotation,
	                                                                        const char* boneName);

	/**
	 * @brief Sets bone property type
	 * @param bone Ptr to bone receiving the new classification
	 * @param boneType unsigned int form of the bone type enum, see @ref CALUMI::UNIV::BoneTypeProperty::BoneType
	 * "Bone Type"
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Bone type could not be set
	 */
	CALUMIANIMATION_API int SetBoneTypeC(const CALUMI::UNIV::SkeletonBone* bone, uint32_t boneType);
	/**
	 *
	 * @param ptrRef Reference to ptr of the bone to delete, this allows safe handling and setting the ptr to
	 * nullptr on completion.
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
	 */
	CALUMIANIMATION_API int DeleteBoneC(const CALUMI::UNIV::SkeletonBone** ptrRef);
	/**
	 * @param rig Skeleton rig in which this bone resides
	 * @param oldName Name of the bone that will be renamed
	 * @param newName New name of the bone (must be unique and valid)
	 * @return -1 = invalid rig\n 0 = Success\n 1 = Bone not found\n 2 = New name is invalid\n 3 = New name is same
	 * as old\n 4 = Name could not be set
	 */
	CALUMIANIMATION_API int RenameBoneC(const CALUMI::UNIV::SkeletonRig* rig, const char* oldName, const char* newName);
	/**
	 * @brief Sets bone property type
	 * @param bone Ptr to bone receiving the new classification
	 * @param boneType string form of the bone type enum, see @ref CALUMI::UNIV::BoneTypeProperty::BoneType "Bone Type"
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = The bone could not re-assigned
	 */
	CALUMIANIMATION_API int SetBoneTypeFromStringC(const CALUMI::UNIV::SkeletonBone* bone, const char* boneType);
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
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = The bone was not reassigned and
	 * remains unaffected\n 2 = Reassign bone type attempted but failed\n 3 = The operation could not be completed
	 */
	CALUMIANIMATION_API int SetTwistBonePropertiesC(const CALUMI::UNIV::SkeletonBone* bone, bool reassign, const
		char* twistDriver, float twistDriverWeight);
	/**
	 * @brief
	 * @param bone
	 * @return Name of the bone that dynamically drives this bone in game, empty string if none exists or is not set
	 */
	CALUMIANIMATION_API const char* GetTwistBoneDriverC(const CALUMI::UNIV::SkeletonBone* bone);
	/**
	 * @brief
	 * @param bone
	 * @return Drive parameter of the twist bone interaction, each game may have a different amount of
	 * weightedness for this value. Will return NaN if unsuccessful
	 */
	CALUMIANIMATION_API float GetTwistBoneDriverWeightC(const CALUMI::UNIV::SkeletonBone* bone);
	/**
	 * @brief
	 * @param source Ptr to skeleton rig to check
	 * @return The total number of bones belonging to this rig, which always has a root
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
	 * @brief Count the number of bones that are of a specific type. Currently, the UNIV system only uses two
	 * basic types so this convention will follow closely to the Animated and Non-Animated type counts
	 * @param source
	 * @param typeAsInteger Int form of @ref CALUMI::UNIV::BoneTypeProperty::BoneType "Bone Type"
	 * @return Count, or 0 if no rig or bones are found
	 */
	CALUMIANIMATION_API unsigned int GetSkeletonRigBoneTypeCountC(const CALUMI::UNIV::SkeletonRig* source,
	                                                              uint32_t typeAsInteger);
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
	 * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteQuaternionC
	 * @return Quaternion representation of the bone's global rotation
	 */
	CALUMIANIMATION_API const CALUMI::Math::Quaternion* GetGlobalSkeletonBoneRotationC(const CALUMI::UNIV::SkeletonBone* source);
	/**
	 * @brief Returns a heap allocated quaternion of this bone's rotation relative to its parent, if one exists
	 * @param source
	 * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteQuaternionC
	 * @return Quaternion representation of the bone's rotation relative to its parent
	 */
	CALUMIANIMATION_API const CALUMI::Math::Quaternion* GetLocalSkeletonBoneRotationC(const CALUMI::UNIV::SkeletonBone* source);
	/**
	 * @brief Returns the position of this bone, in global coordinates
	 * @param source Ptr to the bone in question
	 * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteVector3C
	 * @return Vector3 (float) representation of the bone's global position
	 */
	CALUMIANIMATION_API const CALUMI::Math::Vector3* GetGlobalSkeletonBonePositionC(const CALUMI::UNIV::SkeletonBone* source);
	/**
	 * @brief Returns a heap allocated vector3 of this bone's position relative to its parent, if one exists
	 * @param source
	 * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteVector3C
	 * @return Nullptr if no bone is found, global position if no parent is found
	 */
	CALUMIANIMATION_API const CALUMI::Math::Vector3* GetLocalSkeletonBonePositionC(const CALUMI::UNIV::SkeletonBone* source);
	/**
	 * @brief Simplified method for setting a bone's global position.
	 * @details Works by defining a local position based on the global input relative to the bone's current parent
	 * @param bone
	 * @param x
	 * @param y
	 * @param z
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Parent could not be found, use
	 * local setter instead
	 */
	CALUMIANIMATION_API int SetGlobalSkeletonBonePositionC(const CALUMI::UNIV::SkeletonBone* bone, float x, float y, float z);
	/**
	 * @brief Simplified method for setting a bone's local position.
	 * @param bone
	 * @param x
	 * @param y
	 * @param z
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
	 */
	CALUMIANIMATION_API int SetLocalSkeletonBonePositionC(const CALUMI::UNIV::SkeletonBone* bone, float x, float y, float z);
	/**
	 * @brief Simplified method for setting a bone's global rotation.
	 * @param bone
	 * @param x
	 * @param y
	 * @param z
	 * @param w
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Parent could not be found, use
	 * local setter instead
	 */
	CALUMIANIMATION_API int SetGlobalSkeletonBoneRotationC(const CALUMI::UNIV::SkeletonBone* bone,
	                                                       float x, float y, float z, float w);
	/**
	 * @brief Simplified method for setting a bone's local rotation.
	 * @param bone
	 * @param x
	 * @param y
	 * @param z
	 * @param w
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
	 */
	CALUMIANIMATION_API int SetLocalSkeletonBoneRotationC(const CALUMI::UNIV::SkeletonBone* bone,
														   float x, float y, float z, float w);
	/**
	 * @brief Simplified method for setting a bone's global rotation with an euler definition.
	 * @param bone
	 * @param rot1
	 * @param rot2
	 * @param rot3
	 * @param eulerOrder Integer form of @ref CALUMI::Math::EulerDefinition::EulerOrder "Euler Order"
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Parent could not be found, use local
	 * setter instead
	 */
	CALUMIANIMATION_API int SetGlobalSkeletonBoneRotationWithEulerC(const CALUMI::UNIV::SkeletonBone* bone,
	                                                                float rot1, float rot2, float rot3, int eulerOrder);
	/**
	 * @brief Simplified method for setting a bone's local rotation with an euler definition.
	 * @param bone
	 * @param rot1
	 * @param rot2
	 * @param rot3
	 * @param eulerOrder Integer form of @ref CALUMI::Math::EulerDefinition::EulerOrder "Euler Order"
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
	 */
	CALUMIANIMATION_API int SetLocalSkeletonBoneRotationWithEulerC(const CALUMI::UNIV::SkeletonBone* bone,
	                                                               float rot1, float rot2, float rot3, int eulerOrder);
	/**
	 *
	 * @param bone
	 * @param posX
	 * @param posY
	 * @param posZ
	 * @param rotX
	 * @param rotY
	 * @param rotZ
	 * @param rotW
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Parent could not be found, use local
	 * setter instead
	 */
	CALUMIANIMATION_API int SetGlobalSkeletonBoneTransformC(const CALUMI::UNIV::SkeletonBone* bone,
	                                                        float posX, float posY, float posZ,
	                                                        float rotX, float rotY, float rotZ, float rotW);
	/**
	 *
	 * @param bone
	 * @param posX
	 * @param posY
	 * @param posZ
	 * @param rotX
	 * @param rotY
	 * @param rotZ
	 * @param rotW
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
	 */
	CALUMIANIMATION_API int SetLocalSkeletonBoneTransformC(const CALUMI::UNIV::SkeletonBone* bone,
	                                                       float posX, float posY, float posZ,
	                                                       float rotX, float rotY, float rotZ, float rotW);
	/**
	 *
	 * @param bone
	 * @param posX
	 * @param posY
	 * @param posZ
	 * @param rot1
	 * @param rot2
	 * @param rot3
	 * @param eulerOrder Integer form of @ref CALUMI::Math::EulerDefinition::EulerOrder "Euler Order"
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Parent could not be found, use local
	 * setter instead
	 */
	CALUMIANIMATION_API int SetGlobalSkeletonBoneTransformWithEulerC(const CALUMI::UNIV::SkeletonBone* bone,
	                                                                 float posX, float posY, float posZ,
	                                                                 float rot1, float rot2, float rot3, int eulerOrder);
	/**
	 *
	 * @param bone
	 * @param posX
	 * @param posY
	 * @param posZ
	 * @param rot1
	 * @param rot2
	 * @param rot3
	 * @param eulerOrder Integer form of @ref CALUMI::Math::EulerDefinition::EulerOrder "Euler Order"
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
	 */
	CALUMIANIMATION_API int SetLocalSkeletonBoneTransformWithEulerC(const CALUMI::UNIV::SkeletonBone* bone,
	                                                                float posX, float posY, float posZ,
	                                                                float rot1, float rot2, float rot3, int eulerOrder);
	/**
	 *
	 * @param bone
	 * @param boneParent
	 * @warning Bone parent cannot be set on root
	 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Parent could not be set
	 */
	CALUMIANIMATION_API int SetSkeletonBoneParentC(const CALUMI::UNIV::SkeletonBone* bone,
	                                               const CALUMI::UNIV::SkeletonBone* boneParent);
}

/**
 * @}
 * @}
 * @}
 */

