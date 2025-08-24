//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include <string>
#include <filesystem>
#include <expected>
#include "FileError.h"

namespace CALUMI {
	class CALUMIANIMATION_API ReadWritable
	{
		virtual std::expected<bool, FileError> ReadFromFile(std::filesystem::path& inputFilePath) = 0;
		virtual std::expected<std::string, FileError> WriteToFile(std::filesystem::path& outputFilePath) = 0;
	};
}

