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

		//Header Flag Defs

		HeaderFlags::HeaderFlags(char c0, char c1, char c2, char c3)
		{
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

		HeaderFlags::HeaderFlags(char input[4])
		{
			firstEntry = input[0] >> 0 & 0b1;
			shortKeyCounters = input[0] >> 1 & 0b1;
			shortKeyFrameEntries = input[0] >> 2 & 0b1;
			scalarSequenceFlag = input[0] >> 3 & 0b1;
			u1 = input[0] >> 4 & 0b1;
			u2 = input[0] >> 5 & 0b1;
			u3 = input[0] >> 6 & 0b1;
			u4 = input[0] >> 7 & 0b1;

			this->c1 = input[1];
			this->c2 = input[2];
			this->c3 = input[3];
		}

		//Preamble Defs
		Preamble::Preamble(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex, size_t frameCount)
		{
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 2, 2, &count);
			

			preSet.resize(static_cast<size_t>(count) * 2);
			for (int i = 0; i < (static_cast<size_t>(count) * 2); i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 4, 4, &preSet.at(i));
			}
			mainSet.resize(frameCount);
			for (int i = 0; i < frameCount; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 4, 4, &mainSet.at(i));
			}
			//int nCount = count == 1 ? 2 : count;
			footer1.resize(count);
			for (int i = 0; i < count; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 2, 2, &footer1.at(i));
			}

			footer2.resize(count);
			for (int i = 0; i < count; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 1, &footer2.at(i));
			}

		}

		Preamble& Preamble::operator=(const Preamble& other)
		{
			count = other.count;
			preSet = other.preSet;
			mainSet = other.mainSet;
			footer1 = other.footer1;
			footer2 = other.footer2;
			return *this;
		}

		Preamble::Preamble(const Preamble& other)
		{
			count = other.count;
			preSet = other.preSet;
			mainSet = other.mainSet;
			footer1 = other.footer1;
			footer2 = other.footer2;
		}

		//Animation Block Defs
		AnimationBlock::AnimationBlock(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex, const HeaderFlags& flags)
		{
			//address index can help with alignment when working with a raw buffer of bytes
			//eg. if the current index is at 0x07 and we have an aligned float
			//then we go forward (alignmentSize)-(index%(alignmentSize)) for the appropriate address
			// or in this case (4-7%4) = 1 and now our float can begin on address 0x08 as it should
			//We must make sure to advance the address index along with the currentAddress pointer
			uint8_t cSize = 1 + (int)flags.shortKeyCounters;
			uint8_t kSize = 1 + (int)flags.shortKeyFrameEntries;

			//fill the initial 6 counter values
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &_rotationCount);
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &_rotationPrefixCount);
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &_translationCount);
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &_translationPrefixCount);
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &_scalarCount);
			CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, cSize, cSize, &_bonePriorityCount);

			//Fill Keyframe arrays
			{
				rotationKeyFrames.resize(_rotationCount);
				rotationKeyFrames.fill(0); //resize should be all zero by default, but just in case
				for (unsigned short i = 0; i < _rotationCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &rotationKeyFrames.at(i));
				}


				translationKeyFrames.resize(_translationCount);
				translationKeyFrames.fill(0); //""
				for (unsigned short i = 0; i < _translationCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &translationKeyFrames.at(i));
				}

				scalarKeyFrames.resize(_scalarCount);
				scalarKeyFrames.fill(0); //""
				for (unsigned short i = 0; i < _scalarCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &scalarKeyFrames.at(i));
				}

				bonePriorityKeyFrames.resize(_bonePriorityCount);
				bonePriorityKeyFrames.fill(0); //""
				for (unsigned short i = 0; i < _bonePriorityCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, kSize, kSize, &bonePriorityKeyFrames.at(i));
				}
			}

			//Fill Keyframe Entries and Prefix Entries
			{
				rotationEntries.resize(_rotationCount); //Resize to fill with default entries (empty)
				for (unsigned short i = 0; i < _rotationCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 3, &rotationEntries.at(i));
				}

				rotationPrefixEntries.resize(_rotationPrefixCount);
				for (unsigned short i = 0; i < _rotationPrefixCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 4, &rotationPrefixEntries.at(i));
				}


				translationEntries.resize(_translationCount);
				for (unsigned short i = 0; i < _translationCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 3, &translationEntries.at(i));
				}

				translationPrefixEntries.resize(_translationPrefixCount);
				for (unsigned short i = 0; i < _translationPrefixCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 2, 8, &translationPrefixEntries.at(i));
				}

				scalarEntries.resize(_scalarCount);
				scalarEntries.fill(0);
				for (unsigned short i = 0; i < _scalarCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 2, 2, &scalarEntries.at(i));
				}

				bonePriorityEntries.resize(_bonePriorityCount);
				bonePriorityEntries.fill(0);
				for (unsigned short i = 0; i < _bonePriorityCount; i++)
				{
					CALUMI::Utilities::AlignBufferAndRead(buffer, addressIndex, 1, 1, &bonePriorityEntries.at(i));
				}
			}

			//END OF ANIMATION BLOCK

		}

		void AnimationBlock::SerializeIntoBuffer(Utilities::VectorContainer<char>& buffer, unsigned long long& addressIndex, const HeaderFlags& flags)
		{
			uint8_t cSize = 1 + (int)flags.shortKeyCounters;
			uint8_t kSize = 1 + (int)flags.shortKeyFrameEntries; //alignment and sizing helpers

			//Counts
			{
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &_rotationCount);
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &_rotationPrefixCount);
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &_translationCount);
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &_translationPrefixCount);
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &_scalarCount);
				CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, cSize, cSize, &_bonePriorityCount);
			}

			//KeyFrame arrays
			{
				for (unsigned short i = 0; i < _rotationCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &rotationKeyFrames.at(i));
				}

				for (unsigned short i = 0; i < _translationCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &translationKeyFrames.at(i));
				}

				for (unsigned short i = 0; i < _scalarCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &scalarKeyFrames.at(i));
				}

				for (unsigned short i = 0; i < _bonePriorityCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, kSize, kSize, &bonePriorityKeyFrames.at(i));
				}
			}

			//Keyframe Entries and Prefix Entries
			{
				for (unsigned short i = 0; i < _rotationCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 3, &rotationEntries.at(i));
				}

				for (unsigned short i = 0; i < _rotationPrefixCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 4, &rotationPrefixEntries.at(i));
				}

				for (unsigned short i = 0; i < _translationCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 3, &translationEntries.at(i));
				}

				for (unsigned short i = 0; i < _translationPrefixCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 2, 8, &translationPrefixEntries.at(i));
				}

				for (unsigned short i = 0; i < _scalarCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 2, 2, &scalarEntries.at(i));
				}

				for (unsigned short i = 0; i < _bonePriorityCount; i++)
				{
					CALUMI::Utilities::AlignFillBufferAndWrite(buffer, addressIndex, 1, 1, &bonePriorityEntries.at(i));
				}
			}

			//END OF ANIMATION BLOCK
		}



		//Animation Defs

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

			if (_frameCount > 0xFF) newFlags.shortKeyFrameEntries = true;

			for (int i = 0; i < animationBlocks.size(); i++)
			{
				if (animationBlocks.at(i)._rotationCount > 0xFF || animationBlocks.at(i)._translationCount > 0xFF || animationBlocks.at(i)._scalarCount > 0xFF || animationBlocks.at(i)._bonePriorityCount > 0xFF)
					newFlags.shortKeyCounters = true;


				if (animationBlocks.at(i)._scalarCount > 0)
					newFlags.scalarSequenceFlag = true;

				//if (_headerFlags.scalarSequenceFlag && _headerFlags.shortKeyCounters) break; //we can exit loop early as both flags are set

			}
			_headerFlags = newFlags;
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

			fileSize = buffer.value().size();
			animationFileName = inputFilePath.stem().c_str();
			//iterator tracking
			//char* currentAddress = buffer.value().data();
			unsigned long long addressIndex = 0;

			//Evaluate header
			{
				std::memcpy(&_headerStart, &buffer.value().at(addressIndex), sizeof(_headerStart));
				addressIndex += sizeof(_headerStart);

				char flagBuff[4] = {};
				std::memcpy(&flagBuff, &buffer.value().at(addressIndex), sizeof(flagBuff));
				_headerFlags = HeaderFlags(flagBuff);
				addressIndex += sizeof(flagBuff);

				std::memcpy(&_versionNumber, &buffer.value().at(addressIndex), sizeof(_versionNumber));
				addressIndex += sizeof(_versionNumber);

				std::memcpy(&_boneCount, &buffer.value().at(addressIndex), sizeof(_boneCount));
				addressIndex += sizeof(_boneCount);

				std::memcpy(&_frameCount, &buffer.value().at(addressIndex), sizeof(_frameCount));
				addressIndex += sizeof(_frameCount);

				std::memcpy(&_indexAtlasCounter, &buffer.value().at(addressIndex), sizeof(_indexAtlasCounter));
				addressIndex += sizeof(_indexAtlasCounter);

				std::memcpy(&_unknownFillCount, &buffer.value().at(addressIndex), sizeof(_unknownFillCount));
				addressIndex += sizeof(_unknownFillCount);

				std::memcpy(&_preambleOffset, &buffer.value().at(addressIndex), sizeof(_preambleOffset));
				addressIndex += sizeof(_preambleOffset);

				std::memcpy(&_nZeroFloats, &buffer.value().at(addressIndex), sizeof(_nZeroFloats));
				addressIndex += sizeof(_nZeroFloats);
			}
			//Validate header?

			

			//Evaluate Preamble
			size_t newOffset = addressIndex + _preambleOffset;
			if (_preambleOffset > 0)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer.value(), addressIndex, 4, 4, &preambleCount);
				preamble.reserve(preambleCount);
				for (size_t i = 0; i < preambleCount; i++)
				{

					preamble.push_back(Preamble(buffer.value(), addressIndex, _frameCount));

				}
			}

			//FILL PAD????
			//addressIndex += 4 * static_cast<unsigned long long>(_unknownFillCount);
			_suffixFillerValues.resize(_unknownFillCount);
			for (size_t i = 0; i < _unknownFillCount; i++)
			{
				CALUMI::Utilities::AlignBufferAndRead(buffer.value(), addressIndex, 4, 4, &_suffixFillerValues.at(i));
			}

			//Aligning To 4 before hitting the Animation Block Evalutation
			CALUMI::Utilities::AlignBuffer(buffer.value(), addressIndex, 4);

			//Evaluate Animation Blocks
			//INDEX ATLAS
			{
				_indexAtlas.resize(_indexAtlasCounter);
				_indexAtlas.fill(0); //resize should be all zero by default, but just in case
				int8_t atlasEntrySize = 1; //possible need to be able to switch between byte and short sizing here
				for (int i = 0; i < _indexAtlasCounter; i++)
				{
					std::memcpy(&_indexAtlas.at(i), &buffer.value().at(addressIndex), atlasEntrySize);
					addressIndex += atlasEntrySize;
				}
			}

			//Animation Blocks
			unsigned short j = 0; unsigned short k = 0;
			for (unsigned short i = 0; i < _SumIndices(_indexAtlas, IndexCountingSolution::all); i++)
			{
				if (k >= _indexAtlas.at(j))
				{
					j++; //move to next atlas section
					k = 0;
				}

				if (j % 2 == 0)
				{
					AnimationBlock toAdd;
					animationBlocks.push_back(toAdd);
				}
				else
				{
					animationBlocks.push_back(AnimationBlock(buffer.value(), addressIndex, _headerFlags));
				}

				k++; //increment counter
				
				
			}


			animationSuffixBlocks.reserve(_unknownFillCount);
			for (unsigned short i = 0; i < _unknownFillCount; i++)
			{
				animationSuffixBlocks.push_back(AnimationBlock(buffer.value(), addressIndex, _headerFlags));
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
				std::memcpy(&buffer.at(addressIndex), &_headerStart, sizeof(_headerStart));
				addressIndex += sizeof(_headerStart);
				
				std::memcpy(&buffer.at(addressIndex),&_headerFlags, sizeof(_headerFlags));
				addressIndex += sizeof(_headerFlags);

				std::memcpy(&buffer.at(addressIndex), &_versionNumber, sizeof(_versionNumber));
				addressIndex += sizeof(_versionNumber);

				std::memcpy( &buffer.at(addressIndex), &_boneCount, sizeof(_boneCount));
				addressIndex += sizeof(_boneCount);

				std::memcpy( &buffer.at(addressIndex), &_frameCount, sizeof(_frameCount));
				addressIndex += sizeof(_frameCount);

				std::memcpy( &buffer.at(addressIndex), &_indexAtlasCounter, sizeof(_indexAtlasCounter));
				addressIndex += sizeof(_indexAtlasCounter);

				std::memcpy( &buffer.at(addressIndex), &_unknownFillCount, sizeof(_unknownFillCount));
				addressIndex += sizeof(_unknownFillCount);

				std::memcpy( &buffer.at(addressIndex), &_preambleOffset, sizeof(_preambleOffset));
				addressIndex += sizeof(_preambleOffset);

				std::memcpy( &buffer.at(addressIndex), &_nZeroFloats, sizeof(_nZeroFloats));
				addressIndex += sizeof(_nZeroFloats);

			}

			//PREAMBLE AND FILLER FLOATS WOULD GO HERE. IGNORED FOR NOW

			//Index Atlas
			int8_t atlasEntrySize = 1; //possible need to be able to switch between byte and short sizing here
			buffer.insert(buffer.end(), static_cast<size_t>(atlasEntrySize * _indexAtlasCounter), 0);
			for (int i = 0; i < _indexAtlasCounter; i++)
			{
				std::memcpy( &buffer.at(addressIndex), &_indexAtlas.at(i), atlasEntrySize);
				addressIndex += atlasEntrySize;
			}

			//Animation Blocks
			unsigned short animationBlockCount = _SumIndices(_indexAtlas, IndexCountingSolution::odd);
			for (unsigned short i = 0; i < animationBlockCount; i++)
			{
				animationBlocks.at(i).SerializeIntoBuffer(buffer,addressIndex,_headerFlags);
			}

			//Skipping additional block entries until we know more



			if (fileSize != addressIndex && fileSize > 0)
			{
				std::println("===========================================================");
				std::println("===========================================================");
				std::println("==WARNING CURRENT BUFFER ADDRESS IS NOT AT FINAL POSITION==");
				//std::println("CURRENT  POS: {}",(void*)currentAddress);
				//std::println("CURRENT ITER: {}",addressIndex);
				std::println("BUFFER  SIZE: {}", buffer.size());
				std::println("FILE  SIZE: {}", fileSize);
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

}
}
