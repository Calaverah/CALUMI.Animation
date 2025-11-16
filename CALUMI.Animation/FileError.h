//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_Utilities.h"

enum class CALUMIANIMATION_API FileErrorCode : uint8_t
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
	FileErrorCode GetFileErrorCode() const;
	CALUMI::Utilities::PathContainer GetFilePath() const;
	const char* GetErrorMessage() const;

	CALUMI::Utilities::StringContainer ToString();
	FileError();
	~FileError();
	FileError(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const char* errorMessage);
	FileError(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const CALUMI::Utilities::StringContainer& errorMessage);
	FileError(const FileError& source);

	FileError& operator=(const FileError& source);

private:
	struct Impl;
	Impl* pImpl;
};

#pragma warning(disable: 4661)
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<bool, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::PathContainer, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::StringContainer, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::StringMap, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<char>, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<CALUMI::Utilities::StringContainer>, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<CALUMI::Utilities::PathContainer>, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<int>, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<float>, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<double>, FileError>;
#pragma warning(default: 4661)