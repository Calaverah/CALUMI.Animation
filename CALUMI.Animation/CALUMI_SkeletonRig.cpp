//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_SkeletonRig.h"
#include <format>
#include <set>
#include <print>
#include "CALUMI_Utilities.h"
#include <string>

namespace CALUMI{ namespace UNIV{
    SkeletonRig::SkeletonRig(Utilities::StringContainer& rigName)
    {
        this->rigName = rigName;
    }

    SkeletonRig::SkeletonRig(const char* rigName)
    {
        this->rigName = rigName;
    }

    Utilities::ExpectedConatiner< bool, Utilities::StringContainer> SkeletonRig::ValidateNames() const
    {
        
        if (boneEntries.size() == 0)
        {
            Utilities::ExpectedConatiner<bool, Utilities::StringContainer> tempOutput;
            tempOutput.SetErrorValue("Two Bone Entries Share The Same Name In This Rig"); //Duplicate name found
            return tempOutput;
        }

        std::set<std::string> uniqueNames;
        for (unsigned int i = 0; i < boneEntries.size(); i++)
        {
            auto result = uniqueNames.insert(boneEntries.at(i).name.c_str());
            if (!result.second)
            {
                Utilities::ExpectedConatiner<bool, Utilities::StringContainer> tempOutput;
                tempOutput.SetErrorValue("No Bone Entries For This Rig Have Been Found");
                return tempOutput;
            }
        }

        return true;
    }

    Utilities::ExpectedConatiner< bool, Utilities::StringContainer> SkeletonRig::ValidateParentIndices()
    {
        if (boneEntries.size() < 2)
        {
            Utilities::ExpectedConatiner<bool, Utilities::StringContainer> tempOutput;
            tempOutput.SetErrorValue("Not Enough Bone Entries To Validate Parent Entries");
            return tempOutput;
        }

        for (int i = 0; i < boneEntries.size(); i++)
        {
            if (boneEntries.at(i).parentBoneIndex >= i)
            {
                Utilities::ExpectedConatiner<bool, Utilities::StringContainer> tempOutput;
                tempOutput.SetErrorValue(std::format("Bone Index: {} Has Parent Index: {}. Parent Index Cannot Be Greater Than Or Equal To Bone's Index", i, boneEntries.at(i).parentBoneIndex).c_str());
                return tempOutput;
            }
        }
        return true;
    }

    bool SkeletonRig::AddBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, Utilities::StringContainer boneName, Utilities::StringContainer parentName, bool localValues)
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

    bool SkeletonRig::AddBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, Utilities::StringContainer boneName, int parentIndex, bool localValues)
    {
        if (boneEntries.size() >= MaxBoneCount)
        {
            std::println("[CALUMI.Animation API] Bone Count Limit Reached For Rig: {}",rigName.c_str());
            return false;
        }

        SkeletonBone input;
        input.name = boneName;
        input.parentBoneIndex = parentIndex;

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
            rootRotation = rotation * boneEntries.at(input.parentBoneIndex).globalRotation;
            localRotation = rotation;
            rootPosition = position + boneEntries.at(input.parentBoneIndex).globalPosition;
            localPosition = position;
        }
        else
        {
            CALUMI::Math::Quaternion inverseParentRoot;
            boneEntries.at(input.parentBoneIndex).globalRotation.Inverse(inverseParentRoot); //why did they not make this a return value.
            localRotation = rotation * inverseParentRoot;
            rootRotation = rotation;
            localPosition = position - boneEntries.at(input.parentBoneIndex).globalPosition;
            rootPosition = position;
        }

        
        
        input.localPosition = localPosition;
        input.globalPosition = rootPosition;
        input.localRotation = localRotation;
        input.globalRotation = rootRotation;

        input.ResetMirrorBoneIndex(); //static_cast<int>(boneEntries.size());

        boneEntries.push_back(input);

        return true;
    }
    bool SkeletonRig::CreateBoneMirrorPair(int i1, int i2)
    {
        if(i1 >= boneEntries.size() || i2 >= boneEntries.size())
            return false;

        boneEntries.at(i1).SetMirrorBoneIndex(i2);
        boneEntries.at(i2).SetMirrorBoneIndex(i1);
        return true;
    }
    bool SkeletonRig::ResetAllBoneMirrors()
    {
        for (unsigned int i = 0; i < boneEntries.size(); i++)
        {
            boneEntries.at(i).ResetMirrorBoneIndex();
        }
        return true;
    }
    ;

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
    bool AddBoneToSkeletonRigC(SkeletonRig* rig, float rotationX, float rotationY, float rotationZ, float rotationW, float positionX, float positionY, float positionZ, const char* boneName, int parentIndex, bool usingLocalValues, Utilities::StringContainer& errorMessage)
    {
        errorMessage.Clear();

        if (boneName == "")
        {
            errorMessage += "[CALUMI.Animation API] Bone Entry Must Have Bone Name!";
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
        if (!result)
            errorMessage += std::format("[CALUMI.Animation API] Failure When Adding Bone: {} To Rig.", boneName).c_str();  
        else
            errorMessage += std::format("[CALUMI.Animation API] {} Added To Rig Successfully!", boneName).c_str();

        return result;
    }
    bool SetBoneTypeC(SkeletonBone* bone, uint32_t boneType)
    {
        return bone->SetBoneTypeProperty(static_cast<UNIV::BoneType>(boneType));
    }
    bool SetBoneTypeFromStringC(SkeletonBone* bone, const char* boneStr)
    {
        return bone->SetBoneTypeProperty(BoneTypeFromString(boneStr));
    }
    uint32_t GetBoneTypeC(SkeletonBone* bone)
    {
        return uint32_t(bone->GetBoneTypeProperty()->GetType());
    }
    const char* GetBoneTypeAsStringC(SkeletonBone* bone)
    {
        return bone->GetBoneTypeProperty()->GetTypeString();
    }
    bool SetTwistBonePropertiesC(SkeletonBone* bone, bool reassign, int32_t twistDriverIndex, float twistDriverWeight, Utilities::StringContainer& errorMessage)
    {
        errorMessage.Clear();

        if (bone->GetBoneTypeProperty()->GetType() != UNIV::BoneType::Twist)
        {
            if(!reassign)
            {
                errorMessage += std::format("[CALUMI.Animation API] Bone: {}'s Type Does Not Match Desired Values, Reassigned Set To False. No changes have been made.", bone->name.c_str()).c_str();
                return false;
            }

            if (!bone->SetBoneTypeProperty(UNIV::BoneType::Twist))
            {
                errorMessage += std::format("[CALUMI.Animation API] Bone Type Could Not Be Set For Bone: {}", bone->name.c_str()).c_str();
                return false;
            }
        }

        if (twistDriverIndex < 0)
        {
            errorMessage += std::format("[CALUMI.Animation API] Desired Twist Driver Index For Bone: {} is set to non-value or negative, did you mean to set this bone to Default?", bone->name.c_str()).c_str();
            return false;
        }
        
        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->GetBoneTypeProperty()));
        tProp->twistDriverIndex = twistDriverIndex;
        tProp->twistDriverWeight = twistDriverWeight;

        return true;
    }
    int GetTwistBoneDriverIndexC(SkeletonBone* bone, Utilities::StringContainer& errorMessage)
    {
        errorMessage.Clear();

        if (bone->GetBoneTypeProperty()->GetType() != UNIV::BoneType::Twist)
        {
            errorMessage += std::format("[CALUMI.Animation API] Bone: %s is not set to Twist Type, returning -1", bone->name.c_str()).c_str();
            return -1;
        }

        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->GetBoneTypeProperty()));

        return tProp->twistDriverIndex;
    }
    float GetTwistBoneDriverWeightC(SkeletonBone* bone, Utilities::StringContainer& errorMessage)
    {
        errorMessage.Clear();

        if (bone->GetBoneTypeProperty()->GetType() != UNIV::BoneType::Twist)
        {
            errorMessage += std::format("[CALUMI.Animation API] Bone: %s is not set to Twist Type, returning NaN Float", bone->name.c_str()).c_str();
            return std::numeric_limits<float>::quiet_NaN();
        }

        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->GetBoneTypeProperty()));
        return tProp->twistDriverWeight;
    }
    int SetMirrorIndexC(SkeletonBone* bone, int index)
    {
        bone->SetMirrorBoneIndex(index);
        return bone->GetMirrorBoneIndex();
    }
    int GetMirrorIndexC(SkeletonBone* bone)
    {
        return bone->GetMirrorBoneIndex();
    }
    bool CreateBoneMirrorPairC(SkeletonRig* rig, int index1, int index2)
    {
        return rig->CreateBoneMirrorPair(index1,index2);
    }
    bool ResetAllBoneMirrorsC(SkeletonRig* rig)
    {
        return rig->ResetAllBoneMirrors();
    }
    bool VerifyExclusiveBoneMirrorsC(SkeletonRig* rig)
    {
        return rig->VerifyExclusiveBoneMirrors();
    }
    size_t GetSkeletonRigBoneCountC(SkeletonRig* source)
    {
        return source->GetBoneCount();
    }
    size_t GetSkeletonRigAnimatedBoneCountC(SkeletonRig* source)
    {
        return source->GetAnimatedBoneCount();
    }
    const char* GetSkeletonRigNameC(SkeletonRig* source)
    {
        return source->rigName.c_str();
    }
    SkeletonBone* GetSkeletonBoneC(SkeletonRig* source, int index, Utilities::StringContainer& errorMessage)
    {
        errorMessage.Clear();

        if(source->boneEntries.size() <= index)
        {
            errorMessage += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
            return nullptr;
        }
        if (index < 0)
        {
            errorMessage += "[CALUMI.Animation API] Index Is Negative! Is This Parent Index Coming From The Root?";
            return nullptr;
        }
        return &source->boneEntries.at(index);
    }
    const char* GetSkeletonBoneNameC(SkeletonBone* source)
    {
        return source->name.c_str();
    }
    int GetSkeletonBoneParentIndexC(SkeletonBone* source)
    {
        return source->parentBoneIndex;
    }
    CALUMI::Math::Quaternion* GetSkeletonBoneRotationC(SkeletonBone* source, bool fromRoot)
    {
        if (fromRoot)
        {
            return &source->globalRotation;
        }
        
        return &source->localRotation;
    }
    CALUMI::Math::Vector3* GetSkeletonBoneTranslationC(SkeletonBone* source, bool fromRoot)
    {
        if (fromRoot)
        {
            return &source->globalPosition;
        }

        return &source->localPosition;
    }
    bool ValidateSkeletonRigNamesC(SkeletonRig* source, Utilities::StringContainer& errorMessage)
    {
        errorMessage.Clear();
        auto result = source->ValidateNames();
        if (!result.has_value())
        {
            errorMessage += result.error().c_str();
            return false;
        }
        return true;
    }
    bool ValidateSkeletonRigParentIndicesC(SkeletonRig* source, Utilities::StringContainer& errorMessage)
    {
        errorMessage.Clear();
        auto result = source->ValidateParentIndices();
        if (!result.has_value())
        {
            errorMessage += result.error().c_str();
            return false;
        }
        return true;
    }

    bool SkeletonRig::VerifyExclusiveBoneMirrors()
    {
        for (unsigned int i = 0; i < boneEntries.size(); i++)
        {
            int mirror = boneEntries.at(i).GetMirrorBoneIndex();
            if (mirror == -1)
            {
                //skip as we have declared this as having no mirror
            }
            else
            {
                if (mirror != i && mirror >= 0)
                {
                    if (i != boneEntries.at(mirror).GetMirrorBoneIndex())
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    size_t UNIV::SkeletonRig::GetAnimatedBoneCount()
    {
        unsigned int AnimatedBoneCount = 0;

        for (unsigned int i = 0; i < boneEntries.size(); i++)
        {
            if (boneEntries.at(i).GetBoneTypeProperty()->GetType() == UNIV::BoneType::Default)
            {
                AnimatedBoneCount++;
            }
        }

        return AnimatedBoneCount;
    }

    size_t SkeletonRig::GetBoneCount() const
    {
        return boneEntries.size();
    }

    Utilities::StringContainer SkeletonRig::ToJSON(const size_t indents = 0) const {
        Utilities::StringContainer output = Utilities::Indent(indents).c_str();
        output += "{\n";
        output += std::format("{0}\"rigName\":\"{1}\",\n{0}\"boneEntries\":", Utilities::Indent(indents + 1).c_str(),rigName.c_str()).c_str();
        output += Utilities::VectorToJSON(boneEntries,indents + 1);
        output += "\n ";
        output += Utilities::Indent(indents).c_str();
        output += "}";
        return output;
    }

    bool UNIV::SkeletonBone::SetBoneTypeProperty(UNIV::BoneType boneType, bool resetExisting)
    {

        if (boneTypeProperties != nullptr)
        {
            if (!resetExisting && boneType == boneTypeProperties->GetType())
                return false;
        }

        BoneTypeProperties* property;

        switch (boneType)
        {
        case CALUMI::UNIV::BoneType::Default:
            property = new DefaultBoneProperties;
            break;
        case CALUMI::UNIV::BoneType::Twist:
            property = new TwistBoneProperties;
            break;
        default:
            return false;
        }

        if (boneTypeProperties != nullptr)
        {
            delete boneTypeProperties;
            boneTypeProperties = nullptr;
        }
        boneTypeProperties = property;
        return true;
    }

    const BoneTypeProperties* SkeletonBone::GetBoneTypeProperty()
    {
        if (boneTypeProperties != nullptr)
        {
            return boneTypeProperties;
        }

        ResetBoneTypeProperty();
        return boneTypeProperties;
    }

    bool UNIV::SkeletonBone::ResetBoneTypeProperty(UNIV::BoneType boneType)
    {
        return this->SetBoneTypeProperty(boneType, true);
    }

    void SkeletonBone::SetMirrorBoneIndex(int idx) { mirrorBoneIndex = idx; }

    void SkeletonBone::ResetMirrorBoneIndex() { mirrorBoneIndex = -1; }

    int SkeletonBone::GetMirrorBoneIndex() const { return mirrorBoneIndex; }

    void SkeletonBone::SetParentBoneIndex(int idx) { parentBoneIndex = idx; }

    void SkeletonBone::ResetParentBoneIndex() { parentBoneIndex = 0; }

    void SkeletonBone::SetAsRootBone() { parentBoneIndex = -1; }

    int SkeletonBone::GetParentBoneIndex() const { return parentBoneIndex; }

    //This is not the preferred way
    const char* SkeletonBone::_GetBoneTypeString() const
    {
        if (boneTypeProperties == nullptr)
            return UNIV::DefaultBoneTypeStr;

        return boneTypeProperties->GetTypeString();
    }
    Utilities::StringContainer SkeletonBone::ToJSON(const size_t indents = 0) const {

        std::string output = (
            Utilities::Indent(indents) + "{\n" +  
            Utilities::Indent(indents+1).c_str() + "\"name\":\"" + name.c_str() + "\",\n" +
            Utilities::Indent(indents+1).c_str() + "\"parentBoneIndex\":" + std::to_string(parentBoneIndex).c_str() + ",\n" +
            Utilities::Indent(indents + 1).c_str() + "\"boneType\":" + _GetBoneTypeString() + ",\n" +
            Utilities::Indent(indents + 1).c_str() + "\"mirrorBoneIndex\":" + std::to_string(mirrorBoneIndex).c_str() + ",\n"
            ).c_str();

        output += (Utilities::Indent(indents+1) + "\"localRotation\": [").c_str();
        output += std::to_string(localRotation.x) + ", " + std::to_string(localRotation.y) + ", " + std::to_string(localRotation.z) + ", " + std::to_string(localRotation.w) + "],\n";
        
        output += (Utilities::Indent(indents + 1) + "\"globalRotation\": [").c_str();
        output += std::to_string(globalRotation.x) + ", " + std::to_string(globalRotation.y) + ", " + std::to_string(globalRotation.z) + ", " + std::to_string(globalRotation.w) + "],\n";
        
        output += (Utilities::Indent(indents + 1) + "\"localPosition\": [").c_str();
        output += std::to_string(localPosition.x) + ", " + std::to_string(localPosition.y) + ", " + std::to_string(localPosition.z) + "],\n";
        
        output += (Utilities::Indent(indents + 1) + "\"globalPosition\": [").c_str();
        output += std::to_string(globalPosition.x) + ", " + std::to_string(globalPosition.y) + ", " + std::to_string(globalPosition.z) + "]\n";
        
        output += (Utilities::Indent(indents) + "}").c_str();
        return output.c_str();
    }

}

}

