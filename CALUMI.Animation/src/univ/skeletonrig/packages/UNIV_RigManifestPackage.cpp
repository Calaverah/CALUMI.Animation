//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "univ/skeletonrig/packages/UNIV_RigManifestPackage.h"

#include <set>
#include <string>
#include <vector>

#include "internalplatform.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"

namespace CALUMI::UNIV
{
    struct RigManifestPackage::Impl
    {
        std::vector<std::string> m_list;
    };

    RigManifestPackage::RigManifestPackage() : pImpl(new Impl) {}

    RigManifestPackage::RigManifestPackage(const RigManifestPackage& other) : RigManifestPackage()
    {
        *this = other;
    }

    RigManifestPackage::~RigManifestPackage()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    RigManifestPackage& RigManifestPackage::operator=(const RigManifestPackage& other)
    {
        if (this != &other)
        {
            pImpl->m_list = other.pImpl->m_list;
        }
        return *this;
    }

    bool RigManifestPackage::addBone(const char* boneName) const
    {
        if (SCOMPARE(boneName, "") == 0)
            return false;

        for (const auto& entry : pImpl->m_list)
        {
            if (SCOMPARE(entry.c_str(), boneName) == 0)
                return false;
        }

        pImpl->m_list.emplace_back(boneName);
        return true;
    }

    bool RigManifestPackage::insertBone(const char* boneName, const unsigned int index) const
    {
        if (SCOMPARE(boneName, "") == 0)
            return false;

        if (index >= pImpl->m_list.size())
            return false;

        pImpl->m_list.insert(pImpl->m_list.begin() + index, boneName);
        return true;
    }

    bool RigManifestPackage::removeBone(const char* boneName) const
    {
        const unsigned int size = pImpl->m_list.size();

        std::erase_if(pImpl->m_list, [boneName](const auto& entry)
        {
            return SCOMPARE(entry.c_str(), boneName) == 0;
        });

        return pImpl->m_list.size() != size;
    }

    unsigned int RigManifestPackage::count() const
    {
        return pImpl->m_list.size();
    }

    const char* RigManifestPackage::bone(const unsigned int index) const
    {
        if (index >= pImpl->m_list.size())
            return nullptr;

        return pImpl->m_list[index].c_str();
    }

    bool RigManifestPackage::AddPackage(const SkeletonRig& rig, const bool overwrite)
    {
        const auto& mgr = rig.getPackageManager();

        auto* pkg = new RigManifestPackage;

        if (mgr.addPackage(pkg, overwrite))
        {
            return true;
        }

        delete pkg;
        pkg = nullptr;
        return false;
    }

    RigManifestPackage& RigManifestPackage::GetPackage(const SkeletonRig& rig)
    {
        auto& mgr = rig.getPackageManager();
        if (const auto pkg = dynamic_cast<RigManifestPackage*>(mgr.getPackage(MANIFEST_RIG_PACKAGE)))
        {
            return *pkg;
        }

        AddPackage(rig, false);

        if (const auto pkg = dynamic_cast<RigManifestPackage*>(mgr.getPackage(MANIFEST_RIG_PACKAGE)))
        {
            return *pkg;
        }

        throw std::runtime_error("RigManifestPackage::GetRigManifestPackage() could not find nor add rig package.");
    }

    bool RigManifestPackage::RemovePackage(const SkeletonRig& rig)
    {
        return rig.getPackageManager().removePackage(MANIFEST_RIG_PACKAGE);
    }

    static void s_RecursiveLineageManifest(const Utilities::StringList& list,
                                           std::set<std::string>& checkList,
                                           const SkeletonRig& rig,
                                           const SkeletonBone& bone,
                                           uint16_t& failSafe)
    {
        if (failSafe >= SkeletonRig::MaxBoneCount + 1)
            return;

        if (checkList.contains(bone.name()))
            return;

        const auto parent = dynamic_cast<const SkeletonBone*>(bone.parent());

        failSafe++;

        if (parent)
            s_RecursiveLineageManifest(list, checkList, rig, *parent, failSafe);

        uint64_t offset = 0;

        if (!list.empty())
        {
            offset += list.getOffset(list.size() -1);
            offset += list.stringLength(list.size()-1, true);
        }

        list.push_back(bone.name(), offset);
        checkList.insert(bone.name());
    }

    Utilities::StringList RigManifestPackage::processPackage(const SkeletonRig& rig) const
    {
        const Utilities::StringList output;
        std::set<std::string> checkList;

        const auto root = rig.root();

        //root must always come first, we can save time by adding it here
        output.push_back(root->name());
        checkList.insert(root->name());

        //A little sanity check for preventing recursion looping to infinity
        uint16_t failSafe = 0;

        for (auto& entry : pImpl->m_list)
        {
            if (const auto bPtr = dynamic_cast<const SkeletonBone*>(rig.bone(entry.c_str())))
                s_RecursiveLineageManifest(output, checkList, rig, *bPtr, failSafe);
        }

        //repeat the process for the entire rig, which skips bones already added
        const auto fullList = rig.boneList();
        for (int i = 0; i < fullList.size(); i++)
        {
            if (const auto bPtr = dynamic_cast<const SkeletonBone*>(rig.bone(fullList.c_str(i))))
                s_RecursiveLineageManifest(output, checkList, rig, *bPtr, failSafe);
        }

        if (!output.empty())
        {
            uint64_t finalOffset = output.getOffset(output.size() - 1);
            finalOffset += output.stringLength(output.size() - 1, true);
            output.setFinalOffset(finalOffset);
        }

        return output;
    }

   const char* RigManifestPackage::getPackageType() const
    {
        return MANIFEST_RIG_PACKAGE;
    }

    Utilities::StringContainer RigManifestPackage::toJSON(uint64_t indents) const
    {
        return {};
    }

    IPackage* RigManifestPackage::clone() const
    {
        return new RigManifestPackage(*this);
    }

    bool RigManifestPackage::handleBoneRename(const char* oldBone, const char* newName)
    {
        if (SCOMPARE(oldBone, newName) == 0)
            return false;

        if (SCOMPARE(newName, "") == 0)
            return false;

        for (auto& entry : pImpl->m_list)
        {
            if (SCOMPARE(entry.c_str(), oldBone) == 0)
                entry = newName;
        }

        return true;
    }
}
