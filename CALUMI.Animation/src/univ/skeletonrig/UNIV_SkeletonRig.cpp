//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalvectordef.h"
#include "internalplatform.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"
#include "utilities/CALUMI_Utilities.h"
#include <set>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <ostream>
#include <unordered_set>


namespace CALUMI::UNIV{


#pragma region SKELETONBONE
    struct SkeletonBone::Impl
    {
    public:
        std::shared_ptr<BoneTypeProperties> boneTypeProperties = std::make_shared<DefaultBoneProperties>();
        std::string _parentBone;
        Math::Quaternion globalRotation;
        Math::Vector3 globalPosition;
        Utilities::StringContainer name;
    };

    SkeletonBone::SkeletonBone()
    {
        pImpl = new Impl;
    }
    SkeletonBone::SkeletonBone(const SkeletonBone& other) : SkeletonBone()
    {
        *this = other;
    }
    SkeletonBone::~SkeletonBone()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }
    SkeletonBone& SkeletonBone::operator=(const SkeletonBone& other)
    {
        if (this != &other)
        {
            pImpl->_parentBone = other.pImpl->_parentBone;
            pImpl->name = other.pImpl->name;
            pImpl->globalPosition = other.pImpl->globalPosition;
            pImpl->globalRotation = other.pImpl->globalRotation;

            if (setBoneTypeProperty(other.pImpl->boneTypeProperties->getType(), true))
                *pImpl->boneTypeProperties = *other.pImpl->boneTypeProperties;
        }
        return *this;
    }
    //const Math::Quaternion& SkeletonBone::localRotation() const { return pImpl->localRotation; }
    const Math::Quaternion& SkeletonBone::globalRotation() const { return pImpl->globalRotation; }

    //const Math::Vector3& SkeletonBone::localPosition() const { return pImpl->localPosition; }
    const Math::Vector3& SkeletonBone::globalPosition() const { return pImpl->globalPosition; }
    void SkeletonBone::setRotation(const Math::Quaternion& global) const
    {
        pImpl->globalRotation = global;
    }
    void SkeletonBone::setPosition(const Math::Vector3 & global) const
    {
        pImpl->globalPosition = global;
    }
    const char* SkeletonBone::name() const
    {
        return pImpl->name.c_str();
    }

    bool SkeletonBone::setBoneTypeProperty(const BoneType boneType, const bool resetExisting) const
    {
        if (pImpl->boneTypeProperties)
        {
            if (!resetExisting && boneType == pImpl->boneTypeProperties->getType())
                return false;
        }

        switch (boneType)
        {
        case BoneType::Default:
            pImpl->boneTypeProperties = std::make_shared<DefaultBoneProperties>();
            break;
        case BoneType::Twist:
            pImpl->boneTypeProperties = std::make_shared<TwistBoneProperties>();
            break;
        default:
            return false;
        }
        return true;
    }

    const BoneTypeProperties* SkeletonBone::boneTypeProperty() const
    {
        return pImpl->boneTypeProperties.get();
    }

    bool SkeletonBone::resetBoneTypeProperty(const BoneType boneType) const
    {
        return this->setBoneTypeProperty(boneType, true);
    }

    void SkeletonBone::setParentBone(const char* name) const
    {
        if (SCOMPARE(name, pImpl->name.c_str()) == 0)
            return;

        pImpl->_parentBone = name;
    }

    const char* SkeletonBone::parentBone() const
    {
        return pImpl->_parentBone.c_str();
    }

    Utilities::StringContainer SkeletonBone::toJSON(const uint64_t indents = 0) const {

        const std::string boneTypeOutput = pImpl->boneTypeProperties == nullptr ? "ERROR" : pImpl->boneTypeProperties->getTypeString();

        std::string output = (
            Utilities::Indent(indents) + "{\n" +
            Utilities::Indent(indents+1).c_str() + "\"name\":" + pImpl->name.c_str() + "\",\n" +
            Utilities::Indent(indents+1).c_str() + "\"parentBone\":" + pImpl->_parentBone.c_str() + ",\n" +
            Utilities::Indent(indents + 1).c_str() + "\"boneType\":" + boneTypeOutput.c_str() + ",\n" //+
            //Utilities::Indent(indents + 1).c_str() + "\"mirrorBoneIndex\":" + std::to_string(pImpl->mirrorBoneIndex).c_str() + ",\n"
            ).c_str();

        //output += (Utilities::Indent(indents+1) + "\"localRotation\": [").c_str();
        //output += std::to_string(pImpl->localRotation.getX()) + ", " + std::to_string(pImpl->localRotation.getY()) + ", " + std::to_string(pImpl->localRotation.getZ()) + ", " + std::to_string(pImpl->localRotation.getW()) + "],\n";

        output += (Utilities::Indent(indents + 1) + "\"globalRotation\": [").c_str();
        output += std::to_string(pImpl->globalRotation.x()) + ", " + std::to_string(pImpl->globalRotation.y()) + ", " + std::to_string(pImpl->globalRotation.z()) + ", " + std::to_string(pImpl->globalRotation.w()) + "],\n";

        //output += (Utilities::Indent(indents + 1) + "\"localPosition\": [").c_str();
        //output += std::to_string(pImpl->localPosition.getX()) + ", " + std::to_string(pImpl->localPosition.getY()) + ", " + std::to_string(pImpl->localPosition.getZ()) + "],\n";

        output += (Utilities::Indent(indents + 1) + "\"globalPosition\": [").c_str();
        output += std::to_string(pImpl->globalPosition.x()) + ", " + std::to_string(pImpl->globalPosition.y()) + ", " + std::to_string(pImpl->globalPosition.z()) + "]\n";

        output += (Utilities::Indent(indents) + "}").c_str();
        return output.c_str();
    }
#pragma endregion

    VECTORDEF(SkeletonBoneVector, SkeletonBone)

#pragma region SKELETONRIG

    struct SkeletonRig::Impl
    {
        /// <summary>
        /// Optional name for the skeleton rig.
        /// </summary>
        std::string _rigName = "MySkeletonRig";
        SkeletonBoneVector _boneEntries;
        RigPackageManager _rigPackageManager;
        std::unordered_map<std::string, std::string> _mirrors;
        Impl() = default;
    };

    const char* SkeletonRig::name() const { return pImpl->_rigName.c_str(); }

    bool SkeletonRig::setName(const char* name) const
    {
        if (SCOMPARE(name, "") == 0)
            return false;

        pImpl->_rigName = name;
        return true;
    }

    const SkeletonBoneVector& SkeletonRig::boneEntries() const { return pImpl->_boneEntries; }
    SkeletonBone* SkeletonRig::bone(const char* boneName) const
    {
        const auto idx = findBone(boneName);

        if(idx < 0)
            return nullptr;

        return &pImpl->_boneEntries.at(idx);
    }
    RigPackageManager& SkeletonRig::getPackageManager() const { return pImpl->_rigPackageManager; }


    SkeletonRig::SkeletonRig()
    {
        pImpl = new Impl;
    }
    SkeletonRig::SkeletonRig(const SkeletonRig& input) : SkeletonRig()
    {
        *pImpl = *input.pImpl;
    }
    SkeletonRig::~SkeletonRig()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }
    SkeletonRig::SkeletonRig(const Utilities::StringContainer& _rigName) : SkeletonRig()
    {
        pImpl->_rigName = _rigName.c_str();
    }
    SkeletonRig::SkeletonRig(const char* _rigName) : SkeletonRig()
    {
        pImpl->_rigName = _rigName;
    }

    bool SkeletonRig::validateNames() const
    {
        std::set<std::string> uniqueNames;

        for (unsigned int i = 0; i < pImpl->_boneEntries.size(); i++)
        {
            if (const auto [it, result] = uniqueNames.insert(pImpl->_boneEntries.at(i).name()); !result)
            {
                return false;
            }
        }

        return true;
    }

    SkeletonBone* SkeletonRig::addBoneToRig(const Math::Quaternion& rotation, const Math::Vector3& position, const char* boneName, const char* parentName, const bool relativeToParent) const
    {
        if (pImpl->_boneEntries.size() >= MaxBoneCount)
            return nullptr;

        if (SCOMPARE(boneName, "") == 0)
            return nullptr;

        if (SCOMPARE(boneName, parentName) == 0)
            return nullptr;

        if (findBone(boneName) >= 0)
            return nullptr;

        const SkeletonBone input;
        input.pImpl->name = boneName;
        input.setParentBone(parentName);

        if (const auto pIdx = findBone(parentName); pIdx >= 0 && relativeToParent)
        {
            auto qGlobal = rotation;
            qGlobal = pImpl->_boneEntries.at(pIdx).globalRotation() * qGlobal;
            input.setRotation(qGlobal);

            auto pGlobal = position;
            pGlobal += pImpl->_boneEntries.at(pIdx).globalPosition();
            input.setPosition(pGlobal);
        } else
        {
            input.setPosition(position);
            input.setRotation(rotation);
        }

        pImpl->_boneEntries.push_back(input);

        return &pImpl->_boneEntries.at(pImpl->_boneEntries.size()-1);
    }

    void SkeletonRig::shiftChildren(const SkeletonBone& bone, const Math::Vector3& posOffset, const Math::Quaternion& rotOffset) const
    {
        for (int i =0 ; i < pImpl->_boneEntries.size(); i++)
        {
            if (const auto& child = pImpl->_boneEntries.at(i); SCOMPARE(child.parentBone(), bone.name()) == 0)
            {
                Math::Vector3 newPos = child.globalPosition() + posOffset;
                Math::Quaternion newRot = child.globalRotation();

                newRot.rotateBy(rotOffset);
                child.setRotation(newRot);
                child.setPosition(newPos);

                shiftChildren(child, posOffset, rotOffset);
            }
        }
    }

    static bool s_HasParentLoop(const SkeletonRig& rig, const char* boneName, std::unordered_set<std::string>& parentNames)
    {
        if (const auto bone = rig.bone(boneName))
        {
            if (SCOMPARE(bone->parentBone(), "") ==0 )
                return false;

            if (SCOMPARE(bone->parentBone(), boneName) == 0)
                return true;

            if (parentNames.contains(bone->parentBone()))
                return true;

            if (parentNames.size() == rig.boneEntries().size())
                return true;

            parentNames.insert(bone->parentBone());

            return s_HasParentLoop(rig, bone->parentBone(), parentNames);
        }
        return false;
    }

    bool SkeletonRig::setRoot(const char* boneName, const bool keepRelative, const bool keepChildrenRelative) const
    {
        const auto idx = findBone(boneName);

        if (idx < 0)
            return false;

        //we store the previous globals for child relative calculations
        const Math::Quaternion prevRotation = pImpl->_boneEntries.at(idx).globalRotation();
        const Math::Vector3 prevPosition = pImpl->_boneEntries.at(idx).globalPosition();

        //if keep relative, then new global will be equal to current relative
        const Math::Quaternion newRotation = boneRotation(boneName, keepRelative);
        const Math::Vector3 newPosition = bonePosition(boneName, keepRelative);

        if (setBoneIndex(boneName, 0) != 0)
            return false;

        //the new index is set so we move forward
        pImpl->_boneEntries.at(0).setParentBone("");
        pImpl->_boneEntries.at(0).setRotation(newRotation);
        pImpl->_boneEntries.at(0).setPosition(newPosition);

        if (!keepChildrenRelative)
            return true;

        if (std::unordered_set<std::string> parentNames; s_HasParentLoop(*this, boneName, parentNames))
            return false;

        shiftChildren(pImpl->_boneEntries.at(0),
                      newPosition - prevPosition,
                      Math::Quaternion::rotationOffset(prevRotation, newRotation));
        return true;
    }

    bool SkeletonRig::setBoneParent(const char* boneName, const char* parentName, const bool keepRelative, const bool keepChildrenRelative) const
    {
        const int idx = findBone(boneName);
        const int pIdx = findBone(parentName);

        if (idx < 0 || pIdx < 0 || pIdx == idx)
            return false;

        const Math::Quaternion pgRotation = boneRotation(boneName, false);
        const Math::Vector3 pgPosition = bonePosition(boneName, false);
        Math::Quaternion rotation = boneRotation(boneName, keepRelative);
        Math::Vector3 position = bonePosition(boneName, keepRelative);

        pImpl->_boneEntries.at(idx).setParentBone(parentName);

        if (!keepRelative)
            return true;

        position += pImpl->_boneEntries.at(pIdx).globalPosition();
        rotation.rotateBy(pImpl->_boneEntries.at(pIdx).globalRotation());

        pImpl->_boneEntries.at(idx).setPosition(position);
        pImpl->_boneEntries.at(idx).setRotation(rotation);

        if (!keepChildrenRelative)
            return true;

        if (std::unordered_set<std::string> parentNames; s_HasParentLoop(*this, boneName, parentNames))
            return false;

        shiftChildren(pImpl->_boneEntries.at(idx),
                      position - pgPosition,
                      Math::Quaternion::rotationOffset(pgRotation, rotation));

        return true;
    }

    bool SkeletonRig::renameBone(const char* oldBoneName, const char* newBoneName) const
    {
        if (SCOMPARE(newBoneName, "") == 0)
            return false;

        const auto result = findBone(oldBoneName);
        const auto unqCheck = findBone(newBoneName);

        if(result < 0)
            return false;

        if (unqCheck >= 0)
            return false;

        pImpl->_boneEntries.at(result).pImpl->name = newBoneName;

        for (size_t i = 0; i < pImpl->_boneEntries.size(); i++)
        {
            if (SCOMPARE(pImpl->_boneEntries.at(i).parentBone(), oldBoneName) == 0)
                pImpl->_boneEntries.at(i).setParentBone(newBoneName);
        }

        return pImpl->_rigPackageManager.onBoneRename(oldBoneName, newBoneName);
    }
    bool SkeletonRig::createBoneMirrorPair(const char* bone1, const char* bone2) const
    {
        if(SCOMPARE(bone1, "") == 0 && SCOMPARE(bone2, "") == 0)
            return false;

        if (pImpl->_mirrors.contains(bone1))
        {
            if (const char* prev = pImpl->_mirrors[bone1].c_str(); pImpl->_mirrors.contains(prev))
                pImpl->_mirrors[prev] = "";
        }

        if (pImpl->_mirrors.contains(bone2))
        {
            if (const char* prev = pImpl->_mirrors[bone2].c_str(); pImpl->_mirrors.contains(prev))
            {
                pImpl->_mirrors[prev] = "";
            }
        }

        if (SCOMPARE(bone1, "") != 0)
        {
            pImpl->_mirrors[bone1] = bone2;
        }

        if (SCOMPARE(bone2, "") != 0)
        {
            pImpl->_mirrors[bone2] = bone1;
        }

        return true;
    }
    bool SkeletonRig::resetAllBoneMirrors() const
    {
        pImpl->_mirrors.clear();

        return pImpl->_mirrors.empty();
    }

    bool SkeletonRig::removeBone(const char* boneName) const
    {
        const int idx = findBone(boneName);
        const uint64_t size = pImpl->_boneEntries.size();

        if(idx < 0)
            return false;

        pImpl->_boneEntries.erase(idx);

        if (pImpl->_mirrors.contains(boneName))
            pImpl->_mirrors.erase(boneName);

        std::erase_if(pImpl->_mirrors, [boneName](const auto& item)
        {
            return SCOMPARE(item.second.c_str(), boneName) == 0;
        });

        return pImpl->_boneEntries.size() != size;
    }

    Math::Quaternion SkeletonRig::boneRotation(const char* boneName, const bool relativeToParent) const
    {
        Math::Quaternion rotation;

        const int idx = findBone(boneName);

        if (idx < 0)
            return rotation;

        rotation = pImpl->_boneEntries.at(idx).globalRotation();

        if (!relativeToParent)
            return rotation;

        const int pIdx = findBone(pImpl->_boneEntries.at(idx).parentBone());

        if (pIdx < 0)
            return rotation;

        return Math::Quaternion::rotationOffset(pImpl->_boneEntries.at(pIdx).globalRotation(), rotation);
    }

    Math::Vector3 SkeletonRig::bonePosition(const char* boneName, const bool relativeToParent) const
    {
        const int idx = findBone(boneName);

        if (idx < 0)
            return Math::Vector3::Zero;

        Math::Vector3 position = pImpl->_boneEntries.at(idx).globalPosition();

        if (!relativeToParent)
            return position;

        const int pIdx = findBone(pImpl->_boneEntries.at(idx).parentBone());

        if (pIdx < 0)
            return position;

        return position - pImpl->_boneEntries.at(pIdx).globalPosition();
    }

    int SkeletonRig::setBoneIndex(const char* boneName, const int index) const
    {
        const int prevIndex = findBone(boneName);
        int newIndex = 0;

        if (prevIndex < 0)
            return -1;

        if(index > -1)
            newIndex = index;

        if (newIndex >= pImpl->_boneEntries.size())
            newIndex = static_cast<int>(pImpl->_boneEntries.size() - 1);

        const auto copy = pImpl->_boneEntries.at(prevIndex);

        const auto pSize = pImpl->_boneEntries.size();
        pImpl->_boneEntries.erase(prevIndex);

        if (pSize == pImpl->_boneEntries.size())
            return prevIndex;

        pImpl->_boneEntries.insert_r(newIndex, copy);

        if (pSize != pImpl->_boneEntries.size())
        {
            pImpl->_boneEntries.insert_r(prevIndex, copy);
            return prevIndex;
        }

        if (SCOMPARE(pImpl->_boneEntries.at(newIndex).name(), copy.name()) == 0)
            return newIndex;

        for (int i = 0; i < pImpl->_boneEntries.size(); i++)
        {
            if (SCOMPARE(pImpl->_boneEntries.at(i).name(), copy.name()) == 0)
                return i;
        }

        return -1;
    }

    bool SkeletonRig::verifyExclusiveBoneMirrors() const
    {
        for (unsigned int i = 0; i < pImpl->_boneEntries.size(); i++)
        {
            const auto bone1 = pImpl->_boneEntries.at(i).name();

            if (!pImpl->_mirrors.contains(bone1))
                continue;

            const auto bone2 = pImpl->_mirrors[bone1].c_str();

            if (SCOMPARE(bone2, "") == 0)
                continue;

            if (!pImpl->_mirrors.contains(bone2))
                return false;
            
            if (SCOMPARE(pImpl->_mirrors[bone2].c_str(), bone1) != 0)
                return false;
        }

        return true;
    }

    const char* SkeletonRig::getBoneMirrorName(const char* boneName) const
    {
        if (const int idx = findBone(boneName); idx >= 0)
        {
            if (const char* bone = pImpl->_boneEntries.at(idx).name(); pImpl->_mirrors.contains(bone))
                return pImpl->_mirrors[bone].c_str();
        }

        return "";
    }

    uint64_t SkeletonRig::animatedBoneCount() const
    {
        unsigned int AnimatedBoneCount = 0;

        for (unsigned int i = 0; i < pImpl->_boneEntries.size(); i++)
        {
            if (pImpl->_boneEntries.at(i).boneTypeProperty()->getType() == BoneType::Default)
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

    Utilities::StringContainer SkeletonRig::toJSON(const uint64_t indents = 0) const {
        Utilities::StringContainer output = Utilities::Indent(indents).c_str();
        output += "{\n";
        output += std::format("{0}\"rigName\":\"{1}\",\n{0}\"boneEntries\":", Utilities::Indent(indents + 1).c_str(), pImpl->_rigName).c_str();
        // output += Utilities::VectorToJSON(pImpl->_boneEntries,indents + 1);
        output += "\n ";
        output += Utilities::Indent(indents).c_str();
        output += "}";
        return output;
    }

    
    int SkeletonRig::findBone(const char* name) const
    {
        if (SCOMPARE(name, "") == 0)
            return -1;

        for (size_t i = 0; i < pImpl->_boneEntries.size() && i < std::numeric_limits<int>::max(); i++)
        {
            if (SCOMPARE(pImpl->_boneEntries.at(i).name(), name) == 0)
                return static_cast<int>(i);
        }

        return -1;
    }
    int SkeletonRig::findBoneParent(const char* name) const
    {
        if (SCOMPARE(name, "") == 0)
            return -1;

        const int idx = findBone(name);

        if (idx < 0)
            return -1;

        const char* parentName = pImpl->_boneEntries.at(idx).parentBone();

        for (size_t i = 0; i < pImpl->_boneEntries.size() && i < std::numeric_limits<int>::max(); i++)
        {
            if (SCOMPARE(pImpl->_boneEntries.at(i).name(), parentName) == 0)
                return static_cast<int>(i);
        }

        return -1;
    }

    SkeletonRig& SkeletonRig::operator=(const SkeletonRig& other)
    {
        if (this != &other)
        {
            *pImpl = *other.pImpl;
        }
        return *this;
    }
#pragma endregion

#pragma region EXTERN"C"

    SkeletonRig* CreateSkeletonRigC(const char* _rigName)
    {
        auto* outputSkeletonRig = new SkeletonRig(_rigName);
        return outputSkeletonRig;
    }
    bool DeleteSkeletonRigC(const SkeletonRig* ptr)
    {
        if (ptr)
        {
            delete ptr;
            ptr = nullptr;
            return true;
        }
        return false;
    }
    SkeletonBone* AddBoneToSkeletonRigC(const SkeletonRig* rig, const float rotationX, const float rotationY, const float rotationZ, const float rotationW, float positionX, float positionY, float positionZ, const char* boneName, const char* parentName, const bool usingLocalValues, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (SCOMPARE(boneName, "") == 0)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Bone Entry Must Have Bone Name!\n";
            return nullptr;
        }

        float w = rotationW;
        
        if (rotationX == 0.0 && rotationY == 0.0 && rotationZ == 0.0)
        {
            w = 1.0;
        }

        Math::Quaternion q1 = { rotationX, rotationY, rotationZ, w };
        q1.normalize();

        const auto result = rig->addBoneToRig(q1, { positionX,positionY,positionZ }, boneName, parentName, usingLocalValues);

        if (!result)
            *errorMessageHolder += std::format("[CALUMI.Animation API] Failure When Adding Bone: {} To Rig.\n", boneName).c_str();
        else
            *errorMessageHolder += std::format("[CALUMI.Animation API] {} Added To Rig Successfully!\n", boneName).c_str();

        return result;
    }
    bool AddBoneToSkeletonRigWithEulerC(const SkeletonRig* rig, const float rotationX, const float rotationY, const float rotationZ, uint8_t order, const float positionX, const float positionY, const float positionZ, const char* boneName, const char* parentName, const bool usingLocalValues, Utilities::StringContainer* errorMessage)
    {
        const Math::Quaternion::EulerOrder eOrder = order > static_cast<uint8_t>(Math::Quaternion::EulerOrder::Max) ? Math::Quaternion::EulerOrder::XYZ : static_cast<Math::Quaternion::EulerOrder>(order);

        Math::Quaternion rotation(rotationX, rotationY, rotationZ,eOrder );
        rotation.normalize();

        return AddBoneToSkeletonRigC(rig, rotation.x(), rotation.y(), rotation.z(), rotation.w(), positionX, positionY, positionZ, boneName, parentName, usingLocalValues, errorMessage);
    }
    bool SetBoneTypeC(const SkeletonBone* bone, uint32_t boneType)
    {
        return bone->setBoneTypeProperty(static_cast<BoneType>(boneType));
    }
    bool SetBoneTypeFromStringC(const SkeletonBone* bone, const char* boneType)
    {
        return bone->setBoneTypeProperty(BoneTypeFromString(boneType));
    }
    uint32_t GetBoneTypeC(const SkeletonBone* bone)
    {
        return static_cast<uint32_t>(bone->boneTypeProperty()->getType());
    }
    const char* GetBoneTypeAsStringC(const SkeletonBone* bone)
    {
        return bone->boneTypeProperty()->getTypeString();
    }
    bool SetTwistBonePropertiesC(const SkeletonBone* bone, const bool reassign, const char* twistDriver, const float twistDriverWeight, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (bone->boneTypeProperty()->getType() != BoneType::Twist)
        {
            if (!reassign)
            {
                *errorMessageHolder += std::format("[CALUMI.Animation API] Bone: {}'s Type Does Not Match Desired Values, Reassigned Set To False. No changes have been made.\n", bone->name()).c_str();
                return false;
            }

            if (!bone->setBoneTypeProperty(BoneType::Twist))
            {
                *errorMessageHolder += std::format("[CALUMI.Animation API] Bone Type Could Not Be Set For Bone: {} \n", bone->name()).c_str();
                return false;
            }
        }

        const TwistBoneProperties* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->boneTypeProperty()));
        tProp->setTwistDriver(twistDriver);
        tProp->setTwistDriverWeight(twistDriverWeight);

        return true;
    }
    const char* GetTwistBoneDriverC(const SkeletonBone* bone, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (bone->boneTypeProperty()->getType() != BoneType::Twist)
        {
            *errorMessageHolder += std::format("[CALUMI.Animation API] Bone: %s is not set to Twist Type, returning empty string\n", bone->name()).c_str();
            return "";
        }

        const auto* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->boneTypeProperty()));

        return tProp->twistDriver();
    }
    float GetTwistBoneDriverWeightC(const SkeletonBone* bone, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (bone->boneTypeProperty()->getType() != BoneType::Twist)
        {
            *errorMessageHolder += std::format("[CALUMI.Animation API] Bone: %s is not set to Twist Type, returning NaN Float\n", bone->name()).c_str();
            return std::numeric_limits<float>::quiet_NaN();
        }

        const auto* tProp = dynamic_cast<TwistBoneProperties*>(const_cast<BoneTypeProperties*>(bone->boneTypeProperty()));
        return tProp->twistDriverWeight();
    }
    bool CreateBoneMirrorPairC(const SkeletonRig* rig, const char* bone1, const char* bone2)
    {
        return rig->createBoneMirrorPair(bone1, bone2);
    }
    bool ResetBoneMirrorC(const SkeletonRig* rig, const char* boneName)
    {
        return rig->createBoneMirrorPair(boneName, "");
    }
    bool ResetAllBoneMirrorsC(const SkeletonRig* rig)
    {
        return rig->resetAllBoneMirrors();
    }
    bool VerifyExclusiveBoneMirrorsC(const SkeletonRig* rig)
    {
        return rig->verifyExclusiveBoneMirrors();
    }
    uint64_t GetSkeletonRigBoneCountC(const SkeletonRig* source)
    {
        return source->boneCount();
    }
    uint64_t GetSkeletonRigAnimatedBoneCountC(const SkeletonRig* source)
    {
        return source->animatedBoneCount();
    }
    const char* GetSkeletonRigNameC(const SkeletonRig* source)
    {
        return source->name();
    }
    SkeletonBone* GetSkeletonBoneC(const SkeletonRig* source, const char* boneName)
    {
        return source->bone(boneName);
    }
    const char* GetSkeletonBoneNameC(const SkeletonBone* source)
    {
        return source->name();
    }
    const char* GetSkeletonBoneParentC(const SkeletonBone* source)
    {
        return source->parentBone();
    }
    const Math::Quaternion* GetSkeletonBoneRotationC(const SkeletonBone* source)
    {
        return &source->globalRotation();
    }
    const Math::Vector3* GetSkeletonBonePositionC(const SkeletonBone* source)
    {
        return &source->globalPosition();
    }
    bool ValidateSkeletonRigNamesC(const SkeletonRig* source)
    {
        return source->validateNames();
    }
    Math::Quaternion* GetRelativeSkeletonBoneRotationFromNameC(const SkeletonRig* rig, const char* boneName)
    {
        const SkeletonBone* bone = rig->bone(boneName);

        if (!bone)
            return nullptr;

        auto* output = new Math::Quaternion(rig->boneRotation(bone->name(), true));

        return output;
    }

    bool RenameBoneC(const SkeletonRig* rig, const char* oldName, const char* newName)
    {
        return rig->renameBone(oldName, newName);
    }
    Math::Vector3* GetRelativeSkeletonBonePositionC(const SkeletonRig* rig, const char* boneName)
    {
        const SkeletonBone* bone = rig->bone(boneName);

        if (!bone)
            return nullptr;

        auto* output = new Math::Vector3(rig->bonePosition(bone->name(), true));

        return output;
    }

    bool RemoveBoneC(const SkeletonRig* rig, const char* boneName)
    {
        return rig->removeBone(boneName);
    }
    bool SetRootC(const SkeletonRig* rig, const char* boneName, const bool keepRelative, const bool keepChildrenRelative)
    {
        return rig->setRoot(boneName, keepRelative, keepChildrenRelative);
    }
    int FindBoneInRigC(const SkeletonRig* rig, const char* boneName)
    {
        return rig->findBone(boneName);
    }
    int FindBoneParentInRigC(const SkeletonRig* rig, const char* boneName)
    {
        return rig->findBoneParent(boneName);
    }

#pragma endregion

}



