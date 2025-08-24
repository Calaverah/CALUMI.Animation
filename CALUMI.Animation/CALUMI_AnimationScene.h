//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include <vector>
#include <string>
#include "CALUMI_Animation.h"
#include "CALUMI_SkeletonRig.h"

namespace CALUMI{namespace UNIV{

	extern  "C" {

		class CALUMIANIMATION_API AnimationScene
		{
		public:
			std::string sceneName = "MyAnimationScene";
			std::vector<Animation> animations;
			SkeletonRig rig;


		};
	}
}}

