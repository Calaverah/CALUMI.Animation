//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Common.h"
#include "interfaces/IReadWritable.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"
#include "SFBGS_RigPackage.h"
#include "io/FileResult.h"




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

#ifdef DEBUG_BUILD
		int32_t getPad01() const;
		int32_t getPad02() const;
#endif

		//Constructors
		SkeletonBone();
		SkeletonBone(const SkeletonBone& input);
		~SkeletonBone();
		SkeletonBone(Utilities::BufferObject& buffer, unsigned long long& addressIndex);

		SkeletonBone& operator=(const SkeletonBone& input);

		//dummy operator
		bool operator<(const SkeletonBone& input);

		/// <summary>
		/// Fills end of buffer with 96 bytes and copies the information into the vector.
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="addressIndex"></param>
		void SerializeIntoBuffer(Utilities::BufferObject& buffer, unsigned long long& addressIndex) const;

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

	VECTORDEC(SkeletonBoneVector, SkeletonBone)

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
		Utilities::U64Vector getMatchingThree() const;
		void setMatchingThree(uint64_t m1, uint64_t m2, uint64_t m3);
		float LowPrecision() const;
		void LowPrecision(float value);
		float HighPrecision() const;
		void HighPrecision(float value);
		uint16_t BoneCount() const;
		void BoneCount(uint16_t count);
		uint16_t BoneCountAnimated() const;
		void BoneCountAnimated(uint16_t count);
		SkeletonBoneVector& BoneEntries() const;
		Utilities::S16Vector BoneMapArray() const;
		void BoneMapArray(Utilities::S16Vector& input);
		Utilities::StringList& StringArray() const;
#ifdef DEBUG_BUILD
		Utilities::CharVector EndOfHeader() const;
#endif

		bool IsMarkedMannequin() const;

		// Inherited via IReadWritable
		Utilities::FileResult ReadFromFile(Utilities::PathContainer& inputFilePath) override;
        Utilities::FileResult ReadFromFile(Utilities::PathContainer&& inputFilePath) override;
		Utilities::FileResult WriteToFile(Utilities::PathContainer& outputFilePath) override;
        Utilities::FileResult WriteToFile(Utilities::PathContainer&& outputFilePath) override;

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

    }

}


