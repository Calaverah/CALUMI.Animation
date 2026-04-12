//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalplatform.h"
#include "univ/skeletonrig/UNIV_BoneTypes.h"



namespace CALUMI::UNIV {


	BoneType BoneTypeFromString(const char* boneTypeStr)
	{
		if (SCOMPARE(boneTypeStr, TwistBoneTypeStr) == 0)
			return BoneType::Twist;

		if (SCOMPARE(boneTypeStr, DefaultBoneTypeStr) == 0)
			return BoneType::Default;
		
		return BoneType::UNDEFINED;
	}
    const char* DefaultBoneProperties::getTypeString() const
	{
		return DefaultBoneTypeStr;
	}
    BoneType DefaultBoneProperties::getType() const
	{
		return BoneType::Default;
	}


	struct TwistBoneProperties::Impl
	{
		std::string _twistDriver;
		float _twistDriverWeight = 0.0f;
		Impl() = default;
	};
    const char* TwistBoneProperties::twistDriver() const
	{
		return pImpl->_twistDriver.c_str();
	}
    void TwistBoneProperties::setTwistDriver(const char* boneName) const
    {
		pImpl->_twistDriver = boneName;
	}
    float TwistBoneProperties::twistDriverWeight() const
	{
		return pImpl->_twistDriverWeight;
	}
    void TwistBoneProperties::setTwistDriverWeight(const float wgt) const
    {
		pImpl->_twistDriverWeight = wgt;
	}
	TwistBoneProperties::TwistBoneProperties()
	{
		pImpl = new Impl;
	}
	TwistBoneProperties::~TwistBoneProperties()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	TwistBoneProperties::TwistBoneProperties(const TwistBoneProperties& input) : TwistBoneProperties()
	{
		*this = input;
	}
    const char* TwistBoneProperties::getTypeString() const
	{
		return TwistBoneTypeStr;
	}
    BoneType TwistBoneProperties::getType() const
	{
		return BoneType::Twist;
	}
	TwistBoneProperties& TwistBoneProperties::operator=(const TwistBoneProperties& input)
	{
    	if (this != &input)
    	{
    		pImpl->_twistDriver = input.pImpl->_twistDriver;
    		pImpl->_twistDriverWeight = input.pImpl->_twistDriverWeight;
    	}
		return *this;
	}
}

