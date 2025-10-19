//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_Utilities.h"

namespace CALUMI {
	namespace UNIV {

		enum class CALUMIANIMATION_API BoneType : uint32_t
		{
			Default = 0,
			Twist = 1,
			UNDEFINED = 0xFFFFFFFF
		};

		inline static const char* DefaultBoneTypeStr = "Default";
		inline static const char* TwistBoneTypeStr = "Twist";



		//Returns default when the switch case... defaults... 
		CALUMIANIMATION_API const UNIV::BoneType BoneTypeFromString(const Utilities::StringContainer& boneTypeStr);

		//Abstract Parent Struct for Bone Type Data. All Types will have an enum informing the user how to cast the child struct.
		struct CALUMIANIMATION_API BoneTypeProperties
		{
			const UNIV::BoneType GetType() const;

			virtual const char* GetTypeString() const = 0;

			virtual ~BoneTypeProperties() {};

		protected:
			UNIV::BoneType boneType = UNIV:: BoneType::Default;

		};

		//Default Type
		struct CALUMIANIMATION_API DefaultBoneProperties : BoneTypeProperties
		{
			DefaultBoneProperties();
			~DefaultBoneProperties() override {};
			const char* GetTypeString() const override;
		};

		//Basic Twist Type
		struct CALUMIANIMATION_API TwistBoneProperties : BoneTypeProperties
		{

			int32_t twistDriverIndex = -1;
			float twistDriverWeight = 0.0f;


			TwistBoneProperties();
			~TwistBoneProperties() override {};

			const char* GetTypeString() const override;
		};

}}