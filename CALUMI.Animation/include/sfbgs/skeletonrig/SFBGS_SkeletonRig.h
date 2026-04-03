//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Common.h"
#include "interfaces/IReadWritable.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"
#include "SFBGS_RigPackage.h"
#include "io/FileError.h"




namespace CALUMI {namespace SFBGS {

	struct SkeletonRig;

	/**
	 * @brief SFBGS Struct defining a skeleton bone
	 */
	struct CALUMIANIMATION_API SkeletonBone
	{

		/**
		* @brief Bone type for SFBGS Rig Bones currently include these enums
		*
		*/
		enum class BoneType : int32_t
		{
			Default = -1, /**< Animation driven **in game** */
			Twist	=  1  /**< Dynamically driven **in game**  */
		};
		
		CALUMI::Math::Quaternion& LocalRotation() const;
		
		CALUMI::Math::Quaternion& GlobalRotation() const;
		
		CALUMI::Math::Vector3& Position() const;

		BoneType getBoneType() const;
		void setBoneType(BoneType t);
		
		uint64_t getNameOffset() const;
		void setNameOffset(uint64_t offset);
		int32_t getParentBoneIndex() const;
		void setParentBoneIndex(int32_t idx);
		int32_t getTwistDriverMqnIndex() const;
		void setTwistDriverMqnIndex(int32_t idx);
		int32_t getTwistDriverIndex() const;
		void setTwistDriverIndex(int32_t idx);
		int32_t getMirrorBoneIndex() const;
		void setMirrorBoneIndex(int32_t idx);
		int32_t getTerm05() const;
		void setTerm05(int32_t value);
		float getTwistDriverWeight() const;
		void setTwistDriverWeight(float weight);
		float getUnknownScalar() const;
		void setUnknownScalar(float value);
		int32_t getTerm08() const;
		void setTerm08(int32_t value);

#ifdef _DEBUG
		int32_t getPad01() const;
		int32_t getPad02() const;
#endif

		//Constructors
		SkeletonBone();
		SkeletonBone(const SkeletonBone& input);
		~SkeletonBone();
		SkeletonBone(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex);

		SkeletonBone& operator=(const SkeletonBone& input);

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
		
	private:
		//CONVERSION ONLY, DOES NOT ADD MQN TWIST INDEX
		bool SetBoneTypeFromUNIV(UNIV::SkeletonBone& univBone);
		bool SetBoneTypeToUNIV(UNIV::SkeletonBone& univBone);

	private:
		struct Impl;
		Impl* pImpl;

	private:
		friend struct SkeletonRig;
	};

	struct CALUMIANIMATION_API SkeletonRig : CALUMI::IReadWritable
	{
		int VersionNumber() const;
		void VersionNumber(int v);
		uint32_t FileSize() const;
		void FileSize(uint32_t size);
		uint32_t HeaderSize() const;
		void HeaderSize(uint32_t size);
		uint32_t BoneMapOffset() const;
		void BoneMapOffset(uint32_t offset);
		Utilities::VectorContainer<uint64_t> getMatchingThree() const;
		void setMatchingThree(uint64_t m1, uint64_t m2, uint64_t m3);
		float LowPrecision() const;
		void LowPrecision(float value);
		float HighPrecision() const;
		void HighPrecision(float value);
		uint16_t BoneCount() const;
		void BoneCount(uint16_t count);
		uint16_t BoneCountAnimated() const;
		void BoneCountAnimated(uint16_t count);
		Utilities::VectorContainer<SkeletonBone>& BoneEntries() const;
		Utilities::VectorContainer<int16_t> BoneMapArray() const;
		void BoneMapArray(Utilities::VectorContainer<int16_t>& input);
		Utilities::VectorContainer<Utilities::StringContainer>& StringArray() const;
#ifdef _DEBUG
		Utilities::VectorContainer<char> EndOfHeader() const;
#endif

		bool IsMarkedMannequin() const;

		// Inherited via IReadWritable
		Utilities::ExpectedContainer<bool, Utilities::FileError> ReadFromFile(Utilities::PathContainer& inputFilePath) override;
		Utilities::ExpectedContainer<bool, Utilities::FileError> ReadFromFile(const wchar_t* inputFilePath);
		Utilities::ExpectedContainer<Utilities::StringContainer, Utilities::FileError> WriteToFile(Utilities::PathContainer& outputFilePath) override;
		Utilities::ExpectedContainer<Utilities::StringContainer, Utilities::FileError> WriteToFile(const wchar_t* outputFilePath);

		~SkeletonRig();
		SkeletonRig();
		SkeletonRig(const SFBGS::SkeletonRig& input);
		SkeletonRig(const UNIV::SkeletonRig& input);

		SkeletonRig& operator=(const SkeletonRig& input);

		CALUMI::UNIV::SkeletonRig ConvertToUniversalRig() const;
		void ConvertFromUniversalRig(const CALUMI::UNIV::SkeletonRig& inputRig);

	public:
		//DEBUG FUNCTIONS
		uint8_t DEBUG_CheckAssumedHeaderEntries();
	private:
		struct Impl;
		Impl* pImpl;
	};
	

#pragma warning(disable: 4661)
	template struct CALUMIANIMATION_API Utilities::VectorContainer <SkeletonBone>;
#pragma warning(default: 4661)
}}


