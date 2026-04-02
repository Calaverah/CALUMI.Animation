//Copyright © 2025-2026 Calaverah. All rights reserved.
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

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace CALUMI {
	enum class CALUMIANIMATION_API SupportedGames : uint8_t
	{
		Starfield
	};
}

/**
		 *
		 * @defgroup extern_c Extern "C" API
		 * @brief Group of C Style functions to be used by non c++ functions such as Python with C-Types
		 * @{
		 * @}
		 * 
		 * @defgroup Singletons
		 * @brief Group of singletons that provide convenient methods globally
		 * @{
		 * @}
		 * */