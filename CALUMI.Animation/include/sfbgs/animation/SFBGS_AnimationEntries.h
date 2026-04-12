//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Common.h"
#include "math/Math.h"


namespace CALUMI::SFBGS{

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
		explicit RotationPrefix(const unsigned char* buffer);
		~RotationPrefix();

		/// @}
		/// @name Data
		/// @{
		
        [[nodiscard]] int8_t first() const;
        void setFirst(int8_t input);
        [[nodiscard]] bool firstFlag() const;
        void setFirstFlag(bool input);
        [[nodiscard]] int8_t second() const;
        void setSecond(int8_t input);
        [[nodiscard]] bool secondFlag() const;
        void setSecondFlag(bool input);
        [[nodiscard]] int8_t third() const;
        void setThird(int8_t input);
        [[nodiscard]] bool thirdFlag() const;
        void setThirdFlag(bool input);
        [[nodiscard]] uint8_t count() const;
        void setCount(uint8_t input);
        [[nodiscard]] uint8_t missing() const;
        void setMissing(uint8_t input);

		[[nodiscard]] const void* const data() const;

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
        [[nodiscard]] int8_t first() const;
        void setFirst(int8_t input);
        [[nodiscard]] int8_t second() const;
        void setSecond(int8_t input);
        [[nodiscard]] int8_t third() const;
        void setThird(int8_t input);

		RotationEntry();
		RotationEntry(const RotationEntry& input);
		~RotationEntry();
		RotationEntry(const int8_t& first, const int8_t& second, const int8_t& third);
		explicit RotationEntry(const unsigned char* buffer); //warning this method expects a 3 byte buffer. It uses memcpy for quick transfer, caution advised

		[[nodiscard]] const void* const data() const;

		RotationEntry& operator=(const RotationEntry& input);
		RotationEntry& operator=(const unsigned char* buffer); //warning this method expects a 3 byte buffer. It uses memcpy for quick transfer, caution advised

	private:
		struct Impl;
		Impl* pImpl;
	};

	//TODO: Documentation for SFBGS Translation Prefix
	struct CALUMIANIMATION_API TranslationPrefix
	{
		[[nodiscard]] int16_t x() const;
		[[nodiscard]] int16_t y() const;
		[[nodiscard]] int16_t z() const;
		[[nodiscard]] uint16_t count() const;

		void setX(int16_t x);
		void setY(int16_t x);
		void setZ(int16_t x);
		void setCount(uint16_t x);

		TranslationPrefix();
		~TranslationPrefix();
		TranslationPrefix(int16_t x, int16_t y, int16_t z, uint16_t count);
		TranslationPrefix(const TranslationPrefix& input);
		explicit TranslationPrefix(const unsigned char* buffer); //expects 8 bytes
		//Count is not considered in comparison
		bool operator==(const TranslationPrefix& other) const;
		bool operator!=(const TranslationPrefix& other) const;

		[[nodiscard]] const void* const data() const;

		TranslationPrefix& operator=(const TranslationPrefix& input);
		TranslationPrefix& operator=(const unsigned char* buffer);

	private:
		struct Impl;
		Impl* pImpl;
	};

	//TODO: Documentation for SFBGS Translation Entry
	struct CALUMIANIMATION_API TranslationEntry
	{
		[[nodiscard]] int8_t x() const;
		[[nodiscard]] int8_t y() const;
		[[nodiscard]] int8_t z() const;
		void setX(int8_t x);
		void setY(int8_t y);
		void setZ(int8_t z);

		TranslationEntry();
		~TranslationEntry();
		TranslationEntry(int8_t x, int8_t y, int8_t z);
		explicit TranslationEntry(const unsigned char* buffer); //3 bytes expected
		TranslationEntry(const TranslationEntry& input);
		TranslationEntry& operator=(const unsigned char* buffer); //3 bytes expected
		TranslationEntry& operator=(const TranslationEntry& input);

		[[nodiscard]] const void* const data() const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 * @brief A convenient container for storing a compressed rotation in the SFBGS namespace
	 */
	struct CALUMIANIMATION_API CompressedRotation
	{
		CompressedRotation(const RotationPrefix& prefix, const RotationEntry& entry);
		~CompressedRotation();

		[[nodiscard]] const RotationPrefix& prefix() const;
		[[nodiscard]] const RotationEntry& suffix() const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 * @brief A convenient container for storing a compressed translation in the SFBGS namespace
	 */
	struct CALUMIANIMATION_API CompressedTranslation
	{
		CompressedTranslation(const TranslationPrefix& prefix, const TranslationEntry& entry);
		~CompressedTranslation();

		[[nodiscard]] const TranslationPrefix& prefix() const;
		[[nodiscard]] const TranslationEntry& suffix() const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	VECTORDEC(RotationEntrySequence, RotationEntry)
	VECTORDEC(TranslationEntrySequence, TranslationEntry)
	VECTORDEC(RotationPrefixSequence, RotationPrefix)
	VECTORDEC(TranslationPrefixSequence, TranslationPrefix)

	

    CALUMIANIMATION_API CompressedRotation GetSFBGSRotationPair(const Math::Quaternion& input);
    CALUMIANIMATION_API Math::Quaternion GetUniversalRotation(const RotationPrefix& prefix, const RotationEntry& suffix);

    CALUMIANIMATION_API CompressedTranslation GetSFBGSTranslationPair(const Math::Vector3D& input, const float& highPrecision, const float& lowPrecision);
    CALUMIANIMATION_API Math::Vector3D GetUniversalTranslation(const TranslationPrefix& prefix, const TranslationEntry& suffix, const float& highPrecision, const float& lowPrecision);
	
    CALUMIANIMATION_API TranslationPrefixSequence UnfoldTranslationPrefixSequence(const TranslationPrefixSequence& input);
    CALUMIANIMATION_API TranslationPrefixSequence FoldTranslationPrefixSequence(const TranslationPrefixSequence& input);

    CALUMIANIMATION_API RotationPrefixSequence UnfoldRotationPrefixSequence(const RotationPrefixSequence& input);
    CALUMIANIMATION_API RotationPrefixSequence FoldRotationPrefixSequence (const RotationPrefixSequence& input);

}
