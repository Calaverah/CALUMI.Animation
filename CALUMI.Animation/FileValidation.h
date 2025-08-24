//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include <string>
#include <filesystem>
#include <expected>
#include "FileError.h"
#include <fstream>

namespace CALUMI{


	std::expected<std::vector<char>, FileError> ValidateFile(const std::filesystem::path& inputPath);
	std::expected<std::vector<char>, FileError> ValidateFile(const std::filesystem::path& inputPath, const std::vector<std::string> fileExtensions, bool allowFiles);
	std::expected<std::vector<char>, FileError> ValidateFile(const std::filesystem::path& inputPath, const std::vector<std::string> fileExtensions, unsigned int minFileSize, unsigned long long maxFileSize, bool allowFiles);
	std::expected<std::vector<char>, FileError> ValidateFile(const std::filesystem::path& inputPath, unsigned int minFileSize, unsigned long long maxFileSize);

	std::expected<std::string, FileError> WriteToBinaryFile(const std::filesystem::path& outputPath, std::vector<char>& buffer);


	namespace priv {
		//PRIVATE FUNCTIONS
		std::expected<bool, FileError> _CommonValidation(const std::filesystem::path& inputPath);
		std::expected<std::vector<char>, FileError> _GetBuffer(const std::filesystem::path& inputPath);
		std::expected<bool, FileError> _FileExtValidation(const std::filesystem::path& inputPath, const std::vector<std::string>& fileExtensions, bool allowFiles);
		std::expected<bool, FileError> _ValidateFileSize(const std::filesystem::path& inputPath, unsigned int& minFileSize, unsigned long long& maxFileSize);
	}
}