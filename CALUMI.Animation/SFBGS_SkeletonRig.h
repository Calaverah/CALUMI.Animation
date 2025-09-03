//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "FileError.h"
#include "CALUMI_ReadWritable.h"
#include "CALUMI_SkeletonRig.h"
#include "FileValidation.h"
#include <cstdint>
#include <expected>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>



namespace CALUMI {namespace SFBGS {


	struct CALUMIANIMATION_API SkeletonBone
	{
		CALUMI::Math::Quaternion localRotation;
		CALUMI::Math::Quaternion rootRotation;
		CALUMI::Math::Vector3 position;
		int32_t term01 = -1; //Only +1 on twist bones?? Correlates to the unknown bone count on SFBGS::SkeletonRig
		uint64_t nameOffset = 0;
		int32_t parentBoneIndex = -1; //-1 for the root bone
		int32_t term02 = -1;
		int32_t term03 = -1; //Possibly a second parent or influencer?
		int32_t term04 = -1;
		int32_t mirrorBoneIndex = 0; //Same as this bone's index by default
		int32_t term05 = 0;
		int32_t term06 = 0;
		int32_t term07 = 0;
		float unknownScalar = 0.0;
		int32_t term08 = 0;

		SkeletonBone() = default;
		SkeletonBone(std::vector<char>& buffer, unsigned long long& addressIndex);

		/// <summary>
		/// Fills end of buffer with 96 bytes and copies the information into the vector.
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="addressIndex"></param>
		void SerializeIntoBuffer(std::vector<char>& buffer, unsigned long long& addressIndex);
	};



	struct CALUMIANIMATION_API SkeletonRig : CALUMI::ReadWritable
	{

		//HEADER
		int versionNumber = 05;
		unsigned int fileSize = 0; //NOTE: 4th Char in Buffer
		int headerEntry80 = 0x50;  //Currently the only value seen is 0x50 (80)
		unsigned int headerEmpty01 = 0; //Always empty, possibly padding
		unsigned int suffixOffset = 0; //96* bone count + 80 bytes //NOTE: 16th Char in Buffer
		unsigned int headerEmpty02 = 0; //Always empty, possibly padding
		uint64_t matchingThree[3] = { 0,0,0 };  //No matter what, these three ALWAYS match. Could be internal number tracking for BGS and may not matter to anyone outside of the company
		float lowPrecision = 0.03125;  //default precision values. For ships use 0.25. For first person use 0.0078125 (1/128)
		float highPrecision = 0.00025; //default precision values. For ships use 0.002. For first person use 6.25e-5 (1/16000)
		uint16_t boneCount = 0;
		uint16_t unknownCount = 0;
		unsigned int headerEmpty03 = 0; //Always empty
		double endOfHeader[2] = { 0.0,0.0 };


		std::vector<SkeletonBone> boneEntries;

		//Don't ask why I'm initializing like this... let's just move on.
		int16_t suffixArray[157] = 
		{
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1
		};

		std::vector<std::string> stringArray;

		// Inherited via ReadWritable
		std::expected<bool, FileError> ReadFromFile(std::filesystem::path& inputFilePath) override;
		std::expected<std::string, FileError> WriteToFile(std::filesystem::path& outputFilePath) override;


	};
	SkeletonRig ConvertToSFBGSRig(const CALUMI::UNIV::SkeletonRig& inputRig, float& highPrecision, float& lowPrecision);
	CALUMI::UNIV::SkeletonRig ConvertToUniversalRig(const CALUMI::SFBGS::SkeletonRig& inputRig);

	extern  "C" {
		CALUMIANIMATION_API UNIV::SkeletonRig* LoadRigFromFile(const wchar_t* filePath, const char* errorMessage);
	}


}}


