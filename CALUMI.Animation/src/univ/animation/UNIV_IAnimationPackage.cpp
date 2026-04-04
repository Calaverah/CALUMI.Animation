//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include "univ/animation/UNIV_IAnimationPackage.h"


namespace CALUMI {
	namespace UNIV {

#pragma region Animation Package Manager

	AnimationPackageManager::AnimationPackageManager() : IPackageManager() { }

	Utilities::StringContainer AnimationPackageManager::ToJSON(std::size_t indents) const
	{
		//TODO: Animation Package Manager JSON
		return Utilities::StringContainer();
	}
#pragma endregion

} }