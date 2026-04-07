//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include "univ/skeletonrig/UNIV_IRigPackage.h"
#include "univ/skeletonrig/UNIV_BoneTypes.h"
#include <AnimUtil>

namespace CALUMI {
	namespace UNIV {
#pragma region RIGPACKAGEMANAGER

        RigPackageManager::RigPackageManager() : IPackageManager() {} 


        Utilities::StringContainer RigPackageManager::toJSON(uint64_t indents) const
        {
            //TODO: RigPackageManager JSON
            return Utilities::StringContainer();
        }

        bool RigPackageManager::onBoneRename(const char* oldBone, const char* newName, uint64_t idx)
        {
            bool output = true;

            for (uint64_t i = 0; i < packageCount(); i++)
            {
                if (auto pkg = dynamic_cast<IRigPackage*>(getPackage(i)))
                {
                    if (!pkg->handleBoneRename(oldBone, newName, idx))
                        output = false;
                }
            }

            return output;
        }

#pragma endregion
} }