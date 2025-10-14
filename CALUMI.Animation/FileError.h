//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_Utilities.h"

enum class FileErrorCode
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
	CALUMI::Utilities::PathContainer path;
	CALUMI::Utilities::StringContainer errorMessage;
	

	//friend std::ostream& operator<<(std::ostream& os, FileError error);

	CALUMI::Utilities::StringContainer ToString();
	FileError() = default;


	FileError(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const char* errorMessage) 
		: fileCode(fileCode), path(path), errorMessage(errorMessage) {};
	FileError(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const CALUMI::Utilities::StringContainer& errorMessage)
		: fileCode(fileCode), path(path), errorMessage(errorMessage) {};
	FileError(const FileError& source);
	FileError& operator=(const FileError& source);
};

template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedConatiner<bool, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedConatiner<CALUMI::Utilities::PathContainer, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedConatiner<CALUMI::Utilities::StringContainer, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedConatiner<CALUMI::Utilities::StringMap, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedConatiner<CALUMI::Utilities::VectorContainer<char>, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedConatiner<CALUMI::Utilities::VectorContainer<CALUMI::Utilities::StringContainer>, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedConatiner<CALUMI::Utilities::VectorContainer<CALUMI::Utilities::PathContainer>, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedConatiner<CALUMI::Utilities::VectorContainer<int>, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedConatiner<CALUMI::Utilities::VectorContainer<float>, FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedConatiner<CALUMI::Utilities::VectorContainer<double>, FileError>;