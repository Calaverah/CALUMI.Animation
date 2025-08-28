//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include <string>
#include <filesystem>
#include <print>
#include <format>



enum CALUMIANIMATION_API FileErrorCode
{
	FileNotFound,
	PermissionDenied,
	NotAFile,
	ReadFailure,
	IncorrectFileType,
	FileTooLarge,
	FileTooSmall,
	UnknownErrorCode,
	WriteFailure
};

struct CALUMIANIMATION_API FileError
{
	FileErrorCode fileCode = FileErrorCode::UnknownErrorCode;
	std::filesystem::path path;
	std::string errorMessage;

	//friend std::ostream& operator<<(std::ostream& os, FileError error);

	std::string ToString();
	//FileError() = default;
};

