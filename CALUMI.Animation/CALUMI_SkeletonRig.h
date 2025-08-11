#pragma once
#ifdef CALUMIANIMATION_EXPORTS // This macro is typically defined by Visual Studio for DLL projects
#define CALUMIANIMATION_API __declspec(dllexport)
#else
#define CALUMIANIMATION_API __declspec(dllimport)
#endif

#include <vector>
#include <string>
#include <cstdint>
#include <expected>
#include <set>
#include <SimpleMath.h>
#include <format>

namespace CALUMI{ namespace UNIV{


	struct CALUMIANIMATION_API SkeletonBone
	{
		DirectX::SimpleMath::Quaternion localRotation;
		DirectX::SimpleMath::Quaternion rootRotation;
		DirectX::SimpleMath::Vector3 localPosition;
		DirectX::SimpleMath::Vector3 rootPosition;

		std::string name;

		int parentBoneIndex = -1;


		SkeletonBone() = default;
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
		bool AddBoneToRig(DirectX::SimpleMath::Quaternion rotation, DirectX::SimpleMath::Vector3 position, std::string boneName, std::string parentName, bool localValues = true);

		/// <summary>
		/// The proper way to add bones to a universal rig definition.
		/// </summary>
		/// <param name="rotation">		Relative Rotation as Quaternion</param>
		/// <param name="position">		Relative Position as Vector3 (floats)</param>
		/// <param name="boneName">		Must be unique string</param>
		/// <param name="parentIndex">	If a parent name is not found, the parent will default to the root bone</param>
		/// <param name="localValues">	Enter false for values to be read as relative to root</param>
		/// <returns></returns>
		bool AddBoneToRig(DirectX::SimpleMath::Quaternion rotation, DirectX::SimpleMath::Vector3 position, std::string boneName, int parentIndex, bool localValues = true);
	};




}}
