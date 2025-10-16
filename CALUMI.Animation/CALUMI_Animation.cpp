//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_Animation.h"
#include "CALUMI_Utilities.h"
#include <format>
#include <string>

namespace CALUMI {
	namespace UNIV {


		UNIV::Animation::Animation(const Utilities::StringContainer& title, const int initialBoneCount, int initialBlockCount = 8)
		{
			animationTitle = title;
			boneCount = initialBoneCount;
			animationBlocks.reserve(initialBlockCount);
		}

		bool UNIV::Animation::AddAnimationBlock(AnimationBlock& blockToAdd, bool overwrite)
		{
			for (unsigned int i = 0; i < animationBlocks.size(); i ++)
			{
				if (animationBlocks.at(i).boneIndex == blockToAdd.boneIndex)
				{
					if (!overwrite) return false;
					else 
					{
						animationBlocks.at(i) = blockToAdd;
						return true;
					}
				}
			}

			animationBlocks.push_back(blockToAdd);
			return true;
		}

		//inline CALUMIANIMATION_API UNIV::AnimationBlock GetAnimationBlock(unsigned int i) const { return animationBlocks.at(i); }
		//inline CALUMIANIMATION_API std::vector<UNIV::AnimationBlock> GetAnimationBlockVector() const { return animationBlocks; }
		void Animation::ClearAnimationBlocks() { animationBlocks.clear(); }

		size_t Animation::GetAnimationBlockCount() const { return animationBlocks.size(); }

		unsigned int UNIV::Animation::GetFrameCount()
		{
			unsigned int output = 0;
			for (unsigned int i = 0; i < animationBlocks.size(); i++)
			{
				unsigned int temp = animationBlocks.at(i).GetLastFrameInBlock();
				if (temp > output)
				{
					output = temp;
				}
			}
			output++; //adding one to sum up total frames in animation. If final frame is 50, the count is 51
			return output;
		}

		unsigned int UNIV::AnimationBlock::GetLastFrameInBlock()
		{
			unsigned int output = 0;
			if (_rotationSequence.size() > 0) {
				if (_rotationSequence.at(_rotationSequence.size()-1).frame > output)
				{
					output = _rotationSequence.at(_rotationSequence.size()-1).frame;
				}
			}
			if (_translationSequence.size() > 0) {
				if (_translationSequence.at(_translationSequence.size()-1).frame > output)
				{
					output = _translationSequence.at(_translationSequence.size() - 1).frame;
				}
			}
			if (_scalarSequence.size() > 0) {
				if (_scalarSequence.at(_scalarSequence.size()-1).frame > output)
				{
					output = _scalarSequence.at(_scalarSequence.size() - 1).frame;
				}
			}
			if (_prioritySequence.size() > 0) {
				if (_prioritySequence.at(_prioritySequence.size()-1).frame > output)
				{
					output = _prioritySequence.at(_prioritySequence.size() - 1).frame;
				}
			}

			return output;
		}

		bool AnimationBlock::AddRotationEntry(CALUMI::UNIV::Rotation& input, bool overwrite)
		{

			for (size_t i = 0; i < _rotationSequence.size(); i++)
			{
				if (_rotationSequence.at(i).frame == input.frame)
				{
					if (!overwrite) return false;
					else
					{
						_rotationSequence.at(i) = input;
						return true;
					}
				}
				else if (_rotationSequence.at(i).frame > input.frame)
				{
					_rotationSequence.insert_r(i, input);
					return true;
				}
			}
			
			_rotationSequence.push_back(input);
			return true;
		}

		bool AnimationBlock::RemoveRotationEntry(unsigned int frame)
		{
			for (unsigned int i = 0; i < _rotationSequence.size(); i++)
			{
				if (frame == _rotationSequence.at(i).frame)
				{
					_rotationSequence.erase(i);
					return true;
				}
			}

			return false;
		}

		void AnimationBlock::ClearRotationEntries() { _rotationSequence.clear(); }

		size_t AnimationBlock::GetRotationEntryCount() const { return _rotationSequence.size(); }

		bool AnimationBlock::AddTranslationEntry(CALUMI::UNIV::Translation& input, bool overwrite)
		{
			for (size_t i = 0; i < _translationSequence.size(); i++)
			{
				if (_translationSequence.at(i).frame == input.frame)
				{
					if (!overwrite) return false;
					else
					{
						_translationSequence.at(i) = input;
						return true;
					}
				}
				else if (_translationSequence.at(i).frame > input.frame)
				{
					_translationSequence.insert_r(i, input);
					return true;
				}
			}

			_translationSequence.push_back(input);
			return true;
		}

		bool AnimationBlock::RemoveTranslationEntry(unsigned int frame)
		{
			for (unsigned int i = 0; i < _translationSequence.size(); i++)
			{
				if (frame == _translationSequence.at(i).frame)
				{
					_translationSequence.erase(i);
					return true;
				}
			}

			return false;
		}

		void AnimationBlock::ClearTranslationEntries() { _translationSequence.clear(); }

		size_t AnimationBlock::GetTranslationEntryCount() const { return _translationSequence.size(); }

		bool AnimationBlock::AddScalarEntry(CALUMI::UNIV::Scalar& input, bool overwrite)
		{
			for (size_t i = 0; i < _scalarSequence.size(); i++)
			{
				if (_scalarSequence.at(i).frame == input.frame)
				{
					if (!overwrite) return false;
					else
					{
						_scalarSequence.at(i) = input;
						return true;
					}
				}
				else if (_scalarSequence.at(i).frame > input.frame)
				{
					_scalarSequence.insert_r(i, input);
					return true;
				}
			}

			_scalarSequence.push_back(input);
			return true;
		}

		bool AnimationBlock::RemoveScalarEntry(unsigned int frame)
		{
			for (unsigned int i = 0; i < _scalarSequence.size(); i++)
			{
				if (frame == _scalarSequence.at(i).frame)
				{
					_scalarSequence.erase(i);
					return true;
				}
			}

			return false;
		}

		void AnimationBlock::ClearScalarEntries() { _scalarSequence.clear(); }

		size_t AnimationBlock::GetScalarEntryCount() const { return _scalarSequence.size(); }

		bool AnimationBlock::AddPriorityEntry(CALUMI::UNIV::Priority& input, bool overwrite)
		{
			for (size_t i = 0; i < _prioritySequence.size(); i++)
			{
				if (_prioritySequence.at(i).frame == input.frame)
				{
					if (!overwrite) return false;
					else
					{
						_prioritySequence.at(i) = input;
						return true;
					}
				}
				else if (_prioritySequence.at(i).frame > input.frame)
				{
					_prioritySequence.insert_r(i, input);
					return true;
				}
			}

			_prioritySequence.push_back(input);
			return true;
		}

		bool AnimationBlock::RemovePriorityEntry(unsigned int frame)
		{
			for (unsigned int i = 0; i < _prioritySequence.size(); i++)
			{
				if (frame == _prioritySequence.at(i).frame)
				{
					_prioritySequence.erase(i);
					return true;
				}
			}

			return false;
		}

		void AnimationBlock::ClearPriorityEntries() { _prioritySequence.clear(); }

		size_t AnimationBlock::GetPriorityEntryCount() const { return _prioritySequence.size(); }

		UNIV::AnimationBlock& AnimationBlock::operator=(const AnimationBlock& other)
		{
			//skip reassignment functions
			if (this == &other) return *this;

			boneIndex = other.boneIndex;
			boneName = other.boneName;
			_rotationSequence = other._rotationSequence;
			_translationSequence = other._translationSequence;
			_scalarSequence = other._scalarSequence;
			_prioritySequence = other._prioritySequence;

			return *this;
		}

		Utilities::StringContainer Animation::ToJSON(const size_t indents) const
		{
			Utilities::StringContainer output = Utilities::Indent(indents).c_str();
			output += "{\n";
			output += std::format("{0}\"animationTitle\":\"{1}\",\n{0}\"boneCount\":{2},\n{0}\"animationBlocks\":", Utilities::Indent(indents + 1).c_str(), animationTitle.c_str(), boneCount).c_str();
			output += Utilities::VectorToJSON(animationBlocks, indents + 1);
			output += "\n";
			output += Utilities::Indent(indents).c_str();
			output += "}";
			return output;
		}

		Utilities::StringContainer AnimationBlock::ToJSON(const size_t indents) const {
			Utilities::StringContainer output = Utilities::Indent(indents).c_str(); 
			output += "{\n";
			output += std::format("{0}\"boneName\":\"{1}\",\n{0}\"boneIndex\":{2},\n", Utilities::Indent(indents + 1).c_str(), boneName.c_str(), boneIndex).c_str();
			output += std::format("{0}\"rotationSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(_rotationSequence, indents + 1).c_str()).c_str();
			output += std::format("{0}\"translationSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(_translationSequence, indents + 1).c_str()).c_str();
			output += std::format("{0}\"scalarSequence\":{1},\n", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(_scalarSequence, indents + 1).c_str()).c_str();
			output += std::format("{0}\"prioritySequence\":{1}\n{2}", Utilities::Indent(indents + 1).c_str(), Utilities::VectorToJSON(_prioritySequence, indents + 1).c_str(), Utilities::Indent(indents).c_str()).c_str();
			output += "}";
			return output;
		}

		bool AnimationBlock::operator<(const AnimationBlock& other) const
		{
			return boneIndex < other.boneIndex;
		}
		bool AnimationBlock::operator>(const AnimationBlock& other) const
		{
			return boneIndex > other.boneIndex;
		}
		/*bool Animation::operator<(const Animation& other) const
		{
			return boneCount < other.boneCount;
		}
		bool Animation::operator>(const Animation& other) const
		{
			return boneCount > other.boneCount;
		}*/

		//--------------------------------------------------------------------------------------------------------------C FUNCTIONS

		Animation* CreateAnimationC(const char* animationTitle, int rigBoneCount)
		{
			Animation* outputAnimation = new Animation(animationTitle, rigBoneCount);
			return outputAnimation;
		}
		AnimationBlock* GetAnimationBlockC(Animation* source, int index, Utilities::StringContainer* errorMessage)
		{
			errorMessage->Clear();

			if (source->animationBlocks.size() <= index || index < 0)
			{
				*errorMessage += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->animationBlocks.at(index);
		}
		size_t GetAnimationBlockCountC(Animation* source)
		{
			return source->animationBlocks.size();
		}
		const char* GetAnimationTitleC(Animation* source)
		{
			return source->animationTitle.c_str();
		}
		size_t GetAnimationBoneCountC(Animation* source)
		{
			return source->boneCount;
		}
		size_t GetFrameCountC(Animation* source)
		{
			return source->GetFrameCount();
		}
		bool DeleteAnimationC(Animation* ptr)
		{
			if (ptr)
			{
				delete ptr;
				return true;
			}
			return false;
		}
		bool AddAnimBlockToAnimationC(Animation* anim, AnimationBlock* blockToAdd, bool overwrite, Utilities::StringContainer* errorMessage)
		{
			errorMessage->Clear();

			if (!anim->AddAnimationBlock(*blockToAdd, overwrite))
			{
				*errorMessage += "[CALUMI.Animation API] Existing Animation Block Found and Not Overwritten!";
				return false;
			}
			delete blockToAdd;
			*errorMessage += "[CALUMI.Animation API] Animation Block Copied Into Animation Successfully. Original Block Deleted!";
			return true;
		}
		AnimationBlock* CreateAnimBlockC(const char* boneName, int boneIndex, Utilities::StringContainer* errorMessage)
		{
			errorMessage->Clear();

			if (boneName == "")
			{
				*errorMessage += "[CALUMI.Animation API] Animation Block Must Have Bone Name!";
				return nullptr;
			}
			if (boneIndex < 0)
			{
				*errorMessage += "[CALUMI.Animation API] Animation Block Must Have Valid Bone Index! (ix > -1)";
				return nullptr;
			}
			AnimationBlock* outputAnimationBlock = new AnimationBlock;
			outputAnimationBlock->boneIndex = boneIndex;
			outputAnimationBlock->boneName = boneName;
			*errorMessage += "[CALUMI.Animation API] Animation Block Created!";
			return outputAnimationBlock;
		}
		bool DeleteAnimationBlockC(AnimationBlock* ptr)
		{
			if (ptr)
			{
				delete ptr;
				return true;
			}
			return false;
		}
		const char* GetAnimBlockBoneNameC(AnimationBlock* source)
		{
			return source->boneName.c_str();
		}
		int GetAnimBlockBoneIndexC(AnimationBlock* source)
		{
			return source->boneIndex;
		}
		unsigned int GetLastFrameInAnimBlockC(AnimationBlock* source)
		{
			return source->GetLastFrameInBlock();
		}
		bool AddRotationSqToAnimBlockC(AnimationBlock* block, Rotation* rotSq, int size, bool overwrite)
		{
			for (int i = 0; i < size; i++)
			{
				if(!block->AddRotationEntry((rotSq[i]),overwrite)) return false;
			}
			return true;
		}
		Rotation* GetRotationSqArrayC(AnimationBlock* source)
		{
			if(source->_rotationSequence.empty())
			return nullptr;

			return source->_rotationSequence.data();
		}
		Rotation* GetRotationFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage)
		{
			errorMessage->Clear();

			if (index >= source->_rotationSequence.size() || index < 0)
			{
				*errorMessage += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->_rotationSequence.at(index);
		}
		size_t GetRotationSqSizeC(AnimationBlock* source)
		{
			return source->_rotationSequence.size();
		}
		bool AddTranslationSqToAnimBlockC(AnimationBlock* block, Translation* trnSq, int size, bool overwrite)
		{
			for (int i = 0; i < size; i++)
			{
				if (!block->AddTranslationEntry((trnSq[i]), overwrite)) return false;
			}
			return true;
		}
		Translation* GetTranslationSqArrayC(AnimationBlock* source)
		{
			if (source->_translationSequence.empty())
				return nullptr;

			return source->_translationSequence.data();
		}
		Translation* GetTranslationFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage)
		{
			errorMessage->Clear();

			if (index >= source->_translationSequence.size() || index < 0)
			{
				*errorMessage += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->_translationSequence.at(index);
		}
		size_t GetTranslationSqSizeC(AnimationBlock* source)
		{
			return source->_translationSequence.size();
		}
		bool AddScalarSqToAnimBlockC(AnimationBlock* block, Scalar* sclrSq, int size, bool overwrite)
		{
			for (int i = 0; i < size; i++)
			{
				if (!block->AddScalarEntry((sclrSq[i]), overwrite)) return false;
			}
			return true;
		}
		Scalar* GetScalarSqArrayC(AnimationBlock* source)
		{
			if(source->_scalarSequence.empty())
			return nullptr;

			return source->_scalarSequence.data();
		}
		Scalar* GetScalarFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage)
		{
			errorMessage->Clear();

			if (index >= source->_scalarSequence.size() || index < 0)
			{
				*errorMessage += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->_scalarSequence.at(index);
		}
		size_t GetScalarSqSizeC(AnimationBlock* source)
		{
			return source->_scalarSequence.size();
		}
		bool AddPrioritySqToAnimBlockC(AnimationBlock* block, Priority* prtySq, int size, bool overwrite)
		{
			for (int i = 0; i < size; i++)
			{
				if (!block->AddPriorityEntry((prtySq[i]), overwrite)) return false;
			}
			return true;
		}
		Priority* GetPrioritySqArrayC(AnimationBlock* source)
		{
			if(source->_prioritySequence.empty())
			return nullptr;

			return source->_prioritySequence.data();
		}
		Priority* GetPriorityFromSqC(AnimationBlock* source, int index, Utilities::StringContainer* errorMessage)
		{
			errorMessage->Clear();

			if (index >= source->_prioritySequence.size() || index < 0)
			{
				*errorMessage += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
				return nullptr;
			}
			return &source->_prioritySequence.at(index);
		}
		size_t GetPrioritySqSizeC(AnimationBlock* source)
		{
			return source->_prioritySequence.size();
		}

	}
	
}
