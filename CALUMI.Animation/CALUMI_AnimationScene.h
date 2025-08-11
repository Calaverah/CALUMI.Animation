#pragma once
#ifdef CALUMIANIMATION_EXPORTS // This macro is typically defined by Visual Studio for DLL projects
#define CALUMIANIMATION_API __declspec(dllexport)
#else
#define CALUMIANIMATION_API __declspec(dllimport)
#endif

#include <vector>
#include <string>
#include "CALUMI_Animation.h"
#include "CALUMI_SkeletonRig.h"

namespace CALUMI{namespace UNIV{



	class CALUMIANIMATION_API AnimationScene
	{
	public:
		std::string sceneName = "MyAnimationScene";
		std::vector<Animation> animations;
		SkeletonRig rig;


	};
}}

