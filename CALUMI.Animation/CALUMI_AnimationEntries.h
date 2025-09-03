//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_Math.h"
#include <algorithm>
#include <cstdint>
#include <vector>

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

		std::string ToJSON() const;
	};

	struct CALUMIANIMATION_API Rotation
	{
		uint16_t frame = 0;
		CALUMI::Math::Quaternion rotation;

		Rotation() = default;
		Rotation(const uint16_t& frame, const CALUMI::Math::Quaternion& rotation)
			: frame(frame), rotation(rotation)
		{
		}

		std::string ToJSON() const;
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

		std::string ToJSON() const;
	};

	struct CALUMIANIMATION_API Priority
	{
		uint16_t frame = 0;
		uint8_t priority;

		Priority(const uint16_t& frame, const uint8_t& priority)
			: frame(frame), priority(priority)
		{
		}

		std::string ToJSON() const;
	};

	extern  "C" {
		CALUMIANIMATION_API Rotation* CreateRotationEntryC(uint16_t frame, float x, float y, float z, float w);
		CALUMIANIMATION_API bool DeleteRotationEntryC(Rotation* ptr);
		CALUMIANIMATION_API Translation* CreateTranslationEntryC(uint16_t frame, double x, double y, double z);
		CALUMIANIMATION_API bool DeleteTranslationEntryC(Translation* ptr);
		CALUMIANIMATION_API Scalar* CreateScalarEntryC(uint16_t frame, float scalar);
		CALUMIANIMATION_API bool DeleteScalarEntryC(Scalar* ptr);
		CALUMIANIMATION_API Priority* CreatePriorityEntryC(uint16_t frame, uint8_t priority);
		CALUMIANIMATION_API bool DeletePriorityEntryC(Priority* ptr);
		/// <summary>
		/// This will return a uint16_t corresponding to the frame of the sequence entry provided.
		/// </summary>
		/// <param name="source">Caution: If the source given is not of the proper entry type, then the returned value may be garbage</param>
		/// <returns></returns>
		CALUMIANIMATION_API uint16_t GetFrameFromEntryC(void* source);
		/// <summary>
		/// Returns a pointer to a rotation entry value (Quaternion). An array of 4 floats (4 bytes each)
		/// </summary>
		/// <param name="source"></param>
		/// <returns></returns>
		CALUMIANIMATION_API Math::Quaternion* GetValueFromRotationEntryC(Rotation* source);
		/// <summary>
		/// Returns a pointer to the translation entry value (Vector3D). An array of 3 doubles  (8 bytes each)
		/// </summary>
		/// <param name="source"></param>
		/// <returns></returns>
		CALUMIANIMATION_API Math::Vector3D* GetValueFromTranslationEntryC(Translation* source);
		/// <summary>
		/// Returns the scalar entry value (float)
		/// </summary>
		/// <param name="source"></param>
		/// <returns></returns>
		CALUMIANIMATION_API float GetValueFromScalarEntryC(Scalar* source);
		/// <summary>
		/// Returns the priority entry value (uint8_t)
		/// </summary>
		/// <param name="source"></param>
		/// <returns></returns>
		CALUMIANIMATION_API uint8_t GetValueFromPriorityEntryC(Priority* source);
	}
}}
