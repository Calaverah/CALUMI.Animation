//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalplatform.h"
#include "io/FileResult.h"
#include <utilities/CALUMI_Utilities.h>
#include <cstdint>
#include <vector>

namespace CALUMI::Utilities
{

	struct FileResult::Impl
	{
		FileErrorCode _fileCode = FileErrorCode::UnknownErrorCode;
		PathContainer _path;
		StringContainer _message;
		Impl() = default;
	};

	StringContainer FileResult::toString() const
	{

		std::string fileCodeString;

		switch (pImpl->_fileCode) {
		case FileErrorCode::FileNotFound:		fileCodeString = "File not found. "; break;
		case FileErrorCode::PermissionDenied:	fileCodeString = "Permission denied. "; break;
		case FileErrorCode::NotAFile:			fileCodeString = "Path is not a regular file. "; break;
		case FileErrorCode::ReadFailure:		fileCodeString = "Failed to read file. "; break;
		case FileErrorCode::IncorrectFileType:	fileCodeString = "File is not expected type. "; break;
		case FileErrorCode::FileTooLarge:		fileCodeString = "File exceeds size limits. "; break;
		case FileErrorCode::FileTooSmall:		fileCodeString = "File does not meet size requirement. "; break;
		case FileErrorCode::UnknownErrorCode:	fileCodeString = "Unknown Error. "; break;
		case FileErrorCode::WriteFailure:		fileCodeString = "Failed to write to file. "; break;
		case FileErrorCode::Success:			fileCodeString = "Success. "; break;
		default: fileCodeString = "No Error Code Found. See fileCode - FileErrorCode Enum. ";
		}

		std::string em;
		if (!pImpl->_message.empty()) em = std::format("({})", pImpl->_message.c_str());
		else em = "";

		StringContainer output("FILE RESULT: [");
		output += pImpl->_path.strContainer();
		output += "]: ";
		output += fileCodeString.c_str();
		output += em.c_str();
		return output;
	}

	bool FileResult::hasError() const
	{
		return pImpl->_fileCode != FileErrorCode::Success;
	}

	FileResult::FileErrorCode FileResult::fileErrorCode() const
	{
		return pImpl->_fileCode;
	}

	PathContainer FileResult::filePath() const
	{
		return pImpl->_path;
	}

	const char* FileResult::operationMessage() const
	{
		return pImpl->_message.c_str();
	}

	FileResult::~FileResult()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}


	FileResult::FileResult()
	{
		pImpl = new Impl;
	}

	FileResult::FileResult(const FileErrorCode fileCode, PathContainer&& path, const char* errorMessage) noexcept : FileResult()
	{
		pImpl->_fileCode = fileCode;
		pImpl->_path = path;
		pImpl->_message = errorMessage;
	}

	FileResult::FileResult(const FileErrorCode& fileCode, const PathContainer& path, const char* errorMessage) : FileResult()
	{
		pImpl->_fileCode = fileCode;
		pImpl->_message = errorMessage;
		pImpl->_path = path;
	}

	FileResult::FileResult(const FileErrorCode& fileCode, const PathContainer& path, const StringContainer& errorMessage) : FileResult(fileCode, path, errorMessage.c_str()) {}

	FileResult::FileResult(const FileResult& source) : FileResult()
	{
		*this = source;
	}

	FileResult& FileResult::operator=(const FileResult& source)
	{
		if (this != &source)
		{
			pImpl->_fileCode = source.pImpl->_fileCode;
			pImpl->_message = source.pImpl->_message;
			pImpl->_path = source.pImpl->_path;
		}
		return *this;
	}

	struct FileBufferResult::Impl
	{
		FileResult _result;
	};

	FileBufferResult::FileBufferResult() : pImpl(new Impl()) { }

	FileBufferResult::~FileBufferResult()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	void FileBufferResult::setResult(const FileResult& result) const
	{
		pImpl->_result = result;
	}
	const FileResult& FileBufferResult::result() const
	{
		return pImpl->_result;
	}
}
