//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include "univ/skeletonrig/UNIV_IRigPackage.h"
#include "univ/skeletonrig/UNIV_BoneTypes.h"
#include <AnimUtil>

namespace CALUMI::UNIV
{
#pragma region RIGPACKAGEMANAGER

    Utilities::StringContainer RigPackageManager::toJSON(uint64_t indents) const
    {
        //TODO: RigPackageManager JSON
        return {};
    }

    bool RigPackageManager::onBoneRename(const char* oldBone, const char* newName)
    {
        bool output = true;

        const auto keys = getPackageList();

        for (uint64_t i = 0; i < keys.size(); i++)
        {
            if (const auto pkg = dynamic_cast<IRigPackage*>(getPackage(keys.c_str(i))))
            {
                if (!pkg->handleBoneRename(oldBone, newName))
                    output = false;
            }
        }

        return output;
    }

#pragma endregion
}
