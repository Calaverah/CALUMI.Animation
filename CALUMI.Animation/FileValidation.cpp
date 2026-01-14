//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com
#include "pch.h"
#include "FileValidation.h"
#include <string>
#include <filesystem>
#include <expected>
#include <fstream>
namespace CALUMI {


#pragma region PrivateStaticFunctions

	/// <summary>
	/// This method will check for common validations to be used by the validate file method and it's overloads. It will return a meaningless bool, or an error code to pass along.
	/// </summary>
	/// <param name="inputPath"></param>
	/// <returns></returns>
	static Utilities::ExpectedContainer<bool, FileError> _CommonValidation(const Utilities::PathContainer& inputPath)
	{
		std::error_code ec;
		Utilities::ExpectedContainer<bool, FileError> output;

		//Checking if file exists
		if (!std::filesystem::exists(inputPath.w_str()))
		{
			output.SetErrorValue(FileError{ FileErrorCode::FileNotFound, inputPath.w_str(), ec.message().c_str() });
			return output;
		}

		//Checking if file is file
		if (!std::filesystem::is_regular_file(inputPath.w_str(), ec))
		{
			output.SetErrorValue(FileError{ FileErrorCode::NotAFile, inputPath.w_str(), ec.message().c_str() });
			return output;
		}

		//Check if file is empty
		const auto size = std::filesystem::file_size(inputPath.w_str(), ec);
		if (ec)
		{
			output.SetErrorValue(FileError{ FileErrorCode::ReadFailure, inputPath.w_str(), ec.message().c_str() });
			return output;
		}
		if (size < 4) //no file should be less than 4 bytes, if so it is too small to contain any useful information worth reading
		{
			output.SetErrorValue(FileError{ FileErrorCode::FileTooSmall, inputPath.w_str(), ec.message().c_str() });
			return output;
		}

		std::ifstream file(inputPath.w_str(), std::ios::binary);
		if (!file)
		{
			output.SetErrorValue(FileError{ FileErrorCode::PermissionDenied, inputPath.w_str(), ec.message().c_str() });
			return output;
		}

		//additional common file checks go here

		output.SetValue(true);
		//finally we return that the check was a success
		return output;
	}

	/// <summary>
	/// This method will return a buffer for binary file reading, or an error code to pass along. To be used only after _CommonValidation
	/// </summary>
	/// <param name="inputPath"></param>
	/// <returns></returns>
	static Utilities::ExpectedContainer<Utilities::VectorContainer<char>, FileError> _GetBuffer(const Utilities::PathContainer& inputPath)
	{
		std::error_code ec;
		const auto size = std::filesystem::file_size(inputPath.w_str());
		std::ifstream file(inputPath.w_str(), std::ios::binary);

		Utilities::VectorContainer<char> buffer(static_cast<size_t>(size));

		if (!file.read(buffer.data(), buffer.size()))
		{
			Utilities::ExpectedContainer < Utilities::VectorContainer<char>, FileError> tempOutput;
			tempOutput.SetErrorValue(FileError{ FileErrorCode::ReadFailure, inputPath.w_str(), ec.message().c_str() });
			return tempOutput;
		}
		
		return Utilities::ExpectedContainer<Utilities::VectorContainer<char>,FileError>(buffer);
	}

	static std::expected<bool, FileError> _FileExtValidation(const Utilities::PathContainer& inputPath, const Utilities::VectorContainer<Utilities::StringContainer>& fileExtensions, bool allowFiles)
	{
		std::error_code ec;

		//Ensure input extension isn't empty for some reason
		if (!inputPath.has_extension())
			return std::unexpected(FileError{ FileErrorCode::IncorrectFileType, inputPath.w_str(), ec.message().c_str() });

		//If ban list is empty, allow the file to move forward
		if (fileExtensions.empty() && !allowFiles)
			return true;
		//If approve list is empty, the file cannot be approved
		else if (fileExtensions.empty())
			return std::unexpected(FileError{ FileErrorCode::IncorrectFileType, inputPath.w_str(), ec.message().c_str() });


		//Check each extension in the list, if a match is found, determine whether to approve or reject
		for (int i = 0; i < fileExtensions.size(); i++)
		{
			Utilities::PathContainer subjectAsPath(std::format("filteringFileName{}",fileExtensions.at(i).c_str()).c_str());
			if ((inputPath.extension() == subjectAsPath.extension()) && (inputPath.extension() != ".")) //check to see if extensions match as well if there is a non "." value
			{
				//A match has been found, determine response
				if (allowFiles)
					return true;
				else
					return std::unexpected(FileError{ FileErrorCode::IncorrectFileType, inputPath.w_str(), ec.message().c_str() });
			}
		}

		if (!allowFiles) {
			return true;
		}
		return std::unexpected(FileError{ FileErrorCode::IncorrectFileType, inputPath.w_str(), ec.message().c_str() });
	}

	static std::expected<bool, FileError> _ValidateFileSize(const Utilities::PathContainer& inputPath, unsigned int& minFileSize, unsigned long long& maxFileSize)
	{
		std::error_code ec;
		const auto size = std::filesystem::file_size(inputPath.w_str(), ec);

		//If max size is 0, then we skip that step
		if (maxFileSize != 0 && size > maxFileSize)
			return std::unexpected(FileError{ FileErrorCode::FileTooLarge, inputPath.w_str(), ec.message().c_str() });

		if (size < minFileSize)
			return std::unexpected(FileError{ FileErrorCode::FileTooSmall, inputPath.w_str(), ec.message().c_str() });

		return true;
	}

#pragma endregion

	//Input Path Only
	Utilities::ExpectedContainer<Utilities::VectorContainer<char>, FileError> ValidateFile(const Utilities::PathContainer& inputPath)
	{
		auto result = _CommonValidation(inputPath);
		if (!result.has_value())
		{
			Utilities::ExpectedContainer < Utilities::VectorContainer<char>, FileError> tempOutput;
			tempOutput.SetErrorValue(result.error());
			return tempOutput;
		}

		return _GetBuffer(inputPath);
	}

	/// <summary>
	/// Check if this file is valid, is the expected file extension, and return the buffer.
	/// </summary>
	/// <param name="inputPath"></param>
	/// <param name="fileExtensions">A string vector of approved and forbidden file extensions to check for. Ex: {.png, .jpg, .jpeg}</param>
	/// <param name="allowFiles">Declare if the file should be approved if its extension is listed, or denied.
	/// <returns></returns>
	Utilities::ExpectedContainer<Utilities::VectorContainer<char>, FileError> ValidateFile(const Utilities::PathContainer& inputPath, const Utilities::VectorContainer<Utilities::StringContainer> fileExtensions, bool allowFiles)
	{
		auto result = _CommonValidation(inputPath);
		if (!result.has_value())
		{
			Utilities::ExpectedContainer < Utilities::VectorContainer<char>, FileError> tempOutput;
			tempOutput.SetErrorValue(result.error());
			return tempOutput;
		}

		//Check file extensions
		auto extResult = _FileExtValidation(inputPath, fileExtensions, allowFiles);
		if (!extResult.has_value())
		{
			Utilities::ExpectedContainer < Utilities::VectorContainer<char>, FileError> tempOutput;
			tempOutput.SetErrorValue(extResult.error());
			return tempOutput;
		}

		return _GetBuffer(inputPath);
	}

	/// <summary>
	/// This method checks the file's size as well as validates the file's extension against a list.
	/// </summary>
	/// <param name="inputPath"></param>
	/// <param name="minFileSize">4 by default, no file should be less than 4 bytes long unless specified</param>
	/// <param name="maxFileSize">0 by default, if set to 0 this parameter will be skipped</param>
	/// <param name="fileExtensions">A string vector of approved and forbidden file extensions to check for. Ex: {.png, .jpg, .jpeg}</param>
	/// <param name="allowFiles">Declare if the file should be approved if its extension is listed, or denied.
	/// <returns></returns>
	Utilities::ExpectedContainer<Utilities::VectorContainer<char>, FileError> ValidateFile(const Utilities::PathContainer& inputPath, const Utilities::VectorContainer<Utilities::StringContainer> fileExtensions, unsigned int minFileSize, unsigned long long maxFileSize, bool allowFiles)
	{
		auto result = _CommonValidation(inputPath);
		if (!result.has_value())
		{
			Utilities::ExpectedContainer < Utilities::VectorContainer<char>, FileError> tempOutput;
			tempOutput.SetErrorValue(result.error());
			return tempOutput;
		}

		//Check file extension
		auto extResult = _FileExtValidation(inputPath, fileExtensions, allowFiles);
		if (!extResult.has_value()) 
		{
			Utilities::ExpectedContainer < Utilities::VectorContainer<char>, FileError> tempOutput;
			tempOutput.SetErrorValue(extResult.error());
			return tempOutput;
		}

		//Check file size
		auto sizeResult = _ValidateFileSize(inputPath, minFileSize, maxFileSize);
		if (!sizeResult.has_value())
		{
			Utilities::ExpectedContainer < Utilities::VectorContainer<char>, FileError> tempOutput;
			tempOutput.SetErrorValue(sizeResult.error());
			return tempOutput;
		}

		return _GetBuffer(inputPath);
	}

	/// <summary>
	/// This method checks the file's size.
	/// </summary>
	/// <param name="inputPath"></param>
	/// <param name="minFileSize">4 by default, no file should be less than 4 bytes long unless specified</param>
	/// <param name="maxFileSize">0 by default, if set to 0 this parameter will be skipped</param>
	/// <returns></returns>
	Utilities::ExpectedContainer<Utilities::VectorContainer<char>, FileError> ValidateFile(const Utilities::PathContainer& inputPath, unsigned int minFileSize, unsigned long long maxFileSize)
	{
		auto result = _CommonValidation(inputPath);
		if (!result.has_value())
		{
			Utilities::ExpectedContainer < Utilities::VectorContainer<char>, FileError> tempOutput;
			tempOutput.SetErrorValue(result.error());
			return tempOutput;
		}

		//Check file size
		auto sizeResult = _ValidateFileSize(inputPath, minFileSize, maxFileSize);
		if (!sizeResult.has_value())
		{
			Utilities::ExpectedContainer < Utilities::VectorContainer<char>, FileError> tempOutput;
			tempOutput.SetErrorValue(sizeResult.error());
			return tempOutput;
		}

		return _GetBuffer(inputPath);
	}

	Utilities::ExpectedContainer<Utilities::StringContainer, FileError> WriteToBinaryFile(const Utilities::PathContainer& outputPath, Utilities::VectorContainer<char>& buffer)
	{
		std::ofstream file;
		std::error_code ec;

		std::filesystem::path parentPath = std::filesystem::path(outputPath.w_str()).parent_path();
		if (!std::filesystem::exists(parentPath))
		{
			std::filesystem::create_directories(parentPath);
		}

		if (std::filesystem::exists(outputPath.w_str()))
		{

			if (!std::filesystem::is_regular_file(outputPath.w_str(), ec))
			{
				Utilities::ExpectedContainer<Utilities::StringContainer, FileError> tempOutput;
				tempOutput.SetErrorValue(FileError{ FileErrorCode::NotAFile, outputPath.w_str(), ec.message().c_str() });
				return tempOutput; //We found an entry with this path and it is not a file to be written to
			}

			file = std::ofstream(outputPath.w_str(), std::ios::binary);
			if(!file.is_open())
			{
				Utilities::ExpectedContainer<Utilities::StringContainer, FileError> tempOutput;
				tempOutput.SetErrorValue(FileError{ FileErrorCode::PermissionDenied, outputPath.w_str(), ec.message().c_str() });
				return tempOutput;
			}
		}
		else
		{
			file = std::ofstream(outputPath.w_str(), std::ios::binary);
			if (!file.is_open())
			{
				Utilities::ExpectedContainer<Utilities::StringContainer, FileError> tempOutput;
				tempOutput.SetErrorValue(FileError{ FileErrorCode::UnknownErrorCode, outputPath.w_str(), ec.message().c_str() });
				return tempOutput;
			}
		}

		file.write(buffer.data(), buffer.size());
		if (!file)
		{
			file.close();
			Utilities::ExpectedContainer<Utilities::StringContainer, FileError> tempOutput;
			tempOutput.SetErrorValue(FileError{ FileErrorCode::WriteFailure, outputPath.w_str(), ec.message().c_str() });
			return tempOutput;
		}
		file.close();
		return Utilities::ExpectedContainer<Utilities::StringContainer, FileError>(Utilities::StringContainer(std::format("Output successful, written to {}", outputPath.c_str()).c_str()));
	}

	Utilities::ExpectedContainer<Utilities::StringContainer, FileError> WriteToBinaryFile(const Utilities::PathContainer& outputPath, const Utilities::StringContainer& buffer)
	{
		Utilities::VectorContainer<char> vBuffer;
		vBuffer.reserve(buffer.Length(true));
		for (size_t i = 0; i < buffer.Length(); i++)
		{
			vBuffer.push_back(buffer.at(i));
		}
		vBuffer.push_back('\0');

		return WriteToBinaryFile(outputPath, vBuffer);
	}
	
}