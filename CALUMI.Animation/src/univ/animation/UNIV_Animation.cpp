//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include "internalplatform.h"
#include "utilities/CALUMI_Common.h"
#include "utilities/CALUMI_Utilities.h"
#include "univ/animation/UNIV_Animation.h"
#include <string>
#include "univ/animation/UNIV_IAnimationPackage.h"

namespace CALUMI {
	namespace UNIV {

#pragma region ANIMATION
		struct Animation::Impl
		{
			Utilities::StringContainer _animationTitle = "NO TITLE";
			Utilities::VectorContainer<AnimationBlock> _animationBlocks;

			AnimationPackageManager _packageManager;
			
			Impl() = default;
		};
		Animation::Animation() { pImpl = new Impl; }
		Utilities::VectorContainer<AnimationBlock>& Animation::AnimationBlocks() const
		{
			return pImpl->_animationBlocks;
		}
		const char* Animation::AnimationTitle() const
		{
			return pImpl->_animationTitle.c_str();
		}
		void Animation::AnimationTitle(const char* title)
		{
			pImpl->_animationTitle = title;
		}
		void Animation::AnimationTitle(const Utilities::StringContainer& title)
		{
			pImpl->_animationTitle = title;
		}
		UNIV::Animation::Animation(const Utilities::StringContainer& title, unsigned int initialBlockCount = 8) : Animation()
		{
			pImpl->_animationTitle = title;
			//boneCount = initialBoneCount;
			pImpl->_animationBlocks.reserve(initialBlockCount);
		}
		UNIV::Animation::Animation(const Animation& input) : Animation()
		{
			*pImpl = *(input.pImpl);
		}
		Animation::~Animation() { if (pImpl) delete pImpl; }
		Animation& Animation::operator=(const Animation& input) { *pImpl = *(input.pImpl); return *this; }

		bool UNIV::Animation::AddAnimationBlock(AnimationBlock& blockToAdd, bool overwrite)
		{
			for (unsigned int i = 0; i < pImpl->_animationBlocks.size(); i ++)
			{
				if (pImpl->_animationBlocks.at(i).BoneIndex() == blockToAdd.BoneIndex())
				{
					if (!overwrite) return false;
					else 
					{
						pImpl->_animationBlocks.at(i) = blockToAdd;
						return true;
					}
				}
			}

			pImpl->_animationBlocks.push_back(blockToAdd);
			return true;
		}

		//inline CALUMIANIMATION_API UNIV::AnimationBlock GetAnimationBlock(unsigned int i) const { return _animationBlocks.at(i); }
		//inline CALUMIANIMATION_API std::vector<UNIV::AnimationBlock> GetAnimationBlockVector() const { return _animationBlocks; }
		void Animation::ClearAnimationBlocks() { pImpl->_animationBlocks.clear(); }
		std::size_t Animation::GetAnimationBlockCount() const { return pImpl->_animationBlocks.size(); }
		AnimationPackageManager& Animation::getPackageManager() const { return pImpl->_packageManager; }

		unsigned int UNIV::Animation::GetFrameCount()
		{
			unsigned int output = 0;
			for (unsigned int i = 0; i < pImpl->_animationBlocks.size(); i++)
			{
				unsigned int temp = pImpl->_animationBlocks.at(i).GetLastFrameInBlock();
				if (temp > output)
				{
					output = temp;
				}
			}
			output++; //adding one to sum up total frames in animation. If final frame is 50, the count is 51
			return output;
		}
		Utilities::StringContainer Animation::ToJSON(const std::size_t indents) const
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
			int boneIndex = -2;
			Utilities::StringContainer boneName = "UNNAMED";
			Utilities::VectorContainer<CALUMI::UNIV::Rotation> rotationSequence;
			Utilities::VectorContainer<CALUMI::UNIV::Translation> translationSequence;
			Utilities::VectorContainer<CALUMI::UNIV::Scalar> scalarSequence;
			Utilities::VectorContainer<CALUMI::UNIV::Priority> prioritySequence;
			Impl() = default;
			///@}
        };

		AnimationBlock::AnimationBlock() { pImpl = new Impl; }
		AnimationBlock::~AnimationBlock() { if (pImpl) delete pImpl; }
		AnimationBlock::AnimationBlock(const AnimationBlock& input) : AnimationBlock() { *pImpl = *(input.pImpl); }
		UNIV::AnimationBlock& AnimationBlock::operator=(const AnimationBlock& other)
		{
			//skip reassignment functions
			if (pImpl == other.pImpl) return *this;

			pImpl->boneIndex = other.pImpl->boneIndex;
			pImpl->boneName = other.pImpl->boneName;
			pImpl->rotationSequence = other.pImpl->rotationSequence;
			pImpl->translationSequence = other.pImpl->translationSequence;
			pImpl->scalarSequence = other.pImpl->scalarSequence;
			pImpl->prioritySequence = other.pImpl->prioritySequence;

			return *this;
		}

		unsigned int UNIV::AnimationBlock::GetLastFrameInBlock()
		{
			unsigned int output = 0;
			if (pImpl->rotationSequence.size() > 0) {
				if (pImpl->rotationSequence.at(pImpl->rotationSequence.size()-1).Frame() > output)
				{
					output = pImpl->rotationSequence.at(pImpl->rotationSequence.size()-1).Frame();
				}
			}
			if (pImpl->translationSequence.size() > 0) {
				if (pImpl->translationSequence.at(pImpl->translationSequence.size()-1).Frame() > output)
				{
					output = pImpl->translationSequence.at(pImpl->translationSequence.size() - 1).Frame();
				}
			}
			if (pImpl->scalarSequence.size() > 0) {
				if (pImpl->scalarSequence.at(pImpl->scalarSequence.size()-1).Frame() > output)
				{
					output = pImpl->scalarSequence.at(pImpl->scalarSequence.size() - 1).Frame();
				}
			}
			if (pImpl->prioritySequence.size() > 0) {
				if (pImpl->prioritySequence.at(pImpl->prioritySequence.size()-1).Frame() > output)
				{
					output = pImpl->prioritySequence.at(pImpl->prioritySequence.size() - 1).Frame();
				}
			}

			return output;
		}

		int AnimationBlock::BoneIndex() const
		{
			return pImpl->boneIndex;
		}

		void AnimationBlock::BoneIndex(int idx)
		{
			pImpl->boneIndex = idx;
		}

		const char* AnimationBlock::BoneName() const
		{
			return pImpl->boneName.c_str();
		}

		void AnimationBlock::BoneName(const char* name)
		{
			pImpl->boneName = name;
		}

		Utilities::VectorContainer<CALUMI::UNIV::Rotation>& AnimationBlock::RotationSequence() const
		{
			return pImpl->rotationSequence;
		}

		bool AnimationBlock::AddRotationEntry(CALUMI::UNIV::Rotation& input, bool overwrite)
		{

			for (std::size_t i = 0; i < pImpl->rotationSequence.size(); i++)
			{
				if (pImpl->rotationSequence.at(i).Frame() == input.Frame())
				{
					if (!overwrite) return false;
					else
					{
						pImpl->rotationSequence.at(i) = input;
						return true;
					}
				}
				else if (pImpl->rotationSequence.at(i).Frame() > input.Frame())
				{
					pImpl->rotationSequence.insert_r(i, input);
					return true;
				}
			}
			
			pImpl->rotationSequence.push_back(input);
			return true;
		}

		bool AnimationBlock::RemoveRotationEntry(unsigned int frame)
		{
			for (unsigned int i = 0; i < pImpl->rotationSequence.size(); i++)
			{
				if (frame == pImpl->rotationSequence.at(i).Frame())
				{
					pImpl->rotationSequence.erase(i);
					return true;
				}
			}

			return false;
		}

		void AnimationBlock::ClearRotationEntries() { pImpl->rotationSequence.clear(); }

		std::size_t AnimationBlock::GetRotationEntryCount() const { return pImpl->rotationSequence.size(); }

		static Utilities::VectorContainer<UNIV::Rotation> _RDP_Rotation_Recursive(Utilities::VectorContainer<UNIV::Rotation> input, float tolerance)
		{
			if (input.size() <= 2) return input;


			//Maximum Distance
			float dMax = 0.0f;
			std::size_t index = 0;
			auto f0 = input.at(0).Frame();
			auto fn = input.at(input.size() - 1).Frame();

			bool skip = false;

			float distance = input.at(0).RotationQuaternion().AngularDistance(input.at(input.size()-1).RotationQuaternion());
			//std::println("size {}, dist {}, start/finish {}/{}", input.size(),distance,input.at(0).Frame(),input.at(input.size()-1).Frame());

			//We force the split in this range of entries as Slerp for 0-180 degrees will have an undefined direction 
			// and almost always will give an slerp/key angle of some value above 180 degrees...
			// By setting the index to half (or at least 1) and skipping any index assignment we force at least one key in between the 0-180 degree 
			// interpolation allowing the user to keep their intended direction.
			if (distance >= Math::ToRadians(180.0) - tolerance) {
				index = input.size()/2 > 0 ? input.size()/2 : 1;
				skip = true;
			}

			if(!skip) {
				for (std::size_t i = 1; i < input.size()-1; i++)
				{

					float t = static_cast<float>(input.at(i).Frame() - f0) / (fn - f0);

					//Slerp isn't quite the "perpindicular" distance needed for this algorithm, but it makes for a practical compromise
					Math::Quaternion interp = input.at(0).RotationQuaternion().Slerp(input.at(input.size() - 1).RotationQuaternion(), t);
					
					float angle = interp.AngularDistance(input.at(i).RotationQuaternion());
					//std::println("--angle {}, t {}, interp {}, f0/fn {}/{}", angle, t, interp.ToString().c_str(),f0,fn);
					
					if (angle > dMax) {
						dMax = angle;
						index = i;
					}
				}
			}

			Utilities::VectorContainer<UNIV::Rotation> output;

			if (dMax > tolerance || skip) {
				auto left = _RDP_Rotation_Recursive(input.range(0, index), tolerance);
				auto right = _RDP_Rotation_Recursive(input.range(index, input.size() - 1),tolerance);

				output.reserve(left.size() + right.size() - 1);

				for (std::size_t i = 0; i < left.size(); i++)
				{
					output.push_back(left.at(i));
				}

				for (std::size_t i = 1; i < right.size(); i++)
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

		void AnimationBlock::ExecuteRDPReduction_Rotation(float tolerance)
		{
			pImpl->rotationSequence = _RDP_Rotation_Recursive(pImpl->rotationSequence, tolerance);
		}

		Utilities::VectorContainer<CALUMI::UNIV::Translation>& AnimationBlock::TranslationSequence() const
		{
			return pImpl->translationSequence;
		}

		bool AnimationBlock::AddTranslationEntry(CALUMI::UNIV::Translation& input, bool overwrite)
		{
			for (std::size_t i = 0; i < pImpl->translationSequence.size(); i++)
			{
				if (pImpl->translationSequence.at(i).Frame() == input.Frame())
				{
					if (!overwrite) return false;
					else
					{
						pImpl->translationSequence.at(i) = input;
						return true;
					}
				}
				else if (pImpl->translationSequence.at(i).Frame() > input.Frame())
				{
					pImpl->translationSequence.insert_r(i, input);
					return true;
				}
			}

			pImpl->translationSequence.push_back(input);
			return true;
		}

		bool AnimationBlock::RemoveTranslationEntry(unsigned int frame)
		{
			for (unsigned int i = 0; i < pImpl->translationSequence.size(); i++)
			{
				if (frame == pImpl->translationSequence.at(i).Frame())
				{
					pImpl->translationSequence.erase(i);
					return true;
				}
			}

			return false;
		}

		void AnimationBlock::ClearTranslationEntries() { pImpl->translationSequence.clear(); }

		std::size_t AnimationBlock::GetTranslationEntryCount() const { return pImpl->translationSequence.size(); }

		static Utilities::VectorContainer<UNIV::Translation> _RDP_Translation_Recursive(Utilities::VectorContainer<UNIV::Translation> input, float tolerance)
		{
			if (input.size() <= 2) return input;


			//Maximum Distance
			double dMax = 0.0f;
			std::size_t index = 0;
			auto f0 = input.at(0).Frame();
			auto fn = input.at(input.size() - 1).Frame();

			for (std::size_t i = 1; i < input.size() - 1; i++)
			{
				double t = static_cast<float>(input.at(i).Frame() - f0) / (fn - f0);

				//Lerp isn't quite the "perpindicular" distance needed for this algorithm, but it makes for a practical compromise
				Math::Vector3D interp = input.at(0).TranslationVector().Lerp(input.at(input.size() - 1).TranslationVector(), t);
				double distance = (interp - input.at(i).TranslationVector()).Length();

				if (distance > dMax) {
					dMax = distance;
					index = i;
				}
			}

			Utilities::VectorContainer<UNIV::Translation> output;

			if (dMax > tolerance) {

				auto left = _RDP_Translation_Recursive(input.range(0, index), tolerance);
				auto right = _RDP_Translation_Recursive(input.range(index, input.size() - 1), tolerance);

				output.reserve(left.size() + right.size() - 1);

				for (std::size_t i = 0; i < left.size(); i++)
				{
					output.push_back(left.at(i));
				}

				for (std::size_t i = 1; i < right.size(); i++)
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

		void AnimationBlock::ExecuteRDPReduction_Translation(float tolerance)
		{
			pImpl->translationSequence = _RDP_Translation_Recursive(pImpl->translationSequence, tolerance);
		}

		Utilities::VectorContainer<CALUMI::UNIV::Scalar>& AnimationBlock::ScalarSequence() const
		{
			return pImpl->scalarSequence;
		}

		bool AnimationBlock::AddScalarEntry(CALUMI::UNIV::Scalar& input, bool overwrite)
		{
			for (std::size_t i = 0; i < pImpl->scalarSequence.size(); i++)
			{
				if (pImpl->scalarSequence.at(i).Frame() == input.Frame())
				{
					if (!overwrite) return false;
					else
					{
						pImpl->scalarSequence.at(i) = input;
						return true;
					}
				}
				else if (pImpl->scalarSequence.at(i).Frame() > input.Frame())
				{
					pImpl->scalarSequence.insert_r(i, input);
					return true;
				}
			}

			pImpl->scalarSequence.push_back(input);
			return true;
		}

		bool AnimationBlock::RemoveScalarEntry(unsigned int frame)
		{
			for (unsigned int i = 0; i < pImpl->scalarSequence.size(); i++)
			{
				if (frame == pImpl->scalarSequence.at(i).Frame())
				{
					pImpl->scalarSequence.erase(i);
					return true;
				}
			}

			return false;
		}

		void AnimationBlock::ClearScalarEntries() { pImpl->scalarSequence.clear(); }

		std::size_t AnimationBlock::GetScalarEntryCount() const { return pImpl->scalarSequence.size(); }

		static Utilities::VectorContainer<UNIV::Scalar> _RDP_Scalar_Recursive(Utilities::VectorContainer<UNIV::Scalar> input, float tolerance)
		{
			if (input.size() <= 2) return input;


			//Maximum Distance
			double dMax = 0.0f;
			std::size_t index = 0;
			auto f0 = input.at(0).Frame();
			auto fN = input.at(input.size() - 1).Frame();

			Math::Vector2D vec0(f0, input.at(0).ScalarValue());
			Math::Vector2D vecN(fN, input.at(input.size()-1).ScalarValue());

			for (std::size_t i = 1; i < input.size() - 1; i++)
			{
				double t = static_cast<float>(input.at(i).Frame() - f0) / (fN - f0);

				//Lerp isn't quite the "perpindicular" distance needed for this algorithm, but it makes for a practical compromise
				Math::Vector2D interp = vec0.Lerp(vecN, t);
				Math::Vector2D current(input.at(i).Frame(), input.at(i).ScalarValue());
				double distance = (interp - current).Length();

				if (distance > dMax) {
					dMax = distance;
					index = i;
				}
			}

			Utilities::VectorContainer<UNIV::Scalar> output;

			if (dMax > tolerance) {

				auto left = _RDP_Scalar_Recursive(input.range(0, index), tolerance);
				auto right = _RDP_Scalar_Recursive(input.range(index, input.size() - 1), tolerance);

				output.reserve(left.size() + right.size() - 1);

				for (std::size_t i = 0; i < left.size(); i++)
				{
					output.push_back(left.at(i));
				}

				for (std::size_t i = 1; i < right.size(); i++)
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

		void AnimationBlock::ExecuteRDPReduction_Scalar(float tolerance)
		{
			pImpl->scalarSequence = _RDP_Scalar_Recursive(pImpl->scalarSequence, tolerance);
		}

		Utilities::VectorContainer<CALUMI::UNIV::Priority>& AnimationBlock::PrioritySequence()
		{
			return pImpl->prioritySequence;
		}

		bool AnimationBlock::AddPriorityEntry(CALUMI::UNIV::Priority& input, bool overwrite)
		{
			for (std::size_t i = 0; i < pImpl->prioritySequence.size(); i++)
			{
				if (pImpl->prioritySequence.at(i).Frame() == input.Frame())
				{
					if (!overwrite) return false;
					else
					{
						pImpl->prioritySequence.at(i) = input;
						return true;
					}
				}
				else if (pImpl->prioritySequence.at(i).Frame() > input.Frame())
				{
					pImpl->prioritySequence.insert_r(i, input);
					return true;
				}
			}

			pImpl->prioritySequence.push_back(input);
			return true;
		}

		bool AnimationBlock::RemovePriorityEntry(unsigned int frame)
		{
			for (unsigned int i = 0; i < pImpl->prioritySequence.size(); i++)
			{
				if (frame == pImpl->prioritySequence.at(i).Frame())
				{
					pImpl->prioritySequence.erase(i);
					return true;
				}
			}

			return false;
		}

		void AnimationBlock::ClearPriorityEntries() { pImpl->prioritySequence.clear(); }

		std::size_t AnimationBlock::GetPriorityEntryCount() const { return pImpl->prioritySequence.size(); }

		Utilities::StringContainer AnimationBlock::ToJSON(const std::size_t indents) const {
			Utilities::StringContainer output = Utilities::Indent(indents).c_str();
			output += "{\n";
			output += std::format("{0}\"boneName\":\"{1}\",\n{0}\"boneIndex\":{2},\n", Utilities::Indent(indents + 1).c_str(), pImpl->boneName.c_str(), pImpl->boneIndex).c_str();
            // output += std::format("{0}\"rotationSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->rotationSequence, indents + 1).c_str()).c_str();
            // output += std::format("{0}\"translationSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->translationSequence, indents + 1).c_str()).c_str();
            // output += std::format("{0}\"scalarSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->scalarSequence, indents + 1).c_str()).c_str();
            // output += std::format("{0}\"prioritySequence\":{1}\n{2}", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->prioritySequence, indents + 1).c_str(), Utilities::Indent(indents).c_str()).c_str();
			output += "}";
			return output;
		}

		bool AnimationBlock::operator<(const AnimationBlock& other) const
		{
			return pImpl->boneIndex < other.pImpl->boneIndex;
		}
		bool AnimationBlock::operator>(const AnimationBlock& other) const
		{
			return pImpl->boneIndex > other.pImpl->boneIndex;
		}
#pragma endregion

		
#pragma region EXTERN"C"

		Animation* CreateAnimationC(const char* animationTitle, unsigned int rigBoneCount)
		{
			Animation* outputAnimation = new Animation(animationTitle, rigBoneCount);
			return outputAnimation;
		}
		AnimationBlock* GetAnimationBlockC(Animation* source, int index, Utilities::StringContainer* errorMessage)
		{
			Utilities::StringContainer tempErrorMessage;
			Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
			errorMessageHolder->clear();

			if (source->AnimationBlocks().size() <= index || index < 0)
			{
				*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->AnimationBlocks().at(index);
		}
		std::size_t GetAnimationBlockCountC(Animation* source)
		{
			return source->AnimationBlocks().size();
		}
		const char* GetAnimationTitleC(Animation* source)
		{
			return source->AnimationTitle();
		}
		/*size_t GetAnimationBoneCountC(Animation* source)
		{
			return source->boneCount;
		}*/
		std::size_t GetFrameCountC(Animation* source)
		{
			return source->GetFrameCount();
		}
		bool DeleteAnimationC(Animation* ptr)
		{
			if (ptr)
			{
				delete ptr;
				ptr = nullptr;
				return true;
			}
			return false;
		}
		bool AddAnimBlockToAnimationC(Animation* anim, AnimationBlock* blockToAdd, bool overwrite, Utilities::StringContainer* errorMessage)
		{
			Utilities::StringContainer tempErrorMessage;
			Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
			errorMessageHolder->clear();

			if (!anim->AddAnimationBlock(*blockToAdd, overwrite))
			{
				*errorMessageHolder += "[CALUMI.Animation API] Existing Animation Block Found and Not Overwritten!";
				return false;
			}
			delete blockToAdd;
			*errorMessageHolder += "[CALUMI.Animation API] Animation Block Copied Into Animation Successfully. Original Block Deleted!";
			return true;
		}
		AnimationBlock* CreateAnimBlockC(const char* boneName, int boneIndex, Utilities::StringContainer* errorMessage)
		{
			Utilities::StringContainer tempErrorMessage;
			Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
			errorMessageHolder->clear();

            if (std::string(boneName).empty())
			{
				*errorMessageHolder += "[CALUMI.Animation API] Animation Block Must Have Bone Name!";
				return nullptr;
			}
			if (boneIndex < 0)
			{
				*errorMessageHolder += "[CALUMI.Animation API] Animation Block Must Have Valid Bone Index! (ix > -1)";
				return nullptr;
			}
			AnimationBlock* outputAnimationBlock = new AnimationBlock;
			outputAnimationBlock->BoneIndex(boneIndex);
			outputAnimationBlock->BoneName(boneName);
			*errorMessageHolder += "[CALUMI.Animation API] Animation Block Created!";
			return outputAnimationBlock;
		}
		bool DeleteAnimationBlockC(AnimationBlock* ptr)
		{
			if (ptr)
			{
				delete ptr;
				ptr = nullptr;
				return true;
			}
			return false;
		}
		const char* GetAnimBlockBoneNameC(AnimationBlock* source)
		{
			return source->BoneName();
		}
		int GetAnimBlockBoneIndexC(AnimationBlock* source)
		{
			return source->BoneIndex();
		}
		unsigned int GetLastFrameInAnimBlockC(AnimationBlock* source)
		{
			return source->GetLastFrameInBlock();
		}
		bool AddRotationSqToAnimBlockC(AnimationBlock* block, Rotation* rotSq, unsigned int size, bool overwrite)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				if(!block->AddRotationEntry((rotSq[i]),overwrite)) return false;
			}
			return true;
		}
		Rotation* GetRotationSqArrayC(AnimationBlock* source)
		{
			if(source->RotationSequence().empty())
			return nullptr;

			return source->RotationSequence().data();
		}
		Rotation* GetRotationFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage)
		{
			Utilities::StringContainer tempErrorMessage;
			Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
			errorMessageHolder->clear();

			if (index >= source->RotationSequence().size() || index < 0)
			{
				*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->RotationSequence().at(index);
		}
		std::size_t GetRotationSqSizeC(AnimationBlock* source)
		{
			return source->RotationSequence().size();
		}
		void ExecuteRDPReduction_RotationC(AnimationBlock* source, float tolerance)
		{
			if (!source) return;

			source->ExecuteRDPReduction_Rotation(tolerance);
		}
		bool AddTranslationSqToAnimBlockC(AnimationBlock* block, Translation* trnSq, unsigned int size, bool overwrite)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				if (!block->AddTranslationEntry((trnSq[i]), overwrite)) return false;
			}
			return true;
		}
		Translation* GetTranslationSqArrayC(AnimationBlock* source)
		{
			if (source->TranslationSequence().empty())
				return nullptr;

			return source->TranslationSequence().data();
		}
		Translation* GetTranslationFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage)
		{
			Utilities::StringContainer tempErrorMessage;
			Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
			errorMessageHolder->clear();

			if (index >= source->TranslationSequence().size() || index < 0)
			{
				*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->TranslationSequence().at(index);
		}
		std::size_t GetTranslationSqSizeC(AnimationBlock* source)
		{
			return source->TranslationSequence().size();
		}
		void ExecuteRDPReduction_TranslationC(AnimationBlock* source, float tolerance)
		{
			if (!source) return;

			source->ExecuteRDPReduction_Translation(tolerance);
		}
		bool AddScalarSqToAnimBlockC(AnimationBlock* block, Scalar* sclrSq, unsigned int size, bool overwrite)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				if (!block->AddScalarEntry((sclrSq[i]), overwrite)) return false;
			}
			return true;
		}
		Scalar* GetScalarSqArrayC(AnimationBlock* source)
		{
			if(source->ScalarSequence().empty())
			return nullptr;

			return source->ScalarSequence().data();
		}
		Scalar* GetScalarFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage)
		{
			Utilities::StringContainer tempErrorMessage;
			Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
			errorMessageHolder->clear();

			if (index >= source->ScalarSequence().size() || index < 0)
			{
				*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->ScalarSequence().at(index);
		}
		std::size_t GetScalarSqSizeC(AnimationBlock* source)
		{
			return source->ScalarSequence().size();
		}
		void ExecuteRDPReduction_ScalarC(AnimationBlock* source, float tolerance)
		{
			if (!source) return;

			source->ExecuteRDPReduction_Scalar(tolerance);
		}
		bool AddPrioritySqToAnimBlockC(AnimationBlock* block, Priority* prtySq, unsigned int size, bool overwrite)
		{
			for (unsigned int i = 0; i < size; i++)
			{
				if (!block->AddPriorityEntry((prtySq[i]), overwrite)) return false;
			}
			return true;
		}
		Priority* GetPrioritySqArrayC(AnimationBlock* source)
		{
			if(source->PrioritySequence().empty())
			return nullptr;

			return source->PrioritySequence().data();
		}
		Priority* GetPriorityFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage)
		{
			Utilities::StringContainer tempErrorMessage;
			Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
			errorMessageHolder->clear();

			if (index >= source->PrioritySequence().size() || index < 0)
			{
				*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->PrioritySequence().at(index);
		}
		std::size_t GetPrioritySqSizeC(AnimationBlock* source)
		{
			return source->PrioritySequence().size();
		}
#pragma endregion

	}
	
}
