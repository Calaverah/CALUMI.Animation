//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "FileError.h"

namespace CALUMI{


	Utilities::ExpectedConatiner<Utilities::VectorContainer<char>, FileError> ValidateFile(const Utilities::PathContainer& inputPath);
	Utilities::ExpectedConatiner<Utilities::VectorContainer<char>, FileError> ValidateFile(const Utilities::PathContainer& inputPath, const Utilities::VectorContainer<Utilities::StringContainer> fileExtensions, bool allowFiles = true);
	Utilities::ExpectedConatiner<Utilities::VectorContainer<char>, FileError> ValidateFile(const Utilities::PathContainer& inputPath, const Utilities::VectorContainer<Utilities::StringContainer> fileExtensions, unsigned int minFileSize, unsigned long long maxFileSize = 0, bool allowFiles = true);
	Utilities::ExpectedConatiner<Utilities::VectorContainer<char>, FileError> ValidateFile(const Utilities::PathContainer& inputPath, unsigned int minFileSize, unsigned long long maxFileSize = 0);

	Utilities::ExpectedConatiner<Utilities::StringContainer, FileError> WriteToBinaryFile(const Utilities::PathContainer& outputPath, Utilities::VectorContainer<char>& buffer);
	Utilities::ExpectedConatiner<Utilities::StringContainer, FileError> WriteToBinaryFile(const Utilities::PathContainer& outputPath, const Utilities::StringContainer& buffer);

}