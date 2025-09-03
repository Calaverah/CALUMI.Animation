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

	//void _AlignBuffer(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize);
	void AlignBufferAndRead(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Destination);
	void AlignFillBufferAndWrite(std::vector<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Source);

	inline std::string Indent(const int indents) {
		return std::string(indents*2, ' ');
	}

	template <typename T>
	std::string VectorToJSON(const std::vector<T>& vec, const int indents = 0) {
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

	inline void WriteJSONToFile(const std::filesystem::path filePath, const std::string& json) {

		if (filePath.empty()) {
			std::print("[CALUMI.Utilities] Provided file path is empty. Cannot write JSON.\n");
			return;
		}

		std::ofstream outFile(filePath);

		if (!outFile.is_open()) {
			std::print("[CALUMI.Utilities] Failed to open file for writing: {}\n", filePath.string());
			return;
		}

		outFile.clear();
		outFile << json;
		outFile.close();

		std::print("[CALUMI.Utilities] JSON written to: {}\n", filePath.string());
	}


}
}

