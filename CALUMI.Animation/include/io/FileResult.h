//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Utilities.h"
#include <cstdint>


namespace CALUMI {
	namespace Utilities {

		/**
		 * @brief Contains data about a specific error being returned
		 */
		struct CALUMIANIMATION_API FileResult
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
				WriteFailure = 8,
				Success = 0xFF
			};


			/** @name Initialization */
			/// @{
			FileResult();
			~FileResult();
			FileResult(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const char* errorMessage);
			FileResult(const FileErrorCode& fileCode, const CALUMI::Utilities::PathContainer& path, const CALUMI::Utilities::StringContainer& errorMessage);
			FileResult(const FileResult& source);
			/// @}
			/** @name Operators */
			/// @{
			FileResult& operator=(const FileResult& source);
			/// @}

			/** @name Result Info */
			/// @{
			
			/**
			 * @brief Simple way to check for error in the result
			 * @return 
			 */
			bool hasError() const;

			/**
			 * @brief Error Code Enum
			 * @return Enum
			 */
			FileErrorCode getFileErrorCode() const;
			/**
			 * @brief File _path in question
			 * @return Path Container of the file _path
			 */
			CALUMI::Utilities::PathContainer getFilePath() const;
			/**
			 * @brief Message specific to the operation
			 * @return 
			 */
			const char* getOpMessage() const;
			/// @}

			/** @name Print */
			/// @{

			/**
			 * @brief Conbines the message, error code, and filepath into a human readable format
			 * @return String container with formatted message
			 */
			CALUMI::Utilities::StringContainer toString();
			/// @}

		private:
			struct Impl;
			Impl* pImpl;
		};

		/**
		 * @brief File result with simple vector based file buffer
		 */
		struct CALUMIANIMATION_API FileBufferResult : public BufferObject
		{
			FileBufferResult();
			virtual ~FileBufferResult() override;

			void setResult(FileResult result);
			const FileResult& result() const;

		private:
			struct Impl;
			Impl* pImpl;
		};


#pragma warning(disable: 4661)
//template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<bool, CALUMI::Utilities::FileResult>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::PathContainer, CALUMI::Utilities::FileResult>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::StringContainer, CALUMI::Utilities::FileResult>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::StringList, CALUMI::Utilities::FileResult>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<char>, CALUMI::Utilities::FileResult>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<CALUMI::Utilities::StringContainer>, CALUMI::Utilities::FileResult>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<CALUMI::Utilities::PathContainer>, CALUMI::Utilities::FileResult>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<int>, CALUMI::Utilities::FileResult>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::FloatVector, CALUMI::Utilities::FileResult>;
//template struct CALUMIANIMATION_API CALUMI::Utilities::ExpectedContainer<CALUMI::Utilities::VectorContainer<double>, CALUMI::Utilities::FileResult>;
#pragma warning(default: 4661)
}
}