//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include <corecrt_math_defines.h>
#include "SFBGS_AnimationEntries.h"
#include <print>
#include <utility>
#include <vector>
#include <algorithm>



namespace CALUMI { namespace SFBGS{

#pragma region ROTATIONPREFIX

	struct RotationPrefix::Impl
	{
		int8_t _first : 7 = 0;
		bool _firstFlag : 1 = false;

		int8_t _second : 7 = 0;
		bool _secondFlag : 1 = false;

		int8_t _third : 7 = 0;
		bool _thirdFlag : 1 = false;

		uint8_t _count : 6 = 0;
		uint8_t _missing : 2 = 0b11;
		Impl() = default;
	};
	RotationPrefix::RotationPrefix()
	{
		pImpl = new Impl;
	}
	RotationPrefix::RotationPrefix(const RotationPrefix& input) : RotationPrefix()
	{
		pImpl->_first = input.pImpl->_first;
		pImpl->_firstFlag = input.pImpl->_firstFlag;
		pImpl->_second = input.pImpl->_second;
		pImpl->_secondFlag = input.pImpl->_secondFlag;
		pImpl->_third = input.pImpl->_third;
		pImpl->_thirdFlag = input.pImpl->_thirdFlag;
		pImpl->_count= input.pImpl->_count;
		pImpl->_missing = input.pImpl->_missing;
	}
	RotationPrefix::~RotationPrefix()
	{
		if (pImpl)
			delete pImpl;
	}
	RotationPrefix::RotationPrefix(int8_t X, bool QX, int8_t Y, bool QY, int8_t Z, bool QZ, uint8_t C, uint8_t ID) : RotationPrefix()
	{
		pImpl->_first = std::clamp((int)X, 0b0, 0b01111111);
		pImpl->_firstFlag = std::clamp((int)QX, 0b0, 0b01);

		pImpl->_second = std::clamp((int)Y, 0b0, 0b01111111);
		pImpl->_secondFlag = std::clamp((int)QY, 0b0, 0b01);

		pImpl->_third = std::clamp((int)Z, 0b0, 0b01111111);
		pImpl->_thirdFlag = std::clamp((int)QZ, 0b0, 0b01);

		pImpl->_count = std::clamp((int)C, 0b0, 0b00111111);
		pImpl->_missing = std::clamp((int)ID, 0b0, 0b11);
	}

	RotationPrefix::RotationPrefix(const unsigned char* buffer) : RotationPrefix()
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
	}

	bool RotationPrefix::operator==(const RotationPrefix& other) const
	{
		return pImpl->_first == other.pImpl->_first 
			&& pImpl->_firstFlag == other.pImpl->_firstFlag 
			&& pImpl->_second == other.pImpl->_second 
			&& pImpl->_secondFlag == other.pImpl->_secondFlag 
			&& pImpl->_third == other.pImpl->_third 
			&& pImpl->_thirdFlag == other.pImpl->_thirdFlag 
			&& pImpl->_missing == other.pImpl->_missing;
	}

	bool RotationPrefix::operator!=(const RotationPrefix& other) const
	{
		return !(*this == other);
	}
	RotationPrefix& RotationPrefix::operator=(const RotationPrefix& input)
	{
		pImpl->_first = input.pImpl->_first;
		pImpl->_firstFlag = input.pImpl->_firstFlag;
		pImpl->_second = input.pImpl->_second;
		pImpl->_secondFlag = input.pImpl->_secondFlag;
		pImpl->_third = input.pImpl->_third;
		pImpl->_thirdFlag = input.pImpl->_thirdFlag;
		pImpl->_count = input.pImpl->_count;
		pImpl->_missing = input.pImpl->_missing;
		return *this;
	}
	RotationPrefix& RotationPrefix::operator=(const unsigned char* buffer)
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
		return *this;
	}
	int8_t SFBGS::RotationPrefix::First() const
	{
		return pImpl->_first;
	}
	void RotationPrefix::First(int8_t input)
	{
		pImpl->_first = input;
	}
	bool SFBGS::RotationPrefix::FirstFlag() const
	{
		return pImpl->_firstFlag;
	}
	void RotationPrefix::FirstFlag(bool input)
	{
		pImpl->_firstFlag = input;
	}
	int8_t SFBGS::RotationPrefix::Second() const
	{
		return pImpl->_second;
	}
	void RotationPrefix::Second(int8_t input)
	{
		pImpl->_second = input;
	}
	bool SFBGS::RotationPrefix::SecondFlag() const
	{
		return pImpl->_secondFlag;
	}
	void RotationPrefix::SecondFlag(bool input)
	{
		pImpl->_secondFlag = input;
	}
	int8_t SFBGS::RotationPrefix::Third() const
	{
		return pImpl->_third;
	}
	void RotationPrefix::Third(int8_t input)
	{
		pImpl->_third = input;
	}
	bool SFBGS::RotationPrefix::ThirdFlag() const
	{
		return pImpl->_thirdFlag;
	}
	void RotationPrefix::ThirdFlag(bool input)
	{
		pImpl->_thirdFlag = input;
	}
	uint8_t SFBGS::RotationPrefix::Count() const
	{
		return pImpl->_count;
	}
	void RotationPrefix::Count(uint8_t input)
	{
		pImpl->_count = input;
	}
	uint8_t SFBGS::RotationPrefix::Missing() const
	{
		return pImpl->_missing;
	}
	void RotationPrefix::Missing(uint8_t input)
	{
		pImpl->_missing = input;	
	}
	const void* const RotationPrefix::getRawData() const
	{
		return pImpl;
	}
#pragma endregion

#pragma region ROTATION
	struct RotationEntry::Impl
	{
		int8_t _first = 0;
		int8_t _second = 0;
		int8_t _third = 0;
		Impl() = default;
	};
	RotationEntry::~RotationEntry()
	{
		if (pImpl)
			delete pImpl;
	}
	RotationEntry::RotationEntry()
	{
		pImpl = new Impl;
	}
	RotationEntry::RotationEntry(const RotationEntry& input) : RotationEntry()
	{
		pImpl->_first = input.pImpl->_first;
		pImpl->_second = input.pImpl->_second;
		pImpl->_third = input.pImpl->_third;
	}
	RotationEntry::RotationEntry(const int8_t& first, const int8_t& second, const int8_t& third) : RotationEntry()
	{
		pImpl->_first = first;
		pImpl->_second = second;
		pImpl->_third = third;
	}
	RotationEntry::RotationEntry(const unsigned char* buffer) : RotationEntry()
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
	}
	RotationEntry& RotationEntry::operator=(const RotationEntry& input)
	{
		pImpl->_first = input.pImpl->_first;
		pImpl->_second = input.pImpl->_second;
		pImpl->_third = input.pImpl->_third;
		return *this;
	}

	RotationEntry& RotationEntry::operator=(const unsigned char* buffer)
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
		return *this;
	}

	int8_t SFBGS::RotationEntry::First() const
	{
		return pImpl->_first;
	}
	void RotationEntry::First(int8_t input)
	{
		pImpl->_first = input;
	}
	int8_t SFBGS::RotationEntry::Second() const
	{
		return pImpl->_second;
	}
	void RotationEntry::Second(int8_t input)
	{
		pImpl->_second = input;
	}
	int8_t SFBGS::RotationEntry::Third() const
	{
		return pImpl->_third;
	}
	void RotationEntry::Third(int8_t input)
	{
		pImpl->_third = input;
	}

	const void* const RotationEntry::getRawData() const
	{
		return pImpl;
	}
#pragma endregion


#pragma region TRANSLATIONPREFIX

	struct TranslationPrefix::Impl
	{
		int16_t _x = 0;
		int16_t _y = 0;
		int16_t _z = 0;
		uint16_t _count = 1; //This RLE uses non-zero based numbering for some reason
		Impl() = default;
	};
	TranslationPrefix::TranslationPrefix()
	{
		pImpl = new Impl;
	}
	TranslationPrefix::~TranslationPrefix()
	{
		if (pImpl) delete pImpl;
	}
	int16_t TranslationPrefix::getX() const { return pImpl->_x; }
	int16_t TranslationPrefix::getY() const { return pImpl->_y; }
	int16_t TranslationPrefix::getZ() const { return pImpl->_z; }
	uint16_t TranslationPrefix::getCount() const { return pImpl->_count; }
	void TranslationPrefix::setX(int16_t x) { pImpl->_x = x; }
	void TranslationPrefix::setY(int16_t y) { pImpl->_y = y; }
	void TranslationPrefix::setZ(int16_t z) { pImpl->_z = z; }
	void TranslationPrefix::setCount(uint16_t count) { pImpl->_count = count; }

	TranslationPrefix::TranslationPrefix(int16_t x, int16_t y, int16_t z, uint16_t count) : TranslationPrefix()
	{
		pImpl->_x = x; pImpl->_y = y; pImpl->_z = z; pImpl->_count = count;
	}
	TranslationPrefix::TranslationPrefix(const TranslationPrefix& input) : TranslationPrefix()
	{
		pImpl->_x = input.pImpl->_x; pImpl->_y = input.pImpl->_y; pImpl->_z = input.pImpl->_z; pImpl->_count = input.pImpl->_count;
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
		pImpl->_x = input.pImpl->_x; pImpl->_y = input.pImpl->_y; pImpl->_z = input.pImpl->_z; pImpl->_count = input.pImpl->_count; return *this;
	}
	bool TranslationPrefix::operator==(const TranslationPrefix& other) const
	{
		return pImpl->_x == other.pImpl->_x && pImpl->_y == other.pImpl->_y && pImpl->_z == other.pImpl->_z;
	}
	bool TranslationPrefix::operator!=(const TranslationPrefix& other) const
	{
		return !(*this == other);
	}
	const void* const TranslationPrefix::getRawData() const
	{
		return pImpl;
	}
#pragma endregion

	static std::tuple<int8_t, int8_t, bool> GetSFBGSRotationComponents(const float& component)
	{
		float lowPrecision = static_cast<float>(M_SQRT1_2 / 64.0); //1.0 / (sqrt(2) * 64.0);		//2^6
		float highPrecision = static_cast<float>(M_SQRT1_2 / 16384.0); //1.0 / (sqrt(2) * 16384.0);	//2^14

		float value = 0.0;
		if(component != 0)
			value = component + std::copysignf(highPrecision / 10.0f, component); //increase the value by just enough to correct rounding errors on conversion to int

		int8_t sVal = 0;
		int8_t lVal = 0;
		float tempS = 0;
		bool flag = false;

		lVal = std::clamp((int)(value / lowPrecision), -64, 63);
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
			lVal = static_cast<int8_t>((value - (sVal * highPrecision)) / lowPrecision);
		}
		return std::make_tuple(lVal, sVal, flag);
	}

#pragma region TRANSLATION
	struct TranslationEntry::Impl
	{
		int8_t _x = 0;
		int8_t _y = 0;
		int8_t _z = 0;
		Impl() = default;
	};

	TranslationEntry::TranslationEntry()
	{
		pImpl = new Impl;
	}

	TranslationEntry::~TranslationEntry()
	{
		if (pImpl)
			delete pImpl;
	}
	TranslationEntry::TranslationEntry(int8_t x, int8_t y, int8_t z) : TranslationEntry()
	{
		pImpl->_x = x; pImpl->_y = y; pImpl->_z = z;
	}
	TranslationEntry::TranslationEntry(const unsigned char* buffer) : TranslationEntry()
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
	}
	TranslationEntry::TranslationEntry(const TranslationEntry& input) : TranslationEntry()
	{
		pImpl->_x = input.pImpl->_x; pImpl->_y = input.pImpl->_y; pImpl->_z = input.pImpl->_z;
	}
	TranslationEntry& TranslationEntry::operator=(const unsigned char* buffer)
	{
		std::memcpy(pImpl, buffer, sizeof(Impl));
		return *this;
	}
	TranslationEntry& TranslationEntry::operator=(const TranslationEntry& input)
	{
		pImpl->_x = input.pImpl->_x; pImpl->_y = input.pImpl->_y; pImpl->_z = input.pImpl->_z;
		return *this;
	}
	int8_t TranslationEntry::getX() const { return pImpl->_x; }
	int8_t TranslationEntry::getY() const { return pImpl->_y; }
	int8_t TranslationEntry::getZ() const { return pImpl->_z; }
	void TranslationEntry::setX(int8_t x) { pImpl->_x = x; }
	void TranslationEntry::setY(int8_t y) { pImpl->_y = y; }
	void TranslationEntry::setZ(int8_t z) { pImpl->_z = z; }

	const void* const TranslationEntry::getRawData() const
	{
		return pImpl;
	}
#pragma endregion


	/// <summary>
	/// Will return pair of SFBGS Compressed rotations. The RLE value, or the prefix, will always have a count of 0 (1).
	/// Use the RLE scheme to fold a vector of these prefix values into a counted RLE vector.
	/// </summary>
	/// <param name="input"></param>
	/// <returns></returns>
	Utilities::PairContainer<RotationPrefix, RotationEntry> GetSFBGSRotationPair(const CALUMI::Math::Quaternion& input)
	{
		RotationPrefix prefix;
		CALUMI::Math::Quaternion tempInput = input;

		prefix.Missing(3);
		if (abs(input.getX()) > M_SQRT1_2) prefix.Missing(0);
		if (abs(input.getY()) > M_SQRT1_2) prefix.Missing(1);
		if (abs(input.getZ()) > M_SQRT1_2) prefix.Missing(2);

		//We must flip the quaternion values such that the missing value is positive. When derived, the missing will always be positive. So long as all values flip the quaternion is equal.
		if (prefix.Missing() == 0 && input.getX() < 0) tempInput = -input;
		if (prefix.Missing() == 1 && input.getY() < 0) tempInput = -input;
		if (prefix.Missing() == 2 && input.getZ() < 0) tempInput = -input;
		if (prefix.Missing() == 3 && input.getW() < 0) tempInput = -input;

		std::vector<float> fResults; fResults.reserve(3);
		if (prefix.Missing() != 0) fResults.push_back(tempInput.getX());
		if (prefix.Missing() != 1) fResults.push_back(tempInput.getY());
		if (prefix.Missing() != 2) fResults.push_back(tempInput.getZ());
		if (prefix.Missing() != 3) fResults.push_back(tempInput.getW());

		auto first = GetSFBGSRotationComponents(fResults.at(0));
		auto second = GetSFBGSRotationComponents(fResults.at(1));
		auto third = GetSFBGSRotationComponents(fResults.at(2));

		prefix.First(std::get<0>(first));
		prefix.FirstFlag(std::get<2>(first));
		prefix.Second(std::get<0>(second));
		prefix.SecondFlag(std::get<2>(second));
		prefix.Third(std::get<0>(third));
		prefix.ThirdFlag(std::get<2>(third));
		prefix.Count(0);

		RotationEntry suffix(std::get<1>(first), std::get<1>(second), std::get<1>(third) );

		
		return Utilities::PairContainer<RotationPrefix, RotationEntry>(prefix, suffix);
	}

	CALUMI::Math::Quaternion GetUniversalRotation(const CALUMI::SFBGS::RotationPrefix& prefix, const CALUMI::SFBGS::RotationEntry& suffix)
	{
		double lowPrecision		= M_SQRT1_2 / 64.0;
		double highPrecision	= M_SQRT1_2 / 16384.0;

		uint8_t s1 = 1 << (3 * prefix.FirstFlag());
		uint8_t s2 = 1 << (3 * prefix.SecondFlag());
		uint8_t s3 = 1 << (3 * prefix.ThirdFlag());

		double component1 = lowPrecision * prefix.First() + highPrecision * suffix.First() * s1;
		double component2 = lowPrecision * prefix.Second() + highPrecision * suffix.Second() * s2;
		double component3 = lowPrecision * prefix.Third() + highPrecision * suffix.Third() * s3;
		double component4 = sqrt(1 - pow(component1, 2) - pow(component2, 2) - pow(component3, 2));

		CALUMI::Math::Quaternion output;

		switch (prefix.Missing())
		{
		case 0:     output.setX(static_cast<float>(component4)); output.setY(static_cast<float>(component1)); output.setZ(static_cast<float>(component2)); output.setW(static_cast<float>(component3)); break;
		case 1:     output.setY(static_cast<float>(component4)); output.setX(static_cast<float>(component1)); output.setZ(static_cast<float>(component2)); output.setW(static_cast<float>(component3)); break;
		case 2:     output.setZ(static_cast<float>(component4)); output.setX(static_cast<float>(component1)); output.setY(static_cast<float>(component2)); output.setW(static_cast<float>(component3)); break;
		default:    output.setW(static_cast<float>(component4)); output.setX(static_cast<float>(component1)); output.setY(static_cast<float>(component2)); output.setZ(static_cast<float>(component3)); break;
		}

		return output;
	}

	static std::pair<int16_t, int8_t> GetSFBGSTranslationComponents(const double& component, const float& highPrecision, const float& lowPrecision)
	{
		double value = 0.0;
		if (component != 0)
			value = component + std::copysign(highPrecision / 3.0, component); //increase the value by just enough to correct rounding errors on conversion to int

		int8_t sVal = static_cast<int8_t>(fmod(value, lowPrecision) / highPrecision);
		int16_t lVal = static_cast<int16_t>((value - sVal * highPrecision) / lowPrecision);

		return std::make_pair(lVal, sVal);
	}

	Utilities::PairContainer<TranslationPrefix, TranslationEntry> GetSFBGSTranslationPair(const CALUMI::Math::Vector3D& input, const float& highPrecision, const float& lowPrecision)
	{
		auto x = GetSFBGSTranslationComponents(input.getX(), highPrecision, lowPrecision);
		auto y = GetSFBGSTranslationComponents(input.getY(), highPrecision, lowPrecision);
		auto z = GetSFBGSTranslationComponents(input.getZ(), highPrecision, lowPrecision);

		TranslationPrefix prefix(x.first, y.first, z.first, static_cast < uint16_t>(1));
		TranslationEntry suffix(x.second, y.second, z.second);

		return Utilities::PairContainer<TranslationPrefix, TranslationEntry>(prefix, suffix);
	}

	CALUMI::Math::Vector3D GetUniversalTranslation(const CALUMI::SFBGS::TranslationPrefix& prefix, const CALUMI::SFBGS::TranslationEntry& suffix, const float& highPrecision, const float& lowPrecision)
	{
		//std::println("PRE AND SUFF {} {} {} & {} {} {}", prefix._x, prefix._y, prefix._z, suffix._x, suffix._y, suffix._z);
		CALUMI::Math::Vector3D output((prefix.getX() * lowPrecision + suffix.getX() * highPrecision), (prefix.getY() * lowPrecision + suffix.getY() * highPrecision), (prefix.getZ() * lowPrecision + suffix.getZ() * highPrecision));
		//std::println("Output {} {} {}", output._x, output._y, output._z);
		return output;
	}
	
	Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> UnfoldTranslationPrefixSequence(const Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix>& input)
	{
		Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> output;
		uint16_t totalSize = 0;
		for (int i = 0; i<input.size();i++)
		{
			//totalSize++;
			totalSize += input.at(i).getCount();
		}
		//std::println("totalSize {}", totalSize);
		output.reserve(totalSize);

		for (int i = 0; i < input.size(); i++)
		{
			for (uint32_t j = 0; j < input.at(i).getCount(); j++) //Translations use classical counters
			{
				CALUMI::SFBGS::TranslationPrefix newPrefix = input.at(i);
				newPrefix.setCount(1);
				output.push_back(newPrefix);
			}
		}
		//std::println("UNFOLD LENGTH {}", output.size());
		return output;
	}

	Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> FoldTranslationPrefixSequence(const Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix>& input)
	{
		if (input.size() < 2)return input;

		Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> outputList; outputList.reserve(input.size());
		CALUMI::SFBGS::TranslationPrefix referencePrefix = input.at(0);
		for (uint16_t i = 1; i < input.size(); i++)
		{
			if (referencePrefix == input.at(i) && referencePrefix.getCount() <0xFFFF)
			{
				referencePrefix.setCount(referencePrefix.getCount()+1);
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

	Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix> UnfoldRotationPrefixSequence(const Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix>& input)
	{
		Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix> output;
		uint16_t totalSize = 0;
		for (int i =0; i < input.size(); i++)
		{
			totalSize++;
			totalSize += input.at(i).Count();
		}

		output.reserve(totalSize);

		for (int z =0; z < input.size(); z++)
		{
			for (uint8_t i = 0; i <= input.at(z).Count(); i++) //Rotations use programming counters
			{
				CALUMI::SFBGS::RotationPrefix newPrefix = input.at(z);
				newPrefix.Count(0);
				output.push_back(newPrefix);
			}
		}
		return output;
	}

	Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix> FoldRotationPrefixSequence(const Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix>& input)
	{
		if (input.size() < 2)return input;
		
		Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix> outputList;
		outputList.reserve(input.size());
		
		CALUMI::SFBGS::RotationPrefix referencePrefix = input.at(0);

		for (uint16_t i = 1; i < input.size(); i++)
		{
			if (referencePrefix == input.at(i) && referencePrefix.Count() < 63)
			{
				referencePrefix.Count(referencePrefix.Count()+1);
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

}
}