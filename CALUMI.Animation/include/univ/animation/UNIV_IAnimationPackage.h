//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "interfaces/IPackageManager.h"

namespace CALUMI {
	namespace UNIV {
		/**
	 * @brief A game specific package of data that can be applied directly to an animation
	 */
		struct CALUMIANIMATION_API IAnimationPackage : public IPackage
		{

			/// @name Initialization
			/// @{
			IAnimationPackage() = default;
			virtual ~IAnimationPackage() = default;
			/// @}

		private:
			friend struct AnimationPackageManager;

		};

		/**
		 * @brief The Animation Package Manager handles the data and memory allocation of the packages for an Animation
		 */
		struct CALUMIANIMATION_API AnimationPackageManager : public IPackageManager
		{
		public:
			/** @name Initialization*/
			/// @{

			AnimationPackageManager();
			virtual ~AnimationPackageManager() = default;
			///@}

		public:

			/**
			 * @brief Serialization
			 * @param indents Spaces for formatting
			 * @return The StringContainer of the serialized struct
			 */
			Utilities::StringContainer ToJSON(size_t indents) const override;

		private:
			friend class Animation;
		};

		/**
		 * @addtogroup extern_c
		 * @{
		 * @defgroup c_univ_anim_package Animation Package "C" API
		 * @brief C Style API for Animation Packages
		 * @{
		 * @}
		 * @}
		 * */
} }