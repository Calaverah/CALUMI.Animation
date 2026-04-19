//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include "CALUMI_Common.h"
#include "CALUMI_Utilities.h"

namespace CALUMI::Utilities
{
	/// @addtogroup Singletons
	/// @{
	/**
	 * @brief The Hash Registry holds data that's established from reverse engineering efforts as well as from
	 * the user in order to reverse hash keys into the input values that generated them.
	 * @details At present this library only needs to consider simple string hashes so it is not expected to
	 * expand beyond that purpose.
	 */
	class HashRegistry {
	private:
		HashRegistry();
		~HashRegistry();

	public:
		HashRegistry(const HashRegistry&) = delete;
		HashRegistry& operator=(const HashRegistry&) = delete;
		HashRegistry(HashRegistry&&) = delete;
		HashRegistry& operator=(HashRegistry&&) = delete;

	public:
		/**
			 * @brief Returns the singleton for the Hashing Registry
			 * @return
			 */
		[[nodiscard]] CALUMIANIMATION_API static HashRegistry& getInstance();

		/**
			 * @brief Returns the string that generates the given hash key, if known currently.
			 * @param hash Hash Key to check known values for
			 * @return Nullptr if there is no known input value, however if the key is 0x0, then the entry may just be an emptry string. With that in mind, it is important to also check if the known input exists.
			 */
		[[nodiscard]] CALUMIANIMATION_API const char* getKnownHashString(uint32_t hash) const;
		/**
			 * @brief A simple way to see if a known input is paired to this hash key
			 * @param hash
			 * @return
			 */
		[[nodiscard]] CALUMIANIMATION_API bool isKnownHash(uint32_t hash) const;
		/**
			 * @brief Will add this string to the registry
			 * @param string String to add
			 * @return Hash key generated from string input
			 */
		CALUMIANIMATION_API uint32_t registerHash(const char* string) const;
		/**
			 * @brief A simple way to get a list of the user's registered strings for storage/cache outside the application
			 * @return
			 */
		[[nodiscard]] CALUMIANIMATION_API StringList getRegisteredStrings() const;

#ifdef DEBUG_BUILD
		/**
		 * @brief Tests the known hashes to check if the hash calculated matches the hash value assumed
		 * @return List of strings/keys that did not pass
		 */
		[[nodiscard]] CALUMIANIMATION_API StringList testKnownHashes() const;

#endif

	private:
		struct Impl;
		Impl* pImpl;

	};

	/// @}
}


	/**
		 * @addtogroup extern_c
		 * @{
		 * @defgroup c_hash Hashes
		 * @brief Provides C Style API for the known hashing algorithms
		 * @{
		 */
	extern "C" {
	/**
			 * @brief Generates a hash for the given string using the current BGS Hashing algorithm
			 * @param string Null terminated c string
			 * @return Generated hash
			 */
	CALUMIANIMATION_API uint32_t BGS_Str_CRC32(const char* string);
	}
	/// @}
	/// @}
