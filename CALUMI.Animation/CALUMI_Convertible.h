//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_AnimationScene.h"

namespace CALUMI {namespace UNIV
{

	//template <class T>
	class CALUMIANIMATION_API Convertible
	{
	public:
		virtual bool ConvertFromUniversalScene(AnimationScene& input) = 0;
		virtual AnimationScene ConvertToUniversalScene() = 0;
	};
}}