//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalplatform.h"
#include "sfbgs/animation/SFBGS_Animation.h"
#include "utilities/CALUMI_Utilities.h"
#include "io/FileValidation.h"
#include <iostream>
#include "internalvectordef.h"
#include <cstring>
#include <io/FileResult.h>
#include <vector>

namespace CALUMI::SFBGS {


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

		unsigned char c1 = 0x0;  //UNKNOWN: Appears Blank
		unsigned char c2 = 0x0;  //UNKNOWN: Appears Blank
		unsigned char c3 = 0x0;  //UNKNOWN: Appears Blank

		Impl() = default;
		Impl(const unsigned char c0, const unsigned char c1, const unsigned char c2, const unsigned char c3) {
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
		[[nodiscard]] unsigned char data() const
		{
			unsigned char output = 0;
			if (firstEntry)
				output |= 0b1 << 0;

			if (shortKeyCounters)
				output |= 0b1 << 1;

			if (shortKeyFrameEntries)
				output |= 0b1 << 2;

			if (scalarSequenceFlag)
				output |= 0b1 << 3;

			if (u1) output |= 0b1 << 4;
			if (u2) output |= 0b1 << 5;
			if (u3) output |= 0b1 << 6;
			if (u4) output |= 0b1 << 7;

			return output;
		}
	};

	HeaderFlags::HeaderFlags() : pImpl(new Impl()) {}

	HeaderFlags::~HeaderFlags()
	{
		if(pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}

	HeaderFlags::HeaderFlags(const char c0, const char c1, const char c2, const char c3) : pImpl(new Impl(c0, c1, c2, c3)) {}

	HeaderFlags::HeaderFlags(char input[4]) : pImpl(new Impl(input[0], input[1], input[2], input[3])) {}

	HeaderFlags::HeaderFlags(const HeaderFlags& input) : pImpl(new Impl(input.data(), input.c1(), input.c2(), input.c3())) {}

	HeaderFlags& HeaderFlags::operator=(const HeaderFlags& input)
	{
		if (this != &input)
		{
			if (pImpl)
			{
				delete pImpl;
				pImpl = nullptr;
			}
			pImpl = new Impl(input.data(), input.c1(), input.c2(), input.c3());
		}
		return *this;
	}

	bool HeaderFlags::firstEntry() const
	{
		return pImpl->firstEntry;
	}

	void HeaderFlags::setFirstEntry(const bool input) const
	{
		pImpl->firstEntry = input;
	}

	bool HeaderFlags::areKeyCounters2Byte() const
	{
		return pImpl->shortKeyCounters;
	}

	void HeaderFlags::setKeyCountersFlag(const bool input) const
	{
		pImpl->shortKeyCounters = input;
	}

	bool HeaderFlags::areKeyFrameEntries2Byte() const
	{
		return pImpl->shortKeyFrameEntries;
	}

	void HeaderFlags::setKeyFrameEntriesFlag(const bool input) const
	{
		pImpl->shortKeyFrameEntries = input;
	}

	bool HeaderFlags::hasScalarSequence() const
	{
		return pImpl->scalarSequenceFlag;
	}

	void HeaderFlags::setScalarSequenceFlag(const bool input) const
	{
		pImpl->scalarSequenceFlag = input;
	}

	bool HeaderFlags::u1() const
	{
		return pImpl->u1;
	}

	bool HeaderFlags::u2() const
	{
		return pImpl->u2;
	}

	bool HeaderFlags::u3() const
	{
		return pImpl->u3;
	}

	bool HeaderFlags::u4() const
	{
		return pImpl->u4;
	}

	void HeaderFlags::setU1(const bool input) const
	{
		pImpl->u1 = input;
	}

	void HeaderFlags::setU2(const bool input) const
	{
		pImpl->u2 = input;
	}

	void HeaderFlags::setU3(const bool input) const
	{
		pImpl->u3 = input;
	}

	void HeaderFlags::setU4(const bool input) const
	{
		pImpl->u4 = input;
	}

	unsigned char HeaderFlags::data() const
	{
		return pImpl->data();
	}

	unsigned char HeaderFlags::c1() const
	{
		return pImpl->c1;
	}

	unsigned char HeaderFlags::c2() const
	{
		return pImpl->c2;
	}

	unsigned char HeaderFlags::c3() const
	{
		return pImpl->c3;
	}

	void HeaderFlags::setC1(const unsigned char input) const
	{
		pImpl->c1 = input;
	}

	void HeaderFlags::setC2(const unsigned char input) const
	{
		pImpl->c2 = input;
	}

	void HeaderFlags::setC3(const unsigned char input) const
	{
		pImpl->c3 = input;
	}

#pragma endregion

#pragma region PREAMBLE

	struct Preamble::Impl
	{
		uint16_t count = 0;
		Utilities::FloatVector preSet;
		Utilities::FloatVector mainSet;
		Utilities::S16Vector footer1;
		Utilities::S8Vector footer2;

		Impl() = default;
	};

	Preamble::~Preamble()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	uint16_t Preamble::count() const
	{
		return pImpl->count;
	}
	void Preamble::setCount(const uint16_t sz) const
	{
		pImpl->count = sz;
	}
	Utilities::FloatVector& Preamble::preSet() const
	{
		return pImpl->preSet;
	}
	void Preamble::setPreSet(const Utilities::FloatVector& input) const
	{
		pImpl->preSet = input;
	}
	Utilities::FloatVector& Preamble::mainSet() const
	{
		return pImpl->mainSet;
	}
	void Preamble::setMainSet(const Utilities::FloatVector& input) const
	{
		pImpl->mainSet = input;
	}
	Utilities::S16Vector& Preamble::footer1() const
	{
		return pImpl->footer1;
	}
	void Preamble::setFooter1(const Utilities::S16Vector& input) const
	{
		pImpl->footer1 = input;
	}
	Utilities::S8Vector& Preamble::footer2() const
	{
		return pImpl->footer2;
	}
	void Preamble::setFooter2(const Utilities::S8Vector& input) const
	{
		pImpl->footer2 = input;
	}
	Preamble::Preamble()
	{
		pImpl = new Impl;
	}

	//Preamble Defs
	Preamble::Preamble(Utilities::BufferObject& buffer, unsigned long long& addressIndex, const uint64_t frameCount) : Preamble()
	{
		CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 2, 2, &pImpl->count);

		pImpl->preSet.resize(static_cast<size_t>(pImpl->count) * 2);
		for (int i = 0; i < static_cast<size_t>(pImpl->count) * 2; i++)
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
		if (this != &other)
		{
			pImpl->count = other.pImpl->count;
			pImpl->preSet = other.pImpl->preSet;
			pImpl->mainSet = other.pImpl->mainSet;
			pImpl->footer1 = other.pImpl->footer1;
			pImpl->footer2 = other.pImpl->footer2;
		}
		return *this;
	}

	Preamble::Preamble(const Preamble& other) : Preamble()
	{
		*this = other;
	}
#pragma endregion

	VECTORDEF(PreambleVector, Preamble)

#pragma region ANIMATIONBLOCK

	struct AnimationBlock::Impl
	{
		uint16_t _rotationCount = 0;
		uint16_t _rotationPrefixCount = 0; //rotation prefix contains the first half of the rotations in a total of 4 bytes, _x,_y,_z, and a count/identifier such that the prefix can be reused on multiple keyframes to save space

		uint16_t _translationCount = 0;
		uint16_t _translationPrefixCount = 0; //same as rotation prefix, however this entry is always 2 bytes each with the final 2 bytes (ushort) being the count


		uint16_t _scalarCount = 0;
		uint16_t _bonePriorityCount = 0; //Never appears to have a count of more than 1, if at all

		Utilities::U16Vector _rotationKeyFrames;
		Utilities::U16Vector _translationKeyFrames;
		Utilities::U16Vector _scalarKeyFrames;
		Utilities::U16Vector _bonePriorityKeyFrames; //Usually just a single entry for the 0th frame


		//ROTATIONS SECTION
		RotationEntrySequence _rotationEntries;
		RotationPrefixSequence _rotationPrefixEntries;

		//TRANSLATIONS SECTION
		TranslationEntrySequence _translationEntries;
		TranslationPrefixSequence _translationPrefixEntries;

		//ADDITIONAL SECTION
		Utilities::S16Vector _scalarEntries;
		Utilities::U8Vector _bonePriorityEntries; //Unsure how to process this information as of now. Appears to be in units/100 for a scalar of 0-100%

		Impl() = default;
	};
	uint16_t AnimationBlock::rotationCount() const
	{
		return pImpl->_rotationCount;
	}
	uint16_t AnimationBlock::rotationPrefixCount() const
	{
		return pImpl->_rotationPrefixCount;
	}
	uint16_t AnimationBlock::translationCount() const
	{
		return pImpl->_translationCount;
	}
	uint16_t AnimationBlock::translationPrefixCount() const
	{
		return pImpl->_translationPrefixCount;
	}
	uint16_t AnimationBlock::scalarCount() const
	{
		return pImpl->_scalarCount;
	}
	uint16_t AnimationBlock::priorityCount() const
	{
		return pImpl->_bonePriorityCount;
	}
	void AnimationBlock::setRotationCount(const uint16_t input) const
	{
		pImpl->_rotationCount = input;
	}
	void AnimationBlock::setRotationPrefixCount(const uint16_t input) const
	{
		pImpl->_rotationPrefixCount = input;
	}
	void AnimationBlock::setTranslationCount(const uint16_t input) const
	{
		pImpl->_translationCount = input;
	}
	void AnimationBlock::setTranslationPrefixCount(const uint16_t input) const
	{
		pImpl->_translationPrefixCount = input;
	}
	void AnimationBlock::setScalarCount(const uint16_t input) const
	{
		pImpl->_scalarCount = input;
	}
	void AnimationBlock::setPriorityCount(const uint16_t input) const
	{
		pImpl->_bonePriorityCount = input;
	}
	Utilities::U16Vector& AnimationBlock::rotationKeyFrames() const
	{
		return pImpl->_rotationKeyFrames;
	}
	Utilities::U16Vector& AnimationBlock::translationKeyFrames() const
	{
		return pImpl->_translationKeyFrames;
	}
	Utilities::U16Vector& AnimationBlock::scalarKeyFrames() const
	{
		return pImpl->_scalarKeyFrames;
	}
	Utilities::U16Vector& AnimationBlock::priorityKeyFrames() const
	{
		return pImpl->_bonePriorityKeyFrames;
	}
	RotationEntrySequence& AnimationBlock::rotationEntries() const
	{
		return pImpl->_rotationEntries;
	}
	RotationPrefixSequence& AnimationBlock::rotationPrefixEntries() const
	{
		return pImpl->_rotationPrefixEntries;
	}
	TranslationEntrySequence& AnimationBlock::translationEntries() const
	{
		return pImpl->_translationEntries;
	}
	TranslationPrefixSequence& AnimationBlock::translationPrefixEntries() const
	{
		return pImpl->_translationPrefixEntries;
	}
	Utilities::S16Vector& AnimationBlock::scalarEntries() const
	{
		return pImpl->_scalarEntries;
	}
	Utilities::U8Vector& AnimationBlock::priorityEntries() const
	{
		return pImpl->_bonePriorityEntries;
	}
	AnimationBlock::AnimationBlock() : pImpl(new Impl()) {}

	AnimationBlock::AnimationBlock(const AnimationBlock& input) : AnimationBlock()
	{
		*pImpl = *input.pImpl;
	}
	AnimationBlock& AnimationBlock::operator=(const AnimationBlock& input)
	{
		if (this != &input)
			*pImpl = *input.pImpl;

		return *this;
	}
	AnimationBlock::~AnimationBlock()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}

	AnimationBlock::AnimationBlock(Utilities::BufferObject& buffer, unsigned long long& addressIndex, const HeaderFlags& flags) : AnimationBlock()
	{
		//address index can help with alignment when working with a raw buffer of bytes
		//e.g. if the current index is at 0x07, and we have an aligned float
		//then we go forward (alignmentSize)-(index%(alignmentSize)) for the appropriate address
		// or in this case (4-7%4) = 1 and now our float can begin on address 0x08 as it should
		//We must make sure to advance the address index along with the currentAddress pointer
		const uint8_t cSize = 1 + static_cast<int>(flags.areKeyCounters2Byte());
		const uint8_t kSize = 1 + static_cast<int>(flags.areKeyFrameEntries2Byte());

		//fill the initial 6 counter values
		CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_rotationCount);
		CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_rotationPrefixCount);
		CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_translationCount);
		CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_translationPrefixCount);
		CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_scalarCount);
		CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &pImpl->_bonePriorityCount);

		//Fill Keyframe arrays
		{
			pImpl->_rotationKeyFrames.resize(pImpl->_rotationCount);
			//Utilities::fill<unsigned short>(_rotationKeyFrames,0); //resize should be all zero by default, but just in case
			for (unsigned short i = 0; i < pImpl->_rotationCount; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &pImpl->_rotationKeyFrames.at(i));
			}


			pImpl->_translationKeyFrames.resize(pImpl->_translationCount);
			//Utilities::fill<unsigned short>(_translationKeyFrames,0); //""
			for (unsigned short i = 0; i < pImpl->_translationCount; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &pImpl->_translationKeyFrames.at(i));
			}

			pImpl->_scalarKeyFrames.resize(pImpl->_scalarCount);
			//Utilities::fill<unsigned short>(_scalarKeyFrames,0); //""
			for (unsigned short i = 0; i < pImpl->_scalarCount; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &pImpl->_scalarKeyFrames.at(i));
			}

			pImpl->_bonePriorityKeyFrames.resize(pImpl->_bonePriorityCount);
			//Utilities::fill<unsigned short>(_bonePriorityKeyFrames,0); //""
			for (unsigned short i = 0; i < pImpl->_bonePriorityCount; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &pImpl->_bonePriorityKeyFrames.at(i));
			}
		}

		//Fill Keyframe Entries and Prefix Entries
		{
			pImpl->_rotationEntries.resize(pImpl->_rotationCount); //Resize to fill with default entries (empty)
			{	unsigned char rBuffer[3] = {};
				for (unsigned short i = 0; i < pImpl->_rotationCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 3, rBuffer);
					pImpl->_rotationEntries.at(i) = rBuffer;
				}}

			pImpl->_rotationPrefixEntries.resize(pImpl->_rotationPrefixCount);
			{	unsigned char rBuffer[4] = {};
				for (unsigned short i = 0; i < pImpl->_rotationPrefixCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 4, rBuffer);
					pImpl->_rotationPrefixEntries.at(i) = rBuffer;
				}}


			pImpl->_translationEntries.resize(pImpl->_translationCount);
			{	unsigned char tBuffer[3] = {};
				for (unsigned short i = 0; i < pImpl->_translationCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 3, tBuffer);
					pImpl->_translationEntries.at(i) = tBuffer;
				}}

			pImpl->_translationPrefixEntries.resize(pImpl->_translationPrefixCount);
			{	unsigned char tBuffer[8] = {};
				for (unsigned short i = 0; i < pImpl->_translationPrefixCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 2, 8, tBuffer);
					pImpl->_translationPrefixEntries.at(i) = tBuffer;
				}}

			pImpl->_scalarEntries.resize(pImpl->_scalarCount);
			//Utilities::fill<short>(_scalarEntries,0);
			for (unsigned short i = 0; i < pImpl->_scalarCount; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 2, 2, &pImpl->_scalarEntries.at(i));
			}

			pImpl->_bonePriorityEntries.resize(pImpl->_bonePriorityCount);
			//Utilities::fill<uint8_t>(_bonePriorityEntries, 0);
			for (unsigned short i = 0; i < pImpl->_bonePriorityCount; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 1, &pImpl->_bonePriorityEntries.at(i));
			}
		}

		//END OF ANIMATION BLOCK

	}

	void AnimationBlock::serializeIntoBuffer(Utilities::BufferObject& buffer, unsigned long long& addressIndex, const HeaderFlags& flags) const
	{
		const uint8_t cSize = 1 + static_cast<int>(flags.areKeyCounters2Byte());
		const uint8_t kSize = 1 + static_cast<int>(flags.areKeyFrameEntries2Byte()); //alignment and sizing helpers

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
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &pImpl->_rotationKeyFrames.at(i));
			}

			for (unsigned short i = 0; i < pImpl->_translationCount; i++)
			{
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &pImpl->_translationKeyFrames.at(i));
			}

			for (unsigned short i = 0; i < pImpl->_scalarCount; i++)
			{
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &pImpl->_scalarKeyFrames.at(i));
			}

			for (unsigned short i = 0; i < pImpl->_bonePriorityCount; i++)
			{
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &pImpl->_bonePriorityKeyFrames.at(i));
			}
		}

		//Keyframe Entries and Prefix Entries
		{

			for (unsigned short i = 0; i < pImpl->_rotationCount; i++)
			{
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 3, pImpl->_rotationEntries.at(i).data());
			}

			for (unsigned short i = 0; i < pImpl->_rotationPrefixCount; i++)
			{
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 4, pImpl->_rotationPrefixEntries.at(i).data());
			}


			for (unsigned short i = 0; i < pImpl->_translationCount; i++)
			{
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 3, pImpl->_translationEntries.at(i).data());
			}

			for (unsigned short i = 0; i < pImpl->_translationPrefixCount; i++)
			{
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 2, 8, pImpl->_translationPrefixEntries.at(i).data());
			}

			for (unsigned short i = 0; i < pImpl->_scalarCount; i++)
			{
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 2, 2, &pImpl->_scalarEntries.at(i));
			}

			for (unsigned short i = 0; i < pImpl->_bonePriorityCount; i++)
			{
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 1, &pImpl->_bonePriorityEntries.at(i));
			}
		}

		//END OF ANIMATION BLOCK
	}

	void AnimationBlock::setRotationSequenceFromUNIV(UNIV::RotationSequence& input) const
	{
		setRotationCount(static_cast<uint16_t>(input.size()));
		rotationKeyFrames().reserve(input.size());
		rotationEntries().reserve(input.size());
		rotationPrefixEntries().reserve(input.size());
		for (int i =0; i < input.size(); i++)
		{
			rotationKeyFrames().push_back(input.at(i).frame());
			auto result = CompressedRotation(input.at(i).rotationQuaternion());
			rotationPrefixEntries().push_back(result.prefix());
			rotationEntries().push_back(result.suffix());
		}

		rotationPrefixEntries ()= FoldRotationPrefixSequence(rotationPrefixEntries());
		setRotationPrefixCount(static_cast<uint16_t>(rotationPrefixEntries().size()));
	}

	UNIV::RotationSequence AnimationBlock::rotationSequenceAsUNIV() const
	{
		UNIV::RotationSequence output;
		output.reserve(rotationCount());
		RotationPrefixSequence unfoldedPrefixes = UnfoldRotationPrefixSequence(rotationPrefixEntries());
		for (unsigned int i = 0; i < rotationCount(); i++)
		{
			UNIV::RotationFrame toAdd(rotationKeyFrames().at(i), CompressedRotation(unfoldedPrefixes.at(i), rotationEntries().at(i)).toQuaternion());
			output.push_back(toAdd);
		}
		SortRotationSequence(output,false);
		return output;
	}

	void AnimationBlock::setTranslationSequenceFromUNIV(UNIV::TranslationSequence& input, const float& highPrecision, const float& lowPrecision) const
	{
		setTranslationCount(static_cast<uint16_t>(input.size()));
		translationKeyFrames().reserve(input.size());
		translationEntries().reserve(input.size());
		translationPrefixEntries().reserve(input.size());
		for (int i = 0; i < input.size(); i++)
		{
			translationKeyFrames().push_back(input.at(i).frame());
			auto result = CompressedTranslation(input.at(i).translationVector(), highPrecision, lowPrecision);
			translationPrefixEntries().push_back(result.prefix());
			translationEntries().push_back(result.suffix());
		}
		translationPrefixEntries ()= FoldTranslationPrefixSequence(translationPrefixEntries());
		setTranslationPrefixCount(static_cast<uint16_t>(translationPrefixEntries().size()));
	}

	UNIV::TranslationSequence AnimationBlock::translationSequenceAsUNIV(const float& highPrecision, const float& lowPrecision) const
	{
		UNIV::TranslationSequence output;
		output.reserve(translationCount());
		TranslationPrefixSequence unfoldedPrefixes = UnfoldTranslationPrefixSequence(translationPrefixEntries());
		for (unsigned int i = 0; i < translationCount(); i++)
		{
			UNIV::TranslationFrame toAdd(translationKeyFrames().at(i), CompressedTranslation(unfoldedPrefixes.at(i), translationEntries().at(i)).toVector3D(lowPrecision, highPrecision));
			output.push_back(toAdd);
		}
		SortTranslationSequence(output);
		return output;
	}

	void AnimationBlock::setScalarSequenceFromUNIV(UNIV::ScalarSequence& input) const
	{
		setScalarCount(static_cast<uint16_t>(input.size()));
		scalarEntries().reserve(scalarCount());
		scalarKeyFrames().reserve(scalarCount());
		for (int i = 0; i < input.size(); i++)
		{
			scalarKeyFrames().push_back(input.at(i).frame());
			scalarEntries().push_back(static_cast<int16_t>(input.at(i).scalarValue() * 5000));
		}
	}

	UNIV::ScalarSequence AnimationBlock::scalarSequenceAsUNIV() const
	{
		UNIV::ScalarSequence output;
		output.reserve(scalarCount());
		for (unsigned int i = 0; i < scalarCount(); i++)
		{
			UNIV::ScalarFrame toAdd(scalarKeyFrames().at(i), static_cast<float>(scalarEntries().at(i))/5000.0f);
			output.push_back(toAdd);
		}
		SortScalarSequence(output);
		return output;
	}

	void AnimationBlock::setPrioritySequenceFromUNIV(UNIV::PrioritySequence& input) const
	{
		setPriorityCount(static_cast<uint16_t>(input.size()));
		priorityEntries().resize(priorityCount());
		priorityKeyFrames().resize(priorityCount());
		for (unsigned int i = 0; i < priorityCount(); i++)
		{
			priorityKeyFrames().at(i) = input.at(i).frame();
			priorityEntries().at(i) = input.at(i).priorityValue();
		}
	}

	UNIV::PrioritySequence AnimationBlock::prioritySequenceAsUNIV() const
	{
		UNIV::PrioritySequence output;
		output.reserve(priorityCount());
		for (unsigned int i = 0; i < priorityCount(); i++)
		{
			UNIV::PriorityFrame toAdd(priorityKeyFrames().at(i), priorityEntries().at(i));
			output.push_back(toAdd);
		}
		SortPrioritySequence(output);
		return output;
	}

#pragma endregion

	VECTORDEF(AnimationBlockVector, AnimationBlock)

#pragma region ANIMATION

	struct Animation::Impl
	{
		Utilities::StringContainer animationFileName; //Not present in file. For file tracking. Should be unique without extension
		uint64_t fileSize = 0; //Not present in file. For debugging
		//
		//9x4bytes, 2 blanks (CONFIRMED), 4 Quat Components (or all zero), 3 unknown (possibly xyz values)
		uint64_t _magicNumber = 0;
		Math::Quaternion _headerRotation;
		Math::Vector3 _headerTranslation;
		HeaderFlags _headerFlags; //1 byte and 3 empty bytes
		short _versionNumber = 0x05; //Always 05

		uint16_t _boneCount = 0;
		uint16_t _frameCount = 0;

		uint16_t _amendedBlockCount = 0;

		uint16_t _indexAtlasCounter = 0x02; //At least 2, unless bones are zero (special case where addition bones are filled in at the bottom, but they may not be from the main rig)

		uint16_t _preambleOffset = 0; //NOTE: this tells us how long the unknown "Preamble" section is and when the regular animation data begins

		float _nZeroFloats[3] = { 0.0,-0.0,0.0 }; //UNKNOWN:

		uint32_t _preambleCount = 0;
		PreambleVector _preamble;

		Utilities::U32Vector _amendedHashSet;

		//animation index atlas goes here
		Utilities::U16Vector _indexAtlas; //NOTE: this index is the size of the indexAtlasCounter, it is unknown if the entries are one byte only or if they can be expanded to two bytes,
		//so we will keep our entries as a short (2 bytes) and cast them if they can be cast as 1 byte

		//animation blocks go here
		AnimationBlockVector _animationBlocks;
		AnimationBlockVector _amendedAnimationBlocks;

		Impl() = default;
	};

	Animation::~Animation()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	Animation& Animation::operator=(const Animation& input)
	{
		if (this != &input)
			*pImpl = *input.pImpl;

		return *this;
	}
	Animation::Animation() : pImpl(new Impl()) {}

	Animation::Animation(const Animation& input) : Animation()
	{
		*this = input;
	}

	uint64_t Animation::sourceFileSize() const
	{
		return pImpl->fileSize;
	}

	Utilities::StringContainer& Animation::animationFileName() const
	{
		return pImpl->animationFileName;
	}

	void Animation::setAnimationFileName(const Utilities::StringContainer& input) const
	{
		pImpl->animationFileName = input;
	}

	uint64_t Animation::magicNumber() const
	{
		return pImpl->_magicNumber;
	}

	void Animation::setMagicNumber(const uint64_t input) const
	{
		pImpl->_magicNumber = input;
	}

	Math::Quaternion& Animation::headerRotation() const
	{
		return pImpl->_headerRotation;
	}

	void Animation::setHeaderRotation(const Math::Quaternion& input) const
	{
		pImpl->_headerRotation = input;
	}

	Math::Vector3& Animation::headerTranslation() const
	{
		return pImpl->_headerTranslation;
	}

	void Animation::setHeaderTranslation(const Math::Vector3& input) const
	{
		pImpl->_headerTranslation = input;
	}

	HeaderFlags& Animation::headerFlags() const
	{
		return pImpl->_headerFlags;
	}

	void Animation::setHeaderFlags(const HeaderFlags& input) const
	{
		pImpl->_headerFlags = input;
	}

	short Animation::versionNumber() const
	{
		return pImpl->_versionNumber;
	}

	void Animation::setVersionNumber(const short v) const
	{
		pImpl->_versionNumber = v;
	}

	uint16_t Animation::boneCount() const
	{
		return pImpl->_boneCount;
	}

	void Animation::setBoneCount(const uint16_t input) const
	{
		pImpl->_boneCount = input;
	}

	uint16_t Animation::frameCount() const
	{
		return pImpl->_frameCount;
	}

	void Animation::setFrameCount(const uint16_t input) const
	{
		pImpl->_frameCount = input;
	}

	uint16_t Animation::indexAtlasCount() const
	{
		return pImpl->_indexAtlasCounter;
	}

	void Animation::setIndexAtlasCount(const uint16_t input) const
	{
		pImpl->_indexAtlasCounter = input;
	}

	uint16_t Animation::amendedBlockCount() const
	{
		return pImpl->_amendedBlockCount;
	}

	void Animation::setAmendedBlockCount(const uint16_t input) const
	{
		pImpl->_amendedBlockCount = input;
	}

	uint16_t Animation::preambleOffset() const
	{
		return pImpl->_preambleOffset;
	}

	void Animation::setPreambleOffset(const uint16_t input) const
	{
		pImpl->_preambleOffset = input;
	}

	Utilities::FloatVector Animation::nZeroFloats() const
	{
		Utilities::FloatVector output;

		output.resize(3);

		for (uint64_t i = 0; i < 3; i++)
		{
			output.at(i) = pImpl->_nZeroFloats[i];
		}
		return output;
	}

	void Animation::setNZeroFloats(const float input[3]) const
	{
		for (uint64_t i = 0; i < 3; i++) { pImpl->_nZeroFloats[i] = input[i]; }
	}

	uint32_t Animation::preambleCount() const
	{
		return pImpl->_preambleCount;
	}

	void Animation::setPreambleCount(const uint32_t input) const
	{
		pImpl->_preambleCount = input;
	}

	PreambleVector& Animation::preamble() const
	{
		return pImpl->_preamble;
	}

	void Animation::setPreamble(const PreambleVector& input) const
	{
		pImpl->_preamble = input;
	}

	Utilities::U32Vector& Animation::amendedHashSet() const
	{
		return pImpl->_amendedHashSet;
	}

	void Animation::setAmendedHashSet(const Utilities::U32Vector& input) const
	{
		pImpl->_amendedHashSet = input;
	}

	Utilities::U16Vector& Animation::indexAtlas() const
	{
		return pImpl->_indexAtlas;
	}

	void Animation::setIndexAtlast(const Utilities::U16Vector& input) const
	{
		pImpl->_indexAtlas = input;
	}

	AnimationBlockVector& Animation::animationBlocks() const
	{
		return pImpl->_animationBlocks;
	}

	AnimationBlockVector& Animation::amendedAnimationBlocks() const
	{
		return pImpl->_amendedAnimationBlocks;
	}

	void Animation::setAnimationBlocks(const AnimationBlockVector& input) const
	{
		pImpl->_animationBlocks = input;
	}

	void Animation::setAmendedAnimationBlocks(const AnimationBlockVector& input) const
	{
		pImpl->_amendedAnimationBlocks = input;
	}

	static unsigned short s_SumIndices(Utilities::U16Vector inputVector, Animation::IndexCountingSolution type)
	{
		unsigned short sum = 0;
		for (int i = 0; i < inputVector.size(); i++)
		{
			//we inverse the logic so that (i=0)%2 != 1(even) or 2(all), (i=1)%2 != 0(odd) or 2(all)
			if (i % 2 != static_cast<int8_t>(type))
				sum += inputVector.at(i);
		}
		return sum;
	}

	void Animation::evaluateHeaderFlags() const
	{
		const HeaderFlags newFlags;

		if (pImpl->_frameCount > 0xFF) newFlags.setKeyFrameEntriesFlag(true);

		for (int i = 0; i < pImpl->_animationBlocks.size(); i++)
		{
			if (pImpl->_animationBlocks.at(i).rotationCount() > 0xFF || pImpl->_animationBlocks.at(i).translationCount() > 0xFF || pImpl->_animationBlocks.at(i).scalarCount() > 0xFF || pImpl->_animationBlocks.at(i).priorityCount() > 0xFF)
				newFlags.setKeyCountersFlag(true);


			if (pImpl->_animationBlocks.at(i).scalarCount() > 0)
				newFlags.setScalarSequenceFlag(true);

			//if (_headerFlags.scalarSequenceFlag && _headerFlags.shortKeyCounters) break; //we can exit loop early as both flags are set

		}
		pImpl->_headerFlags = newFlags;
	}

	Utilities::FileResult Animation::readFromFile(Utilities::PathContainer&& inputFilePath)
	{
		Utilities::PathContainer output(inputFilePath);
		return readFromFile(output);
	}
	Utilities::FileResult Animation::readFromFile(Utilities::PathContainer& inputFilePath)
	{
		//Check to see if file exists and is valid
		const Utilities::StringList vec;
		vec.push_back(".af");

		auto buffer = ValidateFile(inputFilePath, vec, 64, 0, true);
		if (buffer.result().hasError())
		{
			return buffer.result();
		}

		pImpl->fileSize = buffer.size();
		pImpl->animationFileName = inputFilePath.stem().c_str();
		//iterator tracking
		//char* currentAddress = buffer.data();
		unsigned long long addressIndex = 0;

		//Evaluate header
		{
			std::memcpy(&pImpl->_magicNumber, &buffer.at(addressIndex), sizeof(pImpl->_magicNumber));
			addressIndex += sizeof(pImpl->_magicNumber);

			{
				float qBuffer[4] = {}; float vBuffer[3] = {};
				std::memcpy(&qBuffer, &buffer.at(addressIndex), sizeof(qBuffer));
				addressIndex += sizeof(qBuffer);
				pImpl->_headerRotation = Math::Quaternion(qBuffer[1], qBuffer[2], qBuffer[3], qBuffer[0]);

				std::memcpy(&vBuffer, &buffer.at(addressIndex), sizeof(vBuffer));
				addressIndex += sizeof(vBuffer);
				pImpl->_headerTranslation = Math::Vector3(vBuffer);
			}

			char flagBuff[4] = {};
			std::memcpy(&flagBuff, &buffer.at(addressIndex), sizeof(flagBuff));
			pImpl->_headerFlags = HeaderFlags(flagBuff);
			addressIndex += sizeof(flagBuff);

			std::memcpy(&pImpl->_versionNumber, &buffer.at(addressIndex), sizeof(pImpl->_versionNumber));
			addressIndex += sizeof(pImpl->_versionNumber);

			std::memcpy(&pImpl->_boneCount, &buffer.at(addressIndex), sizeof(pImpl->_boneCount));
			addressIndex += sizeof(pImpl->_boneCount);

			std::memcpy(&pImpl->_frameCount, &buffer.at(addressIndex), sizeof(pImpl->_frameCount));
			addressIndex += sizeof(pImpl->_frameCount);

			std::memcpy(&pImpl->_indexAtlasCounter, &buffer.at(addressIndex), sizeof(pImpl->_indexAtlasCounter));
			addressIndex += sizeof(pImpl->_indexAtlasCounter);

			std::memcpy(&pImpl->_amendedBlockCount, &buffer.at(addressIndex), sizeof(pImpl->_amendedBlockCount));
			addressIndex += sizeof(pImpl->_amendedBlockCount);

			std::memcpy(&pImpl->_preambleOffset, &buffer.at(addressIndex), sizeof(pImpl->_preambleOffset));
			addressIndex += sizeof(pImpl->_preambleOffset);

			std::memcpy(&pImpl->_nZeroFloats, &buffer.at(addressIndex), sizeof(pImpl->_nZeroFloats));
			addressIndex += sizeof(pImpl->_nZeroFloats);
		}
		//Validate header?



		//Evaluate Preamble
		// uint64_t newOffset = addressIndex + pImpl->_preambleOffset;
		if (pImpl->_preambleOffset > 0)
		{
			Utilities::AlignBufferAndRead(buffer, addressIndex, 4, 4, &pImpl->_preambleCount);
			pImpl->_preamble.reserve(pImpl->_preambleCount);
			for (uint64_t i = 0; i < pImpl->_preambleCount; i++)
			{

				pImpl->_preamble.push_back(Preamble(buffer, addressIndex, pImpl->_frameCount));

			}
		}

		//FILL PAD????
		//addressIndex += 4 * static_cast<unsigned long long>(_amendedBlockCount);
		pImpl->_amendedHashSet.resize(pImpl->_amendedBlockCount);
		for (uint64_t i = 0; i < pImpl->_amendedBlockCount; i++)
		{
			Utilities::AlignBufferAndRead(buffer, addressIndex, 4, 4, &pImpl->_amendedHashSet.at(i));
		}

		//Aligning To 4 before hitting the Animation Block Evaluation
		Utilities::AlignBuffer(addressIndex, 4);

		//Evaluate Animation Blocks
		//INDEX ATLAS
		{
			pImpl->_indexAtlas.resize(pImpl->_indexAtlasCounter);

			// ReSharper disable once CppLocalVariableMayBeConst
			int8_t atlasEntrySize = 1;

			//possible need to be able to switch between byte and short sizing here
			for (int i = 0; i < pImpl->_indexAtlasCounter; i++)
			{
				std::memcpy(&pImpl->_indexAtlas.at(i), &buffer.at(addressIndex), atlasEntrySize);
				addressIndex += atlasEntrySize;
			}
		}

		//Animation Blocks
		unsigned short j = 0; unsigned short k = 0;
		for (unsigned short i = 0; i < s_SumIndices(pImpl->_indexAtlas, IndexCountingSolution::all); i++)
		{
			if (k >= pImpl->_indexAtlas.at(j))
			{
				j++; //move to next atlas section
				k = 0;
			}

			if (j % 2 == 0)
			{
				AnimationBlock toAdd;
				pImpl->_animationBlocks.push_back(toAdd);
			}
			else
			{
				pImpl->_animationBlocks.push_back(AnimationBlock(buffer, addressIndex, pImpl->_headerFlags));
			}

			k++; //increment counter


		}


		pImpl->_amendedAnimationBlocks.reserve(pImpl->_amendedBlockCount);
		for (unsigned short i = 0; i < pImpl->_amendedBlockCount; i++)
		{
			pImpl->_amendedAnimationBlocks.push_back(AnimationBlock(buffer, addressIndex, pImpl->_headerFlags));
		}

#ifdef DEBUG_BUILD
		if (buffer.size() != addressIndex)
		{
			std::cout << "===========================================================" << std::endl;
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << std::format("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==").c_str() << std::endl;
			std::cout << std::format("BUFFER  SIZE: {}", buffer.size()).c_str() << std::endl;
			std::cout << std::format("BUFFER START: {}", static_cast<void*>(buffer.data())).c_str() << std::endl;
			std::cout << std::format("Size+Start: {}", static_cast<void*>(buffer.size() + buffer.data())).c_str() << std::endl;
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << "Press ENTER to continue running the program." << std::endl;
			std::cin.get();
			std::cout << std::format("===========================================================").c_str() << std::endl;
		}
#endif
		return {Utilities::FileResult::FileErrorCode::Success, inputFilePath, ""};
	}

	Utilities::FileResult Animation::writeToFile(Utilities::PathContainer& outputFilePath)
	{
		//D:/ModOrganizer/Starfield_Mod_Authoring_01/mods/ExtractedData/meshes/actors/human/animations/scenes/mq101_001_miningscene/female/animstart_lin.af has the largest size of 780896 bytes


		Utilities::BufferObject buffer;
		buffer.reserve(781000); //temp optimization based on largest BGS file size

		//We write as little endian by default, can include a bswap in the future
		unsigned long long addressIndex = 0;


		//Header Section
		{
			buffer.insert(buffer.endPos(), 64, 0); //Prepares empty entries for the header. Header is always 64 bytes long as of file version 05
			std::memcpy(&buffer.at(addressIndex), &pImpl->_magicNumber, sizeof(pImpl->_magicNumber));
			addressIndex += sizeof(pImpl->_magicNumber);

			{
				const float qBuffer[4] = {pImpl->_headerRotation.w(),pImpl->_headerRotation.x(),pImpl->_headerRotation.y(),pImpl->_headerRotation.z()};
				const float vBuffer[3] = { pImpl->_headerTranslation.x(),pImpl->_headerTranslation.y(),pImpl->_headerTranslation.z() };
				std::memcpy(&buffer.at(addressIndex), &qBuffer, sizeof(qBuffer));
				addressIndex += sizeof(qBuffer);
				std::memcpy(&buffer.at(addressIndex), &vBuffer, sizeof(vBuffer));
				addressIndex += sizeof(vBuffer);
			}

			const unsigned char flagBuffer[4] = { pImpl->_headerFlags.data(),pImpl->_headerFlags.c1(),pImpl->_headerFlags.c2(),pImpl->_headerFlags.c3()};
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

			std::memcpy( &buffer.at(addressIndex), &pImpl->_amendedBlockCount, sizeof(pImpl->_amendedBlockCount));
			addressIndex += sizeof(pImpl->_amendedBlockCount);

			std::memcpy( &buffer.at(addressIndex), &pImpl->_preambleOffset, sizeof(pImpl->_preambleOffset));
			addressIndex += sizeof(pImpl->_preambleOffset);

			std::memcpy( &buffer.at(addressIndex), &pImpl->_nZeroFloats, sizeof(pImpl->_nZeroFloats));
			addressIndex += sizeof(pImpl->_nZeroFloats);

		}

		//PREAMBLE AND FILLER FLOATS WOULD GO HERE. IGNORED FOR NOW

		//Index Atlas

		//possible need to be able to switch between byte and short sizing here
		// ReSharper disable once CppVariableCanBeMadeConstexpr
		const int8_t atlasEntrySize = 1;

		buffer.insert(buffer.endPos(), static_cast<size_t>(atlasEntrySize * pImpl->_indexAtlasCounter), 0);
		for (int i = 0; i < pImpl->_indexAtlasCounter; i++)
		{
			std::memcpy( &buffer.at(addressIndex), &pImpl->_indexAtlas.at(i), atlasEntrySize);
			addressIndex += atlasEntrySize;
		}

		//Animation Blocks
		const unsigned short animationBlockCount = s_SumIndices(pImpl->_indexAtlas, IndexCountingSolution::odd);
		for (unsigned short i = 0; i < animationBlockCount; i++)
		{
			pImpl->_animationBlocks.at(i).serializeIntoBuffer(buffer,addressIndex, pImpl->_headerFlags);
		}

		//Skipping additional block entries until we know more


#ifdef DEBUG_BUILD
		if (pImpl->fileSize != addressIndex && pImpl->fileSize > 0)
		{
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << std::format("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==").c_str() << std::endl;
			std::cout << std::format("BUFFER  SIZE: {}", buffer.size()).c_str() << std::endl;
			std::cout << std::format("FILE  SIZE: {}", pImpl->fileSize).c_str() << std::endl;
			std::cout << std::format("===========================================================").c_str() << std::endl;
			std::cout << "Press ENTER to continue running the program." << std::endl;
			std::cin.get();
			std::cout << std::format("===========================================================").c_str() << std::endl;
		}
#endif
		//Writing to file


		return WriteToBinaryFile(outputFilePath, buffer);
	}

	Utilities::FileResult Animation::writeToFile(Utilities::PathContainer&& outputFilePath)
	{
		Utilities::PathContainer output(outputFilePath);
		return writeToFile(output);
	}
	bool Animation::verifyAmendedBlocks() const
	{
		return pImpl->_amendedBlockCount == pImpl->_amendedHashSet.size() && pImpl->_amendedBlockCount == pImpl->_amendedAnimationBlocks.size();
	}
#pragma endregion


}
