//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Utilities.h"
#include "interfaces/IPackageManager.h"

namespace CALUMI {
	namespace UNIV {


		/**
		 * @brief RigPackages provide a convenient way for users to apply game specific data to a rig
		 *
		 * @details Rig Packages allow a rig to have multiple games worth of data applied to the rig without conflicting. The intention is that users can export a single rig for various games at the same time... assuming the game is supported.
		 */
		struct CALUMIANIMATION_API IRigPackage : public IPackage
		{
			/** @name Initialization */
			/// @{
			IRigPackage() = default;
			virtual ~IRigPackage() = default;

			/// @}

		protected:
			/**
			 * @brief Event called when a rig renames a bone.
			 * @param oldBone Name of the bone being renamed
			 * @param newName The new bone name
			 * @param idx Index of the interaction
			 * @return Whether the operation was successful
			 */
			virtual bool HandleBoneRename(const char* oldBone, const char* newName, size_t idx) = 0;

		private:
			friend struct RigPackageManager;
		};

		/**
		 * @brief The Rig Package Manager handles the data and memory allocation of the packages for a rig
		 */
		struct CALUMIANIMATION_API RigPackageManager : public IPackageManager
		{
		public:
			/** @name Initialization*/
			/// @{

			RigPackageManager();

			///@}

		public:
			/**
			 * @brief Serialization
			 * @param indents Spaces for formatting
			 * @return The StringContainer of the serialized struct
			 */
			Utilities::StringContainer ToJSON(size_t indents) const;

		private:
			/**
			 * @brief Bone Rename Event that's passed to the packages being managed
			 * @param oldBone Old name of the bone
			 * @param newName New name of the bone
			 * @param idx Index of the bone
			 * @return Whether the operation was a success
			 */
			bool HandleBoneRename(const char* oldBone, const char* newName, size_t idx);
			friend struct SkeletonRig;
		};

		/**
		 * @addtogroup extern_c
		 * @{
		 * @defgroup c_univ_rig_package Rig Package "C" API
		 * @brief C Style API for Rig Packages
		 * @{
		 * @}
		 * @}
		 * */

} }