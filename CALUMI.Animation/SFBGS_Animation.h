//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_ReadWritable.h"
#include "SFBGS_AnimationEntries.h"



namespace CALUMI{
	namespace SFBGS {

		enum class CALUMIANIMATION_API IndexCountingSolution : uint8_t
		{
			odd, //1,3,5,etc
			even, //0,2,4,etc
			all
		};

		struct CALUMIANIMATION_API HeaderFlags
		{
			bool getFirstEntry() const;
			void setFirstEntry(bool input);
			bool areKeyCounters2Byte() const;
			void setKeyCountersFlag(bool input);
			bool areKeyFrameEntries2Byte() const;
			void setKeyFrameEntriesFlag(bool input);
			bool hasScalarSequence() const;
			void setScalarSequenceFlag(bool input);

			bool getU1() const;
			bool getU2() const;
			bool getU3() const;
			bool getU4() const;
			void setU1(bool input);
			void setU2(bool input);
			void setU3(bool input);
			void setU4(bool input);

			char getData() const;
			char getC1() const;
			char getC2() const;
			char getC3() const;
			void setC1(char input);
			void setC2(char input);
			void setC3(char input);

			HeaderFlags();
			~HeaderFlags();
			HeaderFlags(char c0, char c1, char c2, char c3);
			HeaderFlags(char input[4]);
			HeaderFlags(const HeaderFlags& input);

			HeaderFlags& operator=(const HeaderFlags& input);

		private:
			struct Impl;
			Impl* pImpl;
		};

		struct CALUMIANIMATION_API AnimationBlock
		{
			
			uint16_t getRotationCount() const;
			uint16_t getRotationPrefixCount() const;
			uint16_t getTranslationCount() const;
			uint16_t getTranslationPrefixCount() const;
			uint16_t getScalarCount() const;
			uint16_t getPriorityCount() const;

			void setRotationCount(uint16_t input);
			void setRotationPrefixCount(uint16_t input);
			void setTranslationCount(uint16_t input);
			void setTranslationPrefixCount(uint16_t input);
			void setScalarCount(uint16_t input);
			void setPriorityCount(uint16_t input);

			Utilities::VectorContainer<uint16_t>& RotationKeyFrames() const;
			Utilities::VectorContainer<uint16_t>& TranslationKeyFrames() const;
			Utilities::VectorContainer<uint16_t>& ScalarKeyFrames() const;
			Utilities::VectorContainer<uint16_t>& BonePriorityKeyFrames() const;

			Utilities::VectorContainer<CALUMI::SFBGS::RotationEntry>& RotationEntries() const;
			Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix>& RotationPrefixEntries() const;
			Utilities::VectorContainer<CALUMI::SFBGS::TranslationEntry>& TranslationEntries() const;
			Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix>& TranslationPrefixEntries() const;

			Utilities::VectorContainer<int16_t>& ScalarEntries() const;
			Utilities::VectorContainer<uint8_t>& BonePriorityEntries() const;


			AnimationBlock();
			AnimationBlock(const AnimationBlock& input);
			~AnimationBlock();
			AnimationBlock(Utilities::VectorContainer<char>&buffer, unsigned long long& addressIndex, const HeaderFlags & flags);
			void SerializeIntoBuffer(Utilities::VectorContainer<char>&buffer, unsigned long long& addressIndex, const HeaderFlags & flags);

			AnimationBlock& operator=(const AnimationBlock& input);

		private:
			struct Impl;
			Impl* pImpl;
		};

		struct CALUMIANIMATION_API Preamble
		{
			uint16_t getCount() const;
			void setCount(uint16_t sz);

			Utilities::VectorContainer<float>& getPreSet() const;
			void setPreSet(const Utilities::VectorContainer<float>& input);
			Utilities::VectorContainer<float>& getMainSet() const;
			void setMainSet(const Utilities::VectorContainer<float>& input);
			Utilities::VectorContainer<int16_t>& getFooter1() const;
			void setFooter1(const Utilities::VectorContainer<int16_t>& input);
			Utilities::VectorContainer<int8_t>& getFooter2() const;
			void setFooter2(const Utilities::VectorContainer<int8_t>& input);


			Preamble();
			~Preamble();
			Preamble(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex, size_t frameCount);
			Preamble& operator=(const Preamble& other);
			Preamble(const Preamble& other);

		private:
			struct Impl;
			Impl* pImpl;
		};

		
		class CALUMIANIMATION_API Animation : CALUMI::ReadWritable
		{

		public:
			size_t getSourceFileSize() const;
			
			CALUMI::Utilities::StringContainer& getAnimationFileName() const;
			void setAnimationFileName(const CALUMI::Utilities::StringContainer& input);
		
			uint64_t getMagicNumber() const;
			void setMagicNumber(uint64_t input);

			Math::Quaternion& getHeaderRotation() const;
			void setHeaderRotation(const Math::Quaternion& input);

			Math::Vector3& getHeaderTranslation() const;
			void setHeaderTranslation(const Math::Vector3& input);

			HeaderFlags& getHeaderFlags() const;
			void setHeaderFlags(const HeaderFlags& input);

			short getVersionNumber() const;
			void setVersionNumber(short v);


			uint16_t getBoneCount() const;
			void setBoneCount(uint16_t input);
			uint16_t getFrameCount() const;
			void setFrameCount(uint16_t input);
			uint16_t getIndexAtlasCount() const;
			void setIndexAtlasCount(uint16_t input);

			uint16_t getUnknownFillCount() const;
			void setUnknownFillCount(uint16_t input);

			uint16_t getPreambleOffset() const;
			void setPreambleOffset(uint16_t input);

			CALUMI::Utilities::VectorContainer<float> getNZeroFloats() const;
			void setNZeroFloats(float input[3]);

			Utilities::VectorContainer<float>& getUnknownSuffixFillFloats() const;
			void setUnknownSuffixFillFloats(const Utilities::VectorContainer<float>& input);
			//--------------------------------------------------The header to this point is 64 bytes. The Animation blocks begin at address = 64+preambleOffset

			//preamble section goes here
				//NOTE: Preamble begins after the nZeroFloat array and is [preambleOffset] bytes long.
			uint32_t getPreambleCount() const;
			void setPreambleCount(uint32_t input);
			Utilities::VectorContainer<Preamble>& getPreamble() const;
			void setPreamble(const Utilities::VectorContainer<Preamble>& input);

			Utilities::VectorContainer<float>& getSuffixFillerValues() const;
			void getSuffixFillerValues(const Utilities::VectorContainer<float>& input);

			Utilities::VectorContainer<uint16_t>& getIndexAtlas() const;
			void setIndexAtlast(const Utilities::VectorContainer<uint16_t>& input);

			Utilities::VectorContainer<AnimationBlock>& getAnimationBlocks() const;
			Utilities::VectorContainer<AnimationBlock>& getAnimationSuffixBlocks() const;
			void setAnimationBlocks(const Utilities::VectorContainer<AnimationBlock>& input);
			void setAnimationSuffixBlocks(const Utilities::VectorContainer<AnimationBlock>& input);

			unsigned short _SumIndices(Utilities::VectorContainer<unsigned short>inputVector, IndexCountingSolution type);
			/// <summary>
			/// Header flags will be reset based on the values of the entries. It is not recommended to call this directly.
			/// </summary>
			void _evaluateHeaderFlags();

			Animation();
			Animation(const Animation& input);
			~Animation();

			Animation& operator=(const Animation& input);

			// Inherited via CALUMI::ReadWritable
			Utilities::ExpectedContainer<bool, FileError> ReadFromFile(Utilities::PathContainer& inputFilePath) override;
			Utilities::ExpectedContainer<bool, FileError> ReadFromFile(const wchar_t* inputFilePath);
			Utilities::ExpectedContainer<Utilities::StringContainer, FileError> WriteToFile(Utilities::PathContainer& outputFilePath) override;
			Utilities::ExpectedContainer<Utilities::StringContainer, FileError> WriteToFile(const wchar_t* outputFilePath);

		private:
			struct Impl;
			Impl* pImpl;
		};
#pragma warning(disable: 4661)
		template struct CALUMIANIMATION_API Utilities::VectorContainer<AnimationBlock>;
		template struct CALUMIANIMATION_API Utilities::VectorContainer<Animation>;
		template struct CALUMIANIMATION_API Utilities::VectorContainer<Preamble>;
#pragma warning(default: 4661)
	}
}

