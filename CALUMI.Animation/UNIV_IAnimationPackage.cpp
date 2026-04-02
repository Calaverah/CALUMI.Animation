//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "UNIV_IAnimationPackage.h"
#include "UNIV_IPackageManager.h"


namespace CALUMI {
	namespace UNIV {

#pragma region Animation Package Manager

	AnimationPackageManager::AnimationPackageManager() : IPackageManager() { }

	Utilities::StringContainer AnimationPackageManager::ToJSON(size_t indents) const
	{
		//TODO: Animation Package Manager JSON
		return Utilities::StringContainer();
	}
#pragma endregion

} }