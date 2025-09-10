//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include "CALUMI_Animation.h"
#include "CALUMI_Common.h"
#include "CALUMI_SkeletonRig.h"
#include <filesystem>
#include <string>
#include <vector>

namespace CALUMI{namespace UNIV{

	

	class CALUMIANIMATION_API AnimationScene
	{
	public:
		std::string sceneName = "MyAnimationScene";
		std::vector<Animation> animations;
		SkeletonRig rig;

		std::expected<std::vector<std::filesystem::path>, std::string> GetFilePathsFromAnimationScene(const wchar_t* directoryPath, const char* extension);
		std::string ToJSON(int indents) const;
	};

	

	extern  "C" {
		CALUMIANIMATION_API AnimationScene* CreateAnimationSceneC(const char* sceneName);
		CALUMIANIMATION_API bool AddRigToAnimationSceneC(AnimationScene* scene, SkeletonRig* rig, const char* errorMessage);
		CALUMIANIMATION_API bool AddAnimationToAnimationSceneC(AnimationScene* scene, Animation* animation, const char* errorMessage);
		CALUMIANIMATION_API bool DeleteAnimationSceneC(AnimationScene* ptr);
		CALUMIANIMATION_API Animation* GetAnimationC(AnimationScene* source, int index, const char* errorMessage);
		CALUMIANIMATION_API size_t GetAnimationCountC(AnimationScene* source);
		CALUMIANIMATION_API const char* GetAnimationSceneNameC(AnimationScene* source);
		CALUMIANIMATION_API SkeletonRig* GetSkeletonRigC(AnimationScene* source);
		CALUMIANIMATION_API bool HasSkeletonRigC(AnimationScene* source);
	}
}}

