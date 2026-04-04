//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "univ/UNIV_AnimationScene.h"
#include "utilities/CALUMI_Common.h"
#include "interfaces/IConvertible.h"
#include "animation/SFBGS_Animation.h"
#include "skeletonrig/SFBGS_SkeletonRig.h"


namespace CALUMI {
	namespace SFBGS {
		
		class CALUMIANIMATION_API AnimationScene : UNIV::IConvertibleScene
		{
		public:
			Utilities::StringContainer& SceneName() const;
			Utilities::VectorContainer<Animation>& Animations() const;
			SkeletonRig& Rig() const;
			void setRig(const SkeletonRig& rig);

			AnimationScene();
			~AnimationScene();
			AnimationScene(const char* sceneName);
			AnimationScene(const Utilities::StringContainer& sceneName);
			AnimationScene(const AnimationScene& input);

			bool AddAnimationToScene(SFBGS::Animation& animation, bool overwrite = true);
			bool RemoveAnimationFromScene(Utilities::StringContainer& sceneToRemove);
			bool RemoveAnimationFromScene(const char* sceneToRemove);
			bool RemoveAnimationFromScene(unsigned int idx);

			// Inherited via IConvertibleScene
			bool ConvertFromUniversalScene(UNIV::AnimationScene& input) override;
			bool ConvertFromUniversalScene(CALUMI::UNIV::AnimationScene& input, SFBGS::SkeletonRig& rigReference);
			UNIV::AnimationScene ConvertToUniversalScene() override;

			AnimationScene& operator=(const AnimationScene& input);

		private:
			struct Impl;
			Impl* pImpl;
		};


		extern "C" {
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatC(UNIV::AnimationScene* scene, const wchar_t* directoryPath, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatPathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, std::size_t arraySize, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatUsingRigReferenceC(UNIV::AnimationScene* scene, const wchar_t* directoryPath,const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatUsingRigReferencePathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, std::size_t arraySize, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API UNIV::AnimationScene* LoadAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray, int numberOfFiles, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API UNIV::AnimationScene* LoadAnimationSceneFromSFBGSFormatAndSaveToJsonC(const wchar_t** filePathsArray, int numberOfFiles, Utilities::StringContainer* errorMessage, const wchar_t* jsonOutputPath);
			CALUMIANIMATION_API UNIV::SkeletonRig* LoadSFBGSSkeletonRigFromFileC(const wchar_t* filePath, Utilities::StringContainer* errorMessage);

			CALUMIANIMATION_API bool SaveAnimationToSFBGSFormatDirectC(UNIV::Animation* animation, const wchar_t* filePath, UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API bool SaveAnimationToSFBGSFormatWithExistingRigDirectC(UNIV::Animation* animation, const wchar_t* filePath, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API bool SaveSkeletonRigToSFBGSFormatDirectC(UNIV::SkeletonRig* rig, const wchar_t* filePath, Utilities::StringContainer* errorMessage);
		}
	}
}

