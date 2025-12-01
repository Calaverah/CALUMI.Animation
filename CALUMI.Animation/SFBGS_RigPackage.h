#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_SkeletonRig.h"

constexpr int SFBGSMAPSIZE = 157;
static const CALUMI::Utilities::PairContainer<float, float> SFBGSDefaultPrecision = { 0.00025f , 0.03125f }; // 1/4000 and 1/32
static const CALUMI::Utilities::PairContainer<float, float> SFBGSFirstPersonPrecision = { 0.0000625f , 0.0078125f }; // 1/16000 and 1/128
static const CALUMI::Utilities::PairContainer<float, float> SFBGSShipPrecision = { 0.002f , 0.25f }; // 1/500 and 1/4

namespace CALUMI {
	namespace SFBGS {

		inline static const char* SFBGS_RIG_PACKAGE = "SFBGS_RIG_PACKAGE";

		enum class CALUMIANIMATION_API BoneMapKey : uint8_t
		{
			Root = 0,
			AnimObjectA = 1,
			AnimObjectB = 2,
			AnimObjectC = 3,
			AnimObjectD = 4,
			Camera = 5,
			Camera_Control = 6,
			CamTargetParent = 7,
			CameraTarget = 8,
			COM = 9,
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

		CALUMIANIMATION_API bool BoneTagExists(BoneMapKey input);

		enum class CALUMIANIMATION_API PrecisionSet : uint8_t
		{
			Default = 0,
			FirstPerson = 1,
			Ship = 2,

			Custom = 0xFF
		};

		struct CALUMIANIMATION_API SFBGS_RigPackage : UNIV::RigPackage
		{
		public:

			bool IsMannequin() const;
			void IsMannequin(bool mqn);

			// Inherited via RigPackage
			const char* GetPackageType() const override;

			SFBGS_RigPackage();
			SFBGS_RigPackage(const SFBGS_RigPackage& source);
			~SFBGS_RigPackage() override;

			bool BoneIsMapped(const char* boneName) const;
			bool KeyIsMapped(BoneMapKey key);

			bool AddBoneToMap(BoneMapKey key, const char* boneName, bool overwrite = true);
			bool AddBoneToMap(BoneMapKey key, UNIV::SkeletonBone& bone, bool overwrite = true);

			bool RemoveBoneFromMap(BoneMapKey key);
			bool RemoveBoneFromMap(const char* boneName);

			SFBGS_RigPackage& operator=(const SFBGS_RigPackage& other);

			BoneMapKey GetBoneKey(const char* boneName) const;
			const char* GetBoneNameFromKey(BoneMapKey key);

			void SetPrecisionValues(PrecisionSet setting, float custom1 = SFBGSDefaultPrecision.getFirst(), float custom2 = SFBGSDefaultPrecision.getSecond());
			float LowPrecisionValue() const;
			float HighPrecisionValue() const;

			// Inherited via RigPackage
			Utilities::StringContainer ToJSON(size_t indents) const override;

			// Inherited via RigPackage
			bool HandleBoneRename(const char* oldBone, const char* newName, size_t idx) override;
		private:
			struct Impl;
			Impl* pImpl;
		};

		SFBGS_RigPackage* CreateNewSFBGSRigPackage(UNIV::SkeletonRig& rig, bool overwrite = true);
		bool RemoveSFBGSRigPackage(UNIV::SkeletonRig& rig);

		Utilities::VectorContainer<int16_t> ConvertSFBGSRigPackage(UNIV::SkeletonRig& rig);




		extern "C"
		{
			CALUMIANIMATION_API bool SFBGSRigPackage_AddPackageToSkeletonRigC(UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage, bool overwrite);
			CALUMIANIMATION_API bool SFBGSRigPackage_RemoveRigPackageFromSkeletonRigC(UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage);

			CALUMIANIMATION_API bool SFBGSRigPackage_BoneIsMappedC(UNIV::SkeletonRig* rig, const char* boneName);
			CALUMIANIMATION_API bool SFBGSRigPackage_KeyIsMappedC(UNIV::SkeletonRig* rig, uint8_t key);

			CALUMIANIMATION_API bool SFBGSRigPackage_AddBoneNameToMapC(UNIV::SkeletonRig* rig, uint8_t key, const char* boneName, Utilities::StringContainer* errorMessage, bool overwrite);
			CALUMIANIMATION_API bool SFBGSRigPackage_AddBoneToMapC(UNIV::SkeletonRig* rig, uint8_t key, UNIV::SkeletonBone* bone, Utilities::StringContainer* errorMessage, bool overwrite);

			CALUMIANIMATION_API bool SFBGSRigPackage_RemoveBoneFromMapUsingKeyC(UNIV::SkeletonRig* rig, uint8_t key);
			CALUMIANIMATION_API bool SFBGSRigPackage_RemoveBoneFromMapUsingNameC(UNIV::SkeletonRig* rig, const char* boneName);

			CALUMIANIMATION_API uint8_t SFBGSRigPackage_GetBoneKeyC(UNIV::SkeletonRig* rig, const char* boneName);
			CALUMIANIMATION_API const char* SFBGSRigPackage_GetBoneNameFromKeyC(UNIV::SkeletonRig* rig, uint8_t key);

			CALUMIANIMATION_API bool SFBGSRigPackage_SetMannequinC(UNIV::SkeletonRig* rig, bool isMannequin);
			CALUMIANIMATION_API bool SFBGSRigPackage_IsMannequinC(UNIV::SkeletonRig* rig);
			CALUMIANIMATION_API size_t SFBGSRigPackage_GetRigMapSize();

			CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToDefaultC(UNIV::SkeletonRig* rig);
			CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToFirstPersonC(UNIV::SkeletonRig* rig);
			CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToShipValuesC(UNIV::SkeletonRig* rig);
			CALUMIANIMATION_API void SFBGSRigPackage_SetPrecisionToCustomC(UNIV::SkeletonRig* rig, float custom1, float custom2);

			CALUMIANIMATION_API float SFBGSRigPackage_GetHighPrecisionValueC(UNIV::SkeletonRig* rig);
			CALUMIANIMATION_API float SFBGSRigPackage_GetLowPrecisionValueC(UNIV::SkeletonRig* rig);

		}

} }
#pragma warning(disable: 4661)
_VECTORTEMPLATE(CALUMI::SFBGS::SFBGS_RigPackage);
#pragma warning(default: 4661)