//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppExpressionWithoutSideEffects
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
    AnimationScene::~AnimationScene()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }
    AnimationScene::AnimationScene(const AnimationScene& input) : AnimationScene() { *pImpl = *input.pImpl; }
    AnimationScene::AnimationScene(const Utilities::StringContainer& sceneName) : AnimationScene() { pImpl->sceneName = sceneName; }
    AnimationScene::AnimationScene(const char* sceneName) : AnimationScene() { pImpl->sceneName = sceneName; }

    AnimationScene& AnimationScene::operator=(const AnimationScene& input)
    {
        if (this != &input)
            *pImpl = *input.pImpl;

        return *this;
    }

    const SkeletonRig& AnimationScene::rig() const
    {
        return pImpl->rig;
    }
    void AnimationScene::setRig(const SkeletonRig& skeletonRig) const
    {
        pImpl->rig = skeletonRig;
    }

    Animation* AnimationScene::animation(const uint64_t idx) const
    {
        if (idx < pImpl->animations.size())
            return &pImpl->animations.at(idx);

        return nullptr;
    }

    Animation* AnimationScene::animation(const char* name) const
    {
        for (auto& animation : pImpl->animations)
        {
            if (SCOMPARE(animation.animationTitle(), name) == 0)
                return &animation;
        }

        return nullptr;
    }

    uint64_t AnimationScene::animationCount() const { return pImpl->animations.size(); }
    const char* AnimationScene::sceneName() const { return pImpl->sceneName.c_str(); }
    void AnimationScene::setSceneName(const char* name) const { pImpl->sceneName = name; }
    void AnimationScene::setSceneName(const Utilities::StringContainer& input) const { pImpl->sceneName = input; }

    bool AnimationScene::addAnimationToScene(const Animation& animation, const bool overwrite) const
    {
        for (unsigned int i = 0; i < pImpl->animations.size(); i++)
        {
            if (SCOMPARE(pImpl->animations.at(i).animationTitle(), animation.animationTitle()) == 0)
            {
                if (!overwrite)
                    return false;

                pImpl->animations.erase(pImpl->animations.begin() + i);
                break;

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
}
#pragma endregion

#pragma region EXTERN "C"

CALUMI::UNIV::AnimationScene* CreateAnimationSceneC(const char* sceneName)
{
    const auto univAnimationScene = new CALUMI::UNIV::AnimationScene;
    univAnimationScene->setSceneName(sceneName);

    return univAnimationScene;
}

int AddRigToAnimationSceneC(const CALUMI::UNIV::AnimationScene* scene, const CALUMI::UNIV::SkeletonRig* rig)
{
    if (scene && rig)
        try
        {
            scene->setRig(*rig);
            return 0;
        }
    catch (std::exception&)
    {
    }
    return -1;
}

int AddAnimationToAnimationSceneC(const CALUMI::UNIV::AnimationScene* scene, const CALUMI::UNIV::Animation* animation, const bool overwrite)
{
    if (scene && animation)
        try
        {
            return scene->addAnimationToScene(*animation, overwrite) ? 0 : 1;
        }
    catch (std::exception&)
    {
    }
    return -1;
}

int DeleteAnimationSceneC(const CALUMI::UNIV::AnimationScene** ptr)
{
    if (ptr && *ptr)
    {
        delete *ptr;
        *ptr = nullptr;
        return 0;
    }
    return -1;
}

CALUMI::UNIV::Animation* GetAnimationWithIndexC(const CALUMI::UNIV::AnimationScene* source, const int index)
{
    if (source)
        try
        {
            return source->animation(index);
        }
    catch (std::exception&)
    {
    }
    return nullptr;
}

CALUMI::UNIV::Animation* GetAnimationWithNameC(const CALUMI::UNIV::AnimationScene* source, const char* name)
{
    if (source)
        try
        {
            return source->animation(name);
        }
    catch (std::exception&)
    {
    }
    return nullptr;
}

uint64_t GetAnimationCountC(const CALUMI::UNIV::AnimationScene* source)
{
    if (source)
        return source->animationCount();

    return 0;
}

const char* GetAnimationSceneNameC(const CALUMI::UNIV::AnimationScene* source)
{
    if (source)
        return source->sceneName();

    return nullptr;
}

const CALUMI::UNIV::SkeletonRig* GetSkeletonRigC(const CALUMI::UNIV::AnimationScene* source)
{
    if (source)
        return &source->rig();

    return nullptr;
}

#pragma endregion
