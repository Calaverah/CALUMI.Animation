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

	template <typename T>
	std::string VectorToJSON(const std::vector<T>& vec) {
		std::string output = "[\n";
		for (size_t i = 0; i < vec.size(); i++) {
			output += vec.at(i).ToJSON();
			if (i < vec.size() - 1) {
				output += ",\n";
			}
		}
		output += "\n]";
		return output;
	}

	inline void WriteJSONToFile(const std::string& filePath, const std::string& json) {
		if (filePath.empty()) {
			std::print("[CALUMI.Utilities] File path is empty. Cannot write JSON to file.\n");
			return;
		}

		std::ofstream outFile(filePath);

		if (!outFile.is_open()) {
			std::print("[CALUMI.Utilities] Failed to open file for writing: {}\n", filePath);
			// Now print the absolute path (cwd)
			std::filesystem::path absPath = std::filesystem::absolute(filePath);
			std::print("[CALUMI.Utilities] Absolute path attempted: {}\n", absPath.string());
			return;
		}

		outFile << json;
		outFile.close();

		std::filesystem::path absPath = std::filesystem::absolute(filePath);
		std::print("[CALUMI.Utilities] JSON written to: {}\n", absPath.string());
	}
}
}

