//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Utilities.h"
#include "io/FileResult.h"

namespace CALUMI {
	/**
	 * @brief Provides an interface for a class to be written/read from a given _path
	 */
	class CALUMIANIMATION_API IReadWritable
	{
	public:
		virtual ~IReadWritable() = default;

	public:
		/**
		 * @brief Reads in a file from a given file path
		 * @param inputFilePath 
		 * @return Expected container with error status and success result
		 */
        virtual Utilities::FileResult readFromFile(Utilities::PathContainer& inputFilePath) = 0;
		/**
		 * @brief Reads in a file from a given file path
		 * @param inputFilePath
		 * @return Expected container with error status and success result
		 */
		virtual Utilities::FileResult readFromFile(Utilities::PathContainer&& inputFilePath) = 0;
		/**
		 * @brief Writes to a file at a given file path
		 * @param outputFilePath
		 * @return Expected container with error status and file result
		 */
        virtual Utilities::FileResult writeToFile(Utilities::PathContainer& outputFilePath) = 0;
		/**
		 * @brief Writes to a file at a given file path
		 * @param outputFilePath
		 * @return Expected container with error status and file result
		 */
		virtual Utilities::FileResult writeToFile(Utilities::PathContainer&& outputFilePath) = 0;
	};
}

