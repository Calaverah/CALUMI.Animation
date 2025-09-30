#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "CALUMI_Common.h"
#include "CALUMI_SkeletonRig.h"

#define SFBGSMAPSIZE 157

namespace CALUMI {
	namespace SFBGS {

		inline static const char* SFBGS_RIG_PACKAGE = "SFBGS_RIG_PACKAGE";

		enum class BoneMapKey : uint8_t
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

		bool BoneTagExists(BoneMapKey input);

		struct CALUMIANIMATION_API RigMap
		{
		private:
			std::string boneNameList[SFBGSMAPSIZE];

		public:
			bool BoneIsMapped(const char* boneName);
			bool KeyHasBone(BoneMapKey key);

			bool AddBoneToMap(BoneMapKey key, const char* boneName, bool shouldReassign = false);

			bool RemoveBoneFromMap(BoneMapKey key);
			bool RemoveBoneFromMap(const char* boneName);


			BoneMapKey GetBoneKey(const char* boneName);
			const char* GetBoneFromKey(BoneMapKey key);

		};


		struct SFBGS_RigPackage : UNIV::RigPackage
		{
			bool isMannequin = false;
			SFBGS::RigMap rigMap;


			// Inherited via RigPackage
			inline const char* GetPackageType() const override{ return SFBGS_RIG_PACKAGE; }
		};

} }
