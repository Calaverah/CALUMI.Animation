//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "FileError.h"
#include <format>
#include <string>

CALUMI::Utilities::StringContainer FileError::ToString()
{

	std::string fileCodeString;

	switch (fileCode) {
	case FileErrorCode::FileNotFound:		fileCodeString = "File not found. "; break;
	case FileErrorCode::PermissionDenied:	fileCodeString = "Permission denied. "; break;
	case FileErrorCode::NotAFile:			fileCodeString = "Path is not a regular file. "; break;
	case FileErrorCode::ReadFailure:		fileCodeString = "Failed to read file. "; break;
	case FileErrorCode::IncorrectFileType:	fileCodeString = "File is not expected type. "; break;
	case FileErrorCode::FileTooLarge:		fileCodeString = "File exceeds size limits. "; break;
	case FileErrorCode::FileTooSmall:		fileCodeString = "File does not meet size requirement. "; break;
	case FileErrorCode::UnknownErrorCode:	fileCodeString = "Unknown Error. "; break;
	case FileErrorCode::WriteFailure:		fileCodeString = "Failed to write to file. "; break;
	default: fileCodeString = "No Error Code Found. See fileCode - FileErrorCode Enum. ";
	}

	std::string em;
	if (!errorMessage.Empty()) em = std::format("({})", errorMessage.c_str());
	else em = "";

	CALUMI::Utilities::StringContainer output("FILE ERROR: [");
	output += path.StringContainer();
	output += "]: ";
	output += fileCodeString.c_str();
	output += em.c_str();
	return output;
}

FileError::FileError(const FileError& source)
{
	fileCode = source.fileCode;
	errorMessage = source.errorMessage;
	path = source.path;
}

FileError& FileError::operator=(const FileError& source)
{
	fileCode = source.fileCode;
	errorMessage = source.errorMessage;
	path = source.path;
	return *this;
}

