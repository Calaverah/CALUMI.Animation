//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Common.h"
#include "math/Math.h"


namespace CALUMI{ namespace SFBGS{

	/**
	 * @brief Prefix RLE based entry component for Rotations
	 */
	struct CALUMIANIMATION_API RotationPrefix
	{
		/// @name Constructors
		/// @{

		RotationPrefix();
		RotationPrefix(const RotationPrefix& input);
		RotationPrefix(int8_t X, bool QX, int8_t Y, bool QY, int8_t Z, bool QZ, uint8_t C, uint8_t ID);
		/**
		 * @brief Raw copy constructor
		 * @param buffer Expects a 4 byte buffer
		 */
		RotationPrefix(const unsigned char* buffer);
		~RotationPrefix();

		/// @}
		/// @name Data
		/// @{
		
		int8_t First() const;
		void First(int8_t input);
		bool FirstFlag() const;
		void FirstFlag(bool input);
		int8_t Second() const;
		void Second(int8_t input);
		bool SecondFlag() const;
		void SecondFlag(bool input);
		int8_t Third() const;
		void Third(int8_t input);
		bool ThirdFlag() const;
		void ThirdFlag(bool input);
		uint8_t Count() const;
		void Count(uint8_t input);
		uint8_t Missing() const;
		void Missing(uint8_t input);

		const void* const getRawData() const;

		/// @}
		/// @name Operators
		/// @{
		
		/**
		 * @brief Equality operator
		 * @param other 
		 * @return Returns true if all data, excluding the RLE count, are equal
		 */
		bool operator==(const RotationPrefix& other) const;
		/**
		 * @brief Equality operator
		 * @param other
		 * @return Returns true if any data, excluding the RLE count, are not equal
		 */
		bool operator!=(const RotationPrefix& other) const;

		RotationPrefix& operator=(const RotationPrefix& input);
		/**
		 * @brief Raw data assignment
		 * @param buffer 4 bytes expected
		 * @return 
		 */
		RotationPrefix& operator=(const unsigned char* buffer);

		inline bool operator<(const RotationPrefix& other) { return false; }

		/// @}
	private:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 * @brief Basic entry component for Rotations
	 */
	struct CALUMIANIMATION_API RotationEntry
	{
		//TODO: Documentation for SFBGS Rotation Entry
		int8_t First() const;
		void First(int8_t input);
		int8_t Second() const;
		void Second(int8_t input);
		int8_t Third() const;
		void Third(int8_t input);

		RotationEntry();
		RotationEntry(const RotationEntry& input);
		~RotationEntry();
		RotationEntry(const int8_t& first, const int8_t& second, const int8_t& third);
		RotationEntry(const unsigned char* buffer); //warning this method expects a 3 byte buffer. It uses memcpy for quick transfer, caution advised

		const void* const getRawData() const;

		RotationEntry& operator=(const RotationEntry& input);
		RotationEntry& operator=(const unsigned char* buffer); //warning this method expects a 3 byte buffer. It uses memcpy for quick transfer, caution advised

		inline bool operator<(const RotationEntry& other) { return false; }

	private:
		struct Impl;
		Impl* pImpl;
	};

	//TODO: Documentation for SFBGS Translation Prefix
	struct CALUMIANIMATION_API TranslationPrefix
	{
		int16_t getX() const;
		int16_t getY() const;
		int16_t getZ() const;
		uint16_t getCount() const;

		void setX(int16_t x);
		void setY(int16_t x);
		void setZ(int16_t x);
		void setCount(uint16_t x);

		TranslationPrefix();
		~TranslationPrefix();
		TranslationPrefix(int16_t x, int16_t y, int16_t z, uint16_t count);
		TranslationPrefix(const TranslationPrefix& input);
		TranslationPrefix(const unsigned char* buffer); //expects 8 bytes
		//Count is not considered in comparison
		bool operator==(const TranslationPrefix& other) const;
		bool operator!=(const TranslationPrefix& other) const;

		const void* const getRawData() const;

		TranslationPrefix& operator=(const TranslationPrefix& input);
		TranslationPrefix& operator=(const unsigned char* buffer);

		inline bool operator<(const TranslationPrefix& other) { return false; }

	private:
		struct Impl;
		Impl* pImpl;
	};

	//TODO: Documentation for SFBGS Translation Entry
	struct CALUMIANIMATION_API TranslationEntry
	{
		int8_t getX() const;
		int8_t getY() const;
		int8_t getZ() const;
		void setX(int8_t x);
		void setY(int8_t y);
		void setZ(int8_t z);

		TranslationEntry();
		~TranslationEntry();
		TranslationEntry(int8_t x, int8_t y, int8_t z);
		TranslationEntry(const unsigned char* buffer); //3 bytes expected
		TranslationEntry(const TranslationEntry& input);
		TranslationEntry& operator=(const unsigned char* buffer); //3 bytes expected
		TranslationEntry& operator=(const TranslationEntry& input);

		inline bool operator<(const TranslationEntry& other) { return false; }

		const void* const getRawData() const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	struct CALUMIANIMATION_API CompressedRotation
	{
		CompressedRotation(const RotationPrefix& prefix, const RotationEntry& entry);
		~CompressedRotation();

		const RotationPrefix& prefix() const;
		const RotationEntry& suffix() const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	struct CALUMIANIMATION_API CompressedTranslation
	{
		CompressedTranslation(const TranslationPrefix& prefix, const TranslationEntry& entry);
		~CompressedTranslation();

		const TranslationPrefix& prefix() const;
		const TranslationEntry& suffix() const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	VECTORDEC(RotationEntrySequence, RotationEntry)
	VECTORDEC(TranslationEntrySequence, TranslationEntry)
	VECTORDEC(RotationPrefixSequence, RotationPrefix)
	VECTORDEC(TranslationPrefixSequence, TranslationPrefix)

	

	CompressedRotation GetSFBGSRotationPair(const CALUMI::Math::Quaternion& input);
	CALUMI::Math::Quaternion GetUniversalRotation(const CALUMI::SFBGS::RotationPrefix& prefix, const CALUMI::SFBGS::RotationEntry& suffix);

	CompressedTranslation GetSFBGSTranslationPair(const CALUMI::Math::Vector3D& input, const float& highPrecision, const float& lowPrecision);
	CALUMI::Math::Vector3D GetUniversalTranslation(const CALUMI::SFBGS::TranslationPrefix& prefix, const CALUMI::SFBGS::TranslationEntry& suffix, const float& highPrecision, const float& lowPrecision);
	
	SFBGS::TranslationPrefixSequence UnfoldTranslationPrefixSequence(const SFBGS::TranslationPrefixSequence& input);
	SFBGS::TranslationPrefixSequence FoldTranslationPrefixSequence(const SFBGS::TranslationPrefixSequence& input);

	SFBGS::RotationPrefixSequence UnfoldRotationPrefixSequence(const SFBGS::RotationPrefixSequence& input);
	SFBGS::RotationPrefixSequence FoldRotationPrefixSequence (const SFBGS::RotationPrefixSequence& input);



    }

    }
