//Copyright © 2025 aka Calaverah. All rights reserved.
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

		std::memcpy(&rootRotation.w, &buffer.at(addressIndex), sizeof(rootRotation.w));
		addressIndex += sizeof(rootRotation.w);

		std::memcpy(&rootRotation.x, &buffer.at(addressIndex), sizeof(rootRotation.x)*3);
		addressIndex += (sizeof(rootRotation.x)*3);

		std::memcpy(&position, &buffer.at(addressIndex), sizeof(position));
		addressIndex += sizeof(position);

		std::memcpy(&term01, &buffer.at(addressIndex), sizeof(term01));
		addressIndex += sizeof(term01);

		std::memcpy(&nameOffset, &buffer.at(addressIndex), sizeof(nameOffset));
		addressIndex += sizeof(nameOffset);

		std::memcpy(&parentBoneIndex, &buffer.at(addressIndex), sizeof(parentBoneIndex));
		addressIndex += sizeof(parentBoneIndex);

		std::memcpy(&term02, &buffer.at(addressIndex), sizeof(term02));
		addressIndex += sizeof(term02);

		std::memcpy(&term03, &buffer.at(addressIndex), sizeof(term03));
		addressIndex += sizeof(term03);

		std::memcpy(&term04, &buffer.at(addressIndex), sizeof(term04));
		addressIndex += sizeof(term04);

		std::memcpy(&mirrorBoneIndex, &buffer.at(addressIndex), sizeof(mirrorBoneIndex));
		addressIndex += sizeof(mirrorBoneIndex);

		std::memcpy(&term05, &buffer.at(addressIndex), sizeof(term05));
		addressIndex += sizeof(term05);

		std::memcpy(&term06, &buffer.at(addressIndex), sizeof(term06));
		addressIndex += sizeof(term06);
		
		std::memcpy(&term07, &buffer.at(addressIndex), sizeof(term07));
		addressIndex += sizeof(term07);

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

		std::memcpy(&buffer.at(addressIndex), &rootRotation.w, sizeof(rootRotation.w));
		addressIndex += sizeof(rootRotation.w);

		std::memcpy(&buffer.at(addressIndex), &rootRotation.x, sizeof(rootRotation.x)*3);
		addressIndex += (sizeof(rootRotation.x)*3);

		std::memcpy( &buffer.at(addressIndex), &position, sizeof(position));
		addressIndex += sizeof(position);

		std::memcpy( &buffer.at(addressIndex), &term01, sizeof(term01));
		addressIndex += sizeof(term01);

		std::memcpy( &buffer.at(addressIndex), &nameOffset, sizeof(nameOffset));
		addressIndex += sizeof(nameOffset);

		std::memcpy( &buffer.at(addressIndex), &parentBoneIndex, sizeof(parentBoneIndex));
		addressIndex += sizeof(parentBoneIndex);

		std::memcpy( &buffer.at(addressIndex), &term02, sizeof(term02));
		addressIndex += sizeof(term02);

		std::memcpy( &buffer.at(addressIndex), &term03, sizeof(term03));
		addressIndex += sizeof(term03);

		std::memcpy( &buffer.at(addressIndex), &term04, sizeof(term04));
		addressIndex += sizeof(term04);

		std::memcpy( &buffer.at(addressIndex), &mirrorBoneIndex, sizeof(mirrorBoneIndex));
		addressIndex += sizeof(mirrorBoneIndex);

		std::memcpy( &buffer.at(addressIndex), &term05, sizeof(term05));
		addressIndex += sizeof(term05);

		std::memcpy( &buffer.at(addressIndex), &term06, sizeof(term06));
		addressIndex += sizeof(term06);

		std::memcpy( &buffer.at(addressIndex), &term07, sizeof(term07));
		addressIndex += sizeof(term07);

		std::memcpy( &buffer.at(addressIndex), &unknownScalar, sizeof(unknownScalar));
		addressIndex += sizeof(unknownScalar);

		std::memcpy( &buffer.at(addressIndex), &term08, sizeof(term08));
		addressIndex += sizeof(term08);
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
	SkeletonRig ConvertToSFBGSRig(const CALUMI::UNIV::SkeletonRig& inputRig, float& highPrecision, float& lowPrecision)
	{
		SkeletonRig output;
		auto stringResult = createStringVectorFromRig(inputRig);


		output.fileSize += stringResult.second.at(stringResult.first.size()); //The size of the string array should be one less than the offset array. This will give us the total char array size including null terms
		output.suffixOffset += 80 + 96 * inputRig.boneEntries.size();
		output.fileSize += output.suffixOffset + sizeof(output.suffixArray);

		output.lowPrecision = lowPrecision;
		output.highPrecision = highPrecision;

		output.boneCount = inputRig.boneEntries.size();
		output.unknownCount = output.boneCount;

		output.boneEntries.reserve(output.boneCount);
		for (unsigned int i = 0; i < output.boneCount; i++)
		{
			SkeletonBone toAdd;
			toAdd.localRotation = inputRig.boneEntries.at(i).localRotation;
			toAdd.rootRotation = inputRig.boneEntries.at(i).rootRotation;
			toAdd.position = inputRig.boneEntries.at(i).localPosition;
			toAdd.nameOffset = stringResult.second.at(i);
			toAdd.parentBoneIndex = inputRig.boneEntries.at(i).parentBoneIndex;
			toAdd.mirrorBoneIndex = i;
			output.boneEntries.push_back(toAdd);
		}
		std::fill(std::begin(output.suffixArray), std::end(output.suffixArray), (int16_t)-1);

		output.stringArray = stringResult.first;

		return output;
	}

	CALUMI::UNIV::SkeletonRig ConvertToUniversalRig(const CALUMI::SFBGS::SkeletonRig& inputRig)
	{
		CALUMI::UNIV::SkeletonRig output;
		
		for (unsigned int i = 0; i < inputRig.boneEntries.size(); i++)
		{
			const SFBGS::SkeletonBone& bone = inputRig.boneEntries.at(i);
			output.AddBoneToRig(bone.localRotation,bone.position,inputRig.stringArray.at(i),bone.parentBoneIndex,true);
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

			std::memcpy(&headerEntry80, &buffer.value().at(addressIndex), sizeof(headerEntry80));
			addressIndex += sizeof(headerEntry80);

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

			std::memcpy(&unknownCount, &buffer.value().at(addressIndex), sizeof(unknownCount));
			addressIndex += sizeof(unknownCount);

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

			std::memcpy(&buffer.at(addressIndex), &headerEntry80, sizeof(headerEntry80));
			addressIndex += sizeof(headerEntry80);

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

			std::memcpy(&buffer.at(addressIndex), &unknownCount, sizeof(unknownCount));
			addressIndex += sizeof(unknownCount);

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

	std::expected<bool, FileError> SkeletonRig::ReadFromFile(char* inputFilePath)
	{
		std::filesystem::path newPath(inputFilePath);
		return ReadFromFile(newPath);
	}

	std::expected<std::string, FileError> SkeletonRig::WriteToFile(char* outputFilePath)
	{
		std::filesystem::path newPath(outputFilePath);
		return WriteToFile(newPath);
	}

}}