//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include "univ/animation/UNIV_Animation.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"
#include "utilities/CALUMI_Utilities.h"

namespace CALUMI{namespace UNIV{

	

	class CALUMIANIMATION_API AnimationScene
	{
	public:
        SkeletonRig& rig() const;

		Animation& animation(uint64_t idx) const;
		uint64_t animationCount() const;

        const char* sceneName() const;
        void setSceneName(const char* name);
        void setSceneName(const Utilities::StringContainer& input);

        bool addAnimationToScene(UNIV::Animation& animation, bool overwrite = true);
        bool removeAnimationFromScene(Utilities::StringContainer& sceneToRemove);
        bool removeAnimationFromScene(unsigned int idx);
		
		//Utilities::ExpectedContainer<Utilities::VectorContainer<Utilities::PathContainer>, Utilities::StringContainer> GetFilePathsFromAnimationScene(const wchar_t* directoryPath, const char* extension);

        Utilities::StringContainer toJSON(uint64_t indents) const;

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

	



	
}}

extern "C" {
CALUMIANIMATION_API CALUMI::UNIV::AnimationScene* CreateAnimationSceneC(const char* sceneName);
CALUMIANIMATION_API bool AddRigToAnimationSceneC(CALUMI::UNIV::AnimationScene* scene, CALUMI::UNIV::SkeletonRig* rig, CALUMI::Utilities::StringContainer* errorMessage);
CALUMIANIMATION_API bool AddAnimationToAnimationSceneC(CALUMI::UNIV::AnimationScene* scene, CALUMI::UNIV::Animation* animation, bool overwrite, CALUMI::Utilities::StringContainer* errorMessage);
CALUMIANIMATION_API bool DeleteAnimationSceneC(CALUMI::UNIV::AnimationScene* ptr);
CALUMIANIMATION_API CALUMI::UNIV::Animation* GetAnimationC(CALUMI::UNIV::AnimationScene* source, int index, CALUMI::Utilities::StringContainer* errorMessage);
CALUMIANIMATION_API uint64_t GetAnimationCountC(CALUMI::UNIV::AnimationScene* source);
CALUMIANIMATION_API const char* GetAnimationSceneNameC(CALUMI::UNIV::AnimationScene* source);
CALUMIANIMATION_API CALUMI::UNIV::SkeletonRig* GetSkeletonRigC(CALUMI::UNIV::AnimationScene* source);
CALUMIANIMATION_API bool HasSkeletonRigC(CALUMI::UNIV::AnimationScene* source);
}