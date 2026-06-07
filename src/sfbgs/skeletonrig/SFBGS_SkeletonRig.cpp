//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppExpressionWithoutSideEffects
#include "internalplatform.h"
#include "internalvectordef.h"
#include <io/FileValidation.h>
#include <limits>
#include <AnimStarfield>
#include <AnimFile>
#include <iostream>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <utilities/CALUMI_Utilities.h>
#include "sfbgs/skeletonrig/SFBGS_RigPackage.h"
#include "univ/skeletonrig/packages/UNIV_RigManifestPackage.h"
#include "univ/skeletonrig/packages/UNIV_RigMirrorPackage.h"

namespace CALUMI::SFBGS
{
	static std::vector<unsigned int> s_getSFBGSRigStringOffsets(const Utilities::StringList& stringEntries)
	{
		std::vector<unsigned int> output;
		//offset should be 80 + 96+boneCount + 314... Yes we are static casting twice, it's just to prevent an overflow message and clean up compiler messages
		auto offset = static_cast<unsigned int>(static_cast<size_t>(80) + 314 + 96 * stringEntries.size());

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
		Math::Quaternion m_localRotation;
		Math::Quaternion m_globalRotation;
		Math::Vector3 m_position;

		BoneType m_boneType = BoneType::Default;

		uint64_t m_nameOffset = 0;
		/// <summary>
		/// -1 for the root bone
		/// </summary>
		int32_t m_parentBoneIndex = -1;
		/// <summary>
		/// Twist influence override? Always points to parent on mannequin twist bones
		/// </summary>
		int32_t m_twistDriverMqnIndex = -1;
		/// <summary>
		/// Twist influence, -1 if skipped
		/// </summary>
		int32_t m_twistDriverIndex = -1;
		/// <summary>
		/// Always -1
		/// </summary>
		int32_t m_pad01 = -1;
		/// <summary>
		/// Same as this bone's index by default
		/// </summary>
		int32_t m_mirrorBoneIndex = 0;
		int32_t m_LOD = 0;
		/// <summary>
		/// Negative if pointing to parent
		/// </summary>
		float m_twistDriverWeight = 0;
		/// <summary>
		/// Always 0
		/// </summary>
		int32_t m_pad02 = 0;
		float m_unknownScalar = 0.0;
		int32_t m_pad03 = 0; //appears to be garbage/leftover memory from whatever block of memory was used during output at BGS

		Impl() = default;
	};

#ifdef DEBUG_BUILD
	int32_t SkeletonBone::pad01() const
	{
		return pImpl->m_pad01;
	}
	int32_t SkeletonBone::pad02() const
	{
		return pImpl->m_pad02;
	}
	float SkeletonBone::unknownScalar() const { return pImpl->m_unknownScalar; }
	void SkeletonBone::setUnknownScalar(const float value) const { pImpl->m_unknownScalar = value; }
	int32_t SkeletonBone::term08() const { return pImpl->m_pad03; }
	void SkeletonBone::setTerm08(const int32_t value) const { pImpl->m_pad03 = value; }
#endif

	SkeletonBone::SkeletonBone() : pImpl(new Impl) {}

	SkeletonBone::~SkeletonBone()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	SkeletonBone::SkeletonBone(Utilities::BufferObject& buffer, unsigned long long& addressIndex) : SkeletonBone()
	{
		//QUAT UNITS ARE SERIALIZED AS W-XYZ AND NEED TO BE REFORMATTED TO XYZ-W

		{
			float w;
			std::memcpy(&w, &buffer.at(addressIndex), sizeof(w));
			addressIndex += sizeof(w);
			pImpl->m_localRotation.setW(w);
		}

		{
			float f[3] = {};
			std::memcpy(&f, &buffer.at(addressIndex), sizeof(f));
			addressIndex += sizeof(f);
			pImpl->m_localRotation.setX(f[0]); pImpl->m_localRotation.setY(f[1]); pImpl->m_localRotation.setZ(f[2]);
		}

		{
			float w;
			std::memcpy(&w, &buffer.at(addressIndex), sizeof(w));
			addressIndex += sizeof(w);
			pImpl->m_globalRotation.setW(w);
		}

		{
			float f[3] = {};
			std::memcpy(&f, &buffer.at(addressIndex), sizeof(f));
			addressIndex += sizeof(f);
			pImpl->m_globalRotation.setX(f[0]); pImpl->m_globalRotation.setY(f[1]); pImpl->m_globalRotation.setZ(f[2]);
		}

		{
			float f[3] = {};
			std::memcpy(&f, &buffer.at(addressIndex), sizeof(f));
			addressIndex += sizeof(f);
			pImpl->m_position.setX(f[0]);
			pImpl->m_position.setY(f[1]);
			pImpl->m_position.setZ(f[2]);
		}



		std::memcpy(&pImpl->m_boneType, &buffer.at(addressIndex), sizeof(pImpl->m_boneType));
		addressIndex += sizeof(pImpl->m_boneType);

		std::memcpy(&pImpl->m_nameOffset, &buffer.at(addressIndex), sizeof(pImpl->m_nameOffset));
		addressIndex += sizeof(pImpl->m_nameOffset);

		std::memcpy(&pImpl->m_parentBoneIndex, &buffer.at(addressIndex), sizeof(pImpl->m_parentBoneIndex));
		addressIndex += sizeof(pImpl->m_parentBoneIndex);

		std::memcpy(&pImpl->m_twistDriverMqnIndex, &buffer.at(addressIndex), sizeof(pImpl->m_twistDriverMqnIndex));
		addressIndex += sizeof(pImpl->m_twistDriverMqnIndex);

		std::memcpy(&pImpl->m_twistDriverIndex, &buffer.at(addressIndex), sizeof(pImpl->m_twistDriverIndex));
		addressIndex += sizeof(pImpl->m_twistDriverIndex);

		std::memcpy(&pImpl->m_pad01, &buffer.at(addressIndex), sizeof(pImpl->m_pad01));
		addressIndex += sizeof(pImpl->m_pad01);

		std::memcpy(&pImpl->m_mirrorBoneIndex, &buffer.at(addressIndex), sizeof(pImpl->m_mirrorBoneIndex));
		addressIndex += sizeof(pImpl->m_mirrorBoneIndex);

		std::memcpy(&pImpl->m_LOD, &buffer.at(addressIndex), sizeof(pImpl->m_LOD));
		addressIndex += sizeof(pImpl->m_LOD);

		std::memcpy(&pImpl->m_twistDriverWeight, &buffer.at(addressIndex), sizeof(pImpl->m_twistDriverWeight));
		addressIndex += sizeof(pImpl->m_twistDriverWeight);

		std::memcpy(&pImpl->m_pad02, &buffer.at(addressIndex), sizeof(pImpl->m_pad02));
		addressIndex += sizeof(pImpl->m_pad02);

		std::memcpy(&pImpl->m_unknownScalar, &buffer.at(addressIndex), sizeof(pImpl->m_unknownScalar));
		addressIndex += sizeof(pImpl->m_unknownScalar);

		std::memcpy(&pImpl->m_pad03, &buffer.at(addressIndex), sizeof(pImpl->m_pad03));
		addressIndex += sizeof(pImpl->m_pad03);
	}
	SkeletonBone::SkeletonBone(const SkeletonBone& input) : SkeletonBone()
	{
		*this = input;
	}
	SkeletonBone& SkeletonBone::operator=(const SkeletonBone& input)
	{
		if (this != &input)
			*pImpl = *input.pImpl;

		return *this;
	}

	Math::Quaternion& SkeletonBone::localRotation() const { return pImpl->m_localRotation; }
	Math::Quaternion& SkeletonBone::globalRotation() const { return pImpl->m_globalRotation; }
	Math::Vector3& SkeletonBone::position() const { return pImpl->m_position; }
	SkeletonBone::BoneType SkeletonBone::boneType() const { return pImpl->m_boneType; }
	void SkeletonBone::setBoneType(const BoneType t) const { pImpl->m_boneType = t; }
	uint64_t SkeletonBone::nameOffset() const { return pImpl->m_nameOffset; }
	void SkeletonBone::setNameOffset(const uint64_t offset) const { pImpl->m_nameOffset = offset; }
	int32_t SkeletonBone::parentBoneIndex() const { return pImpl->m_parentBoneIndex; }
	void SkeletonBone::setParentBoneIndex(const int32_t idx) const { pImpl->m_parentBoneIndex = idx; }
	int32_t SkeletonBone::twistDriverMqnIndex() const { return pImpl->m_twistDriverMqnIndex; }
	void SkeletonBone::setTwistDriverMqnIndex(const int32_t idx) const { pImpl->m_twistDriverMqnIndex = idx; }
	int32_t SkeletonBone::twistDriverIndex() const { return pImpl->m_twistDriverIndex; }
	void SkeletonBone::setTwistDriverIndex(const int32_t idx) const { pImpl->m_twistDriverIndex = idx; }
	int32_t SkeletonBone::mirrorBoneIndex() const { return pImpl->m_mirrorBoneIndex; }
	void SkeletonBone::setMirrorBoneIndex(const int32_t idx) const { pImpl->m_mirrorBoneIndex = idx; }
	int32_t SkeletonBone::levelOfDetail() const { return pImpl->m_LOD; }
	void SkeletonBone::setLevelOfDetail(const int32_t value) const { pImpl->m_LOD = value; }
	float SkeletonBone::twistDriverWeight() const { return pImpl->m_twistDriverWeight; }
	void SkeletonBone::setTwistDriverWeight(const float weight) const { pImpl->m_twistDriverWeight = weight; }


	void SkeletonBone::serializeIntoBuffer(Utilities::BufferObject& buffer, unsigned long long& addressIndex) const
	{
		buffer.insert(buffer.endPos(), 96, 0);

		//QUAT UNITS ARE SERIALIZED AS W-XYZ AND NEED TO BE REFORMATTED FROM XYZ-W
		{
			const float w = pImpl->m_localRotation.w();
			const float f[3] = { pImpl->m_localRotation.x(),pImpl->m_localRotation.y(), pImpl->m_localRotation.z() };

			std::memcpy(&buffer.at(addressIndex), &w, sizeof(w));
			addressIndex += sizeof(w);

			std::memcpy(&buffer.at(addressIndex), &f, sizeof(f));
			addressIndex += sizeof(f);
		}

		{
			const float w = pImpl->m_globalRotation.w();
			const float f[3] = { pImpl->m_globalRotation.x(),pImpl->m_globalRotation.y(), pImpl->m_globalRotation.z() };

			std::memcpy(&buffer.at(addressIndex), &w, sizeof(w));
			addressIndex += sizeof(w);

			std::memcpy(&buffer.at(addressIndex), &f, sizeof(f));
			addressIndex += sizeof(f);
		}

		{
			const float f[3] = { pImpl->m_position.x(), pImpl->m_position.y(), pImpl->m_position.z()};

			std::memcpy(&buffer.at(addressIndex), f, sizeof(f));
			addressIndex += sizeof(f);
		}

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_boneType, sizeof(pImpl->m_boneType));
		addressIndex += sizeof(pImpl->m_boneType);

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_nameOffset, sizeof(pImpl->m_nameOffset));
		addressIndex += sizeof(pImpl->m_nameOffset);

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_parentBoneIndex, sizeof(pImpl->m_parentBoneIndex));
		addressIndex += sizeof(pImpl->m_parentBoneIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_twistDriverMqnIndex, sizeof(pImpl->m_twistDriverMqnIndex));
		addressIndex += sizeof(pImpl->m_twistDriverMqnIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_twistDriverIndex, sizeof(pImpl->m_twistDriverIndex));
		addressIndex += sizeof(pImpl->m_twistDriverIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_pad01, sizeof(pImpl->m_pad01));
		addressIndex += sizeof(pImpl->m_pad01);

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_mirrorBoneIndex, sizeof(pImpl->m_mirrorBoneIndex));
		addressIndex += sizeof(pImpl->m_mirrorBoneIndex);

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_LOD, sizeof(pImpl->m_LOD));
		addressIndex += sizeof(pImpl->m_LOD);

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_twistDriverWeight, sizeof(pImpl->m_twistDriverWeight));
		addressIndex += sizeof(pImpl->m_twistDriverWeight);

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_pad02, sizeof(pImpl->m_pad02));
		addressIndex += sizeof(pImpl->m_pad02);

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_unknownScalar, sizeof(pImpl->m_unknownScalar));
		addressIndex += sizeof(pImpl->m_unknownScalar);

		std::memcpy(&buffer.at(addressIndex), &pImpl->m_pad03, sizeof(pImpl->m_pad03));
		addressIndex += sizeof(pImpl->m_pad03);
	}

	UNIV::BoneTypeProperty::BoneType SkeletonBone::boneTypeAsUNIVEnum() const
	{
		switch (pImpl->m_boneType)
		{
		case BoneType::Default:
			return UNIV::BoneTypeProperty::BoneType::Default;
		case BoneType::Twist:
			return UNIV::BoneTypeProperty::BoneType::Twist;
		default:
			return UNIV::BoneTypeProperty::BoneType::UNDEFINED;
		}
	}

	const char* SkeletonBone::boneTypeAsString() const
	{
		switch (pImpl->m_boneType)
		{
		case BoneType::Default:
			return "Default";
		case BoneType::Twist:
			return "Twist";
		default:
			return "SFBGS UNDEFINED";
		}
	}

#pragma endregion

	VECTORDEF(SkeletonBoneVector, SkeletonBone)

#pragma region SKELETONRIG

	struct SkeletonRig::Impl
	{
		int m_versionNumber = 05;

		uint32_t m_fileSize = 0;

		/// <summary>
		/// Currently the only value seen is 0x50 (80)
		/// </summary>
		uint64_t m_headerSize = 0x50;

		/// <summary>
		/// Always empty, possibly padding, see _PRIVATE_ for placement in struct
		/// </summary>
		//uint32_t _headerEmpty01 = 0;
		/// <summary>
		/// 96* bone count + 80 bytes
		/// </summary>
		uint64_t m_boneMapOffset = 0;

		/// <summary>
		/// Always empty, possibly padding, see _PRIVATE_ for placement in struct
		/// </summary>
		//uint32_t _headerEmpty02 = 0;
		/// <summary>
			/// No matter what, these three ALWAYS match. Could be internal number tracking for BGS and may not matter to anyone outside the company
			/// </summary>
		uint64_t m_matchingThree[3] = { 0,0,0 };
		/// <summary>
		/// default precision values. For ships use 0.25. For first person use 0.0078125 (1/128)
		/// </summary>
		float m_lowPrecision = 0.03125f;
		/// <summary>
		/// default precision values. For ships use 0.002. For first person use 6.25e-5 (1/16000)
		/// </summary>
		float m_highPrecision = 0.00025f;

		uint16_t m_boneCount = 0;
		uint16_t m_boneCountAnimated = 0;
		/// <summary>
		/// Always empty, possibly padding
		/// </summary>
		uint32_t m_headerEmpty03 = 0;
		/// <summary>
		/// ;)
		/// </summary>
		uint8_t m_endOfHeader[16] = { 0x0, 0x0, 0x43, 0x41, 0x4C, 0x55, 0x4D, 0x49, 0x44, 0x56, 0x52, 0x53, 0x4A, 0x4F, 0x4A, 0x4F };

		SkeletonBoneVector m_boneEntries;

		//Don't ask why I'm initializing like this... let's just move on.
		int16_t m_boneMapArray[SFBGSMAPSIZE] =
		{
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1
		};

		/// <summary>
		/// String array for the bone names
		/// </summary>
		Utilities::StringList m_stringArray;
		Impl() = default;
	};

	SkeletonRig::SkeletonRig() : pImpl(new Impl) {}

	SkeletonRig::~SkeletonRig()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	SkeletonRig::SkeletonRig(const SkeletonRig& input) : SkeletonRig()
	{
		*this = input;
	}
	SkeletonRig& SkeletonRig::operator=(const SkeletonRig& input)
	{
		if (this != &input)
			*pImpl = *input.pImpl;

		return *this;
	}

	SkeletonRig::SkeletonRig(const UNIV::SkeletonRig& input) : SkeletonRig()
	{
		const auto& manifestPkg = UNIV::RigManifestPackage::GetPackage(input);
		const auto& mirrorPkg = UNIV::RigMirrorPackage::GetPackage(input);
		const auto& sfbgsPackage = SFBGS_RigPackage::GetPackage(input);

		pImpl->m_stringArray = manifestPkg.processPackage(input);

		bool isMarkedMannequin = false;

		isMarkedMannequin = sfbgsPackage.isMannequin();
		setLowPrecision(sfbgsPackage.precisionSet().low());
		setHighPrecision(sfbgsPackage.precisionSet().high());

		setFileSize(fileSize() + static_cast<unsigned int>(pImpl->m_stringArray.finalOffset()));
		setBoneMapOffset(boneMapOffset() + (80 + 96 * input.boneCount()));
		setFileSize(fileSize() + boneMapOffset() + SFBGSMAPSIZE * 2);

		setBoneCount(static_cast<uint16_t>(input.boneCount()));

		uint16_t animatedBoneCount = 0;
		boneEntries().reserve(boneCount());

		for (int i = 0; i < boneCount() && i < pImpl->m_stringArray.size(); i++)
		{
			const auto refBone = input.bone(pImpl->m_stringArray.c_str(i));

			SkeletonBone toAdd;
			if (refBone)
			{
				const auto& localTransform = refBone->localTransform();
				const auto globalTransform = refBone->globalTransform();
				toAdd.pImpl->m_localRotation = localTransform.rotation();
				toAdd.pImpl->m_globalRotation = globalTransform.rotation();
				toAdd.pImpl->m_position = localTransform.position();
				toAdd.pImpl->m_nameOffset = pImpl->m_stringArray.offset(i) + boneMapOffset() + SFBGSMAPSIZE * 2;

				if (refBone->isRoot())
				{
					//The root entry from our UNIV rig
					//which in SFBGS has no parent index and is set to -1
					toAdd.pImpl->m_parentBoneIndex = -1;
				}
				else
				{
					const auto parentBone = dynamic_cast<const UNIV::SkeletonBone*>(refBone->parent());

					if (parentBone)
					{
						toAdd.pImpl->m_parentBoneIndex = static_cast<int32_t>(pImpl->m_stringArray.find(parentBone->name(),0));
					}
					else
					{
						toAdd.pImpl->m_parentBoneIndex = 0;
					}
				}

				toAdd.pImpl->m_mirrorBoneIndex = static_cast<int32_t>(pImpl->m_stringArray.find(mirrorPkg.pairedBone(refBone->name()), i));

				// ReSharper disable once CppExpressionWithoutSideEffects
				setBoneTypeFromUNIV(*refBone, toAdd, &pImpl->m_stringArray);

				if (isMarkedMannequin && toAdd.pImpl->m_boneType == SkeletonBone::BoneType::Twist)
				{
					toAdd.pImpl->m_twistDriverMqnIndex = toAdd.pImpl->m_parentBoneIndex;
				}
				else
				{
					animatedBoneCount++;
				}

				auto lod = sfbgsPackage.boneLod(refBone->name());

				if (lod == SFBGS_RigPackage::LODSetting::UNDEFINED)
					lod = SFBGS_RigPackage::DefaultLOD;

				toAdd.pImpl->m_LOD = static_cast<int32_t>(lod);
			}
			boneEntries().push_back(toAdd);
		}

		setBoneCountAnimated(animatedBoneCount);

		Utilities::S16Vector vecPackage = sfbgsPackage.convertMap(pImpl->m_stringArray);
		setBoneMapArray(vecPackage);
	}

	void SkeletonRig::setBoneMapArray(Utilities::S16Vector& input) const
	{
		for (uint8_t j = 0; j < input.size() && j < std::numeric_limits<uint8_t>::max(); j++)
		{
			pImpl->m_boneMapArray[j] = input.at(j);
		}
	}

	Utilities::StringList& SkeletonRig::stringArray() const
	{
		return pImpl->m_stringArray;
	}

	int SkeletonRig::versionNumber() const
	{
		return pImpl->m_versionNumber;
	}

	void SkeletonRig::setVersionNumber(const int v) const
	{
		pImpl->m_versionNumber = v;
	}

	uint32_t SkeletonRig::fileSize() const
	{
		return pImpl->m_fileSize;
	}

	void SkeletonRig::setFileSize(const uint32_t size) const
	{
		pImpl->m_fileSize = size;
	}

	uint64_t SkeletonRig::headerSize() const
	{
		return pImpl->m_headerSize;
	}

	void SkeletonRig::setHeaderSize(const uint64_t size) const
	{
		pImpl->m_headerSize = size;
	}

	uint64_t SkeletonRig::boneMapOffset() const
	{
		return pImpl->m_boneMapOffset;
	}

	void SkeletonRig::setBoneMapOffset(const uint64_t offset) const
	{
		pImpl->m_boneMapOffset = offset;
	}

	Utilities::U64Vector SkeletonRig::matchingThree() const
	{
		Utilities::U64Vector output;
		output.resize(3);
		output.at(0) = pImpl->m_matchingThree[0];
		output.at(1) = pImpl->m_matchingThree[1];
		output.at(2) = pImpl->m_matchingThree[2];
		return output;
	}

	void SkeletonRig::setMatchingThree(const uint64_t m1, const uint64_t m2, const uint64_t m3) const
	{
		pImpl->m_matchingThree[0] = m1;
		pImpl->m_matchingThree[1] = m2;
		pImpl->m_matchingThree[2] = m3;
	}

	float SkeletonRig::lowPrecision() const
	{
		return pImpl->m_lowPrecision;
	}

	void SkeletonRig::setLowPrecision(const float value) const
	{
		pImpl->m_lowPrecision = value;
	}

	float SkeletonRig::highPrecision() const
	{
		return pImpl->m_highPrecision;
	}

	void SkeletonRig::setHighPrecision(const float value) const
	{
		pImpl->m_highPrecision = value;
	}

	uint16_t SkeletonRig::boneCount() const
	{
		return pImpl->m_boneCount;
	}

	void SkeletonRig::setBoneCount(const uint16_t count) const
	{
		pImpl->m_boneCount = count;
	}

	uint16_t SkeletonRig::boneCountAnimated() const
	{
		return pImpl->m_boneCountAnimated;
	}

	void SkeletonRig::setBoneCountAnimated(const uint16_t count) const
	{
		pImpl->m_boneCountAnimated = count;
	}

	SkeletonBoneVector& SkeletonRig::boneEntries() const
	{
		return pImpl->m_boneEntries;
	}

	Utilities::S16Vector SkeletonRig::boneMapArray() const
	{
		Utilities::S16Vector output;
		output.reserve(SFBGSMAPSIZE);
		for (int16_t i : pImpl->m_boneMapArray)
		{
			output.push_back(i);
		}
		return output;
	}

#ifdef DEBUG_BUILD
	Utilities::CharVector SkeletonRig::endOfHeader() const
	{
		Utilities::CharVector output;
		output.resize(16);
		for (uint8_t i = 0; i < 16; i++)
		{
			output.at(i) = static_cast<char>(pImpl->m_endOfHeader[i]);
		}
		return output;
	}
#endif

	Utilities::FileResult SkeletonRig::readFromFile(Utilities::PathContainer&& inputFilePath)
	{
		Utilities::PathContainer output(inputFilePath);
		return readFromFile(output);
	}
	bool SkeletonRig::isMarkedMannequin() const
	{
		for (uint64_t i = 0; i < pImpl->m_boneEntries.size(); i++)
		{
			if (pImpl->m_boneEntries.at(i).twistDriverMqnIndex() > 0)
				return true;
		}
		return false;
	}
	Utilities::FileResult SkeletonRig::readFromFile(Utilities::PathContainer& inputFilePath)
	{
		const Utilities::StringList vec;
		vec.push_back(".rig");

		auto buffer = ValidateFile(inputFilePath, vec, 80, 0, true);
		if (buffer.result().hasError())
		{
			return buffer.result();
		}

		//iterator tracking
		unsigned long long addressIndex = 0;

		//HEADER READING
		{
			std::memcpy(&pImpl->m_versionNumber, &buffer.at(addressIndex), sizeof(pImpl->m_versionNumber));
			addressIndex += sizeof(pImpl->m_versionNumber);

			std::memcpy(&pImpl->m_fileSize, &buffer.at(addressIndex), sizeof(pImpl->m_fileSize));
			addressIndex += sizeof(pImpl->m_fileSize);

			std::memcpy(&pImpl->m_headerSize, &buffer.at(addressIndex), sizeof(pImpl->m_headerSize));
			addressIndex += sizeof(pImpl->m_headerSize);

			std::memcpy(&pImpl->m_boneMapOffset, &buffer.at(addressIndex), sizeof(pImpl->m_boneMapOffset));
			addressIndex += sizeof(pImpl->m_boneMapOffset);

			std::memcpy(&pImpl->m_matchingThree, &buffer.at(addressIndex), sizeof(pImpl->m_matchingThree));
			addressIndex += sizeof(pImpl->m_matchingThree);

			std::memcpy(&pImpl->m_lowPrecision, &buffer.at(addressIndex), sizeof(pImpl->m_lowPrecision));
			addressIndex += sizeof(pImpl->m_lowPrecision);

			std::memcpy(&pImpl->m_highPrecision, &buffer.at(addressIndex), sizeof(pImpl->m_highPrecision));
			addressIndex += sizeof(pImpl->m_highPrecision);

			std::memcpy(&pImpl->m_boneCount, &buffer.at(addressIndex), sizeof(pImpl->m_boneCount));
			addressIndex += sizeof(pImpl->m_boneCount);

			std::memcpy(&pImpl->m_boneCountAnimated, &buffer.at(addressIndex), sizeof(pImpl->m_boneCountAnimated));
			addressIndex += sizeof(pImpl->m_boneCountAnimated);

			std::memcpy(&pImpl->m_headerEmpty03, &buffer.at(addressIndex), sizeof(pImpl->m_headerEmpty03));
			addressIndex += sizeof(pImpl->m_headerEmpty03);

			std::memcpy(&pImpl->m_endOfHeader, &buffer.at(addressIndex), sizeof(pImpl->m_endOfHeader));
			addressIndex += sizeof(pImpl->m_endOfHeader);
		}

#ifdef DEBUG_BUILD
		//WARN IF FILE SIZE VAR != BUFFER
		if (pImpl->m_fileSize != buffer.size())
			std::cout << std::format("WARNING: THE VARIABLE, FILE SIZE = {} DOES NOT MATCH THE BUFFER SIZE OF {}", pImpl->m_fileSize, buffer.size()).c_str() << std::endl;
#endif

		//READ BONE ENTRIES IN ORDER
		{
			pImpl->m_boneEntries.reserve(pImpl->m_boneCount);
			for (unsigned short i = 0; i < pImpl->m_boneCount; i++)
			{
				pImpl->m_boneEntries.push_back(SkeletonBone(buffer, addressIndex));
			}
		}


		//READ SUFFIX
		std::memcpy(&pImpl->m_boneMapArray, &buffer.at(addressIndex), sizeof(pImpl->m_boneMapArray));
		addressIndex += sizeof(pImpl->m_boneMapArray);

		//READ STRINGS
		pImpl->m_stringArray.reserve(pImpl->m_boneCount);
		// for (uint16_t i = 0; i < pImpl->m_boneCount; i++)
		// {
		// 	pImpl->m_stringArray.push_back(&buffer.at(pImpl->m_boneEntries.at(i).nameOffset()),pImpl->m_boneEntries.at(i).nameOffset());
		// 	addressIndex += pImpl->m_stringArray.stringLength(i, true);
		// }
		unsigned int strCount = 0;
		while (addressIndex < pImpl->m_fileSize)
		{
			const char* str = &buffer.at(addressIndex);
			pImpl->m_stringArray.push_back(str, addressIndex);
			addressIndex += pImpl->m_stringArray.stringLength(strCount, true);
			strCount++;
		}


#ifdef DEBUG_BUILD
		if (buffer.size() != addressIndex)
		{
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << std::format("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==").c_str() << std::endl;
			std::cout << std::format("BUFFER  SIZE: {}", buffer.size()).c_str() << std::endl;
			std::cout << std::format("BUFFER START: {}", static_cast<void*>(buffer.data())).c_str() << std::endl;
			std::cout << std::format("Size+Start: {}", static_cast<void*>(buffer.size() + buffer.data())). c_str() << std::endl;
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << "Press ENTER to continue running the program." << std::endl;
			std::cin.get();
			std::cout << std::format("===========================================================").c_str() << std::endl;
		}
#endif

		return {Utilities::FileResult::FileErrorCode::Success, inputFilePath.w_str(), ""};
	}

	Utilities::FileResult SkeletonRig::writeToFile(Utilities::PathContainer&& outputFilePath)
	{
		Utilities::PathContainer output(outputFilePath);
		return writeToFile(output);
	}

	Utilities::FileResult SkeletonRig::writeToFile(Utilities::PathContainer& outputFilePath)
	{
		//LARGEST FILE: "D:/ModOrganizer/Starfield_Mod_Authoring_01/mods/ExtractedData/meshes/furniture/armillary/characterassets/skeleton.rig" at 18301 bytes

		Utilities::FileBufferResult buffer;
		buffer.reserve(18500);

		unsigned long long addressIndex = 0;
		const std::vector<unsigned int> offsets = s_getSFBGSRigStringOffsets(pImpl->m_stringArray);

		//HEADER
		{
			buffer.insert(buffer.endPos(), 80, 0);
			std::memcpy(&buffer.at(addressIndex), &pImpl->m_versionNumber, sizeof(pImpl->m_versionNumber));
			addressIndex += sizeof(pImpl->m_versionNumber);

			std::memcpy(&buffer.at(addressIndex), &pImpl->m_fileSize, sizeof(pImpl->m_fileSize));
			addressIndex += sizeof(pImpl->m_fileSize);

			std::memcpy(&buffer.at(addressIndex), &pImpl->m_headerSize, sizeof(pImpl->m_headerSize));
			addressIndex += sizeof(pImpl->m_headerSize);

			std::memcpy(&buffer.at(addressIndex), &pImpl->m_boneMapOffset, sizeof(pImpl->m_boneMapOffset));
			addressIndex += sizeof(pImpl->m_boneMapOffset);

			std::memcpy(&buffer.at(addressIndex), &pImpl->m_matchingThree, sizeof(pImpl->m_matchingThree));
			addressIndex += sizeof(pImpl->m_matchingThree);

			std::memcpy(&buffer.at(addressIndex), &pImpl->m_lowPrecision, sizeof(pImpl->m_lowPrecision));
			addressIndex += sizeof(pImpl->m_lowPrecision);

			std::memcpy(&buffer.at(addressIndex), &pImpl->m_highPrecision, sizeof(pImpl->m_highPrecision));
			addressIndex += sizeof(pImpl->m_highPrecision);

			std::memcpy(&buffer.at(addressIndex), &pImpl->m_boneCount, sizeof(pImpl->m_boneCount));
			addressIndex += sizeof(pImpl->m_boneCount);

			std::memcpy(&buffer.at(addressIndex), &pImpl->m_boneCountAnimated, sizeof(pImpl->m_boneCountAnimated));
			addressIndex += sizeof(pImpl->m_boneCountAnimated);

			std::memcpy(&buffer.at(addressIndex), &pImpl->m_headerEmpty03, sizeof(pImpl->m_headerEmpty03));
			addressIndex += sizeof(pImpl->m_headerEmpty03);

			std::memcpy(&buffer.at(addressIndex), &pImpl->m_endOfHeader, sizeof(pImpl->m_endOfHeader));
			addressIndex += sizeof(pImpl->m_endOfHeader);
		}

		//BONE ENTRIES
		{
			for (unsigned int i = 0; i < pImpl->m_boneEntries.size(); i++)
			{
				pImpl->m_boneEntries.at(i).setNameOffset(offsets.at(i));
				pImpl->m_boneEntries.at(i).serializeIntoBuffer(buffer, addressIndex);
			}
		}

		//SUFFIX (PI SIZED SECTION)
		{
			buffer.insert(buffer.endPos(), sizeof(pImpl->m_boneMapArray), -1);

			//Setting header value to confirm offset. In case the SFBGS Rig Values were changed incorrectly by the user
			pImpl->m_boneMapOffset = static_cast<unsigned int>(addressIndex);
			std::memcpy(&buffer.at(16), &pImpl->m_boneMapOffset, sizeof(pImpl->m_boneMapOffset));

			std::memcpy(&buffer.at(addressIndex), &pImpl->m_boneMapArray, sizeof(pImpl->m_boneMapArray));
			addressIndex += sizeof(pImpl->m_boneMapArray);
		}

		//STRING ARRAY
		{
			for (uint16_t i = 0; i < pImpl->m_stringArray.size() && i < std::numeric_limits<uint16_t>::max(); i++)
			{
				for (int j = 0; j < pImpl->m_stringArray.stringLength(i); j++)
				{
					buffer.push_back(*(pImpl->m_stringArray.c_str(i) + j));
				}
				buffer.push_back('\0');
				addressIndex += pImpl->m_stringArray.stringLength(i, true);
			}
		}

#ifdef DEBUG_BUILD
		if (pImpl->m_fileSize != addressIndex)
		{
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << std::format("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==").c_str() << std::endl;
			std::cout << std::format("BUFFER  SIZE: {}", buffer.size()).c_str() << std::endl;
			std::cout << std::format("FILE  SIZE: {}", pImpl->m_fileSize).c_str() << std::endl;
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << "Press ENTER to continue running the program." << std::endl;
			std::cin.get();
			std::cout << std::format("===========================================================").c_str() << std::endl;
		}
#endif

		pImpl->m_fileSize = static_cast<unsigned int>(buffer.size());
		std::memcpy(&buffer.at(4), &pImpl->m_fileSize, sizeof(pImpl->m_fileSize));

		return WriteToBinaryFile(outputFilePath, buffer);
	}


#ifdef DEBUG_BUILD
	uint8_t SkeletonRig::checkAssumedHeaderEntries() const
	{
		uint8_t output = 0;

		if (pImpl->m_headerSize > pImpl->m_fileSize)
		 	output += 0b1;
		if (pImpl->m_boneMapOffset > pImpl->m_fileSize)
		 	output += 0b10;
		if (pImpl->m_headerEmpty03 != 0)
			output += 0b100;

		if (pImpl->m_matchingThree[0] != pImpl->m_matchingThree[1] || pImpl->m_matchingThree[0] != pImpl->m_matchingThree[2])
			output += 0b1000;

		if (pImpl->m_headerSize != 0x50)
			output += 0b10000;

		if (pImpl->m_versionNumber != 5)
			output += 0b100000;

		int aCount = 0;
		for (uint64_t i = 0; i < pImpl->m_boneEntries.size(); i++)
		{
			if (pImpl->m_boneEntries.at(i).boneType() == SkeletonBone::BoneType::Default)
			{
				aCount++;
			}
		}
		if (aCount != pImpl->m_boneCountAnimated)
			output += 0b1000000;

		return output;
	}
#endif

#pragma endregion

	void SkeletonRig::convertFromUniversalRig(const UNIV::SkeletonRig& inputRig)
	{
		*this = SkeletonRig(inputRig);
	}

	bool SkeletonRig::setBoneTypeFromUNIV(const UNIV::SkeletonBone& univBone, const SkeletonBone& sfbgsBone, const Utilities::StringList* boneList) const
	{
		switch (univBone.boneTypeProperty()->type())
		{
		case UNIV::BoneTypeProperty::BoneType::Default:
			sfbgsBone.pImpl->m_boneType = SkeletonBone::BoneType::Default;
			sfbgsBone.pImpl->m_twistDriverIndex = -1;
			sfbgsBone.pImpl->m_twistDriverMqnIndex = -1;
			sfbgsBone.pImpl->m_twistDriverWeight = 0.0;
			break;
		case UNIV::BoneTypeProperty::BoneType::Twist:
			{
				sfbgsBone.pImpl->m_boneType = SkeletonBone::BoneType::Twist;

				const auto tProp = dynamic_cast<UNIV::TwistBoneProperty*>(const_cast<UNIV::BoneTypeProperty*>(univBone.boneTypeProperty()));

				int driverIndex = -1;

				if (boneList)
					driverIndex = static_cast<int>(boneList->find(tProp->twistDriver(), -1));
				else
					driverIndex = findBoneIndex(tProp->twistDriver());

				sfbgsBone.pImpl->m_twistDriverIndex = driverIndex;
				sfbgsBone.pImpl->m_twistDriverWeight = tProp->twistDriverWeight();
			}
			break;
		default:
			return false;
		}
		return true;
	}

	bool SkeletonRig::setBoneTypeToUNIV(const SkeletonBone& sfbgsBone, const UNIV::SkeletonBone& univBone) const
	{
		if (!univBone.setBoneTypeProperty(sfbgsBone.boneTypeAsUNIVEnum()))
			return false;

		switch (univBone.boneTypeProperty()->type())
		{
		case UNIV::BoneTypeProperty::BoneType::Default:
			break;
		case UNIV::BoneTypeProperty::BoneType::Twist:
			{
				const auto tProp = dynamic_cast<UNIV::TwistBoneProperty*>(const_cast<UNIV::BoneTypeProperty*>(univBone.boneTypeProperty()));

				if (const int idx = sfbgsBone.pImpl->m_twistDriverIndex; idx >= 0)
					tProp->setTwistDriver(pImpl->m_stringArray.c_str(idx));

				tProp->setTwistDriverWeight(sfbgsBone.pImpl->m_twistDriverWeight);
			}
			break;
		default:
			break;
		}
		return true;
	}

	int SkeletonRig::findBoneIndex(const char* boneName) const
	{
		for (uint64_t i = 0; i < pImpl->m_stringArray.size() && i < std::numeric_limits<int>::max(); i++)
		{
			if (SCOMPARE(pImpl->m_stringArray.c_str(i), boneName) == 0)
				return static_cast<int>(i);
		}

		return -1;
	}

	SkeletonBone* SkeletonRig::boneEntry(const int index) const
	{
		if (index < 0 || index >= pImpl->m_boneEntries.size())
			return nullptr;

		return &pImpl->m_boneEntries.at(index);
	}

	UNIV::SkeletonRig SkeletonRig::convertToUniversalRig() const
	{
		UNIV::SkeletonRig output;
		const auto& mirrorPackage = UNIV::RigMirrorPackage::GetPackage(output);
		const auto& manifestPackage = UNIV::RigManifestPackage::GetPackage(output);
		const auto& sfbgsPackage = SFBGS_RigPackage::GetPackage(output);

		std::unordered_map<std::string, const UNIV::SkeletonBone*> uPtrs;

		for (unsigned int i = 0; i < pImpl->m_boneEntries.size(); i++)
		{
			const SkeletonBone& bone = pImpl->m_boneEntries.at(i);
			const int pIdx = bone.parentBoneIndex();
			const UNIV::SkeletonBone* uParentPtr = nullptr;

			if (pIdx != -1 && uPtrs.contains(pImpl->m_stringArray.c_str(pIdx)))
			{
				uParentPtr = uPtrs[pImpl->m_stringArray.c_str(pIdx)];
			}

			if (const auto boneName = pImpl->m_stringArray.stringAt(bone.nameOffset());
				!manifestPackage.hasBone(boneName))
			{
				manifestPackage.addBone(boneName);
			}

			//If the parent hasn't been added then we move on and will return to this bone later
			if (!uParentPtr && pIdx != -1)
				continue;

			//If this bone has already been added, we skip
			if (uPtrs.contains(pImpl->m_stringArray.c_str(i)))
				continue;

			const UNIV::SkeletonBone* uAddedBone = nullptr;

			if (pIdx == -1)
			{
				uAddedBone = output.root();
				uAddedBone->setLocalTransform(bone.pImpl->m_position,bone.pImpl->m_localRotation);
				uAddedBone->setName(pImpl->m_stringArray.c_str(i));
			}
			else
			{
				uAddedBone = uParentPtr->addChildBone(pImpl->m_stringArray.c_str(i), bone.pImpl->m_position, bone.pImpl->m_localRotation);
			}

			if (uAddedBone)
			{
				uPtrs[uAddedBone->name()] = uAddedBone;

				setBoneTypeToUNIV(bone, *uAddedBone);

				std::string setter = bone.pImpl->m_mirrorBoneIndex == i && bone.pImpl->m_mirrorBoneIndex >= 0 ? "" : pImpl->m_stringArray.c_str(bone.pImpl->m_mirrorBoneIndex);
				if (!setter.empty())
				{
					mirrorPackage.addPair(pImpl->m_stringArray.c_str(i), setter.c_str());
				}

				const SFBGS_RigPackage::LODSetting lod = SFBGS_RigPackage::GetLODFromInt(bone.pImpl->m_LOD);

				sfbgsPackage.setBoneLod(uAddedBone->name(), lod);

				//resets the loop
				i = -1;
			}
		}

		auto pBoneMapArray = boneMapArray();
		for (int key = 0; key < SFBGSMAPSIZE; key++)
		{
			if (const int16_t keyValue = pBoneMapArray.at(key); keyValue >= 0 && keyValue < pImpl->m_stringArray.size())
			{
				const char* sfbgsMappedBoneName = pImpl->m_stringArray.c_str(keyValue);

				//If the bone has successfully made it to the UNIV Rig, add it to the map
				if(const UNIV::SkeletonBone* uMappedBonePtr = output.bone(sfbgsMappedBoneName); SCOMPARE(sfbgsMappedBoneName, "") != 0)
					sfbgsPackage.addBoneToMap(static_cast<SFBGS_RigPackage::BoneMapKey>(key), uMappedBonePtr->name(), true);
			}
		}

		if (isMarkedMannequin())
			sfbgsPackage.setIsMannequin(true);

		sfbgsPackage.setPrecisionValues(PrecisionSet(lowPrecision(), highPrecision()));

		return output;
	}
}
