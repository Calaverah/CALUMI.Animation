//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include <algorithm>
#include <AnimStarfield>
#include "internalplatform.h"
#include <ranges>
#include <unordered_map>

#include "sfbgs/skeletonrig/SFBGS_SkeletonRig.h"
#include "univ/skeletonrig/packages/UNIV_RigManifestPackage.h"

namespace CALUMI::SFBGS
{

	bool SFBGS_RigPackage::BoneTagExists(const BoneMapKey input)
	{
		switch (input)
		{
		case BoneMapKey::None:
			return false;
		case BoneMapKey::Root:
		case BoneMapKey::AnimObjectA:
		case BoneMapKey::AnimObjectB:
		case BoneMapKey::AnimObjectC:
		case BoneMapKey::AnimObjectD:
		case BoneMapKey::Camera:
		case BoneMapKey::Camera_Control:
		case BoneMapKey::CamTargetParent:
		case BoneMapKey::CameraTarget:
		case BoneMapKey::COM:
		case BoneMapKey::C_Hips:
		case BoneMapKey::R_Thigh:
		case BoneMapKey::R_Calf:
		case BoneMapKey::R_Foot:
		case BoneMapKey::R_Toe:
		case BoneMapKey::L_Thigh:
		case BoneMapKey::L_Calf:
		case BoneMapKey::L_Foot:
		case BoneMapKey::L_Toe:
		case BoneMapKey::C_Spine:
		case BoneMapKey::C_Spine1:
		case BoneMapKey::C_Spine2:
		case BoneMapKey::C_Chest:
		case BoneMapKey::C_Neck:
		case BoneMapKey::C_Neck1:
		case BoneMapKey::C_Head:
		case BoneMapKey::L_Clavicle:
		case BoneMapKey::L_Biceps:
		case BoneMapKey::L_Forearm:
		case BoneMapKey::L_Wrist:
		case BoneMapKey::L_Thumb:
		case BoneMapKey::L_Thumb1:
		case BoneMapKey::L_Thumb2:
		case BoneMapKey::L_Cup:
		case BoneMapKey::L_Pinky:
		case BoneMapKey::L_Pinky1:
		case BoneMapKey::L_Pinky2:
		case BoneMapKey::L_Ring:
		case BoneMapKey::L_Ring1:
		case BoneMapKey::L_Ring2:
		case BoneMapKey::L_Middle:
		case BoneMapKey::L_Middle1:
		case BoneMapKey::L_Middle2:
		case BoneMapKey::L_Index:
		case BoneMapKey::L_Index1:
		case BoneMapKey::L_Index2:
		case BoneMapKey::L_AnimObject1:
		case BoneMapKey::L_AnimObject2:
		case BoneMapKey::L_AnimObject3:
		case BoneMapKey::L_Arm:
		case BoneMapKey::L_Elbow:
		case BoneMapKey::C_BackPack:
		case BoneMapKey::C_BackPackHose:
		case BoneMapKey::R_Clavicle:
		case BoneMapKey::R_Biceps:
		case BoneMapKey::R_Forearm:
		case BoneMapKey::R_Wrist:
		case BoneMapKey::R_Thumb:
		case BoneMapKey::R_Thumb1:
		case BoneMapKey::R_Thumb2:
		case BoneMapKey::R_Cup:
		case BoneMapKey::R_Pinky:
		case BoneMapKey::R_Pinky1:
		case BoneMapKey::R_Pinky2:
		case BoneMapKey::R_Ring:
		case BoneMapKey::R_Ring1:
		case BoneMapKey::R_Ring2:
		case BoneMapKey::R_Middle:
		case BoneMapKey::R_Middle1:
		case BoneMapKey::R_Middle2:
		case BoneMapKey::R_Index:
		case BoneMapKey::R_Index1:
		case BoneMapKey::R_Index2:
		case BoneMapKey::R_AnimObject1:
		case BoneMapKey::R_AnimObject2:
		case BoneMapKey::R_AnimObject3:
		case BoneMapKey::R_Arm:
		case BoneMapKey::R_Elbow:
		case BoneMapKey::Weapon:
		case BoneMapKey::Bolt01:
		case BoneMapKey::Bolt02:
		case BoneMapKey::WeaponLeft:
		case BoneMapKey::DirectAt:
		case BoneMapKey::C_Waist:
		case BoneMapKey::Camera_Control_FP:
		case BoneMapKey::R_Thigh_Twist:
		case BoneMapKey::R_Thigh_Twist1:
		case BoneMapKey::L_Thigh_Twist:
		case BoneMapKey::L_Thigh_Twist1:
		case BoneMapKey::C_Neck_Twist:
		case BoneMapKey::L_Wrist_Twist:
		case BoneMapKey::L_Wrist_Twist1:
		case BoneMapKey::L_Wrist_Twist2:
		case BoneMapKey::L_Biceps_Twist:
		case BoneMapKey::L_Biceps_Twist1:
		case BoneMapKey::R_Wrist_Twist:
		case BoneMapKey::R_Wrist_Twist1:
		case BoneMapKey::R_Wrist_Twist2:
		case BoneMapKey::R_Biceps_Twist:
		case BoneMapKey::R_Biceps_Twist1:
			return true;
		default:
			return false;

		}
	}

	bool SFBGS_RigPackage::AddPackage(const UNIV::SkeletonRig& rig, const bool overwrite)
	{
		const auto& mgr = rig.getPackageManager();

		auto* pkg = new SFBGS_RigPackage();

		if (mgr.addPackage(pkg, overwrite))
		{
			return true;
		}

		delete pkg;
		pkg = nullptr;
		return false;
	}

	bool SFBGS_RigPackage::RemovePackage(const UNIV::SkeletonRig& rig)
	{
		return rig.getPackageManager().removePackage(SFBGS_RIG_PACKAGE);
	}

	SFBGS_RigPackage& SFBGS_RigPackage::GetPackage(const UNIV::SkeletonRig& rig)
	{
		auto& mgr = rig.getPackageManager();
		if (const auto pkg = dynamic_cast<SFBGS_RigPackage*>(mgr.getPackage(SFBGS_RIG_PACKAGE)))
		{
			return *pkg;
		}

		AddPackage(rig, false);

		if (const auto pkg = dynamic_cast<SFBGS_RigPackage*>(mgr.getPackage(SFBGS_RIG_PACKAGE)))
		{
			return *pkg;
		}

		throw std::runtime_error("RigMirrorPackage::GetPackage() could not find nor add rig package.");
	}

	Utilities::S16Vector SFBGS_RigPackage::ConvertMap(const Utilities::StringList& boneList) const
	{
		Utilities::S16Vector output;
		output.resize(SFBGSMAPSIZE);

		for (int i = 0; i < output.size(); i++)
		{
			output.at(i) = -1;
		}

		for (int i = 0; i < output.size(); i++)
		{
			if (const auto result = boneList.find(boneNameFromKey(static_cast<BoneMapKey>(i)),-1); result >= 0
				&& result < std::numeric_limits<int16_t>::max())
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
		//Utilities::StringContainer rigMap[SFBGSMAPSIZE];
		std::unordered_map<BoneMapKey, std::string> _boneMap;
		Impl() = default;
	};

	SFBGS_RigPackage* SFBGS_RigPackage::clone() const
	{
		const auto output = new SFBGS_RigPackage(*this);
		return output;
	}

	bool SFBGS_RigPackage::isMannequin() const
	{
		return pImpl->isMannequin;
	}

	void SFBGS_RigPackage::setIsMannequin(const bool mqn) const
	{
		pImpl->isMannequin = mqn;
	}

	struct PrecisionSet::Impl {
		float _high;
		float _low;
	};

	PrecisionSet::PrecisionSet() : pImpl(new Impl()) {}

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
		if (this != &other)
		{
			pImpl->_high = other.pImpl->_high;
			pImpl->_low = other.pImpl->_low;
		}
		return *this;
	}

	bool PrecisionSet::operator==(const PrecisionSet& other) const
	{
		return pImpl->_high == other.pImpl->_high && pImpl->_low == other.pImpl->_low;
	}

	bool PrecisionSet::operator!=(const PrecisionSet& other) const
	{
		return !(*this == other);
	}

	PrecisionSet::PrecisionSet(const float value1, const float value2) : PrecisionSet()
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

	float PrecisionSet::high() const
	{
		return pImpl->_high;
	}
	float PrecisionSet::low() const
	{
		return pImpl->_low;
	}

	// 1/4000 and 1/32
	PrecisionSet PrecisionSet::DefaultPrecision()
	{
		return {0.00025f, 0.03125f};
	}

	// 1/16000 and 1/128
	PrecisionSet PrecisionSet::FirstPersonPrecision()
	{
		return {0.0000625f, 0.0078125f};
	}

	// 1/500 and 1/4
	PrecisionSet PrecisionSet::ShipPrecision()
	{
		return {0.002f, 0.25f};
	}

	const char* PrecisionSet::precisionType() const
	{
		if (*this == DefaultPrecision())
			return "Default";

		if (*this == FirstPersonPrecision())
			return "First Person";

		if (*this == ShipPrecision())
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
		if (this != &other)
		{
			pImpl->_precisionSet = other.pImpl->_precisionSet;
			pImpl->isMannequin = other.pImpl->isMannequin;
			pImpl->_boneMap = other.pImpl->_boneMap;
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
		return std::ranges::any_of(pImpl->_boneMap, [boneName](const auto& item)
		{
			return SCOMPARE(item.second.c_str(), boneName) == 0;
		});
	}

	bool SFBGS_RigPackage::keyIsMapped(const BoneMapKey key) const
	{
		return pImpl->_boneMap.contains(key);
	}

	bool SFBGS_RigPackage::addBoneToMap(const BoneMapKey key, const char* boneName, const bool overwrite) const
	{
		if (boneIsMapped(boneName) || keyIsMapped(key))
		{
			if (!overwrite)
				return false;
		}

		pImpl->_boneMap[key] = boneName;
		return true;
	}

	bool SFBGS_RigPackage::addBoneToMap(const BoneMapKey key, const UNIV::SkeletonBone& bone, const bool overwrite) const
	{
		return addBoneToMap(key, bone.name(), overwrite);
	}

	bool SFBGS_RigPackage::removeBoneFromMap(const BoneMapKey key) const
	{
		if (!pImpl->_boneMap.contains(key))
			return false;

		return pImpl->_boneMap.erase(key) > 0;
	}

	bool SFBGS_RigPackage::removeBoneFromMap(const char* boneName) const
	{
		return	removeBoneFromMap(boneKey(boneName));
	}

	SFBGS_RigPackage::BoneMapKey SFBGS_RigPackage::boneKey(const char* boneName) const
	{
		for (const auto& [key, bone] : pImpl->_boneMap)
		{
			if (SCOMPARE(bone.c_str(), boneName) == 0)
				return key;
		}
		return BoneMapKey::None;
	}

	const char* SFBGS_RigPackage::boneNameFromKey(const BoneMapKey key) const
	{
		if(!keyIsMapped(key))
			return "";

		return pImpl->_boneMap[key].c_str();
	}

	void SFBGS_RigPackage::setPrecisionValues(const PrecisionSet& setting) const
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
		return {};
	}

	bool SFBGS_RigPackage::handleBoneRename(const char* oldBone, const char* newName)
	{
		for (auto& bone : pImpl->_boneMap | std::views::values)
		{
			if (SCOMPARE(bone.c_str(),oldBone) == 0)
				bone = newName;
		}

		return false;
	}



#pragma region Extern"C"
	bool SFBGSRigPackage_AddPackageToSkeletonRigC(const UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage, const bool overwrite)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();

		*errorMessageHolder += "[CALUMI.Animation API] ";
		*errorMessageHolder += SFBGS_RIG_PACKAGE;

		if (SFBGS_RigPackage::AddPackage(*rig, overwrite))
		{
			*errorMessageHolder += " Successfully Added To";
			*errorMessageHolder += rig->name();
			return true;
		}

		*errorMessageHolder += " Was Not Added To ";
		*errorMessageHolder += rig->name();
		*errorMessageHolder += ", It May Already Exist And Was Not Set To Overwrite";

		return false;

	}
	bool SFBGSRigPackage_RemoveRigPackageFromSkeletonRigC(const UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();

		*errorMessageHolder += "[CALUMI.Animation API] ";
		*errorMessageHolder += SFBGS_RIG_PACKAGE;

		if (SFBGS_RigPackage::RemovePackage(*rig))
		{
			*errorMessageHolder += " Successfully Removed From ";
			*errorMessageHolder += rig->name();
			return true;
		}

		*errorMessageHolder += " Was Not Removed From ";
		*errorMessageHolder += rig->name();
		*errorMessageHolder += ", It Either Does Not Exist Or Is Mislabeled";

		return false;
	}

	bool SFBGSRigPackage_BoneIsMappedC(const UNIV::SkeletonRig* rig, const char* boneName)
	{
		if(const auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
			return rigPackage->boneIsMapped(boneName);

		return false;
	}

	bool SFBGSRigPackage_KeyIsMappedC(const UNIV::SkeletonRig* rig, uint8_t key)
	{
		if (const auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
			return rigPackage->keyIsMapped(static_cast<SFBGS_RigPackage::BoneMapKey>(key));

		return false;
	}

	bool SFBGSRigPackage_AddBoneNameToMapC(const UNIV::SkeletonRig* rig, uint8_t key, const char* boneName, Utilities::StringContainer* errorMessage, const bool overwrite)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();
		if(const auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
		{
			if (rigPackage->addBoneToMap(static_cast<SFBGS_RigPackage::BoneMapKey>(key), boneName, overwrite))
				return true;
		}

		*errorMessageHolder += std::format("[CALUMI.Animation API] Bone: {} Could Not Be Added To SFBGS Rig Map On Rig: {}", boneName, rig->name()).c_str();
		return false;

	}

	bool SFBGSRigPackage_AddBoneToMapC(const UNIV::SkeletonRig* rig, const uint8_t key, const UNIV::SkeletonBone* bone, Utilities::StringContainer* errorMessage, const bool overwrite)
	{
		return SFBGSRigPackage_AddBoneNameToMapC(rig,key, bone->name(), errorMessage, overwrite);
	}

	bool SFBGSRigPackage_RemoveBoneFromMapUsingKeyC(const UNIV::SkeletonRig* rig, uint8_t key)
	{
		const auto* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return false;

		return rigPackage->removeBoneFromMap(static_cast<SFBGS_RigPackage::BoneMapKey>(key));
	}

	bool SFBGSRigPackage_RemoveBoneFromMapUsingNameC(const UNIV::SkeletonRig* rig, const char* boneName)
	{
		const auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return false;

		return rigPackage->removeBoneFromMap(boneName);
	}

	uint8_t SFBGSRigPackage_GetBoneKeyC(const UNIV::SkeletonRig* rig, const char* boneName)
	{
		const auto* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage) return -1;

		return static_cast<uint8_t>(rigPackage->boneKey(boneName));
	}

	const char* SFBGSRigPackage_GetBoneNameFromKeyC(const UNIV::SkeletonRig* rig, uint8_t key)
	{
		const auto* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage)
			return "";

		return rigPackage->boneNameFromKey(static_cast<SFBGS_RigPackage::BoneMapKey>(key));
	}

	bool SFBGSRigPackage_SetMannequinC(const UNIV::SkeletonRig* rig, const bool isMannequin)
	{
		const auto* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE));

		if (!rigPackage)
			return false;

		rigPackage->setIsMannequin(isMannequin);

		return true;
	}

	bool SFBGSRigPackage_IsMannequinC(const UNIV::SkeletonRig* rig)
	{
		if (const auto* rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
			return rigPackage->isMannequin();

		return false;
	}

	uint64_t SFBGSRigPackage_GetRigMapSize()
	{
		return SFBGSMAPSIZE;
	}

	void SFBGSRigPackage_SetPrecisionToDefaultC(const UNIV::SkeletonRig* rig)
	{
		if (const auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
			rigPackage->setPrecisionValues(PrecisionSet::DefaultPrecision());
	}

	void SFBGSRigPackage_SetPrecisionToFirstPersonC(const UNIV::SkeletonRig* rig)
	{
		if (const auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
			rigPackage->setPrecisionValues(PrecisionSet::FirstPersonPrecision());
	}

	void SFBGSRigPackage_SetPrecisionToShipValuesC(const UNIV::SkeletonRig* rig)
	{
		if (const auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
			rigPackage->setPrecisionValues(PrecisionSet::ShipPrecision());
	}

	void SFBGSRigPackage_SetPrecisionToCustomC(const UNIV::SkeletonRig* rig, const float custom1, const float custom2)
	{
		if (const auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
			rigPackage->setPrecisionValues(PrecisionSet(custom1, custom2));
	}

	const char* SFBGSRigPackage_GetPrecisionType(const UNIV::SkeletonRig* rig)
	{
		if (const auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
			return rigPackage->precisionSet().precisionType();

		return nullptr;
	}

	float SFBGSRigPackage_GetHighPrecisionValueC(const UNIV::SkeletonRig* rig)
	{
		if (const auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
			return rigPackage->precisionSet().high();

		return PrecisionSet::DefaultPrecision().high();
	}

	float SFBGSRigPackage_GetLowPrecisionValueC(const UNIV::SkeletonRig* rig)
	{
		if (const auto rigPackage = dynamic_cast<SFBGS_RigPackage*>(rig->getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
			return rigPackage->precisionSet().low();

		return PrecisionSet::DefaultPrecision().low();
	}

#pragma endregion

}
