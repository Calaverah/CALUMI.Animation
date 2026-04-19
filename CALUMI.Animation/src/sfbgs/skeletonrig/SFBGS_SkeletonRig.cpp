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
		Math::Quaternion _localRotation;
		Math::Quaternion _globalRotation;
		Math::Vector3 _position;

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
			pImpl->_localRotation.setW(w);
		}

		{
			float f[3] = {};
			std::memcpy(&f, &buffer.at(addressIndex), sizeof(f));
			addressIndex += sizeof(f);
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
			addressIndex += sizeof(f);
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
		*this = input;
	}
	SkeletonBone& SkeletonBone::operator=(const SkeletonBone& input)
	{
		if (this != &input)
			*pImpl = *input.pImpl;

		return *this;
	}

	Math::Quaternion& SkeletonBone::localRotation() const { return pImpl->_localRotation; }
	Math::Quaternion& SkeletonBone::globalRotation() const { return pImpl->_globalRotation; }
	Math::Vector3& SkeletonBone::position() const { return pImpl->_position; }
	SkeletonBone::BoneType SkeletonBone::getBoneType() const { return pImpl->_boneType; }
	void SkeletonBone::setBoneType(const BoneType t) const { pImpl->_boneType = t; }
	uint64_t SkeletonBone::getNameOffset() const { return pImpl->_nameOffset; }
	void SkeletonBone::setNameOffset(const uint64_t offset) const { pImpl->_nameOffset = offset; }
	int32_t SkeletonBone::getParentBoneIndex() const { return pImpl->_parentBoneIndex; }
	void SkeletonBone::setParentBoneIndex(const int32_t idx) const { pImpl->_parentBoneIndex = idx; }
	int32_t SkeletonBone::getTwistDriverMqnIndex() const { return pImpl->_twistDriverMqnIndex; }
	void SkeletonBone::setTwistDriverMqnIndex(const int32_t idx) const { pImpl->_twistDriverMqnIndex = idx; }
	int32_t SkeletonBone::getTwistDriverIndex() const { return pImpl->_twistDriverIndex; }
	void SkeletonBone::setTwistDriverIndex(const int32_t idx) const { pImpl->_twistDriverIndex = idx; }
	int32_t SkeletonBone::getMirrorBoneIndex() const { return pImpl->_mirrorBoneIndex; }
	void SkeletonBone::setMirrorBoneIndex(const int32_t idx) const { pImpl->_mirrorBoneIndex = idx; }
	int32_t SkeletonBone::getTerm05() const { return pImpl->_term05; }
	void SkeletonBone::setTerm05(const int32_t value) const { pImpl->_term05 = value; }
	float SkeletonBone::getTwistDriverWeight() const { return pImpl->_twistDriverWeight; }
	void SkeletonBone::setTwistDriverWeight(const float weight) const { pImpl->_twistDriverWeight = weight; }
	float SkeletonBone::getUnknownScalar() const { return pImpl->_unknownScalar; }
	void SkeletonBone::setUnknownScalar(const float value) const { pImpl->_unknownScalar = value; }
	int32_t SkeletonBone::getTerm08() const { return pImpl->_pad03; }
	void SkeletonBone::setTerm08(const int32_t value) const { pImpl->_pad03 = value; }

	void SkeletonBone::serializeIntoBuffer(Utilities::BufferObject& buffer, unsigned long long& addressIndex) const
	{
		buffer.insert(buffer.endPos(), 96, 0);

		//QUAT UNITS ARE SERIALIZED AS W-XYZ AND NEED TO BE REFORMATTED FROM XYZ-W
		{
			const float w = pImpl->_localRotation.w();
			const float f[3] = { pImpl->_localRotation.x(),pImpl->_localRotation.y(), pImpl->_localRotation.z() };

			std::memcpy(&buffer.at(addressIndex), &w, sizeof(w));
			addressIndex += sizeof(w);

			std::memcpy(&buffer.at(addressIndex), &f, sizeof(f));
			addressIndex += sizeof(f);
		}

		{
			const float w = pImpl->_globalRotation.w();
			const float f[3] = { pImpl->_globalRotation.x(),pImpl->_globalRotation.y(), pImpl->_globalRotation.z() };

			std::memcpy(&buffer.at(addressIndex), &w, sizeof(w));
			addressIndex += sizeof(w);

			std::memcpy(&buffer.at(addressIndex), &f, sizeof(f));
			addressIndex += sizeof(f);
		}

		{
			const float f[3] = { pImpl->_position.x(), pImpl->_position.y(), pImpl->_position.z()};

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

	UNIV::BoneType SkeletonBone::getBoneTypeAsUNIVEnum() const
	{
		switch (pImpl->_boneType)
		{
		case BoneType::Default:
			return UNIV::BoneType::Default;
		case BoneType::Twist:
			return UNIV::BoneType::Twist;
		default:
			return UNIV::BoneType::UNDEFINED;
		}
	}

	const char* SkeletonBone::getBoneTypeAsString() const
	{
		switch (pImpl->_boneType)
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
			/// No matter what, these three ALWAYS match. Could be internal number tracking for BGS and may not matter to anyone outside the company
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

		pImpl->_stringArray = manifestPkg.processPackage(input);

		bool isMarkedMannequin = false;

		isMarkedMannequin = sfbgsPackage.isMannequin();
		setLowPrecision(sfbgsPackage.precisionSet().low());
		setHighPrecision(sfbgsPackage.precisionSet().high());

		setFileSize(fileSize() + static_cast<unsigned int>(pImpl->_stringArray.getFinalOffset()));
		setBoneMapOffset(boneMapOffset() + (80 + 96 * input.boneCount()));
		setFileSize(fileSize() + boneMapOffset() + SFBGSMAPSIZE * 2);

		setBoneCount(static_cast<uint16_t>(input.boneCount()));

		uint16_t animatedBoneCount = 0;
		boneEntries().reserve(boneCount());

		for (int i = 0; i < boneCount() && i < pImpl->_stringArray.size(); i++)
		{
			const auto refBone = input.bone(pImpl->_stringArray.c_str(i));

			SkeletonBone toAdd;
			if (refBone)
			{
				const auto& localTransform = refBone->localTransform();
				const auto globalTransform = refBone->globalTransform();
				toAdd.pImpl->_localRotation = localTransform.rotation();
				toAdd.pImpl->_globalRotation = globalTransform.rotation();
				toAdd.pImpl->_position = localTransform.position();
				toAdd.pImpl->_nameOffset = pImpl->_stringArray.getOffset(i) + boneMapOffset() + SFBGSMAPSIZE * 2;

				if (i == 0)
				{
					//The first entry from our UNIV rig will be treated as the root bone
					//which in SFBGS has no parent and is set to -1
					toAdd.pImpl->_parentBoneIndex = -1;
				}
				else
				{
					const auto parentBone = dynamic_cast<const UNIV::SkeletonBone*>(refBone->parent());

					if (parentBone)
					{
						toAdd.pImpl->_parentBoneIndex = static_cast<int32_t>(pImpl->_stringArray.find(parentBone->name(),0));
					}
					else
					{
						toAdd.pImpl->_parentBoneIndex = 0;
					}
				}

				toAdd.pImpl->_mirrorBoneIndex = static_cast<int32_t>(pImpl->_stringArray.find(mirrorPkg.getPairedBone(refBone->name()), i));

				// ReSharper disable once CppExpressionWithoutSideEffects
				setBoneTypeFromUNIV(*refBone, toAdd, &pImpl->_stringArray);

				if (isMarkedMannequin && toAdd.pImpl->_boneType == SkeletonBone::BoneType::Twist)
				{
					toAdd.pImpl->_twistDriverMqnIndex = toAdd.pImpl->_parentBoneIndex;
				}
				else
				{
					animatedBoneCount++;
				}
			}
			boneEntries().push_back(toAdd);
		}

		setBoneCountAnimated(animatedBoneCount);

		Utilities::S16Vector vecPackage = sfbgsPackage.ConvertMap(pImpl->_stringArray);
		setBoneMapArray(vecPackage);
	}

	void SkeletonRig::setBoneMapArray(Utilities::S16Vector& input) const
	{
		for (uint8_t j = 0; j < input.size() && j < std::numeric_limits<uint8_t>::max(); j++)
		{
			pImpl->_boneMapArray[j] = input.at(j);
		}
	}

	Utilities::StringList& SkeletonRig::stringArray() const
	{
		return pImpl->_stringArray;
	}

	int SkeletonRig::versionNumber() const
	{
		return pImpl->_versionNumber;
	}

	void SkeletonRig::setVersionNumber(const int v) const
	{
		pImpl->_versionNumber = v;
	}

	uint32_t SkeletonRig::fileSize() const
	{
		return pImpl->_fileSize;
	}

	void SkeletonRig::setFileSize(const uint32_t size) const
	{
		pImpl->_fileSize = size;
	}

	uint32_t SkeletonRig::headerSize() const
	{
		return pImpl->_headerSize;
	}

	void SkeletonRig::setHeaderSize(const uint32_t size) const
	{
		pImpl->_headerSize = size;
	}

	uint32_t SkeletonRig::boneMapOffset() const
	{
		return pImpl->_boneMapOffset;
	}

	void SkeletonRig::setBoneMapOffset(const uint32_t offset) const
	{
		pImpl->_boneMapOffset = offset;
	}

	Utilities::U64Vector SkeletonRig::matchingThree() const
	{
		Utilities::U64Vector output;
		output.resize(3);
		output.at(0) = pImpl->_matchingThree[0];
		output.at(1) = pImpl->_matchingThree[1];
		output.at(2) = pImpl->_matchingThree[2];
		return output;
	}

	void SkeletonRig::setMatchingThree(const uint64_t m1, const uint64_t m2, const uint64_t m3) const
	{
		pImpl->_matchingThree[0] = m1;
		pImpl->_matchingThree[1] = m2;
		pImpl->_matchingThree[2] = m3;
	}

	float SkeletonRig::lowPrecision() const
	{
		return pImpl->_lowPrecision;
	}

	void SkeletonRig::setLowPrecision(const float value) const
	{
		pImpl->_lowPrecision = value;
	}

	float SkeletonRig::highPrecision() const
	{
		return pImpl->_highPrecision;
	}

	void SkeletonRig::setHighPrecision(const float value) const
	{
		pImpl->_highPrecision = value;
	}

	uint16_t SkeletonRig::boneCount() const
	{
		return pImpl->_boneCount;
	}

	void SkeletonRig::setBoneCount(const uint16_t count) const
	{
		pImpl->_boneCount = count;
	}

	uint16_t SkeletonRig::boneCountAnimated() const
	{
		return pImpl->_boneCountAnimated;
	}

	void SkeletonRig::setBoneCountAnimated(const uint16_t count) const
	{
		pImpl->_boneCountAnimated = count;
	}

	SkeletonBoneVector& SkeletonRig::boneEntries() const
	{
		return pImpl->_boneEntries;
	}

	Utilities::S16Vector SkeletonRig::boneMapArray() const
	{
		Utilities::S16Vector output;
		output.reserve(SFBGSMAPSIZE);
		for (int16_t i : pImpl->_boneMapArray)
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
			output.at(i) = static_cast<char>(pImpl->_endOfHeader[i]);
		}
		return output;
	}
#endif

	Utilities::FileResult SkeletonRig::readFromFile(Utilities::PathContainer&& inputFilePath)
	{
		Utilities::PathContainer output(inputFilePath);
		return readFromFile(output);
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
			std::cout << std::format("WARNING: THE VARIABLE, FILE SIZE = {} DOES NOT MATCH THE BUFFER SIZE OF {}", pImpl->_fileSize, buffer.size()).c_str() << std::endl;
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
			addressIndex += pImpl->_stringArray.stringLength(i, true);
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
		const std::vector<unsigned int> offsets = s_getSFBGSRigStringOffsets(pImpl->_stringArray);

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
				pImpl->_boneEntries.at(i).serializeIntoBuffer(buffer, addressIndex);
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
			for (uint16_t i = 0; i < pImpl->_stringArray.size() && i < std::numeric_limits<uint16_t>::max(); i++)
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
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << std::format("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==").c_str() << std::endl;
			std::cout << std::format("BUFFER  SIZE: {}", buffer.size()).c_str() << std::endl;
			std::cout << std::format("FILE  SIZE: {}", pImpl->_fileSize).c_str() << std::endl;
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << "Press ENTER to continue running the program." << std::endl;
			std::cin.get();
			std::cout << std::format("===========================================================").c_str() << std::endl;
		}
#endif

		pImpl->_fileSize = static_cast<unsigned int>(buffer.size());
		std::memcpy(&buffer.at(4), &pImpl->_fileSize, sizeof(pImpl->_fileSize));

		return WriteToBinaryFile(outputFilePath, buffer);
	}


#ifdef DEBUG_BUILD
	uint8_t SkeletonRig::checkAssumedHeaderEntries() const
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
			if (pImpl->_boneEntries.at(i).getBoneType() == SkeletonBone::BoneType::Default)
			{
				aCount++;
			}
		}
		if (aCount != pImpl->_boneCountAnimated)
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
		switch (univBone.boneTypeProperty()->getType())
		{
		case UNIV::BoneType::Default:
			sfbgsBone.pImpl->_boneType = SkeletonBone::BoneType::Default;
			sfbgsBone.pImpl->_twistDriverIndex = -1;
			sfbgsBone.pImpl->_twistDriverMqnIndex = -1;
			sfbgsBone.pImpl->_twistDriverWeight = 0.0;
			break;
		case UNIV::BoneType::Twist:
			{
				sfbgsBone.pImpl->_boneType = SkeletonBone::BoneType::Twist;

				const auto tProp = dynamic_cast<UNIV::TwistBoneProperties*>(const_cast<UNIV::BoneTypeProperties*>(univBone.boneTypeProperty()));

				int driverIndex = -1;

				if (boneList)
					driverIndex = static_cast<int>(boneList->find(tProp->twistDriver(), -1));
				else
					driverIndex = findBoneIndex(tProp->twistDriver());

				sfbgsBone.pImpl->_twistDriverIndex = driverIndex;
				sfbgsBone.pImpl->_twistDriverWeight = tProp->twistDriverWeight();
			}
			break;
		default:
			return false;
		}
		return true;
	}

	bool SkeletonRig::setBoneTypeToUNIV(const SkeletonBone& sfbgsBone, const UNIV::SkeletonBone& univBone) const
	{
		if (!univBone.setBoneTypeProperty(sfbgsBone.getBoneTypeAsUNIVEnum()))
			return false;

		switch (univBone.boneTypeProperty()->getType())
		{
		case UNIV::BoneType::Default:
			break;
		case UNIV::BoneType::Twist:
			{
				const auto tProp = dynamic_cast<UNIV::TwistBoneProperties*>(const_cast<UNIV::BoneTypeProperties*>(univBone.boneTypeProperty()));

				if (const int idx = sfbgsBone.pImpl->_twistDriverIndex; idx >= 0)
					tProp->setTwistDriver(pImpl->_stringArray.c_str(idx));

				tProp->setTwistDriverWeight(sfbgsBone.pImpl->_twistDriverWeight);
			}
			break;
		default:
			break;
		}
		return true;
	}

	int SkeletonRig::findBoneIndex(const char* boneName) const
	{
		for (uint64_t i = 0; i < pImpl->_stringArray.size() && i < std::numeric_limits<int>::max(); i++)
		{
			if (SCOMPARE(pImpl->_stringArray.c_str(i), boneName) == 0)
				return static_cast<int>(i);
		}

		return -1;
	}

	UNIV::SkeletonRig SkeletonRig::convertToUniversalRig() const
	{
		UNIV::SkeletonRig output;
		const auto& mirrorPackage = UNIV::RigMirrorPackage::GetPackage(output);
		const auto& manifestPackage = UNIV::RigManifestPackage::GetPackage(output);
		const auto& sfbgsPackage = SFBGS_RigPackage::GetPackage(output);

		std::unordered_map<std::string, const UNIV::SkeletonBone*> uPtrs;

		//get and assign root
		if (!pImpl->_boneEntries.empty() && !pImpl->_stringArray.empty())
		{
			const auto& uRootRef = output.root();
			const auto& sfbgsRootRef = pImpl->_boneEntries.at(0);
			if (!uRootRef.setName(pImpl->_stringArray.c_str(0)))
				uRootRef.setName("unknownRootName");

			uRootRef.setLocalTransform(sfbgsRootRef.pImpl->_position,sfbgsRootRef.pImpl->_localRotation);
			setBoneTypeToUNIV(sfbgsRootRef, uRootRef);
			uPtrs[uRootRef.name()] = &uRootRef;
			manifestPackage.addBone(uRootRef.name());
		}

		for (unsigned int i = 1; i < pImpl->_boneEntries.size(); i++)
		{
			const SkeletonBone& bone = pImpl->_boneEntries.at(i);
			const int pIdx = bone.getParentBoneIndex();
			const UNIV::SkeletonBone* uParentPtr = nullptr;

			if (uPtrs.contains(pImpl->_stringArray.c_str(pIdx)))
				uParentPtr = uPtrs[pImpl->_stringArray.c_str(pIdx)];

			if (!uParentPtr)
				uParentPtr = &output.root();

			if (const auto uAddedBone = uParentPtr->addChildBone(pImpl->_stringArray.c_str(i), bone.pImpl->_position, bone .pImpl->_localRotation))
			{
				uPtrs[uAddedBone->name()] = uAddedBone;
				manifestPackage.addBone(uAddedBone->name());

				setBoneTypeToUNIV(bone, *uAddedBone);

				std::string setter = bone.pImpl->_mirrorBoneIndex == i && bone.pImpl->_mirrorBoneIndex >= 0 ? "" : pImpl->_stringArray.c_str(bone.pImpl->_mirrorBoneIndex);
				if (!setter.empty())
				{
					mirrorPackage.addPair(pImpl->_stringArray.c_str(i), setter.c_str());
				}
			}
		}

		auto pBoneMapArray = boneMapArray();
		for (int key = 0; key < SFBGSMAPSIZE; key++)
		{
			if (const int16_t keyValue = pBoneMapArray.at(key); keyValue >= 0 && keyValue < pImpl->_stringArray.size())
			{
				const char* sfbgsMappedBoneName = pImpl->_stringArray.c_str(keyValue);

				//If the bone has successfully made it to the UNIV Rig, add it to the map
				if(const UNIV::SkeletonBone* uMappedBonePtr = output.bone(sfbgsMappedBoneName); SCOMPARE(sfbgsMappedBoneName, "") != 0)
					sfbgsPackage.addBoneToMap(static_cast<SFBGS_RigPackage::BoneMapKey>(key), uMappedBonePtr->name(), true);
			}
		}
		if (IsMarkedMannequin())
			sfbgsPackage.setIsMannequin(true);

		sfbgsPackage.setPrecisionValues(PrecisionSet(lowPrecision(), highPrecision()));

		return output;
	}
}
