//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "FileResult.h"


namespace CALUMI{
	/**
	 * @brief Provides a simplified way to read in a file with builtin error handling
	 *
	 * @defgroup c_file_validation FileIO API
	 * @{
	 */

	/**
	 * @brief Checks the file with the standard validation criteria
	 * @details Checks that the file is not a directory, that it exists, and can be accessed
	 * @param inputPath 
	 * @return Expected container with the value being a char based buffer of the file if the operation was a success, and/or an error code
	 */
	Utilities::FileBufferResult CALUMIANIMATION_API ValidateFile(const Utilities::PathContainer& inputPath);
	
	/**
	 * @brief Checks the file with the standard validation criteria
	 * @param inputPath 
	 * @param fileExtensions File extensions to check
	 * @param allowFiles If true the list of extensions acts as an allowed extension list, if false then acts as an exclusion list
	 * @return Expected container with the value being a char based buffer of the file if the operation was a success, and/or an error code
	 */
	Utilities::FileBufferResult CALUMIANIMATION_API ValidateFile(const Utilities::PathContainer& inputPath, const Utilities::StringList& fileExtensions, bool allowFiles = true);

	/**
	 * @brief This method checks the file's size as well as validates the file's extension against a list.
	 * @param inputPath 
	 * @param fileExtensions File extensions to check
	 * @param minFileSize File must be this size or larger
	 * @param maxFileSize File must be this size or smaller, 0 will default the max value of 0xFFFFFFFFFFFFFFFF ULL
	 * @param allowFiles If true the list of extensions acts as an allowed extension list, if false then acts as an exclusion list
	 * @return 
	 */
	Utilities::FileBufferResult CALUMIANIMATION_API ValidateFile(const Utilities::PathContainer& inputPath, const Utilities::StringList& fileExtensions, unsigned int minFileSize, unsigned long long maxFileSize = 0, bool allowFiles = true);
	Utilities::FileBufferResult CALUMIANIMATION_API ValidateFile(const Utilities::PathContainer& inputPath, unsigned int minFileSize, unsigned long long maxFileSize = 0);

	/**
	 * @brief Writes a buffer to a binary file
	 * @param outputPath The _path to write to
	 * @param buffer char based buffer or string container to write from
	 * @return Success/Error status message
	 */
	Utilities::FileResult CALUMIANIMATION_API WriteToBinaryFile(const Utilities::PathContainer& outputPath, Utilities::BufferObject& buffer);
	Utilities::FileResult CALUMIANIMATION_API WriteToBinaryFile(const Utilities::PathContainer& outputPath, const Utilities::StringContainer& buffer);

	/** 
	 * @} 
	 */
}

