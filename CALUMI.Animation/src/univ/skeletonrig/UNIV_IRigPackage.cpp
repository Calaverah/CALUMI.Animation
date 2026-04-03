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


        Utilities::StringContainer RigPackageManager::ToJSON(size_t indents) const
        {
            //TODO: RigPackageManager JSON
            return Utilities::StringContainer();
        }

        bool RigPackageManager::HandleBoneRename(const char* oldBone, const char* newName, size_t idx)
        {
            bool output = true;

            for (size_t i = 0; i < packageCount(); i++)
            {
                if (auto pkg = dynamic_cast<IRigPackage*>(getPackage(i)))
                {
                    if (!pkg->HandleBoneRename(oldBone, newName, idx))
                        output = false;
                }
            }

            return output;
        }

#pragma endregion
} }