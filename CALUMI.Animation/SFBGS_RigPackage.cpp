#include "pch.h"
#include "SFBGS_RigPackage.h"

namespace CALUMI {namespace SFBGS{

	bool BoneTagExists(BoneMapKey input)
	{
		switch (input)
		{
		case CALUMI::SFBGS::BoneMapKey::None:
			return false;
		case CALUMI::SFBGS::BoneMapKey::Root:
		case CALUMI::SFBGS::BoneMapKey::AnimObjectA:
		case CALUMI::SFBGS::BoneMapKey::AnimObjectB:
		case CALUMI::SFBGS::BoneMapKey::AnimObjectC:
		case CALUMI::SFBGS::BoneMapKey::AnimObjectD:
		case CALUMI::SFBGS::BoneMapKey::Camera:
		case CALUMI::SFBGS::BoneMapKey::Camera_Control:
		case CALUMI::SFBGS::BoneMapKey::CamTargetParent:
		case CALUMI::SFBGS::BoneMapKey::CameraTarget:
		case CALUMI::SFBGS::BoneMapKey::COM:
		case CALUMI::SFBGS::BoneMapKey::C_Hips:
		case CALUMI::SFBGS::BoneMapKey::R_Thigh:
		case CALUMI::SFBGS::BoneMapKey::R_Calf:
		case CALUMI::SFBGS::BoneMapKey::R_Foot:
		case CALUMI::SFBGS::BoneMapKey::R_Toe:
		case CALUMI::SFBGS::BoneMapKey::L_Thigh:
		case CALUMI::SFBGS::BoneMapKey::L_Calf:
		case CALUMI::SFBGS::BoneMapKey::L_Foot:
		case CALUMI::SFBGS::BoneMapKey::L_Toe:
		case CALUMI::SFBGS::BoneMapKey::C_Spine:
		case CALUMI::SFBGS::BoneMapKey::C_Spine1:
		case CALUMI::SFBGS::BoneMapKey::C_Spine2:
		case CALUMI::SFBGS::BoneMapKey::C_Chest:
		case CALUMI::SFBGS::BoneMapKey::C_Neck:
		case CALUMI::SFBGS::BoneMapKey::C_Neck1:
		case CALUMI::SFBGS::BoneMapKey::C_Head:
		case CALUMI::SFBGS::BoneMapKey::L_Clavicle:
		case CALUMI::SFBGS::BoneMapKey::L_Biceps:
		case CALUMI::SFBGS::BoneMapKey::L_Forearm:
		case CALUMI::SFBGS::BoneMapKey::L_Wrist:
		case CALUMI::SFBGS::BoneMapKey::L_Thumb:
		case CALUMI::SFBGS::BoneMapKey::L_Thumb1:
		case CALUMI::SFBGS::BoneMapKey::L_Thumb2:
		case CALUMI::SFBGS::BoneMapKey::L_Cup:
		case CALUMI::SFBGS::BoneMapKey::L_Pinky:
		case CALUMI::SFBGS::BoneMapKey::L_Pinky1:
		case CALUMI::SFBGS::BoneMapKey::L_Pinky2:
		case CALUMI::SFBGS::BoneMapKey::L_Ring:
		case CALUMI::SFBGS::BoneMapKey::L_Ring1:
		case CALUMI::SFBGS::BoneMapKey::L_Ring2:
		case CALUMI::SFBGS::BoneMapKey::L_Middle:
		case CALUMI::SFBGS::BoneMapKey::L_Middle1:
		case CALUMI::SFBGS::BoneMapKey::L_Middle2:
		case CALUMI::SFBGS::BoneMapKey::L_Index:
		case CALUMI::SFBGS::BoneMapKey::L_Index1:
		case CALUMI::SFBGS::BoneMapKey::L_Index2:
		case CALUMI::SFBGS::BoneMapKey::L_AnimObject1:
		case CALUMI::SFBGS::BoneMapKey::L_AnimObject2:
		case CALUMI::SFBGS::BoneMapKey::L_AnimObject3:
		case CALUMI::SFBGS::BoneMapKey::L_Arm:
		case CALUMI::SFBGS::BoneMapKey::L_Elbow:
		case CALUMI::SFBGS::BoneMapKey::C_BackPack:
		case CALUMI::SFBGS::BoneMapKey::C_BackPackHose:
		case CALUMI::SFBGS::BoneMapKey::R_Clavicle:
		case CALUMI::SFBGS::BoneMapKey::R_Biceps:
		case CALUMI::SFBGS::BoneMapKey::R_Forearm:
		case CALUMI::SFBGS::BoneMapKey::R_Wrist:
		case CALUMI::SFBGS::BoneMapKey::R_Thumb:
		case CALUMI::SFBGS::BoneMapKey::R_Thumb1:
		case CALUMI::SFBGS::BoneMapKey::R_Thumb2:
		case CALUMI::SFBGS::BoneMapKey::R_Cup:
		case CALUMI::SFBGS::BoneMapKey::R_Pinky:
		case CALUMI::SFBGS::BoneMapKey::R_Pinky1:
		case CALUMI::SFBGS::BoneMapKey::R_Pinky2:
		case CALUMI::SFBGS::BoneMapKey::R_Ring:
		case CALUMI::SFBGS::BoneMapKey::R_Ring1:
		case CALUMI::SFBGS::BoneMapKey::R_Ring2:
		case CALUMI::SFBGS::BoneMapKey::R_Middle:
		case CALUMI::SFBGS::BoneMapKey::R_Middle1:
		case CALUMI::SFBGS::BoneMapKey::R_Middle2:
		case CALUMI::SFBGS::BoneMapKey::R_Index:
		case CALUMI::SFBGS::BoneMapKey::R_Index1:
		case CALUMI::SFBGS::BoneMapKey::R_Index2:
		case CALUMI::SFBGS::BoneMapKey::R_AnimObject1:
		case CALUMI::SFBGS::BoneMapKey::R_AnimObject2:
		case CALUMI::SFBGS::BoneMapKey::R_AnimObject3:
		case CALUMI::SFBGS::BoneMapKey::R_Arm:
		case CALUMI::SFBGS::BoneMapKey::R_Elbow:
		case CALUMI::SFBGS::BoneMapKey::Weapon:
		case CALUMI::SFBGS::BoneMapKey::Bolt01:
		case CALUMI::SFBGS::BoneMapKey::Bolt02:
		case CALUMI::SFBGS::BoneMapKey::WeaponLeft:
		case CALUMI::SFBGS::BoneMapKey::DirectAt:
		case CALUMI::SFBGS::BoneMapKey::C_Waist:
		case CALUMI::SFBGS::BoneMapKey::Camera_Control_FP:
		case CALUMI::SFBGS::BoneMapKey::R_Thigh_Twist:
		case CALUMI::SFBGS::BoneMapKey::R_Thigh_Twist1:
		case CALUMI::SFBGS::BoneMapKey::L_Thigh_Twist:
		case CALUMI::SFBGS::BoneMapKey::L_Thigh_Twist1:
		case CALUMI::SFBGS::BoneMapKey::C_Neck_Twist:
		case CALUMI::SFBGS::BoneMapKey::L_Wrist_Twist:
		case CALUMI::SFBGS::BoneMapKey::L_Wrist_Twist1:
		case CALUMI::SFBGS::BoneMapKey::L_Wrist_Twist2:
		case CALUMI::SFBGS::BoneMapKey::L_Biceps_Twist:
		case CALUMI::SFBGS::BoneMapKey::L_Biceps_Twist1:
		case CALUMI::SFBGS::BoneMapKey::R_Wrist_Twist:
		case CALUMI::SFBGS::BoneMapKey::R_Wrist_Twist1:
		case CALUMI::SFBGS::BoneMapKey::R_Wrist_Twist2:
		case CALUMI::SFBGS::BoneMapKey::R_Biceps_Twist:
		case CALUMI::SFBGS::BoneMapKey::R_Biceps_Twist1:
			return true;
		default:
			return false;

		}
	}

	bool RigMap::BoneIsMapped(const char* boneName)
	{
		for (int i = 0; i < SFBGSMAPSIZE; i++)
		{
			if (strcmp(boneNameList[i].c_str(), boneName))
			{
				return true;
			}
		}
		return false;
	}

	bool RigMap::KeyHasBone(BoneMapKey key)
	{
		if (!BoneTagExists(key) || static_cast<uint8_t>(key) >= SFBGSMAPSIZE)
			return false;

		return !boneNameList[static_cast<uint8_t>(key)].Empty();
	}

	bool RigMap::AddBoneToMap(BoneMapKey key, const char* boneName, bool overwrite)
	{
		if (!BoneTagExists(key) || static_cast<uint8_t>(key) >= SFBGSMAPSIZE)
			return false;

		if (BoneIsMapped(boneName))
		{
			if (!overwrite)
				return false;

			RemoveBoneFromMap(boneName);
		}

		boneNameList[static_cast<uint8_t>(key)] = boneName;
		return true;
	}

	bool RigMap::RemoveBoneFromMap(BoneMapKey key)
	{
		if (!BoneTagExists(key) || static_cast<uint8_t>(key) >= SFBGSMAPSIZE)
			return false;

		boneNameList[static_cast<uint8_t>(key)].Clear();
		return true;
	}

	bool RigMap::RemoveBoneFromMap(const char* boneName)
	{
		return	RemoveBoneFromMap(GetBoneKey(boneName));
	}

	BoneMapKey RigMap::GetBoneKey(const char* boneName)
	{
		for (int i = 0; i < SFBGSMAPSIZE; i++)
		{
			if (strcmp(boneNameList[i].c_str(), boneName))
			{
				return (BoneMapKey)i;
			}
		}


		return BoneMapKey::None;
	}

	Utilities::StringContainer RigMap::GetBoneNameFromKey(BoneMapKey key)
	{
		if(!KeyHasBone(key))
			return "";

		return boneNameList[static_cast<uint8_t>(key)];
	}


	// Inherited via RigPackage
	const char* SFBGS_RigPackage::GetPackageType() const { return SFBGS_RIG_PACKAGE; }

}
}