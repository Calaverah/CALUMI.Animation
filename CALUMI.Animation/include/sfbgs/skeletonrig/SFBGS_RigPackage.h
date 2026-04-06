//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Common.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"

inline constexpr int SFBGSMAPSIZE = 157;

namespace CALUMI {
	namespace SFBGS {

		inline constexpr const char* SFBGS_RIG_PACKAGE = "SFBGS_RIG_PACKAGE";

		/**
		 * @brief Simple pair of values used to compress translations on Starfield animations
		 */
		struct PrecisionSet{
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
			bool operator==(const PrecisionSet& other);
			bool operator!=(const PrecisionSet& other);
			/// @}
			/// @name Data
			/// @{
			float high();
			float low();
			const char* precisionType();
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
				AnimObjectA = 1, //AnimObject Identifier
				AnimObjectB = 2, //AnimObject Identifier
				AnimObjectC = 3, //AnimObject Identifier
				AnimObjectD = 4, //AnimObject Identifier
				Camera = 5, //Camera Identifier
				Camera_Control = 6, //Camera Controller
				CamTargetParent = 7,
				CameraTarget = 8,
				COM = 9, //Possibly needed for Zero-G and physics systems
				C_Hips = 10,
				R_Thigh = 11,
				R_Calf = 12,
				R_Foot = 13,
				R_Toe = 14,
				L_Thigh = 19,
				L_Calf = 20,
				L_Foot = 21,
				L_Toe = 22,
				C_Spine = 29,
				C_Spine1 = 30,
				C_Spine2 = 31,
				C_Chest = 32,
				C_Neck = 33,
				C_Neck1 = 34,
				C_Head = 35,
				L_Clavicle = 39,
				L_Biceps = 40,
				L_Forearm = 41,
				L_Wrist = 42,
				L_Thumb = 43,
				L_Thumb1 = 44,
				L_Thumb2 = 45,
				L_Cup = 46,
				L_Pinky = 47,
				L_Pinky1 = 48,
				L_Pinky2 = 49,
				L_Ring = 50,
				L_Ring1 = 51,
				L_Ring2 = 52,
				L_Middle = 53,
				L_Middle1 = 54,
				L_Middle2 = 55,
				L_Index = 56,
				L_Index1 = 57,
				L_Index2 = 58,
				L_AnimObject1 = 59,
				L_AnimObject2 = 60,
				L_AnimObject3 = 61,
				L_Arm = 62,
				L_Elbow = 64,
				C_BackPack = 68,
				C_BackPackHose = 69,
				R_Clavicle = 72,
				R_Biceps = 73,
				R_Forearm = 74,
				R_Wrist = 75,
				R_Thumb = 76,
				R_Thumb1 = 77,
				R_Thumb2 = 78,
				R_Cup = 79,
				R_Pinky = 80,
				R_Pinky1 = 81,
				R_Pinky2 = 82,
				R_Ring = 83,
				R_Ring1 = 84,
				R_Ring2 = 85,
				R_Middle = 86,
				R_Middle1 = 87,
				R_Middle2 = 88,
				R_Index = 89,
				R_Index1 = 90,
				R_Index2 = 91,
				R_AnimObject1 = 92,
				R_AnimObject2 = 93,
				R_AnimObject3 = 94,
				R_Arm = 95,
				R_Elbow = 97,
				Weapon = 101,
				Bolt01 = 119,
				Bolt02 = 120,
				WeaponLeft = 134,
				DirectAt = 137,
				C_Waist = 140,
				Camera_Control_FP = 141,
				R_Thigh_Twist = 142,
				R_Thigh_Twist1 = 143,
				L_Thigh_Twist = 144,
				L_Thigh_Twist1 = 145,
				C_Neck_Twist = 146,
				L_Wrist_Twist = 147,
				L_Wrist_Twist1 = 148,
				L_Wrist_Twist2 = 149,
				L_Biceps_Twist = 150,
				L_Biceps_Twist1 = 151,
				R_Wrist_Twist = 152,
				R_Wrist_Twist1 = 153,
				R_Wrist_Twist2 = 154,
				R_Biceps_Twist = 155,
				R_Biceps_Twist1 = 156,



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
			bool IsMannequin() const;
			/**
			 * @brief Marks this rig for Mannequin processing
			 * @param mqn 
			 */
			void IsMannequin(bool mqn);
			/// @}
			/// @name Bone Maps
			/// @{
			
			/**
			 * @brief Checks if a specific bone is mapped via name
			 * @param boneName 
			 * @return 
			 */
			bool BoneIsMapped(const char* boneName) const;
			/**
			 * @brief Checks to see if a bone is mapped to the given key
			 * @param key 
			 * @return 
			 */
			bool KeyIsMapped(BoneMapKey key);
			/**
			 * @brief Adds a bone to the map by name alone
			 * @param key Tag/Key to map to
			 * @param boneName Bone name to be mapped
			 * @param overwrite Whether to replace an existing bone, if found
			 * @return Whether the operation was a success
			 */
			bool AddBoneToMap(BoneMapKey key, const char* boneName, bool overwrite = true);
			/**
			 * @brief Adds a bone to the map by instnace
			 * @param key Tag/Key to map to
			 * @param bone Bone who's name will be mapped
			 * @param overwrite Whether to replace an existing bone, if found
			 * @return Whether the operation was a success
			 */
			bool AddBoneToMap(BoneMapKey key, UNIV::SkeletonBone& bone, bool overwrite = true);
			/**
			 * @brief Removes the bone assigned to this key/tag
			 * @param key 
			 * @return Whether the operation was successful
			 */
			bool RemoveBoneFromMap(BoneMapKey key);
			/**
			 * @brief Removes a bone with the given name from it's assigned key/tag
			 * @param boneName 
			 * @return Whether the operation was successful
			 */
			bool RemoveBoneFromMap(const char* boneName);

			/**
			 * @brief Returns the key/tag that this bone is assigned to
			 * @param boneName 
			 * @return 
			 */
			BoneMapKey GetBoneKey(const char* boneName) const;
			/**
			 * @brief Returns the name of the bone assigned to this key, if any
			 * @param key 
			 * @return 
			 */
			const char* GetBoneNameFromKey(BoneMapKey key);
			/// @}
			/// @name Precision
			/// @{
			
			/**
			 * @brief Sets the precision values to be used by this rig and its animations when compressing translations
			 * @param setting 
			 */
			void SetPrecisionValues(PrecisionSet setting = PrecisionSet::DefaultPrecision());
			/**
			 * @brief Returns the precision set used by this rig and its animation when compressing translations
			 * @return 
			 */
			PrecisionSet getPrecisionSet() const;
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
			const char* getPackageType() const override;
			/**
			 * @brief NOT YET IMPLEMENTED
			 * @param indents 
			 * @return 
			 */
			Utilities::StringContainer ToJSON(uint64_t indents) const override;

		protected:
			/**
			 * @brief Event called when the parent rig renames bones
			 * @param oldBone 
			 * @param newName 
			 * @param idx 
			 * @return 
			 */
			bool HandleBoneRename(const char* oldBone, const char* newName, uint64_t idx) override;
			/**
			 * @brief Creates shallow copies of this package's elements without worry of ptr handling
			 * @return 
			 */
			SFBGS_RigPackage* Clone() const override;
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
			static bool CreateNewSFBGSRigPackage(UNIV::SkeletonRig& rig, bool overwrite = true);
			/**
			 * @brief Method for removing a Starfield Rig Package from a UNIV Skeleton Rig
			 * @param rig 
			 * @return 
			 */
			static bool RemoveSFBGSRigPackage(UNIV::SkeletonRig& rig);

			/**
			 * @brief Creates a convenience SFBGS BoneMap vector in the expected Starfield format
			 * @param rig 
			 * @return 
			 */
			static Utilities::S16Vector ConvertSFBGSRigPackage(const UNIV::SkeletonRig& rig);
			/// @}
		};


		/**
		 *
		 *
		 * @addtogroup c_univ_rig_package
		 * @{
		 * @defgroup c_sfbgs_rig_package Starfield
		 * @{
		 */


		extern "C"
		{
			/**
			 * @brief Adds a Starfield Rig Package to the UNIV Skeleton Rig
			 * @param rig UNIV Rig
			 * @param _message *optional* error message container for returning error statements 
			 * @param overwrite If set to true, will replace the Starfield Rig package, if one exists
			 * @return Whether the operation was successful
			 */
			CALUMIANIMATION_API bool SFBGSRigPackage_AddPackageToSkeletonRigC(UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage, bool overwrite);
			/**
			 * @brief 
			 * @param rig UNIV Rig
			 * @param _message *optional* error message container for returning error statements 
			 * @return Whether the operation was successful
			 */
			CALUMIANIMATION_API bool SFBGSRigPackage_RemoveRigPackageFromSkeletonRigC(UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage);

			/**
			 * @brief 
			 * @param rig UNIV Rig
			 * @param boneName Bone to check for
			 * @return Whether the bone name in question is mapped to any keys/tags
			 */
			CALUMIANIMATION_API bool SFBGSRigPackage_BoneIsMappedC(UNIV::SkeletonRig* rig, const char* boneName);
			/**
			 * @brief 
			 * @param rig UNIV Rig
			 * @param key uint8_t form of the BoneMapKey
			 * @return Whether a bone is mapped to the given key/tag
			 */
			CALUMIANIMATION_API bool SFBGSRigPackage_KeyIsMappedC(UNIV::SkeletonRig* rig, uint8_t key);

			/**
			 * @brief 
			 * @param rig UNIV Rig
			 * @param key uint8_t form of the BoneMapKey
			 * @param boneName Bone to add
			 * @param _message *optional* error message container for returning error statements 
			 * @param overwrite If set to true, will replace the bone that currently is assigned to this key/tag, if one exists
			 * @return Whether the operation was successful
			 */
			CALUMIANIMATION_API bool SFBGSRigPackage_AddBoneNameToMapC(UNIV::SkeletonRig* rig, uint8_t key, const char* boneName, Utilities::StringContainer* errorMessage, bool overwrite);
			/**
			 * @brief 
			 * @param rig UNIV Rig
			 * @param key uint8_t form of the BoneMapKey
			 * @param bone Bone to add, will use the bone's given name
			 * @param _message *optional* error message container for returning error statements 
			 * @param overwrite If set to true, will overwrite the bone assigned to this key/tag, if one exists
			 * @return Whether the operation was successful
			 */
			CALUMIANIMATION_API bool SFBGSRigPackage_AddBoneToMapC(UNIV::SkeletonRig* rig, uint8_t key, UNIV::SkeletonBone* bone, Utilities::StringContainer* errorMessage, bool overwrite);

			/**
			 * @brief Removes a bone assignment from the given key/tag
			 * @param rig UNIV Rig
			 * @param key uint8_t form of the BoneMapKey
			 * @return Whether the operation was successful
			 */
			CALUMIANIMATION_API bool SFBGSRigPackage_RemoveBoneFromMapUsingKeyC(UNIV::SkeletonRig* rig, uint8_t key);
			/**
			 * @brief Removes the given bone from any assignments
			 * @param rig UNIV Rig
			 * @param boneName 
			 * @return Whether the operation was successful
			 */
			CALUMIANIMATION_API bool SFBGSRigPackage_RemoveBoneFromMapUsingNameC(UNIV::SkeletonRig* rig, const char* boneName);

			/**
			 * @brief Returns the key assigned to a bone name, if one is assigned
			 * @param rig UNIV Rig
			 * @param boneName 
			 * @return The uint8_t form of the BoneMapKey enum
			 */
			CALUMIANIMATION_API uint8_t SFBGSRigPackage_GetBoneKeyC(UNIV::SkeletonRig* rig, const char* boneName);
			/**
			 * @brief Returns the name of the bone assigned to this key, if one is assigned
			 * @param rig UNIV Rig
			 * @param key uint8_t form of the BoneMapKey
			 * @return 
			 */
			CALUMIANIMATION_API const char* SFBGSRigPackage_GetBoneNameFromKeyC(UNIV::SkeletonRig* rig, uint8_t key);

			/**
			 * @brief Sets the rig to be processed as a mannequin when exporting as a Starfield skeleton.rig
			 * @param rig UNIV Rig
			 * @param isMannequin 
			 * @return Whether the operation was successful
			 */
			CALUMIANIMATION_API bool SFBGSRigPackage_SetMannequinC(UNIV::SkeletonRig* rig, bool isMannequin);
			/**
			 * @brief Returns if the rig is set to process as a mannequin when exporting as a Starfield skeleton.rig
			 * @param rig UNIV Rig
			 * @return 
			 */
			CALUMIANIMATION_API bool SFBGSRigPackage_IsMannequinC(UNIV::SkeletonRig* rig);
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
			CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToDefaultC(UNIV::SkeletonRig* rig);
			/**
			 * @brief Sets the rig to process all relative translations with the *1st Person* precision set
			 * @param rig UNIV Rig
			 */
			CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToFirstPersonC(UNIV::SkeletonRig* rig);
			/**
			 * @brief Sets the rig to process all relative translations with the *Ship* precision set
			 * @param rig UNIV Rig
			 */
			CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToShipValuesC(UNIV::SkeletonRig* rig);
			/**
			 * @brief Sets the rig to process all relative translations with *Custom* precision values
			 * @details Will automatically sort the two custom values into a high and low precision set
			 * @param rig UNIV Rig
			 * @param custom1 Non zero value
			 * @param custom2 Non zero value
			 */
			CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToCustomC(UNIV::SkeletonRig* rig, float custom1, float custom2);

			/**
			 * @brief Returns the precision type that this rig is set to use when compressing into the Starfield format
			 * @details This method is a convenience method for UI building or message passing to the user
			 * @param rig UNIV Rig
			 * @return A string representing the set (Default, Ship, First Person, Custom)
			 */
			CALUMIANIMATION_API const char* SFBGSRigPackage_GetPrecisionType(UNIV::SkeletonRig* rig);
			/**
			 * @brief The high precision value used by this rig for compressing into the Starfield format
			 * @param rig UNIV Rig
			 * @return 
			 */
			CALUMIANIMATION_API float SFBGSRigPackage_GetHighPrecisionValueC(UNIV::SkeletonRig* rig);
			/**
			 * @brief The low precision value used by this rig for compressing into the Starfield format
			 * @param rig UNIV Rig
			 * @return 
			 */
			CALUMIANIMATION_API float SFBGSRigPackage_GetLowPrecisionValueC(UNIV::SkeletonRig* rig);

		}

		/// @}
		/// @}
} }
