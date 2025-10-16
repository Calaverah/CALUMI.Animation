//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_AnimationScene.h"
#include "CALUMI_Common.h"
#include "CALUMI_Convertible.h"
#include "SFBGS_Animation.h"
#include "SFBGS_SkeletonRig.h"



namespace CALUMI {
	namespace SFBGS {

		
		class CALUMIANIMATION_API AnimationScene : UNIV::Convertible
		{
		public:
			Utilities::StringContainer sceneName = "MyStarfieldAnimationScene";
			Utilities::VectorContainer<Animation> animations;
			SkeletonRig rig;

			/// <summary>
			/// Small increment multiplier. Default is 1/4000. For ships and first person, respectively, choose 1/500 and 1/16000
			/// </summary>
			float highPrecisionValue = 0.00025f;
			/// <summary>
			/// Large increment multiplier. Default is 1/32. For ships and first person, respectively, choose 1/4 and 1/128
			/// </summary>
			float lowPrecisionValue = 0.03125f;

			AnimationScene() = default;
			AnimationScene(const char* sceneName, float highPrecisionValue, float lowPrecisionValue)
				: sceneName(sceneName), highPrecisionValue(highPrecisionValue), lowPrecisionValue(lowPrecisionValue)
			{
			}
			AnimationScene(Utilities::StringContainer sceneName, float highPrecisionValue, float lowPrecisionValue)
				: sceneName(sceneName), highPrecisionValue(highPrecisionValue), lowPrecisionValue(lowPrecisionValue)
			{
			}

			bool AddAnimationToScene(SFBGS::Animation& animation, bool overwrite = true);
			bool RemoveAnimationFromScene(Utilities::StringContainer& sceneToRemove);
			bool RemoveAnimationFromScene(const char* sceneToRemove);
			bool RemoveAnimationFromScene(unsigned int idx);

			// Inherited via Convertible
			bool ConvertFromUniversalScene(UNIV::AnimationScene& input) override;
			bool ConvertFromUniversalScene(CALUMI::UNIV::AnimationScene& input, SFBGS::SkeletonRig& rigReference);
			UNIV::AnimationScene ConvertToUniversalScene() override;

			
		};


		extern  "C" {
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatC(UNIV::AnimationScene* scene, float highPrecisionValue, float lowPrecisionValue, const wchar_t* directoryPath, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatPathOverrideC(UNIV::AnimationScene* scene, float highPrecisionValue, float lowPrecisionValue, const wchar_t** directoryPathArray, size_t arraySize, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatUsingRigReferenceC(UNIV::AnimationScene* scene, const wchar_t* directoryPath,const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatUsingRigReferencePathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, size_t arraySize, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API UNIV::AnimationScene* LoadAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray, int numberOfFiles, Utilities::StringContainer* errorMessage);
			CALUMIANIMATION_API UNIV::AnimationScene* LoadJsonAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray, int numberOfFiles, Utilities::StringContainer* errorMessage, const wchar_t* jsonOutputPath);
			CALUMIANIMATION_API UNIV::SkeletonRig* LoadSFBGSSkeletonRigFromFileC(const wchar_t* filePath, Utilities::StringContainer* errorMessage);
		}
	}
}

