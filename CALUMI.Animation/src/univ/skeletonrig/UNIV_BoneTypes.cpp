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

		if (typeAsInteger <= BoneTypeMax)
			type = static_cast<BoneType>(typeAsInteger);

		return type;
	}

	const char* DefaultBoneProperty::typeAsString() const
	{
		return DefaultBoneTypeStr;
	}
    BoneTypeProperty::BoneType DefaultBoneProperty::type() const
	{
		return BoneType::Default;
	}

    Utilities::JsonObject DefaultBoneProperty::toJson() const
    {
		Utilities::JsonObject output;

		output["type"] = typeAsString();

		return output;
    }


    struct TwistBoneProperty::Impl
	{
		std::string m_twistDriver;
		float m_twistDriverWeight = 0.0f;
		Impl() = default;
	};
    const char* TwistBoneProperty::twistDriver() const
	{
		return pImpl->m_twistDriver.c_str();
	}
    void TwistBoneProperty::setTwistDriver(const char* boneName) const
    {
		pImpl->m_twistDriver = boneName;
	}
    float TwistBoneProperty::twistDriverWeight() const
	{
		return pImpl->m_twistDriverWeight;
	}
    void TwistBoneProperty::setTwistDriverWeight(const float wgt) const
    {
		pImpl->m_twistDriverWeight = wgt;
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
    const char* TwistBoneProperty::typeAsString() const
	{
		return TwistBoneTypeStr;
	}
    BoneTypeProperty::BoneType TwistBoneProperty::type() const
	{
		return BoneType::Twist;
	}
	TwistBoneProperty& TwistBoneProperty::operator=(const TwistBoneProperty& input)
	{
    	if (this != &input)
    	{
    		pImpl->m_twistDriver = input.pImpl->m_twistDriver;
    		pImpl->m_twistDriverWeight = input.pImpl->m_twistDriverWeight;
    	}
		return *this;
	}

	Utilities::JsonObject TwistBoneProperty::toJson() const
	{
    	Utilities::JsonObject output;

    	output["type"] = typeAsString();
    	output["driver"] = pImpl->m_twistDriver.c_str();
    	output["weight"] = pImpl->m_twistDriverWeight;

    	return output;
	}
}

