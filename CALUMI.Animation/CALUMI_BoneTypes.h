//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include <cstdint>
#include <string>

namespace CALUMI {
	namespace UNIV {

		enum class BoneType : uint32_t
		{
			Default = 0,
			Twist = 1,
			UNDEFINED = 0xFFFFFFFF
		};

		inline static const char* DefaultBoneTypeStr = "Default";
		inline static const char* TwistBoneTypeStr = "Twist";



		//Returns default when the switch case... defaults... 
		inline const UNIV::BoneType CALUMIANIMATION_API BoneTypeFromString(const std::string boneTypeStr)
		{
			if (_stricmp(boneTypeStr.c_str(), TwistBoneTypeStr))
				return UNIV::BoneType::Twist;
			else if (_stricmp(boneTypeStr.c_str(), DefaultBoneTypeStr))
				return UNIV::BoneType::Default;
			else
				return UNIV::BoneType::UNDEFINED;
		}

		//Abstract Parent Struct for Bone Type Data. All Types will have an enum informing the user how to cast the child struct.
		struct BoneTypeProperties
		{
			inline const UNIV::BoneType GetType() const
			{
				return boneType;
			}

			virtual const char* GetTypeString() const = 0;

			virtual ~BoneTypeProperties() = default;

		protected:
			UNIV::BoneType boneType = UNIV:: BoneType::Default;

		};

		//Default Type
		struct DefaultBoneProperties : BoneTypeProperties
		{
			inline DefaultBoneProperties()
			{
				boneType = UNIV::BoneType::Default;
			}

			inline const char* GetTypeString() const override
			{
				return DefaultBoneTypeStr;
			}
		};

		//Basic Twist Type                                                             (Note Manqn Override being considered currently)
		struct TwistBoneProperties : BoneTypeProperties
		{

			int32_t twistDriverIndex = -1;
			float twistDriverWeight = 0.0;


			inline TwistBoneProperties()
			{
				boneType = UNIV::BoneType::Twist;
			}

			inline const char* GetTypeString() const override
			{
				return TwistBoneTypeStr;
			}
		};

}}