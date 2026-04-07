//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalvectordef.h"
#include "internalplatform.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"
#include "utilities/CALUMI_Utilities.h"
#include <set>
#include <print>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>


namespace CALUMI{ namespace UNIV{

#pragma region SKELETONRIG

    struct SkeletonRig::Impl
    {
        /// <summary>
        /// Optional name for your skeleton rig.
        /// </summary>
        Utilities::StringContainer _rigName = "MySkeletonRig";
        SkeletonBoneVector _boneEntries;

        RigPackageManager _rigPackageManager;
        Impl() = default;
    };
    Utilities::StringContainer& SkeletonRig::rigName() const { return pImpl->_rigName; }
    SkeletonBoneVector& SkeletonRig::boneEntries() const { return pImpl->_boneEntries; }
    RigPackageManager& SkeletonRig::getPackageManager() const { return pImpl->_rigPackageManager; }


    SkeletonRig::SkeletonRig() { pImpl = new Impl; }
    SkeletonRig::SkeletonRig(const SkeletonRig& input) : SkeletonRig() { *pImpl = *(input.pImpl); }
    SkeletonRig::~SkeletonRig() { if (pImpl) delete pImpl; }
    SkeletonRig::SkeletonRig(Utilities::StringContainer& _rigName) : SkeletonRig() { pImpl->_rigName = _rigName; }
    SkeletonRig::SkeletonRig(const char* _rigName) : SkeletonRig() { pImpl->_rigName = _rigName; }

    bool SkeletonRig::validateNames() const
    {
        std::set<std::string> uniqueNames;

        for (unsigned int i = 0; i < pImpl->_boneEntries.size(); i++)
        {
            auto result = uniqueNames.insert(pImpl->_boneEntries.at(i).name().c_str());
            if (!result.second)
            {
                return false;
            }
        }

        return true;
    }

    bool SkeletonRig::validateParentIndices()
    {
        for (int i = 0; i < pImpl->_boneEntries.size(); i++)
        {
            if (pImpl->_boneEntries.at(i).parentBoneIndex() >= i)
            {
                //tempOutput.setErrorValue(std::format("Bone Index: {} Has Parent Index: {}. Parent Index Cannot Be Greater Than Or Equal To Bone's Index", i, pImpl->_boneEntries.at(i).GetParentBoneIndex()).c_str());
                return false;
            }
        }
        return true;
    }

    bool SkeletonRig::addBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, Utilities::StringContainer boneName, Utilities::StringContainer parentName, bool localValues)
    {
        int newParentIndex = 0;
        if (pImpl->_boneEntries.size() == 0)
        {
            newParentIndex = -1;
        }
        else
        {
            for (unsigned int i = 0; i < pImpl->_boneEntries.size(); i++)
            {
                if (pImpl->_boneEntries.at(i).name().compare(parentName, false) == 0)
                {
                    newParentIndex = i;
                    break;
                }
            }
        }
        return addBoneToRig(rotation,position,boneName,newParentIndex, localValues);
    }

    bool SkeletonRig::addBoneToRig(CALUMI::Math::Quaternion rotation, CALUMI::Math::Vector3 position, Utilities::StringContainer boneName, int parentIndex, bool localValues)
    {
        if (pImpl->_boneEntries.size() >= MaxBoneCount)
        {
            std::println("[CALUMI.Animation API] Bone Count Limit Reached For Rig: {}", pImpl->_rigName.c_str());
            return false;
        }

        if (boneName == "")
        {
            std::println("[CALUMI.Animation API] Bone Must Have Valid Bone Name");
            return false;
        }

        for (uint64_t i = 0; i < pImpl->_boneEntries.size(); i++)
        {
            if (boneName.compare(pImpl->_boneEntries.at(i).name(), false) == 0)
            {
                std::println("[CALUMI.Animation API] Bone Name Must Be Unique");
                return false;
            }
        }

        SkeletonBone input;
        input.name() = boneName;
        input.setParentBoneIndex(parentIndex);

        CALUMI::Math::Quaternion rootRotation;
        CALUMI::Math::Quaternion localRotation;
        CALUMI::Math::Vector3 rootPosition;
        CALUMI::Math::Vector3 localPosition;
        if (pImpl->_boneEntries.empty())
        {
            rootRotation = rotation;
            localRotation = rotation;
            rootPosition = position;
            localPosition = position;
        }
        else if (localValues)
        {
            rootRotation = rotation * pImpl->_boneEntries.at(input.parentBoneIndex()).globalRotation();
            localRotation = rotation;
            rootPosition = position + pImpl->_boneEntries.at(input.parentBoneIndex()).globalPosition();
            localPosition = position;
        }
        else
        {
            CALUMI::Math::Quaternion inverseParentRoot;
            pImpl->_boneEntries.at(input.parentBoneIndex()).globalRotation().inverse(inverseParentRoot);
            localRotation = rotation * inverseParentRoot;
            rootRotation = rotation;
            localPosition = position - pImpl->_boneEntries.at(input.parentBoneIndex()).globalPosition();
            rootPosition = position;
        }
        
        input.setPosition(localPosition, rootPosition);
        input.setRotation(localRotation, rootRotation);

        input.resetMirrorBoneIndex();

        pImpl->_boneEntries.push_back(input);

        return true;
    }
    bool SkeletonRig::renameBone(const char* oldBoneName, const char* newBoneName)
    {
        auto result = boneIndex(oldBoneName);
        if(result < 0)
            return false;

        return renameBone(result, newBoneName);
    }
    bool SkeletonRig::renameBone(uint64_t boneIndex, const char* newBoneName)
    {
        Utilities::StringContainer oldName = pImpl->_boneEntries.at(boneIndex).name();
        pImpl->_boneEntries.at(boneIndex).name() = newBoneName;
        return pImpl->_rigPackageManager.onBoneRename(oldName.c_str(), newBoneName, boneIndex);
    }
    bool SkeletonRig::createBoneMirrorPair(int i1, int i2)
    {
        if(i1 >= pImpl->_boneEntries.size() || i2 >= pImpl->_boneEntries.size())
            return false;

        pImpl->_boneEntries.at(i1).setMirrorBoneIndex(i2);
        pImpl->_boneEntries.at(i2).setMirrorBoneIndex(i1);
        return true;
    }
    bool SkeletonRig::resetAllBoneMirrors()
    {
        for (unsigned int i = 0; i < pImpl->_boneEntries.size(); i++)
        {
            pImpl->_boneEntries.at(i).resetMirrorBoneIndex();
        }
        return true;
    }

    bool SkeletonRig::verifyExclusiveBoneMirrors()
    {
        for (unsigned int i = 0; i < pImpl->_boneEntries.size(); i++)
        {
            int mirror = pImpl->_boneEntries.at(i).mirrorBoneIndex();
            if (mirror == -1)
            {
                //skip as we have declared this as having no mirror
            }
            else
            {
                if (mirror != i && mirror >= 0)
                {
                    if (i != pImpl->_boneEntries.at(mirror).mirrorBoneIndex())
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    uint64_t UNIV::SkeletonRig::animatedBoneCount()
    {
        unsigned int AnimatedBoneCount = 0;

        for (unsigned int i = 0; i < pImpl->_boneEntries.size(); i++)
        {
            if (pImpl->_boneEntries.at(i).boneTypeProperty()->getType() == UNIV::BoneType::Default)
            {
                AnimatedBoneCount++;
            }
        }

        return AnimatedBoneCount;
    }

    uint64_t SkeletonRig::boneCount() const
    {
        return pImpl->_boneEntries.size();
    }

    int SkeletonRig::boneIndex(Utilities::StringContainer boneName) const
    {
        int output = -1;
        if (boneName == "") return output;

        for (uint64_t i = 0; i < pImpl->_boneEntries.size() && i < MaxBoneCount; i++)
        {
            if (pImpl->_boneEntries.at(i).name() == boneName)
            {
                output = static_cast<int>(i);
                break;
            }
        }
        return output;

    }

    Utilities::StringContainer SkeletonRig::toJSON(const uint64_t indents = 0) const {
        Utilities::StringContainer output = Utilities::Indent(indents).c_str();
        output += "{\n";
        output += std::format("{0}\"rigName\":\"{1}\",\n{0}\"boneEntries\":", Utilities::Indent(indents + 1).c_str(), pImpl->_rigName.c_str()).c_str();
        // output += Utilities::VectorToJSON(pImpl->_boneEntries,indents + 1);
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

            return boneTypeProperties->getTypeString();
        }

    public:
        CALUMI::Math::Quaternion localRotation;
        CALUMI::Math::Quaternion globalRotation;
        CALUMI::Math::Vector3 localPosition;
        CALUMI::Math::Vector3 globalPosition;
        Utilities::StringContainer name;
    };

    const Math::Quaternion& SkeletonBone::localRotation() const { return pImpl->localRotation; }
    const Math::Quaternion& SkeletonBone::globalRotation() const { return pImpl->globalRotation; }
    const Math::Vector3& SkeletonBone::localPosition() const { return pImpl->localPosition; }
    const Math::Vector3& SkeletonBone::globalPosition() const { return pImpl->globalPosition; }
    void SkeletonBone::setRotation(Math::Quaternion& local, Math::Quaternion& global)
    {
        pImpl->localRotation = local;
        pImpl->globalRotation = global;
    }
    void SkeletonBone::setPosition(Math::Vector3& local, Math::Vector3& global)
    {
        pImpl->localPosition = local;
        pImpl->globalPosition = global;
    }
    Utilities::StringContainer& SkeletonBone::name() const { return pImpl->name; }

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

    bool UNIV::SkeletonBone::setBoneTypeProperty(UNIV::BoneType boneType, bool resetExisting)
    {

        if (pImpl->boneTypeProperties.get())
        {
            if (!resetExisting && boneType == pImpl->boneTypeProperties->getType())
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

    const BoneTypeProperties* SkeletonBone::boneTypeProperty()
    {
        if (pImpl->boneTypeProperties.get())
        {
            return pImpl->boneTypeProperties.get();
        }

        resetBoneTypeProperty();
        return pImpl->boneTypeProperties.get();
    }

    bool UNIV::SkeletonBone::resetBoneTypeProperty(UNIV::BoneType boneType)
    {
        return this->setBoneTypeProperty(boneType, true);
    }

    void SkeletonBone::setMirrorBoneIndex(int idx) { pImpl->mirrorBoneIndex = idx; }

    void SkeletonBone::resetMirrorBoneIndex() { pImpl->mirrorBoneIndex = -1; }

    int SkeletonBone::mirrorBoneIndex() const { return pImpl->mirrorBoneIndex; }

    void SkeletonBone::setParentBoneIndex(int idx) { pImpl->parentBoneIndex = idx; }

    void SkeletonBone::resetParentBoneIndex() { pImpl->parentBoneIndex = 0; }

    void SkeletonBone::setAsRootBone() { pImpl->parentBoneIndex = -1; }

    int SkeletonBone::parentBoneIndex() const { return pImpl->parentBoneIndex; }

    Utilities::StringContainer SkeletonBone::toJSON(const uint64_t indents = 0) const {

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

#pragma region EXTERN"C"

    SkeletonRig* CreateSkeletonRigC(const char* _rigName)
    {
        SkeletonRig* outputSkeletonRig = new SkeletonRig(_rigName);
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
        errorMessageHolder->clear();

        if (std::string(boneName) == "")
        {
            *errorMessageHolder += "[CALUMI.Animation API] Bone Entry Must Have Bone Name!\n";
            return false;
        }

        float w = rotationW;
        
        if (rotationX == 0.0 && rotationY == 0.0 && rotationZ == 0.0)
        {
            w = 1.0;
        }

        CALUMI::Math::Quaternion q1 = { rotationX, rotationY, rotationZ, w };
        q1.normalize();

        bool result = rig->addBoneToRig(q1, { positionX,positionY,positionZ }, boneName, parentIndex, usingLocalValues);

        if (!result)
            *errorMessageHolder += std::format("[CALUMI.Animation API] Failure When Adding Bone: {} To Rig.\n", boneName).c_str();
        else
            *errorMessageHolder += std::format("[CALUMI.Animation API] {} Added To Rig Successfully!\n", boneName).c_str();

        return result;
    }
    bool AddBoneToSkeletonRigWithEulerC(SkeletonRig* rig, float rotationX, float rotationY, float rotationZ, uint8_t order, float positionX, float positionY, float positionZ, const char* boneName, int parentIndex, bool usingLocalValues, Utilities::StringContainer* errorMessage)
    {
        Math::Quaternion::EulerOrder eOrder = order > static_cast<uint8_t>(Math::Quaternion::EulerOrder::Max) ? Math::Quaternion::EulerOrder::XYZ : static_cast<Math::Quaternion::EulerOrder>(order);

        Math::Quaternion rotation(rotationX, rotationY, rotationZ,eOrder );
        rotation.normalize();

        return AddBoneToSkeletonRigC(rig, rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW(), positionX, positionY, positionZ, boneName, parentIndex, usingLocalValues, errorMessage);
    }
    bool SetBoneTypeC(SkeletonBone* bone, uint32_t boneType)
    {
        return bone->setBoneTypeProperty(static_cast<UNIV::BoneType>(boneType));
    }
    bool SetBoneTypeFromStringC(SkeletonBone* bone, const char* boneStr)
    {
        return bone->setBoneTypeProperty(BoneTypeFromString(boneStr));
    }
    uint32_t GetBoneTypeC(SkeletonBone* bone)
    {
        return uint32_t(bone->boneTypeProperty()->getType());
    }
    const char* GetBoneTypeAsStringC(SkeletonBone* bone)
    {
        return bone->boneTypeProperty()->getTypeString();
    }
    bool SetTwistBonePropertiesC(SkeletonBone* bone, bool reassign, int32_t twistDriverIndex, float twistDriverWeight, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (bone->boneTypeProperty()->getType() != UNIV::BoneType::Twist)
        {
            if (!reassign)
            {
                *errorMessageHolder += std::format("[CALUMI.Animation API] Bone: {}'s Type Does Not Match Desired Values, Reassigned Set To False. No changes have been made.\n", bone->name().c_str()).c_str();
                return false;
            }

            if (!bone->setBoneTypeProperty(UNIV::BoneType::Twist))
            {
                *errorMessageHolder += std::format("[CALUMI.Animation API] Bone Type Could Not Be Set For Bone: {} \n", bone->name().c_str()).c_str();
                return false;
            }
        }

        if (twistDriverIndex < 0)
        {
            *errorMessageHolder += std::format("[CALUMI.Animation API] Desired Twist Driver Index For Bone: {} is set to non-value or negative, did you mean to set this bone to Default?\n", bone->name().c_str()).c_str();
            return false;
        }

        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->boneTypeProperty()));
        tProp->setTwistDriverIndex(twistDriverIndex);
        tProp->setTwistDriverWeight(twistDriverWeight);

        return true;
    }
    int GetTwistBoneDriverIndexC(SkeletonBone* bone, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (bone->boneTypeProperty()->getType() != UNIV::BoneType::Twist)
        {
            *errorMessageHolder += std::format("[CALUMI.Animation API] Bone: %s is not set to Twist Type, returning -1\n", bone->name().c_str()).c_str();
            return -1;
        }

        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->boneTypeProperty()));

        return tProp->twistDriverIndex();
    }
    float GetTwistBoneDriverWeightC(SkeletonBone* bone, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (bone->boneTypeProperty()->getType() != UNIV::BoneType::Twist)
        {
            *errorMessageHolder += std::format("[CALUMI.Animation API] Bone: %s is not set to Twist Type, returning NaN Float\n", bone->name().c_str()).c_str();
            return std::numeric_limits<float>::quiet_NaN();
        }

        TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->boneTypeProperty()));
        return tProp->twistDriverWeight();
    }
    int SetMirrorIndexC(SkeletonBone* bone, int index)
    {
        bone->setMirrorBoneIndex(index);
        return bone->mirrorBoneIndex();
    }
    int GetMirrorIndexC(SkeletonBone* bone)
    {
        return bone->mirrorBoneIndex();
    }
    bool CreateBoneMirrorPairC(SkeletonRig* rig, int index1, int index2)
    {
        return rig->createBoneMirrorPair(index1, index2);
    }
    bool ResetAllBoneMirrorsC(SkeletonRig* rig)
    {
        return rig->resetAllBoneMirrors();
    }
    bool VerifyExclusiveBoneMirrorsC(SkeletonRig* rig)
    {
        return rig->verifyExclusiveBoneMirrors();
    }
    uint64_t GetSkeletonRigBoneCountC(SkeletonRig* source)
    {
        return source->boneCount();
    }
    uint64_t GetSkeletonRigAnimatedBoneCountC(SkeletonRig* source)
    {
        return source->animatedBoneCount();
    }
    const char* GetSkeletonRigNameC(SkeletonRig* source)
    {
        return source->rigName().c_str();
    }
    SkeletonBone* GetSkeletonBoneC(SkeletonRig* source, int index, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (source->boneEntries().size() <= index)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
            return nullptr;
        }
        if (index < 0)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Index Is Negative! Is This Parent Index Coming From The Root?";
            return nullptr;
        }
        return &source->boneEntries().at(index);
    }
    const char* GetSkeletonBoneNameC(SkeletonBone* source)
    {
        return source->name().c_str();
    }
    int GetSkeletonBoneParentIndexC(SkeletonBone* source)
    {
        return source->parentBoneIndex();
    }
    const CALUMI::Math::Quaternion* GetSkeletonBoneRotationC(SkeletonBone* source, bool fromRoot)
    {
        if (fromRoot)
        {
            return &source->globalRotation();
        }

        return &source->localRotation();
    }
    const CALUMI::Math::Vector3* GetSkeletonBonePositionC(SkeletonBone* source, bool fromRoot)
    {
        if (fromRoot)
        {
            return &source->globalPosition();
        }

        return &source->localPosition();
    }
    bool ValidateSkeletonRigNamesC(SkeletonRig* source)
    {
        return source->validateNames();
    }
    bool ValidateSkeletonRigParentIndicesC(SkeletonRig* source)
    {
        return source->validateParentIndices();        
    }
#pragma endregion

    VECTORDEF(SkeletonBoneVector, SkeletonBone)

}}

