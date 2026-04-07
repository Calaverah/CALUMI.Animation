//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalplatform.h"
#include "univ/skeletonrig/UNIV_BoneTypes.h"


namespace CALUMI {
	namespace UNIV {

		//Returns default when the switch case... defaults... 
		const UNIV::BoneType BoneTypeFromString(const Utilities::StringContainer& boneTypeStr)
		{
			if (SCOMPARE(boneTypeStr.c_str(), TwistBoneTypeStr) == 0)
				return UNIV::BoneType::Twist;
			else if (SCOMPARE(boneTypeStr.c_str(), DefaultBoneTypeStr) == 0)
				return UNIV::BoneType::Default;
			else
				return UNIV::BoneType::UNDEFINED;
		}
        const char* DefaultBoneProperties::getTypeString() const
		{
			return DefaultBoneTypeStr;
		}
        UNIV::BoneType DefaultBoneProperties::getType() const
		{
			return UNIV::BoneType::Default;
		}


		struct TwistBoneProperties::Impl 
		{
			int32_t twistDriverIndex = -1;
			float twistDriverWeight = 0.0f;
			Impl() = default;
		};
        int32_t TwistBoneProperties::twistDriverIndex() const
		{
			return pImpl->twistDriverIndex;
		}
        void TwistBoneProperties::setTwistDriverIndex(int32_t idx)
		{
			pImpl->twistDriverIndex = idx;
		}
        float TwistBoneProperties::twistDriverWeight() const
		{
			return pImpl->twistDriverWeight;
		}
        void TwistBoneProperties::setTwistDriverWeight(float wgt)
		{
			pImpl->twistDriverWeight = wgt;
		}
		TwistBoneProperties::TwistBoneProperties()
		{
			pImpl = new Impl;
		}
		TwistBoneProperties::~TwistBoneProperties()
		{
			if (pImpl) delete pImpl;
		}
		TwistBoneProperties::TwistBoneProperties(const TwistBoneProperties* input) : TwistBoneProperties()
		{
			*pImpl = *(input->pImpl);
		}
        const char* TwistBoneProperties::getTypeString() const
		{
			return TwistBoneTypeStr;
		}
        UNIV::BoneType TwistBoneProperties::getType() const
		{
			return UNIV::BoneType::Twist;
		}
		TwistBoneProperties& TwistBoneProperties::operator=(const TwistBoneProperties& input)
		{
			*pImpl = *(input.pImpl);
			return *this;
		}
	}
}
