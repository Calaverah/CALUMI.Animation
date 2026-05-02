//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "univ/skeletonrig/packages/UNIV_RigMirrorPackage.h"

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "internalplatform.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"
#include "univ/skeletonrig/UNIV_IRigPackage.h"

namespace CALUMI::UNIV
{
    struct RigMirrorPackage::Impl
    {
        std::vector<std::pair<std::string, std::string>> m_mirrors;
    };

    RigMirrorPackage::RigMirrorPackage() : pImpl(new Impl) {}

    RigMirrorPackage::~RigMirrorPackage()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    void RigMirrorPackage::addPair(const char* bone1, const char* bone2) const
    {
        //if both entries are blank, there is nothing to do
        if (SCOMPARE(bone1, "") == 0 && SCOMPARE(bone2, "") == 0)
            return;

        //remove any existing usages of either of the bone entries
        std::erase_if(pImpl->m_mirrors, [bone1, bone2](const std::pair<std::string, std::string>& pair)
        {
            return SCOMPARE(bone1, pair.first.c_str()) == 0 ||
                   SCOMPARE(bone1, pair.second.c_str()) == 0 ||
                   SCOMPARE(bone2, pair.first.c_str()) == 0 ||
                   SCOMPARE(bone2, pair.second.c_str()) == 0;
        });

        //if one of the entries is blank, we only care about removing, no need to add
        if (SCOMPARE(bone1, "") == 0 || SCOMPARE(bone2, "") == 0)
            return;

        pImpl->m_mirrors.emplace_back(std::string(bone1), std::string(bone2));
    }

    const char* RigMirrorPackage::pairedBone(const char* bone) const
    {
        for (const auto& [first, second] : pImpl->m_mirrors)
        {
            if (SCOMPARE(first.c_str(), bone) == 0)
                return second.c_str();

            if (SCOMPARE(second.c_str(), bone) == 0)
                return first.c_str();
        }

        return "";
    }

    unsigned int RigMirrorPackage::pairCount() const
    {
        return pImpl->m_mirrors.size();
    }

    const char* RigMirrorPackage::firstOfPair(const unsigned int index) const
    {
        if (index >= pImpl->m_mirrors.size())
            return "";

        return pImpl->m_mirrors[index].first.c_str();
    }

    const char* RigMirrorPackage::secondOfPair(const unsigned int index) const
    {
        if (index >= pImpl->m_mirrors.size())
            return "";

        return pImpl->m_mirrors[index].second.c_str();
    }

    const char* RigMirrorPackage::packageType() const
    {
        return MIRROR_RIG_PACKAGE;
    }

    IPackage* RigMirrorPackage::clone() const
    {
        const auto output = new RigMirrorPackage();

        *output = *this;

        return output;
    }

    bool RigMirrorPackage::handleBoneRename(const char* oldBone, const char* newName)
    {
        bool result = false;

        if (SCOMPARE(oldBone, "") == 0 || SCOMPARE(newName, "") == 0)
            return result;


        for (auto& [first, second] : pImpl->m_mirrors)
        {
            if (SCOMPARE(oldBone, second.c_str()) == 0)
            {
                second = newName;
                result = true;
            }

            if (SCOMPARE(oldBone, first.c_str()) == 0)
            {
                first = newName;
                result = true;
            }
        }

        return result;
    }

    RigMirrorPackage& RigMirrorPackage::operator=(const RigMirrorPackage& other)
    {
        if (this != &other)
        {
            pImpl->m_mirrors = other.pImpl->m_mirrors;
        }
        return *this;
    }

    Utilities::JsonObject RigMirrorPackage::toJson() const
    {
        Utilities::JsonObject output;

        for (const auto& [first, second] : pImpl->m_mirrors)
        {
            output[first.c_str()] = second.c_str();
        }

        return output;
    }

    bool RigMirrorPackage::AddPackage(const SkeletonRig& rig, const bool overwrite)
    {
        const auto& mgr = rig.packageManager();

        auto* pkg = new RigMirrorPackage();

        if (mgr.addPackage(pkg, overwrite))
        {
            return true;
        }

        delete pkg;
        pkg = nullptr;
        return false;
    }

    bool RigMirrorPackage::RemovePackage(const SkeletonRig& rig)
    {
        return rig.packageManager().removePackage(MIRROR_RIG_PACKAGE);
    }

    RigMirrorPackage& RigMirrorPackage::GetPackage(const SkeletonRig& rig)
    {
        auto& mgr = rig.packageManager();
        if (const auto pkg = dynamic_cast<RigMirrorPackage*>(mgr.package(MIRROR_RIG_PACKAGE)))
        {
            return *pkg;
        }

        AddPackage(rig, false);

        if (const auto pkg = dynamic_cast<RigMirrorPackage*>(mgr.package(MIRROR_RIG_PACKAGE)))
        {
            return *pkg;
        }

        throw std::runtime_error("RigMirrorPackage::GetPackage() could not find nor add rig package.");
    }
}
    int UNIVMirrorRigPackage_AddPackageToSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig, const bool overwrite)
    {
        if (!rig)
            return -1;

        try
        {
            if (CALUMI::UNIV::RigMirrorPackage::AddPackage(*rig, overwrite))
                return 0;

            return 1;
        }
        catch ( const std::exception&) {}
            return -1;
    }

    int UNIVMirrorRigPackage_RemoveRigPackageFromSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig)
    {
        if (!rig)
            return -1;

        try
        {
            if (CALUMI::UNIV::RigMirrorPackage::RemovePackage(*rig))
                return 0;

            return 1;
        }
        catch ( const std::exception&) {}
        return -1;
    }

    int UNIVMirrorRigPackage_AddPairToRigPackageFromSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig, const char*
        bone1,
        const
        char* bone2)
    {
        if (!rig)
            return -1;

        try
        {
            auto& mgr = rig->packageManager();
            const auto pkg = dynamic_cast<CALUMI::UNIV::RigMirrorPackage*>(mgr.package
                (CALUMI::UNIV::MIRROR_RIG_PACKAGE));

            if (!pkg)
                return 1;

            pkg->addPair(bone1, bone2);
            return 0;
        }
        catch ( const std::exception&) {}
        return -1;
    }

    int64_t UNIVMirrorRigPackage_GetPairCountC(const CALUMI::UNIV::SkeletonRig* rig)
    {
        if (!rig)
            return -1;

        try
        {
            auto& mgr = rig->packageManager();
            const auto pkg = dynamic_cast<CALUMI::UNIV::RigMirrorPackage*>(mgr.package
                (CALUMI::UNIV::MIRROR_RIG_PACKAGE));

            if (!pkg)
                return 0;

            return pkg->pairCount();
        }
        catch ( const std::exception&) {}
        return -1;
    }
    const char* UNIVMirrorRigPackage_GetFirstOfPairEntryC(const CALUMI::UNIV::SkeletonRig* rig, const unsigned int
        index)
    {
        if (!rig)
            return nullptr;

        try
        {
            auto& mgr = rig->packageManager();
            const auto pkg = dynamic_cast<CALUMI::UNIV::RigMirrorPackage*>(mgr.package
                (CALUMI::UNIV::MIRROR_RIG_PACKAGE));

            if (!pkg)
                return "";

            return pkg->firstOfPair(index);
        }
        catch ( const std::exception&) {}
        return nullptr;
    }
    const char* UNIVMirrorRigPackage_GetSecondOfPairEntryC(const CALUMI::UNIV::SkeletonRig* rig, const unsigned int
        index)
    {
        if (!rig)
            return nullptr;

        try
        {
            auto& mgr = rig->packageManager();
            const auto pkg = dynamic_cast<CALUMI::UNIV::RigMirrorPackage*>(mgr.package
                (CALUMI::UNIV::MIRROR_RIG_PACKAGE));

            if (!pkg)
                return "";

            return pkg->secondOfPair(index);
        }
        catch ( const std::exception&) {}
        return nullptr;
    }
