//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "SFBGS_SkeletonRig.h"

namespace CALUMI {namespace SFBGS {


	SkeletonBone::SkeletonBone(std::vector<char>& buffer, unsigned long long& addressIndex)
	{
		//QUAT UNITS ARE SERIALIZED AS WXYZ AND NEED TO BE READ INTO XYZW
		std::memcpy(&localRotation.w, &buffer.at(addressIndex), sizeof(localRotation.w));
		addressIndex += sizeof(localRotation.w);

		std::memcpy(&localRotation.x, &buffer.at(addressIndex), sizeof(localRotation.x) * 3);
		addressIndex += (sizeof(localRotation.x) * 3);

		std::memcpy(&globalRotation.w, &buffer.at(addressIndex), sizeof(globalRotation.w));
		addressIndex += sizeof(globalRotation.w);

		std::memcpy(&globalRotation.x, &buffer.at(addressIndex), sizeof(globalRotation.x)*3);
		addressIndex += (sizeof(globalRotation.x)*3);

		std::memcpy(&position, &buffer.at(addressIndex), sizeof(position));
		addressIndex += sizeof(position);

		std::memcpy(&boneType, &buffer.at(addressIndex), sizeof(boneType));
		addressIndex += sizeof(boneType);

		std::memcpy(&nameOffset, &buffer.at(addressIndex), sizeof(nameOffset));
		addressIndex += sizeof(nameOffset);

		std::memcpy(&parentBoneIndex, &buffer.at(addressIndex), sizeof(parentBoneIndex));
		addressIndex += sizeof(parentBoneIndex);

		std::memcpy(&twistDriverMqnIndex, &buffer.at(addressIndex), sizeof(twistDriverMqnIndex));
		addressIndex += sizeof(twistDriverMqnIndex);

		std::memcpy(&twistDriverIndex, &buffer.at(addressIndex), sizeof(twistDriverIndex));
		addressIndex += sizeof(twistDriverIndex);

		std::memcpy(&_pad01, &buffer.at(addressIndex), sizeof(_pad01));
		addressIndex += sizeof(_pad01);

		std::memcpy(&mirrorBoneIndex, &buffer.at(addressIndex), sizeof(mirrorBoneIndex));
		addressIndex += sizeof(mirrorBoneIndex);

		std::memcpy(&term05, &buffer.at(addressIndex), sizeof(term05));
		addressIndex += sizeof(term05);

		std::memcpy(&twistDriverWeight, &buffer.at(addressIndex), sizeof(twistDriverWeight));
		addressIndex += sizeof(twistDriverWeight);
		
		std::memcpy(&_pad02, &buffer.at(addressIndex), sizeof(_pad02));
		addressIndex += sizeof(_pad02);

		std::memcpy(&unknownScalar, &buffer.at(addressIndex), sizeof(unknownScalar));
		addressIndex += sizeof(unknownScalar);

		std::memcpy(&term08, &buffer.at(addressIndex), sizeof(term08));
		addressIndex += sizeof(term08);
	}

	void SkeletonBone::SerializeIntoBuffer(std::vector<char>& buffer, unsigned long long& addressIndex)
	{
		buffer.insert(buffer.end(), 96, 0);

		//QUAT UNITS ARE SERIALIZED AS WXYZ AND NEED TO BE WRITTEN FROM XYZW
		std::memcpy(&buffer.at(addressIndex), &localRotation.w, sizeof(localRotation.w));
		addressIndex += sizeof(localRotation.w);

		std::memcpy(&buffer.at(addressIndex), &localRotation.x, sizeof(localRotation.x) * 3);
		addressIndex += (sizeof(localRotation.x) * 3);

		std::memcpy(&buffer.at(addressIndex), &globalRotation.w, sizeof(globalRotation.w));
		addressIndex += sizeof(globalRotation.w);

		std::memcpy(&buffer.at(addressIndex), &globalRotation.x, sizeof(globalRotation.x)*3);
		addressIndex += (sizeof(globalRotation.x)*3);

		std::memcpy( &buffer.at(addressIndex), &position, sizeof(position));
		addressIndex += sizeof(position);

		std::memcpy( &buffer.at(addressIndex), &boneType, sizeof(boneType));
		addressIndex += sizeof(boneType);

		std::memcpy( &buffer.at(addressIndex), &nameOffset, sizeof(nameOffset));
		addressIndex += sizeof(nameOffset);

		std::memcpy( &buffer.at(addressIndex), &parentBoneIndex, sizeof(parentBoneIndex));
		addressIndex += sizeof(parentBoneIndex);

		std::memcpy( &buffer.at(addressIndex), &twistDriverMqnIndex, sizeof(twistDriverMqnIndex));
		addressIndex += sizeof(twistDriverMqnIndex);

		std::memcpy( &buffer.at(addressIndex), &twistDriverIndex, sizeof(twistDriverIndex));
		addressIndex += sizeof(twistDriverIndex);

		std::memcpy( &buffer.at(addressIndex), &_pad01, sizeof(_pad01));
		addressIndex += sizeof(_pad01);

		std::memcpy( &buffer.at(addressIndex), &mirrorBoneIndex, sizeof(mirrorBoneIndex));
		addressIndex += sizeof(mirrorBoneIndex);

		std::memcpy( &buffer.at(addressIndex), &term05, sizeof(term05));
		addressIndex += sizeof(term05);

		std::memcpy( &buffer.at(addressIndex), &twistDriverWeight, sizeof(twistDriverWeight));
		addressIndex += sizeof(twistDriverWeight);

		std::memcpy( &buffer.at(addressIndex), &_pad02, sizeof(_pad02));
		addressIndex += sizeof(_pad02);

		std::memcpy( &buffer.at(addressIndex), &unknownScalar, sizeof(unknownScalar));
		addressIndex += sizeof(unknownScalar);

		std::memcpy( &buffer.at(addressIndex), &term08, sizeof(term08));
		addressIndex += sizeof(term08);
	}

	bool SFBGS::SkeletonBone::SetBoneTypeFromUNIV(UNIV::SkeletonBone& univBone)
	{
		switch (univBone.GetBoneTypeProperty()->GetType())
		{
			case CALUMI::UNIV::BoneType::Default:
				boneType = SFBGS::BoneType::Default;
				//UNIV::DefaultBoneProperties* dProp = dynamic_cast<UNIV::DefaultBoneProperties*>(const_cast<UNIV::BoneTypeProperties*>(univBone.GetBoneTypeProperty()));
				twistDriverIndex = -1;
				twistDriverMqnIndex = -1;
				twistDriverWeight = 0.0;
				break;
			case CALUMI::UNIV::BoneType::Twist:
				{
				boneType = SFBGS::BoneType::Twist;
				//We are permitting const_casting as we are paying special attention not to delete the struct while modifying
				UNIV::TwistBoneProperties* tProp = dynamic_cast<UNIV::TwistBoneProperties*>(const_cast<UNIV::BoneTypeProperties*>(univBone.GetBoneTypeProperty()));
				twistDriverIndex = tProp->twistDriverIndex;
				twistDriverWeight = tProp->twistDriverWeight;
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
			tProp->twistDriverIndex = twistDriverIndex;
			tProp->twistDriverWeight = twistDriverWeight;
			}
			break;
		default:
			break;
		}
		return true;
	}

	UNIV::BoneType SFBGS::SkeletonBone::GetBoneTypeAsUNIVEnum()
	{
		switch (boneType)
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
		switch (boneType)
		{
		case CALUMI::SFBGS::BoneType::Default:
			return "Default";
		case CALUMI::SFBGS::BoneType::Twist:
			return "Twist";
		default:
			return "SFBGS UNDEFINED";
		}
	}

	std::pair<std::vector<std::string>, std::vector<unsigned int>>createStringVectorFromRig(const CALUMI::UNIV::SkeletonRig& inputRig)
	{
		size_t size = 0;
		std::vector<unsigned int> nameOffsets;
		std::vector<std::string> nameEntries;
		nameOffsets.reserve(inputRig.boneEntries.size() + 1);
		nameEntries.reserve(inputRig.boneEntries.size());
		for (const auto& entry : inputRig.boneEntries)
		{
			nameEntries.push_back(entry.name);
			nameOffsets.push_back(size);
			size += entry.name.length() + 1;
		}
		nameOffsets.push_back(size); //final value can assist with determining SFBGS File Size Entry


		return std::make_pair(nameEntries, nameOffsets);
	}

	std::vector<unsigned int> _getSFBGSRigStringOffsets(const std::vector<std::string>& stringEntries)
	{
		std::vector<unsigned int> output;
		//offset should be 80 + 96+boneCount + 314
		unsigned int offset = 80 + 314 + 96 * stringEntries.size();

		output.reserve(stringEntries.size());
		for (unsigned int i = 0; i < stringEntries.size(); i++)
		{
			output.push_back(offset);
			offset += (stringEntries.at(i).length() + 1);
		}
		return output;
	}

	//Simple Rig Implementation For Now
	SkeletonRig ConvertToSFBGSRig(CALUMI::UNIV::SkeletonRig& inputRig, float& highPrecision, float& lowPrecision)
	{
		SkeletonRig output;
		auto stringResult = createStringVectorFromRig(inputRig);


		output.fileSize += stringResult.second.at(stringResult.first.size()); //The size of the string array should be one less than the offset array. This will give us the total char array size including null terms
		output.suffixOffset += 80 + 96 * inputRig.boneEntries.size();
		output.fileSize += output.suffixOffset + sizeof(output.suffixArray);

		output.lowPrecision = lowPrecision;
		output.highPrecision = highPrecision;

		output.boneCount = inputRig.boneEntries.size();
		output.boneCount_Animated = output.boneCount;

		output.boneEntries.reserve(output.boneCount);
		for (unsigned int i = 0; i < output.boneCount; i++)
		{
			SkeletonBone toAdd;
			toAdd.localRotation = inputRig.boneEntries.at(i).localRotation;
			toAdd.globalRotation = inputRig.boneEntries.at(i).globalRotation;
			toAdd.position = inputRig.boneEntries.at(i).localPosition;
			toAdd.nameOffset = stringResult.second.at(i);
			toAdd.parentBoneIndex = inputRig.boneEntries.at(i).parentBoneIndex;
			toAdd.mirrorBoneIndex = inputRig.boneEntries.at(i).mirrorBoneIndex;
			if (!toAdd.SetBoneTypeFromUNIV(inputRig.boneEntries.at(i)))
			{
				std::println("[CALUMI.Animation API] UNIV Rig: {} Bone: {} ({}) Bone Type: {} Is Not An Acceptable Type For SFBGS Skeleton Rigs! This Bone Will Remain As The Default Type", inputRig.rigName, inputRig.boneEntries.at(i).name, i, inputRig.boneEntries.at(i).GetBoneTypeProperty()->GetTypeString());
			}
			output.boneEntries.push_back(toAdd);
		}
		std::fill(std::begin(output.suffixArray), std::end(output.suffixArray), (int16_t)-1);

		output.stringArray = stringResult.first;

		return output;
	}

	CALUMI::UNIV::SkeletonRig ConvertToUniversalRig(CALUMI::SFBGS::SkeletonRig& inputRig)
	{
		CALUMI::UNIV::SkeletonRig output;
		
		for (unsigned int i = 0; i < inputRig.boneEntries.size(); i++)
		{
			SFBGS::SkeletonBone& bone = inputRig.boneEntries.at(i);
			output.AddBoneToRig(bone.localRotation,bone.position,inputRig.stringArray.at(i),bone.parentBoneIndex,true);
			bone.SetBoneTypeToUNIV(output.boneEntries.at(i));
			output.boneEntries.at(i).mirrorBoneIndex = bone.mirrorBoneIndex;
		}

		return output;
	}


	std::expected<bool, FileError> SkeletonRig::ReadFromFile(std::filesystem::path& inputFilePath)
	{
		auto buffer = CALUMI::ValidateFile(inputFilePath, { ".rig" }, 80, 0, true);
		if (!buffer.has_value()) return std::unexpected((buffer.error()));

		//iterator tracking
		unsigned long long addressIndex = 0;

		//HEADER READING
		{
			std::memcpy(&versionNumber, &buffer.value().at(addressIndex), sizeof(versionNumber));
			addressIndex += sizeof(versionNumber);

			std::memcpy(&fileSize, &buffer.value().at(addressIndex), sizeof(fileSize));
			addressIndex += sizeof(fileSize);

			std::memcpy(&headerSize, &buffer.value().at(addressIndex), sizeof(headerSize));
			addressIndex += sizeof(headerSize);

			std::memcpy(&headerEmpty01, &buffer.value().at(addressIndex), sizeof(headerEmpty01));
			addressIndex += sizeof(headerEmpty01);

			std::memcpy(&suffixOffset, &buffer.value().at(addressIndex), sizeof(suffixOffset));
			addressIndex += sizeof(suffixOffset);

			std::memcpy(&headerEmpty02, &buffer.value().at(addressIndex), sizeof(headerEmpty02));
			addressIndex += sizeof(headerEmpty02);

			std::memcpy(&matchingThree, &buffer.value().at(addressIndex), sizeof(matchingThree));
			addressIndex += sizeof(matchingThree);

			std::memcpy(&lowPrecision, &buffer.value().at(addressIndex), sizeof(lowPrecision));
			addressIndex += sizeof(lowPrecision);

			std::memcpy(&highPrecision, &buffer.value().at(addressIndex), sizeof(highPrecision));
			addressIndex += sizeof(highPrecision);

			std::memcpy(&boneCount, &buffer.value().at(addressIndex), sizeof(boneCount));
			addressIndex += sizeof(boneCount);

			std::memcpy(&boneCount_Animated, &buffer.value().at(addressIndex), sizeof(boneCount_Animated));
			addressIndex += sizeof(boneCount_Animated);

			std::memcpy(&headerEmpty03, &buffer.value().at(addressIndex), sizeof(headerEmpty03));
			addressIndex += sizeof(headerEmpty03);

			std::memcpy(&endOfHeader, &buffer.value().at(addressIndex), sizeof(endOfHeader));
			addressIndex += sizeof(endOfHeader);
		}

		//WARN IF FILE SIZE VAR != BUFFER
		if (fileSize != buffer.value().size()) std::println("WARNING: THE VARIABLE, FILE SIZE = {} DOES NOT MATCH THE BUFFER SIZE OF {}", fileSize, buffer.value().size());

		//READ BONE ENTRIES IN ORDER
		{
			boneEntries.reserve(boneCount);
			for (unsigned short i = 0; i < boneCount; i++)
			{
				boneEntries.push_back(SkeletonBone(buffer.value(), addressIndex));
			}
		}


		//READ SUFFIX
		std::memcpy(&suffixArray, &buffer.value().at(addressIndex), sizeof(suffixArray));
		addressIndex += sizeof(suffixArray);

		//READ STRINGS
		stringArray.reserve(boneCount);
		for (uint16_t i = 0; i < boneCount; i++)
		{
			stringArray.push_back(std::string(&buffer.value().at(boneEntries.at(i).nameOffset)));
			addressIndex += (stringArray.at(i).length() + 1); //accounting for null terminator
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


	std::expected<std::string, FileError> SkeletonRig::WriteToFile(std::filesystem::path& outputFilePath)
	{
		//LARGEST FILE: "D:/ModOrganizer/Starfield_Mod_Authoring_01/mods/ExtractedData/meshes/furniture/armillary/characterassets/skeleton.rig" at 18301 bytes

		std::vector<char> buffer;
		buffer.reserve(18500);

		unsigned long long addressIndex = 0;
		std::vector<unsigned int> offsets = _getSFBGSRigStringOffsets(stringArray);

		//HEADER
		{
			buffer.insert(buffer.end(), 80, 0);
			std::memcpy(&buffer.at(addressIndex), &versionNumber, sizeof(versionNumber));
			addressIndex += sizeof(versionNumber);

			std::memcpy(&buffer.at(addressIndex), &fileSize, sizeof(fileSize));
			addressIndex += sizeof(fileSize);

			std::memcpy(&buffer.at(addressIndex), &headerSize, sizeof(headerSize));
			addressIndex += sizeof(headerSize);

			std::memcpy(&buffer.at(addressIndex), &headerEmpty01, sizeof(headerEmpty01));
			addressIndex += sizeof(headerEmpty01);

			std::memcpy(&buffer.at(addressIndex), &suffixOffset, sizeof(suffixOffset));
			addressIndex += sizeof(suffixOffset);

			std::memcpy(&buffer.at(addressIndex), &headerEmpty02, sizeof(headerEmpty02));
			addressIndex += sizeof(headerEmpty02);

			std::memcpy(&buffer.at(addressIndex), &matchingThree, sizeof(matchingThree));
			addressIndex += sizeof(matchingThree);

			std::memcpy(&buffer.at(addressIndex), &lowPrecision, sizeof(lowPrecision));
			addressIndex += sizeof(lowPrecision);

			std::memcpy(&buffer.at(addressIndex), &highPrecision, sizeof(highPrecision));
			addressIndex += sizeof(highPrecision);

			std::memcpy(&buffer.at(addressIndex), &boneCount, sizeof(boneCount));
			addressIndex += sizeof(boneCount);

			std::memcpy(&buffer.at(addressIndex), &boneCount_Animated, sizeof(boneCount_Animated));
			addressIndex += sizeof(boneCount_Animated);

			std::memcpy(&buffer.at(addressIndex), &headerEmpty03, sizeof(headerEmpty03));
			addressIndex += sizeof(headerEmpty03);

			std::memcpy(&buffer.at(addressIndex), &endOfHeader, sizeof(endOfHeader));
			addressIndex += sizeof(endOfHeader);
		}

		//BONE ENTRIES
		{
			for (unsigned int i = 0; i < boneEntries.size(); i++)
			{
				boneEntries.at(i).nameOffset = offsets.at(i);
				boneEntries.at(i).SerializeIntoBuffer(buffer, addressIndex);
			}
		}

		//SUFFIX (PI SIZED SECTION)
		{
			buffer.insert(buffer.end(), sizeof(suffixArray), -1);

			//Setting header value to confirm offset. In case the SFBGS Rig Values were changed incorrectly by the user
			suffixOffset = addressIndex;
			std::memcpy(&buffer.at(16), &suffixOffset, sizeof(suffixOffset));

			std::memcpy(&buffer.at(addressIndex), &suffixArray, sizeof(suffixArray));
			addressIndex += sizeof(suffixArray);
		}

		//STRING ARRAY
		{
			for (uint16_t i = 0; i < stringArray.size(); i++)
			{
				buffer.insert(buffer.end(), stringArray.at(i).begin(), stringArray.at(i).end());
				buffer.push_back('\0');
				addressIndex += stringArray.at(i).length();
				addressIndex++; //accounting for null term
			}
		}

		if (fileSize != addressIndex)
		{
			std::println("===========================================================");
			std::println("===========================================================");
			std::println("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==");
			//std::println("CURRENT  POS: {}",(void*)currentAddress);
			//std::println("CURRENT ITER: {}",addressIndex);
			std::println("BUFFER  SIZE: {}", buffer.size());
			std::println("FILE  SIZE: {}", fileSize);
			std::println("===========================================================");
			std::cout << "Press ENTER to continue running the program." << std::endl;
			std::cin.get();
			std::println("===========================================================");
		}

		fileSize = buffer.size();
		std::memcpy(&buffer.at(4), &fileSize, sizeof(fileSize));

		return CALUMI::WriteToBinaryFile(outputFilePath,buffer);
	}

	uint8_t SkeletonRig::_CheckAssumedHeaderEntries()
	{
		uint8_t output = 0;

		if (headerEmpty01 != 0)
			output += 0b1;
		if (headerEmpty02 != 0)
			output += 0b10;
		if (headerEmpty03 != 0)
			output += 0b100;

		if (matchingThree[0] != matchingThree[1] || matchingThree[0] != matchingThree[2])
			output += 0b1000;

		if (headerSize != 0x50)
			output += 0b10000;

		if (versionNumber != 5)
			output += 0b100000;

		int aCount = 0;
		for (const SFBGS::SkeletonBone &bone : boneEntries)
		{
			if (bone.boneType == SFBGS::BoneType::Default)
			{
				aCount++;
			}
		}
		if (aCount != boneCount_Animated)
			output += 0b1000000;

		return output;
	}

}}