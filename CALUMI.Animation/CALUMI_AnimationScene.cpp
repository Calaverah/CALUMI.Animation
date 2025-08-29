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
        Animation* GetAnimationC(AnimationScene* source, int index, const char* errorMessage)
        {
            if(source->animations.size()<=index)
            {
                errorMessage = "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
                return nullptr;
            }
            return &source->animations.at(index);
        }
        size_t GetAnimationCountC(AnimationScene* source)
        {
            return source->animations.size();
        }
        const char* GetAnimationSceneName(AnimationScene* source)
        {
            return source->sceneName.c_str();
        }
        SkeletonRig* GetSkeletonRig(AnimationScene* source)
        {
            return &source->rig;
        }
        bool HasSkeletonRig(AnimationScene* source)
        {
            return !source->rig.boneEntries.empty();
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

