//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalplatform.h"
#include "univ/skeletonrig/UNIV_BoneTypes.h"



namespace CALUMI::UNIV {


	BoneTypeProperty::BoneType BoneTypeProperty::BoneTypeFromString(const char* boneTypeStr)
	{
		if (SCOMPARE(boneTypeStr, TwistBoneTypeStr) == 0)
			return BoneType::Twist;

		if (SCOMPARE(boneTypeStr, DefaultBoneTypeStr) == 0)
			return BoneType::Default;
		
		return BoneType::UNDEFINED;
	}

	BoneTypeProperty::BoneType BoneTypeProperty::GetBoneType(uint32_t typeAsInteger)
	{
		auto type = BoneType::Default;

		if (typeAsInteger <= static_cast<uint32_t>(BoneType::Max))
			type = static_cast<BoneType>(typeAsInteger);

		return type;
	}

	const char* DefaultBoneProperty::getTypeString() const
	{
		return DefaultBoneTypeStr;
	}
    BoneTypeProperty::BoneType DefaultBoneProperty::getType() const
	{
		return BoneType::Default;
	}


	struct TwistBoneProperty::Impl
	{
		std::string _twistDriver;
		float _twistDriverWeight = 0.0f;
		Impl() = default;
	};
    const char* TwistBoneProperty::twistDriver() const
	{
		return pImpl->_twistDriver.c_str();
	}
    void TwistBoneProperty::setTwistDriver(const char* boneName) const
    {
		pImpl->_twistDriver = boneName;
	}
    float TwistBoneProperty::twistDriverWeight() const
	{
		return pImpl->_twistDriverWeight;
	}
    void TwistBoneProperty::setTwistDriverWeight(const float wgt) const
    {
		pImpl->_twistDriverWeight = wgt;
	}
	TwistBoneProperty::TwistBoneProperty()
	{
		pImpl = new Impl;
	}
	TwistBoneProperty::~TwistBoneProperty()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	TwistBoneProperty::TwistBoneProperty(const TwistBoneProperty& input) : TwistBoneProperty()
	{
		*this = input;
	}
    const char* TwistBoneProperty::getTypeString() const
	{
		return TwistBoneTypeStr;
	}
    BoneTypeProperty::BoneType TwistBoneProperty::getType() const
	{
		return BoneType::Twist;
	}
	TwistBoneProperty& TwistBoneProperty::operator=(const TwistBoneProperty& input)
	{
    	if (this != &input)
    	{
    		pImpl->_twistDriver = input.pImpl->_twistDriver;
    		pImpl->_twistDriverWeight = input.pImpl->_twistDriverWeight;
    	}
		return *this;
	}
}

