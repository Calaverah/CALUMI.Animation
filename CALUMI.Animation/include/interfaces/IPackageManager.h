//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Utilities.h"

namespace CALUMI {
	namespace UNIV {

		/**
		 * @brief RigPackages provide a convenient way for users to apply game specific data to a rig
		 *
		 * @details Rig Packages allow a rig to have multiple games worth of data applied to the rig without conflicting. The intention is that users can export a single rig for various games at the same time... assuming the game is supported.
		 */
		struct CALUMIANIMATION_API IPackage
		{
			/** @name Initialization */
			/// @{
			IPackage() = default;
			virtual ~IPackage() = default;

			/// @}

		public:
			/**
			 * @brief A way to get a string describing the package.
			 * @return A string of the package type
			 */
			virtual const char* getPackageType() const = 0;

			/**
			 * @brief Serialization of the class
			 * @param indents Amount of spaces for formatting
			 * @return The serialized struct as a strContainer
			 */
			virtual Utilities::StringContainer ToJSON(std::size_t indents) const = 0;

		protected:

			/**
			 * @brief A convenient way to duplicate the struct into a new dynamically allocated version.
			 * @details This function should only be used by the package manager as a simple way to copy data without sharing the struct or using pvt/shared ptrs
			 * @return The new dynamically allocated clone of the package
			 */
			virtual IPackage* Clone() const = 0;

		private:
			friend struct IPackageManager;
		};


		/**
		 * @brief The Rig Package Manager handles the data and memory allocation of the packages for a rig
		 */
		struct CALUMIANIMATION_API IPackageManager
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
			virtual IPackageManager& operator=(const IPackageManager& other);
		
			///@}
		
		public:
			/**
			 * @brief Retrieves a package by name/type
			 * @param packageName Name/Type
			 * @return The first package ptr that matches, if it exists
			 */
			virtual IPackage* getPackage(const char* packageName);

			/**
			 * @brief Returns the package at the given index
			 * @param index 
			 * @return Nullptr if none exists, or out of bounds
			 */
			virtual IPackage* getPackage(int index);
		
			/**
			 * @brief
			 * @param packageName Package name/type to search for
			 * @return Index of package, -1 if not found
			 */
			virtual int findPackage(const char* packageName) const;
		
			/**
			 * @brief Removes a package
			 * @param packageName Name/Type
			 * @return Whether the operation was successful
			 */
			virtual bool RemovePackage(const char* packageName);
		
			/**
			 * @brief Removes a package
			 * @param index Index of package
			 * @return Whether the operation was successful
			 */
			virtual bool RemovePackage(int index);
		
			/**
			 * @brief Adds a package to the rig
			 * @param package Package to add
			 * @param overwrite Replaces an existing package, if one of the same type is found
			 * @return Whether the operation was a success
			 */
			virtual bool AddPackage(IPackage* package, bool overwrite = true);

			/**
			 * @brief 
			 * @return Number of packages 
			 */
			std::size_t packageCount() const;
		
			/**
			 * @brief Serialization
			 * @param indents Spaces for formatting
			 * @return The strContainer of the serialized struct
			 */
			virtual Utilities::StringContainer ToJSON(std::size_t indents) const = 0;
		
		
		
		private:
			struct Impl;
			Impl* pImpl;
		};

} }

#pragma warning(disable: 4661)
_VECTORTEMPLATE(CALUMI::UNIV::IPackage*);
#pragma warning(default: 4661)