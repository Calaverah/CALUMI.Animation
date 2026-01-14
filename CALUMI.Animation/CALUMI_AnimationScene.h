//Copyright © 2025-2026 Calaverah. All rights reserved.
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
		SkeletonRig& Rig() const;
		Utilities::VectorContainer<Animation>& Animations() const;
		const char* SceneName() const;
		void SceneName(const char* name);
		void SceneName(const Utilities::StringContainer& input);

		bool AddAnimationToScene(UNIV::Animation& animation, bool overwrite = true);
		bool RemoveAnimationFromScene(Utilities::StringContainer& sceneToRemove);
		bool RemoveAnimationFromScene(unsigned int idx);
		
		Utilities::ExpectedContainer<Utilities::VectorContainer<Utilities::PathContainer>, Utilities::StringContainer> GetFilePathsFromAnimationScene(const wchar_t* directoryPath, const char* extension);
		Utilities::StringContainer ToJSON(size_t indents) const;

		AnimationScene();
		AnimationScene(const Utilities::StringContainer& sceneName);
		AnimationScene(const char* sceneName);
		AnimationScene(const AnimationScene& input);
		~AnimationScene();

		AnimationScene& operator=(const AnimationScene& input);

	private:
		struct Impl;
		Impl* pImpl;
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

