#include "pch.h"
#include "SFBGS_RigPackage.h"
#include <format>

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

	SFBGS_RigPackage* CreateNewSFBGSRigPackage(UNIV::SkeletonRig& rig, bool overwrite)
	{
		SFBGS_RigPackage* newRigPackage = new SFBGS_RigPackage;
		if (!rig.rigPackageManager.AddPackage(newRigPackage, overwrite))
		{
			if (newRigPackage)
				delete newRigPackage;
			return nullptr;
		}
		return newRigPackage;
	}

	bool RemoveSFBGSRigPackage(UNIV::SkeletonRig& rig)
	{
		return rig.rigPackageManager.RemovePackage(SFBGS_RIG_PACKAGE);
	}

	Utilities::VectorContainer<int16_t> ConvertSFBGSRigPackage(UNIV::SkeletonRig& rig)
	{
		Utilities::VectorContainer<int16_t> output(SFBGSMAPSIZE);
		output.fill(-1);
		auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig.rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));
		if (rigPackage == nullptr) { return output; }

		for (int i = 0; i < output.size(); i++)
		{
			auto result = rig.GetBoneIndex(rigPackage->GetBoneNameFromKey(static_cast<BoneMapKey>(i)));
			if (result.has_value())
			{
				output.at(i) = static_cast<int16_t>(result.value());
			}
		}

		return output;
	}

	SFBGS_RigPackage::SFBGS_RigPackage(const SFBGS_RigPackage& source)
	{
		highPrecisionValue = source.highPrecisionValue;
		lowPrecisionValue = source.lowPrecisionValue;
		isMannequin = source.isMannequin;

		for (size_t i = 0; i < SFBGSMAPSIZE; i++)
		{
			rigMap[i] = source.rigMap[i];
		}

	}

	SFBGS_RigPackage::SFBGS_RigPackage(const SFBGS_RigPackage&& source) noexcept
	{
		highPrecisionValue = source.highPrecisionValue;
		lowPrecisionValue = source.lowPrecisionValue;
		isMannequin = source.isMannequin;

		for (size_t i = 0; i < SFBGSMAPSIZE; i++)
		{
			rigMap[i] = source.rigMap[i];
		}
	}

	SFBGS_RigPackage::~SFBGS_RigPackage()
	{

	}

	bool SFBGS_RigPackage::BoneIsMapped(const char* boneName) const
	{
		for (int i = 0; i < SFBGSMAPSIZE; i++)
		{
			if (strcmp(rigMap[i].c_str(), boneName))
			{
				return true;
			}
		}
		return false;
	}

	bool SFBGS_RigPackage::KeyIsMapped(BoneMapKey key)
	{
		if (!BoneTagExists(key) || static_cast<uint8_t>(key) >= SFBGSMAPSIZE)
			return false;

		return !rigMap[static_cast<uint8_t>(key)].Empty();
	}

	bool SFBGS_RigPackage::AddBoneToMap(BoneMapKey key, const char* boneName, bool overwrite)
	{
		if (!BoneTagExists(key) || static_cast<uint8_t>(key) >= SFBGSMAPSIZE)
			return false;

		if (BoneIsMapped(boneName) || KeyIsMapped(key))
		{
			if (!overwrite)
				return false;

			RemoveBoneFromMap(boneName);
		}

		rigMap[static_cast<uint8_t>(key)] = boneName;
		return true;
	}

	bool SFBGS_RigPackage::AddBoneToMap(BoneMapKey key, UNIV::SkeletonBone& bone, bool overwrite)
	{
		return AddBoneToMap(key, bone.name.c_str(), overwrite);
	}

	bool SFBGS_RigPackage::RemoveBoneFromMap(BoneMapKey key)
	{
		if (!BoneTagExists(key) || static_cast<uint8_t>(key) >= SFBGSMAPSIZE)
			return false;

		rigMap[static_cast<uint8_t>(key)].Clear();
		return true;
	}

	bool SFBGS_RigPackage::RemoveBoneFromMap(const char* boneName)
	{
		return	RemoveBoneFromMap(GetBoneKey(boneName));
	}

	SFBGS_RigPackage& SFBGS_RigPackage::operator=(const SFBGS_RigPackage& other)
	{
		lowPrecisionValue = other.lowPrecisionValue;
		highPrecisionValue = other.highPrecisionValue;
		
		for (int i = 0; i < SFBGSMAPSIZE; i++)
		{
			rigMap[i] = other.rigMap[i];
		}

		isMannequin = other.isMannequin;
		return *this;
	}

	BoneMapKey SFBGS_RigPackage::GetBoneKey(const char* boneName) const
	{
		for (int i = 0; i < SFBGSMAPSIZE; i++)
		{
			if (strcmp(rigMap[i].c_str(), boneName))
			{
				return (BoneMapKey)i;
			}
		}


		return BoneMapKey::None;
	}

	const char* SFBGS_RigPackage::GetBoneNameFromKey(BoneMapKey key)
	{
		if(!KeyIsMapped(key))
			return "";

		return rigMap[static_cast<uint8_t>(key)].c_str();
	}

	void SFBGS_RigPackage::SetPrecisionValues(PrecisionSet setting, float custom1, float custom2)
	{
		switch (setting)
		{
		case CALUMI::SFBGS::PrecisionSet::Default:
			highPrecisionValue = SFBGSDefaultPrecision.first;
			lowPrecisionValue = SFBGSDefaultPrecision.second;
			break;
		case CALUMI::SFBGS::PrecisionSet::FirstPerson:
			highPrecisionValue = SFBGSFirstPersonPrecision.first;
			lowPrecisionValue = SFBGSFirstPersonPrecision.second;
			break;
		case CALUMI::SFBGS::PrecisionSet::Ship:
			highPrecisionValue = SFBGSShipPrecision.first;
			lowPrecisionValue = SFBGSShipPrecision.second;
			break;
		case CALUMI::SFBGS::PrecisionSet::Custom:
			highPrecisionValue = custom1 < custom2 ? custom1 : custom2;
			lowPrecisionValue = highPrecisionValue == custom1 ? custom2 : custom1;
			break;
		default:
			break;
		}

	}


	// Inherited via RigPackage
	const char* SFBGS_RigPackage::GetPackageType() const { return SFBGS_RIG_PACKAGE; }

	Utilities::StringContainer SFBGS_RigPackage::ToJSON(size_t indents) const
	{
		return Utilities::StringContainer();
	}

	bool SFBGS_RigPackage::HandleBoneRename(const char* oldBone, const char* newName, size_t idx)
	{
		for (int i = 0; i < SFBGSMAPSIZE; i++)
		{
			if (rigMap[i].compare(oldBone), false)
			{
				rigMap[i] = newName;
				return true;
			}
		}

		return false;
	}

	UNIV::RigPackage* SFBGS_RigPackage::Clone()
	{
		return new SFBGS_RigPackage(*this);
	}



#pragma region ExternC
	bool SFBGSRigPackage_AddPackageToSkeletonRigC(UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage, bool overwrite)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->Clear();

		*errorMessageHolder += "[CALUMI.Animation API] ";
		*errorMessageHolder += SFBGS_RIG_PACKAGE;

		if (CreateNewSFBGSRigPackage(*rig, overwrite))
		{
			*errorMessageHolder += " Successfully Added To";
			*errorMessageHolder += rig->rigName;
			return true;
		}
		else
		{
			*errorMessageHolder += " Was Not Added To ";
			*errorMessageHolder += rig->rigName;
			*errorMessageHolder += ", It May Already Exist And Was Not Set To Overwrite";
		}
		return false;

	}
	bool SFBGSRigPackage_RemoveRigPackageFromSkeletonRigC(UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->Clear();

		*errorMessageHolder += "[CALUMI.Animation API] ";
		*errorMessageHolder += SFBGS_RIG_PACKAGE;

		if (RemoveSFBGSRigPackage(*rig))
		{	
			*errorMessageHolder += " Successfully Removed From ";
			*errorMessageHolder += rig->rigName;
			return true;
		}
		else
		{
			*errorMessageHolder += " Was Not Removed From ";
			*errorMessageHolder += rig->rigName;
			*errorMessageHolder += ", It Either Does Not Exist Or Is Mislabeled";
		}
		return false;
	}

	bool CALUMIANIMATION_API SFBGSRigPackage_BoneIsMappedC(UNIV::SkeletonRig* rig, const char* boneName)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return false;

		return rigPackage->BoneIsMapped(boneName);
	}

	bool SFBGSRigPackage_KeyIsMappedC(UNIV::SkeletonRig* rig, uint8_t key)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return false;

		return rigPackage->KeyIsMapped(static_cast<BoneMapKey>(key));
	}

	CALUMIANIMATION_API bool SFBGSRigPackage_AddBoneNameToMapC(UNIV::SkeletonRig* rig, uint8_t key, const char* boneName, Utilities::StringContainer* errorMessage, bool overwrite)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->Clear();
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));
		
		if (!rigPackage->AddBoneToMap(static_cast<BoneMapKey>(key), boneName, overwrite))
		{
			*errorMessageHolder += std::format("[CALUMI.Animation API] Bone: {} Could Not Be Added To SFBGS Rigmap On Rig: {}",boneName,rig->rigName.c_str()).c_str();
			return false;
		}

		return true;
	}

	bool SFBGSRigPackage_AddBoneToMapC(UNIV::SkeletonRig* rig, uint8_t key, UNIV::SkeletonBone* bone, Utilities::StringContainer* errorMessage, bool overwrite)
	{
		return SFBGSRigPackage_AddBoneNameToMapC(rig,key, bone->name.c_str(),errorMessage, overwrite);
	}

	bool SFBGSRigPackage_RemoveBoneFromMapUsingKeyC(UNIV::SkeletonRig* rig, uint8_t key)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return false;

		return rigPackage->RemoveBoneFromMap(static_cast<BoneMapKey>(key));
	}

	bool SFBGSRigPackage_RemoveBoneFromMapUsingNameC(UNIV::SkeletonRig* rig, const char* boneName)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return false;

		return rigPackage->RemoveBoneFromMap(boneName);
	}

	uint8_t SFBGSRigPackage_GetBoneKeyC(UNIV::SkeletonRig* rig, const char* boneName)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return -1;

		return static_cast<uint8_t>(rigPackage->GetBoneKey(boneName));
	}

	const char* SFBGSRigPackage_GetBoneNameFromKeyC(UNIV::SkeletonRig* rig, uint8_t key)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return "";

		return rigPackage->GetBoneNameFromKey(static_cast<BoneMapKey>(key));
	}

	bool SFBGSRigPackage_SetMannequinC(UNIV::SkeletonRig* rig, bool isMannequin)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return false;

		rigPackage->isMannequin = isMannequin;

		return true;
	}

	size_t SFBGSRigPackage_GetRigMapSize()
	{
		return SFBGSMAPSIZE;
	}

	void SFBGSRigPackage_SetPrecisionToDefaultC(UNIV::SkeletonRig* rig)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));
		if (rigPackage)
			rigPackage->SetPrecisionValues(PrecisionSet::Default);
	}

	void SFBGSRigPackage_SetPrecisionToFirstPersonC(UNIV::SkeletonRig* rig)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));
		if (rigPackage) 
			rigPackage->SetPrecisionValues(PrecisionSet::FirstPerson);
	}

	void SFBGSRigPackage_SetPrecisionToShipValuesC(UNIV::SkeletonRig* rig)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));
		if (rigPackage)
			rigPackage->SetPrecisionValues(PrecisionSet::Ship);
	}

	void SFBGSRigPackage_SetPrecisionToCustomC(UNIV::SkeletonRig* rig, float custom1, float custom2)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->rigPackageManager.GetPackage(SFBGS_RIG_PACKAGE));
		if (rigPackage)
			rigPackage->SetPrecisionValues(PrecisionSet::Custom, custom1, custom2);
	}

#pragma endregion

}
}