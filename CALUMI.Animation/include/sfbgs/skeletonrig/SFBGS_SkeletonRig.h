//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Common.h"
#include "interfaces/IReadWritable.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"
#include "io/FileResult.h"




namespace CALUMI::SFBGS
{

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

		[[nodiscard]] Math::Quaternion& localRotation() const;

		[[nodiscard]] Math::Quaternion& globalRotation() const;

		[[nodiscard]] Math::Vector3& position() const;

		[[nodiscard]] BoneType getBoneType() const;
		void setBoneType(BoneType t) const;

		[[nodiscard]] uint64_t getNameOffset() const;
		void setNameOffset(uint64_t offset) const;
		[[nodiscard]] int32_t getParentBoneIndex() const;
		void setParentBoneIndex(int32_t idx) const;
		[[nodiscard]] int32_t getTwistDriverMqnIndex() const;
		void setTwistDriverMqnIndex(int32_t idx) const;
		[[nodiscard]] int32_t getTwistDriverIndex() const;
		void setTwistDriverIndex(int32_t idx) const;
		[[nodiscard]] int32_t getMirrorBoneIndex() const;
		void setMirrorBoneIndex(int32_t idx) const;
		[[nodiscard]] int32_t getTerm05() const;
		void setTerm05(int32_t value) const;
		[[nodiscard]] float getTwistDriverWeight() const;
		void setTwistDriverWeight(float weight) const;
		[[nodiscard]] float getUnknownScalar() const;
		void setUnknownScalar(float value) const;
		[[nodiscard]] int32_t getTerm08() const;
		void setTerm08(int32_t value) const;

#ifdef DEBUG_BUILD
		[[nodiscard]] int32_t getPad01() const;
		[[nodiscard]] int32_t getPad02() const;
#endif

		//Constructors
		SkeletonBone();
		SkeletonBone(const SkeletonBone& input);
		~SkeletonBone();
		SkeletonBone(Utilities::BufferObject& buffer, unsigned long long& addressIndex);

		SkeletonBone& operator=(const SkeletonBone& input);

		/// <summary>
		/// Fills end of buffer with 96 bytes and copies the information into the vector.
		/// </summary>
		/// <param name="buffer"></param>
		/// <param name="addressIndex"></param>
		void serializeIntoBuffer(Utilities::BufferObject& buffer, unsigned long long& addressIndex) const;

		/// <summary>
		/// Returns the converted UNIV Bone Type
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] UNIV::BoneType getBoneTypeAsUNIVEnum() const;
		[[nodiscard]] const char* getBoneTypeAsString() const;

	private:
		struct Impl;
		Impl* pImpl;

	private:
		friend struct SkeletonRig;
	};

	VECTORDEC(SkeletonBoneVector, SkeletonBone)

	struct CALUMIANIMATION_API SkeletonRig : IReadWritable
	{
		[[nodiscard]] int versionNumber() const;
		void setVersionNumber(int v) const;
		[[nodiscard]] uint32_t fileSize() const;
		void setFileSize(uint32_t size) const;
		[[nodiscard]] uint32_t headerSize() const;
		void setHeaderSize(uint32_t size) const;
		[[nodiscard]] uint32_t boneMapOffset() const;
		void setBoneMapOffset(uint32_t offset) const;
		[[nodiscard]] Utilities::U64Vector matchingThree() const;
		void setMatchingThree(uint64_t m1, uint64_t m2, uint64_t m3) const;
		[[nodiscard]] float lowPrecision() const;
		void setLowPrecision(float value) const;
		[[nodiscard]] float highPrecision() const;
		void setHighPrecision(float value) const;
		[[nodiscard]] uint16_t boneCount() const;
		void setBoneCount(uint16_t count) const;
		[[nodiscard]] uint16_t boneCountAnimated() const;
		void setBoneCountAnimated(uint16_t count) const;
		[[nodiscard]] SkeletonBoneVector& boneEntries() const;
		[[nodiscard]] Utilities::S16Vector boneMapArray() const;
		void setBoneMapArray(Utilities::S16Vector& input) const;
		[[nodiscard]] Utilities::StringList& stringArray() const;


		[[nodiscard]] bool IsMarkedMannequin() const;

		// Inherited via IReadWritable
		Utilities::FileResult readFromFile(Utilities::PathContainer& inputFilePath) override;
		Utilities::FileResult readFromFile(Utilities::PathContainer&& inputFilePath) override;
		Utilities::FileResult writeToFile(Utilities::PathContainer& outputFilePath) override;
		Utilities::FileResult writeToFile(Utilities::PathContainer&& outputFilePath) override;

		~SkeletonRig() override;
		SkeletonRig();
		SkeletonRig(const SkeletonRig& input);
		explicit SkeletonRig(const UNIV::SkeletonRig& input);

		SkeletonRig& operator=(const SkeletonRig& input);

		[[nodiscard]] UNIV::SkeletonRig convertToUniversalRig() const;
		void convertFromUniversalRig(const UNIV::SkeletonRig& inputRig);

		bool setBoneTypeFromUNIV(const UNIV::SkeletonBone& univBone, const SkeletonBone& sfbgsBone, const UNIV::SkeletonRig* univRig = nullptr) const; // NOLINT(*-use-nodiscard)
		bool setBoneTypeToUNIV(const SkeletonBone& sfbgsBone, const UNIV::SkeletonBone& univBone) const; // NOLINT(*-use-nodiscard)

		int findBoneIndex(const char* boneName) const;

#ifdef DEBUG_BUILD
		//DEBUG FUNCTIONS
	public:
		[[nodiscard]] Utilities::CharVector endOfHeader() const;
		[[nodiscard]] uint8_t checkAssumedHeaderEntries() const;
#endif

	private:
		struct Impl;
		Impl* pImpl;
	};

}


