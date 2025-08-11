#include "pch.h"
#include "FileError.h"

std::ostream& operator<<(std::ostream& os, FileError error)
{
	os << "FileError[" << error.path << "]: ";
	switch (error.fileCode) {
	case FileErrorCode::FileNotFound:		os << "File not found. "; break;
	case FileErrorCode::PermissionDenied:	os << "Permission denied. "; break;
	case FileErrorCode::NotAFile:			os << "Path is not a regular file. "; break;
	case FileErrorCode::ReadFailure:		os << "Failed to read file. "; break;
	case FileErrorCode::IncorrectFileType:	os << "File is not expected type. "; break;
	case FileErrorCode::FileTooLarge:		os << "File exceeds size limits. "; break;
	case FileErrorCode::FileTooSmall:		os << "File does not meet size requirement. "; break;
	case FileErrorCode::UnknownErrorCode:	os << "Unknown Error. "; break;
	case FileErrorCode::WriteFailure:		os << "Failed to write to file."; break;
	default: os << "No Error Code Found. See fileCode - FileErrorCode Enum. ";
	}
	if (!error.errorMessage.empty()) os << " (" << error.errorMessage << ")";

	return os;
}

std::string FileError::ToString()
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
	if (!errorMessage.empty()) em = std::format("({})", errorMessage);
	else em = "";

	return ("FILE ERROR: [" + path.string() + "]: " + fileCodeString + em);
}
