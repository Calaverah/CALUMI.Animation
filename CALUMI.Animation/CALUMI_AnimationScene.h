//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include <vector>
#include <string>
#include "CALUMI_Animation.h"
#include "CALUMI_SkeletonRig.h"
#include <filesystem>

namespace CALUMI{namespace UNIV{

	

	class CALUMIANIMATION_API AnimationScene
	{
	public:
		std::string sceneName = "MyAnimationScene";
		std::vector<Animation> animations;
		SkeletonRig rig;


	};


	extern  "C" {
		CALUMIANIMATION_API AnimationScene* CreateAnimationSceneC(const char* sceneName);
		CALUMIANIMATION_API bool AddRigToAnimationSceneC(AnimationScene* scene, SkeletonRig* rig, const char* errorMessage);
		CALUMIANIMATION_API bool AddAnimationToAnimationSceneC(AnimationScene* scene, Animation* animation, const char* errorMessage);
		CALUMIANIMATION_API bool DeleteAnimationSceneC(AnimationScene* ptr);
	}
}}

