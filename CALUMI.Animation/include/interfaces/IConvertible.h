//Copyright � 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "univ/UNIV_AnimationScene.h"


namespace CALUMI {namespace UNIV
{

	/**
	 * @brief Provides interface methods for converting a UNIV Animation Scene and it's data to a given format
	 */
	class CALUMIANIMATION_API IConvertibleScene
	{
	public:
		/**
		 * @brief For populating this class with data based on a UNIV Animation Scene
		 * @param input Universal Animation Scene to pull information from
		 * @return Whether the operation was a success
		 */
        virtual bool convertFromUniversalScene(AnimationScene& input) = 0;
		/**
		 * @brief For creating a new UNIV Animation Scene with this data from this class
		 * @return Newly constructed Universal Animation Scene
		 */
        virtual AnimationScene convertToUniversalScene() = 0;
	};
}}