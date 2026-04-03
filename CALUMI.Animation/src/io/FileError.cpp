//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com
//#include "internalplatform.h"

#include "internalplatform.h"
#include "io/FileError.h"

namespace CALUMI {
	namespace Utilities {

		struct FileError::Impl
		{
			FileErrorCode fileCode = FileErrorCode::UnknownErrorCode;
			CALUMI::Utilities::PathContainer path;
			CALUMI::Utilities::StringContainer errorMessage;
			Impl() = default;
			Impl(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const char* errorMessage) : fileCode(fileCode), path(path), errorMessage(errorMessage) {}
			Impl(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const CALUMI::Utilities::StringContainer& errorMessage) : fileCode(fileCode), path(path), errorMessage(errorMessage) {}
		};

		CALUMI::Utilities::StringContainer FileError::ToString()
		{

			std::string fileCodeString;

			switch (pImpl->fileCode) {
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
			if (!pImpl->errorMessage.Empty()) em = std::format("({})", pImpl->errorMessage.c_str());
			else em = "";

			CALUMI::Utilities::StringContainer output("FILE ERROR: [");
			output += pImpl->path.StringContainer();
			output += "]: ";
			output += fileCodeString.c_str();
			output += em.c_str();
			return output;
		}

		FileError::FileErrorCode FileError::GetFileErrorCode() const
		{
			return pImpl->fileCode;
		}

		CALUMI::Utilities::PathContainer FileError::GetFilePath() const
		{
			return pImpl->path;
		}

		const char* FileError::GetErrorMessage() const
		{
			return pImpl->errorMessage.c_str();
		}

		FileError::~FileError()
		{
			if (pImpl)
				delete pImpl;
		}

		FileError::FileError()
		{
			pImpl = new Impl;
		}

		FileError::FileError(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const char* errorMessage)
		{
			pImpl = new Impl(fileCode, path, errorMessage);
		}

		FileError::FileError(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const CALUMI::Utilities::StringContainer& errorMessage)
		{
			pImpl = new Impl(fileCode, path, errorMessage);
		}

		FileError::FileError(const FileError& source)
		{
			pImpl = new Impl;
			pImpl->fileCode = source.pImpl->fileCode;
			pImpl->errorMessage = source.pImpl->errorMessage;
			pImpl->path = source.pImpl->path;
		}

		FileError& FileError::operator=(const FileError& source)
		{
			pImpl->fileCode = source.pImpl->fileCode;
			pImpl->errorMessage = source.pImpl->errorMessage;
			pImpl->path = source.pImpl->path;
			return *this;
		}

}}