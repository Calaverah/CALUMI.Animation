//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_Math.h"
#include "FileError.h"
#include "FileValidation.h"
#include "CALUMI_ReadWritable.h"
#include "SFBGS_AnimationEntries.h"
#include <cstdint>
#include <expected>
#include <iostream>
#include <vector>
#include "CALUMI_Utilities.h"
#include <print>

namespace CALUMI{
	namespace SFBGS{
		enum indexCountingSolution
		{
			odd, //1,3,5,etc
			even, //0,2,4,etc
			all
		};

		struct HeaderFlags
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

			HeaderFlags() = default;
			HeaderFlags(char c0, char c1, char c2, char c3);
			HeaderFlags(char input[4]);
		};

		struct CALUMIANIMATION_API AnimationBlock
		{
			unsigned short _rotationCount = 0;
			unsigned short _rotationPrefixCount = 0; //rotation prefix contains the first half of the rotations in a total of 4 bytes, x,y,z, and a count/identifier such that the prefix can be reused on multiple keyframes to save space

			unsigned short _translationCount = 0;
			unsigned short _translationPrefixCount = 0; //same as rotation prefix, however this entry is always 2 bytes each with the final 2 bytes (a ushort) being the count

			unsigned short _scalarCount = 0;
			unsigned short _bonePriorityCount = 0; //Never appears to have a count of more than 1, if at all



			std::vector<unsigned short> rotationKeyFrames;
			std::vector<unsigned short> translationKeyFrames;
			std::vector<unsigned short> scalarKeyFrames;
			std::vector<unsigned short> bonePriorityKeyFrames; //Usually just a single entry for the 0th frame


			//ROTATIONS SECTION
			std::vector<CALUMI::SFBGS::RotationEntry> rotationEntries;
			std::vector<CALUMI::SFBGS::RotationPrefix> rotationPrefixEntries;

			//TRANSLATIONS SECTION
			std::vector<CALUMI::SFBGS::TranslationEntry> translationEntries;
			std::vector<CALUMI::SFBGS::TranslationPrefix> translationPrefixEntries;

			//ADDITIONALS SECTION
			std::vector<short> scalarEntries;
			std::vector<uint8_t> bonePriorityEntries; //Unsure how to process this information as of now. Appears to be in units/100 for a scalar of 0-100%

			AnimationBlock() = default;
			AnimationBlock(std::vector<char>& buffer, unsigned long long& addressIndex, const HeaderFlags& flags);
			void SerializeIntoBuffer(std::vector<char>& buffer, unsigned long long& addressIndex, const HeaderFlags& flags);

		};



		class CALUMIANIMATION_API Animation : CALUMI::ReadWritable
		{

		public:
			unsigned int fileSize = 0; //temp value for debugging
			std::string animationFileName; //For file tracking. Should be unique without extension

			float HeaderStart[9] = {}; //2 blanks, 4 Quat Components (or all zero), 3 unknown (possibly xyz values)

			HeaderFlags _headerFlags; //1 byte and 3 empty bytes

			short _versionNumber = 0x05; //Always 05

			unsigned short _boneCount = 0;
			unsigned short _frameCount = 0;
			unsigned short _indexAtlasCounter = 0x02; //At least 2, unless bones are zero (special case where addition bones are filled in at the bottom but they may not be from the main rig)

			unsigned short _unknownFillCount = 0; //UNKNOWN: 

			unsigned short _preambleOffset = 0; //NOTE: this tells us how long the unknown "Preamble" section is and when the regular animation data begins

			float _nZeroFloats[3] = { 0.0,-0.0,0.0 }; //UNKNOWN: Not always zero, sometimes has pi too yummy

			std::vector<float> _unknownSuffixFillFloats;
			//--------------------------------------------------The header to this point is 64 bytes. The Animation blocks begin at address = 64+preambleOffset

			//preamble section goes here
				//NOTE: Preamble begins after the nZeroFloat array and is [preambleOffset] bytes long.



			//animation index atlas goes here
			std::vector<unsigned short> _indexAtlas; //NOTE: this index is the size of the indexAtlasCounter, it is unknown if the entries are one byte only or if they can be expanded to two bytes, 
			//so we will keep our entries as a short (2 bytes) and cast them if they can be casted as 1 byte

			//animation blocks go here
			std::vector<AnimationBlock> animationBlocks;
			std::vector<AnimationBlock> animationSuffixBlocks;


			unsigned short _SumIndices(std::vector<unsigned short>inputVector, indexCountingSolution type);
			/// <summary>
			/// Header flags will be reset based on the values of the entries. It is not recommended to call this directly.
			/// </summary>
			void _evaluateHeaderFlags();
			


			// Inherited via CALUMI::ReadWritable
			std::expected<bool, FileError> ReadFromFile(std::filesystem::path& inputFilePath) override;
			std::expected<std::string, FileError> WriteToFile(std::filesystem::path& outputFilePath) override;

			

		};
	}
}

