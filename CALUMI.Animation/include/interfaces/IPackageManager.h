//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Utilities.h"

namespace CALUMI::UNIV
{

	/**
		 * @brief RigPackages provide a convenient way for users to apply game specific data to a rig
		 *
		 * @details Rig Packages allow a rig to have multiple games worth of data applied to the rig without conflicting. The intention is that users can export a single rig for various games at the same time... assuming the game is supported.
		 */
	class CALUMIANIMATION_API IPackage
	{
	public:
		/** @name Initialization */
		/// @{
		IPackage() = default;
		virtual ~IPackage() = default;

		/// @}

	public:
		/**
			 * @brief A way to get a string describing the package.
			 * @return A c string of the package type
			 */
		[[nodiscard]] virtual const char* packageType() const = 0;

	protected:

		/**
			 * @brief A convenient way to duplicate the struct into a new dynamically allocated version.
			 * @details This function should only be used by the package manager as a simple way to copy data without sharing the struct or using pvt/shared ptrs
			 * @return The new dynamically allocated clone of the package
			 */
		[[nodiscard]] virtual IPackage* clone() const = 0;

	private:
		friend class IPackageManager;
	};


	/**
		 * @brief The Rig Package Manager handles the data and memory allocation of the packages for a rig
		 */
	class CALUMIANIMATION_API IPackageManager
	{
	public:
		/** @name Initialization*/
		/// @{

		IPackageManager();
		IPackageManager(const IPackageManager& input);
		virtual ~IPackageManager();

		/// @}
		/** @name Operators*/
		/// @{
	public:
		IPackageManager& operator=(const IPackageManager& other);

		///@}

	public:
		/**
			 * @brief Retrieves a package by name/type
			 * @param packageName Name/Type
			 * @return The first package ptr that matches, if it exists
			 */
		[[nodiscard]] IPackage* package(const char* packageName);

		[[nodiscard]] Utilities::StringList packageList() const;

		/**
			 * @brief Removes a package
			 * @param packageName Name/Type
			 * @return Whether the operation was successful
			 */
		virtual bool removePackage(const char* packageName);

		/**
			 * @brief Adds a package to the rig
			 * @param package Package to add
			 * @param overwrite Replaces an existing package, if one of the same type is found
			 * @return Whether the operation was a success
			 */
		bool addPackage(IPackage* package, bool overwrite = true) const;

		/**
			 * @brief
			 * @return Number of packages
			 */
		[[nodiscard]] uint64_t packageCount() const;



	private:
		struct Impl;
		Impl* pImpl;
	};

}
