//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_Utilities.h"
#include "FileError.h"

namespace CALUMI {
	class CALUMIANIMATION_API ReadWritable
	{
		virtual Utilities::ExpectedConatiner<bool, FileError> ReadFromFile(Utilities::PathContainer& inputFilePath) = 0;
		virtual Utilities::ExpectedConatiner<Utilities::StringContainer, FileError> WriteToFile(Utilities::PathContainer& outputFilePath) = 0;
	};
}

