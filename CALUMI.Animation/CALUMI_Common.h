//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include <cstdint>

#ifdef CALUMIANIMATION_EXPORTS
	#define CALUMIANIMATION_API __declspec(dllexport)
	#else
	#define CALUMIANIMATION_API __declspec(dllimport)
#endif

#define _PRIVATE_(x) //Private placeholder for visual aid for this var

namespace CALUMI {
	enum class CALUMIANIMATION_API SupportedGames
	{
		Starfield
	};
}