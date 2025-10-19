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

			Utilities::VectorContainer<unsigned short> rotationKeyFrames;
			Utilities::VectorContainer<unsigned short> translationKeyFrames;
			Utilities::VectorContainer<unsigned short> scalarKeyFrames;
			Utilities::VectorContainer<unsigned short> bonePriorityKeyFrames; //Usually just a single entry for the 0th frame


			//ROTATIONS SECTION
			Utilities::VectorContainer<CALUMI::SFBGS::RotationEntry> rotationEntries;
			Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix> rotationPrefixEntries;

			//TRANSLATIONS SECTION
			Utilities::VectorContainer<CALUMI::SFBGS::TranslationEntry> translationEntries;
			Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> translationPrefixEntries;

			//ADDITIONALS SECTION
			Utilities::VectorContainer<short> scalarEntries;
			Utilities::VectorContainer<uint8_t> bonePriorityEntries; //Unsure how to process this information as of now. Appears to be in units/100 for a scalar of 0-100%


			AnimationBlock() = default;
			AnimationBlock(Utilities::VectorContainer<char>&buffer, unsigned long long& addressIndex, const HeaderFlags & flags);
			void SerializeIntoBuffer(Utilities::VectorContainer<char>&buffer, unsigned long long& addressIndex, const HeaderFlags & flags);


		};


		
		class CALUMIANIMATION_API Animation : CALUMI::ReadWritable
		{

		public:
			size_t fileSize = 0; //For debugging
			CALUMI::Utilities::StringContainer animationFileName; //For file tracking. Should be unique without extension
		
			float _headerStart[9] = {}; //2 blanks (CONFIRMED), 4 Quat Components (or all zero), 3 unknown (possibly xyz values)

			HeaderFlags _headerFlags; //1 byte and 3 empty bytes

			short _versionNumber = 0x05; //Always 05

			unsigned short _boneCount = 0;
			unsigned short _frameCount = 0;
			unsigned short _indexAtlasCounter = 0x02; //At least 2, unless bones are zero (special case where addition bones are filled in at the bottom but they may not be from the main rig)

			unsigned short _unknownFillCount = 0; //UNKNOWN: 

			unsigned short _preambleOffset = 0; //NOTE: this tells us how long the unknown "Preamble" section is and when the regular animation data begins

			float _nZeroFloats[3] = { 0.0,-0.0,0.0 }; //UNKNOWN: Not always zero, sometimes has pie too (yummy)

			Utilities::VectorContainer<float> _unknownSuffixFillFloats;
			//--------------------------------------------------The header to this point is 64 bytes. The Animation blocks begin at address = 64+preambleOffset

			//preamble section goes here
				//NOTE: Preamble begins after the nZeroFloat array and is [preambleOffset] bytes long.



			//animation index atlas goes here
			Utilities::VectorContainer<unsigned short> _indexAtlas; //NOTE: this index is the size of the indexAtlasCounter, it is unknown if the entries are one byte only or if they can be expanded to two bytes, 
			//so we will keep our entries as a short (2 bytes) and cast them if they can be casted as 1 byte

			//animation blocks go here
			Utilities::VectorContainer<AnimationBlock> animationBlocks;
			Utilities::VectorContainer<AnimationBlock> animationSuffixBlocks;


			unsigned short _SumIndices(Utilities::VectorContainer<unsigned short>inputVector, IndexCountingSolution type);
			/// <summary>
			/// Header flags will be reset based on the values of the entries. It is not recommended to call this directly.
			/// </summary>
			void _evaluateHeaderFlags();

			Animation() = default;

			// Inherited via CALUMI::ReadWritable
			Utilities::ExpectedContainer<bool, FileError> ReadFromFile(Utilities::PathContainer& inputFilePath) override;
			Utilities::ExpectedContainer<bool, FileError> ReadFromFile(const wchar_t* inputFilePath);
			Utilities::ExpectedContainer<Utilities::StringContainer, FileError> WriteToFile(Utilities::PathContainer& outputFilePath) override;
			Utilities::ExpectedContainer<Utilities::StringContainer, FileError> WriteToFile(const wchar_t* outputFilePath);

		};
#pragma warning(disable: 4661)
		template struct CALUMIANIMATION_API Utilities::VectorContainer<AnimationBlock>;
		template struct CALUMIANIMATION_API Utilities::VectorContainer<Animation>;
#pragma warning(default: 4661)
	}
}

