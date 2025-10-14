#include "pch.h"
#include "CALUMI_BoneTypes.h"


namespace CALUMI {
	namespace UNIV {
		//Returns default when the switch case... defaults... 
		const UNIV::BoneType BoneTypeFromString(const Utilities::StringContainer& boneTypeStr)
		{
			if (_stricmp(boneTypeStr.c_str(), TwistBoneTypeStr) == 0)
				return UNIV::BoneType::Twist;
			else if (_stricmp(boneTypeStr.c_str(), DefaultBoneTypeStr) == 0)
				return UNIV::BoneType::Default;
			else
				return UNIV::BoneType::UNDEFINED;
		}
	}
}
