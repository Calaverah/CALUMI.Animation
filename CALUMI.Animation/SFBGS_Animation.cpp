//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "SFBGS_Animation.h"
#include <iostream>
#include <print>
#include "FileValidation.h"
#include "CALUMI_Utilities.h"


namespace CALUMI{
	namespace SFBGS {

		
#pragma region HEADERFLAGS
		struct HeaderFlags::Impl 
		{
			bool firstEntry : 1 = false;  //UNKNOWN: 
			bool shortKeyCounters : 1 = false; //Flags if the animation block counters should be read/written as shorts (2 bytes)
			bool shortKeyFrameEntries : 1 = false; //Flags if animation block keyframe entries are extended from a byte to a short (2 bytes)
			bool scalarSequenceFlag : 1 = false;  //Must be marked true if scalar is present. If false, file will be read incorrectly if scalars are present

			bool u1 : 1 = false;  //UNKNOWN: Appears Blank
			bool u2 : 1 = false;  //UNKNOWN: Appears Blank
			bool u3 : 1 = false;  //UNKNOWN: Appears Blank
			bool u4 : 1 = false;  //UNKNOWN: Appears Blank

			char c1 = 0x0;  //UNKNOWN: Appears Blank
			char c2 = 0x0;  //UNKNOWN: Appears Blank
			char c3 = 0x0;  //UNKNOWN: Appears Blank

			Impl() = default;
			Impl(char c0, char c1, char c2, char c3) {
				firstEntry = c0 >> 0 & 0b1;
				shortKeyCounters = c0 >> 1 & 0b1;
				shortKeyFrameEntries = c0 >> 2 & 0b1;
				scalarSequenceFlag = c0 >> 3 & 0b1;
				u1 = c0 >> 4 & 0b1;
				u2 = c0 >> 5 & 0b1;
				u3 = c0 >> 6 & 0b1;
				u4 = c0 >> 7 & 0b1;

				this->c1 = c1;
				this->c2 = c2;
				this->c3 = c3;
			}
			char getData() const {	char output = 0;
								output |= (firstEntry << 0); 
								output |= (shortKeyCounters << 1); 
								output |= (shortKeyFrameEntries << 2); 
								output |= (scalarSequenceFlag << 3); 
								output |= (u1 << 4);
								output |= (u2 << 5);
								output |= (u3 << 6);
								output |= (u4 << 7);
								return output;
			}
			//firstEntry = input[0] >> 0 & 0b1;
			//shortKeyCounters = input[0] >> 1 & 0b1;
			//shortKeyFrameEntries = input[0] >> 2 & 0b1;
			//scalarSequenceFlag = input[0] >> 3 & 0b1;
			//u1 = input[0] >> 4 & 0b1;
			//u2 = input[0] >> 5 & 0b1;
			//u3 = input[0] >> 6 & 0b1;
			//u4 = input[0] >> 7 & 0b1;
			//
			//this->c1 = input[1];
			//this->c2 = input[2];
			//this->c3 = input[3];
		};

		HeaderFlags::HeaderFlags()
		{
			pImpl = new Impl;
		}

		HeaderFlags::~HeaderFlags()
		{
			if(pImpl)
				delete pImpl;
		}

		HeaderFlags::HeaderFlags(char c0, char c1, char c2, char c3)
		{
			pImpl = new Impl(c0, c1, c2, c3);
		}

		HeaderFlags::HeaderFlags(char input[4])
		{
			pImpl = new Impl(input[0], input[1], input[2], input[3]);
		}

		HeaderFlags::HeaderFlags(const HeaderFlags& input)
		{
			pImpl = new Impl(input.getData(), input.getC1(), input.getC2(), input.getC3());
		}

		HeaderFlags& HeaderFlags::operator=(const HeaderFlags& input)
		{
			if (pImpl) { delete pImpl; pImpl = nullptr; }
			pImpl = new Impl(input.getData(), input.getC1(), input.getC2(), input.getC3());
			return *this;
		}

		bool HeaderFlags::getFirstEntry() const
		{
			return pImpl->firstEntry;
		}

		void HeaderFlags::setFirstEntry(bool input)
		{
			pImpl->firstEntry = input;
		}

		bool HeaderFlags::areKeyCounters2Byte() const
		{
			return pImpl->shortKeyCounters;
		}

		void HeaderFlags::setKeyCountersFlag(bool input)
		{
			pImpl->shortKeyCounters = input;
		}

		bool HeaderFlags::areKeyFrameEntries2Byte() const
		{
			return pImpl->shortKeyFrameEntries;
		}

		void HeaderFlags::setKeyFrameEntriesFlag(bool input)
		{
			pImpl->shortKeyFrameEntries = input;
		}

		bool HeaderFlags::hasScalarSequence() const
		{
			return pImpl->scalarSequenceFlag;
		}

		void HeaderFlags::setScalarSequenceFlag(bool input)
		{
			pImpl->scalarSequenceFlag = input;
		}

		bool HeaderFlags::getU1() const
		{
			return pImpl->u1;
		}

		bool HeaderFlags::getU2() const
		{
			return pImpl->u2;
		}

		bool HeaderFlags::getU3() const
		{
			return pImpl->u3;
		}

		bool HeaderFlags::getU4() const
		{
			return pImpl->u4;
		}

		void HeaderFlags::setU1(bool input)
		{
			pImpl->u1 = input;
		}

		void HeaderFlags::setU2(bool input)
		{
			pImpl->u2 = input;
		}

		void HeaderFlags::setU3(bool input)
		{
			pImpl->u3 = input;
		}

		void HeaderFlags::setU4(bool input)
		{
			pImpl->u4 = input;
		}

		char HeaderFlags::getData() const
		{
			return pImpl->getData();
		}

		char HeaderFlags::getC1() const
		{
			return pImpl->c1;
		}

		char HeaderFlags::getC2() const
		{
			return pImpl->c2;
		}

		char HeaderFlags::getC3() const
		{
			return pImpl->c3;
		}

		void HeaderFlags::setC1(char input)
		{
			pImpl->c1 = input;
		}

		void HeaderFlags::setC2(char input)
		{
			pImpl->c2 = input;
		}

		void HeaderFlags::setC3(char input)
		{
			pImpl->c3 = input;
		}

#pragma endregion

#pragma region PREAMBLE

		struct Preamble::Impl 
		{
			uint16_t count = 0;
			Utilities::VectorContainer<float> preSet;
			Utilities::VectorContainer<float> mainSet;
			Utilities::VectorContainer<int16_t> footer1;
			Utilities::VectorContainer<int8_t> footer2;

			Impl() = default;
		};

		Preamble::~Preamble()
		{
			if (pImpl)
				delete pImpl;
		}
		uint16_t Preamble::getCount() const
		{
			return pImpl->count;
		}
		void Preamble::setCount(uint16_t sz)
		{
			pImpl->count = sz;
		}
		Utilities::VectorContainer<float>& Preamble::getPreSet() const
		{
			return pImpl->preSet;
		}
		void Preamble::setPreSet(const Utilities::VectorContainer<float>& input)
		{
			pImpl->preSet = input;
		}
		Utilities::VectorContainer<float>& Preamble::getMainSet() const
		{
			return pImpl->mainSet;
		}
		void Preamble::setMainSet(const Utilities::VectorContainer<float>& input)
		{
			pImpl->mainSet = input;
		}
		Utilities::VectorContainer<int16_t>& Preamble::getFooter1() const
		{
			return pImpl->footer1;
		}
		void Preamble::setFooter1(const Utilities::VectorContainer<int16_t>& input)
		{
			pImpl->footer1 = input;
		}
		Utilities::VectorContainer<int8_t>& Preamble::getFooter2() const
		{
			return pImpl->footer2;
		}
		void Preamble::setFooter2(const Utilities::VectorContainer<int8_t>& input)
		{
			pImpl->footer2 = input;
		}
		Preamble::Preamble()
		{
			pImpl = new Impl;
		}

		//Preamble Defs
		Preamble::Preamble(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex, size_t frameCount) : Preamble()
		{
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 2, 2, &pImpl->count);

			pImpl->preSet.resize(static_cast<size_t>(pImpl->count) * 2);
			for (int i = 0; i < (static_cast<size_t>(pImpl->count) * 2); i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 4, 4, &pImpl->preSet.at(i));
			}
			pImpl->mainSet.resize(frameCount);
			for (int i = 0; i < frameCount; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 4, 4, &pImpl->mainSet.at(i));
			}
			//int nCount = count == 1 ? 2 : count;
			pImpl->footer1.resize(pImpl->count);
			for (int i = 0; i < pImpl->count; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 2, 2, &pImpl->footer1.at(i));
			}

			pImpl->footer2.resize(pImpl->count);
			for (int i = 0; i < pImpl->count; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 1, &pImpl->footer2.at(i));
			}

		}

		Preamble& Preamble::operator=(const Preamble& other)
		{
			pImpl->count = other.pImpl->count;
			pImpl->preSet = other.pImpl->preSet;
			pImpl->mainSet = other.pImpl->mainSet;
			pImpl->footer1 = other.pImpl->footer1;
			pImpl->footer2 = other.pImpl->footer2;
			return *this;
		}

		Preamble::Preamble(const Preamble& other)
		{
			pImpl = new Impl;
			pImpl->count = other.pImpl->count;
			pImpl->preSet = other.pImpl->preSet;
			pImpl->mainSet = other.pImpl->mainSet;
			pImpl->footer1 = other.pImpl->footer1;
			pImpl->footer2 = other.pImpl->footer2;
		}
#pragma endregion

#pragma region ANIMATIONBLOCK

		struct AnimationBlock::Impl
		{
			uint16_t _rotationCount = 0;
			uint16_t _rotationPrefixCount = 0; //rotation prefix contains the first half of the rotations in a total of 4 bytes, x,y,z, and a count/identifier such that the prefix can be reused on multiple keyframes to save space

			uint16_t _translationCount = 0;
			uint16_t _translationPrefixCount = 0; //same as rotation prefix, however this entry is always 2 bytes each with the final 2 bytes (a ushort) being the count


			uint16_t _scalarCount = 0;
			uint16_t _bonePriorityCount = 0; //Never appears to have a count of more than 1, if at all

			Utilities::VectorContainer<uint16_t> rotationKeyFrames;
			Utilities::VectorContainer<uint16_t> translationKeyFrames;
			Utilities::VectorContainer<uint16_t> scalarKeyFrames;
			Utilities::VectorContainer<uint16_t> bonePriorityKeyFrames; //Usually just a single entry for the 0th frame


			//ROTATIONS SECTION
			Utilities::VectorContainer<CALUMI::SFBGS::RotationEntry> rotationEntries;
			Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix> rotationPrefixEntries;

			//TRANSLATIONS SECTION
			Utilities::VectorContainer<CALUMI::SFBGS::TranslationEntry> translationEntries;
			Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> translationPrefixEntries;

			//ADDITIONALS SECTION
			Utilities::VectorContainer<int16_t> scalarEntries;
			Utilities::VectorContainer<uint8_t> bonePriorityEntries; //Unsure how to process this information as of now. Appears to be in units/100 for a scalar of 0-100%
			Impl() = default;
		};
		uint16_t AnimationBlock::getRotationCount() const
		{
			return pImpl->_rotationCount;
		}
		uint16_t AnimationBlock::getRotationPrefixCount() const
		{
			return pImpl->_rotationPrefixCount;
		}
		uint16_t AnimationBlock::getTranslationCount() const
		{
			return pImpl->_translationCount;
		}
		uint16_t AnimationBlock::getTranslationPrefixCount() const
		{
			return pImpl->_translationPrefixCount;
		}
		uint16_t AnimationBlock::getScalarCount() const
		{
			return pImpl->_scalarCount;
		}
		uint16_t AnimationBlock::getPriorityCount() const
		{
			return pImpl->_bonePriorityCount;
		}
		void AnimationBlock::setRotationCount(uint16_t input)
		{
			pImpl->_rotationCount = input;
		}
		void AnimationBlock::setRotationPrefixCount(uint16_t input)
		{
			pImpl->_rotationPrefixCount = input;
		}
		void AnimationBlock::setTranslationCount(uint16_t input)
		{
			pImpl->_translationCount = input;
		}
		void AnimationBlock::setTranslationPrefixCount(uint16_t input)
		{
			pImpl->_translationPrefixCount = input;
		}
		void AnimationBlock::setScalarCount(uint16_t input)
		{
			pImpl->_scalarCount = input;
		}
		void AnimationBlock::setPriorityCount(uint16_t input)
		{
			pImpl->_bonePriorityCount = input;
		}
		Utilities::VectorContainer<uint16_t>& AnimationBlock::RotationKeyFrames() const
		{
			return pImpl->rotationKeyFrames;
		}
		Utilities::VectorContainer<uint16_t>& AnimationBlock::TranslationKeyFrames() const
		{
			return pImpl->translationKeyFrames;
		}
		Utilities::VectorContainer<uint16_t>& AnimationBlock::ScalarKeyFrames() const
		{
			return pImpl->scalarKeyFrames;
		}
		Utilities::VectorContainer<uint16_t>& AnimationBlock::BonePriorityKeyFrames() const
		{
			return pImpl->bonePriorityKeyFrames;
		}
		Utilities::VectorContainer<CALUMI::SFBGS::RotationEntry>& AnimationBlock::RotationEntries() const
		{
			return pImpl->rotationEntries;
		}
		Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix>& AnimationBlock::RotationPrefixEntries() const
		{
			return pImpl->rotationPrefixEntries;
		}
		Utilities::VectorContainer<CALUMI::SFBGS::TranslationEntry>& AnimationBlock::TranslationEntries() const
		{
			return pImpl->translationEntries;
		}
		Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix>& AnimationBlock::TranslationPrefixEntries() const
		{
			return pImpl->translationPrefixEntries;
		}
		Utilities::VectorContainer<int16_t>& AnimationBlock::ScalarEntries() const
		{
			return pImpl->scalarEntries;
		}
		Utilities::VectorContainer<uint8_t>& AnimationBlock::BonePriorityEntries() const
		{
			return pImpl->bonePriorityEntries;
		}
		AnimationBlock::AnimationBlock()
		{
			pImpl = new Impl;
		}
		AnimationBlock::AnimationBlock(const AnimationBlock& input) : AnimationBlock()
		{
			*pImpl = *(input.pImpl);
		}
		AnimationBlock& AnimationBlock::operator=(const AnimationBlock& input)
		{
			*pImpl = *(input.pImpl);
			return *this;
		}
		AnimationBlock::~AnimationBlock()
		{
			if (pImpl)
				delete pImpl;
		}

		AnimationBlock::AnimationBlock(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex, const HeaderFlags& flags) : AnimationBlock()
		{
			//address index can help with alignment when working with a raw buffer of bytes
			//eg. if the current index is at 0x07 and we have an aligned float
			//then we go forward (alignmentSize)-(index%(alignmentSize)) for the appropriate address
			// or in this case (4-7%4) = 1 and now our float can begin on address 0x08 as it should
			//We must make sure to advance the address index along with the currentAddress pointer
			uint8_t cSize = 1 + (int)flags.areKeyCounters2Byte();
			uint8_t kSize = 1 + (int)flags.areKeyFrameEntries2Byte();

			//fill the initial 6 counter values
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_rotationCount);
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_rotationPrefixCount);
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_translationCount);
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_translationPrefixCount);
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_scalarCount);
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_bonePriorityCount);

			//Fill Keyframe arrays
			{
				pImpl->rotationKeyFrames.resize(pImpl->_rotationCount);
				//Utilities::fill<unsigned short>(rotationKeyFrames,0); //resize should be all zero by default, but just in case
				for (unsigned short i = 0; i < pImpl->_rotationCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &pImpl->rotationKeyFrames.at(i));
				}


				pImpl->translationKeyFrames.resize(pImpl->_translationCount);
				//Utilities::fill<unsigned short>(translationKeyFrames,0); //""
				for (unsigned short i = 0; i < pImpl->_translationCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &pImpl->translationKeyFrames.at(i));
				}

				pImpl->scalarKeyFrames.resize(pImpl->_scalarCount);
				//Utilities::fill<unsigned short>(scalarKeyFrames,0); //""
				for (unsigned short i = 0; i < pImpl->_scalarCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &pImpl->scalarKeyFrames.at(i));
				}

				pImpl->bonePriorityKeyFrames.resize(pImpl->_bonePriorityCount);
				//Utilities::fill<unsigned short>(bonePriorityKeyFrames,0); //""
				for (unsigned short i = 0; i < pImpl->_bonePriorityCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &pImpl->bonePriorityKeyFrames.at(i));
				}
			}

			//Fill Keyframe Entries and Prefix Entries
			{
				pImpl->rotationEntries.resize(pImpl->_rotationCount); //Resize to fill with default entries (empty)
				{	unsigned char rBuffer[3] = {};
					for (unsigned short i = 0; i < pImpl->_rotationCount; i++)
					{
						CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 3, rBuffer);
						pImpl->rotationEntries.at(i) = rBuffer;
					}}
				
				pImpl->rotationPrefixEntries.resize(pImpl->_rotationPrefixCount);
				{	unsigned char rBuffer[4] = {};
					for (unsigned short i = 0; i < pImpl->_rotationPrefixCount; i++)
					{
						CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 4, rBuffer);
						pImpl->rotationPrefixEntries.at(i) = rBuffer;
					}}


				pImpl->translationEntries.resize(pImpl->_translationCount);
				{	unsigned char tBuffer[3] = {};
					for (unsigned short i = 0; i < pImpl->_translationCount; i++)
					{
						CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 3, tBuffer);
						pImpl->translationEntries.at(i) = tBuffer;
					}}

				pImpl->translationPrefixEntries.resize(pImpl->_translationPrefixCount);
				{	unsigned char tBuffer[8] = {};
					for (unsigned short i = 0; i < pImpl->_translationPrefixCount; i++)
					{
						CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 2, 8, tBuffer);
						pImpl->translationPrefixEntries.at(i) = tBuffer;
					}}

				pImpl->scalarEntries.resize(pImpl->_scalarCount);
				//Utilities::fill<short>(scalarEntries,0);
				for (unsigned short i = 0; i < pImpl->_scalarCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 2, 2, &pImpl->scalarEntries.at(i));
				}

				pImpl->bonePriorityEntries.resize(pImpl->_bonePriorityCount);
				//Utilities::fill<uint8_t>(bonePriorityEntries, 0);
				for (unsigned short i = 0; i < pImpl->_bonePriorityCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 1, &pImpl->bonePriorityEntries.at(i));
				}
			}

			//END OF ANIMATION BLOCK

		}

		void AnimationBlock::SerializeIntoBuffer(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex, const HeaderFlags& flags)
		{
			uint8_t cSize = 1 + (int)flags.areKeyCounters2Byte();
			uint8_t kSize = 1 + (int)flags.areKeyFrameEntries2Byte(); //alignment and sizing helpers

			//Counts
			{
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &pImpl->_rotationCount);
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &pImpl->_rotationPrefixCount);
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &pImpl->_translationCount);
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &pImpl->_translationPrefixCount);
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &pImpl->_scalarCount);
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &pImpl->_bonePriorityCount);
			}

			//KeyFrame arrays
			{
				for (unsigned short i = 0; i < pImpl->_rotationCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &pImpl->rotationKeyFrames.at(i));
				}

				for (unsigned short i = 0; i < pImpl->_translationCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &pImpl->translationKeyFrames.at(i));
				}

				for (unsigned short i = 0; i < pImpl->_scalarCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &pImpl->scalarKeyFrames.at(i));
				}

				for (unsigned short i = 0; i < pImpl->_bonePriorityCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &pImpl->bonePriorityKeyFrames.at(i));
				}
			}

			//Keyframe Entries and Prefix Entries
			{
				
				for (unsigned short i = 0; i < pImpl->_rotationCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 3, pImpl->rotationEntries.at(i).getRawData());
				}

				for (unsigned short i = 0; i < pImpl->_rotationPrefixCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 4, pImpl->rotationPrefixEntries.at(i).getRawData());
				}

				
				for (unsigned short i = 0; i < pImpl->_translationCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 3, pImpl->translationEntries.at(i).getRawData());
				}

				for (unsigned short i = 0; i < pImpl->_translationPrefixCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 2, 8, pImpl->translationPrefixEntries.at(i).getRawData());
				}

				for (unsigned short i = 0; i < pImpl->_scalarCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 2, 2, &pImpl->scalarEntries.at(i));
				}

				for (unsigned short i = 0; i < pImpl->_bonePriorityCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 1, &pImpl->bonePriorityEntries.at(i));
				}
			}

			//END OF ANIMATION BLOCK
		}

#pragma endregion

#pragma region ANIMATION

		struct Animation::Impl
		{
			CALUMI::Utilities::StringContainer animationFileName; //For file tracking. Should be unique without extension
			size_t fileSize = 0; //For debugging
			// 
			//9x4bytes, 2 blanks (CONFIRMED), 4 Quat Components (or all zero), 3 unknown (possibly xyz values)
			uint64_t _magicNumber = 0;
			Math::Quaternion _headerRotation;
			Math::Vector3 _headerTranslation;
			HeaderFlags _headerFlags; //1 byte and 3 empty bytes
			short _versionNumber = 0x05; //Always 05

			uint16_t _boneCount = 0;
			uint16_t _frameCount = 0;

			uint16_t _unknownFillCount = 0;

			uint16_t _indexAtlasCounter = 0x02; //At least 2, unless bones are zero (special case where addition bones are filled in at the bottom but they may not be from the main rig)

			uint16_t _preambleOffset = 0; //NOTE: this tells us how long the unknown "Preamble" section is and when the regular animation data begins

			float _nZeroFloats[3] = { 0.0,-0.0,0.0 }; //UNKNOWN:

			Utilities::VectorContainer<float> _unknownSuffixFillFloats;

			uint32_t preambleCount = 0;
			Utilities::VectorContainer<Preamble> preamble;

			Utilities::VectorContainer<float> _suffixFillerValues;

			//animation index atlas goes here
			Utilities::VectorContainer<uint16_t> _indexAtlas; //NOTE: this index is the size of the indexAtlasCounter, it is unknown if the entries are one byte only or if they can be expanded to two bytes, 
			//so we will keep our entries as a short (2 bytes) and cast them if they can be casted as 1 byte

			//animation blocks go here
			Utilities::VectorContainer<AnimationBlock> animationBlocks;
			Utilities::VectorContainer<AnimationBlock> animationSuffixBlocks;

			Impl() = default;
		};

		Animation::~Animation()
		{
			if (pImpl)
				delete pImpl;
		}
		Animation& Animation::operator=(const Animation& input)
		{
			*pImpl = *(input.pImpl);
			return *this;
		}
		Animation::Animation()
		{
			pImpl = new Impl;
		}

		Animation::Animation(const Animation& input)
		{
			pImpl = new Impl;
			*pImpl = *(input.pImpl);
		}

		size_t Animation::getSourceFileSize() const
		{
			return pImpl->fileSize;
		}

		CALUMI::Utilities::StringContainer& Animation::getAnimationFileName() const
		{
			return pImpl->animationFileName;
		}

		void Animation::setAnimationFileName(const CALUMI::Utilities::StringContainer& input)
		{
			pImpl->animationFileName = input;
		}

		uint64_t Animation::getMagicNumber() const
		{
			return pImpl->_magicNumber;
		}

		void Animation::setMagicNumber(uint64_t input)
		{
			pImpl->_magicNumber = input;
		}

		Math::Quaternion& Animation::getHeaderRotation() const
		{
			return pImpl->_headerRotation;
		}

		void Animation::setHeaderRotation(const Math::Quaternion& input)
		{
			pImpl->_headerRotation = input;
		}

		Math::Vector3& Animation::getHeaderTranslation() const
		{
			return pImpl->_headerTranslation;
		}

		void Animation::setHeaderTranslation(const Math::Vector3& input)
		{
			pImpl->_headerTranslation = input;
		}

		HeaderFlags& Animation::getHeaderFlags() const
		{
			return pImpl->_headerFlags;
		}

		void Animation::setHeaderFlags(const HeaderFlags& input)
		{
			pImpl->_headerFlags = input;
		}

		short Animation::getVersionNumber() const
		{
			return pImpl->_versionNumber;
		}

		void Animation::setVersionNumber(short v)
		{
			pImpl->_versionNumber = v;
		}

		uint16_t Animation::getBoneCount() const
		{
			return pImpl->_boneCount;
		}

		void Animation::setBoneCount(uint16_t input)
		{
			pImpl->_boneCount = input;
		}

		uint16_t Animation::getFrameCount() const
		{
			return pImpl->_frameCount;
		}

		void Animation::setFrameCount(uint16_t input)
		{
			pImpl->_frameCount = input;
		}

		uint16_t Animation::getIndexAtlasCount() const
		{
			return pImpl->_indexAtlasCounter;
		}

		void Animation::setIndexAtlasCount(uint16_t input)
		{
			pImpl->_indexAtlasCounter = input;
		}

		uint16_t Animation::getUnknownFillCount() const
		{
			return pImpl->_unknownFillCount;
		}

		void Animation::setUnknownFillCount(uint16_t input)
		{
			pImpl->_unknownFillCount = input;
		}

		uint16_t Animation::getPreambleOffset() const
		{
			return pImpl->_preambleOffset;
		}

		void Animation::setPreambleOffset(uint16_t input)
		{
			pImpl->_preambleOffset = input;
		}

		CALUMI::Utilities::VectorContainer<float> Animation::getNZeroFloats() const
		{
			Utilities::VectorContainer<float> output(3);
			for (size_t i = 0; i < 3; i++) { output.at(i) = pImpl->_nZeroFloats[i]; }
			return output;
		}

		void Animation::setNZeroFloats(float input[3])
		{
			for (size_t i = 0; i < 3; i++) { pImpl->_nZeroFloats[i] = input[i]; }
		}

		Utilities::VectorContainer<float>& Animation::getUnknownSuffixFillFloats() const
		{
			return pImpl->_unknownSuffixFillFloats;
		}

		void Animation::setUnknownSuffixFillFloats(const Utilities::VectorContainer<float>& input)
		{
			pImpl->_unknownSuffixFillFloats = input;
		}

		uint32_t Animation::getPreambleCount() const
		{
			return pImpl->preambleCount;
		}

		void Animation::setPreambleCount(uint32_t input)
		{
			pImpl->preambleCount = input;
		}

		Utilities::VectorContainer<Preamble>& Animation::getPreamble() const
		{
			return pImpl->preamble;
		}

		void Animation::setPreamble(const Utilities::VectorContainer<Preamble>& input)
		{
			pImpl->preamble = input;
		}

		Utilities::VectorContainer<float>& Animation::getSuffixFillerValues() const
		{
			return pImpl->_suffixFillerValues;
		}

		void Animation::getSuffixFillerValues(const Utilities::VectorContainer<float>& input)
		{
			pImpl->_suffixFillerValues = input;
		}

		Utilities::VectorContainer<uint16_t>& Animation::getIndexAtlas() const
		{
			return pImpl->_indexAtlas;
		}

		void Animation::setIndexAtlast(const Utilities::VectorContainer<uint16_t>& input)
		{
			pImpl->_indexAtlas = input;
		}

		Utilities::VectorContainer<AnimationBlock>& Animation::getAnimationBlocks() const
		{
			return pImpl->animationBlocks;
		}

		Utilities::VectorContainer<AnimationBlock>& Animation::getAnimationSuffixBlocks() const
		{
			return pImpl->animationSuffixBlocks;
		}

		void Animation::setAnimationBlocks(const Utilities::VectorContainer<AnimationBlock>& input)
		{
			pImpl->animationBlocks = input;
		}

		void Animation::setAnimationSuffixBlocks(const Utilities::VectorContainer<AnimationBlock>& input)
		{
			pImpl->animationSuffixBlocks = input;
		}

		unsigned short Animation::_SumIndices(Utilities::VectorContainer<unsigned short>inputVector, IndexCountingSolution type)
		{
			unsigned short sum = 0;
			for (int i = 0; i < inputVector.size(); i++)
			{
				if (i % 2 != static_cast<int8_t>(type)) //we inverse the logic so that (i=0)%2 != 1(even) or 2(all), (i=1)%2 != 0(odd) or 2(all) 
					sum += inputVector.at(i);
			}
			return sum;
		}

		void Animation::_evaluateHeaderFlags()
		{
			HeaderFlags newFlags;

			if (pImpl->_frameCount > 0xFF) newFlags.setKeyFrameEntriesFlag(true);

			for (int i = 0; i < pImpl->animationBlocks.size(); i++)
			{
				if (pImpl->animationBlocks.at(i).getRotationCount() > 0xFF || pImpl->animationBlocks.at(i).getTranslationCount() > 0xFF || pImpl->animationBlocks.at(i).getScalarCount() > 0xFF || pImpl->animationBlocks.at(i).getPriorityCount() > 0xFF)
					newFlags.setKeyCountersFlag(true);


				if (pImpl->animationBlocks.at(i).getScalarCount() > 0)
					newFlags.setScalarSequenceFlag(true);

				//if (_headerFlags.scalarSequenceFlag && _headerFlags.shortKeyCounters) break; //we can exit loop early as both flags are set

			}
			pImpl->_headerFlags = newFlags;
		}

		Utilities::ExpectedContainer<bool, FileError> Animation::ReadFromFile(const wchar_t* inputFilePath)
		{
			Utilities::PathContainer output(inputFilePath);
			return ReadFromFile(output);
		}
		Utilities::ExpectedContainer<bool, FileError> Animation::ReadFromFile(Utilities::PathContainer& inputFilePath)
		{
			//Check to see if file exists and is valid
			Utilities::VectorContainer<Utilities::StringContainer>vec; vec.push_back(".af");
			auto buffer = CALUMI::ValidateFile(inputFilePath, vec, 64, 0, true);
			if (!buffer.has_value())
			{
				Utilities::ExpectedContainer<bool, FileError> tempOutput;
				tempOutput.SetErrorValue(buffer.error());
				return tempOutput;
			}

			pImpl->fileSize = buffer.value().size();
			pImpl->animationFileName = inputFilePath.stem().c_str();
			//iterator tracking
			//char* currentAddress = buffer.value().data();
			unsigned long long addressIndex = 0;

			//Evaluate header
			{
				std::memcpy(&pImpl->_magicNumber, &buffer.value().at(addressIndex), sizeof(pImpl->_magicNumber));
				addressIndex += sizeof(pImpl->_magicNumber);

				{
					float qBuffer[4] = {}; float vBuffer[3] = {};
					std::memcpy(&qBuffer, &buffer.value().at(addressIndex), sizeof(qBuffer));
					addressIndex += sizeof(qBuffer);
					pImpl->_headerRotation = Math::Quaternion(qBuffer[1],qBuffer[2],qBuffer[3],qBuffer[0]);

					std::memcpy(&vBuffer, &buffer.value().at(addressIndex), sizeof(vBuffer));
					addressIndex += sizeof(vBuffer);
					pImpl->_headerTranslation = vBuffer;
				}

				char flagBuff[4] = {};
				std::memcpy(&flagBuff, &buffer.value().at(addressIndex), sizeof(flagBuff));
				pImpl->_headerFlags = HeaderFlags(flagBuff);
				addressIndex += sizeof(flagBuff);

				std::memcpy(&pImpl->_versionNumber, &buffer.value().at(addressIndex), sizeof(pImpl->_versionNumber));
				addressIndex += sizeof(pImpl->_versionNumber);

				std::memcpy(&pImpl->_boneCount, &buffer.value().at(addressIndex), sizeof(pImpl->_boneCount));
				addressIndex += sizeof(pImpl->_boneCount);

				std::memcpy(&pImpl->_frameCount, &buffer.value().at(addressIndex), sizeof(pImpl->_frameCount));
				addressIndex += sizeof(pImpl->_frameCount);

				std::memcpy(&pImpl->_indexAtlasCounter, &buffer.value().at(addressIndex), sizeof(pImpl->_indexAtlasCounter));
				addressIndex += sizeof(pImpl->_indexAtlasCounter);

				std::memcpy(&pImpl->_unknownFillCount, &buffer.value().at(addressIndex), sizeof(pImpl->_unknownFillCount));
				addressIndex += sizeof(pImpl->_unknownFillCount);

				std::memcpy(&pImpl->_preambleOffset, &buffer.value().at(addressIndex), sizeof(pImpl->_preambleOffset));
				addressIndex += sizeof(pImpl->_preambleOffset);

				std::memcpy(&pImpl->_nZeroFloats, &buffer.value().at(addressIndex), sizeof(pImpl->_nZeroFloats));
				addressIndex += sizeof(pImpl->_nZeroFloats);
			}
			//Validate header?

			

			//Evaluate Preamble
			size_t newOffset = addressIndex + pImpl->_preambleOffset;
			if (pImpl->_preambleOffset > 0)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer.value(), addressIndex, 4, 4, &pImpl->preambleCount);
				pImpl->preamble.reserve(pImpl->preambleCount);
				for (size_t i = 0; i < pImpl->preambleCount; i++)
				{

					pImpl->preamble.push_back(Preamble(buffer.value(), addressIndex, pImpl->_frameCount));

				}
			}

			//FILL PAD????
			//addressIndex += 4 * static_cast<unsigned long long>(_unknownFillCount);
			pImpl->_suffixFillerValues.resize(pImpl->_unknownFillCount);
			for (size_t i = 0; i < pImpl->_unknownFillCount; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer.value(), addressIndex, 4, 4, &pImpl->_suffixFillerValues.at(i));
			}

			//Aligning To 4 before hitting the Animation Block Evalutation
			CALUMI::Utilities::AlignBuffer(buffer.value(), addressIndex, 4);

			//Evaluate Animation Blocks
			//INDEX ATLAS
			{
				pImpl->_indexAtlas.resize(pImpl->_indexAtlasCounter);
				//Utilities::fill<unsigned short>(_indexAtlas,0); //resize should be all zero by default, but just in case
				int8_t atlasEntrySize = 1; //possible need to be able to switch between byte and short sizing here
				for (int i = 0; i < pImpl->_indexAtlasCounter; i++)
				{
					std::memcpy(&pImpl->_indexAtlas.at(i), &buffer.value().at(addressIndex), atlasEntrySize);
					addressIndex += atlasEntrySize;
				}
			}

			//Animation Blocks
			unsigned short j = 0; unsigned short k = 0;
			for (unsigned short i = 0; i < _SumIndices(pImpl->_indexAtlas, IndexCountingSolution::all); i++)
			{
				if (k >= pImpl->_indexAtlas.at(j))
				{
					j++; //move to next atlas section
					k = 0;
				}

				if (j % 2 == 0)
				{
					AnimationBlock toAdd;
					pImpl->animationBlocks.push_back(toAdd);
				}
				else
				{
					pImpl->animationBlocks.push_back(AnimationBlock(buffer.value(), addressIndex, pImpl->_headerFlags));
				}

				k++; //increment counter
				
				
			}


			pImpl->animationSuffixBlocks.reserve(pImpl->_unknownFillCount);
			for (unsigned short i = 0; i < pImpl->_unknownFillCount; i++)
			{
				pImpl->animationSuffixBlocks.push_back(AnimationBlock(buffer.value(), addressIndex, pImpl->_headerFlags));
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

		Utilities::ExpectedContainer<Utilities::StringContainer, FileError> Animation::WriteToFile(Utilities::PathContainer& outputFilePath)
		{
			//D:/ModOrganizer/Starfield_Mod_Authoring_01/mods/ExtractedData/meshes/actors/human/animations/scenes/mq101_001_miningscene/female/animstart_lin.af has the largest size of 780896 bytes


			Utilities::VectorContainer<char> buffer;
			buffer.reserve(781000); //temp optimization based on largest BGS file size

			//We write as little endian by default, can include a bswap in the future
			unsigned long long addressIndex = 0;


			//Header Section
			{
				buffer.insert(buffer.end(), 64, 0); //Prepares empty entries for the header. Header is always 64 bytes long as of file version 05
				std::memcpy(&buffer.at(addressIndex), &pImpl->_magicNumber, sizeof(pImpl->_magicNumber));
				addressIndex += sizeof(pImpl->_magicNumber);

				{
					float qBuffer[4] = {pImpl->_headerRotation.getW(),pImpl->_headerRotation.getX(),pImpl->_headerRotation.getY(),pImpl->_headerRotation.getZ()};
					float vBuffer[3] = { pImpl->_headerTranslation.getX(),pImpl->_headerTranslation.getY(),pImpl->_headerTranslation.getZ() };
					std::memcpy(&buffer.at(addressIndex), &qBuffer, sizeof(qBuffer));
					addressIndex += sizeof(qBuffer);
					std::memcpy(&buffer.at(addressIndex), &vBuffer, sizeof(vBuffer));
					addressIndex += sizeof(vBuffer);
				}
				
				char flagBuffer[4] = { pImpl->_headerFlags.getData(),pImpl->_headerFlags.getC1(),pImpl->_headerFlags.getC2(),pImpl->_headerFlags.getC3()};
				std::memcpy(&buffer.at(addressIndex),&flagBuffer, sizeof(flagBuffer));
				addressIndex += sizeof(flagBuffer);

				std::memcpy(&buffer.at(addressIndex), &pImpl->_versionNumber, sizeof(pImpl->_versionNumber));
				addressIndex += sizeof(pImpl->_versionNumber);

				std::memcpy( &buffer.at(addressIndex), &pImpl->_boneCount, sizeof(pImpl->_boneCount));
				addressIndex += sizeof(pImpl->_boneCount);

				std::memcpy( &buffer.at(addressIndex), &pImpl->_frameCount, sizeof(pImpl->_frameCount));
				addressIndex += sizeof(pImpl->_frameCount);

				std::memcpy( &buffer.at(addressIndex), &pImpl->_indexAtlasCounter, sizeof(pImpl->_indexAtlasCounter));
				addressIndex += sizeof(pImpl->_indexAtlasCounter);

				std::memcpy( &buffer.at(addressIndex), &pImpl->_unknownFillCount, sizeof(pImpl->_unknownFillCount));
				addressIndex += sizeof(pImpl->_unknownFillCount);

				std::memcpy( &buffer.at(addressIndex), &pImpl->_preambleOffset, sizeof(pImpl->_preambleOffset));
				addressIndex += sizeof(pImpl->_preambleOffset);

				std::memcpy( &buffer.at(addressIndex), &pImpl->_nZeroFloats, sizeof(pImpl->_nZeroFloats));
				addressIndex += sizeof(pImpl->_nZeroFloats);

			}

			//PREAMBLE AND FILLER FLOATS WOULD GO HERE. IGNORED FOR NOW

			//Index Atlas
			int8_t atlasEntrySize = 1; //possible need to be able to switch between byte and short sizing here
			buffer.insert(buffer.end(), static_cast<size_t>(atlasEntrySize * pImpl->_indexAtlasCounter), 0);
			for (int i = 0; i < pImpl->_indexAtlasCounter; i++)
			{
				std::memcpy( &buffer.at(addressIndex), &pImpl->_indexAtlas.at(i), atlasEntrySize);
				addressIndex += atlasEntrySize;
			}

			//Animation Blocks
			unsigned short animationBlockCount = _SumIndices(pImpl->_indexAtlas, IndexCountingSolution::odd);
			for (unsigned short i = 0; i < animationBlockCount; i++)
			{
				pImpl->animationBlocks.at(i).SerializeIntoBuffer(buffer,addressIndex, pImpl->_headerFlags);
			}

			//Skipping additional block entries until we know more



			if (pImpl->fileSize != addressIndex && pImpl->fileSize > 0)
			{
				std::println("===========================================================");
				std::println("===========================================================");
				std::println("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==");
				//std::println("CURRENT  POS: {}",(void*)currentAddress);
				//std::println("CURRENT ITER: {}",addressIndex);
				std::println("BUFFER  SIZE: {}", buffer.size());
				std::println("FILE  SIZE: {}", pImpl->fileSize);
				std::println("===========================================================");
				std::cout << "Press ENTER to continue running the program." << std::endl;
				std::cin.get();
				std::println("===========================================================");
			}

			//Writing to file
			

			return CALUMI::WriteToBinaryFile(outputFilePath, buffer);
		}

		Utilities::ExpectedContainer<Utilities::StringContainer, FileError> Animation::WriteToFile(const wchar_t* outputFilePath)
		{
			Utilities::PathContainer output(outputFilePath);
			return WriteToFile(output);
		}
#pragma endregion
}
}
