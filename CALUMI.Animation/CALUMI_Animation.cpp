//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_Animation.h"
#include "CALUMI_Utilities.h"
#include <format>
#include <string>
#include "CALUMI_Math.h"

namespace CALUMI {
	namespace UNIV {

#pragma region ANIMATION
		struct Animation::Impl
		{
			Utilities::StringContainer animationTitle = "NO TITLE";
			Utilities::VectorContainer<AnimationBlock> animationBlocks;
			
			Impl() = default;
		};
		Animation::Animation() { pImpl = new Impl; }
		Utilities::VectorContainer<AnimationBlock>& Animation::AnimationBlocks() const
		{
			return pImpl->animationBlocks;
		}
		const char* Animation::AnimationTitle() const
		{
			return pImpl->animationTitle.c_str();
		}
		void Animation::AnimationTitle(const char* title)
		{
			pImpl->animationTitle = title;
		}
		void Animation::AnimationTitle(const Utilities::StringContainer& title)
		{
			pImpl->animationTitle = title;
		}
		UNIV::Animation::Animation(const Utilities::StringContainer& title, unsigned int initialBlockCount = 8) : Animation()
		{
			pImpl->animationTitle = title;
			//boneCount = initialBoneCount;
			pImpl->animationBlocks.reserve(initialBlockCount);
		}
		UNIV::Animation::Animation(const Animation& input) : Animation()
		{
			*pImpl = *(input.pImpl);
		}
		Animation::~Animation() { if (pImpl) delete pImpl; }
		Animation& Animation::operator=(const Animation& input) { *pImpl = *(input.pImpl); return *this; }

		bool UNIV::Animation::AddAnimationBlock(AnimationBlock& blockToAdd, bool overwrite)
		{
			for (unsigned int i = 0; i < pImpl->animationBlocks.size(); i ++)
			{
				if (pImpl->animationBlocks.at(i).BoneIndex() == blockToAdd.BoneIndex())
				{
					if (!overwrite) return false;
					else 
					{
						pImpl->animationBlocks.at(i) = blockToAdd;
						return true;
					}
				}
			}

			pImpl->animationBlocks.push_back(blockToAdd);
			return true;
		}

		//inline CALUMIANIMATION_API UNIV::AnimationBlock GetAnimationBlock(unsigned int i) const { return animationBlocks.at(i); }
		//inline CALUMIANIMATION_API std::vector<UNIV::AnimationBlock> GetAnimationBlockVector() const { return animationBlocks; }
		void Animation::ClearAnimationBlocks() { pImpl->animationBlocks.clear(); }

		size_t Animation::GetAnimationBlockCount() const { return pImpl->animationBlocks.size(); }

		unsigned int UNIV::Animation::GetFrameCount()
		{
			unsigned int output = 0;
			for (unsigned int i = 0; i < pImpl->animationBlocks.size(); i++)
			{
				unsigned int temp = pImpl->animationBlocks.at(i).GetLastFrameInBlock();
				if (temp > output)
				{
					output = temp;
				}
			}
			output++; //adding one to sum up total frames in animation. If final frame is 50, the count is 51
			return output;
		}
		Utilities::StringContainer Animation::ToJSON(const size_t indents) const
		{
			Utilities::StringContainer output = Utilities::Indent(indents).c_str();
			output += "{\n";
			output += std::format("{0}\"animationTitle\":\"{1}\",\n{0}\"animationBlocks\":", Utilities::Indent(indents + 1).c_str(), pImpl->animationTitle.c_str() /*, boneCount*/).c_str(); //    \n{0}\"boneCount\":{2},
			output += Utilities::VectorToJSON(pImpl->animationBlocks, indents + 1);
			output += "\n";
			output += Utilities::Indent(indents).c_str();
			output += "}";
			return output;
		}

#pragma endregion

#pragma region ANIMATIONBLOCK

		struct AnimationBlock::Impl 
		{
			int boneIndex = -2;
			Utilities::StringContainer boneName = "UNNAMED";
			Utilities::VectorContainer<CALUMI::UNIV::Rotation> rotationSequence;
			Utilities::VectorContainer<CALUMI::UNIV::Translation> translationSequence;
			Utilities::VectorContainer<CALUMI::UNIV::Scalar> scalarSequence;
			Utilities::VectorContainer<CALUMI::UNIV::Priority> prioritySequence;
			Impl() = default;
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

			for (size_t i = 0; i < pImpl->rotationSequence.size(); i++)
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

		size_t AnimationBlock::GetRotationEntryCount() const { return pImpl->rotationSequence.size(); }

		bool AnimationBlock::CheckEmptyRotationSequence()
		{
			Math::Quaternion defaultQuaternion;
			size_t ndCount = 0;

			for (size_t i = 0; i < pImpl->rotationSequence.size(); i++)
			{
				if (!pImpl->rotationSequence.at(i).RotationQuaternion().AreEqual(defaultQuaternion,0.0f))
				{
					ndCount++;
				}
			}

			if (ndCount == 0)
			{
				ClearRotationEntries();
				return true;
			}

			return false;
		}

		Utilities::VectorContainer<CALUMI::UNIV::Translation>& AnimationBlock::TranslationSequence() const
		{
			return pImpl->translationSequence;
		}

		bool AnimationBlock::AddTranslationEntry(CALUMI::UNIV::Translation& input, bool overwrite)
		{
			for (size_t i = 0; i < pImpl->translationSequence.size(); i++)
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

		size_t AnimationBlock::GetTranslationEntryCount() const { return pImpl->translationSequence.size(); }

		bool AnimationBlock::CheckEmptyTranslationSequence()
		{
			Math::Vector3D defaultVector;
			size_t ndCount = 0;

			for (size_t i = 0; i < pImpl->translationSequence.size(); i++)
			{
				if (pImpl->translationSequence.at(i).TranslationVector() != defaultVector)
				{
					ndCount++;
				}
			}

			if (ndCount == 0)
			{
				ClearTranslationEntries();
				return true;
			}

			return false;
		}

		Utilities::VectorContainer<CALUMI::UNIV::Scalar>& AnimationBlock::ScalarSequence() const
		{
			return pImpl->scalarSequence;
		}

		bool AnimationBlock::AddScalarEntry(CALUMI::UNIV::Scalar& input, bool overwrite)
		{
			for (size_t i = 0; i < pImpl->scalarSequence.size(); i++)
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

		size_t AnimationBlock::GetScalarEntryCount() const { return pImpl->scalarSequence.size(); }

		bool AnimationBlock::CheckEmptyScalarSequence()
		{
			float defaultScalar = 1.0f;
			size_t ndCount = 0;

			for (size_t i = 0; i < pImpl->scalarSequence.size(); i++)
			{
				if (pImpl->scalarSequence.at(i).ScalarValue() != defaultScalar)
				{
					ndCount++;
				}
			}

			if (ndCount == 0)
			{
				ClearScalarEntries();
				return true;
			}

			return false;
		}

		Utilities::VectorContainer<CALUMI::UNIV::Priority>& AnimationBlock::PrioritySequence()
		{
			return pImpl->prioritySequence;
		}

		bool AnimationBlock::AddPriorityEntry(CALUMI::UNIV::Priority& input, bool overwrite)
		{
			for (size_t i = 0; i < pImpl->prioritySequence.size(); i++)
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

		size_t AnimationBlock::GetPriorityEntryCount() const { return pImpl->prioritySequence.size(); }

		Utilities::StringContainer AnimationBlock::ToJSON(const size_t indents) const {
			Utilities::StringContainer output = Utilities::Indent(indents).c_str();
			output += "{\n";
			output += std::format("{0}\"boneName\":\"{1}\",\n{0}\"boneIndex\":{2},\n", Utilities::Indent(indents + 1).c_str(), pImpl->boneName.c_str(), pImpl->boneIndex).c_str();
			output += std::format("{0}\"rotationSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->rotationSequence, indents + 1).c_str()).c_str();
			output += std::format("{0}\"translationSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->translationSequence, indents + 1).c_str()).c_str();
			output += std::format("{0}\"scalarSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->scalarSequence, indents + 1).c_str()).c_str();
			output += std::format("{0}\"prioritySequence\":{1}\n{2}", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(pImpl->prioritySequence, indents + 1).c_str(), Utilities::Indent(indents).c_str()).c_str();
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
			errorMessageHolder->Clear();

			if (source->AnimationBlocks().size() <= index || index < 0)
			{
				*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->AnimationBlocks().at(index);
		}
		size_t GetAnimationBlockCountC(Animation* source)
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
		size_t GetFrameCountC(Animation* source)
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
			errorMessageHolder->Clear();

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
			errorMessageHolder->Clear();

			if (boneName == "")
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
			errorMessageHolder->Clear();

			if (index >= source->RotationSequence().size() || index < 0)
			{
				*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->RotationSequence().at(index);
		}
		size_t GetRotationSqSizeC(AnimationBlock* source)
		{
			return source->RotationSequence().size();
		}
		bool CheckEmptyRotationSqC(AnimationBlock* source)
		{
			if(!source) return false;

			return source->CheckEmptyRotationSequence();
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
			errorMessageHolder->Clear();

			if (index >= source->TranslationSequence().size() || index < 0)
			{
				*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->TranslationSequence().at(index);
		}
		size_t GetTranslationSqSizeC(AnimationBlock* source)
		{
			return source->TranslationSequence().size();
		}
		bool CheckEmptyTranslationSqC(AnimationBlock* source)
		{
			if (!source) return false;

			return source->CheckEmptyTranslationSequence();
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
			errorMessageHolder->Clear();

			if (index >= source->ScalarSequence().size() || index < 0)
			{
				*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->ScalarSequence().at(index);
		}
		size_t GetScalarSqSizeC(AnimationBlock* source)
		{
			return source->ScalarSequence().size();
		}
		bool CheckEmptyScalarSqC(AnimationBlock* source)
		{
			if(!source) return false;

			return source->CheckEmptyScalarSequence();
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
			errorMessageHolder->Clear();

			if (index >= source->PrioritySequence().size() || index < 0)
			{
				*errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->PrioritySequence().at(index);
		}
		size_t GetPrioritySqSizeC(AnimationBlock* source)
		{
			return source->PrioritySequence().size();
		}
#pragma endregion

	}
	
}
