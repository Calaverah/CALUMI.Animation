//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_AnimationScene.h"


namespace CALUMI {
    namespace UNIV {

        //Extern C Functions
        bool DeleteAnimationSceneC(AnimationScene* ptr)
        {
            if (ptr)
            {
                delete ptr;
                return true;
            }
            return false;
        }
        AnimationScene* UNIV::CreateAnimationSceneC(const char* sceneName)
        {
            UNIV::AnimationScene* univAnimationScene = new UNIV::AnimationScene;
            univAnimationScene->sceneName = sceneName;

            return univAnimationScene;
        }
        bool UNIV::AddRigToAnimationSceneC(AnimationScene* scene, SkeletonRig* rig, const char* errorMessage)
        {
            if (rig->boneEntries.empty())
            {
                errorMessage = "[CALUMI.Animation API] No Bone Entries Found In Rig!";
                return false;
            }
            scene->rig = *rig;
            rig = nullptr;
            errorMessage = "[CALUMI.Animation API] Rig Data Transferred Successfully. Original Ptr Set To Null!";
            return true;
        }
        bool UNIV::AddAnimationToAnimationSceneC(AnimationScene* scene, Animation* animation, const char* errorMessage)
        {
            if (animation->animationTitle == "")
            {
                errorMessage = "[CALUMI.Animation API] Must Have Animation Title!";
                return false;
            }
            for (Animation entry : scene->animations)
            {
                if (animation->animationTitle == entry.animationTitle)
                {
                    errorMessage = "[CALUMI.Animation API] Animation Titles Must Be Unique!";
                    return false;
                }
            }
            scene->animations.push_back(*animation);
            delete animation;

            errorMessage = "[CALUMI.Animation API] Animation Data Copied Into Animation Vector Successfully. Original Ptr Has Been Deleted!";
            return true;
        }
        
    }
}

