//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once


#ifdef CALUMIANIMATION_EXPORTS
	#ifndef CALUMIANIMATION_API
		#define CALUMIANIMATION_API __declspec(dllexport)
		#else
		#define CALUMIANIMATION_API __declspec(dllimport)
	#endif
#endif

#define _PRIVATE_(x) //Private placeholder for visual aid for this var