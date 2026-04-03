//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Utilities.h"
#include "io/FileError.h"

namespace CALUMI {
	/**
	 * @brief Provides an interface for a class to be written/read from a given path
	 */
	class CALUMIANIMATION_API IReadWritable
	{
		/**
		 * @brief Reads in a file from a given file path
		 * @param inputFilePath 
		 * @return Expected container with error status and success result
		 */
		virtual Utilities::ExpectedContainer<bool, Utilities::FileError> ReadFromFile(Utilities::PathContainer& inputFilePath) = 0;
		/**
		 * @brief Writes to a file at a given file path
		 * @param outputFilePath
		 * @return Expected container with error status and file result
		 */
		virtual Utilities::ExpectedContainer<Utilities::StringContainer, Utilities::FileError> WriteToFile(Utilities::PathContainer& outputFilePath) = 0;
	};
}

