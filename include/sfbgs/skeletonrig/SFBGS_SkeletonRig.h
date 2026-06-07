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

	class SkeletonRig;

	/**
	 * @brief SFBGS Struct defining a skeleton bone
	 */
	class CALUMIANIMATION_API SkeletonBone
	{
	public:
		/// @name Runtime Behavior
		/// @{
		/**
		* @brief Bone type for SFBGS Rig Bones currently include these enums
		*/
		enum class BoneType : int32_t
		{
			Default = -1, ///< Animation driven **in game**
			Twist	=  1  ///< Dynamically driven **in game**
		};
		/**
		 * @return
		 */
		[[nodiscard]] BoneType boneType() const;
		/**
		 *
		 * @param t
		 */
		void setBoneType(BoneType t) const;
		/**
		 * @brief Returns the converted UNIV Bone Type
		 */
		[[nodiscard]] UNIV::BoneTypeProperty::BoneType boneTypeAsUNIVEnum() const;
		/**
		 * @return Returns the UNIV Bone Type in the form of a string
		 */
		[[nodiscard]] const char* boneTypeAsString() const;
		/// @}
		/// @name Transform
		/// @{

		/**
		 * @return rotation relative to this bone's parent
		 */
		[[nodiscard]] Math::Quaternion& localRotation() const;
		/**
		 * @return
		 */
		[[nodiscard]] Math::Quaternion& globalRotation() const;
		/**
		 * @return position relative to this bone's parent
		 */
		[[nodiscard]] Math::Vector3& position() const;
		/// @}
		/// @name Name
		/// @{

		/**
		 * @warning address/offset may not be accurate unless the rig is directly imported from a file and remains
		 * unmodified
		 * @return address of the C String relative to beginning of the rig file.
		 */
		[[nodiscard]] uint64_t nameOffset() const;
		/**
		 * @warning Setting the offset of this bone's name is not advised
		 * @param offset
		 */
		void setNameOffset(uint64_t offset) const;
		/// @}
		/// @name Parent
		/// @{
		/**
		 * @return
		 */
		[[nodiscard]] int32_t parentBoneIndex() const;
		/**
		 * @param idx
		 */
		void setParentBoneIndex(int32_t idx) const;
		/// @}
		/// @name Twist Behavior
		/// @{
		/**
		 * @return -1 on standard rigs, or the parent index if this twist bone belongs to a mannequin rig
		 */
		[[nodiscard]] int32_t twistDriverMqnIndex() const;
		/**
		 * @param idx -1 on standard rigs, or the parent index if this twist bone belongs to a mannequin rig
		 */
		void setTwistDriverMqnIndex(int32_t idx) const;
		/**
		 * @return if -1, this bone has a deterministic animation, if a non-negative number, the bone's movement is
		 * determined at runtime where it is influenced by the index given
		 */
		[[nodiscard]] int32_t twistDriverIndex() const;
		/**
		 * @param idx Sets the index of this bone's twist influencer, or if -1, sets this bone to act as an animated
		 * bone
		 */
		void setTwistDriverIndex(int32_t idx) const;
		/**
		 * @return The twist driving weight for this bone, if it is to behave as a twist bone. Will be negative if
		 * the twist bone's influencer is its own parent (however, it may apply if the index of the influencer is
		 * less than the bone's index, it is not known)
		 */
		[[nodiscard]] float twistDriverWeight() const;
		/**
		 * @param weight The twist driving weight for this bone, if it is to behave as a twist bone. Should be
		 * negative if the twist bone's influencer is its own parent (however, it may apply if the index of the
		 * influencer is less than the bone's index, it is not known)
		 */
		void setTwistDriverWeight(float weight) const;
		/// @}
		/// @name Mirror Behavior
		/// @{
		/**
		 * @return The index of the bone this one is set to mirror, or if no mirror is desired, the index will either
		 * be set to this bone's own index, or on a very rare occasion -1
		 */
		[[nodiscard]] int32_t mirrorBoneIndex() const;
		/**
		 * @param idx The index of the bone this one should mirror, or its own index if no mirror behavior is desired
		 */
		void setMirrorBoneIndex(int32_t idx) const;
		/// @}
		/// @name Level Of Detail Behavior
		/// @{
		/**
		 * @return The LOD Setting for this bone. See @ref CALUMI::SFBGS::SFBGS_RigPackage::LODSetting "LOD" for more
		 * information
		 */
		[[nodiscard]] int32_t levelOfDetail() const;
		/**
		 * @param value The LOD Setting for this bone. See @ref CALUMI::SFBGS::SFBGS_RigPackage::LODSetting "LOD" for
		 * more information
		 */
		void setLevelOfDetail(int32_t value) const;
		/// @}
#ifdef DEBUG_BUILD
		/// @name Debug Only
		/// @{
		/**
		 *
		 * @return
		 */
		[[nodiscard]] float unknownScalar() const;
		/**
		 *
		 * @param value
		 */
		void setUnknownScalar(float value) const;

		/**
		 *
		 * @return garbage data leftover in the structure
		 */
		[[nodiscard]] int32_t term08() const;
		/**
		 *
		 * @param value garbage data
		 */
		void setTerm08(int32_t value) const;
		/**
		 * @return Should always be -1
		 */
		[[nodiscard]] int32_t pad01() const;
		/**
		 * @return Should always be 0
		 */
		[[nodiscard]] int32_t pad02() const;
		/// @}
#endif

		/// @name Initialization
		/// @{
		SkeletonBone();
		SkeletonBone(const SkeletonBone& input);
		~SkeletonBone();
		SkeletonBone(Utilities::BufferObject& buffer, unsigned long long& addressIndex);
		/// @}
		/// @name Operators
		/// @{
		SkeletonBone& operator=(const SkeletonBone& input);
		/// @}
	private:
		/**
		 * @brief Fills end of buffer with 96 bytes and copies the information into the vector
		 * @param buffer
		 * @param addressIndex
		 */
		void serializeIntoBuffer(Utilities::BufferObject& buffer, unsigned long long& addressIndex) const;

		struct Impl;
		Impl* pImpl;

		friend class SkeletonRig;
	};

	VECTORDEC(SkeletonBoneVector, SkeletonBone)

	class CALUMIANIMATION_API SkeletonRig : public IReadWritable
	{
		void setVersionNumber(int v) const;
		void setFileSize(uint32_t size) const;
		void setHeaderSize(uint64_t size) const;
		void setBoneMapOffset(uint64_t offset) const;
		void setMatchingThree(uint64_t m1, uint64_t m2, uint64_t m3) const;
		void setBoneCount(uint16_t count) const;
		void setBoneCountAnimated(uint16_t count) const;
	public:
		/// @name Data
		/// @{
		/**
		 * @return The input file's version number
		 */
		[[nodiscard]] int versionNumber() const;
		/**
		 * @return file size recorded during import
		 */
		[[nodiscard]] uint32_t fileSize() const;
		/**
		 * @return Size of header, or alternatively, the address offset in which the bone entry array begins
		 */
		[[nodiscard]] uint64_t headerSize() const;
		/**
		 * @return Address offset to the bone map array
		 */
		[[nodiscard]] uint64_t boneMapOffset() const;
		/**
		 * @return Benign address data leftover during export
		 */
		[[nodiscard]] Utilities::U64Vector matchingThree() const;
		/**
		 * @return Low precision value for compression calculations on translations
		 */
		[[nodiscard]] float lowPrecision() const;
		/**
		 * @param value sets the low precision value
		 */
		void setLowPrecision(float value) const;
		/**
		 * @return High precision value for compression calculations on translations
		 */
		[[nodiscard]] float highPrecision() const;
		/**
		 *
		 * @param value sets the high precision value
		 */
		void setHighPrecision(float value) const;
		/**
		 * @return returns the total bone count
		 */
		[[nodiscard]] uint16_t boneCount() const;
		/**
		 * @return returns the number of non-twist bones
		 */
		[[nodiscard]] uint16_t boneCountAnimated() const;
		/**
		 * @return A Vector of the bone entries
		 */
		[[nodiscard]] SkeletonBoneVector& boneEntries() const;
		/**
		 * @brief The bone map features an array of 157 entries with a value of either -1 or the index of the bone
		 * assigned to the key. Correlates to @ref CALUMI::SFBGS::SFBGS_RigPackage::BoneMapKey "Bone Map Keys"
		 * @return A Vector of the bone map
		 */
		[[nodiscard]] Utilities::S16Vector boneMapArray() const;
		/**
		 * @param input
		 */
		void setBoneMapArray(Utilities::S16Vector& input) const;
		/**
		 *
		 * @return
		 */
		[[nodiscard]] Utilities::StringList& stringArray() const;
		/**
		 * @return Whether this rig has the characteristics found only on mannequin rigs
		 */
		[[nodiscard]] bool isMarkedMannequin() const;
		/// @}
		/// @name File IO
		/// @{
		Utilities::FileResult readFromFile(Utilities::PathContainer& inputFilePath) override;
		Utilities::FileResult readFromFile(Utilities::PathContainer&& inputFilePath) override;
		Utilities::FileResult writeToFile(Utilities::PathContainer& outputFilePath) override;
		Utilities::FileResult writeToFile(Utilities::PathContainer&& outputFilePath) override;
		/// @}
		/// @name Initialization
		/// @{

		~SkeletonRig() override;
		SkeletonRig();
		SkeletonRig(const SkeletonRig& input);
		explicit SkeletonRig(const UNIV::SkeletonRig& input);

		/// @}
		/// @name Operators
		/// @{
		SkeletonRig& operator=(const SkeletonRig& input);
		/// @}
		/// @name Conversion
		/// @{

		[[nodiscard]] UNIV::SkeletonRig convertToUniversalRig() const;
		void convertFromUniversalRig(const UNIV::SkeletonRig& inputRig);

		bool setBoneTypeFromUNIV(const UNIV::SkeletonBone& univBone, const SkeletonBone& sfbgsBone, const Utilities::StringList* boneList = nullptr) const; // NOLINT(*-use-nodiscard)
		bool setBoneTypeToUNIV(const SkeletonBone& sfbgsBone, const UNIV::SkeletonBone& univBone) const; // NOLINT(*-use-nodiscard)
		/// @}
		/// @name Bone Access
		/// @{
		int findBoneIndex(const char* boneName) const;

		[[nodiscard]] SkeletonBone* boneEntry(int index) const;
		/// @}

#ifdef DEBUG_BUILD
		/// @name Debug only
		/// @{
	
		[[nodiscard]] Utilities::CharVector endOfHeader() const;
		[[nodiscard]] uint8_t checkAssumedHeaderEntries() const;
		/// @}
#endif

	private:
		struct Impl;
		Impl* pImpl;
	};

}


