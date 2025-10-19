//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_BoneTypes.h"
#include "CALUMI_Math.h"
#include "CALUMI_Utilities.h"


namespace CALUMI{ namespace UNIV{

	/// <summary>
	/// Rig Packages are meant to be defined by each respective game/file type. Ideally, in the future, a rig should be able to be exported to multiple formats at the same time. If a universal rig definition has these game/format specific pieces of data ready, then the conversion may be much simpler 
	/// </summary>
	struct CALUMIANIMATION_API RigPackage
	{
		virtual const char* GetPackageType() const = 0;
		virtual bool HandleBoneRename(const char* oldBone, const char* newName, size_t idx) = 0;
		virtual RigPackage* Clone() = 0;
		virtual Utilities::StringContainer ToJSON(size_t indents) const = 0;

		RigPackage() = default;
		virtual ~RigPackage();
	};

	struct CALUMIANIMATION_API RigPackageManager
	{
	private:
		Utilities::VectorContainer<RigPackage*> packages;
	public:

		RigPackageManager() = default;

		RigPackage* GetPackage(const char* packageName);
		bool RemovePackage(const char* packageName);
		bool AddPackage(RigPackage* package, bool overwrite = true);

		Utilities::StringContainer ToJSON(size_t indents) const;
		bool HandleBoneRename(const char* oldBone, const char* newName, size_t idx);
		RigPackageManager& operator=(const RigPackageManager& other);
	};

	struct CALUMIANIMATION_API SkeletonBone
	{

	private:
		BoneTypeProperties* boneTypeProperties = new DefaultBoneProperties;
		int mirrorBoneIndex = -1;
		int parentBoneIndex = -1;
		//We keep this private as it is not the preferred way to get this value.
		//It should only be used on serialization functions that are constant where default resetting is not possible
		const char* _GetBoneTypeString() const;

	public:
		CALUMI::Math::Quaternion localRotation;
		CALUMI::Math::Quaternion globalRotation;
		CALUMI::Math::Vector3 localPosition;
		CALUMI::Math::Vector3 globalPosition;

		

		Utilities::StringContainer name;


		SkeletonBone() = default;
		SkeletonBone(const SkeletonBone& other);
		SkeletonBone(const SkeletonBone&& other) noexcept;
		~SkeletonBone();

		bool SetBoneTypeProperty(UNIV::BoneType boneType, bool resetExisting = false);
		const BoneTypeProperties* GetBoneTypeProperty();
		bool ResetBoneTypeProperty(UNIV::BoneType boneType = UNIV::BoneType::Default);

		void SetMirrorBoneIndex(int idx);
		void ResetMirrorBoneIndex();
		int GetMirrorBoneIndex() const;

		void SetParentBoneIndex(int idx);
		void ResetParentBoneIndex();
		void SetAsRootBone();
		int GetParentBoneIndex() const;

		Utilities::StringContainer ToJSON(size_t indents) const;

		SkeletonBone& operator=(const SkeletonBone& other);
	};

	struct CALUMIANIMATION_API SkeletonRig
	{
		/// <summary>
		/// Optional name for your skeleton rig.
		/// </summary>
		Utilities::StringContainer rigName = "MySkeletonRig";
		Utilities::VectorContainer<SkeletonBone> boneEntries;

		RigPackageManager rigPackageManager;

		SkeletonRig() = default;
		SkeletonRig(const char* rigName);
		SkeletonRig(Utilities::StringContainer& rigName);
		Utilities::ExpectedContainer< bool, Utilities::StringContainer> ValidateNames() const;
		Utilities::ExpectedContainer< bool, Utilities::StringContainer> ValidateParentIndices();

		/// <summary>
		/// The proper way to add bones to a universal rig definition.
		/// </summary>
		/// <param name="rotation">Relative Rotation as Quaternion</param>
		/// <param name="position">Relative Position as Vector3 (floats)</param>
		/// <param name="boneName">Must be unique string</param>
		/// <param name="parentName">If a parent name is not found, the parent will default to the root bone</param>
		/// <param name="localValues">Enter false for values to be read as relative to root</param>
		/// <returns></returns>
		bool AddBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, Utilities::StringContainer boneName, Utilities::StringContainer parentName, bool localValues = true);

		/// <summary>
		/// The proper way to add bones to a universal rig definition.
		/// </summary>
		/// <param name="rotation">		Relative Rotation as Quaternion</param>
		/// <param name="position">		Relative Position as Vector3 (floats)</param>
		/// <param name="boneName">		Must be unique string</param>
		/// <param name="parentIndex">	If a parent name is not found, the parent will default to the root bone</param>
		/// <param name="localValues">	Enter false for values to be read as relative to root</param>
		/// <returns></returns>
		bool AddBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, Utilities::StringContainer boneName, int parentIndex, bool localValues = true);

		bool RenameBone(const char* oldBoneName, const char* newBoneName);
		bool RenameBone(size_t boneIndex, const char* newBoneName);

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

		size_t GetAnimatedBoneCount();
		size_t GetBoneCount() const;
		
		Utilities::ExpectedContainer<size_t, Utilities::StringContainer> GetBoneIndex(Utilities::StringContainer boneName);

		Utilities::StringContainer ToJSON(size_t indents) const;
		static const unsigned int MaxBoneCount = 512;

		SkeletonRig& operator=(const SkeletonRig& other);
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
			Utilities::StringContainer* errorMessage
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
		CALUMIANIMATION_API bool SetTwistBonePropertiesC(SkeletonBone* bone, bool reassign, int32_t twistDriverIndex, float twistDriverWeight, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API int GetTwistBoneDriverIndexC(SkeletonBone* bone, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API float GetTwistBoneDriverWeightC(SkeletonBone* bone, Utilities::StringContainer* errorMessage);

		//Mirror setting, resetting, and getting
		CALUMIANIMATION_API int SetMirrorIndexC(SkeletonBone* bone, int index);
		CALUMIANIMATION_API int GetMirrorIndexC(SkeletonBone* bone);
		CALUMIANIMATION_API bool CreateBoneMirrorPairC(SkeletonRig* rig, int index1, int index2);
		CALUMIANIMATION_API bool ResetAllBoneMirrorsC(SkeletonRig* rig);
		CALUMIANIMATION_API bool VerifyExclusiveBoneMirrorsC(SkeletonRig* rig);

		//Bone Count Getters
		CALUMIANIMATION_API size_t GetSkeletonRigBoneCountC(SkeletonRig* source);
		CALUMIANIMATION_API size_t GetSkeletonRigAnimatedBoneCountC(SkeletonRig* source);

		CALUMIANIMATION_API const char* GetSkeletonRigNameC(SkeletonRig* source);
		CALUMIANIMATION_API SkeletonBone* GetSkeletonBoneC(SkeletonRig* source, int index, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API const char* GetSkeletonBoneNameC(SkeletonBone* source);
		CALUMIANIMATION_API int GetSkeletonBoneParentIndexC(SkeletonBone* source);
		CALUMIANIMATION_API CALUMI::Math::Quaternion* GetSkeletonBoneRotationC(SkeletonBone* source, bool global);
		CALUMIANIMATION_API CALUMI::Math::Vector3* GetSkeletonBonePositionC(SkeletonBone* source, bool global);
		CALUMIANIMATION_API bool ValidateSkeletonRigNamesC(SkeletonRig* source, Utilities::StringContainer* errorMessage);
		CALUMIANIMATION_API bool ValidateSkeletonRigParentIndicesC(SkeletonRig* source, Utilities::StringContainer* errorMessage);
	}

}}

#pragma warning(disable: 4661)
_VECTORTEMPLATE(CALUMI::UNIV::SkeletonBone);
_VECTORTEMPLATE(CALUMI::UNIV::RigPackage*);
#pragma warning(default: 4661)