//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include "CALUMI_Animation.h"
#include "CALUMI_Common.h"
#include "CALUMI_SkeletonRig.h"
#include "CALUMI_Utilities.h"

namespace CALUMI{namespace UNIV{

	

	class CALUMIANIMATION_API AnimationScene
	{
	public:
		Utilities::StringContainer sceneName = "MyAnimationScene";
		Utilities::VectorContainer<Animation> animations;
		SkeletonRig rig;

		bool AddAnimationToScene(UNIV::Animation& animation, bool overwrite = true);
		bool RemoveAnimationFromScene(Utilities::StringContainer& sceneToRemove);
		bool RemoveAnimationFromScene(unsigned int idx);
		
		Utilities::ExpectedConatiner<Utilities::VectorContainer<Utilities::PathContainer>, Utilities::StringContainer> GetFilePathsFromAnimationScene(const wchar_t* directoryPath, const char* extension);
		Utilities::StringContainer ToJSON(size_t indents) const;
	};

	

	extern "C" {
		CALUMIANIMATION_API AnimationScene* CreateAnimationSceneC(const char* sceneName);
		CALUMIANIMATION_API bool AddRigToAnimationSceneC(AnimationScene* scene, SkeletonRig* rig, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API bool AddAnimationToAnimationSceneC(AnimationScene* scene, Animation* animation, bool overwrite, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API bool DeleteAnimationSceneC(AnimationScene* ptr);
		CALUMIANIMATION_API Animation* GetAnimationC(AnimationScene* source, int index, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API size_t GetAnimationCountC(AnimationScene* source);
		CALUMIANIMATION_API const char* GetAnimationSceneNameC(AnimationScene* source);
		CALUMIANIMATION_API SkeletonRig* GetSkeletonRigC(AnimationScene* source);
		CALUMIANIMATION_API bool HasSkeletonRigC(AnimationScene* source);
	}

	
}}

