//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Utilities.h"

namespace CALUMI {
	namespace Utilities {

		/**
		 * @brief Contains data about a specific error being returned
		 */
		struct CALUMIANIMATION_API FileError
		{
			/**
			 * @brief Simple enum to determine failure mechanism during file parsing
			 */
			enum class FileErrorCode : uint8_t
			{
				FileNotFound = 0,
				PermissionDenied = 1,
				NotAFile = 2,
				ReadFailure = 3,
				IncorrectFileType = 4,
				FileTooLarge = 5,
				FileTooSmall = 6,
				UnknownErrorCode = 7,
				WriteFailure = 8
			};


			/** @name Initialization */
			/// @{
			FileError();
			~FileError();
			FileError(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const char* errorMessage);
			FileError(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const CALUMI::Utilities::StringContainer& errorMessage);
			FileError(const FileError& source);
			/// @}
			/** @name Operators */
			/// @{
			FileError& operator=(const FileError& source);
			/// @}

			/** @name Error Info */
			/// @{
			

			/**
			 * @brief Error Code Enum
			 * @return Enum
			 */
			FileErrorCode GetFileErrorCode() const;
			/**
			 * @brief File path in question
			 * @return Path Container of the file path
			 */
			CALUMI::Utilities::PathContainer GetFilePath() const;
			/**
			 * @brief Message specific to the operation
			 * @return 
			 */
			const char* GetErrorMessage() const;
			/// @}

			/** @name Print */
			/// @{

			/**
			 * @brief Conbines the message, error code, and filepath into a human readable format
			 * @return String container with formatted message
			 */
			CALUMI::Utilities::StringContainer ToString();
			/// @}

		private:
			struct Impl;
			Impl* pImpl;
		};


#pragma warning(disable: 4661)
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<bool, CALUMI::Utilities::FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::PathContainer, CALUMI::Utilities::FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::StringContainer, CALUMI::Utilities::FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::StringMap, CALUMI::Utilities::FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<char>, CALUMI::Utilities::FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<CALUMI::Utilities::StringContainer>, CALUMI::Utilities::FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<CALUMI::Utilities::PathContainer>, CALUMI::Utilities::FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<int>, CALUMI::Utilities::FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<float>, CALUMI::Utilities::FileError>;
template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<double>, CALUMI::Utilities::FileError>;
#pragma warning(default: 4661)
}
}