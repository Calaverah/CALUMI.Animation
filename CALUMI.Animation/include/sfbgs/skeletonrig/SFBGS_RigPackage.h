//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Common.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"

inline constexpr int SFBGSMAPSIZE = 157;

namespace CALUMI::SFBGS
{
	/**
	 * @relates SFBGS_RigPackage
	 */
	inline constexpr auto SFBGS_RIG_PACKAGE = "SFBGS_RIG_PACKAGE";

	/**
	 * @brief Simple pair of values used to compress translations on Starfield animations
	 */
	struct CALUMIANIMATION_API PrecisionSet{
		/// @name Constructors
	/// @{
	private:
		PrecisionSet();
	public:
		/**
		 * @brief Creates a precision set with the given values
		 * @details Will automatically sort the two values for high and low precision
		 * @param value1
		 * @param value2
		 */
		PrecisionSet(float value1, float value2);
		PrecisionSet(const PrecisionSet& other);
		~PrecisionSet();
		/// @}
		/// @name Operators
		/// @{
		PrecisionSet& operator=(const PrecisionSet& other);
		bool operator==(const PrecisionSet& other) const;
		bool operator!=(const PrecisionSet& other) const;
		/// @}
		/// @name Data
		/// @{
		[[nodiscard]] float high() const;
		[[nodiscard]] float low() const;
		[[nodiscard]] const char* precisionType() const;
		/// @}

		/// @name Typical Values
		/// @{
		/**
		 * @brief Default Values High: 1/4000f and Low: 1/32f
		 * @return
		 */
		static PrecisionSet DefaultPrecision();
		/**
			 * @brief 1st Person Values High: 1/16000f and Low: 1/128f
			 * @return
			 */
		static PrecisionSet FirstPersonPrecision();
		/**
			 * @brief Ship Values High: 1/500f and Low: 1/4f
			 * @return
			 */
		static PrecisionSet ShipPrecision();
		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};

	/// @addtogroup rig_packages
	/// @{
	/**
		 * @brief
		 */
	struct CALUMIANIMATION_API SFBGS_RigPackage : UNIV::IRigPackage
	{
	public:
		/// @name Bone Mapping
		/// @{

		/**
			 * @brief Keys used when "mapping" certain skeletal bones
			 * @details The full implication of these mapping tags are not yet realized, but it is thought to affect the character's movement in zero-g, syncing systems, and more
			 */
		enum class BoneMapKey : uint8_t
		{
			Root = 0, ///< Root
			AnimObjectA = 1, ///< AnimObject Identifier
			AnimObjectB = 2, ///< AnimObject Identifier
			AnimObjectC = 3, ///< AnimObject Identifier
			AnimObjectD = 4, ///< AnimObject Identifier
			Camera = 5, ///< Camera Identifier
			Camera_Control = 6, ///< Camera Controller
			CamTargetParent = 7, ///< _
			CameraTarget = 8, ///< _
			COM = 9, ///< Possibly needed for Zero-G and physics systems
			C_Hips = 10, ///< _
			R_Thigh = 11, ///< _
			R_Calf = 12, ///< _
			R_Foot = 13, ///< _
			R_Toe = 14, ///< _
			R_CalfMass = 15, ///< Unconfirmed
			R_Knee = 16, ///< Unconfirmed
			R_LegHolster = 17, ///< Unconfirmed
			R_Thigh_Twist_Base = 18, ///< Unconfirmed
			L_Thigh = 19, ///< _
			L_Calf = 20, ///< _
			L_Foot = 21, ///< _
			L_Toe = 22, ///< _
			L_CalfMass = 23, ///< Unconfirmed
			L_Knee = 24, ///< Unconfirmed
			L_LegHolster = 25, ///< Unconfirmed
			L_Thigh_Twist_Base = 26, ///< Unconfirmed
			L_Butt = 27, ///< Unconfirmed
			R_Butt = 28, ///< Unconfirmed
			C_Spine = 29, ///< _
			C_Spine1 = 30, ///< _
			C_Spine2 = 31, ///< _
			C_Chest = 32, ///< _
			C_Neck = 33, ///< _
			C_Neck1 = 34, ///< _
			C_Head = 35, ///< _
			L_Eye = 36, ///< Unconfirmed
			R_Eye = 37, ///< Unconfirmed
			C_Neck_Twist_Base = 38, ///< Unconfirmed
			L_Clavicle = 39, ///< _
			L_Biceps = 40, ///< _
			L_Forearm = 41, ///< _
			L_Wrist = 42, ///< _
			L_Thumb = 43, ///< _
			L_Thumb1 = 44, ///< _
			L_Thumb2 = 45, ///< _
			L_Cup = 46, ///< _
			L_Pinky = 47, ///< _
			L_Pinky1 = 48, ///< _
			L_Pinky2 = 49, ///< _
			L_Ring = 50, ///< _
			L_Ring1 = 51, ///< _
			L_Ring2 = 52, ///< _
			L_Middle = 53, ///< _
			L_Middle1 = 54, ///< _
			L_Middle2 = 55, ///< _
			L_Index = 56, ///< _
			L_Index1 = 57, ///< _
			L_Index2 = 58, ///< _
			L_AnimObject1 = 59, ///< _
			L_AnimObject2 = 60, ///< _
			L_AnimObject3 = 61, ///< _
			L_Arm = 62, ///< _
			L_Wrist_Twist_Base = 63, ///< Unconfirmed
			L_Elbow = 64, ///< _
			L_ArmMass = 65, ///< Unconfirmed
			L_Deltoid = 66, ///< Unconfirmed
			L_Biceps_Twist_Base = 67, ///< Unconfirmed
			C_BackPack = 68, ///< _
			C_BackPackHose = 69, ///< _
			L_BackPackHolster = 70, ///< Unconfirmed
			R_BackPackHolster = 71, ///< Unconfirmed
			R_Clavicle = 72, ///< _
			R_Biceps = 73, ///< _
			R_Forearm = 74, ///< _
			R_Wrist = 75, ///< _
			R_Thumb = 76, ///< _
			R_Thumb1 = 77, ///< _
			R_Thumb2 = 78, ///< _
			R_Cup = 79, ///< _
			R_Pinky = 80, ///< _
			R_Pinky1 = 81, ///< _
			R_Pinky2 = 82, ///< _
			R_Ring = 83, ///< _
			R_Ring1 = 84, ///< _
			R_Ring2 = 85, ///< _
			R_Middle = 86, ///< _
			R_Middle1 = 87, ///< _
			R_Middle2 = 88, ///< _
			R_Index = 89, ///< _
			R_Index1 = 90, ///< _
			R_Index2 = 91, ///< _
			R_AnimObject1 = 92, ///< _
			R_AnimObject2 = 93, ///< _
			R_AnimObject3 = 94, ///< _
			R_Arm = 95, ///< _
			R_Wrist_Twist_Base = 96, ///< Unconfirmed
			R_Elbow = 97, ///< _
			R_ArmMass = 98, ///< Unconfirmed
			R_Deltoid = 99, ///< Unconfirmed
			R_Biceps_Twist_Base = 100, ///< Unconfirmed
			Weapon = 101, ///< _
			Magazine = 102, ///< Unconfirmed
			Magazine01 = 103, ///< Unconfirmed
			Magazine02 = 104, ///< Unconfirmed
			Magazine03 = 105, ///< Unconfirmed
			Magazine04 = 106, ///< Unconfirmed
			Magazine05 = 107, ///< Unconfirmed
			Magazine06 = 108, ///< Unconfirmed
			Magazine07 = 109, ///< Unconfirmed
			Magazine08 = 110, ///< Unconfirmed
			Scope = 111, ///< Unconfirmed
			Scope01 = 112, ///< Unconfirmed
			Scope02 = 113, ///< Unconfirmed
			Scope03 = 114, ///< Unconfirmed
			Scope04 = 115, ///< Unconfirmed
			Scope05 = 116, ///< Unconfirmed
			Trigger = 117, ///< Unconfirmed
			Bolt = 118, ///< Unconfirmed
			Bolt01 = 119, ///< _
			Bolt02 = 120, ///< _
			Bolt03 = 121, ///< Unconfirmed
			MiscA = 122, ///< Unconfirmed
			MiscA01 = 123, ///< Unconfirmed
			MiscA02 = 124, ///< Unconfirmed
			MiscA03 = 125, ///< Unconfirmed
			MiscA04 = 126, ///< Unconfirmed
			MiscA05 = 127, ///< Unconfirmed
			MiscB = 128, ///< Unconfirmed
			MiscB01 = 129, ///< Unconfirmed
			MiscB02 = 130, ///< Unconfirmed
			MiscB03 = 131, ///< Unconfirmed
			MiscB04 = 132, ///< Unconfirmed
			MiscB05 = 133, ///< Unconfirmed
			WeaponLeft = 134, ///< _
			HandIkLeft = 135, ///< Unconfirmed
			HandIkRight = 136, ///< Unconfirmed
			DirectAt = 137, ///< _
			R_HipHolster = 138, ///< Unconfirmed
			L_HipHolster = 139, ///< Unconfirmed
			C_Waist = 140, ///< _
			Camera_Control_FP = 141, ///< _
			R_Thigh_Twist = 142, ///< _
			R_Thigh_Twist1 = 143, ///< _
			L_Thigh_Twist = 144, ///< _
			L_Thigh_Twist1 = 145, ///< _
			C_Neck_Twist = 146, ///< _
			L_Wrist_Twist = 147, ///< _
			L_Wrist_Twist1 = 148, ///< _
			L_Wrist_Twist2 = 149, ///< _
			L_Biceps_Twist = 150, ///< _
			L_Biceps_Twist1 = 151, ///< _
			R_Wrist_Twist = 152, ///< _
			R_Wrist_Twist1 = 153, ///< _
			R_Wrist_Twist2 = 154, ///< _
			R_Biceps_Twist = 155, ///< _
			R_Biceps_Twist1 = 156, ///< _



			None = 0xFF
		};

		/**
			 * @brief Convenience function when static casting int to enum
			 * @param input
			 * @return
			 */
		static bool BoneTagExists(BoneMapKey input);
		/// @}
		/// @name Constructors
		/// @{
		SFBGS_RigPackage();
		SFBGS_RigPackage(const SFBGS_RigPackage& source);
		~SFBGS_RigPackage() override;
		/// @}
		/// @name Mannequins
		/// @{
		/**
			 * @brief Checks if rig is marked for Mannequin processing
			 * @details Some rigs in game have specific features that are only seen on Mannequins
			 * @return
			 */
		[[nodiscard]] bool isMannequin() const;
		/**
			 * @brief Marks this rig for Mannequin processing
			 * @param mqn
			 */
		void setIsMannequin(bool mqn) const;
		/// @}
		/// @name Bone Maps
		/// @{

		/**
			 * @brief Checks if a specific bone is mapped via name
			 * @param boneName
			 * @return
			 */
		bool boneIsMapped(const char* boneName) const;
		/**
			 * @brief Checks to see if a bone is mapped to the given key
			 * @param key
			 * @return
			 */
		[[nodiscard]] bool keyIsMapped(BoneMapKey key) const;
		/**
			 * @brief Adds a bone to the map by name alone
			 * @param key Tag/Key to map to
			 * @param boneName Bone name to be mapped
			 * @param overwrite Whether to replace an existing bone, if found
			 * @return Whether the operation was a success
			 */
		bool addBoneToMap(BoneMapKey key, const char* boneName, bool overwrite = true) const;
		/**
			 * @brief Adds a bone to the map by instance
			 * @param key Tag/Key to map to
			 * @param bone Bone whose name will be mapped
			 * @param overwrite Whether to replace an existing bone, if found
			 * @return Whether the operation was a success
			 */
		bool addBoneToMap(BoneMapKey key, const UNIV::SkeletonBone& bone, bool overwrite = true) const; // NOLINT(*-use-nodiscard)
		/**
			 * @brief Removes the bone assigned to this key/tag
			 * @param key
			 * @return Whether the operation was successful
			 */
		bool removeBoneFromMap(BoneMapKey key) const; // NOLINT(*-use-nodiscard)
		/**
			 * @brief Removes a bone with the given name from it's assigned key/tag
			 * @param boneName
			 * @return Whether the operation was successful
			 */
		bool removeBoneFromMap(const char* boneName) const;

		/**
			 * @brief Returns the key/tag that this bone is assigned to
			 * @param boneName
			 * @return
			 */
		BoneMapKey boneKey(const char* boneName) const;
		/**
			 * @brief Returns the name of the bone assigned to this key, if any
			 * @param key
			 * @return
			 */
		[[nodiscard]] const char* boneNameFromKey(BoneMapKey key) const;
		/// @}
		/// @name Precision
		/// @{

		/**
			 * @brief Sets the precision values to be used by this rig and its animations when compressing translations
			 * @param setting
			 */
		void setPrecisionValues(const PrecisionSet& setting = PrecisionSet::DefaultPrecision()) const;
		/**
			 * @brief Returns the precision set used by this rig and its animation when compressing translations
			 * @return
			 */
		[[nodiscard]] PrecisionSet precisionSet() const;
		/// @}
		/// @name Operators
		/// @{
		SFBGS_RigPackage& operator=(const SFBGS_RigPackage& other);
		/// @}
		/// @name Inherited
		/// @{

		/**
			 * @brief Returns a string of the package type
			 * @return
			 */
		[[nodiscard]] const char* getPackageType() const override;
		/**
			 * @brief NOT YET IMPLEMENTED
			 * @param indents
			 * @return
			 */
		[[nodiscard]] Utilities::StringContainer toJSON(uint64_t indents) const override;

	protected:
		/**
			 * @brief Event called when the parent rig renames bones
			 * @param oldBone
			 * @param newName
			 * @return
			 */
		bool handleBoneRename(const char* oldBone, const char* newName) override;
		/**
			 * @brief Creates shallow copies of this package's elements without worry of ptr handling
			 * @return
			 */
		[[nodiscard]] SFBGS_RigPackage* clone() const override;
		/// @}

	private:
		struct Impl;
		Impl* pImpl;

	public:
		/// @name Package Handling
		/// @{

		/**
			 * @brief Method for adding a Starfield Rig Package to the UNIV Skeleton Rig
			 * @param rig
			 * @param overwrite Will overwrite with default package if existing package is found
			 * @return
			 */
		static bool AddPackage(const UNIV::SkeletonRig& rig, bool overwrite = true);
		/**
			 * @brief Method for removing a Starfield Rig Package from a UNIV Skeleton Rig
			 * @param rig
			 * @return
			 */
		static bool RemovePackage(const UNIV::SkeletonRig& rig);
		/**
		 *
		 * @param rig
		 * @return The SFBGS Rig Package on this SkeletonRig. If none exists, one will be created.
		 */
		[[nodiscard]] static SFBGS_RigPackage& GetPackage(const UNIV::SkeletonRig& rig);
		/**
			 * @brief Creates a convenience SFBGS BoneMap vector in the expected Starfield format
			 * @param boneList
			 * @return
			 */
		[[nodiscard]] Utilities::S16Vector ConvertMap(const Utilities::StringList& boneList) const;
		/// @}
	};
	/// @}
}




	 /// @addtogroup c_rig_packages
	 /// @{
	 /// @defgroup c_sfbgs_rig_package Starfield
	 /// @{
	extern "C"
	{
	/**
	 * @brief Adds a Starfield Rig Package to the UNIV Skeleton Rig
	 * @param rig UNIV Rig
	 * @param errorMessage *optional* error message container for returning error statements
	 * @param overwrite If set to true, will replace the Starfield Rig package, if one exists
	 * @return Whether the operation was successful
	 */
	CALUMIANIMATION_API bool SFBGSRigPackage_AddPackageToSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig, CALUMI::Utilities::StringContainer* errorMessage, bool overwrite);
	/**
	 * @brief
	 * @param rig UNIV Rig
	 * @param errorMessage *optional* error message container for returning error statements
	 * @return Whether the operation was successful
	 */
	CALUMIANIMATION_API bool SFBGSRigPackage_RemoveRigPackageFromSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig, CALUMI::Utilities::StringContainer* errorMessage);

	/**
	 * @brief
	 * @param rig UNIV Rig
	 * @param boneName Bone to check for
	 * @return Whether the bone name in question is mapped to any keys/tags
	 */
	CALUMIANIMATION_API bool SFBGSRigPackage_BoneIsMappedC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName);
	/**
	 * @brief
	 * @param rig UNIV Rig
	 * @param key uint8_t form of the BoneMapKey
	 * @return Whether a bone is mapped to the given key/tag
	 */
	CALUMIANIMATION_API bool SFBGSRigPackage_KeyIsMappedC(const CALUMI::UNIV::SkeletonRig* rig, uint8_t key);

	/**
	 * @brief
	 * @param rig UNIV Rig
	 * @param key uint8_t form of the BoneMapKey
	 * @param boneName Bone to add
	 * @param errorMessage *optional* error message container for returning error statements
	 * @param overwrite If set to true, will replace the bone that currently is assigned to this key/tag, if
	 * one exists
	 * @return Whether the operation was successful
	 */
	CALUMIANIMATION_API bool SFBGSRigPackage_AddBoneNameToMapC(const CALUMI::UNIV::SkeletonRig* rig, uint8_t key, const char* boneName, CALUMI::Utilities::StringContainer* errorMessage, bool overwrite);
	/**
	 * @brief
	 * @param rig UNIV Rig
	 * @param key uint8_t form of the BoneMapKey
	 * @param bone Bone to add, will use the bone's given name
	 * @param errorMessage *optional* error message container for returning error statements
	 * @param overwrite If set to true, will overwrite the bone assigned to this key/tag, if one exists
	 * @return Whether the operation was successful
	 */
	CALUMIANIMATION_API bool SFBGSRigPackage_AddBoneToMapC(const CALUMI::UNIV::SkeletonRig* rig, uint8_t key, const CALUMI::UNIV::SkeletonBone* bone, CALUMI::Utilities::StringContainer* errorMessage, bool overwrite);

	/**
	 * @brief Removes a bone assignment from the given key/tag
	 * @param rig UNIV Rig
	 * @param key uint8_t form of the BoneMapKey
	 * @return Whether the operation was successful
	 */
	CALUMIANIMATION_API bool SFBGSRigPackage_RemoveBoneFromMapUsingKeyC(const CALUMI::UNIV::SkeletonRig* rig, uint8_t key);
	/**
	 * @brief Removes the given bone from any assignments
	 * @param rig UNIV Rig
	 * @param boneName
	 * @return Whether the operation was successful
	 */
	CALUMIANIMATION_API bool SFBGSRigPackage_RemoveBoneFromMapUsingNameC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName);

	/**
	 * @brief Returns the key assigned to a bone name, if one is assigned
	 * @param rig UNIV Rig
	 * @param boneName
	 * @return The uint8_t form of the BoneMapKey enum
	 */
	CALUMIANIMATION_API uint8_t SFBGSRigPackage_GetBoneKeyC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName);
	/**
	 * @brief Returns the name of the bone assigned to this key, if one is assigned
	 * @param rig UNIV Rig
	 * @param key uint8_t form of the BoneMapKey
	 * @return
	 */
	CALUMIANIMATION_API const char* SFBGSRigPackage_GetBoneNameFromKeyC(const CALUMI::UNIV::SkeletonRig* rig, uint8_t key);

	/**
	 * @brief Sets the rig to be processed as a mannequin when exporting as a Starfield skeleton.rig
	 * @param rig UNIV Rig
	 * @param isMannequin
	 * @return Whether the operation was successful
	 */
	CALUMIANIMATION_API bool SFBGSRigPackage_SetMannequinC(const CALUMI::UNIV::SkeletonRig* rig, bool isMannequin);
	/**
	 * @brief Returns if the rig is set to process as a mannequin when exporting as a Starfield skeleton.rig
	 * @param rig UNIV Rig
	 * @return
	 */
	CALUMIANIMATION_API bool SFBGSRigPackage_IsMannequinC(const CALUMI::UNIV::SkeletonRig* rig);
	/**
	 * @brief Returns the size of the Rig BoneMap which is always 157 (or *Pi* / 0.02 :) )
	 * @return 157
	 */
	CALUMIANIMATION_API uint64_t SFBGSRigPackage_GetRigMapSize();

	/**
	 * @brief Sets the rig to process all relative translations with the *Default* precision set
	 * @details Default Values High: 1/4000f and Low: 1/32f
	 * @param rig UNIV Rig
	 */
	CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToDefaultC(const CALUMI::UNIV::SkeletonRig* rig);
	/**
	 * @brief Sets the rig to process all relative translations with the *1st Person* precision set
	 * @param rig UNIV Rig
	 */
	CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToFirstPersonC(const CALUMI::UNIV::SkeletonRig* rig);
	/**
	 * @brief Sets the rig to process all relative translations with the *Ship* precision set
	 * @param rig UNIV Rig
	 */
	CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToShipValuesC(const CALUMI::UNIV::SkeletonRig* rig);
	/**
	 * @brief Sets the rig to process all relative translations with *Custom* precision values
	 * @details Will automatically sort the two custom values into a high and low precision set
	 * @param rig UNIV Rig
	 * @param custom1 Non zero value
	 * @param custom2 Non zero value
	 */
	CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToCustomC(const CALUMI::UNIV::SkeletonRig* rig, float custom1, float custom2);

	/**
	 * @brief Returns the precision type that this rig is set to use when compressing into the Starfield format
	 * @details This method is a convenience method for UI building or message passing to the user
	 * @param rig UNIV Rig
	 * @return A string representing the set (Default, Ship, First Person, Custom)
	 */
	CALUMIANIMATION_API const char* SFBGSRigPackage_GetPrecisionType(const CALUMI::UNIV::SkeletonRig* rig);
	/**
	 * @brief The high precision value used by this rig for compressing into the Starfield format
	 * @param rig UNIV Rig
	 * @return
	 */
	CALUMIANIMATION_API float SFBGSRigPackage_GetHighPrecisionValueC(const CALUMI::UNIV::SkeletonRig* rig);
	/**
	 * @brief The low precision value used by this rig for compressing into the Starfield format
	 * @param rig UNIV Rig
	 * @return
	 */
	CALUMIANIMATION_API float SFBGSRigPackage_GetLowPrecisionValueC(const CALUMI::UNIV::SkeletonRig* rig);

	}

	/// @}
	/// @}

