//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_Math.h"
#include <cstdint>
#include <expected>
#include <format>
#include <set>
#include <string>
#include <vector>

namespace CALUMI{ namespace UNIV{

	enum class BoneType : uint32_t
	{
		Default = 0,
		Twist	= 1
	};

	bool CALUMIANIMATION_API BoneTypeExists(uint32_t input);
	bool CALUMIANIMATION_API BoneTypeExists(std::string boneTypeStr);
	std::string CALUMIANIMATION_API BoneTypeToString(UNIV::BoneType boneType);
	UNIV::BoneType CALUMIANIMATION_API BoneTypeFromString(std::string boneTypeStr);

	struct CALUMIANIMATION_API SkeletonBone
	{
		CALUMI::Math::Quaternion localRotation;
		CALUMI::Math::Quaternion rootRotation;
		CALUMI::Math::Vector3 localPosition;
		CALUMI::Math::Vector3 rootPosition;
		UNIV::BoneType boneType = UNIV::BoneType::Default;

		std::string name;

		int parentBoneIndex = -1;


		SkeletonBone() = default;

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

		std::string ToJSON(int indents) const;
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

		//returns true if set is successful, if there is an error, bonetype will remain the same
		CALUMIANIMATION_API bool SetBoneTypeC(SkeletonBone* bone, uint32_t boneType);
		CALUMIANIMATION_API bool SetBoneTypeFromStringC(SkeletonBone* bone, const char* boneStr);
		CALUMIANIMATION_API uint32_t GetBoneTypeC(SkeletonBone* bone);
		CALUMIANIMATION_API const char* GetBoneTypeAsStringC(SkeletonBone* bone);

		CALUMIANIMATION_API size_t GetSkeletonRigBoneCountC(SkeletonRig* source);
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
