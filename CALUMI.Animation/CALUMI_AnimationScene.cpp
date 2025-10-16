//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_AnimationScene.h"
#include "CALUMI_Utilities.h"
#include <string>


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
        Animation* GetAnimationC(AnimationScene* source, int index, Utilities::StringContainer* errorMessage)
        {
            errorMessage->Clear();

            if(source->animations.size()<=index)
            {
                *errorMessage += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
                return nullptr;
            }
            return &source->animations.at(index);
        }
        size_t GetAnimationCountC(AnimationScene* source)
        {
            return source->animations.size();
        }
        const char* GetAnimationSceneNameC(AnimationScene* source)
        {
            return source->sceneName.c_str();
        }
        SkeletonRig* GetSkeletonRigC(AnimationScene* source)
        {
            return &source->rig;
        }
        bool HasSkeletonRigC(AnimationScene* source)
        {
            return !source->rig.boneEntries.empty();
        }
        AnimationScene* UNIV::CreateAnimationSceneC(const char* sceneName)
        {
            UNIV::AnimationScene* univAnimationScene = new UNIV::AnimationScene;
            univAnimationScene->sceneName = sceneName;

            return univAnimationScene;
        }
        bool UNIV::AddRigToAnimationSceneC(AnimationScene* scene, SkeletonRig* rig, Utilities::StringContainer* errorMessage)
        {
            errorMessage->Clear();

            if (rig->boneEntries.empty())
            {
                *errorMessage += "[CALUMI.Animation API] No Bone Entries Found In Rig!";
                return false;
            }
            scene->rig = *rig;
            rig = nullptr;
            *errorMessage += "[CALUMI.Animation API] Rig Data Transferred Successfully. Original Ptr Set To Null!";
            return true;
        }
        bool UNIV::AddAnimationToAnimationSceneC(AnimationScene* scene, Animation* animation, bool overwrite, Utilities::StringContainer* errorMessage)
        {
            errorMessage->Clear();

            if (animation->animationTitle == "")
            {
                *errorMessage += "[CALUMI.Animation API] Must Have Animation Title!";
                return false;
            }
            for (unsigned int i =0; i<scene->animations.size(); i++)
            {
                if (animation->animationTitle == scene->animations.at(i).animationTitle)
                {
                    if(!overwrite)
                    {
                        *errorMessage += "[CALUMI.Animation API] Animation Titles Must Be Unique!";
                        return false;
                    }
                    else
                    {
                        scene->animations.erase(i);
                        break;
                    }
                    
                }
            }
            scene->animations.push_back(*animation);
            delete animation;

            *errorMessage += "[CALUMI.Animation API] Animation Data Copied Into Animation Vector Successfully. Original Ptr Has Been Deleted!";
            return true;
        }
        
        bool AnimationScene::AddAnimationToScene(UNIV::Animation& animation, bool overwrite)
        {
            for (unsigned int i = 0; i < animations.size(); i++)
            {
                if (animations.at(i).animationTitle == animation.animationTitle)
                {
                    if(!overwrite)
                    return false;
                    else
                    {
                        animations.erase(i);
                        break;
                    }
                }
            }
            animations.push_back(animation);
            return true;
        }

        bool AnimationScene::RemoveAnimationFromScene(Utilities::StringContainer& sceneToRemove)
        {
            for (unsigned int i = 0; i < animations.size(); i++)
            {
                if (animations.at(i).animationTitle == sceneToRemove)
                {
                    animations.erase(i);
                    return true;
                }
            }
            return false;
        }

        bool AnimationScene::RemoveAnimationFromScene(unsigned int idx)
        {
            if (idx >= animations.size() || idx < 0 ) return false;

            animations.erase(idx);
            return true;
        }

        Utilities::ExpectedConatiner<Utilities::VectorContainer<Utilities::PathContainer>, Utilities::StringContainer> AnimationScene::GetFilePathsFromAnimationScene(const wchar_t* directoryPath, const char* extension)
        {
            Utilities::VectorContainer<Utilities::PathContainer> animationFilePaths;
            animationFilePaths.reserve(animations.size());
            for (unsigned int i = 0; i < animations.size(); i++)
            {
                if (animations.at(i).animationTitle.Empty())
                {
                    Utilities::ExpectedConatiner<Utilities::VectorContainer<Utilities::PathContainer>, Utilities::StringContainer> tempOutput;
                    tempOutput.SetErrorValue("[CALUMI.Animation API] Empty string found for Animation Title");
                    return tempOutput;
                }
                Utilities::PathContainer pathToAdd(directoryPath);
                pathToAdd /= "animations";
                pathToAdd /= sceneName.c_str();
                pathToAdd /= animations.at(i).animationTitle;
                pathToAdd.replace_extension(extension);
                animationFilePaths.push_back(pathToAdd);
            }

            return animationFilePaths;
        }

        Utilities::StringContainer AnimationScene::ToJSON(const size_t indents = 0) const {
            Utilities::StringContainer output;
            output += Utilities::Indent(indents).c_str();
            output += "{\n"; 
            output += Utilities::Indent(indents + 1).c_str();
            output += "\"sceneName\":\"";
            output += sceneName.c_str();
            output += "\",\n";
            output += Utilities::Indent(indents + 1).c_str();
            output += "\"animations\":";
            output += Utilities::VectorToJSON(animations);
            output += ",\n";
            output += Utilities::Indent(indents + 1).c_str();
            output += "\"rig\":\n";
            output += rig.ToJSON(indents + 1).c_str();
            output += Utilities::Indent(indents + 1).c_str();
            output += "\n}";
            return output;
        }


    }
}

