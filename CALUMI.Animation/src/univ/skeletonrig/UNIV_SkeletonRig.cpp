//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalvectordef.h"
#include "internalplatform.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"
#include "utilities/CALUMI_Utilities.h"
#include <string>
#include <vector>
#include <memory>
#include <limits>


namespace CALUMI::UNIV
{
#pragma region SKELETONBONE
    struct SkeletonBone::Impl
    {
    public:
        std::shared_ptr<BoneTypeProperty> m_boneTypeProperties = std::make_shared<DefaultBoneProperty>();
        Math::Transform m_localTransform;
        std::string m_name;

        const ILineage* m_parent;
        std::vector<std::unique_ptr<SkeletonBone>> m_childBones;
        explicit Impl(const ILineage* parent) : m_parent(parent) {}

        [[nodiscard]] std::vector<std::string> lineageList() const
        {
            std::vector<std::string> output;
            for (const auto& bone : m_childBones)
            {
                output.emplace_back(bone->name());
                output.append_range(bone->pImpl->lineageList());
            }
            return output;
        }
    };

    SkeletonBone::SkeletonBone(const ILineage* parent, const char* name)
    {
        pImpl = new Impl(parent);
        pImpl->m_name = name;
    }
    SkeletonBone::~SkeletonBone()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    void SkeletonBone::DeleteBone(const SkeletonBone*& bone)
    {
        if (!bone)
            return;

        if (bone->isRoot())
            return;

        const auto parent = dynamic_cast<const SkeletonBone*>(bone->parent());

        if (!parent)
            return;

        for (uint32_t i = 0; i < parent->pImpl->m_childBones.size(); i++)
        {
            if (parent->pImpl->m_childBones.at(i).get() == bone)
            {
                parent->pImpl->m_childBones.erase(parent->pImpl->m_childBones.begin() + i);
                bone = nullptr;
                return;
            }
        }
    }

    const ILineage* SkeletonBone::parent() const
    {
        return pImpl->m_parent;
    }

    const SkeletonRig& SkeletonBone::parentRig() const
    {
        auto parentPtr = parent();
        uint64_t failSafe = 0;

        while (parentPtr->parent() != nullptr && failSafe < SkeletonRig::MaxBoneCount)
        {
            parentPtr = parentPtr->parent();
            failSafe++;
        }

        if (const auto rig = dynamic_cast<const SkeletonRig*>(parentPtr))
            return *rig;

        throw std::runtime_error("SkeletonBone::parentRig() cannot resolve lineage to find Rig.");
    }

    SkeletonBone* SkeletonBone::addChildBone(const char* name, const Math::Transform& transform) const
    {
        //check to see if name is empty
        if (SCOMPARE(name, "") == 0)
            return nullptr;

        //Check to see if name is unique
        if (const auto& rig = parentRig(); rig.bone(name))
            return nullptr;

        pImpl->m_childBones.push_back(std::unique_ptr<SkeletonBone>(new SkeletonBone(this, name)));

        auto bone = pImpl->m_childBones.back().get();

        //double check to make sure this is the correct child, if not, search for it in this bone's children list
        if (SCOMPARE(bone->name(), name) != 0)
            bone = childBone(name, true);

        //something went wrong
        if (!bone)
            return nullptr;

        bone->pImpl->m_localTransform = transform;

        return bone;
    }

    SkeletonBone* SkeletonBone::addChildBone(const char* name, const Math::Vector3& position, const Math::Quaternion& rotation) const
    {
        const Math::Transform transform(position, rotation);
        return addChildBone(name, transform);
    }

    SkeletonBone* SkeletonBone::childBone(const char* name, const bool includeDescendents) const
    {
        for (const auto& bone : pImpl->m_childBones)
        {
            if (SCOMPARE(bone->name(), name) == 0)
                return bone.get();

            const auto& childBone = bone->childBone(name, includeDescendents);

            if (childBone)
                return childBone;
        }

        return nullptr;
    }

    SkeletonBone* SkeletonBone::childBone(const unsigned int index) const
    {
        if (index >= pImpl->m_childBones.size())
            return nullptr;

        return pImpl->m_childBones[index].get();
    }

    unsigned int SkeletonBone::childBoneCount() const
    {
        unsigned int count = 0;
        for (const auto& bone : pImpl->m_childBones)
        {
            if (!bone)
                continue;

            count++;
        }

        return count;
    }

    unsigned int SkeletonBone::boneCount() const
    {
        unsigned int boneCount = 0;
        for (const auto& bone : pImpl->m_childBones)
        {
            if (!bone)
                continue;

            boneCount++;
            boneCount += bone->boneCount();
        }

        return boneCount;
    }

    unsigned int SkeletonBone::boneTypeCount(const BoneTypeProperty::BoneType type) const
    {
        unsigned int boneCount = 0;
        for (const auto& bone : pImpl->m_childBones)
        {
            if (!bone)
                continue;

            if (bone->pImpl->m_boneTypeProperties->getType() == type)
                boneCount++;

            boneCount += bone->boneTypeCount(type);
        }

        return boneCount;
    }

    const Math::Transform& SkeletonBone::localTransform() const
    {
        return pImpl->m_localTransform;
    }

    Math::Transform SkeletonBone::globalTransform() const
    {
        const auto parentBone = dynamic_cast<const SkeletonBone*>(parent());

        if (!parentBone)
            return pImpl->m_localTransform;

        return pImpl->m_localTransform.global(parentBone->globalTransform());
    }

    void SkeletonBone::setLocalTransform(const Math::Vector3& position, const Math::Quaternion& rotation) const
    {
        pImpl->m_localTransform = {position, rotation};
    }

    const char* SkeletonBone::name() const
    {
        return pImpl->m_name.c_str();
    }

    bool SkeletonBone::isValidName(const char* name) const
    {
        if (!name)
            return false;

        if (SCOMPARE(name, "") == 0)
            return false;

        if (const auto& rig = parentRig(); rig.bone(name))
            return false;

        return true;
    }

    bool SkeletonBone::setName(const char* name) const
    {
        if (!isValidName(name))
            return false;

        pImpl->m_name = name;
        return true;
    }

    bool SkeletonBone::isRoot() const
    {
        const auto& rig = parentRig();

        return this == rig.root();
    }

    bool SkeletonBone::setBoneTypeProperty(const BoneTypeProperty::BoneType boneType, const bool resetExisting) const
    {
        if (pImpl->m_boneTypeProperties)
        {
            if (!resetExisting && boneType == pImpl->m_boneTypeProperties->getType())
                return false;
        }

        switch (boneType)
        {
        case BoneTypeProperty::BoneType::Default:
            pImpl->m_boneTypeProperties = std::make_shared<DefaultBoneProperty>();
            break;
        case BoneTypeProperty::BoneType::Twist:
            pImpl->m_boneTypeProperties = std::make_shared<TwistBoneProperty>();
            break;
        default:
            return false;
        }
        return true;
    }

    const BoneTypeProperty* SkeletonBone::boneTypeProperty() const
    {
        return pImpl->m_boneTypeProperties.get();
    }

    bool SkeletonBone::resetBoneTypeProperty(const BoneTypeProperty::BoneType boneType) const
    {
        return this->setBoneTypeProperty(boneType, true);
    }

    bool SkeletonBone::setParentBone(const SkeletonBone& newParent) const
    {
        const auto parentBone = dynamic_cast<const SkeletonBone*>(this->parent());

        //Something went wrong or this is the root of a skeleton rig and cannot be reparented
        if (!parentBone)
            return false;

        //We don't need to reassign
        if (&newParent == parentBone)
            return false;

        //Fry Paradox: This bone cannot become a child of its own children.
        if (newParent.isAncestor(*this))
            return false;

        for (uint32_t i = 0; i < parentBone->pImpl->m_childBones.size(); i++)
        {
            auto& entry = parentBone->pImpl->m_childBones[i];
            if (entry.get() == this)
            {
                newParent.pImpl->m_childBones.push_back(std::move(entry));

                //Check to make sure former owner is null and new owner is new parent
                if (!entry && newParent.pImpl->m_childBones.back().get() == this)
                {
                    parentBone->pImpl->m_childBones.erase(parentBone->pImpl->m_childBones.begin() + i);
                    pImpl->m_parent = &newParent;
                    return true;
                }

                return false;
            }
        }

        return true;
    }

    bool SkeletonBone::isAncestor(const SkeletonBone& boneCandidate) const
    {
        auto parentPtr = parent();
        uint64_t failSafe = 0;

        while (parentPtr->parent() != nullptr && failSafe < SkeletonRig::MaxBoneCount)
        {
            parentPtr = parentPtr->parent();

            if (parentPtr == &boneCandidate)
                return true;

            failSafe++;
        }

        if (failSafe >= SkeletonRig::MaxBoneCount)
            throw std::runtime_error("SkeletonBone::isAncestor() detected infinite loop in lineage.");

        return false;
    }

    Utilities::StringContainer SkeletonBone::toJSON(const uint64_t indents = 0) const {

        std::string boneTypeOutput = pImpl->m_boneTypeProperties == nullptr ?
                                "ERROR" : pImpl->m_boneTypeProperties->getTypeString();

        //using this to clear clang tidy output. Will fix JSON output later TODO: Fix JSON
        boneTypeOutput += std::format("{}",indents);

        // std::string output = (
        //     Utilities::Indent(indents) + "{\n" +
        //     Utilities::Indent(indents+1).c_str() + "\"name\":" + pImpl->m_name.c_str() + "\",\n" +
        //     Utilities::Indent(indents+1).c_str() + "\"parentBone\":" + pImpl->_parentBone.c_str() + ",\n" +
        //     Utilities::Indent(indents + 1).c_str() + "\"boneType\":" + boneTypeOutput.c_str() + ",\n" //+
        //     //Utilities::Indent(indents + 1).c_str() + "\"mirrorBoneIndex\":" + std::to_string(pImpl->mirrorBoneIndex).c_str() + ",\n"
        //     ).c_str();
        //
        // //output += (Utilities::Indent(indents+1) + "\"localRotation\": [").c_str();
        // //output += std::to_string(pImpl->localRotation.getX()) + ", " + std::to_string(pImpl->localRotation.getY()) + ", " + std::to_string(pImpl->localRotation.getZ()) + ", " + std::to_string(pImpl->localRotation.getW()) + "],\n";
        //
        // output += (Utilities::Indent(indents + 1) + "\"globalRotation\": [").c_str();
        // output += std::to_string(pImpl->globalRotation.x()) + ", " + std::to_string(pImpl->globalRotation.y()) + ", " + std::to_string(pImpl->globalRotation.z()) + ", " + std::to_string(pImpl->globalRotation.w()) + "],\n";
        //
        // //output += (Utilities::Indent(indents + 1) + "\"localPosition\": [").c_str();
        // //output += std::to_string(pImpl->localPosition.getX()) + ", " + std::to_string(pImpl->localPosition.getY()) + ", " + std::to_string(pImpl->localPosition.getZ()) + "],\n";
        //
        // output += (Utilities::Indent(indents + 1) + "\"globalPosition\": [").c_str();
        // output += std::to_string(pImpl->globalPosition.x()) + ", " + std::to_string(pImpl->globalPosition.y()) + ", " + std::to_string(pImpl->globalPosition.z()) + "]\n";
        //
        // output += (Utilities::Indent(indents) + "}").c_str();
        return ""; // output.c_str();
    }
#pragma endregion


#pragma region SKELETONRIG

    struct SkeletonRig::Impl
    {
        std::string _rigName = "MySkeletonRig";
        RigPackageManager _rigPackageManager;

        SkeletonBone _root;

        explicit Impl(const SkeletonRig& owner) : _root(&owner, "root") {}
    };

    const char* SkeletonRig::name() const { return pImpl->_rigName.c_str(); }

    bool SkeletonRig::setName(const char* name) const
    {
        if (SCOMPARE(name, "") == 0)
            return false;

        pImpl->_rigName = name;
        return true;
    }

    SkeletonBone* SkeletonRig::bone(const char* boneName) const
    {
        if (SCOMPARE(boneName, pImpl->_root.name()) == 0)
            return &pImpl->_root;

        return pImpl->_root.childBone(boneName, true);
    }
    RigPackageManager& SkeletonRig::getPackageManager() const
    {
        return pImpl->_rigPackageManager;
    }
    SkeletonRig::SkeletonRig()
    {
        pImpl = new Impl(*this);
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

    unsigned int SkeletonRig::boneTypeCount(const BoneTypeProperty::BoneType type) const
    {
        unsigned int pAnimatedBoneCount = 0;

        if (pImpl->_root.boneTypeProperty()->getType() == type)
            pAnimatedBoneCount++;

        pAnimatedBoneCount += pImpl->_root.boneTypeCount(type);

        return pAnimatedBoneCount;
    }

    unsigned int SkeletonRig::boneCount() const
    {
        return pImpl->_root.boneCount() + 1;
    }

    SkeletonBone* SkeletonRig::root() const
    {
        return &pImpl->_root;
    }

    Utilities::StringList SkeletonRig::boneList() const
    {
        const Utilities::StringList output;
        output.push_back(pImpl->_root.name());

        for (const auto list = pImpl->_root.pImpl->lineageList(); const auto& entry : list)
        {
            output.push_back(entry.c_str());
        }

        return output;
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

#pragma endregion
}
#pragma region EXTERN"C"

    CALUMI::UNIV::SkeletonRig* CreateSkeletonRigC(const char* name)
    {
        const auto outputSkeletonRig = new CALUMI::UNIV::SkeletonRig(name);
        return outputSkeletonRig;
    }
    int DeleteSkeletonRigC(const CALUMI::UNIV::SkeletonRig** ptrRef)
    {
        if (!ptrRef || !*ptrRef)
            return -1;

        try
        {
            delete *ptrRef;
            *ptrRef = nullptr;
            return 0;
        } catch (std::bad_alloc&) {}
        return -1;
    }
    CALUMI::UNIV::SkeletonBone* GetSkeletonRigRootC(const CALUMI::UNIV::SkeletonRig* rig)
    {
        if(!rig)
            return nullptr;

        try
        {
            return rig->root();
        }
        catch (const std::exception&) {}
        return nullptr;
    }
    int SetBoneTypeC(const CALUMI::UNIV::SkeletonBone* bone, const uint32_t boneType)
    {
        if (!bone)
            return -1;

        try
        {
            return bone->setBoneTypeProperty(CALUMI::UNIV::BoneTypeProperty::GetBoneType(boneType)) ? 0 : 1;
        }
        catch (std::bad_alloc&) {}
        return -1;
    }
    int SetBoneTypeFromStringC(const CALUMI::UNIV::SkeletonBone* bone, const char* boneType)
    {
        if (bone)
        try
        {
            return bone->setBoneTypeProperty(CALUMI::UNIV::BoneTypeProperty::BoneTypeFromString(boneType)) ? 0 : 1;
        }
        catch (std::bad_alloc&) {}
        return -1;
    }
    uint32_t GetBoneTypeC(const CALUMI::UNIV::SkeletonBone* bone)
    {
        return static_cast<uint32_t>(bone->boneTypeProperty()->getType());
    }
    const char* GetBoneTypeAsStringC(const CALUMI::UNIV::SkeletonBone* bone)
    {
        return bone->boneTypeProperty()->getTypeString();
    }
    int SetTwistBonePropertiesC(const CALUMI::UNIV::SkeletonBone* bone, const bool reassign, const char* twistDriver,
        const float twistDriverWeight)
    {
        if (bone)
        try
        {
            if (bone->boneTypeProperty()->getType() != CALUMI::UNIV::BoneTypeProperty::BoneType::Twist)
            {
                if (!reassign)
                    return 1;

                if (!bone->setBoneTypeProperty(CALUMI::UNIV::BoneTypeProperty::BoneType::Twist))
                    return 2;
            }

            const auto tProp = dynamic_cast<CALUMI::UNIV::TwistBoneProperty*>
            (const_cast<CALUMI::UNIV::BoneTypeProperty*>(bone->boneTypeProperty()));

            if (tProp)
            {
                tProp->setTwistDriver(twistDriver);
                tProp->setTwistDriverWeight(twistDriverWeight);
                return 0;
            }

            return 3;
        }
        catch (std::bad_alloc&) {}
        return -1;
    }
    const char* GetTwistBoneDriverC(const CALUMI::UNIV::SkeletonBone* bone)
    {
        if (!bone)
            return "";

        try
        {
            const auto tProp = dynamic_cast<CALUMI::UNIV::TwistBoneProperty*>(const_cast<CALUMI::UNIV::BoneTypeProperty*>(bone->boneTypeProperty()));

            if (tProp)
                return tProp->twistDriver();
        }
        catch(const std::exception&){}
        return "";
    }
    float GetTwistBoneDriverWeightC(const CALUMI::UNIV::SkeletonBone* bone)
    {
        if (bone)
        try
        {
            const auto* tProp = dynamic_cast<CALUMI::UNIV::TwistBoneProperty*>(const_cast<CALUMI::UNIV::BoneTypeProperty*>(bone->boneTypeProperty()));

            if (tProp)
                return tProp->twistDriverWeight();
        }
        catch(const std::exception&){}
        return std::numeric_limits<float>::quiet_NaN();
    }
    unsigned int GetSkeletonRigBoneCountC(const CALUMI::UNIV::SkeletonRig* source)
    {
        if (!source)
            return 0;
        try
        {
            return source->boneCount();
        }
        catch(const std::exception&){}
        return 0;
    }
    unsigned int GetSkeletonRigAnimatedBoneCountC(const CALUMI::UNIV::SkeletonRig* source)
    {
        if (!source)
            return 0;
        try
        {
            return source->boneTypeCount(CALUMI::UNIV::BoneTypeProperty::BoneType::Default);
        }
        catch(const std::exception&){}
        return 0;
    }
    unsigned int GetSkeletonRigNonAnimatedBoneCountC(const CALUMI::UNIV::SkeletonRig* source)
    {
        if (!source)
            return 0;
        try
        {
            return source->boneTypeCount(CALUMI::UNIV::BoneTypeProperty::BoneType::Twist);
        }
        catch(const std::exception&){}
        return 0;
    }
    const char* GetSkeletonRigNameC(const CALUMI::UNIV::SkeletonRig* source)
    {
        if (!source)
            return "";
        try
        {
            return source->name();
        }
        catch(const std::exception&){}
        return "";
    }
    const CALUMI::UNIV::SkeletonBone* GetSkeletonBoneC(const CALUMI::UNIV::SkeletonRig* source, const char* boneName)
    {
        if (!source)
            return nullptr;
        try
        {
            return source->bone(boneName);
        }
        catch(const std::exception&){}
        return nullptr;
    }
    const char* GetSkeletonBoneNameC(const CALUMI::UNIV::SkeletonBone* source)
    {
        if (!source)
            return "";
        try
        {
            return source->name();
        }
        catch(const std::exception&){}
        return "";
    }
    const CALUMI::UNIV::SkeletonBone* GetSkeletonBoneParentC(const CALUMI::UNIV::SkeletonBone* source)
    {
        if (!source)
            return nullptr;
        try
        {
            return dynamic_cast<const CALUMI::UNIV::SkeletonBone*>(source->parent());
        }
        catch(const std::exception&){}
        return nullptr;
    }
    const CALUMI::Math::Quaternion* GetGlobalSkeletonBoneRotationC(const CALUMI::UNIV::SkeletonBone* source)
    {
        if (!source)
            return nullptr;

        auto output = new CALUMI::Math::Quaternion();

        try
        {
            *output = source->globalTransform().rotation();
        }
        catch (const std::exception&)
        {
            delete output;
            output = nullptr;
        }

        return output;
    }
    const CALUMI::Math::Quaternion* GetLocalSkeletonBoneRotationC(const CALUMI::UNIV::SkeletonBone* source)
    {
        if (!source)
            return nullptr;

        auto output = new CALUMI::Math::Quaternion();

        try
        {
            *output = source->localTransform().rotation();
        }
        catch (const std::exception&)
        {
            delete output;
            output = nullptr;
        }

        return output;
    }
    const CALUMI::Math::Vector3* GetGlobalSkeletonBonePositionC(const CALUMI::UNIV::SkeletonBone* source)
    {
        if (!source)
            return nullptr;

        auto output = new CALUMI::Math::Vector3();

        try
        {
            *output = source->globalTransform().position();
        }
        catch (const std::exception&)
        {
            delete output;
            output = nullptr;
        }

        return output;
    }
    const CALUMI::Math::Vector3* GetLocalSkeletonBonePositionC(const CALUMI::UNIV::SkeletonBone* source)
    {
        if (!source)
            return nullptr;

        auto output = new CALUMI::Math::Vector3();

        try
        {
            *output = source->localTransform().position();
        }
        catch (const std::exception&)
        {
            delete output;
            output = nullptr;
        }

        return output;
    }
    int RenameBoneC(const CALUMI::UNIV::SkeletonRig* rig, const char* oldName, const char* newName)
    {
        if (!rig)
            return -1;

        const CALUMI::UNIV::SkeletonBone* bone = nullptr;

        try
        {
            bone = rig->bone(oldName);

        } catch (const std::exception&)
        {
            return -1;
        }

        if (!bone)
            return 1;

        if (SCOMPARE(newName, "") == 0)
            return 2;

        if (SCOMPARE(oldName, newName) == 0)
            return 3;

        if (!bone->setName(newName))
            return 4;

        return 0;
    }
    int DeleteBoneC(const CALUMI::UNIV::SkeletonBone** ptrRef)
    {
        if (!ptrRef || !*ptrRef)
            return -1;

        try
        {
            CALUMI::UNIV::SkeletonBone::DeleteBone(*ptrRef);
            return 0;
        }
        // ReSharper disable once CppDFAUnreachableCode
        catch (const std::exception&){}
        return -1;
    }
    CALUMI::UNIV::SkeletonBone* AddChildBoneC(const CALUMI::UNIV::SkeletonBone* parentBone, float posX, float posY,
                                    float posZ, float rotX, float rotY, float rotZ, float rotW, const char* boneName)
    {
        if (!parentBone)
            return nullptr;

        try
        {
            const auto output = parentBone->addChildBone(boneName, {posX, posY, posZ}, {rotX, rotY, rotZ, rotW});
            return output;
        }
        catch (const std::exception&) {}
        return nullptr;
    }
    CALUMI::UNIV::SkeletonBone* AddChildBoneWithEulerC(const CALUMI::UNIV::SkeletonBone* parentBone,
                                     const float posX, const float posY, float posZ,
                                     const float rad1, const float rad2, const float rad3, const int eulerOrder,
                                     const char* boneName)
    {
        if (!parentBone)
            return nullptr;

        try
        {
            const auto output = parentBone->addChildBone(boneName, {posX, posY, posZ},
                                                         CALUMI::Math::EulerDefinition(rad1, rad2, rad3,
                                                         CALUMI::Math::EulerDefinition::GetEulerOrder(eulerOrder)));
            return output;
        }
        catch (const std::exception&){}
        return nullptr;
    }
    CALUMI::UNIV::SkeletonBone* AddChildBoneWithVectorC(const CALUMI::UNIV::SkeletonBone* parentBone,
                                                        const CALUMI::Math::Vector3* position,
                                                        const CALUMI::Math::Quaternion* rotation,
                                                        const char* boneName)
    {
        if (!parentBone || !position || !rotation)
            return nullptr;

        try
        {
            const auto output = parentBone->addChildBone(boneName, *position, *rotation);
            return output;
        }
        catch (const std::exception&) {}
        return nullptr;
    }
    unsigned int GetSkeletonRigBoneTypeCountC(const CALUMI::UNIV::SkeletonRig* source, const uint32_t typeAsInteger)
    {
        if (!source)
            return 0;

        try
        {
            return source->boneTypeCount(CALUMI::UNIV::BoneTypeProperty::GetBoneType(typeAsInteger));
        }
        catch (const std::exception&) {}
        return 0;
    }
#pragma endregion
