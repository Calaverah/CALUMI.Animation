//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include <AnimStarfield>
#include "internalplatform.h"
#include <cmath>
#include "sfbgs/skeletonrig/SFBGS_SkeletonRig.h"

namespace CALUMI {namespace SFBGS{

    bool SFBGS_RigPackage::BoneTagExists(BoneMapKey input)
	{
		switch (input)
		{
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::None:
			return false;
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::Root:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::AnimObjectA:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::AnimObjectB:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::AnimObjectC:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::AnimObjectD:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::Camera:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::Camera_Control:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::CamTargetParent:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::CameraTarget:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::COM:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_Hips:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Thigh:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Calf:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Foot:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Toe:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Thigh:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Calf:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Foot:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Toe:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_Spine:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_Spine1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_Spine2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_Chest:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_Neck:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_Neck1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_Head:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Clavicle:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Biceps:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Forearm:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Wrist:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Thumb:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Thumb1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Thumb2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Cup:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Pinky:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Pinky1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Pinky2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Ring:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Ring1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Ring2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Middle:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Middle1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Middle2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Index:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Index1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Index2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_AnimObject1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_AnimObject2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_AnimObject3:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Arm:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Elbow:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_BackPack:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_BackPackHose:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Clavicle:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Biceps:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Forearm:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Wrist:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Thumb:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Thumb1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Thumb2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Cup:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Pinky:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Pinky1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Pinky2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Ring:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Ring1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Ring2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Middle:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Middle1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Middle2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Index:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Index1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Index2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_AnimObject1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_AnimObject2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_AnimObject3:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Arm:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Elbow:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::Weapon:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::Bolt01:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::Bolt02:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::WeaponLeft:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::DirectAt:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_Waist:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::Camera_Control_FP:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Thigh_Twist:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Thigh_Twist1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Thigh_Twist:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Thigh_Twist1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::C_Neck_Twist:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Wrist_Twist:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Wrist_Twist1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Wrist_Twist2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Biceps_Twist:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::L_Biceps_Twist1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Wrist_Twist:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Wrist_Twist1:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Wrist_Twist2:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Biceps_Twist:
		case CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::R_Biceps_Twist1:
			return true;
		default:
			return false;

		}
	}

	bool SFBGS_RigPackage::CreateNewSFBGSRigPackage(UNIV::SkeletonRig& rig, bool overwrite)
	{
		auto& mgr = rig.getPackageManager();

		SFBGS_RigPackage* pkg = new SFBGS_RigPackage();

		if (mgr.AddPackage(pkg, overwrite))
		{
			return true;
		}

		if (pkg)
		{
			delete pkg;
			pkg = nullptr;
		}

		return false;
	}

	bool SFBGS_RigPackage::RemoveSFBGSRigPackage(UNIV::SkeletonRig& rig)
	{
		return rig.getPackageManager().RemovePackage(SFBGS_RIG_PACKAGE);
	}

	Utilities::S16Vector SFBGS_RigPackage::ConvertSFBGSRigPackage(const UNIV::SkeletonRig& rig)
	{
		Utilities::S16Vector output;
		output.resize(SFBGSMAPSIZE);

		for (int i = 0; i < output.size(); i++) 
		{ 
			output.at(i) = -1; 
		}

		auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig.getPackageManager().getPackage(SFBGS_RIG_PACKAGE));
		
		if (rigPackage == nullptr) 
		{ 
			return output; 
		}

		for (int i = 0; i < output.size(); i++)
		{
            auto result = rig.boneIndex(rigPackage->boneNameFromKey(static_cast<SFBGS_RigPackage::BoneMapKey>(i)));
			if (result >= 0)
			{
				output.at(i) = static_cast<int16_t>(result);
			}
		}

		return output;
	}

	struct SFBGS_RigPackage::Impl
	{
		PrecisionSet _precisionSet = PrecisionSet::DefaultPrecision();
		bool isMannequin = false;
		Utilities::StringContainer rigMap[SFBGSMAPSIZE];
		Impl() = default;
	};

    SFBGS_RigPackage* SFBGS_RigPackage::clone() const
	{
		SFBGS_RigPackage* output = new SFBGS_RigPackage(*this);
		return output;
	}

    bool SFBGS_RigPackage::isMannequin() const
	{
		return pImpl->isMannequin;
	}

    void SFBGS_RigPackage::setIsMannequin(bool mqn)
	{
		pImpl->isMannequin = mqn;
	}

	struct PrecisionSet::Impl {
		float _high;
		float _low;
	};

	PrecisionSet::PrecisionSet()
	{
		pImpl = new Impl();
	}
	PrecisionSet::~PrecisionSet()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}

	PrecisionSet& PrecisionSet::operator=(const PrecisionSet& other)
	{
		pImpl->_high = other.pImpl->_high;
		pImpl->_low = other.pImpl->_low;

		return *this;
	}

	bool PrecisionSet::operator==(const PrecisionSet& other)
	{
		return pImpl->_high == other.pImpl->_high && pImpl->_low == other.pImpl->_low;
	}

	bool PrecisionSet::operator!=(const PrecisionSet& other)
	{
		return !(*this == other);
	}

	PrecisionSet::PrecisionSet(float value1, float value2) : PrecisionSet()
	{
		if (value1 == 0.0f || value2 == 0.0f || std::isnan(value1) || std::isnan(value2))
		{
			pImpl->_high = DefaultPrecision().high();
			pImpl->_low = DefaultPrecision().low();
		}

		pImpl->_high = std::min(std::abs(value1), std::abs(value2));
		pImpl->_low = std::max(std::abs(value1), std::abs(value2));
	}

	PrecisionSet::PrecisionSet(const PrecisionSet& other) : PrecisionSet() { *this = other; }

	float PrecisionSet::high() 
	{
		return pImpl->_high;
	}
	float PrecisionSet::low()
	{
		return pImpl->_low;
	}

	// 1/4000 and 1/32
	PrecisionSet PrecisionSet::DefaultPrecision()
	{
		return PrecisionSet(0.00025f, 0.03125f);
	}

	// 1/16000 and 1/128
	PrecisionSet PrecisionSet::FirstPersonPrecision()
	{
		return PrecisionSet(0.0000625f, 0.0078125f);
	}

	// 1/500 and 1/4
	PrecisionSet PrecisionSet::ShipPrecision()
	{
		return PrecisionSet(0.002f, 0.25f);
	}

	const char* PrecisionSet::precisionType()
	{
		if (*this == PrecisionSet::DefaultPrecision())
			return "Default";

		if (*this == PrecisionSet::FirstPersonPrecision())
			return "First Person";

		if (*this == PrecisionSet::ShipPrecision())
			return "Ship";

		return "Custom";
	}

	SFBGS_RigPackage::SFBGS_RigPackage()
	{
		pImpl = new Impl;
	}

	SFBGS_RigPackage::SFBGS_RigPackage(const SFBGS_RigPackage& source) : SFBGS_RigPackage()
	{
		*this = source;
	}

	SFBGS_RigPackage& SFBGS_RigPackage::operator=(const SFBGS_RigPackage& other)
	{
		pImpl->_precisionSet = other.pImpl->_precisionSet;
		pImpl->isMannequin = other.pImpl->isMannequin;
		for (uint16_t i = 0; i < SFBGSMAPSIZE; i++)
		{
			pImpl->rigMap[i] = other.pImpl->rigMap[i];
		}
		return *this;
	}

	SFBGS_RigPackage::~SFBGS_RigPackage()
	{
		if (pImpl) 
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}

    bool SFBGS_RigPackage::boneIsMapped(const char* boneName) const
	{
		for (int i = 0; i < SFBGSMAPSIZE; i++)
		{
			if (SCOMPARE(pImpl->rigMap[i].c_str(), boneName)==0)
			{
				return true;
			}
		}
		return false;
	}

    bool SFBGS_RigPackage::keyIsMapped(BoneMapKey key)
	{
        if (!BoneTagExists(key) || static_cast<uint8_t>(key) >= SFBGSMAPSIZE)
			return false;

		return !pImpl->rigMap[static_cast<uint8_t>(key)].empty();
	}

    bool SFBGS_RigPackage::addBoneToMap(BoneMapKey key, const char* boneName, bool overwrite)
	{
        if (!BoneTagExists(key) || static_cast<uint8_t>(key) >= SFBGSMAPSIZE)
			return false;

        if (boneIsMapped(boneName) || keyIsMapped(key))
		{
			if (!overwrite)
				return false;

            removeBoneFromMap(boneName);
		}

		pImpl->rigMap[static_cast<uint8_t>(key)] = boneName;
		return true;
	}

    bool SFBGS_RigPackage::addBoneToMap(BoneMapKey key, UNIV::SkeletonBone& bone, bool overwrite)
	{
        return addBoneToMap(key, bone.name().c_str(), overwrite);
	}

    bool SFBGS_RigPackage::removeBoneFromMap(BoneMapKey key)
	{
        if (!BoneTagExists(key) || static_cast<uint8_t>(key) >= SFBGSMAPSIZE)
			return false;

		pImpl->rigMap[static_cast<uint8_t>(key)].clear();
		return true;
	}

    bool SFBGS_RigPackage::removeBoneFromMap(const char* boneName)
	{
        return	removeBoneFromMap(boneKey(boneName));
	}

    SFBGS_RigPackage::BoneMapKey SFBGS_RigPackage::boneKey(const char* boneName) const
	{
		for (int i = 0; i < SFBGSMAPSIZE; i++)
		{
			if (SCOMPARE(pImpl->rigMap[i].c_str(), boneName)==0)
			{
				return (BoneMapKey)i;
			}
		}
		return BoneMapKey::None;
	}

    const char* SFBGS_RigPackage::boneNameFromKey(BoneMapKey key)
	{
        if(!keyIsMapped(key))
			return "";

		return pImpl->rigMap[static_cast<uint8_t>(key)].c_str();
	}

    void SFBGS_RigPackage::setPrecisionValues(PrecisionSet setting)
	{
		pImpl->_precisionSet = setting;
	}

    PrecisionSet SFBGS_RigPackage::precisionSet() const
	{
		return pImpl->_precisionSet;
	}

	// Inherited via IRigPackage
	const char* SFBGS_RigPackage::getPackageType() const { return SFBGS_RIG_PACKAGE; }

    Utilities::StringContainer SFBGS_RigPackage::toJSON(uint64_t indents) const
	{
		return Utilities::StringContainer();
	}

    bool SFBGS_RigPackage::handleBoneRename(const char* oldBone, const char* newName, uint64_t idx)
	{
		for (int i = 0; i < SFBGSMAPSIZE; i++)
		{
			if (pImpl->rigMap[i].compare(oldBone, false))
			{
				pImpl->rigMap[i] = newName;
				return true;
			}
		}

		return false;
	}



#pragma region Extern"C"
	bool SFBGSRigPackage_AddPackageToSkeletonRigC(UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage, bool overwrite)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();

		*errorMessageHolder += "[CALUMI.Animation API] ";
		*errorMessageHolder += SFBGS_RIG_PACKAGE;

		if (SFBGS_RigPackage::CreateNewSFBGSRigPackage(*rig, overwrite))
		{
			*errorMessageHolder += " Successfully Added To";
            *errorMessageHolder += rig->rigName();
			return true;
		}
		else
		{
			*errorMessageHolder += " Was Not Added To ";
            *errorMessageHolder += rig->rigName();
			*errorMessageHolder += ", It May Already Exist And Was Not Set To Overwrite";
		}
		return false;

	}
	bool SFBGSRigPackage_RemoveRigPackageFromSkeletonRigC(UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();

		*errorMessageHolder += "[CALUMI.Animation API] ";
		*errorMessageHolder += SFBGS_RIG_PACKAGE;

		if (SFBGS_RigPackage::RemoveSFBGSRigPackage(*rig))
		{	
			*errorMessageHolder += " Successfully Removed From ";
            *errorMessageHolder += rig->rigName();
			return true;
		}
		else
		{
			*errorMessageHolder += " Was Not Removed From ";
            *errorMessageHolder += rig->rigName();
			*errorMessageHolder += ", It Either Does Not Exist Or Is Mislabeled";
		}
		return false;
	}

	bool SFBGSRigPackage_BoneIsMappedC(UNIV::SkeletonRig* rig, const char* boneName)
	{
		if(SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
            return rigPackage->boneIsMapped(boneName);

		return false;
	}

	bool SFBGSRigPackage_KeyIsMappedC(UNIV::SkeletonRig* rig, uint8_t key)
	{
		if (SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
            return rigPackage->keyIsMapped(static_cast<SFBGS_RigPackage::BoneMapKey>(key));

		return false;
	}

	bool SFBGSRigPackage_AddBoneNameToMapC(UNIV::SkeletonRig* rig, uint8_t key, const char* boneName, Utilities::StringContainer* errorMessage, bool overwrite)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();
		if(SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
		{
            if (rigPackage->addBoneToMap(static_cast<SFBGS_RigPackage::BoneMapKey>(key), boneName, overwrite))
				return true;
		}

        *errorMessageHolder += std::format("[CALUMI.Animation API] Bone: {} Could Not Be Added To SFBGS Rigmap On Rig: {}", boneName, rig->rigName().c_str()).c_str();
		return false;
		
	}

	bool SFBGSRigPackage_AddBoneToMapC(UNIV::SkeletonRig* rig, uint8_t key, UNIV::SkeletonBone* bone, Utilities::StringContainer* errorMessage, bool overwrite)
	{
        return SFBGSRigPackage_AddBoneNameToMapC(rig,key, bone->name().c_str(), errorMessage, overwrite);
	}

	bool SFBGSRigPackage_RemoveBoneFromMapUsingKeyC(UNIV::SkeletonRig* rig, uint8_t key)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return false;

        return rigPackage->removeBoneFromMap(static_cast<SFBGS_RigPackage::BoneMapKey>(key));
	}

	bool SFBGSRigPackage_RemoveBoneFromMapUsingNameC(UNIV::SkeletonRig* rig, const char* boneName)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return false;

        return rigPackage->removeBoneFromMap(boneName);
	}

	uint8_t SFBGSRigPackage_GetBoneKeyC(UNIV::SkeletonRig* rig, const char* boneName)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return -1;

        return static_cast<uint8_t>(rigPackage->boneKey(boneName));
	}

	const char* SFBGSRigPackage_GetBoneNameFromKeyC(UNIV::SkeletonRig* rig, uint8_t key)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return "";

        return rigPackage->boneNameFromKey(static_cast<SFBGS_RigPackage::BoneMapKey>(key));
	}

	bool SFBGSRigPackage_SetMannequinC(UNIV::SkeletonRig* rig, bool isMannequin)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return false;

        rigPackage->setIsMannequin(isMannequin);

		return true;
	}

	bool SFBGSRigPackage_IsMannequinC(UNIV::SkeletonRig* rig)
	{
		SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE));

		if (rigPackage)
            return rigPackage->isMannequin();

		return false;
	}

	uint64_t SFBGSRigPackage_GetRigMapSize()
	{
		return SFBGSMAPSIZE;
	}

	void SFBGSRigPackage_SetPrecisionToDefaultC(UNIV::SkeletonRig* rig)
	{
		if (SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
            rigPackage->setPrecisionValues(PrecisionSet::DefaultPrecision());
	}

	void SFBGSRigPackage_SetPrecisionToFirstPersonC(UNIV::SkeletonRig* rig)
	{
		if (SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
            rigPackage->setPrecisionValues(PrecisionSet::FirstPersonPrecision());
	}

	void SFBGSRigPackage_SetPrecisionToShipValuesC(UNIV::SkeletonRig* rig)
	{
		if (SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
            rigPackage->setPrecisionValues(PrecisionSet::ShipPrecision());
	}

	void SFBGSRigPackage_SetPrecisionToCustomC(UNIV::SkeletonRig* rig, float custom1, float custom2)
	{
		if (SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
            rigPackage->setPrecisionValues(PrecisionSet(custom1, custom2));
	}

	const char* SFBGSRigPackage_GetPrecisionType(UNIV::SkeletonRig* rig)
	{
		if (SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))	
            return rigPackage->precisionSet().precisionType();

		return nullptr;
	}

	float SFBGSRigPackage_GetHighPrecisionValueC(UNIV::SkeletonRig* rig)
	{
		if (SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
            return rigPackage->precisionSet().high();

		return PrecisionSet::DefaultPrecision().high();
	}

	float SFBGSRigPackage_GetLowPrecisionValueC(UNIV::SkeletonRig* rig)
	{
		if (SFBGS_RigPackage* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
            return rigPackage->precisionSet().low();

		return PrecisionSet::DefaultPrecision().low();
	}

#pragma endregion

}
}