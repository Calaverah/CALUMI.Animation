//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_AnimationScene.h"
#include "CALUMI_Common.h"
#include "CALUMI_Convertible.h"
#include "SFBGS_Animation.h"
#include "SFBGS_SkeletonRig.h"
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace CALUMI {
	namespace SFBGS {

		
		class CALUMIANIMATION_API AnimationScene : UNIV::Convertible
		{
		public:
			std::string sceneName = "MyStarfieldAnimationScene";
			std::vector<Animation> animations;
			SkeletonRig rig;

			/// <summary>
			/// Small increment multiplier. Default is 1/4000. For ships and first person, respectively, choose 1/500 and 1/16000
			/// </summary>
			float highPrecisionValue = 0.00025;
			/// <summary>
			/// Large increment multiplier. Default is 1/32. For ships and first person, respectively, choose 1/4 and 1/128
			/// </summary>
			float lowPrecisionValue = 0.03125;

			AnimationScene() = default;
			const char* GetSceneName() { return sceneName.c_str(); }
			void SetSceneName(const char* name) { sceneName = name; }

			// Inherited via Convertible
			bool ConvertFromUniversalScene(UNIV::AnimationScene& input) override;
			bool ConvertFromUniversalScene(CALUMI::UNIV::AnimationScene& input, SFBGS::SkeletonRig& rigReference);
			UNIV::AnimationScene ConvertToUniversalScene() override;
		};


		extern  "C" {
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatC(UNIV::AnimationScene* scene, float highPrecisionValue, float lowPrecisionValue, const wchar_t* directoryPath, const char* errorMessage);
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatPathOverrideC(UNIV::AnimationScene* scene, float highPrecisionValue, float lowPrecisionValue, const wchar_t** directoryPathArray, size_t arraySize, const char* errorMessage);
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatUsingRigReferenceC(UNIV::AnimationScene* scene, const wchar_t* directoryPath,const wchar_t* sfbgsRigPath, const char* errorMessage);
			CALUMIANIMATION_API bool SaveAnimationSceneToSFBGSFormatUsingRigReferencePathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, size_t arraySize, const wchar_t* sfbgsRigPath, const char* errorMessage);
			CALUMIANIMATION_API UNIV::AnimationScene* LoadAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray, int numberOfFiles, const char* errorMessage, const wchar_t* jsonOutputPath);
		}
	}
}

