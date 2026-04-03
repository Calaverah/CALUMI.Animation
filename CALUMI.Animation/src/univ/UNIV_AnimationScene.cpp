//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalplatform.h"
#include "utilities/CALUMI_Utilities.h"
#include "univ/animation/UNIV_Animation.h"
#include "univ/UNIV_AnimationScene.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"


namespace CALUMI {
    namespace UNIV {

#pragma region ANIMATIONSCENE
        struct AnimationScene::Impl
        {
            Utilities::StringContainer sceneName = "MyAnimationScene";
            Utilities::VectorContainer<Animation> animations;
            SkeletonRig rig;
            Impl() = default;
        };

        AnimationScene::AnimationScene() { pImpl = new Impl; }
        AnimationScene::~AnimationScene() { if (pImpl) delete pImpl; }
        AnimationScene::AnimationScene(const AnimationScene& input) : AnimationScene() { *pImpl = *(input.pImpl); }
        AnimationScene::AnimationScene(const Utilities::StringContainer& sceneName) : AnimationScene() { pImpl->sceneName = sceneName; }
        AnimationScene::AnimationScene(const char* sceneName) : AnimationScene() { pImpl->sceneName = sceneName; }

        AnimationScene& AnimationScene::operator=(const AnimationScene& input) { *pImpl = *(input.pImpl); return *this; }

        SkeletonRig& AnimationScene::Rig() const { return pImpl->rig; }
        Utilities::VectorContainer<Animation>& AnimationScene::Animations() const { return pImpl->animations; }
        const char* AnimationScene::SceneName() const { return pImpl->sceneName.c_str(); }
        void AnimationScene::SceneName(const char* name) { pImpl->sceneName = name; }
        void AnimationScene::SceneName(const Utilities::StringContainer& input) { pImpl->sceneName = input; }

        bool AnimationScene::AddAnimationToScene(UNIV::Animation& animation, bool overwrite)
        {
            for (unsigned int i = 0; i < Animations().size(); i++)
            {
                if (SCOMPARE(pImpl->animations.at(i).AnimationTitle(), animation.AnimationTitle()) == 0)
                {
                    if (!overwrite)
                        return false;
                    else
                    {
                        pImpl->animations.erase(i);
                        break;
                    }
                }
            }
            pImpl->animations.push_back(animation);
            return true;
        }

        bool AnimationScene::RemoveAnimationFromScene(Utilities::StringContainer& sceneToRemove)
        {
            for (unsigned int i = 0; i < pImpl->animations.size(); i++)
            {
                if (SCOMPARE(pImpl->animations.at(i).AnimationTitle(), sceneToRemove.c_str()) == 0)
                {
                    pImpl->animations.erase(i);
                    return true;
                }
            }
            return false;
        }

        bool AnimationScene::RemoveAnimationFromScene(unsigned int idx)
        {
            if (idx >= pImpl->animations.size() || idx < 0) return false;

            pImpl->animations.erase(idx);
            return true;
        }

        Utilities::ExpectedContainer<Utilities::VectorContainer<Utilities::PathContainer>, Utilities::StringContainer> AnimationScene::GetFilePathsFromAnimationScene(const wchar_t* directoryPath, const char* extension)
        {
            Utilities::VectorContainer<Utilities::PathContainer> animationFilePaths;
            animationFilePaths.reserve(pImpl->animations.size());
            for (unsigned int i = 0; i < pImpl->animations.size(); i++)
            {
                if (SCOMPARE(pImpl->animations.at(i).AnimationTitle(), "") == 0)
                {
                    Utilities::ExpectedContainer<Utilities::VectorContainer<Utilities::PathContainer>, Utilities::StringContainer> tempOutput;
                    tempOutput.setErrorValue("[CALUMI.Animation API] Empty string found for Animation Title");
                    return tempOutput;
                }
                Utilities::PathContainer pathToAdd(directoryPath);
                //pathToAdd /= "animations";
                //pathToAdd /= pImpl->sceneName.c_str();
                pathToAdd /= pImpl->animations.at(i).AnimationTitle();
                pathToAdd.replace_extension(extension);
                animationFilePaths.push_back(pathToAdd);
            }

            return animationFilePaths;
        }

        Utilities::StringContainer AnimationScene::ToJSON(const std::size_t indents = 0) const {
            Utilities::StringContainer output;
            output += Utilities::Indent(indents).c_str();
            output += "{\n";
            output += Utilities::Indent(indents + 1).c_str();
            output += "\"sceneName\":\"";
            output += pImpl->sceneName.c_str();
            output += "\",\n";
            output += Utilities::Indent(indents + 1).c_str();
            output += "\"animations\":";
            // output += Utilities::VectorToJSON(pImpl->animations);
            output += ",\n";
            output += Utilities::Indent(indents + 1).c_str();
            output += "\"rig\":\n";
            output += pImpl->rig.ToJSON(indents + 1).c_str();
            output += Utilities::Indent(indents + 1).c_str();
            output += "\n}";
            return output;
        }

#pragma endregion

#pragma region EXTERN"C"
        //Extern C Functions
        bool DeleteAnimationSceneC(AnimationScene* ptr)
        {
            if (ptr)
            {
                delete ptr;
                ptr = nullptr;
                return true;
            }
            return false;
        }
        Animation* GetAnimationC(AnimationScene* source, int index, Utilities::StringContainer* errorMessage)
        {
            Utilities::StringContainer tempErrorMessage;
            Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
            errorMessageHolder->clear();

            if (source->Animations().size() <= index)
            {
                *errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
                return nullptr;
            }
            return &source->Animations().at(index);
        }
        std::size_t GetAnimationCountC(AnimationScene* source)
        {
            return source->Animations().size();
        }
        const char* GetAnimationSceneNameC(AnimationScene* source)
        {
            return source->SceneName();
        }
        SkeletonRig* GetSkeletonRigC(AnimationScene* source)
        {
            return &source->Rig();
        }
        bool HasSkeletonRigC(AnimationScene* source)
        {
            return !source->Rig().BoneEntries().empty();
        }
        AnimationScene* CreateAnimationSceneC(const char* sceneName)
        {
            UNIV::AnimationScene* univAnimationScene = new UNIV::AnimationScene;
            univAnimationScene->SceneName(sceneName);

            return univAnimationScene;
        }
        bool AddRigToAnimationSceneC(AnimationScene* scene, SkeletonRig* rig, Utilities::StringContainer* errorMessage)
        {
            Utilities::StringContainer tempErrorMessage;
            Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
            errorMessageHolder->clear();

            if (rig->BoneEntries().empty())
            {
                *errorMessageHolder += "[CALUMI.Animation API] No Bone Entries Found In Rig!";
                return false;
            }
            scene->Rig() = *rig;

            if (rig)
                DeleteSkeletonRigC(rig);

            *errorMessageHolder += "[CALUMI.Animation API] Rig Data Copied Successfully. Original Rig Has Been Deleted!";
            return true;
        }
        bool AddAnimationToAnimationSceneC(AnimationScene* scene, Animation* animation, bool overwrite, Utilities::StringContainer* errorMessage)
        {
            Utilities::StringContainer tempErrorMessage;
            Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
            errorMessageHolder->clear();

            if (SCOMPARE(animation->AnimationTitle(), "") == 0)
            {
                *errorMessageHolder += "[CALUMI.Animation API] Must Have Animation Title!";
                return false;
            }
            for (unsigned int i = 0; i < scene->Animations().size(); i++)
            {
                if (SCOMPARE(animation->AnimationTitle(), scene->Animations().at(i).AnimationTitle()) == 0)
                {
                    if (!overwrite)
                    {
                        *errorMessageHolder += "[CALUMI.Animation API] Animation Titles Must Be Unique!";
                        return false;
                    }
                    else
                    {
                        scene->Animations().erase(i);
                        break;
                    }

                }
            }
            scene->Animations().push_back(*animation);
            if (animation)
                delete animation;

            *errorMessageHolder += "[CALUMI.Animation API] Animation Data Copied Into Animation Vector Successfully. Original Ptr Has Been Deleted!";
            return true;
        }
#pragma endregion



    }
}
