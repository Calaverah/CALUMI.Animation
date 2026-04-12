//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "univ/UNIV_AnimationScene.h"
#include "utilities/CALUMI_Common.h"
#include "interfaces/IConvertible.h"
#include "animation/SFBGS_Animation.h"
#include "skeletonrig/SFBGS_SkeletonRig.h"


namespace CALUMI::SFBGS
{

	class CALUMIANIMATION_API AnimationScene : UNIV::IConvertibleScene
	{
	public:
		[[nodiscard]] Utilities::StringContainer& sceneName() const;
		[[nodiscard]] Animation& animation(uint64_t idx) const;
		[[nodiscard]] uint64_t animationCount() const;
		[[nodiscard]] SkeletonRig& rig() const;
		void setRig(const SkeletonRig& rig) const;

		AnimationScene();
		~AnimationScene() override;
		explicit AnimationScene(const char* sceneName);
		explicit AnimationScene(const Utilities::StringContainer& sceneName);
		AnimationScene(const AnimationScene& input);

		bool addAnimationToScene(const Animation& animation, bool overwrite = true) const;
		bool removeAnimationFromScene(const Utilities::StringContainer& sceneToRemove) const;
		bool removeAnimationFromScene(const char* sceneToRemove) const;
		bool removeAnimationFromScene(unsigned int idx) const;

		// Inherited via IConvertibleScene
		bool convertFromUniversalScene(UNIV::AnimationScene& input) override;
		bool convertFromUniversalScene(const UNIV::AnimationScene& input, const SkeletonRig& rigReference) const;
		UNIV::AnimationScene convertToUniversalScene() override;

		AnimationScene& operator=(const AnimationScene& input);

	private:
		struct Impl;
		Impl* pImpl;
	};


	extern "C" {
	CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatPathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, uint64_t arraySize, Utilities::StringContainer* errorMessage);
	CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatUsingRigReferencePathOverrideC(const UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, uint64_t arraySize, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage);
	CALUMIANIMATION_API UNIV::AnimationScene* LoadAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray, int numberOfFiles, Utilities::StringContainer* errorMessage);
	CALUMIANIMATION_API UNIV::AnimationScene* LoadAnimationSceneFromSFBGSFormatAndSaveToJsonC(const wchar_t** filePathsArray, int numberOfFiles, Utilities::StringContainer* errorMessage, const wchar_t* jsonOutputPath);
	CALUMIANIMATION_API UNIV::SkeletonRig* LoadSFBGSSkeletonRigFromFileC(const wchar_t* filePath, Utilities::StringContainer* errorMessage);

	CALUMIANIMATION_API bool SaveAnimationToSFBGSFormatDirectC(UNIV::Animation* animation, const wchar_t* filePath, const UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage);
	CALUMIANIMATION_API bool SaveAnimationToSFBGSFormatWithExistingRigDirectC(UNIV::Animation* animation, const wchar_t* filePath, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage);
	CALUMIANIMATION_API bool SaveSkeletonRigToSFBGSFormatDirectC(const UNIV::SkeletonRig* rig, const wchar_t* filePath, Utilities::StringContainer* errorMessage);
	}
}

