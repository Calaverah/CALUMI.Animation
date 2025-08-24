//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_Utilities.h"
#include <print>
namespace CALUMI { namespace Utilities {

	
	void _AlignBuffer(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize)
	{
		if (alignmentSize < 1) return; //if alignment size is for some reason 0, we skip this output and move on
		if (currentIndex % alignmentSize < 1) return; //we are aligned, move on

		int offset = (alignmentSize - (currentIndex % alignmentSize));

		//currentPosition += offset;
		currentIndex += offset;
	}

	void _AlignFillBuffer(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize)
	{
		unsigned long long startingIndex = currentIndex;
		_AlignBuffer(buffer, currentIndex, alignmentSize);
		buffer.insert(buffer.end(), (currentIndex-startingIndex), 0);
	}


	/// <summary>
	/// This method will assign a value from a buffer with memcpy, however it will consider padding from the byte array buffer first.
	/// When finished it will advance the currentPosition index by the appropriate amount of spaces
	/// </summary>
	/// <param name="currentPosition"></param>
	/// <param name="currentIndex"></param>
	/// <param name="alignmentSize"></param>
	/// <param name="Destination"></param>
	void AlignBufferAndRead(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Destination)
	{
		_AlignBuffer(buffer, currentIndex, alignmentSize);
		std::memcpy(Destination, &buffer.at(currentIndex), variableSize);

		currentIndex += variableSize;
	}

	/// <summary>
	/// This method will assign a value to a buffer with memcpy, however it will consider padding
	/// This method is to be used when a buffer is declared but sizing is unknown. Blank values will be inserted
	/// at the end of the vector to be overwritten
	/// </summary>
	/// <param name="currentPosition"></param>
	/// <param name="currentIndex"></param>
	/// <param name="alignmentSize"></param>
	/// <param name="Destination"></param>
	void AlignFillBufferAndWrite(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Source)
	{
		_AlignFillBuffer(buffer, currentIndex, alignmentSize);
		//if source is a short but needs to be a byte, the variableSize will take only the first byte which in Little Endian, is any value under 255
		buffer.insert(buffer.end(), variableSize, 0);
		std::memcpy(&buffer.at(currentIndex), Source, variableSize);
			
		currentIndex += variableSize;
	}

	}}
