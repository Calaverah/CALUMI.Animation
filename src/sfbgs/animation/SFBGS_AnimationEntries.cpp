//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


// ReSharper disable CppConstValueFunctionReturnType
#include <AnimStarfield>
#include <utility>
#include <vector>
#include <algorithm>
#include <cmath>
#include "internal/internalvectordef.h"
// ReSharper disable once CppUnusedIncludeDirective
#include <cstring>


namespace CALUMI::SFBGS
{

#pragma region ROTATIONPREFIX

	struct RotationPrefix::Impl
	{
		int8_t m_first : 7 = 0;
		bool m_firstFlag : 1 = false;

		int8_t m_second : 7 = 0;
		bool m_secondFlag : 1 = false;

		int8_t m_third : 7 = 0;
		bool m_thirdFlag : 1 = false;

		uint8_t m_count : 6 = 0;
		uint8_t m_missing : 2 = 0b11;
		Impl() = default;
	};
	RotationPrefix::RotationPrefix()
	{
		pImpl = new Impl;
	}
	RotationPrefix::RotationPrefix(const RotationPrefix& input) : RotationPrefix()
	{
		pImpl->m_first = input.pImpl->m_first;
		pImpl->m_firstFlag = input.pImpl->m_firstFlag;
		pImpl->m_second = input.pImpl->m_second;
		pImpl->m_secondFlag = input.pImpl->m_secondFlag;
		pImpl->m_third = input.pImpl->m_third;
		pImpl->m_thirdFlag = input.pImpl->m_thirdFlag;
		pImpl->m_count= input.pImpl->m_count;
		pImpl->m_missing = input.pImpl->m_missing;
	}
	RotationPrefix::~RotationPrefix()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	RotationPrefix::RotationPrefix(const int8_t X, const bool QX, const int8_t Y, const bool QY, const int8_t Z, const bool QZ, const uint8_t C, const uint8_t ID) : RotationPrefix()
	{
		pImpl->m_first = std::clamp(static_cast<int>(X), 0b0, 0b01111111);
		pImpl->m_firstFlag = std::clamp(static_cast<int>(QX), 0b0, 0b01);

		pImpl->m_second = std::clamp(static_cast<int>(Y), 0b0, 0b01111111);
		pImpl->m_secondFlag = std::clamp(static_cast<int>(QY), 0b0, 0b01);

		pImpl->m_third = std::clamp(static_cast<int>(Z), 0b0, 0b01111111);
		pImpl->m_thirdFlag = std::clamp(static_cast<int>(QZ), 0b0, 0b01);

		pImpl->m_count = std::clamp(static_cast<int>(C), 0b0, 0b00111111);
		pImpl->m_missing = std::clamp(static_cast<int>(ID), 0b0, 0b11);
	}

	RotationPrefix::RotationPrefix(const unsigned char* buffer) : RotationPrefix()
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
	}

	bool RotationPrefix::operator==(const RotationPrefix& other) const
	{
		return pImpl->m_first == other.pImpl->m_first
			&& pImpl->m_firstFlag == other.pImpl->m_firstFlag
			&& pImpl->m_second == other.pImpl->m_second
			&& pImpl->m_secondFlag == other.pImpl->m_secondFlag
			&& pImpl->m_third == other.pImpl->m_third
			&& pImpl->m_thirdFlag == other.pImpl->m_thirdFlag
			&& pImpl->m_missing == other.pImpl->m_missing;
	}

	bool RotationPrefix::operator!=(const RotationPrefix& other) const
	{
		return !(*this == other);
	}
	RotationPrefix& RotationPrefix::operator=(const RotationPrefix& input)
	{
		pImpl->m_first = input.pImpl->m_first;
		pImpl->m_firstFlag = input.pImpl->m_firstFlag;
		pImpl->m_second = input.pImpl->m_second;
		pImpl->m_secondFlag = input.pImpl->m_secondFlag;
		pImpl->m_third = input.pImpl->m_third;
		pImpl->m_thirdFlag = input.pImpl->m_thirdFlag;
		pImpl->m_count = input.pImpl->m_count;
		pImpl->m_missing = input.pImpl->m_missing;
		return *this;
	}
	RotationPrefix& RotationPrefix::operator=(const unsigned char* buffer)
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
		return *this;
	}
	int8_t RotationPrefix::first() const
	{
		return pImpl->m_first;
	}
	void RotationPrefix::setFirst(const int8_t input) const
	{
		pImpl->m_first = input;
	}
	bool RotationPrefix::firstFlag() const
	{
		return pImpl->m_firstFlag;
	}
	void RotationPrefix::setFirstFlag(const bool input) const
	{
		pImpl->m_firstFlag = input;
	}
	int8_t RotationPrefix::second() const
	{
		return pImpl->m_second;
	}
	void RotationPrefix::setSecond(const int8_t input) const
	{
		pImpl->m_second = input;
	}
	bool RotationPrefix::secondFlag() const
	{
		return pImpl->m_secondFlag;
	}
	void RotationPrefix::setSecondFlag(const bool input) const
	{
		pImpl->m_secondFlag = input;
	}
	int8_t RotationPrefix::third() const
	{
		return pImpl->m_third;
	}
	void RotationPrefix::setThird(const int8_t input) const
	{
		pImpl->m_third = input;
	}
	bool RotationPrefix::thirdFlag() const
	{
		return pImpl->m_thirdFlag;
	}
	void RotationPrefix::setThirdFlag(const bool input) const
	{
		pImpl->m_thirdFlag = input;
	}
	uint8_t RotationPrefix::count() const
	{
		return pImpl->m_count;
	}
	void RotationPrefix::setCount(const uint8_t input) const
	{
		pImpl->m_count = input;
	}
	uint8_t RotationPrefix::missing() const
	{
		return pImpl->m_missing;
	}
	void RotationPrefix::setMissing(const uint8_t input) const
	{
		pImpl->m_missing = input;
	}

	const void* const RotationPrefix::data() const
	{
		return pImpl;
	}
#pragma endregion

#pragma region ROTATION
	struct RotationEntry::Impl
	{
		int8_t m_first = 0;
		int8_t m_second = 0;
		int8_t m_third = 0;
		Impl() = default;
	};
	RotationEntry::~RotationEntry()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	RotationEntry::RotationEntry()
	{
		pImpl = new Impl;
	}
	RotationEntry::RotationEntry(const RotationEntry& input) : RotationEntry()
	{
		pImpl->m_first = input.pImpl->m_first;
		pImpl->m_second = input.pImpl->m_second;
		pImpl->m_third = input.pImpl->m_third;
	}
	RotationEntry::RotationEntry(const int8_t& first, const int8_t& second, const int8_t& third) : RotationEntry()
	{
		pImpl->m_first = first;
		pImpl->m_second = second;
		pImpl->m_third = third;
	}
	RotationEntry::RotationEntry(const unsigned char* buffer) : RotationEntry()
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
	}
	RotationEntry& RotationEntry::operator=(const RotationEntry& input)
	{
		pImpl->m_first = input.pImpl->m_first;
		pImpl->m_second = input.pImpl->m_second;
		pImpl->m_third = input.pImpl->m_third;
		return *this;
	}

	RotationEntry& RotationEntry::operator=(const unsigned char* buffer)
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
		return *this;
	}

	int8_t RotationEntry::first() const
	{
		return pImpl->m_first;
	}
	void RotationEntry::setFirst(const int8_t input) const
	{
		pImpl->m_first = input;
	}
	int8_t RotationEntry::second() const
	{
		return pImpl->m_second;
	}
	void RotationEntry::setSecond(const int8_t input) const
	{
		pImpl->m_second = input;
	}
	int8_t RotationEntry::third() const
	{
		return pImpl->m_third;
	}
	void RotationEntry::setThird(const int8_t input) const
	{
		pImpl->m_third = input;
	}

	const void* const RotationEntry::data() const
	{
		return pImpl;
	}
#pragma endregion


#pragma region TRANSLATIONPREFIX

	struct TranslationPrefix::Impl
	{
		int16_t m_x = 0;
		int16_t m_y = 0;
		int16_t m_z = 0;
		uint16_t m_count = 1; //This RLE uses non-zero based numbering for some reason
		Impl() = default;
	};
	TranslationPrefix::TranslationPrefix()
	{
		pImpl = new Impl;
	}
	TranslationPrefix::~TranslationPrefix()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	int16_t TranslationPrefix::x() const { return pImpl->m_x; }
	int16_t TranslationPrefix::y() const { return pImpl->m_y; }
	int16_t TranslationPrefix::z() const { return pImpl->m_z; }
	uint16_t TranslationPrefix::count() const { return pImpl->m_count; }
	void TranslationPrefix::setX(const int16_t x) const { pImpl->m_x = x; }
	void TranslationPrefix::setY(const int16_t y) const { pImpl->m_y = y; }
	void TranslationPrefix::setZ(const int16_t z) const { pImpl->m_z = z; }
	void TranslationPrefix::setCount(const uint16_t count) const { pImpl->m_count = count; }

	TranslationPrefix::TranslationPrefix(const int16_t x, const int16_t y, const int16_t z, const uint16_t count) :
	TranslationPrefix()
	{
		pImpl->m_x = x; pImpl->m_y = y; pImpl->m_z = z; pImpl->m_count = count;
	}
	TranslationPrefix::TranslationPrefix(const TranslationPrefix& input) : TranslationPrefix()
	{
		pImpl->m_x = input.pImpl->m_x; pImpl->m_y = input.pImpl->m_y; pImpl->m_z = input.pImpl->m_z; pImpl->m_count = input.pImpl->m_count;
	}
	TranslationPrefix::TranslationPrefix(const unsigned char* buffer) : TranslationPrefix()
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
	}
	TranslationPrefix& TranslationPrefix::operator=(const unsigned char* buffer)
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
		return *this;
	}
	TranslationPrefix& TranslationPrefix::operator=(const TranslationPrefix& input)
	{
		pImpl->m_x = input.pImpl->m_x; pImpl->m_y = input.pImpl->m_y; pImpl->m_z = input.pImpl->m_z; pImpl->m_count = input.pImpl->m_count; return *this;
	}
	bool TranslationPrefix::operator==(const TranslationPrefix& other) const
	{
		return pImpl->m_x == other.pImpl->m_x && pImpl->m_y == other.pImpl->m_y && pImpl->m_z == other.pImpl->m_z;
	}
	bool TranslationPrefix::operator!=(const TranslationPrefix& other) const
	{
		return !(*this == other);
	}
	const void* const TranslationPrefix::data() const
	{
		return pImpl;
	}
#pragma endregion

	static std::tuple<int8_t, int8_t, bool> GetSFBGSRotationComponents(const float& component)
	{
		constexpr float lowPrecision = CLA_SQRT1_2 / 64.0; //1.0 / (sqrt(2) * 64.0);		//2^6
		constexpr float highPrecision = CLA_SQRT1_2 / 16384.0; //1.0 / (sqrt(2) * 16384.0);	//2^14

		float value = 0.0;
		if(std::abs(component) != 0.0f)
			value = component + std::copysignf(highPrecision / 10.0f, component); //increase the value by just enough to correct rounding errors on conversion to int

		int8_t sVal = 0;
		int8_t lVal = 0;
		float tempS = 0;
		bool flag = false;

		lVal = std::clamp(static_cast<int>(value / lowPrecision), -64, 63);
		if (lVal == -64 || lVal == 63)
		{
			tempS = (value - lVal * lowPrecision) / highPrecision;
			if (tempS < -128 || tempS > 127)
			{
				sVal = static_cast<int8_t>((value - lVal * lowPrecision) / (8 * highPrecision));
				flag = true;
			}
			else
			{
				sVal = static_cast<int8_t>((value - lVal * lowPrecision) / highPrecision);
			}
		}
		else
		{
			sVal = static_cast<int8_t>(fmod(value, lowPrecision) / highPrecision);
			lVal = static_cast<int8_t>((value - sVal * highPrecision) / lowPrecision);
		}
		return std::make_tuple(lVal, sVal, flag);
	}

#pragma region TRANSLATION
	struct TranslationEntry::Impl
	{
		int8_t m_x = 0;
		int8_t m_y = 0;
		int8_t m_z = 0;
		Impl() = default;
	};

	TranslationEntry::TranslationEntry()
	{
		pImpl = new Impl;
	}

	TranslationEntry::~TranslationEntry()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	TranslationEntry::TranslationEntry(const int8_t x, const int8_t y, const int8_t z) : TranslationEntry()
	{
		pImpl->m_x = x; pImpl->m_y = y; pImpl->m_z = z;
	}
	TranslationEntry::TranslationEntry(const unsigned char* buffer) : TranslationEntry()
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
	}
	TranslationEntry::TranslationEntry(const TranslationEntry& input) : TranslationEntry()
	{
		pImpl->m_x = input.pImpl->m_x; pImpl->m_y = input.pImpl->m_y; pImpl->m_z = input.pImpl->m_z;
	}
	TranslationEntry& TranslationEntry::operator=(const unsigned char* buffer)
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
		return *this;
	}
	TranslationEntry& TranslationEntry::operator=(const TranslationEntry& input)
	{
		pImpl->m_x = input.pImpl->m_x; pImpl->m_y = input.pImpl->m_y; pImpl->m_z = input.pImpl->m_z;
		return *this;
	}
	int8_t TranslationEntry::x() const { return pImpl->m_x; }
	int8_t TranslationEntry::y() const { return pImpl->m_y; }
	int8_t TranslationEntry::z() const { return pImpl->m_z; }
	void TranslationEntry::setX(const int8_t x) const { pImpl->m_x = x; }
	void TranslationEntry::setY(const int8_t y) const { pImpl->m_y = y; }
	void TranslationEntry::setZ(const int8_t z) const { pImpl->m_z = z; }

	const void* const TranslationEntry::data() const
	{
		return pImpl;
	}
#pragma endregion

	static std::pair<int16_t, int8_t> GetSFBGSTranslationComponents(const double& component, const float& highPrecision, const float& lowPrecision)
	{
		double value = 0.0;
		if (component != 0)
			value = component + std::copysign(highPrecision / 3.0, component); //increase the value by just enough to correct rounding errors on conversion to int

		int8_t sVal = static_cast<int8_t>(fmod(value, lowPrecision) / highPrecision);
		int16_t lVal = static_cast<int16_t>((value - sVal * highPrecision) / lowPrecision);

		return std::make_pair(lVal, sVal);
	}

	TranslationPrefixSequence UnfoldTranslationPrefixSequence(const TranslationPrefixSequence& input)
	{
		TranslationPrefixSequence output;
		uint16_t totalSize = 0;
		for (int i = 0; i<input.size();i++)
		{
			//totalSize++;
			totalSize += input.at(i).count();
		}
		//std::println("totalSize {}", totalSize);
		output.reserve(totalSize);

		for (int i = 0; i < input.size(); i++)
		{
			for (uint32_t j = 0; j < input.at(i).count(); j++) //Translations use classical counters
			{
				TranslationPrefix newPrefix = input.at(i);
				newPrefix.setCount(1);
				output.push_back(newPrefix);
			}
		}
		//std::println("UNFOLD LENGTH {}", output.size());
		return output;
	}

	TranslationPrefixSequence FoldTranslationPrefixSequence(const TranslationPrefixSequence& input)
	{
		if (input.size() < 2)return input;

		TranslationPrefixSequence outputList; outputList.reserve(input.size());
		TranslationPrefix referencePrefix = input.at(0);
		for (uint16_t i = 1; i < input.size(); i++)
		{
			if (referencePrefix == input.at(i) && referencePrefix.count() <0xFFFF)
			{
				referencePrefix.setCount(referencePrefix.count()+1);
			}
			else
			{
				outputList.push_back(referencePrefix);
				referencePrefix = input.at(i);
			}
		}
		outputList.push_back(referencePrefix);
		outputList.shrink_to_fit();
		return outputList;
	}

	RotationPrefixSequence UnfoldRotationPrefixSequence(const RotationPrefixSequence& input)
	{
		RotationPrefixSequence output;
		uint16_t totalSize = 0;
		for (int i =0; i < input.size(); i++)
		{
			totalSize++;
			totalSize += input.at(i).count();
		}

		output.reserve(totalSize);

		for (int z =0; z < input.size(); z++)
		{
			for (uint8_t i = 0; i <= input.at(z).count(); i++) //Rotations use programming counters
			{
				RotationPrefix newPrefix = input.at(z);
				newPrefix.setCount(0);
				output.push_back(newPrefix);
			}
		}
		return output;
	}

	RotationPrefixSequence FoldRotationPrefixSequence(const RotationPrefixSequence& input)
	{
		if (input.size() < 2)return input;

		RotationPrefixSequence outputList;
		outputList.reserve(input.size());

		RotationPrefix referencePrefix = input.at(0);

		for (uint16_t i = 1; i < input.size(); i++)
		{
			if (referencePrefix == input.at(i) && referencePrefix.count() < 63)
			{
				referencePrefix.setCount(referencePrefix.count()+1);
			}
			else
			{
				outputList.push_back(referencePrefix);
				referencePrefix = input.at(i);
			}

		}
		outputList.push_back(referencePrefix);
		outputList.shrink_to_fit();
		return outputList;
	}

	struct CompressedRotation::Impl
	{
		RotationPrefix m_prefix;
		RotationEntry m_suffix;
	};

	CompressedRotation::CompressedRotation(const RotationPrefix& prefix, const RotationEntry& entry) : pImpl(new Impl())
	{
		pImpl->m_prefix = prefix;
		pImpl->m_suffix = entry;
	}

	CompressedRotation::CompressedRotation(const Math::Quaternion& input) : pImpl(new Impl())
	{
		const RotationPrefix prefix;
		Math::Quaternion tempInput = input;

		prefix.setMissing(3);
		if (std::abs(input.x()) > CLA_SQRT1_2) prefix.setMissing(0);
		if (std::abs(input.y()) > CLA_SQRT1_2) prefix.setMissing(1);
		if (std::abs(input.z()) > CLA_SQRT1_2) prefix.setMissing(2);

		//We must flip the quaternion values such that the missing value is positive. When derived, the missing will always be positive. So long as all values flip the quaternion is equal.
		if (prefix.missing() == 0 && input.x() < 0) tempInput = -input;
		if (prefix.missing() == 1 && input.y() < 0) tempInput = -input;
		if (prefix.missing() == 2 && input.z() < 0) tempInput = -input;
		if (prefix.missing() == 3 && input.w() < 0) tempInput = -input;

		std::vector<float> fResults; fResults.reserve(3);
		if (prefix.missing() != 0) fResults.push_back(tempInput.x());
		if (prefix.missing() != 1) fResults.push_back(tempInput.y());
		if (prefix.missing() != 2) fResults.push_back(tempInput.z());
		if (prefix.missing() != 3) fResults.push_back(tempInput.w());

		const auto first = GetSFBGSRotationComponents(fResults.at(0));
		const auto second = GetSFBGSRotationComponents(fResults.at(1));
		const auto third = GetSFBGSRotationComponents(fResults.at(2));

		prefix.setFirst(std::get<0>(first));
		prefix.setFirstFlag(std::get<2>(first));
		prefix.setSecond(std::get<0>(second));
		prefix.setSecondFlag(std::get<2>(second));
		prefix.setThird(std::get<0>(third));
		prefix.setThirdFlag(std::get<2>(third));
		prefix.setCount(0);

		const RotationEntry suffix(std::get<1>(first), std::get<1>(second), std::get<1>(third) );

		pImpl->m_prefix = prefix;
		pImpl->m_suffix = suffix;
	}

	CompressedRotation::~CompressedRotation()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}

	const RotationPrefix& CompressedRotation::prefix() const
	{
		return pImpl->m_prefix;
	}

	const RotationEntry& CompressedRotation::suffix() const
	{
		return pImpl->m_suffix;
	}

	Math::Quaternion CompressedRotation::toQuaternion() const
	{
		constexpr double lowPrecision		= CLA_SQRT1_2 / 64.0;
		constexpr double highPrecision	= CLA_SQRT1_2 / 16384.0;

		const uint8_t s1 = 1 << (3 * pImpl->m_prefix.firstFlag());
		const uint8_t s2 = 1 << (3 * pImpl->m_prefix.secondFlag());
		const uint8_t s3 = 1 << (3 * pImpl->m_prefix.thirdFlag());

		const double component1 = lowPrecision * pImpl->m_prefix.first() + highPrecision * pImpl->m_suffix.first() * s1;
		const double component2 = lowPrecision * pImpl->m_prefix.second() + highPrecision * pImpl->m_suffix.second()
		* s2;
		const double component3 = lowPrecision * pImpl->m_prefix.third() + highPrecision * pImpl->m_suffix.third() * s3;
		const double component4 = sqrt(1 - pow(component1, 2) - pow(component2, 2) - pow(component3, 2));

		Math::Quaternion output;

		switch (pImpl->m_prefix.missing())
		{
		case 0:     output.setX(static_cast<float>(component4)); output.setY(static_cast<float>(component1)); output.setZ(static_cast<float>(component2)); output.setW(static_cast<float>(component3)); break;
		case 1:     output.setY(static_cast<float>(component4)); output.setX(static_cast<float>(component1)); output.setZ(static_cast<float>(component2)); output.setW(static_cast<float>(component3)); break;
		case 2:     output.setZ(static_cast<float>(component4)); output.setX(static_cast<float>(component1)); output.setY(static_cast<float>(component2)); output.setW(static_cast<float>(component3)); break;
		default:    output.setW(static_cast<float>(component4)); output.setX(static_cast<float>(component1)); output.setY(static_cast<float>(component2)); output.setZ(static_cast<float>(component3)); break;
		}

		return output;
	}

	struct CompressedTranslation::Impl
	{
		TranslationPrefix m_prefix;
		TranslationEntry m_suffix;
	};

	CompressedTranslation::CompressedTranslation(const TranslationPrefix& prefix, const TranslationEntry& entry) : pImpl(new Impl())
	{
		pImpl->m_prefix = prefix;
		pImpl->m_suffix = entry;
	}

	CompressedTranslation::CompressedTranslation(const Math::Vector3D& input, const float& highPrecision, const float& lowPrecision) : pImpl(new Impl())
	{
		auto [xPrefix, xSuffix] = GetSFBGSTranslationComponents(input.x(), highPrecision, lowPrecision);
		auto [yPrefix, ySuffix] = GetSFBGSTranslationComponents(input.y(), highPrecision, lowPrecision);
		auto [zPrefix, zSuffix] = GetSFBGSTranslationComponents(input.z(), highPrecision, lowPrecision);

		const TranslationPrefix prefix(xPrefix, yPrefix, zPrefix, 1);
		const TranslationEntry suffix(xSuffix, ySuffix, zSuffix);

		pImpl->m_prefix = prefix;
		pImpl->m_suffix = suffix;
	}

	CompressedTranslation::~CompressedTranslation()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}

	const TranslationPrefix& CompressedTranslation::prefix() const
	{
		return pImpl->m_prefix;
	}

	const TranslationEntry& CompressedTranslation::suffix() const
	{
		return pImpl->m_suffix;
	}

	Math::Vector3D CompressedTranslation::toVector3D(const float lowPrecision, const float highPrecision) const
	{
		Math::Vector3D output(
			pImpl->m_prefix.x() * lowPrecision + pImpl->m_suffix.x() * highPrecision,
			pImpl->m_prefix.y() * lowPrecision + pImpl->m_suffix.y() * highPrecision,
			pImpl->m_prefix.z() * lowPrecision + pImpl->m_suffix.z() * highPrecision);
		return output;
	}

	VECTORDEF(RotationEntrySequence, RotationEntry)
	VECTORDEF(TranslationEntrySequence, TranslationEntry)
	VECTORDEF(RotationPrefixSequence, RotationPrefix)
	VECTORDEF(TranslationPrefixSequence, TranslationPrefix)

}
