//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppExpressionWithoutSideEffects
#include "internalplatform.h"
#include "sfbgs/SFBGS_AnimationScene.h"
#include "sfbgs/animation/SFBGS_AnimationPackage.h"
#include "utilities/CALUMI_Hashes.h"

#include "io/FileValidation.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <vector>


namespace CALUMI::SFBGS
{
    struct AnimationScene::Impl
    {
        Utilities::StringContainer sceneName = "MyStarfieldAnimationScene";
        std::vector<Animation>     animations;
        SkeletonRig                rig;
        Impl() = default;
    };

    Utilities::StringContainer& AnimationScene::sceneName() const { return pImpl->sceneName; }
    Animation& AnimationScene::animation(const uint64_t idx) const { return pImpl->animations.at(idx); }
    uint64_t AnimationScene::animationCount() const { return pImpl->animations.size(); }
    SkeletonRig& AnimationScene::rig() const { return pImpl->rig; }
    void AnimationScene::setRig(const SkeletonRig& rig) const { pImpl->rig = rig; }

    AnimationScene::AnimationScene() : pImpl(new Impl)
    {
    }

    AnimationScene::~AnimationScene()
    {
        if (pImpl)
        {
            delete pImpl;
            pImpl = nullptr;
        }
    }

    AnimationScene::AnimationScene(const char* sceneName) : AnimationScene()
    {
        pImpl->sceneName = sceneName;
    }

    AnimationScene::AnimationScene(const Utilities::StringContainer& sceneName) : AnimationScene()
    {
        pImpl->sceneName = sceneName;
    }

    AnimationScene::AnimationScene(const AnimationScene& input) : AnimationScene()
    {
        *this = input;
    }

    AnimationScene& AnimationScene::operator=(const AnimationScene& input)
    {
        if (this != &input)
            pImpl->animations = input.pImpl->animations;
        pImpl->rig       = input.pImpl->rig;
        pImpl->sceneName = input.pImpl->sceneName;

        return *this;
    }

    //Interface Methods For Animation Vector
    bool AnimationScene::addAnimationToScene(const Animation& animation, const bool overwrite) const
    {
        for (unsigned int i = 0; i < pImpl->animations.size(); i++)
        {
            if (pImpl->animations.at(i).animationFileName() == animation.animationFileName())
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
        return removeAnimationFromScene(sceneToRemove.c_str());
    }

    bool AnimationScene::removeAnimationFromScene(const char* sceneToRemove) const
    {
        for (unsigned int i = 0; i < pImpl->animations.size(); i++)
        {
            if (pImpl->animations.at(i).animationFileName() == sceneToRemove)
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


    //Animation Conversion
    static Animation ConvertToSFBGSAnimation(const UNIV::Animation& anim, const SkeletonRig& rig)
    {
        Animation output;
        output.setAnimationFileName(anim.animationTitle());

        output.setBoneCount(rig.boneCount());
        //Just listing all bones for now. Will need compression algorithm eventually
        output.setFrameCount(anim.frameCount());

        output.indexAtlas().reserve(output.indexAtlasCount());
        output.indexAtlas().push_back(0);
        output.indexAtlas().push_back(output.boneCount());


        output.animationBlocks().resize(output.boneCount()); //Create empty blocks for all bones
        for (unsigned int i = 0; i < anim.animationBlocks().size(); i++)
        {
            AnimationBlock toAdd;
            toAdd.setRotationSequenceFromUNIV(anim.animationBlocks().at(i).rotationSequence());
            toAdd.setTranslationSequenceFromUNIV(anim.animationBlocks().at(i).translationSequence(),
                                                 rig.highPrecision(), rig.lowPrecision());
            toAdd.setScalarSequenceFromUNIV(anim.animationBlocks().at(i).scalarSequence());
            toAdd.setPrioritySequenceFromUNIV(anim.animationBlocks().at(i).prioritySequence());

            if (const int boneIdx = rig.findBoneIndex(anim.animationBlocks().at(i).boneName()); boneIdx >= 0 && boneIdx
                < output.animationBlocks().size())
                output.animationBlocks().at(boneIdx) = toAdd; //Fill block for the correct index
        }

        if (const auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(anim.packageManager().package(
         SFBGS_ANIM_PACKAGE)))
        {
            for (int i = 0; i < pkg->amendedBlockCount() && i < std::numeric_limits<int>::max(); i++)
            {
                AnimationBlock toAdd;
                toAdd.setRotationSequenceFromUNIV(pkg->amendedBlock(i)->rotationSequence());

                float hiVal = pkg->usesRigForPrecision() ? rig.highPrecision() : pkg->overridePrecisionSet().high();
                float loVal = pkg->usesRigForPrecision() ? rig.lowPrecision() : pkg->overridePrecisionSet().low();

                toAdd.setTranslationSequenceFromUNIV(pkg->amendedBlock(i)->translationSequence(), hiVal, loVal);

                toAdd.setScalarSequenceFromUNIV(pkg->amendedBlock(i)->scalarSequence());
                toAdd.setPrioritySequenceFromUNIV(pkg->amendedBlock(i)->prioritySequence());
            }
        }

        output.evaluateHeaderFlags();
        return output;
    }

    static UNIV::Animation ConvertToUniversalAnimation(const Animation& anim, const SkeletonRig& rig)
    {
        UNIV::Animation output;
        output.setAnimationTitle(anim.animationFileName());
        //output.boneCount = anim._boneCount;

        output.animationBlocks().reserve(anim.animationBlocks().size());
        for (unsigned int i = 0; i < anim.animationBlocks().size(); i++)
        {
            if (anim.animationBlocks().at(i).rotationCount() || anim.animationBlocks().at(i).translationCount() || anim.
                animationBlocks().at(i).scalarCount() || anim.animationBlocks().at(i).priorityCount())
            {
                UNIV::AnimationBlock  toAdd;
                const AnimationBlock& refBlock = anim.animationBlocks().at(i);
                //toAdd.setBoneIndex(i);
                toAdd.setBoneName(rig.stringArray().c_str(i));

                toAdd.scalarSequence()      = refBlock.scalarSequenceAsUNIV();
                toAdd.prioritySequence()    = refBlock.prioritySequenceAsUNIV();
                toAdd.translationSequence() = refBlock.translationSequenceAsUNIV(
                 rig.highPrecision(), rig.lowPrecision());
                toAdd.rotationSequence() = refBlock.rotationSequenceAsUNIV();

                output.animationBlocks().push_back(toAdd);
            }
        }
        output.animationBlocks().shrink_to_fit();

        //Amended animation blocks (NiNode bones)
        SFBGS_AnimationPackage::AddPackage(output, true);

        if (anim.verifyAmendedBlocks())
        {
            const auto& amendedBlocks = anim.amendedAnimationBlocks();
            auto&       hashSet       = anim.amendedHashSet();

            if (const auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(output.packageManager().package(
             SFBGS_ANIM_PACKAGE)))
            {
                for (uint64_t i = 0; i < amendedBlocks.size(); i++)
                {
                    const uint32_t       hash = hashSet.at(i);
                    UNIV::AnimationBlock toAdd;
                    AnimationBlock       refBlock = amendedBlocks.at(i);

                    std::string tempName = Utilities::HashRegistry::Get().isKnownHash(hash) ?
                                               Utilities::HashRegistry::Get().knownHashString(hash) :
                                               "Unknown_Hash_" + std::to_string(i);
                    toAdd.setBoneName(tempName.c_str());

                    toAdd.scalarSequence()      = refBlock.scalarSequenceAsUNIV();
                    toAdd.prioritySequence()    = refBlock.prioritySequenceAsUNIV();
                    toAdd.translationSequence() = refBlock.translationSequenceAsUNIV(
                     rig.highPrecision(), rig.lowPrecision());
                    toAdd.rotationSequence() = refBlock.rotationSequenceAsUNIV();

                    pkg->addAmendedBlock(hash, toAdd);
                }
            }
        }

        return output;
    }

    //CONVERSION PARENT FUNCTIONS
    bool AnimationScene::convertFromUniversalScene(UNIV::AnimationScene& input)
    {
        pImpl->sceneName = input.sceneName();
        pImpl->rig.convertFromUniversalRig(input.rig());

        pImpl->animations.reserve(input.animationCount());
        for (unsigned int i = 0; i < input.animationCount(); i++)
        {
            pImpl->animations.push_back(ConvertToSFBGSAnimation(*input.animation(i), pImpl->rig));
        }

        return true;
    }

    //Converting a scene overloaded so that an existing converted rig can be used
    bool AnimationScene::convertFromUniversalScene(const UNIV::AnimationScene& input,
                                                   const SkeletonRig&          rigReference) const
    {
        if (rigReference.boneEntries().empty())
            return false;


        pImpl->sceneName = input.sceneName();
        pImpl->rig       = rigReference;

        pImpl->animations.reserve(input.animationCount());
        for (unsigned int i = 0; i < input.animationCount(); i++)
        {
            pImpl->animations.push_back(ConvertToSFBGSAnimation(*input.animation(i), pImpl->rig));
        }

        return true;
    }

    UNIV::AnimationScene AnimationScene::convertToUniversalScene()
    {
        UNIV::AnimationScene outputScene;
        if (pImpl->rig.boneEntries().empty())
            return outputScene;

        outputScene.setSceneName(pImpl->sceneName);
        outputScene.setRig(pImpl->rig.convertToUniversalRig());

        //outputScene.Animations().reserve(pImpl->animations.size());
        for (const auto& animation : pImpl->animations)
        {
            auto newAnim = ConvertToUniversalAnimation(animation, pImpl->rig);
            outputScene.addAnimationToScene(newAnim);
        }

        return outputScene;
    }
}

//EXTERN C =============================================================================================================


int SaveAnimationSceneToSFBGSFormatPathOverrideC(CALUMI::UNIV::AnimationScene* scene,
                                                 const wchar_t**               directoryPathArray,
                                                 const uint64_t                arraySize)
{
    if (arraySize < 1 || !directoryPathArray || !directoryPathArray[0])
    {
        return -1;
    }

    if (scene)
        try
        {
            std::vector<std::filesystem::path> animationFilePaths;
            std::filesystem::path              rigFilePath;

            for (unsigned int i = 0; i < arraySize; i++)
            {
                if (std::filesystem::path pathToAdd(directoryPathArray[i]); pathToAdd.extension() == ".rig")
                    rigFilePath = pathToAdd;
                else
                    animationFilePaths.push_back(pathToAdd);
            }
            animationFilePaths.shrink_to_fit();

            if (rigFilePath.empty())
            {
                return 1;
            }

            CALUMI::SFBGS::AnimationScene translatedScene;
            if (!translatedScene.convertFromUniversalScene(*scene))
            {
                return 2;
            }

            if (const auto rigResult = translatedScene.rig().writeToFile(rigFilePath.c_str()); rigResult.hasError())
            {
                switch (rigResult.fileErrorCode())
                {
                case CALUMI::Utilities::FileResult::FileErrorCode::FileNotFound:
                    return 10;
                case CALUMI::Utilities::FileResult::FileErrorCode::PermissionDenied:
                    return 11;
                case CALUMI::Utilities::FileResult::FileErrorCode::NotAFile:
                    return 12;
                case CALUMI::Utilities::FileResult::FileErrorCode::ReadFailure:
                    return 13;
                case CALUMI::Utilities::FileResult::FileErrorCode::IncorrectFileType:
                    return 14;
                case CALUMI::Utilities::FileResult::FileErrorCode::FileTooLarge:
                    return 15;
                case CALUMI::Utilities::FileResult::FileErrorCode::FileTooSmall:
                    return 16;
                case CALUMI::Utilities::FileResult::FileErrorCode::UnknownErrorCode:
                    return 17;
                case CALUMI::Utilities::FileResult::FileErrorCode::WriteFailure:
                    return 18;
                default:
                    return 19;
                }
            }

            const uint64_t minSize = std::min(translatedScene.animationCount(), arraySize - 1);

            for (int i = 0; i < minSize; i++)
            {
                if (auto animResult = translatedScene.animation(i).writeToFile(animationFilePaths.at(i).c_str());
                    animResult.hasError())
                {
                    switch (animResult.fileErrorCode())
                    {
                    case CALUMI::Utilities::FileResult::FileErrorCode::FileNotFound:
                        return 20;
                    case CALUMI::Utilities::FileResult::FileErrorCode::PermissionDenied:
                        return 21;
                    case CALUMI::Utilities::FileResult::FileErrorCode::NotAFile:
                        return 22;
                    case CALUMI::Utilities::FileResult::FileErrorCode::ReadFailure:
                        return 23;
                    case CALUMI::Utilities::FileResult::FileErrorCode::IncorrectFileType:
                        return 24;
                    case CALUMI::Utilities::FileResult::FileErrorCode::FileTooLarge:
                        return 25;
                    case CALUMI::Utilities::FileResult::FileErrorCode::FileTooSmall:
                        return 26;
                    case CALUMI::Utilities::FileResult::FileErrorCode::UnknownErrorCode:
                        return 27;
                    case CALUMI::Utilities::FileResult::FileErrorCode::WriteFailure:
                        return 28;
                    default:
                        return 29;
                    }
                }
            }

            return 0;
        }
        catch (std::exception&)
        {
        }

    return -1;
}

//Warning, desired directory _path array must be in the same order as the animations in the scene.
int SaveAnimationSceneToSFBGSFormatUsingRigReferencePathOverrideC(const CALUMI::UNIV::AnimationScene* scene,
                                                                  const
                                                                  wchar_t**      directoryPathArray,
                                                                  const uint64_t arraySize,
                                                                  const wchar_t* sfbgsRigPath)
{
    if (arraySize < 1 || !directoryPathArray || !directoryPathArray[0])
    {
        return -1;
    }

    if (scene)
        try
        {
            const std::filesystem::path rigPath(sfbgsRigPath);

            if (!std::filesystem::exists(rigPath))
                return 1;

            CALUMI::SFBGS::SkeletonRig sfbgsRig;

            if (const auto rigResult = sfbgsRig.readFromFile(rigPath.c_str()); rigResult.hasError())
            {
                switch (rigResult.fileErrorCode())
                {
                case CALUMI::Utilities::FileResult::FileErrorCode::FileNotFound:
                    return 10;
                case CALUMI::Utilities::FileResult::FileErrorCode::PermissionDenied:
                    return 11;
                case CALUMI::Utilities::FileResult::FileErrorCode::NotAFile:
                    return 12;
                case CALUMI::Utilities::FileResult::FileErrorCode::ReadFailure:
                    return 13;
                case CALUMI::Utilities::FileResult::FileErrorCode::IncorrectFileType:
                    return 14;
                case CALUMI::Utilities::FileResult::FileErrorCode::FileTooLarge:
                    return 15;
                case CALUMI::Utilities::FileResult::FileErrorCode::FileTooSmall:
                    return 16;
                case CALUMI::Utilities::FileResult::FileErrorCode::UnknownErrorCode:
                    return 17;
                case CALUMI::Utilities::FileResult::FileErrorCode::WriteFailure:
                    return 18;
                default:
                    return 19;
                }
            }

            std::vector<std::filesystem::path> animationFilePaths;
            animationFilePaths.reserve(arraySize);

            for (unsigned int i = 0; i < arraySize; i++)
            {
                std::filesystem::path pathToAdd(directoryPathArray[i]);
                animationFilePaths.push_back(pathToAdd);
            }
            animationFilePaths.shrink_to_fit();

            const CALUMI::SFBGS::AnimationScene translatedScene;
            if (!translatedScene.convertFromUniversalScene(*scene, sfbgsRig))
            {
                return 2;
            }

            const uint64_t minSize = std::min(translatedScene.animationCount(), arraySize - 1);
            for (unsigned int i = 0; i < minSize; i++)
            {
                if (auto animResult = translatedScene.animation(i).writeToFile(animationFilePaths.at(i).c_str());
                    animResult.hasError())
                {
                    switch (animResult.fileErrorCode())
                    {
                    case CALUMI::Utilities::FileResult::FileErrorCode::FileNotFound:
                        return 20;
                    case CALUMI::Utilities::FileResult::FileErrorCode::PermissionDenied:
                        return 21;
                    case CALUMI::Utilities::FileResult::FileErrorCode::NotAFile:
                        return 22;
                    case CALUMI::Utilities::FileResult::FileErrorCode::ReadFailure:
                        return 23;
                    case CALUMI::Utilities::FileResult::FileErrorCode::IncorrectFileType:
                        return 24;
                    case CALUMI::Utilities::FileResult::FileErrorCode::FileTooLarge:
                        return 25;
                    case CALUMI::Utilities::FileResult::FileErrorCode::FileTooSmall:
                        return 26;
                    case CALUMI::Utilities::FileResult::FileErrorCode::UnknownErrorCode:
                        return 27;
                    case CALUMI::Utilities::FileResult::FileErrorCode::WriteFailure:
                        return 28;
                    default:
                        return 29;
                    }
                }
            }
            return 0;
        }
        catch (std::exception&)
        {
        }
    return -1;
}

CALUMI::UNIV::AnimationScene* LoadAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray,
                                                                 const int
                                                                 numberOfFiles)
{
    if (numberOfFiles < 1 || !filePathsArray)
    {
        return nullptr;
    }

    CALUMI::SFBGS::AnimationScene sfbgsAnimationScene;

    for (int i = 0; i < numberOfFiles; i++)
    {
        std::filesystem::path pathToAdd(filePathsArray[i]);
        if (pathToAdd.extension() == ".af")
        {
            CALUMI::SFBGS::Animation anim;
            if (auto animResult = anim.readFromFile(pathToAdd.c_str()); animResult.hasError())
            {
                return nullptr;
            }

            // ReSharper disable once CppExpressionWithoutSideEffects
            sfbgsAnimationScene.addAnimationToScene(anim);
        }
        if (pathToAdd.extension() == ".rig")
        {
            CALUMI::SFBGS::SkeletonRig rig;
            if (auto rigResult = rig.readFromFile(pathToAdd.c_str()); rigResult.hasError())
            {
                return nullptr;
            }
            sfbgsAnimationScene.rig() = rig;
        }
    }

    if (sfbgsAnimationScene.rig().boneEntries().empty())
    {
        return nullptr;
    }

    const auto output = new CALUMI::UNIV::AnimationScene;
    *output = sfbgsAnimationScene.convertToUniversalScene();

    return output;
}

CALUMI::UNIV::SkeletonRig* LoadSFBGSSkeletonRigFromFileC(const wchar_t* filePath)
{
    if (filePath)
        try
        {
            const std::filesystem::path pathToLoad(filePath);
            if (pathToLoad.extension() != ".rig")
            {
                return nullptr;
            }

            CALUMI::SFBGS::SkeletonRig rig;
            if (const auto rigResult = rig.readFromFile(pathToLoad.c_str()); rigResult.hasError())
            {
                return nullptr;
            }

            const auto output = new CALUMI::UNIV::SkeletonRig;
            *output = rig.convertToUniversalRig();
            return output;
        }
        catch (std::exception&)
        {
        }
    return nullptr;
}

int SaveAnimationToSFBGSFormatDirectC(const CALUMI::UNIV::Animation* animation,
                                      const wchar_t*                 filePath,
                                      const CALUMI::
                                      UNIV::SkeletonRig* rig)
{
    if (!filePath || !rig || !animation)
    {
        return -1;
    }

    try
    {
        const CALUMI::SFBGS::SkeletonRig sfbgsRig(*rig);
        CALUMI::SFBGS::Animation         sfbgsAnimation = ConvertToSFBGSAnimation(*animation, sfbgsRig);

        if (const auto result = sfbgsAnimation.writeToFile(filePath); !result.hasError())
        {
            switch (result.fileErrorCode())
            {
            case CALUMI::Utilities::FileResult::FileErrorCode::FileNotFound:
                return 20;
            case CALUMI::Utilities::FileResult::FileErrorCode::PermissionDenied:
                return 21;
            case CALUMI::Utilities::FileResult::FileErrorCode::NotAFile:
                return 22;
            case CALUMI::Utilities::FileResult::FileErrorCode::ReadFailure:
                return 23;
            case CALUMI::Utilities::FileResult::FileErrorCode::IncorrectFileType:
                return 24;
            case CALUMI::Utilities::FileResult::FileErrorCode::FileTooLarge:
                return 25;
            case CALUMI::Utilities::FileResult::FileErrorCode::FileTooSmall:
                return 26;
            case CALUMI::Utilities::FileResult::FileErrorCode::UnknownErrorCode:
                return 27;
            case CALUMI::Utilities::FileResult::FileErrorCode::WriteFailure:
                return 28;
            default:
                return 29;
            }
        }

        return 0;
    }
    catch (std::exception&)
    {
    }
    return -1;
}

int SaveAnimationToSFBGSFormatWithExistingRigDirectC(const CALUMI::UNIV::Animation* animation,
                                                     const wchar_t*                 filePath,
                                                     const wchar_t*                 sfbgsRigPath)
{
    if (!filePath || !sfbgsRigPath || !animation)
    {
        return -1;
    }

    try
    {
        CALUMI::SFBGS::SkeletonRig sfbgsRig;

        if (const auto rigResult = sfbgsRig.readFromFile(sfbgsRigPath); rigResult.hasError() || sfbgsRig.boneEntries().
                                                                        empty())
        {
            switch (rigResult.fileErrorCode())
            {
            case CALUMI::Utilities::FileResult::FileErrorCode::FileNotFound:
                return 10;
            case CALUMI::Utilities::FileResult::FileErrorCode::PermissionDenied:
                return 11;
            case CALUMI::Utilities::FileResult::FileErrorCode::NotAFile:
                return 12;
            case CALUMI::Utilities::FileResult::FileErrorCode::ReadFailure:
                return 13;
            case CALUMI::Utilities::FileResult::FileErrorCode::IncorrectFileType:
                return 14;
            case CALUMI::Utilities::FileResult::FileErrorCode::FileTooLarge:
                return 15;
            case CALUMI::Utilities::FileResult::FileErrorCode::FileTooSmall:
                return 16;
            case CALUMI::Utilities::FileResult::FileErrorCode::UnknownErrorCode:
                return 17;
            case CALUMI::Utilities::FileResult::FileErrorCode::WriteFailure:
                return 18;
            default:
                return 19;
            }
        }

        CALUMI::SFBGS::Animation sfbgsAnimation = CALUMI::SFBGS::ConvertToSFBGSAnimation(*animation, sfbgsRig);

        if (const auto result = sfbgsAnimation.writeToFile(filePath); !result.hasError())
        {
            switch (result.fileErrorCode())
            {
            case CALUMI::Utilities::FileResult::FileErrorCode::FileNotFound:
                return 20;
            case CALUMI::Utilities::FileResult::FileErrorCode::PermissionDenied:
                return 21;
            case CALUMI::Utilities::FileResult::FileErrorCode::NotAFile:
                return 22;
            case CALUMI::Utilities::FileResult::FileErrorCode::ReadFailure:
                return 23;
            case CALUMI::Utilities::FileResult::FileErrorCode::IncorrectFileType:
                return 24;
            case CALUMI::Utilities::FileResult::FileErrorCode::FileTooLarge:
                return 25;
            case CALUMI::Utilities::FileResult::FileErrorCode::FileTooSmall:
                return 26;
            case CALUMI::Utilities::FileResult::FileErrorCode::UnknownErrorCode:
                return 27;
            case CALUMI::Utilities::FileResult::FileErrorCode::WriteFailure:
                return 28;
            default:
                return 29;
            }
        }

        return 0;
    }
    catch (std::exception&)
    {
    }
    return -1;
}

int SaveSkeletonRigToSFBGSFormatDirectC(const CALUMI::UNIV::SkeletonRig* rig, const wchar_t* filePath)
{
    if (!filePath || !rig)
    {
        return -1;
    }

    try
    {
        CALUMI::SFBGS::SkeletonRig sfbgsRig(*rig);

        if (const auto result = sfbgsRig.writeToFile(filePath); !result.hasError())
        {
            switch (result.fileErrorCode())
            {
            case CALUMI::Utilities::FileResult::FileErrorCode::FileNotFound:
                return 10;
            case CALUMI::Utilities::FileResult::FileErrorCode::PermissionDenied:
                return 11;
            case CALUMI::Utilities::FileResult::FileErrorCode::NotAFile:
                return 12;
            case CALUMI::Utilities::FileResult::FileErrorCode::ReadFailure:
                return 13;
            case CALUMI::Utilities::FileResult::FileErrorCode::IncorrectFileType:
                return 14;
            case CALUMI::Utilities::FileResult::FileErrorCode::FileTooLarge:
                return 15;
            case CALUMI::Utilities::FileResult::FileErrorCode::FileTooSmall:
                return 16;
            case CALUMI::Utilities::FileResult::FileErrorCode::UnknownErrorCode:
                return 17;
            case CALUMI::Utilities::FileResult::FileErrorCode::WriteFailure:
                return 18;
            default:
                return 19;
            }
        }
    }
    catch (std::exception&)
    {
    }
    return -1;
}
