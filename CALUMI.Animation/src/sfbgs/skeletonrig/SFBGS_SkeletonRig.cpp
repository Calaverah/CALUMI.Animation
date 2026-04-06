//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalplatform.h"
#include "internalvectordef.h"
#include <io/FileValidation.h>
#include <AnimStarfield>
#include <print>
#include <AnimFile>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstring>
#include <utilities/CALUMI_Utilities.h>

namespace CALUMI {namespace SFBGS {

	static Utilities::StringList CreateStringVectorFromRig(const CALUMI::UNIV::SkeletonRig& inputRig)
	{
		Utilities::StringList stringMap;
		stringMap.reserve(inputRig.BoneEntries().size());
		uint64_t iOffset = 0;
		for (int i = 0; i < inputRig.BoneEntries().size(); i++)
		{
			stringMap.push_back(inputRig.BoneEntries().at(i).Name().c_str(), iOffset);
			iOffset += inputRig.BoneEntries().at(i).Name().length(true);
		}
		stringMap.setFinalOffset(iOffset);
		return stringMap;
	}

	static std::vector<unsigned int> _getSFBGSRigStringOffsets(const Utilities::StringList& stringEntries)
	{
		std::vector<unsigned int> output;
		//offset should be 80 + 96+boneCount + 314... Yes we are static casting twice, it's just to prevent an overflow message and clean up compiler messages
		unsigned int offset = static_cast<unsigned int>(static_cast<size_t>(80) + 314 + 96 * stringEntries.size());

		output.reserve(stringEntries.size());
		for (unsigned int i = 0; i < stringEntries.size(); i++)
		{
			output.push_back(offset);
			offset += static_cast<unsigned int>(stringEntries.stringLength(i, true));
		}
		return output;
	}

#pragma region SKELETONBONE

	struct SkeletonBone::Impl
	{
		CALUMI::Math::Quaternion _localRotation;
		CALUMI::Math::Quaternion _globalRotation;
		CALUMI::Math::Vector3 _position;

		BoneType _boneType = BoneType::Default;

		uint64_t _nameOffset = 0;
		/// <summary>
		/// -1 for the root bone
		/// </summary>
		int32_t _parentBoneIndex = -1;
		/// <summary>
		/// Twist influence override? Always points to parent on mannequin twist bones
		/// </summary>
		int32_t _twistDriverMqnIndex = -1;
		/// <summary>
		/// Twist influence, -1 if skipped
		/// </summary>
		int32_t _twistDriverIndex = -1;
		/// <summary>
		/// Always -1
		/// </summary>
		int32_t _pad01 = -1;
		/// <summary>
		/// Same as this bone's index by default
		/// </summary>
		int32_t _mirrorBoneIndex = 0;
		int32_t _term05 = 0;
		/// <summary>
		/// Negative if pointing to parent
		/// </summary>
		float _twistDriverWeight = 0;
		/// <summary>
		/// Always 0
		/// </summary>
		int32_t _pad02 = 0;
		float _unknownScalar = 0.0;
		int32_t _pad03 = 0; //appears to be garbage/leftover memory from whatever block of memory was used during output at BGS

		Impl() = default;
	};

#ifdef DEBUG_BUILD
	int32_t SkeletonBone::getPad01() const
	{
		return pImpl->_pad01;
	}
	int32_t SkeletonBone::getPad02() const
	{
		return pImpl->_pad02;
	}
#endif

	SkeletonBone::SkeletonBone() { pImpl = new Impl; }
	SkeletonBone::~SkeletonBone() { if (pImpl) delete pImpl; }
	SkeletonBone::SkeletonBone(Utilities::BufferObject& buffer, unsigned long long& addressIndex) : SkeletonBone() 
	{
		//QUAT UNITS ARE SERIALIZED AS WXYZ AND NEED TO BE READ INTO XYZW

		{
			float w;
			std::memcpy(&w, &buffer.at(addressIndex), sizeof(w));
			addressIndex += sizeof(w);
			pImpl->_localRotation.setW(w);
		}

		{
			float f[3] = {};
			std::memcpy(&f, &buffer.at(addressIndex), sizeof(f));
			addressIndex += (sizeof(f));
			pImpl->_localRotation.setX(f[0]); pImpl->_localRotation.setY(f[1]); pImpl->_localRotation.setZ(f[2]);
		}

		{
			float w;
			std::memcpy(&w, &buffer.at(addressIndex), sizeof(w));
			addressIndex += sizeof(w);
			pImpl->_globalRotation.setW(w);
		}

		{
			float f[3] = {};
			std::memcpy(&f, &buffer.at(addressIndex), sizeof(f));
			addressIndex += (sizeof(f));
			pImpl->_globalRotation.setX(f[0]); pImpl->_globalRotation.setY(f[1]); pImpl->_globalRotation.setZ(f[2]);
		}

		{
			float f[3] = {};
			std::memcpy(&f, &buffer.at(addressIndex), sizeof(f));
			addressIndex += sizeof(f);
			pImpl->_position.setX(f[0]);
			pImpl->_position.setY(f[1]);
			pImpl->_position.setZ(f[2]);
		}

		

		std::memcpy(&pImpl->_boneType, &buffer.at(addressIndex), sizeof(pImpl->_boneType));
		addressIndex += sizeof(pImpl->_boneType);

		std::memcpy(&pImpl->_nameOffset, &buffer.at(addressIndex), sizeof(pImpl->_nameOffset));
		addressIndex += sizeof(pImpl->_nameOffset);

		std::memcpy(&pImpl->_parentBoneIndex, &buffer.at(addressIndex), sizeof(pImpl->_parentBoneIndex));
		addressIndex += sizeof(pImpl->_parentBoneIndex);

		std::memcpy(&pImpl->_twistDriverMqnIndex, &buffer.at(addressIndex), sizeof(pImpl->_twistDriverMqnIndex));
		addressIndex += sizeof(pImpl->_twistDriverMqnIndex);

		std::memcpy(&pImpl->_twistDriverIndex, &buffer.at(addressIndex), sizeof(pImpl->_twistDriverIndex));
		addressIndex += sizeof(pImpl->_twistDriverIndex);

		std::memcpy(&pImpl->_pad01, &buffer.at(addressIndex), sizeof(pImpl->_pad01));
		addressIndex += sizeof(pImpl->_pad01);

		std::memcpy(&pImpl->_mirrorBoneIndex, &buffer.at(addressIndex), sizeof(pImpl->_mirrorBoneIndex));
		addressIndex += sizeof(pImpl->_mirrorBoneIndex);

		std::memcpy(&pImpl->_term05, &buffer.at(addressIndex), sizeof(pImpl->_term05));
		addressIndex += sizeof(pImpl->_term05);

		std::memcpy(&pImpl->_twistDriverWeight, &buffer.at(addressIndex), sizeof(pImpl->_twistDriverWeight));
		addressIndex += sizeof(pImpl->_twistDriverWeight);

		std::memcpy(&pImpl->_pad02, &buffer.at(addressIndex), sizeof(pImpl->_pad02));
		addressIndex += sizeof(pImpl->_pad02);

		std::memcpy(&pImpl->_unknownScalar, &buffer.at(addressIndex), sizeof(pImpl->_unknownScalar));
		addressIndex += sizeof(pImpl->_unknownScalar);

		std::memcpy(&pImpl->_pad03, &buffer.at(addressIndex), sizeof(pImpl->_pad03));
		addressIndex += sizeof(pImpl->_pad03);
	}
	SkeletonBone::SkeletonBone(const SkeletonBone& input) : SkeletonBone()
	{
		*pImpl = *(input.pImpl);
	}
	bool SkeletonBone::operator<(const SkeletonBone& input)
	{
		return false;
	}
	SkeletonBone& SkeletonBone::operator=(const SkeletonBone& input)
	{
		*pImpl = *(input.pImpl);
		return *this;
	}
	
	CALUMI::Math::Quaternion& SFBGS::SkeletonBone::LocalRotation() const { return pImpl->_localRotation; }
	CALUMI::Math::Quaternion& SkeletonBone::GlobalRotation() const { return pImpl->_globalRotation; }
	CALUMI::Math::Vector3& SkeletonBone::Position() const { return pImpl->_position; }
	SkeletonBone::BoneType SkeletonBone::getBoneType() const { return pImpl->_boneType; }
	void SkeletonBone::setBoneType(BoneType t) { pImpl->_boneType = t; }
	uint64_t SkeletonBone::getNameOffset() const { return pImpl->_nameOffset; }
	void SkeletonBone::setNameOffset(uint64_t offset) { pImpl->_nameOffset = offset; }
	int32_t SkeletonBone::getParentBoneIndex() const { return pImpl->_parentBoneIndex; }
	void SkeletonBone::setParentBoneIndex(int32_t idx) { pImpl->_parentBoneIndex = idx; }
	int32_t SkeletonBone::getTwistDriverMqnIndex() const { return pImpl->_twistDriverMqnIndex; }
	void SkeletonBone::setTwistDriverMqnIndex(int32_t idx) { pImpl->_twistDriverMqnIndex = idx; }
	int32_t SkeletonBone::getTwistDriverIndex() const { return pImpl->_twistDriverIndex; }
	void SkeletonBone::setTwistDriverIndex(int32_t idx) { pImpl->_twistDriverIndex = idx; }
	int32_t SkeletonBone::getMirrorBoneIndex() const { return pImpl->_mirrorBoneIndex; }
	void SkeletonBone::setMirrorBoneIndex(int32_t idx) { pImpl->_mirrorBoneIndex = idx; }
	int32_t SkeletonBone::getTerm05() const { return pImpl->_term05; }
	void SkeletonBone::setTerm05(int32_t value) { pImpl->_term05 = value; }
	float SkeletonBone::getTwistDriverWeight() const { return pImpl->_twistDriverWeight; }
	void SkeletonBone::setTwistDriverWeight(float weight) { pImpl->_twistDriverWeight = weight; }
	float SkeletonBone::getUnknownScalar() const { return pImpl->_unknownScalar; }
	void SkeletonBone::setUnknownScalar(float value) { pImpl->_unknownScalar = value; }
	int32_t SkeletonBone::getTerm08() const { return pImpl->_pad03; }
	void SkeletonBone::setTerm08(int32_t value) { pImpl->_pad03 = value; }

	void SkeletonBone::SerializeIntoBuffer(Utilities::BufferObject& buffer, unsigned long long& addressIndex) const
	{
		buffer.insert(buffer.endPos(), 96, 0);

		//QUAT UNITS ARE SERIALIZED AS WXYZ AND NEED TO BE WRITTEN FROM XYZW
		{
			float w = pImpl->_localRotation.getW(); float f[3] = { pImpl->_localRotation.getX(),pImpl->_localRotation.getY(), pImpl->_localRotation.getZ() };
			std::memcpy(&buffer.at(addressIndex), &w, sizeof(w));
			addressIndex += sizeof(w);

			std::memcpy(&buffer.at(addressIndex), &f, sizeof(f));
			addressIndex += (sizeof(f));
		}

		{
			float w = pImpl->_globalRotation.getW(); float f[3] = { pImpl->_globalRotation.getX(),pImpl->_globalRotation.getY(), pImpl->_globalRotation.getZ() };
			std::memcpy(&buffer.at(addressIndex), &w, sizeof(w));
			addressIndex += sizeof(w);

			std::memcpy(&buffer.at(addressIndex), &f, sizeof(f));
			addressIndex += (sizeof(f));
		}

		{
			float f[3] = { pImpl->_position.getX(), pImpl->_position.getY(), pImpl->_position.getZ()};
			std::memcpy(&buffer.at(addressIndex), f, sizeof(f));
			addressIndex += sizeof(f);
		}

		std::memcpy(&buffer.at(addressIndex), &pImpl->_boneType, sizeof(pImpl->_boneType));
		addressIndex += sizeof(pImpl->_boneType);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_nameOffset, sizeof(pImpl->_nameOffset));
		addressIndex += sizeof(pImpl->_nameOffset);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_parentBoneIndex, sizeof(pImpl->_parentBoneIndex));
		addressIndex += sizeof(pImpl->_parentBoneIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_twistDriverMqnIndex, sizeof(pImpl->_twistDriverMqnIndex));
		addressIndex += sizeof(pImpl->_twistDriverMqnIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_twistDriverIndex, sizeof(pImpl->_twistDriverIndex));
		addressIndex += sizeof(pImpl->_twistDriverIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_pad01, sizeof(pImpl->_pad01));
		addressIndex += sizeof(pImpl->_pad01);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_mirrorBoneIndex, sizeof(pImpl->_mirrorBoneIndex));
		addressIndex += sizeof(pImpl->_mirrorBoneIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_term05, sizeof(pImpl->_term05));
		addressIndex += sizeof(pImpl->_term05);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_twistDriverWeight, sizeof(pImpl->_twistDriverWeight));
		addressIndex += sizeof(pImpl->_twistDriverWeight);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_pad02, sizeof(pImpl->_pad02));
		addressIndex += sizeof(pImpl->_pad02);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_unknownScalar, sizeof(pImpl->_unknownScalar));
		addressIndex += sizeof(pImpl->_unknownScalar);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_pad03, sizeof(pImpl->_pad03));
		addressIndex += sizeof(pImpl->_pad03);
	}

	bool SFBGS::SkeletonBone::SetBoneTypeFromUNIV(UNIV::SkeletonBone& univBone)
	{
		switch (univBone.GetBoneTypeProperty()->GetType())
		{
		case CALUMI::UNIV::BoneType::Default:
			pImpl->_boneType = SFBGS::SkeletonBone::BoneType::Default;
			//UNIV::DefaultBoneProperties* dProp = dynamic_cast<UNIV::DefaultBoneProperties*>(const_cast<UNIV::BoneTypeProperties*>(univBone.GetBoneTypeProperty()));
			pImpl->_twistDriverIndex = -1;
			pImpl->_twistDriverMqnIndex = -1;
			pImpl->_twistDriverWeight = 0.0;
			break;
		case CALUMI::UNIV::BoneType::Twist:
		{
			pImpl->_boneType = SFBGS::SkeletonBone::BoneType::Twist;
			//We are permitting const_casting as we are paying special attention not to delete the struct while modifying
			UNIV::TwistBoneProperties* tProp = dynamic_cast<UNIV::TwistBoneProperties*>(const_cast<UNIV::BoneTypeProperties*>(univBone.GetBoneTypeProperty()));
			pImpl->_twistDriverIndex = tProp->TwistDriverIndex();
			pImpl->_twistDriverWeight = tProp->TwistDriverWeight();
		}
		break;
		default:
			return false;
		}
		return true;
	}

	bool SFBGS::SkeletonBone::SetBoneTypeToUNIV(UNIV::SkeletonBone& univBone)
	{
		if (!univBone.SetBoneTypeProperty(GetBoneTypeAsUNIVEnum()))
			return false;

		switch (univBone.GetBoneTypeProperty()->GetType())
		{
		case CALUMI::UNIV::BoneType::Default:
			break;
		case CALUMI::UNIV::BoneType::Twist:
		{
			//We are permitting const_casting as we are paying special attention not to delete the struct while modifying
			UNIV::TwistBoneProperties* tProp = dynamic_cast<UNIV::TwistBoneProperties*>(const_cast<UNIV::BoneTypeProperties*>(univBone.GetBoneTypeProperty()));
			tProp->TwistDriverIndex(pImpl->_twistDriverIndex);
			tProp->TwistDriverWeight(pImpl->_twistDriverWeight);
		}
		break;
		default:
			break;
		}
		return true;
	}

	UNIV::BoneType SFBGS::SkeletonBone::GetBoneTypeAsUNIVEnum()
	{
		switch (pImpl->_boneType)
		{
		case SFBGS::SkeletonBone::BoneType::Default:
			return UNIV::BoneType::Default;
		case SFBGS::SkeletonBone::BoneType::Twist:
			return UNIV::BoneType::Twist;
		default:
			return UNIV::BoneType::UNDEFINED;
		}
	}

	const char* SkeletonBone::GetBoneTypeAsString()
	{
		switch (pImpl->_boneType)
		{
			case CALUMI::SFBGS::SkeletonBone::BoneType::Default:
			return "Default";
		case CALUMI::SFBGS::SkeletonBone::BoneType::Twist:
			return "Twist";
		default:
			return "SFBGS UNDEFINED";
		}
	}

#pragma endregion

#pragma region SKELETONRIG

	struct SkeletonRig::Impl
	{
		int _versionNumber = 05;

		uint32_t _fileSize = 0;

		/// <summary>
		/// Currently the only value seen is 0x50 (80)
		/// </summary>
		uint32_t _headerSize = 0x50;

		/// <summary>
		/// Always empty, possibly padding, see _PRIVATE_ for placement in struct
		/// </summary>
		uint32_t _headerEmpty01 = 0;
		/// <summary>
		/// 96* bone count + 80 bytes
		/// </summary>
		uint32_t _boneMapOffset = 0;

		/// <summary>
		/// Always empty, possibly padding, see _PRIVATE_ for placement in struct
		/// </summary>
		uint32_t _headerEmpty02 = 0;
		/// <summary>
			/// No matter what, these three ALWAYS match. Could be internal number tracking for BGS and may not matter to anyone outside of the company
			/// </summary>
		uint64_t _matchingThree[3] = { 0,0,0 };
		/// <summary>
		/// default precision values. For ships use 0.25. For first person use 0.0078125 (1/128)
		/// </summary>
		float _lowPrecision = 0.03125f;
		/// <summary>
		/// default precision values. For ships use 0.002. For first person use 6.25e-5 (1/16000)
		/// </summary>
		float _highPrecision = 0.00025f;

		uint16_t _boneCount = 0;
		uint16_t _boneCountAnimated = 0;
		/// <summary>
		/// Always empty, possibly padding, see _PRIVATE_ for placement in struct
		/// </summary>
		uint32_t _headerEmpty03 = 0;
		/// <summary>
		/// ;)
		/// </summary>
		uint8_t _endOfHeader[16] = { 0x0, 0x0, 0x43, 0x41, 0x4C, 0x55, 0x4D, 0x49, 0x44, 0x56, 0x52, 0x53, 0x4A, 0x4F, 0x4A, 0x4F };

		SkeletonBoneVector _boneEntries;

		//Don't ask why I'm initializing like this... let's just move on.
		int16_t _boneMapArray[SFBGSMAPSIZE] =
		{
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1
		};

		/// <summary>
		/// String array for the bone names
		/// </summary>
		Utilities::StringList _stringArray;
		Impl() = default;
	};

	SkeletonRig::SkeletonRig() { pImpl = new Impl; }
	SkeletonRig::~SkeletonRig() { if (pImpl) delete pImpl; }
	SkeletonRig::SkeletonRig(const SkeletonRig& input) : SkeletonRig() { *pImpl = *(input.pImpl); }
	SkeletonRig& SkeletonRig::operator=(const SkeletonRig& input) { *pImpl = *(input.pImpl);  return *this; }

	SkeletonRig::SkeletonRig(const UNIV::SkeletonRig& input) : SkeletonRig()
	{
		auto stringResult = CreateStringVectorFromRig(input);

		auto sfbgsRigPackagePtr = dynamic_cast<SFBGS_RigPackage*>(input.getPackageManager().getPackage(SFBGS_RIG_PACKAGE));
		SFBGS_RigPackage sfbgsRigPackage;
		bool isMarkedMannequin = false;
		if (sfbgsRigPackagePtr)
		{
			sfbgsRigPackage = *sfbgsRigPackagePtr;
		}

		isMarkedMannequin = sfbgsRigPackage.IsMannequin();
		LowPrecision(sfbgsRigPackage.getPrecisionSet().low());
		HighPrecision(sfbgsRigPackage.getPrecisionSet().high());

		FileSize(FileSize() + static_cast<unsigned int>(stringResult.getFinalOffset()));
		BoneMapOffset(BoneMapOffset() + static_cast<unsigned int>(80 + 96 * input.BoneEntries().size()));
		FileSize(FileSize() + BoneMapOffset() + SFBGSMAPSIZE * 2);

		BoneCount(static_cast<uint16_t>(input.BoneEntries().size()));

		uint16_t animatedBoneCount = 0;
		BoneEntries().reserve(BoneCount());
		for (unsigned int i = 0; i < BoneCount(); i++)
		{
			SkeletonBone toAdd;
			toAdd.pImpl->_localRotation = input.BoneEntries().at(i).LocalRotation();
			toAdd.pImpl->_globalRotation = input.BoneEntries().at(i).GlobalRotation();
			toAdd.pImpl->_position = input.BoneEntries().at(i).LocalPosition();
			toAdd.pImpl->_nameOffset = static_cast<uint64_t>(stringResult.getOffset(i));
			toAdd.pImpl->_parentBoneIndex = input.BoneEntries().at(i).GetParentBoneIndex();

			toAdd.pImpl->_mirrorBoneIndex = (input.BoneEntries().at(i).GetMirrorBoneIndex() < 0 || input.BoneEntries().at(i).GetMirrorBoneIndex() >= input.BoneEntries().size()) ? i : input.BoneEntries().at(i).GetMirrorBoneIndex();

			if (!toAdd.SetBoneTypeFromUNIV(input.BoneEntries().at(i)))
			{
				std::println("[CALUMI.Animation API] UNIV Rig: {} Bone: {} ({}) Bone Type: {} Is Not An Acceptable Type For SFBGS Skeleton Rigs! This Bone Will Remain As The Default Type", input.RigName().c_str(), input.BoneEntries().at(i).Name().c_str(), i, input.BoneEntries().at(i).GetBoneTypeProperty()->GetTypeString());
			}
			if (isMarkedMannequin && toAdd.pImpl->_boneType == SkeletonBone::BoneType::Twist)
			{
				toAdd.pImpl->_twistDriverMqnIndex = toAdd.pImpl->_parentBoneIndex;
			}
			else 
			{ 
				animatedBoneCount++; 
			}

			BoneEntries().push_back(toAdd);
		}

		BoneCountAnimated(animatedBoneCount);

		Utilities::S16Vector vecPackage = SFBGS_RigPackage::ConvertSFBGSRigPackage(input);
		BoneMapArray(vecPackage);


		StringArray().reserve(stringResult.size());
		for (int j = 0; j < stringResult.size(); j++)
		{
			StringArray().push_back(stringResult.c_str(j));
		}
	}

	void SkeletonRig::BoneMapArray(Utilities::S16Vector& input)
	{
		for (uint8_t j = 0; j < input.size(); j++)
		{
			pImpl->_boneMapArray[j] = input.at(j);
		}
	}

	Utilities::StringList& SkeletonRig::StringArray() const
	{
		return pImpl->_stringArray;
	}

	int SkeletonRig::VersionNumber() const
	{
		return pImpl->_versionNumber;
	}

	void SkeletonRig::VersionNumber(int v)
	{
		pImpl->_versionNumber = v;
	}

	uint32_t SkeletonRig::FileSize() const
	{
		return pImpl->_fileSize;
	}

	void SkeletonRig::FileSize(uint32_t size)
	{
		pImpl->_fileSize = size;
	}

	uint32_t SkeletonRig::HeaderSize() const
	{
		return pImpl->_headerSize;
	}

	void SkeletonRig::HeaderSize(uint32_t size)
	{
		pImpl->_headerSize = size;
	}

	uint32_t SkeletonRig::BoneMapOffset() const
	{
		return pImpl->_boneMapOffset;
	}

	void SkeletonRig::BoneMapOffset(uint32_t offset)
	{
		pImpl->_boneMapOffset = offset;
	}

	Utilities::U64Vector SkeletonRig::getMatchingThree() const
	{
		Utilities::U64Vector output;
		output.resize(3);
		output.at(0) = pImpl->_matchingThree[0];
		output.at(1) = pImpl->_matchingThree[1];
		output.at(2) = pImpl->_matchingThree[2];
		return output;
	}

	void SkeletonRig::setMatchingThree(uint64_t m1, uint64_t m2, uint64_t m3)
	{
		pImpl->_matchingThree[0] = m1;
		pImpl->_matchingThree[1] = m2;
		pImpl->_matchingThree[2] = m3;
	}

	float SkeletonRig::LowPrecision() const
	{
		return pImpl->_lowPrecision;
	}

	void SkeletonRig::LowPrecision(float value)
	{
		pImpl->_lowPrecision = value;
	}

	float SkeletonRig::HighPrecision() const
	{
		return pImpl->_highPrecision;
	}

	void SkeletonRig::HighPrecision(float value)
	{
		pImpl->_highPrecision = value;
	}

	uint16_t SkeletonRig::BoneCount() const
	{
		return pImpl->_boneCount;
	}

	void SkeletonRig::BoneCount(uint16_t count)
	{
		pImpl->_boneCount = count;
	}

	uint16_t SkeletonRig::BoneCountAnimated() const
	{
		return pImpl->_boneCountAnimated;
	}

	void SkeletonRig::BoneCountAnimated(uint16_t count)
	{
		pImpl->_boneCountAnimated = count;
	}

	SkeletonBoneVector& SkeletonRig::BoneEntries() const
	{
		return pImpl->_boneEntries;
	}

	Utilities::S16Vector SkeletonRig::BoneMapArray() const
	{
		Utilities::S16Vector output; 
		output.reserve(SFBGSMAPSIZE);
		for (uint8_t i = 0; i < SFBGSMAPSIZE; i++)
		{
			output.push_back(pImpl->_boneMapArray[i]);
		}
		return output;
	}

#ifdef DEBUG_BUILD
	Utilities::CharVector CALUMI::SFBGS::SkeletonRig::EndOfHeader() const
	{
		Utilities::CharVector output;
		output.resize(16);
		for (uint8_t i = 0; i < 16; i++)
		{
			output.at(i) = pImpl->_endOfHeader[i];
		}
		return output;
	}
#endif 

    Utilities::FileResult SkeletonRig::ReadFromFile(Utilities::PathContainer&& inputFilePath)
	{
		Utilities::PathContainer output(inputFilePath);
		return ReadFromFile(output);
	}
	bool SkeletonRig::IsMarkedMannequin() const
	{
		for (uint64_t i = 0; i < pImpl->_boneEntries.size(); i++)
		{
			if (pImpl->_boneEntries.at(i).getTwistDriverMqnIndex() > 0)
				return true;
		}
		return false;
	}
	Utilities::FileResult SkeletonRig::ReadFromFile(Utilities::PathContainer& inputFilePath)
	{
		Utilities::StringList vec; 
		vec.push_back(".rig");

		auto buffer = CALUMI::ValidateFile(inputFilePath, vec, 80, 0, true);
		if (buffer.result().hasError())
		{
			return buffer.result();
		}

		//iterator tracking
		unsigned long long addressIndex = 0;

		//HEADER READING
		{
			std::memcpy(&pImpl->_versionNumber, &buffer.at(addressIndex), sizeof(pImpl->_versionNumber));
			addressIndex += sizeof(pImpl->_versionNumber);

			std::memcpy(&pImpl->_fileSize, &buffer.at(addressIndex), sizeof(pImpl->_fileSize));
			addressIndex += sizeof(pImpl->_fileSize);

			std::memcpy(&pImpl->_headerSize, &buffer.at(addressIndex), sizeof(pImpl->_headerSize));
			addressIndex += sizeof(pImpl->_headerSize);

			std::memcpy(&pImpl->_headerEmpty01, &buffer.at(addressIndex), sizeof(pImpl->_headerEmpty01));
			addressIndex += sizeof(pImpl->_headerEmpty01);

			std::memcpy(&pImpl->_boneMapOffset, &buffer.at(addressIndex), sizeof(pImpl->_boneMapOffset));
			addressIndex += sizeof(pImpl->_boneMapOffset);

			std::memcpy(&pImpl->_headerEmpty02, &buffer.at(addressIndex), sizeof(pImpl->_headerEmpty02));
			addressIndex += sizeof(pImpl->_headerEmpty02);

			std::memcpy(&pImpl->_matchingThree, &buffer.at(addressIndex), sizeof(pImpl->_matchingThree));
			addressIndex += sizeof(pImpl->_matchingThree);

			std::memcpy(&pImpl->_lowPrecision, &buffer.at(addressIndex), sizeof(pImpl->_lowPrecision));
			addressIndex += sizeof(pImpl->_lowPrecision);

			std::memcpy(&pImpl->_highPrecision, &buffer.at(addressIndex), sizeof(pImpl->_highPrecision));
			addressIndex += sizeof(pImpl->_highPrecision);

			std::memcpy(&pImpl->_boneCount, &buffer.at(addressIndex), sizeof(pImpl->_boneCount));
			addressIndex += sizeof(pImpl->_boneCount);

			std::memcpy(&pImpl->_boneCountAnimated, &buffer.at(addressIndex), sizeof(pImpl->_boneCountAnimated));
			addressIndex += sizeof(pImpl->_boneCountAnimated);

			std::memcpy(&pImpl->_headerEmpty03, &buffer.at(addressIndex), sizeof(pImpl->_headerEmpty03));
			addressIndex += sizeof(pImpl->_headerEmpty03);

			std::memcpy(&pImpl->_endOfHeader, &buffer.at(addressIndex), sizeof(pImpl->_endOfHeader));
			addressIndex += sizeof(pImpl->_endOfHeader);
		}

#ifdef DEBUG_BUILD
		//WARN IF FILE SIZE VAR != BUFFER
		if (pImpl->_fileSize != buffer.size()) 
			std::println("WARNING: THE VARIABLE, FILE SIZE = {} DOES NOT MATCH THE BUFFER SIZE OF {}", pImpl->_fileSize, buffer.size());
#endif

		//READ BONE ENTRIES IN ORDER
		{
			pImpl->_boneEntries.reserve(pImpl->_boneCount);
			for (unsigned short i = 0; i < pImpl->_boneCount; i++)
			{
				pImpl->_boneEntries.push_back(SkeletonBone(buffer, addressIndex));
			}
		}


		//READ SUFFIX
		std::memcpy(&pImpl->_boneMapArray, &buffer.at(addressIndex), sizeof(pImpl->_boneMapArray));
		addressIndex += sizeof(pImpl->_boneMapArray);

		//READ STRINGS
		pImpl->_stringArray.reserve(pImpl->_boneCount);
		for (uint16_t i = 0; i < pImpl->_boneCount; i++)
		{
			pImpl->_stringArray.push_back(&buffer.at(pImpl->_boneEntries.at(i).getNameOffset()));
			addressIndex += (pImpl->_stringArray.stringLength(i, true));
		}

#ifdef DEBUG_BUILD
		if (buffer.size() != addressIndex)
		{
			std::println("===========================================================");
			std::println("===========================================================");
			std::println("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==");
			std::println("BUFFER  SIZE: {}", buffer.size());
			std::println("BUFFER START: {}", (void*)buffer.data());
			std::println("Size+Start: {}", (void*)(buffer.size() + buffer.data()));
			std::println("===========================================================");
			std::cout << "Press ENTER to continue running the program." << std::endl;
			std::cin.get();
			std::println("===========================================================");
		}
#endif

		return {Utilities::FileResult::FileErrorCode::Success, inputFilePath.w_str(), ""};
	}

    Utilities::FileResult SkeletonRig::WriteToFile(Utilities::PathContainer&& outputFilePath)
	{
		Utilities::PathContainer output(outputFilePath);
		return WriteToFile(output);
	}

	Utilities::FileResult SkeletonRig::WriteToFile(Utilities::PathContainer& outputFilePath)
	{
		//LARGEST FILE: "D:/ModOrganizer/Starfield_Mod_Authoring_01/mods/ExtractedData/meshes/furniture/armillary/characterassets/skeleton.rig" at 18301 bytes

		Utilities::FileBufferResult buffer;
		buffer.reserve(18500);

		unsigned long long addressIndex = 0;
		std::vector<unsigned int> offsets = _getSFBGSRigStringOffsets(pImpl->_stringArray);

		//HEADER
		{
			buffer.insert(buffer.endPos(), 80, 0);
			std::memcpy(&buffer.at(addressIndex), &pImpl->_versionNumber, sizeof(pImpl->_versionNumber));
			addressIndex += sizeof(pImpl->_versionNumber);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_fileSize, sizeof(pImpl->_fileSize));
			addressIndex += sizeof(pImpl->_fileSize);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_headerSize, sizeof(pImpl->_headerSize));
			addressIndex += sizeof(pImpl->_headerSize);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_headerEmpty01, sizeof(pImpl->_headerEmpty01));
			addressIndex += sizeof(pImpl->_headerEmpty01);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_boneMapOffset, sizeof(pImpl->_boneMapOffset));
			addressIndex += sizeof(pImpl->_boneMapOffset);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_headerEmpty02, sizeof(pImpl->_headerEmpty02));
			addressIndex += sizeof(pImpl->_headerEmpty02);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_matchingThree, sizeof(pImpl->_matchingThree));
			addressIndex += sizeof(pImpl->_matchingThree);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_lowPrecision, sizeof(pImpl->_lowPrecision));
			addressIndex += sizeof(pImpl->_lowPrecision);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_highPrecision, sizeof(pImpl->_highPrecision));
			addressIndex += sizeof(pImpl->_highPrecision);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_boneCount, sizeof(pImpl->_boneCount));
			addressIndex += sizeof(pImpl->_boneCount);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_boneCountAnimated, sizeof(pImpl->_boneCountAnimated));
			addressIndex += sizeof(pImpl->_boneCountAnimated);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_headerEmpty03, sizeof(pImpl->_headerEmpty03));
			addressIndex += sizeof(pImpl->_headerEmpty03);

			std::memcpy(&buffer.at(addressIndex), &pImpl->_endOfHeader, sizeof(pImpl->_endOfHeader));
			addressIndex += sizeof(pImpl->_endOfHeader);
		}

		//BONE ENTRIES
		{
			for (unsigned int i = 0; i < pImpl->_boneEntries.size(); i++)
			{
				pImpl->_boneEntries.at(i).setNameOffset(offsets.at(i));
				pImpl->_boneEntries.at(i).SerializeIntoBuffer(buffer, addressIndex);
			}
		}

		//SUFFIX (PI SIZED SECTION)
		{
			buffer.insert(buffer.endPos(), sizeof(pImpl->_boneMapArray), -1);

			//Setting header value to confirm offset. In case the SFBGS Rig Values were changed incorrectly by the user
			pImpl->_boneMapOffset = static_cast<unsigned int>(addressIndex);
			std::memcpy(&buffer.at(16), &pImpl->_boneMapOffset, sizeof(pImpl->_boneMapOffset));

			std::memcpy(&buffer.at(addressIndex), &pImpl->_boneMapArray, sizeof(pImpl->_boneMapArray));
			addressIndex += sizeof(pImpl->_boneMapArray);
		}

		//STRING ARRAY
		{
			for (uint16_t i = 0; i < pImpl->_stringArray.size(); i++)
			{
				for (int j = 0; j < pImpl->_stringArray.stringLength(i); j++)
				{
					buffer.push_back(*(pImpl->_stringArray.c_str(i) + j));
				}
				buffer.push_back('\0');
				addressIndex += pImpl->_stringArray.stringLength(i, true);
			}
		}

#ifdef DEBUG_BUILD
		if (pImpl->_fileSize != addressIndex)
		{
			std::println("===========================================================");
			std::println("===========================================================");
			std::println("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==");
			std::println("BUFFER  SIZE: {}", buffer.size());
			std::println("FILE  SIZE: {}", pImpl->_fileSize);
			std::println("===========================================================");
			std::cout << "Press ENTER to continue running the program." << std::endl;
			std::cin.get();
			std::println("===========================================================");
		}
#endif

		pImpl->_fileSize = static_cast<unsigned int>(buffer.size());
		std::memcpy(&buffer.at(4), &pImpl->_fileSize, sizeof(pImpl->_fileSize));

		return CALUMI::WriteToBinaryFile(outputFilePath, buffer);
	}

	uint8_t SkeletonRig::DEBUG_CheckAssumedHeaderEntries()
	{
		uint8_t output = 0;

		if (pImpl->_headerEmpty01 != 0)
			output += 0b1;
		if (pImpl->_headerEmpty02 != 0)
			output += 0b10;
		if (pImpl->_headerEmpty03 != 0)
			output += 0b100;

		if (pImpl->_matchingThree[0] != pImpl->_matchingThree[1] || pImpl->_matchingThree[0] != pImpl->_matchingThree[2])
			output += 0b1000;

		if (pImpl->_headerSize != 0x50)
			output += 0b10000;

		if (pImpl->_versionNumber != 5)
			output += 0b100000;

		int aCount = 0;
		for (uint64_t i = 0; i < pImpl->_boneEntries.size(); i++)
		{
			if (pImpl->_boneEntries.at(i).getBoneType() == SFBGS::SkeletonBone::BoneType::Default)
			{
				aCount++;
			}
		}
		if (aCount != pImpl->_boneCountAnimated)
			output += 0b1000000;

		return output;
	}

#pragma endregion

	void SkeletonRig::ConvertFromUniversalRig(const CALUMI::UNIV::SkeletonRig& inputRig)
	{
		SkeletonRig rigBuffer(inputRig);
		*this = rigBuffer;
	}

	CALUMI::UNIV::SkeletonRig SkeletonRig::ConvertToUniversalRig() const
	{
		CALUMI::UNIV::SkeletonRig output;
		
		for (unsigned int i = 0; i < pImpl->_boneEntries.size(); i++)
		{
			SFBGS::SkeletonBone& bone = pImpl->_boneEntries.at(i);
			output.AddBoneToRig(bone.pImpl->_localRotation,bone.pImpl->_position,pImpl->_stringArray.c_str(i), bone.pImpl->_parentBoneIndex, true);
			bone.SetBoneTypeToUNIV(output.BoneEntries().at(i));
			int setter = bone.pImpl->_mirrorBoneIndex == i ? -1 : bone.pImpl->_mirrorBoneIndex;
			output.BoneEntries().at(i).SetMirrorBoneIndex(setter);
		}
		
		SFBGS_RigPackage::CreateNewSFBGSRigPackage(output);
		if (SFBGS_RigPackage* sfbgsRigPackage = dynamic_cast<SFBGS_RigPackage*>(output.getPackageManager().getPackage(SFBGS_RIG_PACKAGE)))
		{
			auto boneMapArray = BoneMapArray();
			for (int key = 0; key < SFBGSMAPSIZE; key++)
			{
				if (boneMapArray.at(key) >= 0)
				{
					sfbgsRigPackage->AddBoneToMap(static_cast<SFBGS_RigPackage::BoneMapKey>(key), output.BoneEntries().at(boneMapArray.at(key)).Name().c_str(), true);
				}
			}
			if (IsMarkedMannequin())
				sfbgsRigPackage->IsMannequin(true);

			sfbgsRigPackage->SetPrecisionValues(SFBGS::PrecisionSet(LowPrecision(), HighPrecision()));
		}

		return output;
	}

	VECTORDEF(SkeletonBoneVector, SkeletonBone)

}
}