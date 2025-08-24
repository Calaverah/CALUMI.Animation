//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include <cstring>
#include <vector>

namespace CALUMI { namespace Utilities {

	//void _AlignBuffer(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize);
	void AlignBufferAndRead(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Destination);
	void AlignFillBufferAndWrite(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Source);

}}

