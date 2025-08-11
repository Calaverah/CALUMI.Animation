#pragma once
#ifdef CALUMIANIMATION_EXPORTS // This macro is typically defined by Visual Studio for DLL projects
#define CALUMIANIMATION_API __declspec(dllexport)
#else
#define CALUMIANIMATION_API __declspec(dllimport)
#endif

#include <cstring>
#include <vector>

namespace CALUMI { namespace Utilities {

	//void _AlignBuffer(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize);
	void AlignBufferAndRead(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Destination);
	void AlignFillBufferAndWrite(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Source);

}}

