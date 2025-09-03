//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_AnimationScene.h"

#include "CALUMI_Utilities.h"


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
        
        std::expected<std::vector<std::filesystem::path>, std::string> AnimationScene::GetFilePathsFromAnimationScene(const wchar_t* directoryPath, const char* extension)
        {
            std::vector<std::filesystem::path> animationFilePaths;
            std::string errorMessage;
            animationFilePaths.reserve(animations.size());
            for (UNIV::Animation entry : animations)
            {
                if (entry.animationTitle.empty())
                {
                    errorMessage = "[CALUMI.Animation API] Empty string found for Animation Title";
                    return std::unexpected(errorMessage);
                }
                std::filesystem::path pathToAdd(directoryPath);
                pathToAdd /= "animations";
                pathToAdd /= sceneName;
                pathToAdd /= entry.animationTitle;
                pathToAdd.replace_extension(extension);
                animationFilePaths.push_back(pathToAdd);
            }

            return animationFilePaths;
        }

        std::string AnimationScene::ToJSON() const {
            std::string output = "{\n\"sceneName\":\"" + sceneName + "\",\n\"animations\":";
            output += Utilities::VectorToJSON(animations);
            output += ",\n\"rig\":";
            output += rig.ToJSON();
            output += "\n}";
            return output;
        }


    }
}

