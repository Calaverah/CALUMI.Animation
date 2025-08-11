#pragma once

#ifdef CALUMIANIMATION_EXPORTS // This macro is typically defined by Visual Studio for DLL projects
#define CALUMIANIMATION_API __declspec(dllexport)
#else
#define CALUMIANIMATION_API __declspec(dllimport)
#endif

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

