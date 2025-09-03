//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_SkeletonRig.h"

#include "CALUMI_Utilities.h"

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
    bool SkeletonRig::AddBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, std::string boneName, std::string parentName, bool localValues)
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

    bool SkeletonRig::AddBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, std::string boneName, int parentIndex, bool localValues)
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

        CALUMI::Math::Quaternion rootRotation;
        CALUMI::Math::Quaternion localRotation;
        CALUMI::Math::Vector3 rootPosition;
        CALUMI::Math::Vector3 localPosition;
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
            CALUMI::Math::Quaternion inverseParentRoot;
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

    SkeletonRig* CreateSkeletonRigC(const char* rigName)
    {
        SkeletonRig* outputSkeletonRig = new SkeletonRig(rigName);
        return outputSkeletonRig;
    }
    bool DeleteSkeletonRigC(SkeletonRig* ptr)
    {
        if (ptr)
        {
            delete ptr;
            return true;
        }
        return false;
    }
    bool AddBoneToSkeletonRigC(SkeletonRig* rig, float rotationX, float rotationY, float rotationZ, float rotationW, float positionX, float positionY, float positionZ, const char* boneName, int parentIndex, bool usingLocalValues, const char* errorMessage)
    {
        if (boneName == "")
        {
            errorMessage = "[CALUMI.Animation API] Bone Entry Must Have Bone Name!";
            return false;
        }
        float w = rotationW;
        if (rotationX == 0.0 && rotationY == 0.0 && rotationZ == 0.0)
        {
            w = 1.0;
        }

        CALUMI::Math::Quaternion q1 = { rotationX, rotationY, rotationZ, w };
        q1.Normalize();

        bool result = rig->AddBoneToRig(q1, { positionX,positionY,positionZ }, boneName, parentIndex, usingLocalValues);
        char buffer[MAX_PATH];
        if (!result)
        {
            snprintf(buffer, sizeof(buffer), "[CALUMI.Animation API] Failure When Adding Bone: %s To Rig", boneName);
            
        }
        else
        {
            snprintf(buffer, sizeof(buffer), "[CALUMI.Animation API] %s Added To Rig Successfully!", boneName);
        }
        errorMessage = buffer;
        return result;
    }
    size_t GetSkeletonRigBoneCount(SkeletonRig* source)
    {
        return source->boneEntries.size();
    }
    const char* GetSkeletonRigName(SkeletonRig* source)
    {
        return source->rigName.c_str();
    }
    SkeletonBone* GetSkeletonBone(SkeletonRig* source, int index, const char* errorMessage)
    {
        if(source->boneEntries.size() <= index)
        {
            errorMessage = "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
            return nullptr;
        }
        if (index < 0)
        {
            errorMessage = "[CALUMI.Animation API] Index Is Negative! Is This Parent Index Coming From The Root?";
            return nullptr;
        }
        return &source->boneEntries.at(index);
    }
    const char* GetSkeletonBoneName(SkeletonBone* source)
    {
        return source->name.c_str();
    }
    int GetSkeletonBoneParentIndex(SkeletonBone* source)
    {
        return source->parentBoneIndex;
    }
    CALUMI::Math::Quaternion* GetSkeletonBoneRotation(SkeletonBone* source, bool fromRoot)
    {
        if (fromRoot)
        {
            return &source->rootRotation;
        }
        
        return &source->localRotation;
    }
    CALUMI::Math::Vector3* GetSkeletonBoneTranslation(SkeletonBone* source, bool fromRoot)
    {
        if (fromRoot)
        {
            return &source->rootPosition;
        }

        return &source->localPosition;
    }
    bool ValidateSkeletonRigNames(SkeletonRig* source, const char* errorMessage)
    {
        auto result = source->ValidateNames();
        if (!result.has_value())
        {
            errorMessage = result.error().c_str();
            return false;
        }
        return true;
    }
    bool ValidateSkeletonRigParentIndices(SkeletonRig* source, const char* errorMessage)
    {
        auto result = source->ValidateParentIndices();
        if (!result.has_value())
        {
            errorMessage = result.error().c_str();
            return false;
        }
        return true;
    }

    std::string SkeletonRig::ToJSON(const int indents = 0) const {
        std::string output = Utilities::Indent(indents) + "{\n" + Utilities::Indent(indents+1) +  "\"rigName\":\"" + rigName + "\",\n" + Utilities::Indent(indents+1) + "\"boneEntries\":";
        output += Utilities::VectorToJSON(boneEntries, indents + 1);
        output += "\n " + Utilities::Indent(indents) + "}";
        return output;
    }

    std::string SkeletonBone::ToJSON(const int indents = 0) const {
        std::string output = Utilities::Indent(indents) + "{\n" +  Utilities::Indent(indents+1) + "\"name\":\"" + name + "\",\n" + Utilities::Indent(indents+1) + "\"parentBoneIndex\":" + std::to_string(parentBoneIndex) + ",\n";
        output += Utilities::Indent(indents+1) + "\"localRotation\": [";
        output += std::to_string(localRotation.x) + ", " + std::to_string(localRotation.y) + ", " + std::to_string(localRotation.z) + ", " + std::to_string(localRotation.w) + "],\n";
        output += Utilities::Indent(indents+1) + "\"rootRotation\": [";
        output += std::to_string(rootRotation.x) + ", " + std::to_string(rootRotation.y) + ", " + std::to_string(rootRotation.z) + ", " + std::to_string(rootRotation.w) + "],\n";
        output += Utilities::Indent(indents+1) + "\"localPosition\": [";
        output += std::to_string(localPosition.x) + ", " + std::to_string(localPosition.y) + ", " + std::to_string(localPosition.z) + "],\n";
        output += Utilities::Indent(indents+1) + "\"rootPosition\": [";
        output += std::to_string(rootPosition.x) + ", " + std::to_string(rootPosition.y) + ", " + std::to_string(rootPosition.z) + "]\n";
        output += Utilities::Indent(indents) + "}";
        return output;
    }

}

}

