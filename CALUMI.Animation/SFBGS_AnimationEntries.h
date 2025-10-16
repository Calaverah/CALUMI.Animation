//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_Math.h"


namespace CALUMI{ namespace SFBGS{

	struct CALUMIANIMATION_API RotationPrefix
	{
		int8_t first : 7 = 0;
		bool firstFlag : 1 = false;

		int8_t second : 7 = 0;
		bool secondFlag : 1 = false;

		int8_t third : 7 = 0;
		bool thirdFlag : 1 = false;

		uint8_t count : 6 = 0;
		uint8_t missing : 2 = 0b11;

		RotationPrefix() = default;
		RotationPrefix(int8_t X, bool QX, int8_t Y, bool QY, int8_t Z, bool QZ, uint8_t C, uint8_t ID);

		//Count is not considered in comparison
		bool operator==(const RotationPrefix& other) const;
		bool operator!=(const RotationPrefix& other) const;

	};

	struct CALUMIANIMATION_API RotationEntry
	{
		int8_t first = 0;
		int8_t second = 0;
		int8_t third = 0;

		RotationEntry() = default;
		RotationEntry(const int8_t& first, const int8_t& second, const int8_t& third)
			: first(first), second(second), third(third)
		{
		}
	};

	struct CALUMIANIMATION_API TranslationPrefix
	{
		int16_t x = 0;
		int16_t y = 0;
		int16_t z = 0;
		uint16_t count = 1;

		TranslationPrefix() = default;
		TranslationPrefix(int16_t x, int16_t y, int16_t z, uint16_t count)
			: x(x), y(y), z(z), count(count)
		{
		}
		//Count is not considered in comparison
		bool operator==(const TranslationPrefix& other) const;
		bool operator!=(const TranslationPrefix& other) const;
	};

	struct CALUMIANIMATION_API TranslationEntry
	{
		int8_t x = 0;
		int8_t y = 0;
		int8_t z = 0;

		TranslationEntry() = default;
		TranslationEntry(int8_t x, int8_t y, int8_t z)
			: x(x), y(y), z(z)
		{
		}
	};

	Utilities::PairContainer<RotationPrefix, RotationEntry> GetSFBGSRotationPair(const CALUMI::Math::Quaternion& input);
	CALUMI::Math::Quaternion GetUniversalRotation(const CALUMI::SFBGS::RotationPrefix& prefix, const CALUMI::SFBGS::RotationEntry& suffix);

	Utilities::PairContainer<TranslationPrefix, TranslationEntry> GetSFBGSTranslationPair(const CALUMI::Math::Vector3D& input, const float& highPrecision, const float& lowPrecision);
	CALUMI::Math::Vector3D GetUniversalTranslation(const CALUMI::SFBGS::TranslationPrefix& prefix, const CALUMI::SFBGS::TranslationEntry& suffix, const float& highPrecision, const float& lowPrecision);
	
	Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> UnfoldTranslationPrefixSequence(const Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix>& input);
	Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> FoldTranslationPrefixSequence(const Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix>& input);

	Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix> UnfoldRotationPrefixSequence(const Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix>& input);
	Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix> FoldRotationPrefixSequence (const Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix>& input);

#pragma warning(disable: 4661)
	template struct CALUMIANIMATION_API Utilities::VectorContainer<RotationEntry>;
	template struct CALUMIANIMATION_API Utilities::VectorContainer<RotationPrefix>;
	template struct CALUMIANIMATION_API Utilities::VectorContainer<TranslationEntry>;
	template struct CALUMIANIMATION_API Utilities::VectorContainer<TranslationPrefix>;
#pragma warning(default: 4661)	

	}}
