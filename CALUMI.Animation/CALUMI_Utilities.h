//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include <cstring>
#include <string>
#include <vector>
#include <print>
#include <filesystem>
#include <fstream>

namespace CALUMI { namespace Utilities {

	//Buff Stuff
	void AlignBufferAndRead(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Destination);
	void AlignFillBufferAndWrite(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Source);

	//String Stuff
	std::string Indent(const int indents);

	//Json Stuff
	template <typename T>
	std::string VectorToJSON(const std::vector<T>& vec, const int indents = 0)
	{
		if (vec.empty()) {
			return " []";
		}

		std::string output = "[\n";
		for (size_t i = 0; i < vec.size(); i++) {
			output += vec.at(i).ToJSON(indents + 1);
			if (i < vec.size() - 1) {
				output += ",\n";
			}
		}
		output += "\n" + Indent(indents) + "]";
		return output;
	}

}
}

