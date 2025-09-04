//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "FileValidation.h"
namespace CALUMI {
	//Input Path Only
	std::expected<std::vector<char>, FileError> ValidateFile(const std::filesystem::path& inputPath)
	{
		auto result = priv::_CommonValidation(inputPath);
		if (!result.has_value()) return std::unexpected((result.error()));

		return priv::_GetBuffer(inputPath);
	}

	/// <summary>
	/// Check if this file is valid, is the expected file extension, and return the buffer.
	/// </summary>
	/// <param name="inputPath"></param>
	/// <param name="fileExtensions">A string vector of approved and forbidden file extensions to check for. Ex: {.png, .jpg, .jpeg}</param>
	/// <param name="allowFiles">Declare if the file should be approved if its extension is listed, or denied.
	/// <returns></returns>
	std::expected<std::vector<char>, FileError> ValidateFile(const std::filesystem::path& inputPath, const std::vector<std::string> fileExtensions, bool allowFiles = true)
	{
		auto result = priv::_CommonValidation(inputPath);
		if (!result.has_value()) return std::unexpected((result.error()));

		//Check file extensions
		auto extResult = priv::_FileExtValidation(inputPath, fileExtensions, allowFiles);
		if (!extResult.has_value()) return std::unexpected((extResult.error()));

		return priv::_GetBuffer(inputPath);
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
	std::expected<std::vector<char>, FileError> ValidateFile(const std::filesystem::path& inputPath, const std::vector<std::string> fileExtensions, unsigned int minFileSize, unsigned long long maxFileSize = 0, bool allowFiles = true)
	{
		auto result = priv::_CommonValidation(inputPath);
		if (!result.has_value()) return std::unexpected((result.error()));

		//Check file extension
		auto extResult = priv::_FileExtValidation(inputPath, fileExtensions, allowFiles);
		if (!extResult.has_value()) return std::unexpected((extResult.error()));

		//Check file size
		auto sizeResult = priv::_ValidateFileSize(inputPath, minFileSize, maxFileSize);
		if (!sizeResult.has_value()) return std::unexpected((sizeResult.error()));

		return priv::_GetBuffer(inputPath);
	}

	/// <summary>
	/// This method checks the file's size.
	/// </summary>
	/// <param name="inputPath"></param>
	/// <param name="minFileSize">4 by default, no file should be less than 4 bytes long unless specified</param>
	/// <param name="maxFileSize">0 by default, if set to 0 this parameter will be skipped</param>
	/// <returns></returns>
	std::expected<std::vector<char>, FileError> ValidateFile(const std::filesystem::path& inputPath, unsigned int minFileSize, unsigned long long maxFileSize = 0)
	{
		auto result = priv::_CommonValidation(inputPath);
		if (!result.has_value()) return std::unexpected((result.error()));

		//Check file size
		auto sizeResult = priv::_ValidateFileSize(inputPath, minFileSize, maxFileSize);
		if (!sizeResult.has_value()) return std::unexpected((sizeResult.error()));

		return priv::_GetBuffer(inputPath);
	}

	std::expected<std::string, FileError> WriteToBinaryFile(const std::filesystem::path& outputPath, std::vector<char>& buffer)
	{
		std::ofstream file;
		std::error_code ec;

		if (std::filesystem::exists(outputPath))
		{
			

			if(!std::filesystem::is_regular_file(outputPath, ec))
				return std::unexpected(FileError{ FileErrorCode::NotAFile, outputPath, ec.message() }); //We found an entry with this path and it is not a file to be written to

			file = std::ofstream(outputPath, std::ios::binary);
			if(!file.is_open())
				return std::unexpected(FileError{ FileErrorCode::PermissionDenied, outputPath, ec.message() });
		}
		else
		{
			file = std::ofstream(outputPath, std::ios::binary);
			if (!file.is_open())
				return std::unexpected(FileError{ FileErrorCode::UnknownErrorCode, outputPath, ec.message() });
		}

		file.write(buffer.data(), buffer.size());
		if (!file)
		{
			file.close();
			return std::unexpected(FileError{ FileErrorCode::WriteFailure, outputPath, ec.message() });
		}
		file.close();
		return std::format("Output successful, written to {}", outputPath.string());
	}

	std::expected<std::string, FileError> WriteToBinaryFile(const std::filesystem::path& outputPath, const std::string& buffer)
	{
		std::vector<char> vBuffer(buffer.begin(),buffer.end());
		vBuffer.push_back('\0');

		return WriteToBinaryFile(outputPath, vBuffer);
	}

	namespace priv {
		/// <summary>
		/// This method will check for common validations to be used by the validate file method and it's overloads. It will return a meaningless bool, or an error code to pass along.
		/// </summary>
		/// <param name="inputPath"></param>
		/// <returns></returns>
		std::expected<bool, FileError> _CommonValidation(const std::filesystem::path& inputPath)
		{
			std::error_code ec;

			//Checking if file exists
			if (!std::filesystem::exists(inputPath))
				return std::unexpected(FileError{ FileErrorCode::FileNotFound, inputPath, ec.message() });

			//Checking if file is file
			if (!std::filesystem::is_regular_file(inputPath, ec))
				return std::unexpected(FileError{ FileErrorCode::NotAFile, inputPath, ec.message() });

			//Check if file is empty
			const auto size = std::filesystem::file_size(inputPath, ec);
			if (ec)
				return std::unexpected(FileError{ FileErrorCode::ReadFailure, inputPath, ec.message() });
			if (size < 4) //no file should be less than 4 bytes, if so it is too small to contain any useful information worth reading
				return std::unexpected(FileError{ FileErrorCode::FileTooSmall, inputPath, ec.message() });

			std::ifstream file(inputPath, std::ios::binary);
			if (!file)
				return std::unexpected(FileError{ FileErrorCode::PermissionDenied, inputPath, ec.message() });

			//additional common file checks go here


			//finally we return that the check was a success
			return true;
		}

		/// <summary>
		/// This method will return a buffer for binary file reading, or an error code to pass along. To be used only after _CommonValidation
		/// </summary>
		/// <param name="inputPath"></param>
		/// <returns></returns>
		std::expected<std::vector<char>, FileError> _GetBuffer(const std::filesystem::path& inputPath)
		{
			std::error_code ec;
			const auto size = std::filesystem::file_size(inputPath);
			std::ifstream file(inputPath, std::ios::binary);

			std::vector<char> buffer(static_cast<size_t>(size));

			if (!file.read(buffer.data(), buffer.size()))
				return std::unexpected(FileError{ FileErrorCode::ReadFailure, inputPath, ec.message() });

			return buffer;
		}

		std::expected<bool, FileError> _FileExtValidation(const std::filesystem::path& inputPath, const std::vector<std::string>& fileExtensions, bool allowFiles)
		{
			std::error_code ec;

			//Ensure input extension isn't empty for some reason
			if (!inputPath.has_extension())
				return std::unexpected(FileError{ FileErrorCode::IncorrectFileType, inputPath, ec.message() });

			//If ban list is empty, allow the file to move forward
			if (fileExtensions.empty() && !allowFiles)
				return true;
			//If approve list is empty, the file cannot be approved
			else if (fileExtensions.empty())
				return std::unexpected(FileError{ FileErrorCode::IncorrectFileType, inputPath, ec.message() });


			//Check each extension in the list, if a match is found, determine whether to approve or reject
			for (std::string subject : fileExtensions)
			{
				std::filesystem::path subjectAsPath(std::string("filteringFileName") + subject);
				if ((inputPath.extension() == subjectAsPath.extension()) && (inputPath.extension() != ".")) //check to see if extensions match as well if there is a non "." value
				{
					//A match has been found, determine response
					if (allowFiles)
						return true;
					else
						return std::unexpected(FileError{ FileErrorCode::IncorrectFileType, inputPath, ec.message() });
				}
			}

			if (!allowFiles) {
				return true;
			}
			return std::unexpected(FileError{ FileErrorCode::IncorrectFileType, inputPath, ec.message() });
		}

		std::expected<bool, FileError> _ValidateFileSize(const std::filesystem::path& inputPath, unsigned int& minFileSize, unsigned long long& maxFileSize)
		{
			std::error_code ec;
			const auto size = std::filesystem::file_size(inputPath, ec);

			//If max size is 0, then we skip that step
			if (maxFileSize != 0 && size > maxFileSize)
				return std::unexpected(FileError{ FileErrorCode::FileTooLarge, inputPath, ec.message() });

			if (size < minFileSize)
				return std::unexpected(FileError{ FileErrorCode::FileTooSmall, inputPath, ec.message() });

			return true;
		}
	}
}