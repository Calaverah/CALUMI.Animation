//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include <vector>
#include <SimpleMath.h>
#include "CALUMI_Math.h"
#include <cstdint>
#include <algorithm>

namespace CALUMI {namespace UNIV {


	struct CALUMIANIMATION_API Translation
	{
		uint16_t frame = 0;
		CALUMI::Math::Vector3D translation;

		Translation() = default;
		Translation(const uint16_t& frame, const CALUMI::Math::Vector3D& translation)
			: frame(frame), translation(translation)
		{
		}
	};

	struct CALUMIANIMATION_API Rotation
	{
		uint16_t frame = 0;
		DirectX::SimpleMath::Quaternion rotation;

		Rotation() = default;
		Rotation(const uint16_t& frame, const DirectX::SimpleMath::Quaternion& rotation)
			: frame(frame), rotation(rotation)
		{
		}
	};

	struct CALUMIANIMATION_API Scalar
	{
		uint16_t frame = 0;
		float scalar = 1.0;

		Scalar() = default;
		Scalar(const uint16_t& frame, float scalar)
			: frame(frame), scalar(scalar)
		{
		}
	};

	struct CALUMIANIMATION_API Priority
	{
		uint16_t frame = 0;
		uint8_t priority;

		Priority(const uint16_t& frame, const uint8_t& priority)
			: frame(frame), priority(priority)
		{
		}
	};

}}
