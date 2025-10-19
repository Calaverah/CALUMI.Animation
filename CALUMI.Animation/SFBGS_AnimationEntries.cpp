//Copyright © 2025 aka Calaverah. All rights reserved.
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
	
	RotationPrefix::RotationPrefix(int8_t X, bool QX, int8_t Y, bool QY, int8_t Z, bool QZ, uint8_t C, uint8_t ID)
	{
		this->first = std::clamp((int)X, 0b0, 0b01111111);
		this->firstFlag = std::clamp((int)QX, 0b0, 0b01);

		this->second = std::clamp((int)Y, 0b0, 0b01111111);
		this->secondFlag = std::clamp((int)QY, 0b0, 0b01);

		this->third = std::clamp((int)Z, 0b0, 0b01111111);
		this->thirdFlag = std::clamp((int)QZ, 0b0, 0b01);

		this->count = std::clamp((int)C, 0b0, 0b00111111);
		this->missing = std::clamp((int)ID, 0b0, 0b11);
	}

	bool RotationPrefix::operator==(const RotationPrefix& other) const
	{
		return first == other.first && firstFlag == other.firstFlag && second == other.second && secondFlag == other.secondFlag && third == other.third && thirdFlag == other.thirdFlag && missing == other.missing;
	}

	bool RotationPrefix::operator!=(const RotationPrefix& other) const
	{
		return !(*this == other);
	}
	bool TranslationPrefix::operator!=(const TranslationPrefix& other) const
	{
		return !(*this == other);
	}
	bool TranslationPrefix::operator==(const TranslationPrefix& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
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

		prefix.missing = 3;
		if (abs(input.x) > M_SQRT1_2) prefix.missing = 0;
		if (abs(input.y) > M_SQRT1_2) prefix.missing = 1;
		if (abs(input.z) > M_SQRT1_2) prefix.missing = 2;

		//We must flip the quaternion values such that the missing value is positive. When derived, the missing will always be positive. So long as all values flip the quaternion is equal.
		if (prefix.missing == 0 && input.x < 0) tempInput = -input;
		if (prefix.missing == 1 && input.y < 0) tempInput = -input;
		if (prefix.missing == 2 && input.z < 0) tempInput = -input;
		if (prefix.missing == 3 && input.w < 0) tempInput = -input;

		std::vector<float> fResults; fResults.reserve(3);
		if (prefix.missing != 0) fResults.push_back(tempInput.x);
		if (prefix.missing != 1) fResults.push_back(tempInput.y);
		if (prefix.missing != 2) fResults.push_back(tempInput.z);
		if (prefix.missing != 3) fResults.push_back(tempInput.w);

		auto first = GetSFBGSRotationComponents(fResults.at(0));
		auto second = GetSFBGSRotationComponents(fResults.at(1));
		auto third = GetSFBGSRotationComponents(fResults.at(2));

		prefix.first = std::get<0>(first);
		prefix.firstFlag = std::get<2>(first);
		prefix.second = std::get<0>(second);
		prefix.secondFlag = std::get<2>(second);
		prefix.third = std::get<0>(third);
		prefix.thirdFlag = std::get<2>(third);
		prefix.count = 0;

		RotationEntry suffix(std::get<1>(first), std::get<1>(second), std::get<1>(third) );

		Utilities::PairContainer<RotationPrefix, RotationEntry> outputPair;
		outputPair.first = prefix;
		outputPair.second = suffix;
		return outputPair;
	}

	CALUMI::Math::Quaternion GetUniversalRotation(const CALUMI::SFBGS::RotationPrefix& prefix, const CALUMI::SFBGS::RotationEntry& suffix)
	{
		double lowPrecision		= M_SQRT1_2 / 64.0;
		double highPrecision	= M_SQRT1_2 / 16384.0;

		uint8_t s1 = 1 << (3 * prefix.firstFlag);
		uint8_t s2 = 1 << (3 * prefix.secondFlag);
		uint8_t s3 = 1 << (3 * prefix.thirdFlag);

		double component1 = lowPrecision * prefix.first + highPrecision * suffix.first * s1;
		double component2 = lowPrecision * prefix.second + highPrecision * suffix.second * s2;
		double component3 = lowPrecision * prefix.third + highPrecision * suffix.third * s3;
		double component4 = sqrt(1 - pow(component1, 2) - pow(component2, 2) - pow(component3, 2));

		CALUMI::Math::Quaternion output;

		switch (prefix.missing)
		{
		case 0:     output.x = static_cast<float>(component4); output.y = static_cast<float>(component1); output.z = static_cast<float>(component2); output.w = static_cast<float>(component3); break;
		case 1:     output.y = static_cast<float>(component4); output.x = static_cast<float>(component1); output.z = static_cast<float>(component2); output.w = static_cast<float>(component3); break;
		case 2:     output.z = static_cast<float>(component4); output.x = static_cast<float>(component1); output.y = static_cast<float>(component2); output.w = static_cast<float>(component3); break;
		default:    output.w = static_cast<float>(component4); output.x = static_cast<float>(component1); output.y = static_cast<float>(component2); output.z = static_cast<float>(component3); break;
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
		auto x = GetSFBGSTranslationComponents(input.x, highPrecision, lowPrecision);
		auto y = GetSFBGSTranslationComponents(input.y, highPrecision, lowPrecision);
		auto z = GetSFBGSTranslationComponents(input.z, highPrecision, lowPrecision);

		TranslationPrefix prefix(x.first, y.first, z.first, static_cast < uint16_t>(1));
		TranslationEntry suffix(x.second, y.second, z.second);

		Utilities::PairContainer<TranslationPrefix, TranslationEntry> outputPair;
		outputPair.first = prefix;
		outputPair.second = suffix;
		return outputPair;
	}

	CALUMI::Math::Vector3D GetUniversalTranslation(const CALUMI::SFBGS::TranslationPrefix& prefix, const CALUMI::SFBGS::TranslationEntry& suffix, const float& highPrecision, const float& lowPrecision)
	{
		//std::println("PRE AND SUFF {} {} {} & {} {} {}", prefix.x, prefix.y, prefix.z, suffix.x, suffix.y, suffix.z);
		CALUMI::Math::Vector3D output((prefix.x * lowPrecision + suffix.x * highPrecision), (prefix.y * lowPrecision + suffix.y * highPrecision), (prefix.z * lowPrecision + suffix.z * highPrecision));
		//std::println("Output {} {} {}", output.x, output.y, output.z);
		return output;
	}
	
	Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> UnfoldTranslationPrefixSequence(const Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix>& input)
	{
		Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> output;
		uint16_t totalSize = 0;
		for (int i = 0; i<input.size();i++)
		{
			//totalSize++;
			totalSize += input.at(i).count;
		}
		//std::println("totalSize {}", totalSize);
		output.reserve(totalSize);

		for (int i = 0; i < input.size(); i++)
		{
			for (uint8_t j = 0; j < input.at(i).count; j++) //Translations use classical counters
			{
				CALUMI::SFBGS::TranslationPrefix newPrefix = input.at(i);
				newPrefix.count = 1;
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
			if (referencePrefix == input.at(i) && referencePrefix.count <0xFFFF)
			{
				referencePrefix.count++;
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
			totalSize += input.at(i).count;
		}

		output.reserve(totalSize);

		for (int z =0; z < input.size(); z++)
		{
			for (uint8_t i = 0; i <= input.at(z).count; i++) //Rotations use programming counters
			{
				CALUMI::SFBGS::RotationPrefix newPrefix = input.at(z);
				newPrefix.count = 0;
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
			if (referencePrefix == input.at(i) && referencePrefix.count < 63)
			{
				referencePrefix.count++;
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

}}