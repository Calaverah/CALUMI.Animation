//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Utilities.h"
#include "interfaces/IPackageManager.h"

namespace CALUMI::UNIV
{
	class SkeletonRig;
	/**
		 * @brief RigPackages provide a convenient way for users to apply game specific data to a rig
		 *
		 * @details Rig Packages allow a rig to have multiple games worth of data applied to the rig without
		 * conflicting. The intention is that users can export a single rig for various games at the same time...
		 * assuming the game is supported.
		 */
	class CALUMIANIMATION_API IRigPackage : public IPackage
	{
	public:
		/** @name Initialization */
		/// @{
		IRigPackage() = default;
		~IRigPackage() override = default;

		/// @}

	protected:
		/**
			 * @brief Event called when a rig renames a bone.
			 * @param oldBone Name of the bone being renamed
			 * @param newName The new bone name
			 * @return Whether the operation was successful
			 */
		virtual bool handleBoneRename(const char* oldBone, const char* newName) = 0;

	private:
		friend class RigPackageManager;
	};

	/**
		 * @brief The Rig Package Manager handles the data and memory allocation of the packages for a rig
		 */
	class CALUMIANIMATION_API RigPackageManager : public IPackageManager
	{
	public:
		/** @name Initialization*/
		/// @{

		RigPackageManager() = default;
		~RigPackageManager() override = default;

		///@}

	public:
		/**
			 * @brief Serialization
			 * @param indents Spaces for formatting
			 * @return The strContainer of the serialized struct
			 */
		[[deprecated]] [[nodiscard]] Utilities::StringContainer toJSON(uint64_t indents) const override;

	private:
		/**
			 * @brief Bone Rename Event that's passed to the packages being managed
			 * @param oldBone Old name of the bone
			 * @param newName New name of the bone
			 * @return Whether the operation was a success
			 */
		bool onBoneRename(const char* oldBone, const char* newName);
		friend class SkeletonRig;
	};
}
namespace CALUMI{

	 /// @addtogroup c_rig
	 /// @{
	 /// @defgroup c_rig_packages Rig Packages
	 /// @{
	 /// @defgroup c_univ_rig_packages Universal
	 /// @brief C Style API for UNIV Rig Packages
	 /// @{
	 /// @}
	 /// @}
	 /// @}
	 /// @defgroup rig
	 /// @{
	 /// @defgroup rig_packages Rig Packages
	 /// @{
	 /// @}
	 /// @}
}
