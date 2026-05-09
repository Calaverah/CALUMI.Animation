//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "interfaces/IPackageManager.h"

namespace CALUMI::UNIV
{
	class Animation;
	/**
	 * @brief A game specific package of data that can be applied directly to an animation
	 */
	class CALUMIANIMATION_API IAnimationPackage : public IPackage
	{
	public:
		/// @name Initialization
		/// @{
		IAnimationPackage() = default;
		~IAnimationPackage() override = default;
		/// @}

	private:
		friend class AnimationPackageManager;
	};

	/**
		 * @brief The Animation Package Manager handles the data and memory allocation of the packages for an Animation
		 */
	class CALUMIANIMATION_API AnimationPackageManager : public IPackageManager
	{
	public:
		/** @name Initialization*/
		/// @{

		AnimationPackageManager() = default;
		~AnimationPackageManager() override = default;
		///@}

	private:
		friend class Animation;
	};
}
namespace CALUMI{

	/**
		 * @addtogroup extern_c
		 * @{
		 * @defgroup c_anim Animation
		 * @brief C Style API for Animations
		 * @{
		 * @defgroup c_anim_packages Packages
		 * @{
		 * @}
		 * @}
		 * @}
		 * */
		 /// @defgroup anim Animation
		 /// @{
		 /// @defgroup anim_packages Animation Packages
		 /// @{
		 /// @}
		 /// @}
}
