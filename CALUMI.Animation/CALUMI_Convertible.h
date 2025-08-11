#pragma once
#ifdef CALUMIANIMATION_EXPORTS // This macro is typically defined by Visual Studio for DLL projects
#define CALUMIANIMATION_API __declspec(dllexport)
#else
#define CALUMIANIMATION_API __declspec(dllimport)
#endif

#include "CALUMI_AnimationScene.h"

namespace CALUMI {namespace UNIV
{

	//template <class T>
	class CALUMIANIMATION_API Convertible
	{
	public:
		virtual void ConvertFromUniversalScene(AnimationScene& input) = 0;
		virtual AnimationScene ConvertToUniversalScene() = 0;
	};
}}