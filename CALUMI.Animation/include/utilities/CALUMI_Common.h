//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#ifdef _WIN32
	#ifdef CALUMIANIMATION_EXPORTS
		#define CALUMIANIMATION_API __declspec(dllexport)
		#else
		#define CALUMIANIMATION_API __declspec(dllimport)
	#endif
#else
	#ifdef CALUMIANIMATION_EXPORTS
		#define CALUMIANIMATION_API __attribute__((visibility("default")))
	#else
		#define CALUMIANIMATION_API 
	#endif
	
#endif

#include <cstdint>

#if defined(_WIN32) || defined(__APPLE__)
// ReSharper disable once CppUnusedIncludeDirective
#include <cstddef>
#endif

namespace CALUMI {
	enum class SupportedGames : uint8_t
	{
		Starfield
	};

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
	 */
}
