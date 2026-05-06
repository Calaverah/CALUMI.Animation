//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppExpressionWithoutSideEffects
#include "internalvectordef.h"
#include "internalplatform.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"
#include "utilities/CALUMI_Utilities.h"
#include <string>
#include <vector>
#include <memory>
#include <limits>

#include "sfbgs/skeletonrig/SFBGS_RigPackage.h"
#include "sfbgs/skeletonrig/SFBGS_SkeletonRig.h"
#include "univ/skeletonrig/packages/UNIV_RigManifestPackage.h"
#include "univ/skeletonrig/packages/UNIV_RigMirrorPackage.h"


namespace CALUMI::UNIV
{
#pragma region SKELETONBONE
    struct SkeletonBone::Impl
    {
    public:
        std::shared_ptr<BoneTypeProperty> m_boneTypeProperty = std::make_shared<DefaultBoneProperty>();
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

    SkeletonBone* SkeletonBone::childBone(const char* name, const bool recursive) const
    {
        for (const auto& bone : pImpl->m_childBones)
        {
            if (SCOMPARE(bone->name(), name) == 0)
                return bone.get();

            if (recursive)
            {
                const auto& childBone = bone->childBone(name, recursive);

                if (childBone)
                    return childBone;
            }
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

            if (bone->pImpl->m_boneTypeProperty->type() == type)
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

    void SkeletonBone::setLocalTransform(const Math::Transform& transform) const
    {
        pImpl->m_localTransform = transform;
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
        if (pImpl->m_boneTypeProperty)
        {
            if (!resetExisting && boneType == pImpl->m_boneTypeProperty->type())
                return false;
        }

        switch (boneType)
        {
        case BoneTypeProperty::BoneType::Default:
            pImpl->m_boneTypeProperty = std::make_shared<DefaultBoneProperty>();
            break;
        case BoneTypeProperty::BoneType::Twist:
            pImpl->m_boneTypeProperty = std::make_shared<TwistBoneProperty>();
            break;
        default:
            return false;
        }
        return true;
    }

    const BoneTypeProperty* SkeletonBone::boneTypeProperty() const
    {
        return pImpl->m_boneTypeProperty.get();
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

    Utilities::JsonObject SkeletonBone::toJson() const
    {
        Utilities::JsonObject output;

        output["name"] = pImpl->m_name.c_str();
        output["property"] = pImpl->m_boneTypeProperty->toJson();
        output["transform"] = pImpl->m_localTransform.toJson();

        Utilities::JsonArray childBones;
        for (const auto& cBone : pImpl->m_childBones)
        {
            childBones.push_back(cBone->toJson());
        }

        output["children"] = childBones;

        return output;
    }

    void SkeletonBone::fromJson(const Utilities::JsonObject& data) const
    {
        if (data.contains("property"))
        {
            if (const auto prop = data["property"].toObject(); prop.contains("type"))
            {
                setBoneTypeProperty (BoneTypeProperty::BoneTypeFromString(prop["type"].toString()),true);
                pImpl->m_boneTypeProperty->fromJson(prop);
            }
            else
                resetBoneTypeProperty();
        }
        else
            resetBoneTypeProperty();

        if (data.contains("children"))
        {
            auto children = data["children"].toArray();
            for (int i = 0; i < children.size(); i++)
            {
                const auto& child = children.at(i).toObject();
                const auto childBone = addChildBone(child["name"].toString(), child["transform"].toObject());
                childBone->fromJson(child);
            }
        }
    }

#pragma endregion


#pragma region SKELETONRIG

    struct SkeletonRig::Impl
    {
        std::string m_rigName = "MySkeletonRig";
        RigPackageManager m_rigPackageManager;

        SkeletonBone m_root;

        explicit Impl(const SkeletonRig& owner) : m_root(&owner, "root") {}
    };

    const char* SkeletonRig::name() const { return pImpl->m_rigName.c_str(); }

    bool SkeletonRig::setName(const char* name) const
    {
        if (SCOMPARE(name, "") == 0)
            return false;

        pImpl->m_rigName = name;
        return true;
    }

    SkeletonBone* SkeletonRig::bone(const char* boneName) const
    {
        if (SCOMPARE(boneName, pImpl->m_root.name()) == 0)
            return &pImpl->m_root;

        return pImpl->m_root.childBone(boneName, true);
    }
    RigPackageManager& SkeletonRig::packageManager() const
    {
        return pImpl->m_rigPackageManager;
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
        pImpl->m_rigName = _rigName.c_str();
    }
    SkeletonRig::SkeletonRig(const char* _rigName) : SkeletonRig()
    {
        pImpl->m_rigName = _rigName;
    }

    unsigned int SkeletonRig::boneTypeCount(const BoneTypeProperty::BoneType type) const
    {
        unsigned int pAnimatedBoneCount = 0;

        if (pImpl->m_root.boneTypeProperty()->type() == type)
            pAnimatedBoneCount++;

        pAnimatedBoneCount += pImpl->m_root.boneTypeCount(type);

        return pAnimatedBoneCount;
    }

    unsigned int SkeletonRig::boneCount() const
    {
        return pImpl->m_root.boneCount() + 1;
    }

    SkeletonBone* SkeletonRig::root() const
    {
        return &pImpl->m_root;
    }

    Utilities::StringList SkeletonRig::boneList() const
    {
        const Utilities::StringList output;
        output.push_back(pImpl->m_root.name());

        for (const auto list = pImpl->m_root.pImpl->lineageList(); const auto& entry : list)
        {
            output.push_back(entry.c_str());
        }

        return output;
    }

    Utilities::JsonObject SkeletonRig::toJson() const
    {
        Utilities::JsonObject output;

        output["name"] = pImpl->m_rigName.c_str();
        output["root"] = pImpl->m_root.toJson();
        output["packages"] = pImpl->m_rigPackageManager.toJson();

        return output;
    }

    SkeletonRig::SkeletonRig(const Utilities::JsonObject& data) : SkeletonRig()
    {

        if (data.contains("name"))
            pImpl->m_rigName = data["name"].toString();

        if (data.contains("root"))
        {
            const Utilities::JsonObject root = data["root"].toObject();
            if ( root.contains("transform"))
                pImpl->m_root.pImpl->m_localTransform.fromJson(root["transform"].toObject());

            if (root.contains("name"))
                pImpl->m_root.pImpl->m_name = root["name"].toString();

            pImpl->m_root.fromJson(root);
        }

        if (data.contains("packages"))
        {
            const auto packages = data["packages"].toObject();
            const auto keys = packages.keys();
            for (int i = 0; i < keys.size(); i++)
            {
                if (SCOMPARE(keys.c_str(i), MANIFEST_RIG_PACKAGE) == 0)
                {
                    auto& manifest = RigManifestPackage::GetPackage(*this);
                    manifest.fromJson(packages[keys.c_str(i)].toObject());
                }
                else if (SCOMPARE(keys.c_str(i), MIRROR_RIG_PACKAGE) == 0)
                {
                    auto& mirror = RigMirrorPackage::GetPackage(*this);
                    mirror.fromJson(packages[keys.c_str(i)].toObject());
                }
                else if (SCOMPARE(keys.c_str(i), SFBGS::SFBGS_RIG_PACKAGE) == 0)
                {
                    auto& sfbgs = SFBGS::SFBGS_RigPackage::GetPackage(*this);
                    sfbgs.fromJson(packages[keys.c_str(i)].toObject());
                }
            }
        }
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
    return static_cast<uint32_t>(bone->boneTypeProperty()->type());
}
const char* GetBoneTypeAsStringC(const CALUMI::UNIV::SkeletonBone* bone)
{
    return bone->boneTypeProperty()->typeAsString();
}
int SetTwistBonePropertiesC(const CALUMI::UNIV::SkeletonBone* bone, const bool reassign, const char* twistDriver,
    const float twistDriverWeight)
{
    if (bone)
    try
    {
        if (bone->boneTypeProperty()->type() != CALUMI::UNIV::BoneTypeProperty::BoneType::Twist)
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
const CALUMI::UNIV::SkeletonBone* GetSkeletonBoneChildC(const CALUMI::UNIV::SkeletonBone* parent, const unsigned int index)
{
    if (parent)
        try
        {
            return parent->childBone(index);
        }
    catch (const std::exception&) {}
    return nullptr;
}
const CALUMI::UNIV::SkeletonBone* GetSkeletonBoneChildWithNameC(const CALUMI::UNIV::SkeletonBone* parent, const char* name, const bool recursive)
{
    if (parent)
        try
        {
            return parent->childBone(name, recursive);
        }
    catch (const std::exception&) {}
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
int SetGlobalSkeletonBonePositionC(const CALUMI::UNIV::SkeletonBone* bone, float x, float y, float z)
{
    if (!bone)
        return -1;

    try
    {
        auto global = bone->globalTransform();
        global.setPosition({x,y,z});

        if (const auto parent = dynamic_cast<const CALUMI::UNIV::SkeletonBone*>(bone->parent()))
        {
            const auto local = global.local(parent->globalTransform());
            bone->setLocalTransform(local);
            return 0;
        }
        if (bone->isRoot())
        {
            return SetLocalSkeletonBonePositionC(bone, x, y, z);
        }

        return 1;

    }
    catch (const std::exception&) {}
    return -1;
}
int SetLocalSkeletonBonePositionC(const CALUMI::UNIV::SkeletonBone* bone, float x, float y, float z)
{
    if (!bone)
        return -1;

    try
    {
        auto local = bone->localTransform();
        local.setPosition({x,y,z});
        bone->setLocalTransform(local);
        return 0;
    }
    catch (const std::exception&) {}
    return -1;
}
int SetGlobalSkeletonBoneRotationC(const CALUMI::UNIV::SkeletonBone* bone, float x, float y, float z, float w)
{
    if (!bone)
        return -1;

    try
    {
        auto global = bone->globalTransform();
        global.setRotation({x,y,z,w});

        if (const auto parent = dynamic_cast<const CALUMI::UNIV::SkeletonBone*>(bone->parent()))
        {
            const auto local = global.local(parent->globalTransform());
            bone->setLocalTransform(local);
            return 0;
        }
        if (bone->isRoot())
        {
            return SetLocalSkeletonBoneRotationC(bone, x, y, z, w);
        }
        return 1;

    }
    catch (const std::exception&) {}
    return -1;
}
int SetLocalSkeletonBoneRotationC(const CALUMI::UNIV::SkeletonBone* bone, float x, float y, float z, float w)
{
    if (!bone)
        return -1;

    try
    {
        auto local = bone->localTransform();
        local.setRotation({x,y,z,w});
        bone->setLocalTransform(local);
        return 0;
    }
    catch (const std::exception&) {}
    return -1;
}
int SetGlobalSkeletonBoneRotationWithEulerC(const CALUMI::UNIV::SkeletonBone* bone, const float rot1, const float rot2,
    const float rot3, const int eulerOrder)
{
    if (!bone)
        return -1;

    try
    {
        auto global = bone->globalTransform();
        global.setRotation({rot1,rot2,rot3,CALUMI::Math::EulerDefinition::GetEulerOrder(eulerOrder)});

        if (const auto parent = dynamic_cast<const CALUMI::UNIV::SkeletonBone*>(bone->parent()))
        {
            const auto local = global.local(parent->globalTransform());
            bone->setLocalTransform(local);
            return 0;
        }
        if (bone->isRoot())
        {
            return SetLocalSkeletonBoneRotationWithEulerC(bone, rot1, rot2, rot3, eulerOrder);
        }
        return 1;
    }
    catch (const std::exception&) {}
    return -1;
}
int SetLocalSkeletonBoneRotationWithEulerC(const CALUMI::UNIV::SkeletonBone* bone, const float rot1, const float rot2,
    const float rot3, const int eulerOrder)
{
    if (!bone)
        return -1;

    try
    {
        auto local = bone->localTransform();
        local.setRotation({rot1,rot2,rot3,CALUMI::Math::EulerDefinition::GetEulerOrder(eulerOrder)});
        bone->setLocalTransform(local);
        return 0;
    }
    catch (const std::exception&) {}
    return -1;
}
int SetGlobalSkeletonBoneTransformC(const CALUMI::UNIV::SkeletonBone* bone, float posX, float posY, float posZ,
    float rotX, float rotY, float rotZ, float rotW)
{
    if (!bone)
        return -1;

    try
    {
        const auto global = CALUMI::Math::Transform({posX, posY, posZ},{rotX,rotY,rotZ,rotW});
        if (const auto parent = dynamic_cast<const CALUMI::UNIV::SkeletonBone*>(bone->parent()))
        {
            const auto local = global.local(parent->globalTransform());
            bone->setLocalTransform(local);
            return 0;
        }
        if (bone->isRoot())
        {
            return SetLocalSkeletonBoneTransformC(bone, posX, posY, posZ, rotX, rotY, rotZ, rotW);
        }
        return 1;
    }
    catch (const std::exception&) {}
    return -1;
}

int SetLocalSkeletonBoneTransformC(const CALUMI::UNIV::SkeletonBone* bone,
                                   const float posX, const float posY, const float posZ,
                                   float rotX, float rotY, float rotZ, float rotW)
{
    if (!bone)
        return -1;

    try
    {
        bone->setLocalTransform({posX, posY, posZ},{rotX,rotY,rotZ,rotW});
        return 0;
    }
    catch (const std::exception&) {}
    return -1;
}

int SetGlobalSkeletonBoneTransformWithEulerC(const CALUMI::UNIV::SkeletonBone* bone, const float posX, const float posY,
                           const float posZ, const float rot1, const float rot2, const float rot3, const int eulerOrder)
{
    if (!bone)
        return -1;

    try
    {
        const auto global = CALUMI::Math::Transform({posX, posY, posZ},{rot1,rot2,rot3,CALUMI::Math::EulerDefinition::GetEulerOrder(eulerOrder)});
        if (const auto parent = dynamic_cast<const CALUMI::UNIV::SkeletonBone*>(bone->parent()))
        {
            const auto local = global.local(parent->globalTransform());
            bone->setLocalTransform(local);
            return 0;
        }
        if (bone->isRoot())
        {
            return SetLocalSkeletonBoneTransformWithEulerC(bone, posX, posY, posZ, rot1, rot2, rot3, eulerOrder);
        }
        return 1;
    }
    catch (const std::exception&) {}
    return -1;
}

int SetLocalSkeletonBoneTransformWithEulerC(const CALUMI::UNIV::SkeletonBone* bone, const float posX, const float posY,
                           const float posZ, const float rot1, const float rot2, const float rot3, const int eulerOrder)
{
    if (!bone)
        return -1;

    try
    {
        bone->setLocalTransform({posX, posY, posZ},{rot1,rot2,rot3,CALUMI::Math::EulerDefinition::GetEulerOrder(eulerOrder)});
        return 0;
    }
    catch (const std::exception&) {}
    return -1;
}
int SetSkeletonBoneParentC(const CALUMI::UNIV::SkeletonBone* bone, const CALUMI::UNIV::SkeletonBone* boneParent)
{
    if (!bone || !boneParent)
        return -1;

    try
    {
        return bone->setParentBone(*boneParent) ? 0 : 1;
    }
    catch (const std::exception&) {}
    return -1;
}

#pragma endregion
