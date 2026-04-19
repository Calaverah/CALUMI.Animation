//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include "internalplatform.h"
#include "utilities/CALUMI_Common.h"
#include "utilities/CALUMI_Utilities.h"
#include "univ/animation/UNIV_Animation.h"
#include <string>
#include "univ/animation/UNIV_IAnimationPackage.h"
#include <vector>
#include "internal/internalvectordef.h"
#include <algorithm>

namespace CALUMI::UNIV
{

#pragma region ANIMATION
	struct Animation::Impl
	{
		Utilities::StringContainer _animationTitle = "NO TITLE";
		AnimationBlockVector _animationBlocks;

		AnimationPackageManager _packageManager;

		Impl() = default;
	};
	Animation::Animation() { pImpl = new Impl; }
	AnimationBlockVector& Animation::animationBlocks() const
	{
		return pImpl->_animationBlocks;
	}
	const char* Animation::animationTitle() const
	{
		return pImpl->_animationTitle.c_str();
	}
	void Animation::setAnimationTitle(const char* title) const
	{
		pImpl->_animationTitle = title;
	}
	void Animation::setAnimationTitle(const Utilities::StringContainer& title) const
	{
		pImpl->_animationTitle = title;
	}
	Animation::Animation(const Utilities::StringContainer& title, const unsigned int initialBlockCount = 8) : Animation()
	{
		pImpl->_animationTitle = title;
		pImpl->_animationBlocks.reserve(initialBlockCount);
	}
	Animation::Animation(const Animation& input) : Animation()
	{
		*pImpl = *input.pImpl;
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
		if (this != &input)
			*pImpl = *input.pImpl;

		return *this;
	}

	bool Animation::addAnimationBlock(const AnimationBlock& blockToAdd, const bool overwrite) const
	{
		for (unsigned int i = 0; i < pImpl->_animationBlocks.size(); i ++)
		{
			if (SCOMPARE(pImpl->_animationBlocks.at(i).boneName(), blockToAdd.boneName()) == 0)
			{
				if (!overwrite)
					return false;

				pImpl->_animationBlocks.at(i) = blockToAdd;
				return true;
			}
		}

		pImpl->_animationBlocks.push_back(blockToAdd);
		return true;
	}

	void Animation::clearAnimationBlocks() const { pImpl->_animationBlocks.clear(); }
	uint64_t Animation::animationBlockCount() const { return pImpl->_animationBlocks.size(); }
	int Animation::findAnimationBlock(const char* boneName) const
	{
		for (int i = 0; i < pImpl->_animationBlocks.size() && i < std::numeric_limits<int>::max(); i++)
		{
			if (SCOMPARE(pImpl->_animationBlocks.at(i).boneName(), boneName) == 0)
				return i;
		}

		return -1;
	}
	AnimationPackageManager& Animation::getPackageManager() const { return pImpl->_packageManager; }

	unsigned int Animation::frameCount() const
	{
		unsigned int output = 0;
		bool entryFound = false;


		for (unsigned int i = 0; i < pImpl->_animationBlocks.size(); i++)
		{
			if (pImpl->_animationBlocks.at(i).priorityEntryCount() > 0 ||
				pImpl->_animationBlocks.at(i).scalarEntryCount() > 0 ||
				pImpl->_animationBlocks.at(i).rotationEntryCount() > 0 ||
				pImpl->_animationBlocks.at(i).translationEntryCount() > 0)
				entryFound = true;

			if (const unsigned int temp = pImpl->_animationBlocks.at(i).GetLastFrameInBlock(); temp > output)
			{
				output = temp;
			}
		}
		if(entryFound)
			output++; //adding one to sum up total frames in animation. If final frame is 50, the count is 51

		return output;
	}
	Utilities::StringContainer Animation::toJSON(const uint64_t indents) const
	{
		Utilities::StringContainer output = Utilities::Indent(indents).c_str();
		output += "{\n";
		output += std::format("{0}\"animationTitle\":\"{1}\",\n{0}\"animationBlocks\":", Utilities::Indent(indents + 1).c_str(), pImpl->_animationTitle.c_str() /*, boneCount*/).c_str(); //    \n{0}\"boneCount\":{2},
		// output += Utilities::VectorToJSON(pImpl->_animationBlocks, indents + 1);
		output += "\n";
		output += Utilities::Indent(indents).c_str();
		output += "}";
		return output;
	}

#pragma endregion

#pragma region ANIMATIONBLOCK

	struct AnimationBlock::Impl
	{
		///@privatesection
			///@{
		Utilities::StringContainer boneName = "UNNAMED";
		RotationSequence rotationSequence;
		TranslationSequence translationSequence;
		ScalarSequence scalarSequence;
		PrioritySequence prioritySequence;
		Impl() = default;
		///@}
	};

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
			pImpl->boneName = other.pImpl->boneName;
			pImpl->rotationSequence = other.pImpl->rotationSequence;
			pImpl->translationSequence = other.pImpl->translationSequence;
			pImpl->scalarSequence = other.pImpl->scalarSequence;
			pImpl->prioritySequence = other.pImpl->prioritySequence;
		}

		return *this;
	}

	unsigned int AnimationBlock::GetLastFrameInBlock() const
	{
		unsigned int output = 0;
		if (!pImpl->rotationSequence.empty()) {
			if (pImpl->rotationSequence.at(pImpl->rotationSequence.size()-1).frame() > output)
			{
				output = pImpl->rotationSequence.at(pImpl->rotationSequence.size()-1).frame();
			}
		}
		if (!pImpl->translationSequence.empty()) {
			if (pImpl->translationSequence.at(pImpl->translationSequence.size()-1).frame() > output)
			{
				output = pImpl->translationSequence.at(pImpl->translationSequence.size() - 1).frame();
			}
		}
		if (!pImpl->scalarSequence.empty()) {
			if (pImpl->scalarSequence.at(pImpl->scalarSequence.size()-1).frame() > output)
			{
				output = pImpl->scalarSequence.at(pImpl->scalarSequence.size() - 1).frame();
			}
		}
		if (!pImpl->prioritySequence.empty()) {
			if (pImpl->prioritySequence.at(pImpl->prioritySequence.size()-1).frame() > output)
			{
				output = pImpl->prioritySequence.at(pImpl->prioritySequence.size() - 1).frame();
			}
		}

		return output;
	}

	const char* AnimationBlock::boneName() const
	{
		return pImpl->boneName.c_str();
	}

	void AnimationBlock::setBoneName(const char* name) const
	{
		pImpl->boneName = name;
	}

	RotationSequence& AnimationBlock::rotationSequence() const
	{
		return pImpl->rotationSequence;
	}

	bool AnimationBlock::addRotationEntry(const Rotation& input, const bool overwrite) const
	{

		for (uint64_t i = 0; i < pImpl->rotationSequence.size(); i++)
		{
			if (pImpl->rotationSequence.at(i).frame() == input.frame())
			{
				if (!overwrite) return false;

				pImpl->rotationSequence.at(i) = input;
				return true;
			}

			if (pImpl->rotationSequence.at(i).frame() > input.frame())
			{
				pImpl->rotationSequence.insert_r(i, input);
				return true;
			}
		}

		pImpl->rotationSequence.push_back(input);
		return true;
	}

	bool AnimationBlock::removeRotationEntry(const unsigned int frame) const
	{
		for (unsigned int i = 0; i < pImpl->rotationSequence.size(); i++)
		{
			if (frame == pImpl->rotationSequence.at(i).frame())
			{
				pImpl->rotationSequence.erase(i);
				return true;
			}
		}

		return false;
	}

	void AnimationBlock::clearRotationEntries() const
	{
		pImpl->rotationSequence.clear();
	}

	uint64_t AnimationBlock::rotationEntryCount() const { return pImpl->rotationSequence.size(); }

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
		pImpl->rotationSequence = s_RDP_Rotation_Recursive(pImpl->rotationSequence, tolerance);
	}

	TranslationSequence& AnimationBlock::translationSequence() const
	{
		return pImpl->translationSequence;
	}

	bool AnimationBlock::addTranslationEntry(const Translation& input, const bool overwrite) const
	{
		for (uint64_t i = 0; i < pImpl->translationSequence.size(); i++)
		{
			if (pImpl->translationSequence.at(i).frame() == input.frame())
			{
				if (!overwrite)
					return false;

				pImpl->translationSequence.at(i) = input;
				return true;
			}

			if (pImpl->translationSequence.at(i).frame() > input.frame())
			{
				pImpl->translationSequence.insert_r(i, input);
				return true;
			}
		}

		pImpl->translationSequence.push_back(input);
		return true;
	}

	bool AnimationBlock::removeTranslationEntry(const unsigned int frame) const
	{
		for (unsigned int i = 0; i < pImpl->translationSequence.size(); i++)
		{
			if (frame == pImpl->translationSequence.at(i).frame())
			{
				pImpl->translationSequence.erase(i);
				return true;
			}
		}

		return false;
	}

	void AnimationBlock::clearTranslationEntries() const
	{
		pImpl->translationSequence.clear();
	}

	uint64_t AnimationBlock::translationEntryCount() const
	{
		return pImpl->translationSequence.size();
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
		pImpl->translationSequence = s_RDP_Translation_Recursive(pImpl->translationSequence, tolerance);
	}

	ScalarSequence& AnimationBlock::scalarSequence() const
	{
		return pImpl->scalarSequence;
	}

	bool AnimationBlock::addScalarEntry(const Scalar& input, const bool overwrite) const
	{
		for (uint64_t i = 0; i < pImpl->scalarSequence.size(); i++)
		{
			if (pImpl->scalarSequence.at(i).frame() == input.frame())
			{
				if (!overwrite)
					return false;

				pImpl->scalarSequence.at(i) = input;
				return true;

			}

			if (pImpl->scalarSequence.at(i).frame() > input.frame())
			{
				pImpl->scalarSequence.insert_r(i, input);
				return true;
			}
		}

		pImpl->scalarSequence.push_back(input);
		return true;
	}

	bool AnimationBlock::removeScalarEntry(const unsigned int frame) const
	{
		for (unsigned int i = 0; i < pImpl->scalarSequence.size(); i++)
		{
			if (frame == pImpl->scalarSequence.at(i).frame())
			{
				pImpl->scalarSequence.erase(i);
				return true;
			}
		}

		return false;
	}

	void AnimationBlock::clearScalarEntries() const
	{
		pImpl->scalarSequence.clear();
	}

	uint64_t AnimationBlock::scalarEntryCount() const { return pImpl->scalarSequence.size(); }

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
		pImpl->scalarSequence = s_RDP_Scalar_Recursive(pImpl->scalarSequence, tolerance);
	}

	PrioritySequence& AnimationBlock::prioritySequence() const
	{
		return pImpl->prioritySequence;
	}

	bool AnimationBlock::addPriorityEntry(const Priority& input, const bool overwrite) const
	{
		for (uint64_t i = 0; i < pImpl->prioritySequence.size(); i++)
		{
			if (pImpl->prioritySequence.at(i).frame() == input.frame())
			{
				if (!overwrite) return false;

				pImpl->prioritySequence.at(i) = input;
				return true;

			}

			if (pImpl->prioritySequence.at(i).frame() > input.frame())
			{
				pImpl->prioritySequence.insert_r(i, input);
				return true;
			}
		}

		pImpl->prioritySequence.push_back(input);
		return true;
	}

	bool AnimationBlock::removePriorityEntry(const unsigned int frame) const
	{
		for (unsigned int i = 0; i < pImpl->prioritySequence.size(); i++)
		{
			if (frame == pImpl->prioritySequence.at(i).frame())
			{
				pImpl->prioritySequence.erase(i);
				return true;
			}
		}

		return false;
	}

	void AnimationBlock::clearPriorityEntries() const
	{
		pImpl->prioritySequence.clear();
	}

	uint64_t AnimationBlock::priorityEntryCount() const { return pImpl->prioritySequence.size(); }

	Utilities::StringContainer AnimationBlock::toJSON(const uint64_t indents) const {
		Utilities::StringContainer output = Utilities::Indent(indents).c_str();
		output += "{\n";
		output += std::format("{0}\"boneName\":\"{1}\",\n", Utilities::Indent(indents + 1).c_str(), pImpl->boneName.c_str()).c_str();
		// output += std::format("{0}\"rotationSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->rotationSequence, indents + 1).c_str()).c_str();
		// output += std::format("{0}\"translationSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->translationSequence, indents + 1).c_str()).c_str();
		// output += std::format("{0}\"scalarSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->scalarSequence, indents + 1).c_str()).c_str();
		// output += std::format("{0}\"prioritySequence\":{1}\n{2}", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->prioritySequence, indents + 1).c_str(), Utilities::Indent(indents).c_str()).c_str();
		output += "}";
		return output;
	}

#pragma endregion

	VECTORDEF(AnimationBlockVector, AnimationBlock)
	VECTORDEF(AnimationVector, Animation)

#pragma region EXTERN"C"

	Animation* CreateAnimationC(const char* animationTitle, const unsigned int rigBoneCount)
	{
		const auto outputAnimation = new Animation(animationTitle, rigBoneCount);
		return outputAnimation;
	}
	AnimationBlock* GetAnimationBlockC(const Animation* source, const char* boneName, Utilities::StringContainer* errorMessage)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();

		const int index = source->findAnimationBlock(boneName);

		if (source->animationBlocks().size() <= index || index < 0)
		{
			*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
			return nullptr;
		}
		return &source->animationBlocks().at(index);
	}
	uint64_t GetAnimationBlockCountC(const Animation* source)
	{
		return source->animationBlocks().size();
	}
	const char* GetAnimationTitleC(const Animation* source)
	{
		return source->animationTitle();
	}
	uint64_t GetFrameCountC(const Animation* source)
	{
		return source->frameCount();
	}
	bool DeleteAnimationC(const Animation* ptr)
	{
		if (ptr)
		{
			delete ptr;
			ptr = nullptr;
			return true;
		}
		return false;
	}
	bool AddAnimBlockToAnimationC(const Animation* anim, const AnimationBlock* blockToAdd, const bool overwrite, Utilities::StringContainer* errorMessage)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();

		if (!anim->addAnimationBlock(*blockToAdd, overwrite))
		{
			*errorMessageHolder += "[CALUMI.Animation API] Existing Animation Block Found and Not Overwritten!";
			return false;
		}
		delete blockToAdd;
		*errorMessageHolder += "[CALUMI.Animation API] Animation Block Copied Into Animation Successfully. Original Block Deleted!";
		return true;
	}
	AnimationBlock* CreateAnimBlockC(const char* boneName, Utilities::StringContainer* errorMessage)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();

		if (std::string(boneName).empty())
		{
			*errorMessageHolder += "[CALUMI.Animation API] Animation Block Must Have Bone Name!";
			return nullptr;
		}
		const auto outputAnimationBlock = new AnimationBlock;
		outputAnimationBlock->setBoneName(boneName);
		*errorMessageHolder += "[CALUMI.Animation API] Animation Block Created!";
		return outputAnimationBlock;
	}
	bool DeleteAnimationBlockC(const AnimationBlock* ptr)
	{
		if (ptr)
		{
			delete ptr;
			ptr = nullptr;
			return true;
		}
		return false;
	}
	const char* GetAnimBlockBoneNameC(const AnimationBlock* source)
	{
		return source->boneName();
	}
	unsigned int GetLastFrameInAnimBlockC(const AnimationBlock* source)
	{
		return source->GetLastFrameInBlock();
	}
	bool AddRotationSqToAnimBlockC(const AnimationBlock* block, const Rotation* rotSq, const unsigned int size, const bool overwrite)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			if(!block->addRotationEntry(rotSq[i],overwrite))
				return false;
		}
		return true;
	}
	Rotation* GetRotationSqArrayC(const AnimationBlock* source)
	{
		if(source->rotationSequence().empty())
			return nullptr;

		return source->rotationSequence().data();
	}
	Rotation* GetRotationFromSqC(const AnimationBlock* source, const int index, Utilities::StringContainer* errorMessage)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();

		if (index >= source->rotationSequence().size() || index < 0)
		{
			*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
			return nullptr;
		}
		return &source->rotationSequence().at(index);
	}
	uint64_t GetRotationSqSizeC(const AnimationBlock* source)
	{
		return source->rotationSequence().size();
	}
	void ExecuteRDPReduction_RotationC(const AnimationBlock* source, const float tolerance)
	{
		if (!source) return;

		source->executeRDPReduction_Rotation(tolerance);
	}
	bool AddTranslationSqToAnimBlockC(const AnimationBlock* block, const Translation* trnSq, const unsigned int size, const bool overwrite)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			if (!block->addTranslationEntry(trnSq[i], overwrite))
				return false;
		}
		return true;
	}
	Translation* GetTranslationSqArrayC(const AnimationBlock* source)
	{
		if (source->translationSequence().empty())
			return nullptr;

		return source->translationSequence().data();
	}
	Translation* GetTranslationFromSqC(const AnimationBlock* source, const int index, Utilities::StringContainer* errorMessage)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();

		if (index >= source->translationSequence().size() || index < 0)
		{
			*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
			return nullptr;
		}
		return &source->translationSequence().at(index);
	}
	uint64_t GetTranslationSqSizeC(const AnimationBlock* source)
	{
		return source->translationSequence().size();
	}
	void ExecuteRDPReduction_TranslationC(const AnimationBlock* source, const float tolerance)
	{
		if (!source) return;

		source->executeRDPReduction_Translation(tolerance);
	}
	bool AddScalarSqToAnimBlockC(const AnimationBlock* block, const Scalar* sclrSq, const unsigned int size, const bool overwrite)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			if (!block->addScalarEntry(sclrSq[i], overwrite))
				return false;
		}
		return true;
	}
	Scalar* GetScalarSqArrayC(const AnimationBlock* source)
	{
		if(source->scalarSequence().empty())
			return nullptr;

		return source->scalarSequence().data();
	}
	Scalar* GetScalarFromSqC(const AnimationBlock* source, const int index, Utilities::StringContainer* errorMessage)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();

		if (index >= source->scalarSequence().size() || index < 0)
		{
			*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
			return nullptr;
		}
		return &source->scalarSequence().at(index);
	}
	uint64_t GetScalarSqSizeC(const AnimationBlock* source)
	{
		return source->scalarSequence().size();
	}
	void ExecuteRDPReduction_ScalarC(const AnimationBlock* source, const float tolerance)
	{
		if (!source) return;

		source->executeRDPReduction_Scalar(tolerance);
	}
	bool AddPrioritySqToAnimBlockC(const AnimationBlock* block, const Priority* prtySq, const unsigned int size, const bool overwrite)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			if (!block->addPriorityEntry(prtySq[i], overwrite))
				return false;
		}
		return true;
	}
	Priority* GetPrioritySqArrayC(const AnimationBlock* source)
	{
		if(source->prioritySequence().empty())
			return nullptr;

		return source->prioritySequence().data();
	}
	Priority* GetPriorityFromSqC(const AnimationBlock* source, const int index, Utilities::StringContainer* errorMessage)
	{
		Utilities::StringContainer tempErrorMessage;
		Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
		errorMessageHolder->clear();

		if (index >= source->prioritySequence().size() || index < 0)
		{
			*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
			return nullptr;
		}
		return &source->prioritySequence().at(index);
	}
	uint64_t GetPrioritySqSizeC(const AnimationBlock* source)
	{
		return source->prioritySequence().size();
	}
#pragma endregion

}
