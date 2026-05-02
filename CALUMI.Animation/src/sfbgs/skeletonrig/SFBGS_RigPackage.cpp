//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include <algorithm>
#include <AnimStarfield>
#include <cmath>

#include "internalplatform.h"
#include <ranges>
#include <string>
#include <unordered_map>

#include "sfbgs/skeletonrig/SFBGS_SkeletonRig.h"
#include "univ/skeletonrig/packages/UNIV_RigManifestPackage.h"

namespace CALUMI::SFBGS
{
	SFBGS_RigPackage::BoneMapKey SFBGS_RigPackage::GetBoneTag(const uint8_t input)
	{
		if (input > MaxBoneMapKey)
			return BoneMapKey::None;

		return static_cast<BoneMapKey>(input);
	}

	bool SFBGS_RigPackage::AddPackage(const UNIV::SkeletonRig& rig, const bool overwrite)
	{
		const auto& mgr = rig.packageManager();

		auto pkg = new SFBGS_RigPackage();

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
		return rig.packageManager().removePackage(SFBGS_RIG_PACKAGE);
	}

	SFBGS_RigPackage& SFBGS_RigPackage::GetPackage(const UNIV::SkeletonRig& rig)
	{
		auto& mgr = rig.packageManager();
		if (const auto pkg = dynamic_cast<SFBGS_RigPackage*>(mgr.package(SFBGS_RIG_PACKAGE)))
		{
			return *pkg;
		}

		AddPackage(rig, false);

		if (const auto pkg = dynamic_cast<SFBGS_RigPackage*>(mgr.package(SFBGS_RIG_PACKAGE)))
		{
			return *pkg;
		}

		throw std::runtime_error("RigMirrorPackage::GetPackage() could not find nor add rig package.");
	}

	Utilities::S16Vector SFBGS_RigPackage::convertMap(const Utilities::StringList& boneList) const
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
		PrecisionSet m_precisionSet = PrecisionSet::DefaultPrecision();
		bool m_isMannequin = false;
		std::unordered_map<BoneMapKey, std::string> m_boneMap;
		std::unordered_map<std::string, LODSetting> m_boneLODs;
		Impl() = default;
	};

	Utilities::JsonObject SFBGS_RigPackage::toJson() const
	{
		Utilities::JsonObject output;

		output["mannequin"] = pImpl->m_isMannequin;

		const Utilities::JsonObject boneMap;
		for (const auto& [keyEnum, boneName] : pImpl->m_boneMap)
		{
			boneMap[std::to_string(static_cast<uint8_t>(keyEnum)).c_str()] = boneName.c_str();
		}
		output["map"] = boneMap;

		const Utilities::JsonObject boneLODs;
		for (const auto& [key, lodEnum] : pImpl->m_boneLODs)
		{
			boneLODs[key.c_str()] = static_cast<int8_t>(lodEnum);
		}
		output["lod"] = boneLODs;

		output["precision"] = pImpl->m_precisionSet.toJson();

		return output;
	}

	SFBGS_RigPackage* SFBGS_RigPackage::clone() const
	{
		const auto output = new SFBGS_RigPackage(*this);
		return output;
	}

	bool SFBGS_RigPackage::isMannequin() const
	{
		return pImpl->m_isMannequin;
	}

	void SFBGS_RigPackage::setIsMannequin(const bool mqn) const
	{
		pImpl->m_isMannequin = mqn;
	}

	struct PrecisionSet::Impl {
		float m_high;
		float m_low;
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
			pImpl->m_high = other.pImpl->m_high;
			pImpl->m_low = other.pImpl->m_low;
		}
		return *this;
	}

	bool PrecisionSet::operator==(const PrecisionSet& other) const
	{
		return pImpl->m_high == other.pImpl->m_high && pImpl->m_low == other.pImpl->m_low;
	}

	bool PrecisionSet::operator!=(const PrecisionSet& other) const
	{
		return !(*this == other);
	}

	PrecisionSet::PrecisionSet(const float value1, const float value2) : PrecisionSet()
	{
		if (value1 == 0.0f || value2 == 0.0f || std::isnan(value1) || std::isnan(value2))
		{
			pImpl->m_high = DefaultPrecision().high();
			pImpl->m_low = DefaultPrecision().low();
		}

		pImpl->m_high = std::min(std::abs(value1), std::abs(value2));
		pImpl->m_low = std::max(std::abs(value1), std::abs(value2));
	}

	PrecisionSet::PrecisionSet(const PrecisionSet& other) : PrecisionSet() { *this = other; }

	float PrecisionSet::high() const
	{
		return pImpl->m_high;
	}
	float PrecisionSet::low() const
	{
		return pImpl->m_low;
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

	Utilities::JsonObject PrecisionSet::toJson() const
	{
		Utilities::JsonObject output;

		output["high"] = pImpl->m_high;
		output["low"] = pImpl->m_low;

		return output;
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
			pImpl->m_precisionSet = other.pImpl->m_precisionSet;
			pImpl->m_isMannequin = other.pImpl->m_isMannequin;
			pImpl->m_boneMap = other.pImpl->m_boneMap;
			pImpl->m_boneLODs = other.pImpl->m_boneLODs;
		}
		return *this;
	}

	SFBGS_RigPackage::LODSetting SFBGS_RigPackage::IntToLOD(int value)
	{
		if (value >= 0 && value <= static_cast<int>(MaxLOD))
			return static_cast<LODSetting>(value);

		return LODSetting::UNDEFINED;
	}

	void SFBGS_RigPackage::setBoneLod(const char* boneName, const LODSetting level) const
	{
		if (SCOMPARE(boneName, "") == 0)
			return;

		pImpl->m_boneLODs[boneName] = level;
	}

	SFBGS_RigPackage::LODSetting SFBGS_RigPackage::boneLod(const char* boneName) const
	{
		if (SCOMPARE(boneName, "") == 0)
			return DefaultLOD;

		if (!pImpl->m_boneLODs.contains(boneName))
			pImpl->m_boneLODs[boneName] = DefaultLOD;

		return pImpl->m_boneLODs[boneName];
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
		return std::ranges::any_of(pImpl->m_boneMap, [boneName](const auto& item)
		{
			return SCOMPARE(item.second.c_str(), boneName) == 0;
		});
	}

	bool SFBGS_RigPackage::keyIsMapped(const BoneMapKey key) const
	{
		return pImpl->m_boneMap.contains(key);
	}

	bool SFBGS_RigPackage::addBoneToMap(const BoneMapKey key, const char* boneName, const bool overwrite) const
	{
		if (boneIsMapped(boneName) || keyIsMapped(key))
		{
			if (!overwrite)
				return false;
		}

		pImpl->m_boneMap[key] = boneName;
		return true;
	}

	bool SFBGS_RigPackage::addBoneToMap(const BoneMapKey key, const UNIV::SkeletonBone& bone, const bool overwrite) const
	{
		return addBoneToMap(key, bone.name(), overwrite);
	}

	bool SFBGS_RigPackage::removeBoneFromMap(const BoneMapKey key) const
	{
		if (!pImpl->m_boneMap.contains(key))
			return false;

		return pImpl->m_boneMap.erase(key) > 0;
	}

	bool SFBGS_RigPackage::removeBoneFromMap(const char* boneName) const
	{
		return	removeBoneFromMap(boneKey(boneName));
	}

	SFBGS_RigPackage::BoneMapKey SFBGS_RigPackage::boneKey(const char* boneName) const
	{
		for (const auto& [key, bone] : pImpl->m_boneMap)
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

		return pImpl->m_boneMap[key].c_str();
	}

	void SFBGS_RigPackage::setPrecisionValues(const PrecisionSet& setting) const
	{
		pImpl->m_precisionSet = setting;
	}

	PrecisionSet SFBGS_RigPackage::precisionSet() const
	{
		return pImpl->m_precisionSet;
	}

	// Inherited via IRigPackage
	const char* SFBGS_RigPackage::packageType() const { return SFBGS_RIG_PACKAGE; }

	bool SFBGS_RigPackage::handleBoneRename(const char* oldBone, const char* newName)
	{
		for (auto& bone : pImpl->m_boneMap | std::views::values)
		{
			if (SCOMPARE(bone.c_str(),oldBone) == 0)
				bone = newName;
		}

		for (auto& [key, setting] : pImpl->m_boneLODs)
		{
			if (SCOMPARE(key.c_str(), oldBone) == 0)
			{
				pImpl->m_boneLODs[newName] = setting;
				pImpl->m_boneLODs.erase(key);
				break;
			}

		}

		return true;
	}
}


#pragma region Extern"C"
	int SFBGSRigPackage_AddPackageToSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig, const bool overwrite)
	{
		if (rig)
			try
			{
				return CALUMI::SFBGS::SFBGS_RigPackage::AddPackage(*rig, overwrite) ? 0 : 1;
			}
			catch ( const std::exception& ){}

		return -1;
	}
	int SFBGSRigPackage_RemoveRigPackageFromSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig)
	{
		if (rig)
			try
			{
				return CALUMI::SFBGS::SFBGS_RigPackage::RemovePackage(*rig) ? 0 : 1;
			}
			catch ( const std::exception&){}
		return -1;
	}

	int SFBGSRigPackage_BoneIsMappedC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName)
	{
		if (rig)
			try
			{
				return CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).boneIsMapped(boneName) ? 1 : 0;
			}
			catch ( const std::exception& ){}

		return -1;
	}

	int SFBGSRigPackage_KeyIsMappedC(const CALUMI::UNIV::SkeletonRig* rig, uint8_t key)
	{
		if (rig)
			try
			{
				return CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).keyIsMapped(static_cast<CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey>(key)) ? 1 : 0;
			}
			catch ( const std::exception& ){}

		return -1;
	}

	int SFBGSRigPackage_AddBoneNameToMapC(const CALUMI::UNIV::SkeletonRig* rig, uint8_t key, const char* boneName, const bool overwrite)
	{
		if (rig)
			try
			{
				return CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).addBoneToMap(static_cast<CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey>(key), boneName, overwrite) ? 0 : 1;
			}
			catch ( const std::exception& ){}

		return -1;
	}

	int SFBGSRigPackage_AddBoneToMapC(const CALUMI::UNIV::SkeletonRig* rig, const uint8_t key,
		const CALUMI::UNIV::SkeletonBone* bone, const bool overwrite)
	{
		return SFBGSRigPackage_AddBoneNameToMapC(rig,key, bone->name(), overwrite);
	}

	int SFBGSRigPackage_RemoveBoneFromMapUsingKeyC(const CALUMI::UNIV::SkeletonRig* rig, uint8_t key)
	{
		if (rig)
			try
			{
				return CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).removeBoneFromMap(static_cast<CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey>(key)) ? 0 : 1;
			}
			catch ( const std::exception& ){}

		return -1;
	}

	int SFBGSRigPackage_RemoveBoneFromMapUsingNameC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName)
	{
		if (rig)
			try
			{
				return CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).removeBoneFromMap(boneName) ? 0 : 1;
			}
			catch ( const std::exception& ){}

		return -1;

	}

	uint8_t SFBGSRigPackage_GetBoneKeyC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName)
	{
		if (rig)
			try
			{
				return static_cast<uint8_t>(CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).boneKey(boneName));
			}
			catch ( const std::exception& ){}

		return -1;
	}

	const char* SFBGSRigPackage_GetBoneNameFromKeyC(const CALUMI::UNIV::SkeletonRig* rig, const uint8_t key)
	{
		if (rig)
			try
			{
				const auto boneMapKey = CALUMI::SFBGS::SFBGS_RigPackage::GetBoneTag(key);

				if (boneMapKey == CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey::None)
					return "";

				return CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).boneNameFromKey(boneMapKey);
			}
			catch ( const std::exception& ){}

		return nullptr;

	}

	int SFBGSRigPackage_SetMannequinC(const CALUMI::UNIV::SkeletonRig* rig, const bool isMannequin)
	{
		if (rig)
			try
			{
				CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).setIsMannequin(isMannequin);
				return 0;
			}
			catch ( const std::exception& ){}

		return -1;
	}

	int SFBGSRigPackage_IsMannequinC(const CALUMI::UNIV::SkeletonRig* rig)
	{
		if (rig)
			try
			{
				return CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).isMannequin() ? 1 : 0;
			}
			catch ( const std::exception& ){}

		return -1;
	}

	uint64_t SFBGSRigPackage_GetRigMapSize()
	{
		return CALUMI::SFBGS::SFBGSMAPSIZE;
	}

	int SFBGSRigPackage_SetPrecisionToDefaultC(const CALUMI::UNIV::SkeletonRig* rig)
	{
		if (rig)
			try
			{
				CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).setPrecisionValues(CALUMI::SFBGS::PrecisionSet::DefaultPrecision());
				return 0;
			}
			catch ( const std::exception& ){}

		return -1;
	}

	int SFBGSRigPackage_SetPrecisionToFirstPersonC(const CALUMI::UNIV::SkeletonRig* rig)
	{
		if (rig)
			try
			{
				CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).setPrecisionValues(CALUMI::SFBGS::PrecisionSet::FirstPersonPrecision());
				return 0;
			}
		catch ( const std::exception& ){}

		return -1;
	}

	int SFBGSRigPackage_SetPrecisionToShipValuesC(const CALUMI::UNIV::SkeletonRig* rig)
	{
		if (rig)
			try
			{
				CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).setPrecisionValues(CALUMI::SFBGS::PrecisionSet::ShipPrecision());
				return 0;
			}
			catch ( const std::exception& ){}

		return -1;
	}

	int SFBGSRigPackage_SetPrecisionToCustomC(const CALUMI::UNIV::SkeletonRig* rig, const float custom1, const float custom2)
	{
		if (rig)
			try
			{
				CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).setPrecisionValues(CALUMI::SFBGS::PrecisionSet(custom1, custom2));
				return 0;
			}
		catch ( const std::exception& ){}

		return -1;
	}

	const char* SFBGSRigPackage_GetPrecisionType(const CALUMI::UNIV::SkeletonRig* rig)
	{
		if (rig)
			try
			{
				return CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).precisionSet().precisionType();
			}
			catch ( const std::exception& ){}

		return nullptr;
	}

	float SFBGSRigPackage_GetHighPrecisionValueC(const CALUMI::UNIV::SkeletonRig* rig)
	{
		if (rig)
			try
			{
				return CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).precisionSet().high();
			}
			catch ( const std::exception& ){}

		return std::numeric_limits<float>::quiet_NaN();
	}

	float SFBGSRigPackage_GetLowPrecisionValueC(const CALUMI::UNIV::SkeletonRig* rig)
	{
		if (rig)
			try
			{
				return CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).precisionSet().low();
			}
			catch ( const std::exception& ){}

		return std::numeric_limits<float>::quiet_NaN();
	}

	int SFBGSRigPackage_GetBoneLODValueC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName)
	{
		if (rig)
			try
			{
				return static_cast<int>(CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).boneLod(boneName));
			}
			catch ( const std::exception& ){}

		return -1;
	}

int SFBGSRigPackage_SetBoneLODValueC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName, int value)
{
	if (rig)
		try
		{
			if (SCOMPARE(boneName, "") ==0)
				return 1;

			const auto lod = value > CALUMI::SFBGS::SFBGS_RigPackage::MaxLOD || value < 0 ?
						CALUMI::SFBGS::SFBGS_RigPackage::DefaultLOD :
					static_cast<CALUMI::SFBGS::SFBGS_RigPackage::LODSetting>(value);

			CALUMI::SFBGS::SFBGS_RigPackage::GetPackage(*rig).setBoneLod(boneName, lod);
			return 0;
		}
		catch ( const std::exception& ){}

	return -1;
}

#pragma endregion
