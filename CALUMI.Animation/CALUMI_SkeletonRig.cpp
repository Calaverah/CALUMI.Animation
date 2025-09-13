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
        if (boneEntries.size() >= MaxBoneCount)
        {
            std::println("[CALUMI.Animation API] Bone Count Limit Reached For Rig: {}",rigName);
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

        input.mirrorBoneIndex = boneEntries.size();

        boneEntries.push_back(input);

        return true;
    }
    bool SkeletonRig::CreateBoneMirrorPair(int i1, int i2)
    {
        if(i1 >= boneEntries.size() || i2 >= boneEntries.size())
            return false;

        boneEntries.at(i1).mirrorBoneIndex = i2;
        boneEntries.at(i2).mirrorBoneIndex = i1;
        return true;
    }
    bool SkeletonRig::ResetAllBoneMirrors()
    {
        for (unsigned int i = 0; i < boneEntries.size(); i++)
        {
            boneEntries.at(i).mirrorBoneIndex = i;
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
    bool SetTwistBonePropertiesC(SkeletonBone* bone, bool reassign, int32_t twistDriverIndex, float twistDriverWeight, const char* errorMessage)
    {
        char buffer[MAX_PATH];
        if (bone->GetBoneTypeProperty()->GetType() != UNIV::BoneType::Twist)
        {
            if(!reassign)
            {
                snprintf(buffer, sizeof(buffer), "[CALUMI.Animation API] Bone: %s 's Type Does Not Match Desired Values, Reassigned Set To False. No changes have been made.", bone->name.c_str());
                errorMessage = buffer;
                return false;
            }

            if (!bone->SetBoneTypeProperty(UNIV::BoneType::Twist))
            {
                snprintf(buffer, sizeof(buffer), "[CALUMI.Animation API] Bone Type Could Not Be Set For Bone: %s", bone->name.c_str());
                errorMessage = buffer;
                return false;
            }
        }

        if (twistDriverIndex < 0)
        {
            snprintf(buffer, sizeof(buffer), "[CALUMI.Animation API] Desired Twist Driver Index For Bone: %s is set to non-value or negative, did you mean to set this bone to Default?", bone->name.c_str());
            errorMessage = buffer;
            return false;
        }
        
        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->GetBoneTypeProperty()));
        tProp->twistDriverIndex = twistDriverIndex;
        tProp->twistDriverWeight = twistDriverWeight;

        return true;
    }
    CALUMIANIMATION_API int GetTwistBoneDriverIndexC(SkeletonBone* bone, const char* errorMessage)
    {
        char buffer[MAX_PATH];
        if (bone->GetBoneTypeProperty()->GetType() != UNIV::BoneType::Twist)
        {
            snprintf(buffer, sizeof(buffer), "[CALUMI.Animation API] Bone: %s is not set to Twist Type, returning -1", bone->name.c_str());
            errorMessage = buffer;
            return -1;
        }

        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->GetBoneTypeProperty()));

        return tProp->twistDriverIndex;
    }
    float GetTwistBoneDriverWeightC(SkeletonBone* bone, const char* errorMessage)
    {
        char buffer[MAX_PATH];
        if (bone->GetBoneTypeProperty()->GetType() != UNIV::BoneType::Twist)
        {
            snprintf(buffer, sizeof(buffer), "[CALUMI.Animation API] Bone: %s is not set to Twist Type, returning NaN Float", bone->name.c_str());
            errorMessage = buffer;
            return std::numeric_limits<float>::quiet_NaN();
        }

        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->GetBoneTypeProperty()));
        return tProp->twistDriverWeight;
    }
    int SetMirrorIndexC(SkeletonBone* bone, int index)
    {
        bone->mirrorBoneIndex = index;
        return bone->mirrorBoneIndex;
    }
    int GetMirrorIndexC(SkeletonBone* bone)
    {
        return bone->mirrorBoneIndex;
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
    unsigned int GetSkeletonRigBoneCountC(SkeletonRig* source)
    {
        return source->GetBoneCount();
    }
    unsigned int GetSkeletonRigAnimatedBoneCountC(SkeletonRig* source)
    {
        return source->GetAnimatedBoneCount();
    }
    const char* GetSkeletonRigNameC(SkeletonRig* source)
    {
        return source->rigName.c_str();
    }
    SkeletonBone* GetSkeletonBoneC(SkeletonRig* source, int index, const char* errorMessage)
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
            return &source->rootRotation;
        }
        
        return &source->localRotation;
    }
    CALUMI::Math::Vector3* GetSkeletonBoneTranslationC(SkeletonBone* source, bool fromRoot)
    {
        if (fromRoot)
        {
            return &source->rootPosition;
        }

        return &source->localPosition;
    }
    bool ValidateSkeletonRigNamesC(SkeletonRig* source, const char* errorMessage)
    {
        auto result = source->ValidateNames();
        if (!result.has_value())
        {
            errorMessage = result.error().c_str();
            return false;
        }
        return true;
    }
    bool ValidateSkeletonRigParentIndicesC(SkeletonRig* source, const char* errorMessage)
    {
        auto result = source->ValidateParentIndices();
        if (!result.has_value())
        {
            errorMessage = result.error().c_str();
            return false;
        }
        return true;
    }

    bool SkeletonRig::VerifyExclusiveBoneMirrors()
    {
        for (unsigned int i = 0; i < boneEntries.size(); i++)
        {
            int mirror = boneEntries.at(i).mirrorBoneIndex;
            if (mirror == -1)
            {
                //skip as we don't fully understand this notion yet
            }
            else
            {
                if (mirror != i && mirror >= 0)
                {
                    if (i != boneEntries.at(mirror).mirrorBoneIndex)
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    unsigned int UNIV::SkeletonRig::GetAnimatedBoneCount()
    {
        unsigned int AnimatedBoneCount = 0;

        for (UNIV::SkeletonBone bone : boneEntries)
        {
            if (bone.GetBoneTypeProperty()->GetType() == UNIV::BoneType::Default)
            {
                AnimatedBoneCount++;
            }
        }

        return AnimatedBoneCount;
    }

    unsigned int SkeletonRig::GetBoneCount()
    {
        return boneEntries.size();
    }

    std::string SkeletonRig::ToJSON(const int indents = 0) const {
        std::string output = Utilities::Indent(indents) + "{\n" + Utilities::Indent(indents+1) +  "\"rigName\":\"" + rigName + "\",\n" + Utilities::Indent(indents+1) + "\"boneEntries\":";
        output += Utilities::VectorToJSON(boneEntries, indents + 1);
        output += "\n " + Utilities::Indent(indents) + "}";
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

    //This is not the preferred way
    const char* SkeletonBone::_GetBoneTypeString() const
    {
        if (boneTypeProperties == nullptr)
            return UNIV::DefaultBoneTypeStr;

        return boneTypeProperties->GetTypeString();
    }
    std::string SkeletonBone::ToJSON(const int indents = 0) const {

        std::string output = 
            Utilities::Indent(indents) + "{\n" +  
            Utilities::Indent(indents+1) + "\"name\":\"" + name + "\",\n" + 
            Utilities::Indent(indents+1) + "\"parentBoneIndex\":" + std::to_string(parentBoneIndex) + ",\n" + 
            Utilities::Indent(indents + 1) + "\"boneType\":" + _GetBoneTypeString() + ",\n" +
            Utilities::Indent(indents + 1) + "\"mirrorBoneIndex\":" + std::to_string(mirrorBoneIndex) + ",\n";

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

