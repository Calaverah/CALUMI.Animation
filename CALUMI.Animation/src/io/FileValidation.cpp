//Copyright � 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "io/FileValidation.h"
#include "internalplatform.h"
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
	static Utilities::FileResult _CommonValidation(const Utilities::PathContainer& inputPath)
	{
		std::error_code ec;
		Utilities::FileResult output;

		//Checking if file exists
		if (!std::filesystem::exists(inputPath.w_str()))
		{
			output = { Utilities::FileResult::FileErrorCode::FileNotFound, inputPath.w_str(), ec.message().c_str() };
			return output;
		}

		//Checking if file is file
		if (!std::filesystem::is_regular_file(inputPath.w_str(), ec))
		{
			output = { Utilities::FileResult::FileErrorCode::NotAFile, inputPath.w_str(), ec.message().c_str() };
			return output;
		}

		//Check if file is empty
		const auto size = std::filesystem::file_size(inputPath.w_str(), ec);
		if (ec)
		{
			output = { Utilities::FileResult::FileErrorCode::ReadFailure, inputPath.w_str(), ec.message().c_str() };
			return output;
		}
		if (size < 4) //no file should be less than 4 bytes, if so it is too small to contain any useful information worth reading
		{
			output = { Utilities::FileResult::FileErrorCode::FileTooSmall, inputPath.w_str(), ec.message().c_str() };
			return output;
		}

#ifdef _WIN32
		const std::ifstream file(inputPath.w_str(), std::ios::binary);
#else
        const std::ifstream file(inputPath.c_str(), std::ios::binary);
#endif

        if (!file)
		{
			output = { Utilities::FileResult::FileErrorCode::PermissionDenied, inputPath.w_str(), ec.message().c_str() };
			return output;
		}

		//additional common file checks go here

		output = { Utilities::FileResult::FileErrorCode::Success, inputPath.w_str(), "" };
		//finally we return that the check was a success
		return output;
	}

	/// <summary>
	/// This method will return a buffer for binary file reading, or an error code to pass along. To be used only after _CommonValidation
	/// </summary>
	/// <param name="inputPath"></param>
	/// <returns></returns>
	static Utilities::FileBufferResult _GetBuffer(const Utilities::PathContainer& inputPath)
	{
		const std::error_code ec;
		const auto size = std::filesystem::file_size(inputPath.w_str());

#ifdef _WIN32
        std::ifstream file(inputPath.w_str(), std::ios::binary);
#else
        std::ifstream file(inputPath.c_str(), std::ios::binary);
#endif

		Utilities::FileBufferResult output;
		output.resize(size);

		if (!file.read(output.data(), static_cast<long long>(output.size())))
		{
			
			output.setResult({Utilities::FileResult::FileErrorCode::ReadFailure, inputPath.w_str(), ec.message().c_str()});
			return output;
		}
		
		output.setResult({ Utilities::FileResult::FileErrorCode::Success, inputPath.w_str(), "" });

		return output;
	}

	static std::expected<bool, Utilities::FileResult> _FileExtValidation(const Utilities::PathContainer& inputPath, const Utilities::StringList& fileExtensions, const bool allowFiles)
	{
		const std::error_code ec;

		//Ensure input extension isn't empty for some reason
		if (!inputPath.has_extension())
			return std::unexpected(Utilities::FileResult{ Utilities::FileResult::FileErrorCode::IncorrectFileType, inputPath.w_str(), ec.message().c_str() });

		//If ban list is empty, allow the file to move forward
		if (fileExtensions.empty() && !allowFiles)
			return true;

		//If approve list is empty, the file cannot be approved
		if (fileExtensions.empty())
			return std::unexpected(Utilities::FileResult{ Utilities::FileResult::FileErrorCode::IncorrectFileType, inputPath.w_str(), ec.message().c_str() });


		//Check each extension in the list, if a match is found, determine whether to approve or reject
		for (int i = 0; i < fileExtensions.size(); i++)
		{
			if (Utilities::PathContainer subjectAsPath(std::format("filteringFileName{}",fileExtensions.c_str(i)).c_str()); inputPath.extension() == subjectAsPath.extension() && inputPath.extension() != ".") //check to see if extensions match as well if there is a non "." value
			{
				//A match has been found, determine response
				if (allowFiles)
					return true;

				return std::unexpected(Utilities::FileResult{ Utilities::FileResult::FileErrorCode::IncorrectFileType, inputPath.w_str(), ec.message().c_str() });
			}
		}

		if (!allowFiles) {
			return true;
		}
		return std::unexpected(Utilities::FileResult{ Utilities::FileResult::FileErrorCode::IncorrectFileType, inputPath.w_str(), ec.message().c_str() });
	}

	static std::expected<bool, Utilities::FileResult> _ValidateFileSize(const Utilities::PathContainer& inputPath, const unsigned int& minFileSize, const unsigned long long& maxFileSize)
	{
		std::error_code ec;
		const auto size = std::filesystem::file_size(inputPath.w_str(), ec);

		//If max size is 0, then we skip that step
		if (maxFileSize != 0 && size > maxFileSize)
			return std::unexpected(Utilities::FileResult{ Utilities::FileResult::FileErrorCode::FileTooLarge, inputPath.w_str(), ec.message().c_str() });

		if (size < minFileSize)
			return std::unexpected(Utilities::FileResult{ Utilities::FileResult::FileErrorCode::FileTooSmall, inputPath.w_str(), ec.message().c_str() });

		return true;
	}

#pragma endregion

	//Input Path Only
	Utilities::FileBufferResult ValidateFile(const Utilities::PathContainer& inputPath)
	{
		if (const auto result = _CommonValidation(inputPath); result.hasError())
		{
			Utilities::FileBufferResult tempOutput;
			tempOutput.setResult(result);
			return tempOutput;
		}

		return _GetBuffer(inputPath);
	}

	
	Utilities::FileBufferResult ValidateFile(const Utilities::PathContainer& inputPath, const Utilities::StringList& fileExtensions, const bool allowFiles)
	{
		if (const auto result = _CommonValidation(inputPath); result.hasError())
		{
			Utilities::FileBufferResult tempOutput;
			tempOutput.setResult(result);
			return tempOutput;
		}

		//Check file extensions
		if (auto extResult = _FileExtValidation(inputPath, fileExtensions, allowFiles); !extResult.has_value())
		{
			Utilities::FileBufferResult tempOutput;
			tempOutput.setResult(extResult.error());
			return tempOutput;
		}

		return _GetBuffer(inputPath);
	}

	Utilities::FileBufferResult ValidateFile(const Utilities::PathContainer& inputPath, const Utilities::StringList& fileExtensions, const unsigned int minFileSize, const unsigned long long maxFileSize, const bool allowFiles)
	{
		if (const auto result = _CommonValidation(inputPath); result.hasError())
		{
			Utilities::FileBufferResult tempOutput;
			tempOutput.setResult(result);
			return tempOutput;
		}

		//Check file extension
		if (auto extResult = _FileExtValidation(inputPath, fileExtensions, allowFiles); !extResult.has_value())
		{
			Utilities::FileBufferResult tempOutput;
			tempOutput.setResult(extResult.error());
			return tempOutput;
		}

		//Check file size
		if (auto sizeResult = _ValidateFileSize(inputPath, minFileSize, maxFileSize); !sizeResult.has_value())
		{
			Utilities::FileBufferResult tempOutput;
			tempOutput.setResult(sizeResult.error());
			return tempOutput;
		}

		return _GetBuffer(inputPath);
	}

	Utilities::FileBufferResult ValidateFile(const Utilities::PathContainer& inputPath, const unsigned int minFileSize, const unsigned long long maxFileSize)
	{
		if (const auto result = _CommonValidation(inputPath); result.hasError())
		{
			Utilities::FileBufferResult tempOutput;
			tempOutput.setResult(result);
			return tempOutput;
		}

		//Check file size
		if (auto sizeResult = _ValidateFileSize(inputPath, minFileSize, maxFileSize); !sizeResult.has_value())
		{
			Utilities::FileBufferResult tempOutput;
			tempOutput.setResult(sizeResult.error());
			return tempOutput;
		}

		return _GetBuffer(inputPath);
	}

	Utilities::FileResult WriteToBinaryFile(const Utilities::PathContainer& outputPath, Utilities::BufferObject& buffer)
	{
		std::ofstream file;
		std::error_code ec;

		if (std::filesystem::path parentPath = std::filesystem::path(outputPath.w_str()).parent_path(); !std::filesystem::exists(parentPath))
		{
			std::filesystem::create_directories(parentPath);
		}

		if (std::filesystem::exists(outputPath.w_str()))
		{

			if (!std::filesystem::is_regular_file(outputPath.w_str(), ec))
			{
				Utilities::FileResult tempOutput(Utilities::FileResult::FileErrorCode::NotAFile, outputPath.w_str(), ec.message().c_str());
				return tempOutput; //We found an entry with this _path and it is not a file to be written to
			}

#ifdef _WIN32
            file = std::ofstream(outputPath.w_str(), std::ios::binary);
#else
            file = std::ofstream(outputPath.c_str(), std::ios::binary);
#endif

            if(!file.is_open())
			{
				Utilities::FileResult tempOutput(Utilities::FileResult::FileErrorCode::PermissionDenied, outputPath.w_str(), ec.message().c_str());
				return tempOutput;
			}
		}
		else
		{
#ifdef _WIN32
			file = std::ofstream(outputPath.w_str(), std::ios::binary);
#else
            file = std::ofstream(outputPath.c_str(), std::ios::binary);
#endif
            if (!file.is_open())
			{
				Utilities::FileResult tempOutput(Utilities::FileResult::FileErrorCode::UnknownErrorCode, outputPath.w_str(), ec.message().c_str());
				return tempOutput;
			}
		}

		file.write(buffer.data(), static_cast<long long>(buffer.size()));
		if (!file)
		{
			file.close();
			Utilities::FileResult tempOutput(Utilities::FileResult::FileErrorCode::WriteFailure, outputPath.w_str(), ec.message().c_str());
			return tempOutput;
		}
		file.close();
		return Utilities::FileResult(Utilities::FileResult::FileErrorCode::Success, outputPath.w_str(), "Output Successful!");
	}

	Utilities::FileResult WriteToBinaryFile(const Utilities::PathContainer& outputPath, const Utilities::StringContainer& buffer)
	{
		Utilities::BufferObject vBuffer;
		vBuffer.reserve(buffer.length(true));
		for (uint64_t i = 0; i < buffer.length(); i++)
		{
			vBuffer.push_back(buffer.at(i));
		}
		vBuffer.push_back('\0');

		return WriteToBinaryFile(outputPath, vBuffer);
	}
	
}