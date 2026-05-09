//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


// ReSharper disable CppExpressionWithoutSideEffects
#include "internalplatform.h"
#include "utilities/CALUMI_Common.h"
#include "utilities/CALUMI_Utilities.h"
#include "univ/animation/UNIV_Animation.h"
#include <string>
#include "univ/animation/UNIV_IAnimationPackage.h"
#include <vector>
#include "internal/internalvectordef.h"
#include <algorithm>
#include <iostream>
#include <ostream>

#include "sfbgs/animation/SFBGS_AnimationPackage.h"

namespace CALUMI::UNIV
{
#pragma region ANIMATION
	struct Animation::Impl
	{
		Utilities::StringContainer m_animationTitle = "NO TITLE";
		AnimationBlockVector m_animationBlocks;
		AnimationPackageManager m_packageManager;

		Impl() = default;
	};
	Animation::Animation() { pImpl = new Impl; }
	AnimationBlockVector& Animation::animationBlocks() const
	{
		return pImpl->m_animationBlocks;
	}
	const char* Animation::animationTitle() const
	{
		return pImpl->m_animationTitle.c_str();
	}
	void Animation::setAnimationTitle(const char* title) const
	{
		pImpl->m_animationTitle = title;
	}
	void Animation::setAnimationTitle(const Utilities::StringContainer& title) const
	{
		pImpl->m_animationTitle = title;
	}
	Animation::Animation(const Utilities::StringContainer& title, const unsigned int initialBlockCount = 8) : Animation()
	{
		pImpl->m_animationTitle = title;
		pImpl->m_animationBlocks.reserve(initialBlockCount);
	}
	Animation::Animation(const Animation& input) : Animation()
	{
		*pImpl = *input.pImpl;
	}

	Animation::Animation(const Utilities::JsonObject& data) : Animation()
	{
		if (data.contains("name"))
			pImpl->m_animationTitle = data["name"].toString();

		if (data.contains("packages"))
		{
			const auto packages = data["packages"].toObject();
			const auto keys = packages.keys();
			for (int i = 0; i < keys.size(); i++)
			{
				if (SCOMPARE(keys.c_str(i), SFBGS::SFBGS_ANIM_PACKAGE) == 0)
				{
					auto& sfbgs = SFBGS::SFBGS_AnimationPackage::GetPackage(*this);
					sfbgs.fromJson(packages[keys.c_str(i)].toObject());
				}
			}
		}

		if (data.contains("blocks"))
		{
			Utilities::JsonArray blocks = data["blocks"].toArray();
			for (unsigned int i = 0; i < blocks.size(); i++)
			{
				AnimationBlock blockToAdd(blocks.at(i).toObject());
				pImpl->m_animationBlocks.push_back(blockToAdd);
			}
		}
	}

	Animation::~Animation()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	Animation& Animation::operator=(const Animation& input)
	{
		if (this == &input)
			return *this;

		pImpl->m_animationTitle = input.pImpl->m_animationTitle;
		pImpl->m_packageManager = input.pImpl->m_packageManager;
		pImpl->m_animationBlocks = input.pImpl->m_animationBlocks;

		return *this;
	}

	bool Animation::operator==(const Animation& other) const
	{
		if (this == &other)
			return true;

		if (pImpl->m_animationBlocks.size() != other.pImpl->m_animationBlocks.size())
			return false;

		for (unsigned int i = 0; i < pImpl->m_animationBlocks.size(); i++)
		{
			if (pImpl->m_animationBlocks.at(i) != other.pImpl->m_animationBlocks.at(i))
				return false;
		}

		return pImpl->m_animationTitle == other.pImpl->m_animationTitle &&
			pImpl->m_packageManager == other.pImpl->m_packageManager;
	}

	bool Animation::addAnimationBlock(const AnimationBlock& blockToAdd, const bool overwrite) const
	{
		if (SCOMPARE(blockToAdd.boneName(), "") == 0)
			return false;

		for (unsigned int i = 0; i < pImpl->m_animationBlocks.size(); i ++)
		{
			if (SCOMPARE(pImpl->m_animationBlocks.at(i).boneName(), blockToAdd.boneName()) == 0)
			{
				if (!overwrite)
					return false;

				pImpl->m_animationBlocks.at(i) = blockToAdd;
				return true;
			}
		}

		pImpl->m_animationBlocks.push_back(blockToAdd);
		return true;
	}

	void Animation::clearAnimationBlocks() const { pImpl->m_animationBlocks.clear(); }
	uint64_t Animation::animationBlockCount() const { return pImpl->m_animationBlocks.size(); }
	int Animation::findAnimationBlock(const char* boneName) const
	{
		for (int i = 0; i < pImpl->m_animationBlocks.size() && i < std::numeric_limits<int>::max(); i++)
		{
			if (SCOMPARE(pImpl->m_animationBlocks.at(i).boneName(), boneName) == 0)
				return i;
		}

		return -1;
	}
	AnimationPackageManager& Animation::packageManager() const { return pImpl->m_packageManager; }

	Utilities::JsonObject Animation::toJson() const
	{
		const Utilities::JsonObject output;

		output["name"] = pImpl->m_animationTitle.c_str();

		output["packages"] = pImpl->m_packageManager.toJson();

		Utilities::JsonArray animationBlocks;

		for (unsigned int i = 0; i < pImpl->m_animationBlocks.size(); i++)
		{
			animationBlocks.push_back(pImpl->m_animationBlocks.at(i).toJson());
		}

		output["blocks"] = animationBlocks;

		return output;
	}

	unsigned int Animation::frameCount() const
	{
		unsigned int output = 0;
		bool entryFound = false;


		for (unsigned int i = 0; i < pImpl->m_animationBlocks.size(); i++)
		{
			if (pImpl->m_animationBlocks.at(i).priorityEntryCount() > 0 ||
				pImpl->m_animationBlocks.at(i).scalarEntryCount() > 0 ||
				pImpl->m_animationBlocks.at(i).rotationEntryCount() > 0 ||
				pImpl->m_animationBlocks.at(i).translationEntryCount() > 0)
				entryFound = true;

			if (const unsigned int temp = pImpl->m_animationBlocks.at(i).lastFrameInBlock(); temp > output)
			{
				output = temp;
			}
		}
		if(entryFound)
			output++; //adding one to sum up total frames in animation. If final frame is 50, the count is 51

		return output;
	}

#pragma endregion

#pragma region ANIMATIONBLOCK

	struct AnimationBlock::Impl
	{
		///@privatesection
		///@{
		Utilities::StringContainer m_boneName = "UNNAMED";
		RotationSequence m_rotationSequence;
		TranslationSequence m_translationSequence;
		ScalarSequence m_scalarSequence;
		PrioritySequence m_prioritySequence;
		Impl() = default;
		///@}
	};

	Utilities::JsonObject AnimationBlock::toJson() const
	{
		Utilities::JsonObject output;

		output["bone"] = pImpl->m_boneName.c_str();

		Utilities::JsonArray rotations;
		for (uint64_t i = 0; i < pImpl->m_rotationSequence.size(); i++)
		{
			rotations.push_back(pImpl->m_rotationSequence.at(i).toJson());
		}
		output["rotations"] = rotations;

		Utilities::JsonArray translations;
		for (uint64_t i = 0; i < pImpl->m_translationSequence.size(); i++)
		{
			translations.push_back(pImpl->m_translationSequence.at(i).toJson());
		}
		output["translations"] = translations;

		Utilities::JsonArray scalars;
		for (uint64_t i = 0; i < pImpl->m_scalarSequence.size(); i++)
		{
			scalars.push_back(pImpl->m_scalarSequence.at(i).toJson());
		}
		output["scalars"] = scalars;

		Utilities::JsonArray priorities;
		for (uint64_t i = 0; i < pImpl->m_prioritySequence.size(); i++)
		{
			priorities.push_back(pImpl->m_prioritySequence.at(i).toJson());
		}
		output["priorities"] = priorities;

		return output;
	}

	AnimationBlock::AnimationBlock(const Utilities::JsonObject& data) : AnimationBlock()
	{
		if (data.contains("bone"))
			pImpl->m_boneName = data["bone"].toString();

		if (data.contains("rotations"))
		{
			const auto rotations = data["rotations"].toArray();
			for (uint64_t i = 0; i < rotations.size(); i++)
			{
				RotationFrame entry(rotations.at(i).toObject());
				addRotationEntry(entry);
			}
		}
		if (data.contains("translations"))
		{
			const auto translations = data["translations"].toArray();
			for (uint64_t i = 0; i < translations.size(); i++)
			{
				TranslationFrame entry(translations.at(i).toObject());
				addTranslationEntry(entry);
			}
		}
		if (data.contains("scalars"))
		{
			const auto scalars = data["scalars"].toArray();
			for (uint64_t i = 0; i < scalars.size(); i++)
			{
				ScalarFrame entry(scalars.at(i).toObject());
				addScalarEntry(entry);
			}
		}
		if (data.contains("priorities"))
		{
			const auto priorities = data["priorities"].toArray();
			for (uint64_t i = 0; i < priorities.size(); i++)
			{
				PriorityFrame entry(priorities.at(i).toObject());
				addPriorityEntry(entry);
			}
		}
	}

	AnimationBlock::AnimationBlock() : pImpl(new Impl()) {}

	AnimationBlock::~AnimationBlock()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	AnimationBlock::AnimationBlock(const AnimationBlock& input) : AnimationBlock()
	{
		*this = input;
	}


	AnimationBlock& AnimationBlock::operator=(const AnimationBlock& other)
	{
		if (this != &other)
		{
			pImpl->m_boneName = other.pImpl->m_boneName;
			pImpl->m_rotationSequence = other.pImpl->m_rotationSequence;
			pImpl->m_translationSequence = other.pImpl->m_translationSequence;
			pImpl->m_scalarSequence = other.pImpl->m_scalarSequence;
			pImpl->m_prioritySequence = other.pImpl->m_prioritySequence;
		}

		return *this;
	}

	bool AnimationBlock::operator==(const AnimationBlock& other) const
	{
		return	pImpl->m_boneName == other.pImpl->m_boneName &&
				CompareRotationSequence(pImpl->m_rotationSequence, other.pImpl->m_rotationSequence) &&
				CompareTranslationSequence(pImpl->m_translationSequence, other.pImpl->m_translationSequence) &&
				CompareScalarSequence(pImpl->m_scalarSequence, other.pImpl->m_scalarSequence) &&
				ComparePrioritySequence(pImpl->m_prioritySequence, other.pImpl->m_prioritySequence);
	}

	unsigned int AnimationBlock::lastFrameInBlock() const
	{
		unsigned int output = 0;
		if (!pImpl->m_rotationSequence.empty()) {
			if (pImpl->m_rotationSequence.at(pImpl->m_rotationSequence.size()-1).frame() > output)
			{
				output = pImpl->m_rotationSequence.at(pImpl->m_rotationSequence.size()-1).frame();
			}
		}
		if (!pImpl->m_translationSequence.empty()) {
			if (pImpl->m_translationSequence.at(pImpl->m_translationSequence.size()-1).frame() > output)
			{
				output = pImpl->m_translationSequence.at(pImpl->m_translationSequence.size() - 1).frame();
			}
		}
		if (!pImpl->m_scalarSequence.empty()) {
			if (pImpl->m_scalarSequence.at(pImpl->m_scalarSequence.size()-1).frame() > output)
			{
				output = pImpl->m_scalarSequence.at(pImpl->m_scalarSequence.size() - 1).frame();
			}
		}
		if (!pImpl->m_prioritySequence.empty()) {
			if (pImpl->m_prioritySequence.at(pImpl->m_prioritySequence.size()-1).frame() > output)
			{
				output = pImpl->m_prioritySequence.at(pImpl->m_prioritySequence.size() - 1).frame();
			}
		}

		return output;
	}

	const char* AnimationBlock::boneName() const
	{
		return pImpl->m_boneName.c_str();
	}

	void AnimationBlock::setBoneName(const char* name) const
	{
		pImpl->m_boneName = name;
	}

	RotationSequence& AnimationBlock::rotationSequence() const
	{
		return pImpl->m_rotationSequence;
	}

	bool AnimationBlock::addRotationEntry(const RotationFrame& input, const bool overwrite) const
	{

		for (uint64_t i = 0; i < pImpl->m_rotationSequence.size(); i++)
		{
			if (pImpl->m_rotationSequence.at(i).frame() == input.frame())
			{
				if (!overwrite) return false;

				pImpl->m_rotationSequence.at(i) = input;
				return true;
			}

			if (pImpl->m_rotationSequence.at(i).frame() > input.frame())
			{
				pImpl->m_rotationSequence.insert_r(i, input);
				return true;
			}
		}

		pImpl->m_rotationSequence.push_back(input);
		return true;
	}

	bool AnimationBlock::removeRotationEntry(const unsigned int frame) const
	{
		for (unsigned int i = 0; i < pImpl->m_rotationSequence.size(); i++)
		{
			if (frame == pImpl->m_rotationSequence.at(i).frame())
			{
				pImpl->m_rotationSequence.erase(i);
				return true;
			}
		}

		return false;
	}

	void AnimationBlock::clearRotationEntries() const
	{
		pImpl->m_rotationSequence.clear();
	}

	uint64_t AnimationBlock::rotationEntryCount() const { return pImpl->m_rotationSequence.size(); }

	static RotationSequence s_RDP_Rotation_Recursive(RotationSequence input, const float tolerance)
	{
		if (input.size() <= 2)
			return input;

		//Maximum Distance
		float dMax = 0.0f;
		uint64_t index = 0;
		const auto f0 = input.at(0).frame();
		const auto fn = input.at(input.size() - 1).frame();

		bool skip = false;

		//std::println("size {}, dist {}, start/finish {}/{}", input.size(),distance,input.at(0).Frame(),input.at(input.size()-1).Frame());

		//We force the split in this range of entries as Slerp for 0-180 degrees will have an undefined direction
		// and almost always will give an sLerp/key angle of some value above 180 degrees...
		// By setting the index to half (or at least 1) and skipping any index assignment we force at least one key in between the 0-180 degree
		// interpolation allowing the user to keep their intended direction.
		if (const float distance = input.at(0).rotationQuaternion().angularDistance(input.at(input.size()-1).rotationQuaternion()); distance >= Math::ToRadians(180.0) - tolerance) {
			index = input.size()/2 > 0 ? input.size()/2 : 1;
			skip = true;
		}

		if(!skip) {
			for (uint64_t i = 1; i < input.size()-1; i++)
			{

				const float t = static_cast<float>(input.at(i).frame() - f0) / static_cast<float>(fn - f0);

				//sLerp isn't quite the "perpendicular" distance needed for this algorithm, but it makes for a practical compromise
				Math::Quaternion interp = input.at(0).rotationQuaternion().sLerp(input.at(input.size() - 1).rotationQuaternion(), t);

				if (const float angle = interp.angularDistance(input.at(i).rotationQuaternion()); angle > dMax) {
					dMax = angle;
					index = i;
				}
			}
		}

		RotationSequence output;

		if (dMax > tolerance || skip) {
			auto left = s_RDP_Rotation_Recursive(input.range(0, index), tolerance);
			auto right = s_RDP_Rotation_Recursive(input.range(index, input.size() - 1),tolerance);

			output.reserve(left.size() + right.size() - 1);

			for (uint64_t i = 0; i < left.size(); i++)
			{
				output.push_back(left.at(i));
			}

			for (uint64_t i = 1; i < right.size(); i++)
			{
				output.push_back(right.at(i));
			}

			output.shrink_to_fit();
		} else {

			output.reserve(2);
			output.push_back(input.at(0));
			output.push_back(input.at(input.size() - 1));
		}

		return output;
	}

	void AnimationBlock::executeRDPReduction_Rotation(const float tolerance) const
	{
		pImpl->m_rotationSequence = s_RDP_Rotation_Recursive(pImpl->m_rotationSequence, tolerance);
	}

	TranslationSequence& AnimationBlock::translationSequence() const
	{
		return pImpl->m_translationSequence;
	}

	bool AnimationBlock::addTranslationEntry(const TranslationFrame& input, const bool overwrite) const
	{
		for (uint64_t i = 0; i < pImpl->m_translationSequence.size(); i++)
		{
			if (pImpl->m_translationSequence.at(i).frame() == input.frame())
			{
				if (!overwrite)
					return false;

				pImpl->m_translationSequence.at(i) = input;
				return true;
			}

			if (pImpl->m_translationSequence.at(i).frame() > input.frame())
			{
				pImpl->m_translationSequence.insert_r(i, input);
				return true;
			}
		}

		pImpl->m_translationSequence.push_back(input);
		return true;
	}

	bool AnimationBlock::removeTranslationEntry(const unsigned int frame) const
	{
		for (unsigned int i = 0; i < pImpl->m_translationSequence.size(); i++)
		{
			if (frame == pImpl->m_translationSequence.at(i).frame())
			{
				pImpl->m_translationSequence.erase(i);
				return true;
			}
		}

		return false;
	}

	void AnimationBlock::clearTranslationEntries() const
	{
		pImpl->m_translationSequence.clear();
	}

	uint64_t AnimationBlock::translationEntryCount() const
	{
		return pImpl->m_translationSequence.size();
	}

	static TranslationSequence s_RDP_Translation_Recursive(TranslationSequence input, const float tolerance)
	{
		if (input.size() <= 2)
			return input;

		//Maximum Distance
		double dMax = 0.0f;
		uint64_t index = 0;
		const auto f0 = input.at(0).frame();
		const auto fn = input.at(input.size() - 1).frame();

		for (uint64_t i = 1; i < input.size() - 1; i++)
		{
			const double t = static_cast<float>(input.at(i).frame() - f0) / static_cast<float>(fn - f0);

			//lerp isn't quite the "perpendicular" distance needed for this algorithm, but it makes for a practical compromise
			Math::Vector3D interp = input.at(0).translationVector().lerp(input.at(input.size() - 1).translationVector(), t);

			if (const double distance = (interp - input.at(i).translationVector()).length(); distance > dMax)
			{
				dMax = distance;
				index = i;
			}
		}

		TranslationSequence output;

		if (dMax > tolerance) {

			auto left = s_RDP_Translation_Recursive(input.range(0, index), tolerance);
			auto right = s_RDP_Translation_Recursive(input.range(index, input.size() - 1), tolerance);

			output.reserve(left.size() + right.size() - 1);

			for (uint64_t i = 0; i < left.size(); i++)
			{
				output.push_back(left.at(i));
			}

			for (uint64_t i = 1; i < right.size(); i++)
			{
				output.push_back(right.at(i));
			}

			output.shrink_to_fit();

		} else {
			output.reserve(2);
			output.push_back(input.at(0));
			output.push_back(input.at(input.size() - 1));
		}
		return output;
	}

	void AnimationBlock::executeRDPReduction_Translation(const float tolerance) const
	{
		pImpl->m_translationSequence = s_RDP_Translation_Recursive(pImpl->m_translationSequence, tolerance);
	}

	ScalarSequence& AnimationBlock::scalarSequence() const
	{
		return pImpl->m_scalarSequence;
	}

	bool AnimationBlock::addScalarEntry(const ScalarFrame& input, const bool overwrite) const
	{
		for (uint64_t i = 0; i < pImpl->m_scalarSequence.size(); i++)
		{
			if (pImpl->m_scalarSequence.at(i).frame() == input.frame())
			{
				if (!overwrite)
					return false;

				pImpl->m_scalarSequence.at(i) = input;
				return true;

			}

			if (pImpl->m_scalarSequence.at(i).frame() > input.frame())
			{
				pImpl->m_scalarSequence.insert_r(i, input);
				return true;
			}
		}

		pImpl->m_scalarSequence.push_back(input);
		return true;
	}

	bool AnimationBlock::removeScalarEntry(const unsigned int frame) const
	{
		for (unsigned int i = 0; i < pImpl->m_scalarSequence.size(); i++)
		{
			if (frame == pImpl->m_scalarSequence.at(i).frame())
			{
				pImpl->m_scalarSequence.erase(i);
				return true;
			}
		}

		return false;
	}

	void AnimationBlock::clearScalarEntries() const
	{
		pImpl->m_scalarSequence.clear();
	}

	uint64_t AnimationBlock::scalarEntryCount() const { return pImpl->m_scalarSequence.size(); }

	static ScalarSequence s_RDP_Scalar_Recursive(ScalarSequence input, const float tolerance)
	{
		if (input.size() <= 2)
			return input;

		//Maximum Distance
		double dMax = 0.0f;
		uint64_t index = 0;
		const auto f0 = input.at(0).frame();
		const auto fN = input.at(input.size() - 1).frame();

		const Math::Vector2D vec0(f0, input.at(0).scalarValue());
		const Math::Vector2D vecN(fN, input.at(input.size()-1).scalarValue());

		for (uint64_t i = 1; i < input.size() - 1; i++)
		{
			const double t = static_cast<float>(input.at(i).frame() - f0) / static_cast<float>(fN - f0);

			//lerp isn't quite the "perpendicular" distance needed for this algorithm, but it makes for a practical compromise
			Math::Vector2D interp = vec0.lerp(vecN, t);
			Math::Vector2D current(input.at(i).frame(), input.at(i).scalarValue());

			if (const double distance = (interp - current).length(); distance > dMax) {
				dMax = distance;
				index = i;
			}
		}

		ScalarSequence output;

		if (dMax > tolerance)
		{

			auto left = s_RDP_Scalar_Recursive(input.range(0, index), tolerance);
			auto right = s_RDP_Scalar_Recursive(input.range(index, input.size() - 1), tolerance);

			output.reserve(left.size() + right.size() - 1);

			for (uint64_t i = 0; i < left.size(); i++)
			{
				output.push_back(left.at(i));
			}

			for (uint64_t i = 1; i < right.size(); i++)
			{
				output.push_back(right.at(i));
			}

			output.shrink_to_fit();

		} else {
			output.reserve(2);
			output.push_back(input.at(0));
			output.push_back(input.at(input.size() - 1));
		}
		return output;
	}

	void AnimationBlock::executeRDPReduction_Scalar(const float tolerance) const
	{
		pImpl->m_scalarSequence = s_RDP_Scalar_Recursive(pImpl->m_scalarSequence, tolerance);
	}

	PrioritySequence& AnimationBlock::prioritySequence() const
	{
		return pImpl->m_prioritySequence;
	}

	bool AnimationBlock::addPriorityEntry(const PriorityFrame& input, const bool overwrite) const
	{
		for (uint64_t i = 0; i < pImpl->m_prioritySequence.size(); i++)
		{
			if (pImpl->m_prioritySequence.at(i).frame() == input.frame())
			{
				if (!overwrite) return false;

				pImpl->m_prioritySequence.at(i) = input;
				return true;

			}

			if (pImpl->m_prioritySequence.at(i).frame() > input.frame())
			{
				pImpl->m_prioritySequence.insert_r(i, input);
				return true;
			}
		}

		pImpl->m_prioritySequence.push_back(input);
		return true;
	}

	bool AnimationBlock::removePriorityEntry(const unsigned int frame) const
	{
		for (unsigned int i = 0; i < pImpl->m_prioritySequence.size(); i++)
		{
			if (frame == pImpl->m_prioritySequence.at(i).frame())
			{
				pImpl->m_prioritySequence.erase(i);
				return true;
			}
		}

		return false;
	}

	void AnimationBlock::clearPriorityEntries() const
	{
		pImpl->m_prioritySequence.clear();
	}

	uint64_t AnimationBlock::priorityEntryCount() const { return pImpl->m_prioritySequence.size(); }


#pragma endregion

	VECTORDEF(AnimationBlockVector, AnimationBlock)
	VECTORDEF(AnimationVector, Animation)
}

#pragma region EXTERN"C"

CALUMI::UNIV::Animation* CreateAnimationC(const char* animationTitle, const unsigned int rigBoneCount)
{
	const auto outputAnimation = new CALUMI::UNIV::Animation(animationTitle, rigBoneCount);
	return outputAnimation;
}

CALUMI::UNIV::AnimationBlock* GetAnimationBlockC(const CALUMI::UNIV::Animation* source, const char* boneName)
{
	if (source)
		try
		{
			const int index = source->findAnimationBlock(boneName);

			if (source->animationBlocks().size() <= index || index < 0)
			{
				return nullptr;
			}
			return &source->animationBlocks().at(index);
		}
	catch (std::exception&)
	{
	}
	return nullptr;
}

uint64_t GetAnimationBlockCountC(const CALUMI::UNIV::Animation* source)
{
	if (source)
		try
		{
			return source->animationBlocks().size();
		}
	catch (std::exception&)
	{
	}
	return 0;
}

const char* GetAnimationTitleC(const CALUMI::UNIV::Animation* source)
{
	if (source)
		try
		{
			return source->animationTitle();
		}
	catch (std::exception&)
	{
	}
	return nullptr;
}

uint64_t GetFrameCountC(const CALUMI::UNIV::Animation* source)
{
	if (source)
		try
		{
			return source->frameCount();
		}
		catch (std::exception&)
		{
		}
	return 0;
}

int DeleteAnimationC(const CALUMI::UNIV::Animation** ptr)
{
	if (ptr && *ptr)
	{
		delete ptr;
		ptr = nullptr;
		return 0;
	}

	return -1;
}

int AddAnimBlockToAnimationC(const CALUMI::UNIV::Animation* anim,
							  const CALUMI::UNIV::AnimationBlock* blockToAdd,
							  const bool overwrite)
{
	if (anim && blockToAdd)
		try
		{
			return anim->addAnimationBlock(*blockToAdd, overwrite) ? 0 : 1;
		}
	catch (std::exception&)
	{
	}
	return -1;
}

CALUMI::UNIV::AnimationBlock* CreateAnimBlockC(const char* boneName)
{
	if (!boneName)
	{
		return nullptr;
	}

	const auto outputAnimationBlock = new CALUMI::UNIV::AnimationBlock;
	outputAnimationBlock->setBoneName(boneName);
	return outputAnimationBlock;
}

int DeleteAnimationBlockC(const CALUMI::UNIV::AnimationBlock** ptr)
{
	if (ptr && *ptr)
	{
		delete *ptr;
		*ptr = nullptr;
		return 0;
	}
	return -1;
}

const char* GetAnimBlockBoneNameC(const CALUMI::UNIV::AnimationBlock* source)
{
	if (source)
		return source->boneName();

	return nullptr;
}

unsigned int GetLastFrameInAnimBlockC(const CALUMI::UNIV::AnimationBlock* source)
{
	if (source)
		return source->lastFrameInBlock();

	return 0;
}

unsigned int AddRotationSqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block,
									   const CALUMI::UNIV::RotationFrame* rotSq,
									   const unsigned int size,
									   const bool overwrite)
{
	unsigned int added = 0;

	if (block && rotSq)
		try
		{
			for (unsigned int i = 0; i < size; i++)
			{
				if (!block->addRotationEntry(rotSq[i], overwrite))
					return added;

				added++;
			}
		}
	catch (std::exception&)
	{
	}

	return added;
}

CALUMI::UNIV::RotationFrame* GetRotationSqArrayC(const CALUMI::UNIV::AnimationBlock* source)
{
	if (!source || source->rotationSequence().empty())
		return nullptr;

	return source->rotationSequence().data();
}

CALUMI::UNIV::RotationFrame* GetRotationFromSqC(const CALUMI::UNIV::AnimationBlock* source, const int index)
{
	if (source)
		try
		{
			if (index >= source->rotationSequence().size() || index < 0)
			{
				return nullptr;
			}
			return &source->rotationSequence().at(index);
		}
	catch (std::exception&)
	{
	}
	return nullptr;
}

uint64_t GetRotationSqSizeC(const CALUMI::UNIV::AnimationBlock* source)
{
	if (source)
		return source->rotationSequence().size();

	return 0;
}

int ExecuteRDPReduction_RotationC(const CALUMI::UNIV::AnimationBlock* source, const float tolerance)
{
	if (source)
		try
		{
			source->executeRDPReduction_Rotation(tolerance);
			return 0;
		}
	catch (std::exception&)
	{
	}

	return -1;
}

unsigned int AddTranslationSqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block,
								  const CALUMI::UNIV::TranslationFrame* trnSq,
								  const unsigned int size,
								  const bool overwrite)
{
	unsigned int added = 0;

	if (block && trnSq)
		try
		{
			for (unsigned int i = 0; i < size; i++)
			{
				if (!block->addTranslationEntry(trnSq[i], overwrite))
					return added;

				added++;
			}
		}
	catch (std::exception&)
	{
	}

	return added;
}

CALUMI::UNIV::TranslationFrame* GetTranslationSqArrayC(const CALUMI::UNIV::AnimationBlock* source)
{
	if (!source || source->translationSequence().empty())
		return nullptr;

	return source->translationSequence().data();
}

CALUMI::UNIV::TranslationFrame* GetTranslationFromSqC(const CALUMI::UNIV::AnimationBlock* source, const int index)
{
	if (source)
		try
		{
			if (index >= source->translationSequence().size() || index < 0)
			{
				return nullptr;
			}
			return &source->translationSequence().at(index);
		}
	catch (std::exception&)
	{
	}
	return nullptr;
}

uint64_t GetTranslationSqSizeC(const CALUMI::UNIV::AnimationBlock* source)
{
	if (source)
		try
		{
			return source->translationSequence().size();
		}
	catch (std::exception&)
	{
	}
	return 0;
}

int ExecuteRDPReduction_TranslationC(const CALUMI::UNIV::AnimationBlock* source, const float tolerance)
{
	if (source)
		try
		{
			source->executeRDPReduction_Translation(tolerance);
			return 0;
		}
	catch (std::exception&)
	{
	}

	return -1;
}

unsigned int AddScalarSqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block,
									 const CALUMI::UNIV::ScalarFrame* sclrSq, const unsigned int size, const bool overwrite)
{
	unsigned int added = 0;

	if (block && sclrSq)
		try
		{
			for (unsigned int i = 0; i < size; i++)
			{
				if (!block->addScalarEntry(sclrSq[i], overwrite))
					return added;

				added++;
			}
		}
	catch (std::exception&)
	{
	}
	return added;
}

CALUMI::UNIV::ScalarFrame* GetScalarSqArrayC(const CALUMI::UNIV::AnimationBlock* source)
{
	if (!source || source->scalarSequence().empty())
		return nullptr;

	return source->scalarSequence().data();
}

CALUMI::UNIV::ScalarFrame* GetScalarFromSqC(const CALUMI::UNIV::AnimationBlock* source, const int index)
{
	if (source)
		try
		{
			if (index >= source->scalarSequence().size() || index < 0)
			{
				return nullptr;
			}
			return &source->scalarSequence().at(index);
		}
	catch (std::exception&)
	{
	}
	return nullptr;
}

uint64_t GetScalarSqSizeC(const CALUMI::UNIV::AnimationBlock* source)
{
	if (source)
		try
		{
			return source->scalarSequence().size();
		}
	catch (std::exception&)
	{
	}
	return 0;
}

int ExecuteRDPReduction_ScalarC(const CALUMI::UNIV::AnimationBlock* source, const float tolerance)
{
	if (source)
		try
		{
			source->executeRDPReduction_Scalar(tolerance);
			return 0;
		}
	catch (std::exception&)
	{
	}
	return -1;
}

bool AddPrioritySqToAnimBlockC(const CALUMI::UNIV::AnimationBlock* block, const CALUMI::UNIV::PriorityFrame* prtySq,
							   const unsigned int size, const bool overwrite)
{
	unsigned int added = 0;

	if (block && prtySq)
		try
		{
			for (unsigned int i = 0; i < size; i++)
			{
				if (!block->addPriorityEntry(prtySq[i], overwrite))
					return added;

				added++;
			}
		}
	catch (std::exception&)
	{
	}
	return added;
}

CALUMI::UNIV::PriorityFrame* GetPrioritySqArrayC(const CALUMI::UNIV::AnimationBlock* source)
{
	if (!source || source->prioritySequence().empty())
		return nullptr;

	return source->prioritySequence().data();
}

CALUMI::UNIV::PriorityFrame* GetPriorityFromSqC(const CALUMI::UNIV::AnimationBlock* source, const int index)
{
	if (source)
		try
		{
			if (index >= source->scalarSequence().size() || index < 0)
			{
				return nullptr;
			}
			return &source->prioritySequence().at(index);
		}
	catch (std::exception&)
	{
	}
	return nullptr;
}

uint64_t GetPrioritySqSizeC(const CALUMI::UNIV::AnimationBlock* source)
{
	if (source)
		try
		{
			return source->prioritySequence().size();
		}
	catch (std::exception&)
	{
	}
	return 0;
}
#pragma endregion
