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
#include <vector>
#include <memory>


namespace CALUMI{ namespace UNIV{

#pragma region SKELETONRIG

    struct SkeletonRig::Impl
    {
        /// <summary>
        /// Optional name for your skeleton rig.
        /// </summary>
        Utilities::StringContainer rigName = "MySkeletonRig";
        Utilities::VectorContainer<SkeletonBone> boneEntries;

        RigPackageManager rigPackageManager;
        Impl() = default;
    };
    Utilities::StringContainer& SkeletonRig::RigName() const { return pImpl->rigName; }
    Utilities::VectorContainer<SkeletonBone>& SkeletonRig::BoneEntries() const { return pImpl->boneEntries; }
    RigPackageManager& SkeletonRig::getRigPackageManager() const { return pImpl->rigPackageManager; }


    SkeletonRig::SkeletonRig() { pImpl = new Impl; }
    SkeletonRig::SkeletonRig(const SkeletonRig& input) : SkeletonRig() { *pImpl = *(input.pImpl); }
    SkeletonRig::~SkeletonRig() { if (pImpl) delete pImpl; }
    SkeletonRig::SkeletonRig(Utilities::StringContainer& rigName) : SkeletonRig() { pImpl->rigName = rigName; }
    SkeletonRig::SkeletonRig(const char* rigName) : SkeletonRig() { pImpl->rigName = rigName; }

    Utilities::ExpectedContainer< bool, Utilities::StringContainer> SkeletonRig::ValidateNames() const
    {
        
        if (pImpl->boneEntries.size() == 0)
        {
            Utilities::ExpectedContainer<bool, Utilities::StringContainer> tempOutput;
            tempOutput.SetErrorValue("Two Bone Entries Share The Same Name In This Rig"); //Duplicate name found
            return tempOutput;
        }

        std::set<std::string> uniqueNames;
        for (unsigned int i = 0; i < pImpl->boneEntries.size(); i++)
        {
            auto result = uniqueNames.insert(pImpl->boneEntries.at(i).Name().c_str());
            if (!result.second)
            {
                Utilities::ExpectedContainer<bool, Utilities::StringContainer> tempOutput;
                tempOutput.SetErrorValue("No Bone Entries For This Rig Have Been Found");
                return tempOutput;
            }
        }

        return true;
    }

    Utilities::ExpectedContainer< bool, Utilities::StringContainer> SkeletonRig::ValidateParentIndices()
    {
        if (pImpl->boneEntries.size() < 2)
        {
            Utilities::ExpectedContainer<bool, Utilities::StringContainer> tempOutput;
            tempOutput.SetErrorValue("Not Enough Bone Entries To Validate Parent Entries");
            return tempOutput;
        }

        for (int i = 0; i < pImpl->boneEntries.size(); i++)
        {
            if (pImpl->boneEntries.at(i).GetParentBoneIndex() >= i)
            {
                Utilities::ExpectedContainer<bool, Utilities::StringContainer> tempOutput;
                tempOutput.SetErrorValue(std::format("Bone Index: {} Has Parent Index: {}. Parent Index Cannot Be Greater Than Or Equal To Bone's Index", i, pImpl->boneEntries.at(i).GetParentBoneIndex()).c_str());
                return tempOutput;
            }
        }
        return true;
    }

    bool SkeletonRig::AddBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, Utilities::StringContainer boneName, Utilities::StringContainer parentName, bool localValues)
    {
        int newParentIndex = 0;
        if (pImpl->boneEntries.size() == 0)
        {
            newParentIndex = -1;
        }
        else
        {
            for (unsigned int i = 0; i < pImpl->boneEntries.size(); i++)
            {
                if (pImpl->boneEntries.at(i).Name().compare(parentName, false) == 0)
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
        if (pImpl->boneEntries.size() >= MaxBoneCount)
        {
            std::println("[CALUMI.Animation API] Bone Count Limit Reached For Rig: {}", pImpl->rigName.c_str());
            return false;
        }

        if (boneName == "")
        {
            std::println("[CALUMI.Animation API] Bone Must Have Valid Bone Name");
            return false;
        }

        for (size_t i = 0; i < pImpl->boneEntries.size(); i++)
        {
            if (boneName.compare(pImpl->boneEntries.at(i).Name(), false) == 0)
            {
                std::println("[CALUMI.Animation API] Bone Name Must Be Unique");
                return false;
            }
        }

        SkeletonBone input;
        input.Name() = boneName;
        input.SetParentBoneIndex(parentIndex);

        CALUMI::Math::Quaternion rootRotation;
        CALUMI::Math::Quaternion localRotation;
        CALUMI::Math::Vector3 rootPosition;
        CALUMI::Math::Vector3 localPosition;
        if (pImpl->boneEntries.empty())
        {
            rootRotation = rotation;
            localRotation = rotation;
            rootPosition = position;
            localPosition = position;
        }
        else if (localValues)
        {
            rootRotation = rotation * pImpl->boneEntries.at(input.GetParentBoneIndex()).GlobalRotation();
            localRotation = rotation;
            rootPosition = position + pImpl->boneEntries.at(input.GetParentBoneIndex()).GlobalPosition();
            localPosition = position;
        }
        else
        {
            CALUMI::Math::Quaternion inverseParentRoot;
            pImpl->boneEntries.at(input.GetParentBoneIndex()).GlobalRotation().Inverse(inverseParentRoot);
            localRotation = rotation * inverseParentRoot;
            rootRotation = rotation;
            localPosition = position - pImpl->boneEntries.at(input.GetParentBoneIndex()).GlobalPosition();
            rootPosition = position;
        }

        
        
        input.LocalPosition() = localPosition;
        input.GlobalPosition() = rootPosition;
        input.LocalRotation() = localRotation;
        input.GlobalRotation() = rootRotation;

        input.ResetMirrorBoneIndex(); //static_cast<int>(boneEntries.size());

        pImpl->boneEntries.push_back(input);

        return true;
    }
    bool SkeletonRig::RenameBone(const char* oldBoneName, const char* newBoneName)
    {
        auto result = GetBoneIndex(oldBoneName);
        if(!result.has_value())
            return false;

        return RenameBone(result.value(), newBoneName);
    }
    bool SkeletonRig::RenameBone(size_t boneIndex, const char* newBoneName)
    {
        Utilities::StringContainer oldName = pImpl->boneEntries.at(boneIndex).Name();
        pImpl->boneEntries.at(boneIndex).Name() = newBoneName;
        return pImpl->rigPackageManager.HandleBoneRename(oldName.c_str(), newBoneName, boneIndex);
    }
    bool SkeletonRig::CreateBoneMirrorPair(int i1, int i2)
    {
        if(i1 >= pImpl->boneEntries.size() || i2 >= pImpl->boneEntries.size())
            return false;

        pImpl->boneEntries.at(i1).SetMirrorBoneIndex(i2);
        pImpl->boneEntries.at(i2).SetMirrorBoneIndex(i1);
        return true;
    }
    bool SkeletonRig::ResetAllBoneMirrors()
    {
        for (unsigned int i = 0; i < pImpl->boneEntries.size(); i++)
        {
            pImpl->boneEntries.at(i).ResetMirrorBoneIndex();
        }
        return true;
    }

    bool SkeletonRig::VerifyExclusiveBoneMirrors()
    {
        for (unsigned int i = 0; i < pImpl->boneEntries.size(); i++)
        {
            int mirror = pImpl->boneEntries.at(i).GetMirrorBoneIndex();
            if (mirror == -1)
            {
                //skip as we have declared this as having no mirror
            }
            else
            {
                if (mirror != i && mirror >= 0)
                {
                    if (i != pImpl->boneEntries.at(mirror).GetMirrorBoneIndex())
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

        for (unsigned int i = 0; i < pImpl->boneEntries.size(); i++)
        {
            if (pImpl->boneEntries.at(i).GetBoneTypeProperty()->GetType() == UNIV::BoneType::Default)
            {
                AnimatedBoneCount++;
            }
        }

        return AnimatedBoneCount;
    }

    size_t SkeletonRig::GetBoneCount() const
    {
        return pImpl->boneEntries.size();
    }

    Utilities::ExpectedContainer<size_t, Utilities::StringContainer> SkeletonRig::GetBoneIndex(Utilities::StringContainer boneName)
    {
        Utilities::ExpectedContainer<size_t, Utilities::StringContainer> tempOutput;

        if (boneName == "") { tempOutput.SetErrorValue(std::format("[CALUMI.Animation API] UNIV::SkeletonRig::GetBoneIndex requires a valid boneName string argument. Given argument was {}", boneName.c_str()).c_str());  return tempOutput; }

        for (size_t i = 0; i < pImpl->boneEntries.size(); i++)
        {
            if (pImpl->boneEntries.at(i).Name() == boneName)
            {
                tempOutput.SetValue(i);
                return tempOutput;
            }
        }
        tempOutput.SetErrorValue(std::format("[CALUMI.Animation API] No Bones With Given Name: {} Exist In Rig: {}", boneName.c_str(), pImpl->rigName.c_str()).c_str());
        return tempOutput;

    }

    Utilities::StringContainer SkeletonRig::ToJSON(const size_t indents = 0) const {
        Utilities::StringContainer output = Utilities::Indent(indents).c_str();
        output += "{\n";
        output += std::format("{0}\"rigName\":\"{1}\",\n{0}\"boneEntries\":", Utilities::Indent(indents + 1).c_str(), pImpl->rigName.c_str()).c_str();
        output += Utilities::VectorToJSON(pImpl->boneEntries,indents + 1);
        output += "\n ";
        output += Utilities::Indent(indents).c_str();
        output += "}";
        return output;
    }

    SkeletonRig& SkeletonRig::operator=(const SkeletonRig& other)
    {
        *pImpl = *(other.pImpl);
        return *this;
    }
#pragma endregion

#pragma region SKELETONBONE
    struct SkeletonBone::Impl
    {
    public:
        std::shared_ptr<BoneTypeProperties> boneTypeProperties = std::make_shared<DefaultBoneProperties>();
        int mirrorBoneIndex = -1;
        int parentBoneIndex = -1;
        //We keep this private as it is not the preferred way to get this value.
        //It should only be used on serialization functions that are constant where default resetting is not possible
        //This is not the preferred way
        const char* _GetBoneTypeString() const
        {
            if (boneTypeProperties.get() == nullptr)
                return "ERROR!";

            return boneTypeProperties->GetTypeString();
        }

    public:
        CALUMI::Math::Quaternion localRotation;
        CALUMI::Math::Quaternion globalRotation;
        CALUMI::Math::Vector3 localPosition;
        CALUMI::Math::Vector3 globalPosition;
        Utilities::StringContainer name;
    };

    Math::Quaternion& SkeletonBone::LocalRotation() const { return pImpl->localRotation; }
    Math::Quaternion& SkeletonBone::GlobalRotation() const { return pImpl->globalRotation; }
    Math::Vector3& SkeletonBone::LocalPosition() const { return pImpl->localPosition; }
    Math::Vector3& SkeletonBone::GlobalPosition() const { return pImpl->globalPosition; }
    Utilities::StringContainer& SkeletonBone::Name() const { return pImpl->name; }

    SkeletonBone::SkeletonBone() { pImpl = new Impl; }
    SkeletonBone::SkeletonBone(const SkeletonBone& other) : SkeletonBone()
    {
        *pImpl = *(other.pImpl);
    }
    UNIV::SkeletonBone& UNIV::SkeletonBone::operator=(const UNIV::SkeletonBone& other)
    {
        *pImpl = *(other.pImpl);
        return *this;
    }
    SkeletonBone::~SkeletonBone()
    {
        if (pImpl) delete pImpl;
    }

    bool UNIV::SkeletonBone::SetBoneTypeProperty(UNIV::BoneType boneType, bool resetExisting)
    {

        if (pImpl->boneTypeProperties.get())
        {
            if (!resetExisting && boneType == pImpl->boneTypeProperties->GetType())
                return false;
        }

        switch (boneType)
        {
        case CALUMI::UNIV::BoneType::Default:
            pImpl->boneTypeProperties = std::make_shared<DefaultBoneProperties>();
            break;
        case CALUMI::UNIV::BoneType::Twist:
            pImpl->boneTypeProperties = std::make_shared<TwistBoneProperties>();
            break;
        default:
            return false;
        }
        return true;
    }

    const BoneTypeProperties* SkeletonBone::GetBoneTypeProperty()
    {
        if (pImpl->boneTypeProperties.get())
        {
            return pImpl->boneTypeProperties.get();
        }

        ResetBoneTypeProperty();
        return pImpl->boneTypeProperties.get();
    }

    bool UNIV::SkeletonBone::ResetBoneTypeProperty(UNIV::BoneType boneType)
    {
        return this->SetBoneTypeProperty(boneType, true);
    }

    void SkeletonBone::SetMirrorBoneIndex(int idx) { pImpl->mirrorBoneIndex = idx; }

    void SkeletonBone::ResetMirrorBoneIndex() { pImpl->mirrorBoneIndex = -1; }

    int SkeletonBone::GetMirrorBoneIndex() const { return pImpl->mirrorBoneIndex; }

    void SkeletonBone::SetParentBoneIndex(int idx) { pImpl->parentBoneIndex = idx; }

    void SkeletonBone::ResetParentBoneIndex() { pImpl->parentBoneIndex = 0; }

    void SkeletonBone::SetAsRootBone() { pImpl->parentBoneIndex = -1; }

    int SkeletonBone::GetParentBoneIndex() const { return pImpl->parentBoneIndex; }

    Utilities::StringContainer SkeletonBone::ToJSON(const size_t indents = 0) const {

        std::string output = (
            Utilities::Indent(indents) + "{\n" +  
            Utilities::Indent(indents+1).c_str() + "\"name\":\"" + pImpl->name.c_str() + "\",\n" +
            Utilities::Indent(indents+1).c_str() + "\"parentBoneIndex\":" + std::to_string(pImpl->parentBoneIndex).c_str() + ",\n" +
            Utilities::Indent(indents + 1).c_str() + "\"boneType\":" + pImpl->_GetBoneTypeString() + ",\n" +
            Utilities::Indent(indents + 1).c_str() + "\"mirrorBoneIndex\":" + std::to_string(pImpl->mirrorBoneIndex).c_str() + ",\n"
            ).c_str();

        output += (Utilities::Indent(indents+1) + "\"localRotation\": [").c_str();
        output += std::to_string(pImpl->localRotation.getX()) + ", " + std::to_string(pImpl->localRotation.getY()) + ", " + std::to_string(pImpl->localRotation.getZ()) + ", " + std::to_string(pImpl->localRotation.getW()) + "],\n";
        
        output += (Utilities::Indent(indents + 1) + "\"globalRotation\": [").c_str();
        output += std::to_string(pImpl->globalRotation.getX()) + ", " + std::to_string(pImpl->globalRotation.getY()) + ", " + std::to_string(pImpl->globalRotation.getZ()) + ", " + std::to_string(pImpl->globalRotation.getW()) + "],\n";
        
        output += (Utilities::Indent(indents + 1) + "\"localPosition\": [").c_str();
        output += std::to_string(pImpl->localPosition.getX()) + ", " + std::to_string(pImpl->localPosition.getY()) + ", " + std::to_string(pImpl->localPosition.getZ()) + "],\n";
        
        output += (Utilities::Indent(indents + 1) + "\"globalPosition\": [").c_str();
        output += std::to_string(pImpl->globalPosition.getX()) + ", " + std::to_string(pImpl->globalPosition.getY()) + ", " + std::to_string(pImpl->globalPosition.getZ()) + "]\n";
        
        output += (Utilities::Indent(indents) + "}").c_str();
        return output.c_str();
    }
#pragma endregion

#pragma region RIGPACKAGEMANAGER

    struct RigPackageManager::Impl
    {
        Utilities::VectorContainer<RigPackage*> packages;
        
        void clear()
        {
            for (size_t i = 0; i < packages.size(); i++)
            {
                if (packages.at(i))
                {
                    delete packages.at(i);
                    packages.at(i) = nullptr;
                }
            }
            packages.clear();
        }

        Impl() = default;
        ~Impl() {
                clear();
        }
    };
    RigPackageManager::RigPackageManager()
    {
        pImpl = new Impl;
    }
    RigPackageManager::RigPackageManager(const RigPackageManager& input) : RigPackageManager()
    {
        for (size_t i = 0; i < input.pImpl->packages.size(); i++)
        {
            if (auto ptr = input.pImpl->packages.at(i))
            {
                pImpl->packages.push_back(ptr->Clone());
            }
        }
    }
    RigPackageManager& RigPackageManager::operator=(const RigPackageManager& other)
    {
        pImpl->clear();
        for (size_t i = 0; i < other.pImpl->packages.size(); i++)
        {
            if (auto ptr = other.pImpl->packages.at(i))
            {
                pImpl->packages.push_back(ptr->Clone());
            }
        }
        return *this;
    }
    RigPackageManager::~RigPackageManager() { if (pImpl) delete pImpl; }

    RigPackage* RigPackageManager::GetPackage(const char* packageName)
    {
        for (int i = 0; i < pImpl->packages.size(); i++)
        {
            if (_stricmp(pImpl->packages.at(i)->GetPackageType(), packageName) == 0)
            {
                return pImpl->packages.at(i);
            }
        }
        return nullptr;
    }

    bool RigPackageManager::RemovePackage(const char* packageName)
    {
        for (int i = 0; i < pImpl->packages.size(); i++)
        {
            if (_stricmp(pImpl->packages.at(i)->GetPackageType(), packageName) == 0)
            {
                if (pImpl->packages.at(i))
                {
                    delete pImpl->packages.at(i);
                    pImpl->packages.at(i) = nullptr;
                    pImpl->packages.erase(i);
                    return true;
                }
            }
        }
        return false;
    }

    bool RigPackageManager::AddPackage(RigPackage* package, bool overwrite)
    {
        for (int i = 0; i < pImpl->packages.size(); i++)
        {
            if (_stricmp(pImpl->packages.at(i)->GetPackageType(), package->GetPackageType()) == 0)
            {
                if (!overwrite)
                {
                    return false;
                }
                pImpl->packages.at(i) = package;
                return true;
            }
        }
        pImpl->packages.push_back(package);
        return true;
    }

    bool RigPackageManager::HandleBoneRename(const char* oldBone, const char* newName, size_t idx)
    {
        for (size_t i = 0; i < pImpl->packages.size(); i++)
        {
            if (!pImpl->packages.at(i)->HandleBoneRename(oldBone, newName, idx))
                return false;
        }
        return true;
    }

#pragma endregion

#pragma region EXTERN"C"

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
            ptr = nullptr;
            return true;
        }
        return false;
    }
    bool AddBoneToSkeletonRigC(SkeletonRig* rig, float rotationX, float rotationY, float rotationZ, float rotationW, float positionX, float positionY, float positionZ, const char* boneName, int parentIndex, bool usingLocalValues, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();

        if (boneName == "")
        {
            *errorMessageHolder += "[CALUMI.Animation API] Bone Entry Must Have Bone Name!";
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
            *errorMessageHolder += std::format("[CALUMI.Animation API] Failure When Adding Bone: {} To Rig.", boneName).c_str();
        else
            *errorMessageHolder += std::format("[CALUMI.Animation API] {} Added To Rig Successfully!", boneName).c_str();

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
    bool SetTwistBonePropertiesC(SkeletonBone* bone, bool reassign, int32_t twistDriverIndex, float twistDriverWeight, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();

        if (bone->GetBoneTypeProperty()->GetType() != UNIV::BoneType::Twist)
        {
            if (!reassign)
            {
                *errorMessageHolder += std::format("[CALUMI.Animation API] Bone: {}'s Type Does Not Match Desired Values, Reassigned Set To False. No changes have been made.", bone->Name().c_str()).c_str();
                return false;
            }

            if (!bone->SetBoneTypeProperty(UNIV::BoneType::Twist))
            {
                *errorMessageHolder += std::format("[CALUMI.Animation API] Bone Type Could Not Be Set For Bone: {}", bone->Name().c_str()).c_str();
                return false;
            }
        }

        if (twistDriverIndex < 0)
        {
            *errorMessageHolder += std::format("[CALUMI.Animation API] Desired Twist Driver Index For Bone: {} is set to non-value or negative, did you mean to set this bone to Default?", bone->Name().c_str()).c_str();
            return false;
        }

        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->GetBoneTypeProperty()));
        tProp->TwistDriverIndex(twistDriverIndex);
        tProp->TwistDriverWeight(twistDriverWeight);

        return true;
    }
    int GetTwistBoneDriverIndexC(SkeletonBone* bone, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();

        if (bone->GetBoneTypeProperty()->GetType() != UNIV::BoneType::Twist)
        {
            *errorMessageHolder += std::format("[CALUMI.Animation API] Bone: %s is not set to Twist Type, returning -1", bone->Name().c_str()).c_str();
            return -1;
        }

        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->GetBoneTypeProperty()));

        return tProp->TwistDriverIndex();
    }
    float GetTwistBoneDriverWeightC(SkeletonBone* bone, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();

        if (bone->GetBoneTypeProperty()->GetType() != UNIV::BoneType::Twist)
        {
            *errorMessageHolder += std::format("[CALUMI.Animation API] Bone: %s is not set to Twist Type, returning NaN Float", bone->Name().c_str()).c_str();
            return std::numeric_limits<float>::quiet_NaN();
        }

        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->GetBoneTypeProperty()));
        return tProp->TwistDriverWeight();
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
        return rig->CreateBoneMirrorPair(index1, index2);
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
        return source->RigName().c_str();
    }
    SkeletonBone* GetSkeletonBoneC(SkeletonRig* source, int index, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();

        if (source->BoneEntries().size() <= index)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
            return nullptr;
        }
        if (index < 0)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Index Is Negative! Is This Parent Index Coming From The Root?";
            return nullptr;
        }
        return &source->BoneEntries().at(index);
    }
    const char* GetSkeletonBoneNameC(SkeletonBone* source)
    {
        return source->Name().c_str();
    }
    int GetSkeletonBoneParentIndexC(SkeletonBone* source)
    {
        return source->GetParentBoneIndex();
    }
    CALUMI::Math::Quaternion* GetSkeletonBoneRotationC(SkeletonBone* source, bool fromRoot)
    {
        if (fromRoot)
        {
            return &source->GlobalRotation();
        }

        return &source->LocalRotation();
    }
    CALUMI::Math::Vector3* GetSkeletonBonePositionC(SkeletonBone* source, bool fromRoot)
    {
        if (fromRoot)
        {
            return &source->GlobalPosition();
        }

        return &source->LocalPosition();
    }
    bool ValidateSkeletonRigNamesC(SkeletonRig* source, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();
        auto result = source->ValidateNames();
        if (!result.has_value())
        {
            *errorMessageHolder += result.error().c_str();
            return false;
        }
        return true;
    }
    bool ValidateSkeletonRigParentIndicesC(SkeletonRig* source, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();
        auto result = source->ValidateParentIndices();
        if (!result.has_value())
        {
            *errorMessageHolder += result.error().c_str();
            return false;
        }
        return true;
    }
#pragma endregion


}}

