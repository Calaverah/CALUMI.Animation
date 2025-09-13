//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_Math.h"
#include "CALUMI_BoneTypes.h"
#include <cstdint>
#include <expected>
#include <format>
#include <set>
#include <string>
#include <vector>

namespace CALUMI{ namespace UNIV{

	struct CALUMIANIMATION_API SkeletonBone
	{

	private:
		BoneTypeProperties* boneTypeProperties = new DefaultBoneProperties;

		//We keep this private as it is not the preferred way to get this value.
		//It should only be used on serialization functions that are constant where default resetting is not possible
		const char* _GetBoneTypeString() const;

	public:
		CALUMI::Math::Quaternion localRotation;
		CALUMI::Math::Quaternion rootRotation;
		CALUMI::Math::Vector3 localPosition;
		CALUMI::Math::Vector3 rootPosition;

		int mirrorBoneIndex = -1;

		std::string name;

		int parentBoneIndex = -1;


		SkeletonBone() = default;

		bool SetBoneTypeProperty(UNIV::BoneType boneType, bool reset = false);
		const BoneTypeProperties* GetBoneTypeProperty();
		bool ResetBoneTypeProperty(UNIV::BoneType boneType = UNIV::BoneType::Default);

		std::string ToJSON(int indents) const;
	};

	struct CALUMIANIMATION_API SkeletonRig
	{
		/// <summary>
		/// Optional name for your skeleton rig.
		/// </summary>
		std::string rigName = "MySkeletonRig";
		std::vector<SkeletonBone> boneEntries;

		SkeletonRig() = default;
		SkeletonRig(std::string rigName);
		std::expected< bool, std::string> ValidateNames();
		std::expected< bool, std::string> ValidateParentIndices();

		/// <summary>
		/// The proper way to add bones to a universal rig definition.
		/// </summary>
		/// <param name="rotation">Relative Rotation as Quaternion</param>
		/// <param name="position">Relative Position as Vector3 (floats)</param>
		/// <param name="boneName">Must be unique string</param>
		/// <param name="parentName">If a parent name is not found, the parent will default to the root bone</param>
		/// <param name="localValues">Enter false for values to be read as relative to root</param>
		/// <returns></returns>
		bool AddBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, std::string boneName, std::string parentName, bool localValues = true);

		/// <summary>
		/// The proper way to add bones to a universal rig definition.
		/// </summary>
		/// <param name="rotation">		Relative Rotation as Quaternion</param>
		/// <param name="position">		Relative Position as Vector3 (floats)</param>
		/// <param name="boneName">		Must be unique string</param>
		/// <param name="parentIndex">	If a parent name is not found, the parent will default to the root bone</param>
		/// <param name="localValues">	Enter false for values to be read as relative to root</param>
		/// <returns></returns>
		bool AddBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, std::string boneName, int parentIndex, bool localValues = true);

		/// <summary>
		/// Returns true if both indices are within the boneEntries current list
		/// </summary>
		/// <param name="i1">Index 1</param>
		/// <param name="i2">Index 2</param>
		/// <returns></returns>
		bool CreateBoneMirrorPair(int i1, int i2);

		bool ResetAllBoneMirrors();
		/// <summary>
		/// Returns true if each bone mirror index is exclusive to the bone mirror index pointing at it.
		/// True: Bone1 mirrors Bone2 and Bone2 mirrors Bone1
		/// False: Bone1 mirrors Bone2, Bone2 mirrors Bone3, etc.
		/// </summary>
		/// <returns></returns>
		bool VerifyExclusiveBoneMirrors();

		unsigned int GetAnimatedBoneCount();
		unsigned int GetBoneCount();
		

		std::string ToJSON(int indents) const;
		static const unsigned int MaxBoneCount = 512;
	};

	extern  "C" {
		CALUMIANIMATION_API SkeletonRig* CreateSkeletonRigC(const char* rigName);
		CALUMIANIMATION_API bool DeleteSkeletonRigC(SkeletonRig* ptr);
		CALUMIANIMATION_API bool AddBoneToSkeletonRigC(
			SkeletonRig* rig,
			float rotationX, float rotationY, float rotationZ, float rotationW,
			float positionX, float positionY, float positionZ,
			const char* boneName,
			int parentIndex,
			bool usingLocalValues,
			const char* errorMessage
		);

		//Basic Bone Type Property Setters and Getters. No data is filled here
		//returns true if set is successful, if there is an error, bonetype will remain the same
		//Simply set a bone type to Default if you would like to reset it, as there are no default values currently
		CALUMIANIMATION_API bool SetBoneTypeC(SkeletonBone* bone, uint32_t boneType);
		CALUMIANIMATION_API bool SetBoneTypeFromStringC(SkeletonBone* bone, const char* boneStr);
		CALUMIANIMATION_API uint32_t GetBoneTypeC(SkeletonBone* bone);
		CALUMIANIMATION_API const char* GetBoneTypeAsStringC(SkeletonBone* bone);

		//Setting Bone Property Values, if reassign is set to false, then the value will return false if the bone type does not match the desired input values and it will be skipped
		//If reassign is set to true, the the bone will be set the desired type as well it's values filled
		CALUMIANIMATION_API bool SetTwistBonePropertiesC(SkeletonBone* bone, bool reassign, int32_t twistDriverIndex, float twistDriverWeight, const char* errorMessage);
		CALUMIANIMATION_API int GetTwistBoneDriverIndexC(SkeletonBone* bone, const char* errorMessage);
		CALUMIANIMATION_API float GetTwistBoneDriverWeightC(SkeletonBone* bone, const char* errorMessage);

		//Mirror setting, resetting, and getting
		CALUMIANIMATION_API int SetMirrorIndexC(SkeletonBone* bone, int index);
		CALUMIANIMATION_API int GetMirrorIndexC(SkeletonBone* bone);
		CALUMIANIMATION_API bool CreateBoneMirrorPairC(SkeletonRig* rig, int index1, int index2);
		CALUMIANIMATION_API bool ResetAllBoneMirrorsC(SkeletonRig* rig);
		CALUMIANIMATION_API bool VerifyExclusiveBoneMirrorsC(SkeletonRig* rig);

		//Bone Count Getters
		CALUMIANIMATION_API unsigned int GetSkeletonRigBoneCountC(SkeletonRig* source);
		CALUMIANIMATION_API unsigned int GetSkeletonRigAnimatedBoneCountC(SkeletonRig* source);

		CALUMIANIMATION_API const char* GetSkeletonRigNameC(SkeletonRig* source);
		CALUMIANIMATION_API SkeletonBone* GetSkeletonBoneC(SkeletonRig* source, int index, const char* errorMessage);
		CALUMIANIMATION_API const char* GetSkeletonBoneNameC(SkeletonBone* source);
		CALUMIANIMATION_API int GetSkeletonBoneParentIndexC(SkeletonBone* source);
		CALUMIANIMATION_API CALUMI::Math::Quaternion* GetSkeletonBoneRotationC(SkeletonBone* source, bool fromRoot);
		CALUMIANIMATION_API CALUMI::Math::Vector3* GetSkeletonBoneTranslationC(SkeletonBone* source, bool fromRoot);
		CALUMIANIMATION_API bool ValidateSkeletonRigNamesC(SkeletonRig* source, const char* errorMessage);
		CALUMIANIMATION_API bool ValidateSkeletonRigParentIndicesC(SkeletonRig* source, const char* errorMessage);
	}

}}
