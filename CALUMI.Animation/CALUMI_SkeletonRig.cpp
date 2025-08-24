//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_SkeletonRig.h"

namespace CALUMI{ namespace UNIV{
    SkeletonRig::SkeletonRig(std::string rigName)
    {
        this->rigName = rigName;
    }

    std::expected< bool, std::string> SkeletonRig::ValidateNames()
    {
        
        if (boneEntries.size() == 0)
        {
            return std::unexpected("No Bone Entries For This Rig Have Been Found");
        }

        std::set<std::string> uniqueNames;
        for (SkeletonBone entry : boneEntries)
        {
            auto result = uniqueNames.insert(entry.name);
            if (!result.second)
            {
                return std::unexpected("Two Bone Entries Share The Same Name In This Rig"); //Duplicate name found
            }
        }

        return true;
    }

    std::expected< bool, std::string> SkeletonRig::ValidateParentIndices()
    {
        if (boneEntries.size() < 2)
        {
            return std::unexpected("Not Enough Bone Entries To Validate Parent Entries");
        }

        for (int i = 0; i < boneEntries.size(); i++)
        {
            if (boneEntries.at(i).parentBoneIndex >= i)
            {
                return std::unexpected(std::format("Bone Index: {} Has Parent Index: {}. Parent Index Cannot Be Greater Than Or Equal To Bone's Index", i, boneEntries.at(i).parentBoneIndex));
            }
        }
        return true;
    }
    bool SkeletonRig::AddBoneToRig(DirectX::SimpleMath::Quaternion rotation, DirectX::SimpleMath::Vector3 position, std::string boneName, std::string parentName, bool localValues)
    {
        int newParentIndex = 0;
        if (boneEntries.size() == 0)
        {
            newParentIndex = -1;
        }
        else
        {
            for (unsigned int i = 0; i < boneEntries.size(); i++)
            {
                if (boneEntries.at(i).name.compare(parentName) == 0)
                {
                    newParentIndex = i;
                    break;
                }
            }
        }
        return AddBoneToRig(rotation,position,boneName,newParentIndex, localValues);
    }

    bool SkeletonRig::AddBoneToRig(DirectX::SimpleMath::Quaternion rotation, DirectX::SimpleMath::Vector3 position, std::string boneName, int parentIndex, bool localValues)
    {
        SkeletonBone input;
        input.name = boneName;
        input.parentBoneIndex = parentIndex;
        //if (boneEntries.empty())
        //{
        //    input.parentBoneIndex = -1; //If this is the first entry, we set parent to -1 as none exists
        //}

        //if (input.parentBoneIndex >= boneEntries.size())
        //{
        //    input.parentBoneIndex = 0; //Cannot have a parent with the same index as child, default to root
        //}

        DirectX::SimpleMath::Quaternion rootRotation;
        DirectX::SimpleMath::Quaternion localRotation;
        DirectX::SimpleMath::Vector3 rootPosition;
        DirectX::SimpleMath::Vector3 localPosition;
        if (boneEntries.empty())
        {
            rootRotation = rotation;
            localRotation = rotation;
            rootPosition = position;
            localPosition = position;
        }
        else if (localValues)
        {
            rootRotation = rotation * boneEntries.at(input.parentBoneIndex).rootRotation;
            localRotation = rotation;
            rootPosition = position + boneEntries.at(input.parentBoneIndex).rootPosition;
            localPosition = position;
        }
        else
        {
            DirectX::SimpleMath::Quaternion inverseParentRoot;
            boneEntries.at(input.parentBoneIndex).rootRotation.Inverse(inverseParentRoot); //why did they not make this a return value.
            localRotation = rotation * inverseParentRoot;
            rootRotation = rotation;
            localPosition = position - boneEntries.at(input.parentBoneIndex).rootPosition;
            rootPosition = position;
        }

        
        
        input.localPosition = localPosition;
        input.rootPosition = rootPosition;
        input.localRotation = localRotation;
        input.rootRotation = rootRotation;

        boneEntries.push_back(input);

        return true;
    };

}}

