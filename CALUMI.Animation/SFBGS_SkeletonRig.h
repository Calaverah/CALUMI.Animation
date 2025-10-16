//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_ReadWritable.h"
#include "CALUMI_SkeletonRig.h"
#include "SFBGS_RigPackage.h"
#include "FileError.h"




namespace CALUMI {namespace SFBGS {

	enum class BoneType : int32_t
	{
		Default		= -1,
		Twist		=  1
	};

	struct CALUMIANIMATION_API SkeletonBone
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
		_PRIVATE_(_pad01)
		/// <summary>
		/// Same as this bone's index by default
		/// </summary>
		int32_t mirrorBoneIndex = 0;
		int32_t term05 = 0;
		/// <summary>
		/// Negative if pointing to parent
		/// </summary>
		float twistDriverWeight = 0;
		_PRIVATE_(_pad02)
		float unknownScalar = 0.0;
		int32_t term08 = 0;


		//Constructors
		SkeletonBone() = default;
		SkeletonBone(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex);

		/// <summary>
		/// Fills end of buffer with 96 bytes and copies the information into the vector.
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="addressIndex"></param>
		void SerializeIntoBuffer(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex) const;

		/// <summary>
		/// Returns the converted UNIV Bone Type
		/// </summary>
		/// <returns></returns>
		UNIV::BoneType GetBoneTypeAsUNIVEnum();
		const char* GetBoneTypeAsString();

		//CONVERSION ONLY
		bool SetBoneTypeFromUNIV(UNIV::SkeletonBone& univBone);
		bool SetBoneTypeToUNIV(UNIV::SkeletonBone& univBone);

	private:
		/// <summary>
		/// Always -1
		/// </summary>
		int32_t _pad01 = -1;
		/// <summary>
		/// Always 0
		/// </summary>
		int32_t _pad02 = 0; 
	};



	struct CALUMIANIMATION_API SkeletonRig : CALUMI::ReadWritable
	{

		//HEADER
		int versionNumber = 05;

		unsigned int fileSize = 0;

		/// <summary>
		/// Currently the only value seen is 0x50 (80)
		/// </summary>
		int headerSize = 0x50;

		_PRIVATE_(headerEmpty01)
		/// <summary>
		/// 96* bone count + 80 bytes
		/// </summary>
		unsigned int boneMapOffset = 0;

		_PRIVATE_(headerEmpty02)
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
		_PRIVATE_(headerEmpty03)
		/// <summary>
		/// ;)
		/// </summary>
		uint8_t endOfHeader[16] = {0x0, 0x0, 0x43, 0x41, 0x4C, 0x55, 0x4D, 0x49, 0x44, 0x56, 0x52, 0x53, 0x4A, 0x4F, 0x4A, 0x4F };


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

		bool IsMarkedMannequin() const;

		// Inherited via ReadWritable
		Utilities::ExpectedConatiner<bool, FileError> ReadFromFile(Utilities::PathContainer& inputFilePath) override;
		Utilities::ExpectedConatiner<bool, FileError> ReadFromFile(const wchar_t* inputFilePath);
		Utilities::ExpectedConatiner<Utilities::StringContainer, FileError> WriteToFile(Utilities::PathContainer& outputFilePath) override;
		Utilities::ExpectedConatiner<Utilities::StringContainer, FileError> WriteToFile(const wchar_t* outputFilePath);

	private:
		/// <summary>
		/// Always empty, possibly padding, see _PRIVATE_ for placement in struct
		/// </summary>
		unsigned int headerEmpty01 = 0;
		/// <summary>
		/// Always empty, possibly padding, see _PRIVATE_ for placement in struct
		/// </summary>
		unsigned int headerEmpty02 = 0;
		/// <summary>
		/// Always empty, possibly padding, see _PRIVATE_ for placement in struct
		/// </summary>
		unsigned int headerEmpty03 = 0;

	public:
		//DEBUG FUNCTIONS
		uint8_t DEBUG_CheckAssumedHeaderEntries();
	};
	SkeletonRig ConvertToSFBGSRig(CALUMI::UNIV::SkeletonRig& inputRig, float& highPrecision, float& lowPrecision);
	CALUMI::UNIV::SkeletonRig ConvertToUniversalRig(CALUMI::SFBGS::SkeletonRig& inputRig);

#pragma warning(disable: 4661)
	template struct CALUMIANIMATION_API Utilities::VectorContainer <SkeletonBone>;
#pragma warning(default: 4661)
}}


