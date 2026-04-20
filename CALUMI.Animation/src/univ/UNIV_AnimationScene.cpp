//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalplatform.h"
#include "utilities/CALUMI_Utilities.h"
#include "univ/animation/UNIV_Animation.h"
#include "univ/UNIV_AnimationScene.h"
#include "univ/skeletonrig/UNIV_SkeletonRig.h"

#include <vector>

namespace CALUMI::UNIV
{

#pragma region ANIMATIONSCENE
    struct AnimationScene::Impl
    {
        Utilities::StringContainer sceneName = "MyAnimationScene";
        std::vector<Animation> animations;
        SkeletonRig rig;
        Impl() = default;
    };

    AnimationScene::AnimationScene() { pImpl = new Impl; }
    AnimationScene::~AnimationScene() { if (pImpl) delete pImpl; }
    AnimationScene::AnimationScene(const AnimationScene& input) : AnimationScene() { *pImpl = *(input.pImpl); }
    AnimationScene::AnimationScene(const Utilities::StringContainer& sceneName) : AnimationScene() { pImpl->sceneName = sceneName; }
    AnimationScene::AnimationScene(const char* sceneName) : AnimationScene() { pImpl->sceneName = sceneName; }

    AnimationScene& AnimationScene::operator=(const AnimationScene& input) { *pImpl = *(input.pImpl); return *this; }

    SkeletonRig& AnimationScene::rig() const { return pImpl->rig; }
    Animation& AnimationScene::animation(uint64_t idx) const { return pImpl->animations.at(idx); }
    uint64_t AnimationScene::animationCount() const { return pImpl->animations.size(); }
    const char* AnimationScene::sceneName() const { return pImpl->sceneName.c_str(); }
    void AnimationScene::setSceneName(const char* name) { pImpl->sceneName = name; }
    void AnimationScene::setSceneName(const Utilities::StringContainer& input) { pImpl->sceneName = input; }

    bool AnimationScene::addAnimationToScene(UNIV::Animation& animation, bool overwrite)
    {
        for (unsigned int i = 0; i < pImpl->animations.size(); i++)
        {
            if (SCOMPARE(pImpl->animations.at(i).animationTitle(), animation.animationTitle()) == 0)
            {
                if (!overwrite)
                    return false;
                else
                {
                    pImpl->animations.erase(pImpl->animations.begin() + i);
                    break;
                }
            }
        }
        pImpl->animations.push_back(animation);
        return true;
    }

    bool AnimationScene::removeAnimationFromScene(const Utilities::StringContainer& sceneToRemove) const
    {
        for (unsigned int i = 0; i < pImpl->animations.size(); i++)
        {
            if (SCOMPARE(pImpl->animations.at(i).animationTitle(), sceneToRemove.c_str()) == 0)
            {
                pImpl->animations.erase(pImpl->animations.begin() + i);
                return true;
            }
        }
        return false;
    }

    bool AnimationScene::removeAnimationFromScene(const unsigned int idx) const
    {
        if (idx >= pImpl->animations.size())
            return false;

        pImpl->animations.erase(pImpl->animations.begin() + idx);
        return true;
    }

    //Utilities::ExpectedContainer<Utilities::VectorContainer<Utilities::PathContainer>, Utilities::StringContainer> AnimationScene::GetFilePathsFromAnimationScene(const wchar_t* directoryPath, const char* extension)
    //{
    //    //Utilities::VectorContainer<Utilities::PathContainer> animationFilePaths;
    //    //animationFilePaths.reserve(pImpl->animations.size());
    //    //for (unsigned int i = 0; i < pImpl->animations.size(); i++)
    //    //{
    //    //    if (SCOMPARE(pImpl->animations.at(i).AnimationTitle(), "") == 0)
    //    //    {
    //    //        Utilities::ExpectedContainer<Utilities::VectorContainer<Utilities::PathContainer>, Utilities::StringContainer> tempOutput;
    //    //        tempOutput.setErrorValue("[CALUMI.Animation API] Empty string found for Animation Title");
    //    //        return tempOutput;
    //    //    }
    //    //    Utilities::PathContainer pathToAdd(directoryPath);
    //    //    //pathToAdd /= "animations";
    //    //    //pathToAdd /= pImpl->sceneName.c_str();
    //    //    pathToAdd /= pImpl->animations.at(i).AnimationTitle();
    //    //    pathToAdd.replace_extension(extension);
    //    //    animationFilePaths.push_back(pathToAdd);
    //    //}
    //
    //    return animationFilePaths;
    //}

    Utilities::StringContainer AnimationScene::toJSON(const uint64_t indents = 0) const {
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
        output += pImpl->rig.toJSON(indents + 1).c_str();
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

        if (source->animationCount() <= index)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Input Index Exceeds Vector Entries";
            return nullptr;
        }
        return &source->animation(index);
    }
    uint64_t GetAnimationCountC(const AnimationScene* source)
    {
        return source->animationCount();
    }
    const char* GetAnimationSceneNameC(const AnimationScene* source)
    {
        return source->sceneName();
    }
    SkeletonRig* GetSkeletonRigC(const AnimationScene* source)
    {
        return &source->rig();
    }
    AnimationScene* CreateAnimationSceneC(const char* sceneName)
    {
        const auto univAnimationScene = new CALUMI::UNIV::AnimationScene;
        univAnimationScene->setSceneName(sceneName);

        return univAnimationScene;
    }
    bool AddRigToAnimationSceneC(AnimationScene* scene, SkeletonRig* rig, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        scene->rig() = *rig;

        return true;
    }
    bool AddAnimationToAnimationSceneC(AnimationScene* scene, Animation* animation, bool overwrite, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (SCOMPARE(animation->animationTitle(), "") == 0)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Must Have Animation Title!";
            return false;
        }
        for (unsigned int i = 0; i < scene->animationCount(); i++)
        {
            if (SCOMPARE(animation->animationTitle(), scene->animation(i).animationTitle()) == 0)
            {
                if (!overwrite)
                {
                    *errorMessageHolder += "[CALUMI.Animation API] Animation Titles Must Be Unique!";
                    return false;
                }
                else
                {
                    scene->removeAnimationFromScene(i);
                    break;
                }

            }
        }
        scene->addAnimationToScene(*animation);

        delete animation;
        animation = nullptr;

        *errorMessageHolder += "[CALUMI.Animation API] Animation Data Copied Into Animation Vector Successfully. Original Ptr Has Been Deleted!";
        return true;
    }
#pragma endregion



}
