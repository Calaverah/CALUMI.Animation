//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "SFBGS_SkeletonRig.h"
#include <print>
#include "FileValidation.h"
#include <algorithm>
#include <iostream>
#include <vector>

namespace CALUMI {namespace SFBGS {

	static Utilities::StringMap CreateStringVectorFromRig(const CALUMI::UNIV::SkeletonRig& inputRig)
	{
		Utilities::StringMap stringMap;
		stringMap.Reserve(inputRig.BoneEntries().size());
		size_t iOffset = 0;
		for (int i = 0; i < inputRig.BoneEntries().size(); i++)
		{
			stringMap.push_back(inputRig.BoneEntries().at(i).Name().c_str(), iOffset);
			iOffset += inputRig.BoneEntries().at(i).Name().Length(true);
		}
		stringMap.SetFinalOffset(iOffset);
		return stringMap;
	}

	static std::vector<unsigned int> _getSFBGSRigStringOffsets(const Utilities::VectorContainer<Utilities::StringContainer>& stringEntries)
	{
		std::vector<unsigned int> output;
		//offset should be 80 + 96+boneCount + 314... Yes we are static casting twice, it's just to prevent an overflow message and clean up compiler messages
		unsigned int offset = static_cast<unsigned int>(static_cast<size_t>(80) + 314 + 96 * stringEntries.size());

		output.reserve(stringEntries.size());
		for (unsigned int i = 0; i < stringEntries.size(); i++)
		{
			output.push_back(offset);
			offset += static_cast<unsigned int>(stringEntries.at(i).Length(true));
		}
		return output;
	}

#pragma region SKELETONBONE

	struct SkeletonBone::Impl
	{
		CALUMI::Math::Quaternion localRotation;
		CALUMI::Math::Quaternion globalRotation;
		CALUMI::Math::Vector3 position;

		BoneType boneType = BoneType::Default;

		uint64_t nameOffset = 0;
		/// <summary>
		/// -1 for the root bone
		/// </summary>
		int32_t parentBoneIndex = -1;
		/// <summary>
		/// Twist influence override? Always points to parent on mannequin twist bones
		/// </summary>
		int32_t twistDriverMqnIndex = -1;
		/// <summary>
		/// Twist influence, -1 if skipped
		/// </summary>
		int32_t twistDriverIndex = -1;
		/// <summary>
		/// Always -1
		/// </summary>
		int32_t _pad01 = -1;
		/// <summary>
		/// Same as this bone's index by default
		/// </summary>
		int32_t mirrorBoneIndex = 0;
		int32_t term05 = 0;
		/// <summary>
		/// Negative if pointing to parent
		/// </summary>
		float twistDriverWeight = 0;
		/// <summary>
		/// Always 0
		/// </summary>
		int32_t _pad02 = 0;
		float unknownScalar = 0.0;
		int32_t term08 = 0;

		Impl() = default;
	};

#ifdef _DEBUG
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
	SkeletonBone::SkeletonBone(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex) : SkeletonBone() 
	{
		//QUAT UNITS ARE SERIALIZED AS WXYZ AND NEED TO BE READ INTO XYZW

		{
			float w;
			std::memcpy(&w, &buffer.at(addressIndex), sizeof(w));
			addressIndex += sizeof(w);
			pImpl->localRotation.setW(w);
		}

		{
			float f[3] = {};
			std::memcpy(&f, &buffer.at(addressIndex), sizeof(f));
			addressIndex += (sizeof(f));
			pImpl->localRotation.setX(f[0]); pImpl->localRotation.setY(f[1]); pImpl->localRotation.setZ(f[2]);
		}

		{
			float w;
			std::memcpy(&w, &buffer.at(addressIndex), sizeof(w));
			addressIndex += sizeof(w);
			pImpl->globalRotation.setW(w);
		}

		{
			float f[3] = {};
			std::memcpy(&f, &buffer.at(addressIndex), sizeof(f));
			addressIndex += (sizeof(f));
			pImpl->globalRotation.setX(f[0]); pImpl->globalRotation.setY(f[1]); pImpl->globalRotation.setZ(f[2]);
		}

		{
			float f[3] = {};
			std::memcpy(&f, &buffer.at(addressIndex), sizeof(f));
			addressIndex += sizeof(f);
			pImpl->position.setX(f[0]);
			pImpl->position.setY(f[1]);
			pImpl->position.setZ(f[2]);
		}

		

		std::memcpy(&pImpl->boneType, &buffer.at(addressIndex), sizeof(pImpl->boneType));
		addressIndex += sizeof(pImpl->boneType);

		std::memcpy(&pImpl->nameOffset, &buffer.at(addressIndex), sizeof(pImpl->nameOffset));
		addressIndex += sizeof(pImpl->nameOffset);

		std::memcpy(&pImpl->parentBoneIndex, &buffer.at(addressIndex), sizeof(pImpl->parentBoneIndex));
		addressIndex += sizeof(pImpl->parentBoneIndex);

		std::memcpy(&pImpl->twistDriverMqnIndex, &buffer.at(addressIndex), sizeof(pImpl->twistDriverMqnIndex));
		addressIndex += sizeof(pImpl->twistDriverMqnIndex);

		std::memcpy(&pImpl->twistDriverIndex, &buffer.at(addressIndex), sizeof(pImpl->twistDriverIndex));
		addressIndex += sizeof(pImpl->twistDriverIndex);

		std::memcpy(&pImpl->_pad01, &buffer.at(addressIndex), sizeof(pImpl->_pad01));
		addressIndex += sizeof(pImpl->_pad01);

		std::memcpy(&pImpl->mirrorBoneIndex, &buffer.at(addressIndex), sizeof(pImpl->mirrorBoneIndex));
		addressIndex += sizeof(pImpl->mirrorBoneIndex);

		std::memcpy(&pImpl->term05, &buffer.at(addressIndex), sizeof(pImpl->term05));
		addressIndex += sizeof(pImpl->term05);

		std::memcpy(&pImpl->twistDriverWeight, &buffer.at(addressIndex), sizeof(pImpl->twistDriverWeight));
		addressIndex += sizeof(pImpl->twistDriverWeight);

		std::memcpy(&pImpl->_pad02, &buffer.at(addressIndex), sizeof(pImpl->_pad02));
		addressIndex += sizeof(pImpl->_pad02);

		std::memcpy(&pImpl->unknownScalar, &buffer.at(addressIndex), sizeof(pImpl->unknownScalar));
		addressIndex += sizeof(pImpl->unknownScalar);

		std::memcpy(&pImpl->term08, &buffer.at(addressIndex), sizeof(pImpl->term08));
		addressIndex += sizeof(pImpl->term08);
	}
	SkeletonBone::SkeletonBone(const SkeletonBone& input) : SkeletonBone()
	{
		*pImpl = *(input.pImpl);
	}
	SkeletonBone& SkeletonBone::operator=(const SkeletonBone& input)
	{
		*pImpl = *(input.pImpl);
		return *this;
	}
	
	CALUMI::Math::Quaternion& SFBGS::SkeletonBone::LocalRotation() const { return pImpl->localRotation; }
	CALUMI::Math::Quaternion& SkeletonBone::GlobalRotation() const { return pImpl->globalRotation; }
	CALUMI::Math::Vector3& SkeletonBone::Position() const { return pImpl->position; }
	BoneType SkeletonBone::getBoneType() const { return pImpl->boneType; }
	void SkeletonBone::setBoneType(BoneType t) { pImpl->boneType = t; }
	uint64_t SkeletonBone::getNameOffset() const { return pImpl->nameOffset; }
	void SkeletonBone::setNameOffset(uint64_t offset) { pImpl->nameOffset = offset; }
	int32_t SkeletonBone::getParentBoneIndex() const { return pImpl->parentBoneIndex; }
	void SkeletonBone::setParentBoneIndex(int32_t idx) { pImpl->parentBoneIndex = idx; }
	int32_t SkeletonBone::getTwistDriverMqnIndex() const { return pImpl->twistDriverMqnIndex; }
	void SkeletonBone::setTwistDriverMqnIndex(int32_t idx) { pImpl->twistDriverMqnIndex = idx; }
	int32_t SkeletonBone::getTwistDriverIndex() const { return pImpl->twistDriverIndex; }
	void SkeletonBone::setTwistDriverIndex(int32_t idx) { pImpl->twistDriverIndex = idx; }
	int32_t SkeletonBone::getMirrorBoneIndex() const { return pImpl->mirrorBoneIndex; }
	void SkeletonBone::setMirrorBoneIndex(int32_t idx) { pImpl->mirrorBoneIndex = idx; }
	int32_t SkeletonBone::getTerm05() const { return pImpl->term05; }
	void SkeletonBone::setTerm05(int32_t value) { pImpl->term05 = value; }
	float SkeletonBone::getTwistDriverWeight() const { return pImpl->twistDriverWeight; }
	void SkeletonBone::setTwistDriverWeight(float weight) { pImpl->twistDriverWeight = weight; }
	float SkeletonBone::getUnknownScalar() const { return pImpl->unknownScalar; }
	void SkeletonBone::setUnknownScalar(float value) { pImpl->unknownScalar = value; }
	int32_t SkeletonBone::getTerm08() const { return pImpl->term08; }
	void SkeletonBone::setTerm08(int32_t value) { pImpl->term08 = value; }

	void SkeletonBone::SerializeIntoBuffer(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex) const
	{
		buffer.insert(buffer.end(), 96, 0);

		//QUAT UNITS ARE SERIALIZED AS WXYZ AND NEED TO BE WRITTEN FROM XYZW
		{
			float w = pImpl->localRotation.getW(); float f[3] = { pImpl->localRotation.getX(),pImpl->localRotation.getY(), pImpl->localRotation.getZ() };
			std::memcpy(&buffer.at(addressIndex), &w, sizeof(w));
			addressIndex += sizeof(w);

			std::memcpy(&buffer.at(addressIndex), &f, sizeof(f));
			addressIndex += (sizeof(f));
		}

		{
			float w = pImpl->globalRotation.getW(); float f[3] = { pImpl->globalRotation.getX(),pImpl->globalRotation.getY(), pImpl->globalRotation.getZ() };
			std::memcpy(&buffer.at(addressIndex), &w, sizeof(w));
			addressIndex += sizeof(w);

			std::memcpy(&buffer.at(addressIndex), &f, sizeof(f));
			addressIndex += (sizeof(f));
		}

		{
			float f[3] = { pImpl->position.getX(), pImpl->position.getY(), pImpl->position.getZ()};
			std::memcpy(&buffer.at(addressIndex), f, sizeof(f));
			addressIndex += sizeof(f);
		}

		std::memcpy(&buffer.at(addressIndex), &pImpl->boneType, sizeof(pImpl->boneType));
		addressIndex += sizeof(pImpl->boneType);

		std::memcpy(&buffer.at(addressIndex), &pImpl->nameOffset, sizeof(pImpl->nameOffset));
		addressIndex += sizeof(pImpl->nameOffset);

		std::memcpy(&buffer.at(addressIndex), &pImpl->parentBoneIndex, sizeof(pImpl->parentBoneIndex));
		addressIndex += sizeof(pImpl->parentBoneIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->twistDriverMqnIndex, sizeof(pImpl->twistDriverMqnIndex));
		addressIndex += sizeof(pImpl->twistDriverMqnIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->twistDriverIndex, sizeof(pImpl->twistDriverIndex));
		addressIndex += sizeof(pImpl->twistDriverIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_pad01, sizeof(pImpl->_pad01));
		addressIndex += sizeof(pImpl->_pad01);

		std::memcpy(&buffer.at(addressIndex), &pImpl->mirrorBoneIndex, sizeof(pImpl->mirrorBoneIndex));
		addressIndex += sizeof(pImpl->mirrorBoneIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->term05, sizeof(pImpl->term05));
		addressIndex += sizeof(pImpl->term05);

		std::memcpy(&buffer.at(addressIndex), &pImpl->twistDriverWeight, sizeof(pImpl->twistDriverWeight));
		addressIndex += sizeof(pImpl->twistDriverWeight);

		std::memcpy(&buffer.at(addressIndex), &pImpl->_pad02, sizeof(pImpl->_pad02));
		addressIndex += sizeof(pImpl->_pad02);

		std::memcpy(&buffer.at(addressIndex), &pImpl->unknownScalar, sizeof(pImpl->unknownScalar));
		addressIndex += sizeof(pImpl->unknownScalar);

		std::memcpy(&buffer.at(addressIndex), &pImpl->term08, sizeof(pImpl->term08));
		addressIndex += sizeof(pImpl->term08);
	}

	bool SFBGS::SkeletonBone::SetBoneTypeFromUNIV(UNIV::SkeletonBone& univBone)
	{
		switch (univBone.GetBoneTypeProperty()->GetType())
		{
		case CALUMI::UNIV::BoneType::Default:
			pImpl->boneType = SFBGS::BoneType::Default;
			//UNIV::DefaultBoneProperties* dProp = dynamic_cast<UNIV::DefaultBoneProperties*>(const_cast<UNIV::BoneTypeProperties*>(univBone.GetBoneTypeProperty()));
			pImpl->twistDriverIndex = -1;
			pImpl->twistDriverMqnIndex = -1;
			pImpl->twistDriverWeight = 0.0;
			break;
		case CALUMI::UNIV::BoneType::Twist:
		{
			pImpl->boneType = SFBGS::BoneType::Twist;
			//We are permitting const_casting as we are paying special attention not to delete the struct while modifying
			UNIV::TwistBoneProperties* tProp = dynamic_cast<UNIV::TwistBoneProperties*>(const_cast<UNIV::BoneTypeProperties*>(univBone.GetBoneTypeProperty()));
			pImpl->twistDriverIndex = tProp->TwistDriverIndex();
			pImpl->twistDriverWeight = tProp->TwistDriverWeight();
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
			tProp->TwistDriverIndex(pImpl->twistDriverIndex);
			tProp->TwistDriverWeight(pImpl->twistDriverWeight);
		}
		break;
		default:
			break;
		}
		return true;
	}

	UNIV::BoneType SFBGS::SkeletonBone::GetBoneTypeAsUNIVEnum()
	{
		switch (pImpl->boneType)
		{
		case SFBGS::BoneType::Default:
			return UNIV::BoneType::Default;
		case SFBGS::BoneType::Twist:
			return UNIV::BoneType::Twist;
		default:
			return UNIV::BoneType::UNDEFINED;
		}
	}

	const char* SkeletonBone::GetBoneTypeAsString()
	{
		switch (pImpl->boneType)
		{
		case CALUMI::SFBGS::BoneType::Default:
			return "Default";
		case CALUMI::SFBGS::BoneType::Twist:
			return "Twist";
		default:
			return "SFBGS UNDEFINED";
		}
	}

#pragma endregion

#pragma region SKELETONRIG

	struct SkeletonRig::Impl
	{
		int versionNumber = 05;

		uint32_t fileSize = 0;

		/// <summary>
		/// Currently the only value seen is 0x50 (80)
		/// </summary>
		uint32_t headerSize = 0x50;

		/// <summary>
		/// Always empty, possibly padding, see _PRIVATE_ for placement in struct
		/// </summary>
		uint32_t headerEmpty01 = 0;
		/// <summary>
		/// 96* bone count + 80 bytes
		/// </summary>
		uint32_t boneMapOffset = 0;

		/// <summary>
		/// Always empty, possibly padding, see _PRIVATE_ for placement in struct
		/// </summary>
		uint32_t headerEmpty02 = 0;
		/// <summary>
			/// No matter what, these three ALWAYS match. Could be internal number tracking for BGS and may not matter to anyone outside of the company
			/// </summary>
		uint64_t matchingThree[3] = { 0,0,0 };
		/// <summary>
		/// default precision values. For ships use 0.25. For first person use 0.0078125 (1/128)
		/// </summary>
		float lowPrecision = 0.03125f;
		/// <summary>
		/// default precision values. For ships use 0.002. For first person use 6.25e-5 (1/16000)
		/// </summary>
		float highPrecision = 0.00025f;

		uint16_t boneCount = 0;
		uint16_t boneCount_Animated = 0;
		/// <summary>
		/// Always empty, possibly padding, see _PRIVATE_ for placement in struct
		/// </summary>
		uint32_t headerEmpty03 = 0;
		/// <summary>
		/// ;)
		/// </summary>
		uint8_t endOfHeader[16] = { 0x0, 0x0, 0x43, 0x41, 0x4C, 0x55, 0x4D, 0x49, 0x44, 0x56, 0x52, 0x53, 0x4A, 0x4F, 0x4A, 0x4F };

		Utilities::VectorContainer<SkeletonBone> boneEntries;

		//Don't ask why I'm initializing like this... let's just move on.
		int16_t boneMapArray[SFBGSMAPSIZE] =
		{
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1
		};

		/// <summary>
		/// String array for the bone names
		/// </summary>
		Utilities::VectorContainer<Utilities::StringContainer> stringArray;
		Impl() = default;
	};

	SkeletonRig::SkeletonRig() { pImpl = new Impl; }
	SkeletonRig::~SkeletonRig() { if (pImpl) delete pImpl; }
	SkeletonRig::SkeletonRig(const SkeletonRig& input) : SkeletonRig() { *pImpl = *(input.pImpl); }
	SkeletonRig& SkeletonRig::operator=(const SkeletonRig& input) { *pImpl = *(input.pImpl);  return *this; }

	void SkeletonRig::BoneMapArray(Utilities::VectorContainer<int16_t>& input)
	{
		for (uint8_t j = 0; j < input.size(); j++)
		{
			pImpl->boneMapArray[j] = input.at(j);
		}
	}

	Utilities::VectorContainer<Utilities::StringContainer>& SkeletonRig::StringArray() const
	{
		return pImpl->stringArray;
	}

	int SkeletonRig::VersionNumber() const
	{
		return pImpl->versionNumber;
	}

	void SkeletonRig::VersionNumber(int v)
	{
		pImpl->versionNumber = v;
	}

	uint32_t SkeletonRig::FileSize() const
	{
		return pImpl->fileSize;
	}

	void SkeletonRig::FileSize(uint32_t size)
	{
		pImpl->fileSize = size;
	}

	uint32_t SkeletonRig::HeaderSize() const
	{
		return pImpl->headerSize;
	}

	void SkeletonRig::HeaderSize(uint32_t size)
	{
		pImpl->headerSize = size;
	}

	uint32_t SkeletonRig::BoneMapOffset() const
	{
		return pImpl->boneMapOffset;
	}

	void SkeletonRig::BoneMapOffset(uint32_t offset)
	{
		pImpl->boneMapOffset = offset;
	}

	Utilities::VectorContainer<uint64_t> SkeletonRig::getMatchingThree() const
	{
		Utilities::VectorContainer<uint64_t> output(3);
		output.at(0) = pImpl->matchingThree[0];
		output.at(1) = pImpl->matchingThree[1];
		output.at(2) = pImpl->matchingThree[2];
		return output;
	}

	void SkeletonRig::setMatchingThree(uint64_t m1, uint64_t m2, uint64_t m3)
	{
		pImpl->matchingThree[0] = m1;
		pImpl->matchingThree[1] = m2;
		pImpl->matchingThree[2] = m3;
	}

	float SkeletonRig::LowPrecision() const
	{
		return pImpl->lowPrecision;
	}

	void SkeletonRig::LowPrecision(float value)
	{
		pImpl->lowPrecision = value;
	}

	float SkeletonRig::HighPrecision() const
	{
		return pImpl->highPrecision;
	}

	void SkeletonRig::HighPrecision(float value)
	{
		pImpl->highPrecision = value;
	}

	uint16_t SkeletonRig::BoneCount() const
	{
		return pImpl->boneCount;
	}

	void SkeletonRig::BoneCount(uint16_t count)
	{
		pImpl->boneCount = count;
	}

	uint16_t SkeletonRig::BoneCountAnimated() const
	{
		return pImpl->boneCount_Animated;
	}

	void SkeletonRig::BoneCountAnimated(uint16_t count)
	{
		pImpl->boneCount_Animated = count;
	}

	Utilities::VectorContainer<SkeletonBone>& SkeletonRig::BoneEntries() const
	{
		return pImpl->boneEntries;
	}

	Utilities::VectorContainer<int16_t> SkeletonRig::BoneMapArray() const
	{
		Utilities::VectorContainer<int16_t> output; output.reserve(SFBGSMAPSIZE);
		for (uint8_t i = 0; i < SFBGSMAPSIZE; i++)
		{
			output.push_back(pImpl->boneMapArray[i]);
		}
		return output;
	}

#ifdef _DEBUG
	Utilities::VectorContainer<char> CALUMI::SFBGS::SkeletonRig::EndOfHeader() const
	{
		Utilities::VectorContainer<char> output(16);
		for (uint8_t i = 0; i < 16; i++)
		{
			output.at(i) = pImpl->endOfHeader[i];
		}
		return output;
	}
#endif 

	Utilities::ExpectedContainer<bool, FileError> SkeletonRig::ReadFromFile(const wchar_t* inputFilePath)
	{
		Utilities::PathContainer output(inputFilePath);
		return ReadFromFile(output);
	}
	bool SkeletonRig::IsMarkedMannequin() const
	{
		for (size_t i = 0; i < pImpl->boneEntries.size(); i++)
		{
			if (pImpl->boneEntries.at(i).getTwistDriverMqnIndex() > 0)
				return true;
		}
		return false;
	}
	Utilities::ExpectedContainer<bool, FileError> SkeletonRig::ReadFromFile(Utilities::PathContainer& inputFilePath)
	{
		Utilities::VectorContainer<Utilities::StringContainer>vec; vec.push_back(".rig");
		auto buffer = CALUMI::ValidateFile(inputFilePath, vec, 80, 0, true);
		if (!buffer.has_value())
		{
			Utilities::ExpectedContainer<bool, FileError> tempOutput;
			tempOutput.SetErrorValue(buffer.error());
			return tempOutput;
		}

		//iterator tracking
		unsigned long long addressIndex = 0;

		//HEADER READING
		{
			std::memcpy(&pImpl->versionNumber, &buffer.value().at(addressIndex), sizeof(pImpl->versionNumber));
			addressIndex += sizeof(pImpl->versionNumber);

			std::memcpy(&pImpl->fileSize, &buffer.value().at(addressIndex), sizeof(pImpl->fileSize));
			addressIndex += sizeof(pImpl->fileSize);

			std::memcpy(&pImpl->headerSize, &buffer.value().at(addressIndex), sizeof(pImpl->headerSize));
			addressIndex += sizeof(pImpl->headerSize);

			std::memcpy(&pImpl->headerEmpty01, &buffer.value().at(addressIndex), sizeof(pImpl->headerEmpty01));
			addressIndex += sizeof(pImpl->headerEmpty01);

			std::memcpy(&pImpl->boneMapOffset, &buffer.value().at(addressIndex), sizeof(pImpl->boneMapOffset));
			addressIndex += sizeof(pImpl->boneMapOffset);

			std::memcpy(&pImpl->headerEmpty02, &buffer.value().at(addressIndex), sizeof(pImpl->headerEmpty02));
			addressIndex += sizeof(pImpl->headerEmpty02);

			std::memcpy(&pImpl->matchingThree, &buffer.value().at(addressIndex), sizeof(pImpl->matchingThree));
			addressIndex += sizeof(pImpl->matchingThree);

			std::memcpy(&pImpl->lowPrecision, &buffer.value().at(addressIndex), sizeof(pImpl->lowPrecision));
			addressIndex += sizeof(pImpl->lowPrecision);

			std::memcpy(&pImpl->highPrecision, &buffer.value().at(addressIndex), sizeof(pImpl->highPrecision));
			addressIndex += sizeof(pImpl->highPrecision);

			std::memcpy(&pImpl->boneCount, &buffer.value().at(addressIndex), sizeof(pImpl->boneCount));
			addressIndex += sizeof(pImpl->boneCount);

			std::memcpy(&pImpl->boneCount_Animated, &buffer.value().at(addressIndex), sizeof(pImpl->boneCount_Animated));
			addressIndex += sizeof(pImpl->boneCount_Animated);

			std::memcpy(&pImpl->headerEmpty03, &buffer.value().at(addressIndex), sizeof(pImpl->headerEmpty03));
			addressIndex += sizeof(pImpl->headerEmpty03);

			std::memcpy(&pImpl->endOfHeader, &buffer.value().at(addressIndex), sizeof(pImpl->endOfHeader));
			addressIndex += sizeof(pImpl->endOfHeader);
		}

		//WARN IF FILE SIZE VAR != BUFFER
		if (pImpl->fileSize != buffer.value().size()) std::println("WARNING: THE VARIABLE, FILE SIZE = {} DOES NOT MATCH THE BUFFER SIZE OF {}", pImpl->fileSize, buffer.value().size());

		//READ BONE ENTRIES IN ORDER
		{
			pImpl->boneEntries.reserve(pImpl->boneCount);
			for (unsigned short i = 0; i < pImpl->boneCount; i++)
			{
				pImpl->boneEntries.push_back(SkeletonBone(buffer.value(), addressIndex));
			}
		}


		//READ SUFFIX
		std::memcpy(&pImpl->boneMapArray, &buffer.value().at(addressIndex), sizeof(pImpl->boneMapArray));
		addressIndex += sizeof(pImpl->boneMapArray);

		//READ STRINGS
		pImpl->stringArray.reserve(pImpl->boneCount);
		for (uint16_t i = 0; i < pImpl->boneCount; i++)
		{
			pImpl->stringArray.push_back(&buffer.value().at(pImpl->boneEntries.at(i).getNameOffset()));
			addressIndex += (pImpl->stringArray.at(i).Length(true));
		}

		if (buffer.value().size() != addressIndex)
		{
			std::println("===========================================================");
			std::println("===========================================================");
			std::println("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==");
			//std::println("CURRENT  POS: {}",(void*)currentAddress);
			//std::println("CURRENT ITER: {}",addressIndex);
			std::println("BUFFER  SIZE: {}", buffer.value().size());
			std::println("BUFFER START: {}", (void*)buffer.value().data());
			std::println("Size+Start: {}", (void*)(buffer.value().size() + buffer.value().data()));
			std::println("===========================================================");
			std::cout << "Press ENTER to continue running the program." << std::endl;
			std::cin.get();
			std::println("===========================================================");
		}
		return true;
	}

	Utilities::ExpectedContainer<Utilities::StringContainer, FileError> SkeletonRig::WriteToFile(const wchar_t* outputFilePath)
	{
		Utilities::PathContainer output(outputFilePath);
		return WriteToFile(output);
	}

	Utilities::ExpectedContainer<Utilities::StringContainer, FileError> SkeletonRig::WriteToFile(Utilities::PathContainer& outputFilePath)
	{
		//LARGEST FILE: "D:/ModOrganizer/Starfield_Mod_Authoring_01/mods/ExtractedData/meshes/furniture/armillary/characterassets/skeleton.rig" at 18301 bytes

		Utilities::VectorContainer<char> buffer;
		buffer.reserve(18500);

		unsigned long long addressIndex = 0;
		std::vector<unsigned int> offsets = _getSFBGSRigStringOffsets(pImpl->stringArray);

		//HEADER
		{
			buffer.insert(buffer.end(), 80, 0);
			std::memcpy(&buffer.at(addressIndex), &pImpl->versionNumber, sizeof(pImpl->versionNumber));
			addressIndex += sizeof(pImpl->versionNumber);

			std::memcpy(&buffer.at(addressIndex), &pImpl->fileSize, sizeof(pImpl->fileSize));
			addressIndex += sizeof(pImpl->fileSize);

			std::memcpy(&buffer.at(addressIndex), &pImpl->headerSize, sizeof(pImpl->headerSize));
			addressIndex += sizeof(pImpl->headerSize);

			std::memcpy(&buffer.at(addressIndex), &pImpl->headerEmpty01, sizeof(pImpl->headerEmpty01));
			addressIndex += sizeof(pImpl->headerEmpty01);

			std::memcpy(&buffer.at(addressIndex), &pImpl->boneMapOffset, sizeof(pImpl->boneMapOffset));
			addressIndex += sizeof(pImpl->boneMapOffset);

			std::memcpy(&buffer.at(addressIndex), &pImpl->headerEmpty02, sizeof(pImpl->headerEmpty02));
			addressIndex += sizeof(pImpl->headerEmpty02);

			std::memcpy(&buffer.at(addressIndex), &pImpl->matchingThree, sizeof(pImpl->matchingThree));
			addressIndex += sizeof(pImpl->matchingThree);

			std::memcpy(&buffer.at(addressIndex), &pImpl->lowPrecision, sizeof(pImpl->lowPrecision));
			addressIndex += sizeof(pImpl->lowPrecision);

			std::memcpy(&buffer.at(addressIndex), &pImpl->highPrecision, sizeof(pImpl->highPrecision));
			addressIndex += sizeof(pImpl->highPrecision);

			std::memcpy(&buffer.at(addressIndex), &pImpl->boneCount, sizeof(pImpl->boneCount));
			addressIndex += sizeof(pImpl->boneCount);

			std::memcpy(&buffer.at(addressIndex), &pImpl->boneCount_Animated, sizeof(pImpl->boneCount_Animated));
			addressIndex += sizeof(pImpl->boneCount_Animated);

			std::memcpy(&buffer.at(addressIndex), &pImpl->headerEmpty03, sizeof(pImpl->headerEmpty03));
			addressIndex += sizeof(pImpl->headerEmpty03);

			std::memcpy(&buffer.at(addressIndex), &pImpl->endOfHeader, sizeof(pImpl->endOfHeader));
			addressIndex += sizeof(pImpl->endOfHeader);
		}

		//BONE ENTRIES
		{
			for (unsigned int i = 0; i < pImpl->boneEntries.size(); i++)
			{
				pImpl->boneEntries.at(i).setNameOffset(offsets.at(i));
				pImpl->boneEntries.at(i).SerializeIntoBuffer(buffer, addressIndex);
			}
		}

		//SUFFIX (PI SIZED SECTION)
		{
			buffer.insert(buffer.end(), sizeof(pImpl->boneMapArray), -1);

			//Setting header value to confirm offset. In case the SFBGS Rig Values were changed incorrectly by the user
			pImpl->boneMapOffset = static_cast<unsigned int>(addressIndex);
			std::memcpy(&buffer.at(16), &pImpl->boneMapOffset, sizeof(pImpl->boneMapOffset));

			std::memcpy(&buffer.at(addressIndex), &pImpl->boneMapArray, sizeof(pImpl->boneMapArray));
			addressIndex += sizeof(pImpl->boneMapArray);
		}

		//STRING ARRAY
		{
			for (uint16_t i = 0; i < pImpl->stringArray.size(); i++)
			{
				for (int j = 0; j < pImpl->stringArray.at(i).Length(); j++)
				{
					buffer.push_back(pImpl->stringArray.at(i).at(j));
				}
				buffer.push_back('\0');
				addressIndex += pImpl->stringArray.at(i).Length(true);
			}
		}

		if (pImpl->fileSize != addressIndex)
		{
			std::println("===========================================================");
			std::println("===========================================================");
			std::println("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==");
			//std::println("CURRENT  POS: {}",(void*)currentAddress);
			//std::println("CURRENT ITER: {}",addressIndex);
			std::println("BUFFER  SIZE: {}", buffer.size());
			std::println("FILE  SIZE: {}", pImpl->fileSize);
			std::println("===========================================================");
			std::cout << "Press ENTER to continue running the program." << std::endl;
			std::cin.get();
			std::println("===========================================================");
		}

		pImpl->fileSize = static_cast<unsigned int>(buffer.size());
		std::memcpy(&buffer.at(4), &pImpl->fileSize, sizeof(pImpl->fileSize));

		return CALUMI::WriteToBinaryFile(outputFilePath, buffer);
	}

	uint8_t SkeletonRig::DEBUG_CheckAssumedHeaderEntries()
	{
		uint8_t output = 0;

		if (pImpl->headerEmpty01 != 0)
			output += 0b1;
		if (pImpl->headerEmpty02 != 0)
			output += 0b10;
		if (pImpl->headerEmpty03 != 0)
			output += 0b100;

		if (pImpl->matchingThree[0] != pImpl->matchingThree[1] || pImpl->matchingThree[0] != pImpl->matchingThree[2])
			output += 0b1000;

		if (pImpl->headerSize != 0x50)
			output += 0b10000;

		if (pImpl->versionNumber != 5)
			output += 0b100000;

		int aCount = 0;
		for (size_t i = 0; i < pImpl->boneEntries.size(); i++)
		{
			if (pImpl->boneEntries.at(i).getBoneType() == SFBGS::BoneType::Default)
			{
				aCount++;
			}
		}
		if (aCount != pImpl->boneCount_Animated)
			output += 0b1000000;

		return output;
	}

#pragma endregion

	SkeletonRig ConvertToSFBGSRig(CALUMI::UNIV::SkeletonRig& inputRig)
	{
		SkeletonRig output;
		auto stringResult = CreateStringVectorFromRig(inputRig);

		auto sfbgsRigPackagePtr = dynamic_cast<SFBGS_RigPackage*>(inputRig.getRigPackageManager().GetPackage(SFBGS_RIG_PACKAGE));
		SFBGS_RigPackage sfbgsRigPackage;
		bool isMarkedMannequin = false;
		if (sfbgsRigPackagePtr)
		{
			sfbgsRigPackage = *sfbgsRigPackagePtr;
		}

		isMarkedMannequin = sfbgsRigPackage.IsMannequin();
		output.LowPrecision(sfbgsRigPackage.LowPrecisionValue());
		output.HighPrecision(sfbgsRigPackage.HighPrecisionValue());

		output.FileSize(output.FileSize()+static_cast<unsigned int>(stringResult.GetFinalOffset()));
		output.BoneMapOffset(output.BoneMapOffset() + static_cast<unsigned int>(80 + 96 * inputRig.BoneEntries().size()));
		output.FileSize(output.FileSize()+ output.BoneMapOffset() + SFBGSMAPSIZE*2);

		output.BoneCount(static_cast<uint16_t>(inputRig.BoneEntries().size()));

		uint16_t animatedBoneCount = 0;
		output.BoneEntries().reserve(output.BoneCount());
		for (unsigned int i = 0; i < output.BoneCount(); i++)
		{
			SkeletonBone toAdd;
			toAdd.pImpl->localRotation = inputRig.BoneEntries().at(i).LocalRotation();
			toAdd.pImpl->globalRotation = inputRig.BoneEntries().at(i).GlobalRotation();
			toAdd.pImpl->position = inputRig.BoneEntries().at(i).LocalPosition();
			toAdd.pImpl->nameOffset = static_cast<uint64_t>(stringResult.GetOffset(i));
			toAdd.pImpl->parentBoneIndex = inputRig.BoneEntries().at(i).GetParentBoneIndex();

			toAdd.pImpl->mirrorBoneIndex = (toAdd.pImpl->mirrorBoneIndex < 0 || toAdd.pImpl->mirrorBoneIndex >= inputRig.BoneEntries().size()) ? i : inputRig.BoneEntries().at(i).GetMirrorBoneIndex();

			if (!toAdd.SetBoneTypeFromUNIV(inputRig.BoneEntries().at(i)))
			{
				std::println("[CALUMI.Animation API] UNIV Rig: {} Bone: {} ({}) Bone Type: {} Is Not An Acceptable Type For SFBGS Skeleton Rigs! This Bone Will Remain As The Default Type", inputRig.RigName().c_str(), inputRig.BoneEntries().at(i).Name().c_str(), i, inputRig.BoneEntries().at(i).GetBoneTypeProperty()->GetTypeString());
			}
			if (isMarkedMannequin && toAdd.pImpl->boneType == BoneType::Twist)
			{
				toAdd.pImpl->twistDriverMqnIndex = toAdd.pImpl->parentBoneIndex;
			}
			else { animatedBoneCount++; }

			output.BoneEntries().push_back(toAdd);
		}

		output.BoneCountAnimated(animatedBoneCount);

		Utilities::VectorContainer<int16_t> vecPackage = ConvertSFBGSRigPackage(inputRig);
		output.BoneMapArray(vecPackage);
		

		output.StringArray().reserve(stringResult.Size());
		for (int j = 0; j < stringResult.Size(); j++)
		{
			output.StringArray().push_back(stringResult.GetString(j));
		}

		return output;
	}

	CALUMI::UNIV::SkeletonRig ConvertToUniversalRig(CALUMI::SFBGS::SkeletonRig& inputRig)
	{
		CALUMI::UNIV::SkeletonRig output;
		
		for (unsigned int i = 0; i < inputRig.BoneEntries().size(); i++)
		{
			SFBGS::SkeletonBone& bone = inputRig.BoneEntries().at(i);
			output.AddBoneToRig(bone.pImpl->localRotation,bone.pImpl->position,inputRig.StringArray().at(i).c_str(), bone.pImpl->parentBoneIndex, true);
			bone.SetBoneTypeToUNIV(output.BoneEntries().at(i));
			int setter = bone.pImpl->mirrorBoneIndex == i ? -1 : bone.pImpl->mirrorBoneIndex;
			output.BoneEntries().at(i).SetMirrorBoneIndex(setter);
		}
		
		CreateNewSFBGSRigPackage(output);
		if (SFBGS_RigPackage* sfbgsRigPackage = dynamic_cast<SFBGS_RigPackage*>(output.getRigPackageManager().GetPackage(SFBGS_RIG_PACKAGE)))
		{
			auto boneMapArray = inputRig.BoneMapArray();
			for (int key = 0; key < SFBGSMAPSIZE; key++)
			{
				if (boneMapArray.at(key) >= 0)
				{
					sfbgsRigPackage->AddBoneToMap(static_cast<BoneMapKey>(key), output.BoneEntries().at(boneMapArray.at(key)).Name().c_str(), true);
				}
			}
			if (inputRig.IsMarkedMannequin())
				sfbgsRigPackage->IsMannequin(true);

			sfbgsRigPackage->SetPrecisionValues(SFBGS::PrecisionSet::Custom, inputRig.LowPrecision(), inputRig.HighPrecision());
		}

		return output;
	}


}
}