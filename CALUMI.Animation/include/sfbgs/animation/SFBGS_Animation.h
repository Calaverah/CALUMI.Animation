//Copyright � 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "interfaces/IReadWritable.h"
#include "SFBGS_AnimationEntries.h"
#include <io/FileResult.h>


namespace CALUMI{

	
	namespace SFBGS {

		class AnimationScene;

		/**
		 * @brief Flags defined in the SFBGS animation file's header
		 */
		struct CALUMIANIMATION_API HeaderFlags
		{
			/**
			 * @name Constructors
			 * @{
			 */
			HeaderFlags();
			~HeaderFlags();
			HeaderFlags(char c0, char c1, char c2, char c3);
			HeaderFlags(char input[4]);
			HeaderFlags(const HeaderFlags& input);
			/// @}

			/**
			 * @name Operators
			 * @{
			 */
			HeaderFlags& operator=(const HeaderFlags& input);
			/// @}

			/**
			 * @name Known Data
			 * @{
			 */

			/**
			 * @brief Unknown flag that appears unused
			 * @return 
			 */
			bool getFirstEntry() const;
			/**
			 * @brief Unknown flag that appears unused
			 * @param input 
			 */
			void setFirstEntry(bool input);
			/**
			 * @brief Counter size flag
			 * @return Whether animation block counters are 1 or 2 bytes in size
			 */
			bool areKeyCounters2Byte() const;
			/**
			 * @brief Counter size flag
			 * @param input Whether animation block counters are 1 or 2 bytes in size
			 */
			void setKeyCountersFlag(bool input);
            /**
			 * @brief Key Frame size flag
			 * @return Whether animation block key frames are 1 or 2 bytes in size
			 */
			bool areKeyFrameEntries2Byte() const;
			/**
			 * @brief Key Frame size flag
			 * @param input Whether animation block key frames are 1 or 2 bytes in size
			 */
			void setKeyFrameEntriesFlag(bool input);
			/**
			 * @brief Scalars present flag
			 * @return Whether scalars are present
			 */
			bool hasScalarSequence() const;
			/**
			 * @brief Scalars present flag
			 * @param input Whether scalars are present
			 */
			void setScalarSequenceFlag(bool input);
			/**
			 * @brief Main byte of flag data
			 * @return Byte containing the 3 used flags
			 */
			char getData() const;

			/// @}

			/**
			 * @name Unknown or Unused
			 * @{
			 */
			bool getU1() const;
			bool getU2() const;
			bool getU3() const;
			bool getU4() const;
			void setU1(bool input);
			void setU2(bool input);
			void setU3(bool input);
			void setU4(bool input);

			char getC1() const;
			char getC2() const;
			char getC3() const;
			void setC1(char input);
			void setC2(char input);
			void setC3(char input);

			/// @}

		private:
			struct Impl;
			Impl* pImpl;
		};

		/**
		 * @brief Section of data that describes transform sequences
		 */
		struct CALUMIANIMATION_API AnimationBlock
		{
			/**
			 * @name Constructors
			 * @{
			 */
			
			AnimationBlock();
			AnimationBlock(const AnimationBlock& input);
			AnimationBlock(Utilities::BufferObject& buffer, unsigned long long& addressIndex, const HeaderFlags& flags);
			~AnimationBlock();

			/// @}
			/// @name Operators
			/// @{
			
			AnimationBlock& operator=(const AnimationBlock& input);
			
			/// @}
			/**
			 * @name Counts
			 */
			
			/**
			 * @brief 
			 * @return The number of rotation entries (suffix) 
			 */
			uint16_t getRotationCount() const;
			/**
			 * @brief 
			 * @return The number of rotation entries (prefix) 
			 */
			uint16_t getRotationPrefixCount() const;
			/**
			 * @brief 
			 * @return The number of translation entries (suffix) 
			 */
			uint16_t getTranslationCount() const;
			/**
			 * @brief 
			 * @return The number of translation entries (prefix) 
			 */
			uint16_t getTranslationPrefixCount() const;
			/**
			 * @brief 
			 * @return The number of scalar entries 
			 */
			uint16_t getScalarCount() const;
			/**
			 * @brief 
			 * @return The number of "priority" entries 
			 */
			uint16_t getPriorityCount() const;


			void setRotationCount(uint16_t input);
			void setRotationPrefixCount(uint16_t input);
			void setTranslationCount(uint16_t input);
			void setTranslationPrefixCount(uint16_t input);
			void setScalarCount(uint16_t input);
			void setPriorityCount(uint16_t input);

			/// @}

			/**
			 * @name Key Frames 
			 * @{
			 */

			/**
			 * @brief Vector of Keyframes of size [RotationCount]
			 * @return 
			 */
			Utilities::U16Vector& RotationKeyFrames() const;
			/**
			 * @brief Vector of Keyframes of size [TranslationCount] 
			 * @return 
			 */
			Utilities::U16Vector& TranslationKeyFrames() const;
			/**
			 * @brief Vector of Keyframes of size [ScalarCount]
			 * @return 
			 */
			Utilities::U16Vector& ScalarKeyFrames() const;
			/**
			 * @brief Vector of Keyframes of size [PriorityCount]
			 * @return 
			 */
			Utilities::U16Vector& BonePriorityKeyFrames() const;

			/// @}
			/**
			* @name Compressed Entries
			*/

			/**
			 * @brief "Suffix" of the compressed Rotation entries, one for each frame
			 * @return 
			 */
			RotationEntrySequence& RotationEntries() const;
			/**
			 * @brief "Prefix" of the compress Rotation entries, RLE sequence
			 * @details The "prefix" entries are folded into an RLE sequence where there is a counter on the bitfield that describes how many frames this entries applies to using the programitc counting method (0,1,2...)
			 * @return 
			 */
			SFBGS::RotationPrefixSequence& RotationPrefixEntries() const;
			/**
			 * @brief "Suffix" of the compressed Translation entries, one for each frame
			 * @return 
			 */
			SFBGS::TranslationEntrySequence& TranslationEntries() const;
			/**
			 * @brief "Prefix" of the compress Translation entries, RLE sequence
			 * @details The "prefix" entries are folded into an RLE sequence where there is a counter on the bitfield that describes how many frames this entries applies to using the common counting method (1,2, 3...)
			 * @return
			 */
			SFBGS::TranslationPrefixSequence& TranslationPrefixEntries() const;
			/**
			 * @brief Scalar entries
			 * @details Value described with a signed short (2 Bytes) where 5000 is the base scale of 1.0f
			 * @return 
			 */
			Utilities::S16Vector& ScalarEntries() const;
			/**
			 * @brief "Priority" entries
			 * @details Not much is known about these. It is believed that they are used by additive animations to determine which animation take precedence when transforming this bone
			 * @return 
			 */
			Utilities::U8Vector& BonePriorityEntries() const;

			/// @}
			/// @name Serialization
			/// @{
			/**
			 * @brief Method for serializing data into the expected file format
			 * @param buffer 
			 * @param addressIndex 
			 * @param flags 
			 */
			void SerializeIntoBuffer(Utilities::BufferObject& buffer, unsigned long long& addressIndex, const HeaderFlags & flags);
			/// @}
			

		private:
			struct Impl;
			Impl* pImpl;
		};

		struct CALUMIANIMATION_API AnimationBlockVector
		{
			AnimationBlockVector();
			~AnimationBlockVector();

			void push_back(const AnimationBlock& val);
			uint64_t size() const;
			bool empty() const;
			void reserve(uint64_t size);
			void resize(uint64_t size);

			AnimationBlock& at(uint64_t idx) const;

		private:
			struct Impl;
			Impl* pImpl;
		};

		/**
		 * @brief Not much is known about this just yet. Nothing that has been proven that is
		 */
		struct CALUMIANIMATION_API Preamble
		{
			uint16_t getCount() const;
			void setCount(uint16_t sz);

			Utilities::FloatVector& getPreSet() const;
			void setPreSet(const Utilities::FloatVector& input);
			Utilities::FloatVector& getMainSet() const;
			void setMainSet(const Utilities::FloatVector& input);
			Utilities::S16Vector& getFooter1() const;
			void setFooter1(const Utilities::S16Vector& input);
			Utilities::S8Vector& getFooter2() const;
			void setFooter2(const Utilities::S8Vector& input);


			Preamble();
			~Preamble();
			Preamble(Utilities::BufferObject& buffer, unsigned long long& addressIndex, uint64_t frameCount);
			Preamble& operator=(const Preamble& other);
			Preamble(const Preamble& other);

		private:
			struct Impl;
			Impl* pImpl;
		};

		struct CALUMIANIMATION_API PreambleVector
		{
			PreambleVector();
			~PreambleVector();

			void push_back(const Preamble& val);
			uint64_t size() const;
			bool empty() const;
			void reserve(uint64_t size);
			void resize(uint64_t size);

			Preamble& at(uint64_t idx) const;

		private:
			struct Impl;
			Impl* pImpl;
		};

		/**
		 * @brief Starfield Animation
		 */
		class CALUMIANIMATION_API Animation : CALUMI::IReadWritable
		{
		public:
			/**
			 * @brief Index Counting Solution for the Index Atlas
			 */
			enum class IndexCountingSolution : uint8_t
			{
				odd		= 0, ///< 1,3,5,etc
				even	= 1, ///< 0,2,4,etc
				all		= 2  ///< all indices
			};

		public:
			/**
			 * @name Constructors
			 * @{
			 */
			
			Animation();
			Animation(const Animation& input);
			~Animation();
			
			/// @}
			/// @name Operators
			/// @{
			
			Animation& operator=(const Animation& input);
			
			/// @}
			/// @name Meta
			/// @{
			/// 
			uint64_t getSourceFileSize() const;
			
			CALUMI::Utilities::StringContainer& getAnimationFileName() const;
			void setAnimationFileName(const CALUMI::Utilities::StringContainer& input);
		
			/// @}
			/// @name Header Data
			/// @{

			uint64_t getMagicNumber() const;
			void setMagicNumber(uint64_t input);

			Math::Quaternion& getHeaderRotation() const;
			void setHeaderRotation(const Math::Quaternion& input);

			Math::Vector3& getHeaderTranslation() const;
			void setHeaderTranslation(const Math::Vector3& input);

			HeaderFlags& getHeaderFlags() const;
			void setHeaderFlags(const HeaderFlags& input);
			void evaluateHeaderFlags();

			short getVersionNumber() const;
			void setVersionNumber(short v);


			uint16_t getBoneCount() const;
			void setBoneCount(uint16_t input);
			uint16_t getFrameCount() const;
			void setFrameCount(uint16_t input);
			uint16_t getIndexAtlasCount() const;
			void setIndexAtlasCount(uint16_t input);

			/**
			 * @brief Count of Non-Rig based animation blocks
			 * @return 
			 */
			uint16_t getAmendedBlockCount() const;
			void setAmendedBlockCount(uint16_t input);

			uint16_t getPreambleOffset() const;
			void setPreambleOffset(uint16_t input);

			CALUMI::Utilities::FloatVector getNZeroFloats() const;
			void setNZeroFloats(float input[3]);

			/**
			 * @brief Hash values to apply the amended animation blocks. The original string is lost during export and must be retraced
			 * @return 
			 */
			Utilities::U32Vector& getAmendedHashSet() const;
			void setAmendedHashSet(const Utilities::U32Vector& input);

			uint32_t getPreambleCount() const;
			void setPreambleCount(uint32_t input);
			PreambleVector& getPreamble() const;
			void setPreamble(const PreambleVector& input);

			
			/// @}
			/// @name Animation Block Entries
			/// @{

			/**
			 * @brief Index Atlas describes how the following data is assigned
			 * @details The atlas consists of an array of unsigned values, the first value (and following odd index values) being the amount of bones to skip during processing 
			 * and the second value (and following even index values) being the amount of bones to apply the data to. \n 
			 * For example an array {1, 9, 11, 1} will skip the SkeletonRig's root bone, apply the first 11 animation blocks to bone indices 1-9, skip bone indices 10-20, 
			 * and then finally apply the final blocks to the bone at index 21
			 * @return 
			 */
			Utilities::U16Vector& getIndexAtlas() const;
			/**
			 * @brief 
			 * @param input An evenly sized array of unsigned values
			 */
			void setIndexAtlast(const Utilities::U16Vector& input);

			/**
			 * @brief The array of animation blocks to apply to the given Starfield skeleton rig
			 * @return 
			 */
			AnimationBlockVector& getAnimationBlocks() const;
			void setAnimationBlocks(const AnimationBlockVector& input);
			/**
			 * @brief The array of animation blocks that are applied by hash value in game, to some form of AnimObject
			 * @return 
			 */
			AnimationBlockVector& getAmendedAnimationBlocks() const;
			void setAmendedAnimationBlocks(const AnimationBlockVector& input);

			/// @}

		public:
			/// @name IReadWritable
			/// @{
			
            Utilities::FileResult readFromFile(Utilities::PathContainer& inputFilePath) override;
            Utilities::FileResult readFromFile(Utilities::PathContainer&& inputFilePath) override;
            Utilities::FileResult writeToFile(Utilities::PathContainer& outputFilePath) override;
            Utilities::FileResult writeToFile(Utilities::PathContainer&& outputFilePath) override;

			/// @}

		public:
			/// @name Validation
			/// @{
			
			/**
			 * @brief Confirms the sizes of various amended block data to be equal
			 * @return 
			 */
			bool VerifyAmendedBlocks() const;

			/// @}

		private:
			struct Impl;
			Impl* pImpl;

		private:
			friend class AnimationScene;
		};
    }
}

