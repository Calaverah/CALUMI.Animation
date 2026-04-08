//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "internalplatform.h"
#include "sfbgs/SFBGS_AnimationScene.h"
#include "sfbgs/animation/SFBGS_AnimationPackage.h"
#include "utilities/CALUMI_Hashes.h"

#include "io/FileValidation.h"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <print>
#include <vector>



namespace CALUMI{ namespace SFBGS{

    struct AnimationScene::Impl
    {
        Utilities::StringContainer sceneName = "MyStarfieldAnimationScene";
        std::vector<Animation> animations;
        SkeletonRig rig;
        Impl() = default;
    };
    Utilities::StringContainer& AnimationScene::sceneName() const { return pImpl->sceneName; }
    Animation& AnimationScene::animation(uint64_t idx) const { return pImpl->animations.at(idx); }
    uint64_t AnimationScene::animationCount() const { return pImpl->animations.size(); }
    SkeletonRig& AnimationScene::rig() const { return pImpl->rig; }
    void AnimationScene::setRig(const SkeletonRig& rig) { pImpl->rig = rig; }

    AnimationScene::AnimationScene() { pImpl = new Impl; }
    AnimationScene::~AnimationScene() { if (pImpl) delete pImpl; }
    AnimationScene::AnimationScene(const char* sceneName) : AnimationScene() { pImpl->sceneName = sceneName; }
    AnimationScene::AnimationScene(const Utilities::StringContainer& sceneName) : AnimationScene() { pImpl->sceneName = sceneName; }
    AnimationScene::AnimationScene(const AnimationScene& input) : AnimationScene() { *this = input; }
    AnimationScene& AnimationScene::operator=(const AnimationScene& input)
    {
        pImpl->animations = input.pImpl->animations; pImpl->rig = input.pImpl->rig; pImpl->sceneName = input.pImpl->sceneName;
        return *this;
    }
    //Interface Methods For Animation Vector
    bool AnimationScene::addAnimationToScene(SFBGS::Animation& animation, bool overwrite)
    {
        for (unsigned int i = 0; i < pImpl->animations.size(); i++)
        {
            if (pImpl->animations.at(i).getAnimationFileName() == animation.getAnimationFileName())
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

    bool AnimationScene::removeAnimationFromScene(Utilities::StringContainer& sceneToRemove)
    {
        return removeAnimationFromScene(sceneToRemove.c_str());
    }

    bool AnimationScene::removeAnimationFromScene(const char* sceneToRemove)
    {
        for (unsigned int i = 0; i < pImpl->animations.size(); i++)
        {
            if (pImpl->animations.at(i).getAnimationFileName() == sceneToRemove)
            {
                pImpl->animations.erase(pImpl->animations.begin() + i);
                return true;
            }
        }
        return false;
    }

    bool AnimationScene::removeAnimationFromScene(unsigned int idx)
    {
        if (idx >= pImpl->animations.size() || idx < 0) return false;

        pImpl->animations.erase(pImpl->animations.begin() + idx);
        return true;
    }

    //Sequence Conversions Directly On Animation Blocks
    static void _ConvertScalarSq(const UNIV::ScalarSequence& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock.setScalarCount(static_cast<uint16_t>(input.size()));
        outputBlock.ScalarEntries().reserve(outputBlock.getScalarCount());
        outputBlock.ScalarKeyFrames().reserve(outputBlock.getScalarCount());
        for (int i = 0; i < input.size(); i++)
        {
            outputBlock.ScalarKeyFrames().push_back(input.at(i).frame());
            outputBlock.ScalarEntries().push_back(static_cast<int16_t>(input.at(i).scalarValue() * 5000));
        }
    }

    static UNIV::ScalarSequence _ConvertScalarSq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        UNIV::ScalarSequence output;
        output.reserve(inputBlock.getScalarCount());
        for (unsigned int i = 0; i < inputBlock.getScalarCount(); i++)
        {
            CALUMI::UNIV::Scalar toAdd(inputBlock.ScalarKeyFrames().at(i), inputBlock.ScalarEntries().at(i)/5000.0f);
            output.push_back(toAdd);
        }
        SortScalarSequence(output);
        return output;
    }

    static void _ConvertPrioritySq(const UNIV::PrioritySequence& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock.setPriorityCount(static_cast<uint16_t>(input.size()));
        outputBlock.BonePriorityEntries().resize(outputBlock.getPriorityCount());
        outputBlock.BonePriorityKeyFrames().resize(outputBlock.getPriorityCount());
        for (unsigned int i = 0; i < outputBlock.getPriorityCount(); i++)
        {
            outputBlock.BonePriorityKeyFrames().at(i) = input.at(i).frame();
            outputBlock.BonePriorityEntries().at(i) = input.at(i).priorityValue();
        }
    }

    static UNIV::PrioritySequence _ConvertPrioritySq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        UNIV::PrioritySequence output;
        output.reserve(inputBlock.getPriorityCount());
        for (unsigned int i = 0; i < inputBlock.getPriorityCount(); i++)
        {
            CALUMI::UNIV::Priority toAdd(inputBlock.BonePriorityKeyFrames().at(i), inputBlock.BonePriorityEntries().at(i));
            output.push_back(toAdd);
        }
        SortPrioritySequence(output);
        return output;
    }

    static void _ConvertRotationSq(const UNIV::RotationSequence& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock.setRotationCount(static_cast<uint16_t>(input.size()));
        outputBlock.RotationKeyFrames().reserve(input.size());
        outputBlock.RotationEntries().reserve(input.size());
        outputBlock.RotationPrefixEntries().reserve(input.size());
        for (int i =0; i < input.size(); i++)
        {
            outputBlock.RotationKeyFrames().push_back(input.at(i).frame());
            auto result = GetSFBGSRotationPair(input.at(i).rotationQuaternion());
            outputBlock.RotationPrefixEntries().push_back(result.prefix());
            outputBlock.RotationEntries().push_back(result.suffix());
        }

        outputBlock.RotationPrefixEntries ()= FoldRotationPrefixSequence(outputBlock.RotationPrefixEntries());
        outputBlock.setRotationPrefixCount(static_cast<uint16_t>(outputBlock.RotationPrefixEntries().size()));
    }

    static UNIV::RotationSequence _ConvertRotationSq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        UNIV::RotationSequence output;
        output.reserve(inputBlock.getRotationCount());
        SFBGS::RotationPrefixSequence unfoldedPrefixes = UnfoldRotationPrefixSequence(inputBlock.RotationPrefixEntries());
        for (unsigned int i = 0; i < inputBlock.getRotationCount(); i++)
        {
            CALUMI::UNIV::Rotation toAdd(inputBlock.RotationKeyFrames().at(i), GetUniversalRotation(unfoldedPrefixes.at(i), inputBlock.RotationEntries().at(i)));
            output.push_back(toAdd);
        }
        SortRotationSequence(output,false);
        return output;
    }

    static void _ConvertTranslationSq(const UNIV::TranslationSequence& input, CALUMI::SFBGS::AnimationBlock& outputBlock, const float& highPrecision, const float& lowPrecision)
    {
        outputBlock.setTranslationCount(static_cast<uint16_t>(input.size()));
        outputBlock.TranslationKeyFrames().reserve(input.size());
        outputBlock.TranslationEntries().reserve(input.size());
        outputBlock.TranslationPrefixEntries().reserve(input.size());
        for (int i = 0; i < input.size(); i++)
        {
            outputBlock.TranslationKeyFrames().push_back(input.at(i).frame());
            auto result = GetSFBGSTranslationPair(input.at(i).translationVector(), highPrecision, lowPrecision);
            outputBlock.TranslationPrefixEntries().push_back(result.prefix());
            outputBlock.TranslationEntries().push_back(result.suffix());
        }
        outputBlock.TranslationPrefixEntries ()= FoldTranslationPrefixSequence(outputBlock.TranslationPrefixEntries());
        outputBlock.setTranslationPrefixCount(static_cast<uint16_t>(outputBlock.TranslationPrefixEntries().size()));
    }

    static UNIV::TranslationSequence _ConvertTranslationSq(const CALUMI::SFBGS::AnimationBlock& inputBlock, const float& highPrecision, const float& lowPrecision)
    {
        UNIV::TranslationSequence output;
        output.reserve(inputBlock.getTranslationCount());
        SFBGS::TranslationPrefixSequence unfoldedPrefixes = UnfoldTranslationPrefixSequence(inputBlock.TranslationPrefixEntries());
        for (unsigned int i = 0; i < inputBlock.getTranslationCount(); i++)
        {
            CALUMI::UNIV::Translation toAdd(inputBlock.TranslationKeyFrames().at(i), GetUniversalTranslation(unfoldedPrefixes.at(i), inputBlock.TranslationEntries().at(i), highPrecision, lowPrecision));
            output.push_back(toAdd);
        }
        SortTranslationSequence(output);
        return output;
    }


    //Animation Conversion
    static Animation ConvertToSFBGSAnimation(CALUMI::UNIV::Animation& anim, const CALUMI::SFBGS::SkeletonRig& rig)
    {
        Animation output;
        output.setAnimationFileName(anim.animationTitle());

        output.setBoneCount(rig.boneCount()); //Just listing all bones for now. Will need compression algorithm eventually
        output.setFrameCount(anim.frameCount());

        output.getIndexAtlas().reserve(output.getIndexAtlasCount());
        output.getIndexAtlas().push_back(0);
        output.getIndexAtlas().push_back(output.getBoneCount());


        output.getAnimationBlocks().resize(output.getBoneCount()); //Create empty blocks for all bones
        for (unsigned int i = 0; i< anim.animationBlocks().size(); i++)
        {
            AnimationBlock toAdd;
            _ConvertRotationSq(anim.animationBlocks().at(i).rotationSequence(), toAdd);
            _ConvertTranslationSq(anim.animationBlocks().at(i).translationSequence(), toAdd, rig.highPrecision(), rig.lowPrecision());
            _ConvertScalarSq(anim.animationBlocks().at(i).scalarSequence(), toAdd);
            _ConvertPrioritySq(anim.animationBlocks().at(i).prioritySequence(), toAdd);

            int boneIdx = rig.findBoneIndex(anim.animationBlocks().at(i).boneName());

            if(boneIdx >= 0 && boneIdx < output.getAnimationBlocks().size())
                output.getAnimationBlocks().at(boneIdx) = toAdd; //Fill block for the correct index
        }

        if (auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(anim.getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
        {
            for (uint64_t i = 0; i < pkg->getAmendedBlockCount(); i++)
            {
                AnimationBlock toAdd;
                _ConvertRotationSq(pkg->getAmendedBlock(i)->rotationSequence(), toAdd);

                float hiVal = pkg->usesRigForPrecision() ? rig.highPrecision() : pkg->getOverridePrecisionSet().high();
                float loVal = pkg->usesRigForPrecision() ? rig.lowPrecision() : pkg->getOverridePrecisionSet().low();

                _ConvertTranslationSq(pkg->getAmendedBlock(i)->translationSequence(), toAdd, hiVal, loVal);

                _ConvertScalarSq(pkg->getAmendedBlock(i)->scalarSequence(), toAdd);
                _ConvertPrioritySq(pkg->getAmendedBlock(i)->prioritySequence(), toAdd);
            }
        }

        output.evaluateHeaderFlags();
        return output;
    }

    static CALUMI::UNIV::Animation ConvertToUniversalAnimation(CALUMI::SFBGS::Animation& anim, const CALUMI::SFBGS::SkeletonRig& rig)
    {
        CALUMI::UNIV::Animation output;
        output.setAnimationTitle(anim.getAnimationFileName());
        //output.boneCount = anim._boneCount;

        output.animationBlocks().reserve(anim.getAnimationBlocks().size());
        for (unsigned int i = 0; i < anim.getAnimationBlocks().size(); i++)
        {
            if (anim.getAnimationBlocks().at(i).getRotationCount() || anim.getAnimationBlocks().at(i).getTranslationCount() || anim.getAnimationBlocks().at(i).getScalarCount()|| anim.getAnimationBlocks().at(i).getPriorityCount())
            {
                CALUMI::UNIV::AnimationBlock toAdd;
                //toAdd.setBoneIndex(i);
                toAdd.setBoneName(rig.stringArray().c_str(i));
                
                toAdd.scalarSequence() = _ConvertScalarSq(anim.getAnimationBlocks().at(i));
                toAdd.prioritySequence() = _ConvertPrioritySq(anim.getAnimationBlocks().at(i));
                toAdd.translationSequence() = _ConvertTranslationSq(anim.getAnimationBlocks().at(i), rig.highPrecision(), rig.lowPrecision());
                toAdd.rotationSequence() = _ConvertRotationSq(anim.getAnimationBlocks().at(i));

                output.animationBlocks().push_back(toAdd);
            }
        }
        output.animationBlocks().shrink_to_fit();

        //Amended animation blocks (NiNode bones)
        CreateNewSFBGSAnimationPackage(output, true);

        if(anim.VerifyAmendedBlocks())
        {
            auto& amendedBlocks = anim.getAmendedAnimationBlocks();
            auto& hashSet = anim.getAmendedHashSet();

            if(auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(output.getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                for (uint64_t i = 0; i < amendedBlocks.size(); i++)
                {
                    uint32_t hash = hashSet.at(i);
                    CALUMI::UNIV::AnimationBlock toAdd;
                    
                    //toAdd.setBoneIndex(i);

                    std::string tempName = Utilities::HashRegistry::getInstance().isKnownHash(hash) ? Utilities::HashRegistry::getInstance().getKnownHashString(hash) : "Unknown_Hash_" + std::to_string(i);
                    toAdd.setBoneName(tempName.c_str());

                    toAdd.scalarSequence() = _ConvertScalarSq(amendedBlocks.at(i));
                    toAdd.prioritySequence() = _ConvertPrioritySq(amendedBlocks.at(i));
                    toAdd.translationSequence() = _ConvertTranslationSq(amendedBlocks.at(i), rig.highPrecision(), rig.lowPrecision());
                    toAdd.rotationSequence() = _ConvertRotationSq(amendedBlocks.at(i));

                    pkg->addAmendedBlock(hash, toAdd);
                }
            }
        }

        return output;
    }

    //CONVERSION PARENT FUNCTIONS
    bool AnimationScene::convertFromUniversalScene(CALUMI::UNIV::AnimationScene& input)
    {
        if (input.rig().boneEntries().empty())
        {
            std::println("No Rig Entry Found In UNIV Animation Scene. Cannot Convert.");
            //std::cin.get();
            return false;
        }

        pImpl->sceneName = input.sceneName();
        pImpl->rig.convertFromUniversalRig(input.rig());

        pImpl->animations.reserve(input.animationCount());
        for (unsigned int i = 0; i < input.animationCount(); i++)
        {
            pImpl->animations.push_back(ConvertToSFBGSAnimation(input.animation(i), pImpl->rig));
        }

        return true;
    }

    //Converting a scene overloaded so that an existing converted rig can be used
    bool AnimationScene::convertFromUniversalScene(CALUMI::UNIV::AnimationScene& input, SFBGS::SkeletonRig& rigReference)
    {
        if (rigReference.boneEntries().empty())
        {
            std::println("No Rig Entry Found In SFBGS Reference Rig. Cannot Convert.");
            return false;
        }

        pImpl->sceneName = input.sceneName();
        pImpl->rig = rigReference;

        pImpl->animations.reserve(input.animationCount());
        for (unsigned int i = 0; i < input.animationCount(); i++)
        {
            pImpl->animations.push_back(ConvertToSFBGSAnimation(input.animation(i), pImpl->rig));
        }

        return true;
    }

    CALUMI::UNIV::AnimationScene AnimationScene::convertToUniversalScene()
    {
        CALUMI::UNIV::AnimationScene outputScene;
        if (pImpl->rig.boneEntries().empty())
        {
            std::println("No Rig Entry Found In SFBGS Animation Scene. Cannot Convert.");
            std::cin.get();
            return outputScene;
        }

        outputScene.setSceneName(pImpl->sceneName);
        outputScene.rig() = pImpl->rig.convertToUniversalRig();

        //outputScene.Animations().reserve(pImpl->animations.size());
        for (int i = 0; i < pImpl->animations.size(); i++)
        {
            auto newAnim = ConvertToUniversalAnimation(pImpl->animations.at(i), pImpl->rig);
            outputScene.addAnimationToScene(newAnim);
        }

        return outputScene;
    }

}

    //EXTERN C ===============================================================================================================================================================================
    //bool SFBGS::SaveAnimationSceneToSFBGSFormatC(UNIV::AnimationScene* scene, const wchar_t* directoryPath, Utilities::StringContainer* errorMessage)
    //{
    //    Utilities::StringContainer tempErrorMessage;
    //    Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
    //    errorMessageHolder->clear();

    //    if (scene->Rig().BoneEntries().empty())
    //    {
    //        *errorMessageHolder += "[CALUMI.Animation API] No Rig Entry Found In Animation Scene. Cannot Convert";
    //        return false;
    //    }

    //    const char* rigExt = ".rig";
    //    const char* afExt  = ".af";
    //    if (scene->Rig().RigName().empty())
    //    {
    //        *errorMessageHolder += "[CALUMI.Animation API] Empty string found for Rig Name";
    //        return false;
    //    }
    //    Utilities::PathContainer rigFilePath(directoryPath);
    //    rigFilePath /= scene->Rig().RigName().c_str();
    //    //rigFilePath /= "characterassets";
    //    //rigFilePath /= "skeleton.rig";
    //    rigFilePath.replace_extension(rigExt);

    //    auto animPathResult = scene->GetFilePathsFromAnimationScene(directoryPath, afExt);
    //    if (!animPathResult.has_value())
    //    {
    //        *errorMessageHolder += animPathResult.error().c_str();
    //        return false;
    //    }
    //    Utilities::VectorContainer<Utilities::PathContainer> animationFilePaths = animPathResult.value();

    //    SFBGS::AnimationScene translatedScene;
    //    if (!translatedScene.ConvertFromUniversalScene(*scene))
    //    {
    //        *errorMessageHolder += "[CALUMI.Animation API] Could not convert animation scene";
    //        return false;
    //    }

    //    auto rigResult = translatedScene.Rig().WriteToFile(rigFilePath);
    //    if (!rigResult.has_value())
    //    {
    //        *errorMessageHolder += "[CALUMI.Animation API] ";
    //        *errorMessageHolder += rigResult.error().ToString().c_str();
    //        return false;
    //    }
    //
    //    for (int i = 0; i < translatedScene.Animations().size(); i++)
    //    {
    //        auto animResult = translatedScene.Animations().at(i).WriteToFile(animationFilePaths.at(i).w_str());
    //        if (!animResult.has_value())
    //        {
    //            *errorMessageHolder += "[CALUMI.Animation API] ";
    //            *errorMessageHolder += animResult.error().ToString().c_str();
    //            return false;
    //        }
    //    }

    //    std::filesystem::path dirPath = directoryPath;
    //    *errorMessageHolder += "Successful Save To ";
    //    *errorMessageHolder += dirPath.string().c_str();
    //    return true;
    //}

    bool SFBGS::SaveAnimationSceneToSFBGSFormatPathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, uint64_t arraySize, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (scene->rig().boneEntries().empty())
        {
            *errorMessageHolder += "[CALUMI.Animation API] No Rig Entry Found In Animation Scene. Cannot Convert";
            return false;
        }

        if (arraySize < 1 || !directoryPathArray || !directoryPathArray[0])
        {
            *errorMessageHolder += ("[CALUMI.Animation API] Error, File Path Array Not Found");
            return false;
        }

        std::vector<std::filesystem::path> animationFilePaths;
        std::filesystem::path rigFilePath;
        std::string pathPrintOut;
        for (unsigned int i = 0; i < arraySize; i++)
        {
            std::filesystem::path pathToAdd(directoryPathArray[i]);
            if (pathToAdd.extension() == ".rig")
                rigFilePath = pathToAdd;
            else
                animationFilePaths.push_back(pathToAdd);

            pathPrintOut += pathToAdd.string();
            pathPrintOut += "\n";
        }
        animationFilePaths.shrink_to_fit();

        if (rigFilePath.empty())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No .rig File Path Found";
            return false;
        }

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.convertFromUniversalScene(*scene))
        {
            *errorMessageHolder += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        auto rigResult = translatedScene.rig().writeToFile(rigFilePath.c_str());
        if (rigResult.hasError())
        {
            *errorMessageHolder += "[CALUMI.Animation API] ";
            *errorMessageHolder += rigResult.toString().c_str();
            return false;
        }

        uint64_t minSize = std::min(translatedScene.animationCount(), (arraySize - 1));

        for (int i = 0; i < minSize; i++)
        {
            auto animResult = translatedScene.animation(i).writeToFile(animationFilePaths.at(i).c_str());
            if (animResult.hasError())
            {
                *errorMessageHolder = "[CALUMI.Animation API] ";
                *errorMessageHolder += animResult.toString().c_str();
                return false;
            }
        }

        
        *errorMessageHolder += "Successful Save To ";
        *errorMessageHolder += pathPrintOut.c_str();
        return true;
    }

    //Warning, desired directory _path array must be in the same order as the animations in the scene.
    bool SFBGS::SaveAnimationSceneToSFBGSFormatUsingRigReferencePathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, uint64_t arraySize, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        std::filesystem::path rigPath(sfbgsRigPath);
        //const char* afExt = ".af";

        SFBGS::SkeletonRig sfbgsRig;
        auto rigResult = sfbgsRig.readFromFile(rigPath.c_str());

        if (rigResult.hasError())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error during rig import with error message:> ";
            *errorMessageHolder += rigResult.toString().c_str();
            return false;
        }
        if (arraySize < 1 || !directoryPathArray || !directoryPathArray[0])
        {
            *errorMessageHolder += ("[CALUMI.Animation API] Error, File Path Array Not Found");
            return false;
        }

        std::string pathPrintOut;
        std::vector<std::filesystem::path> animationFilePaths;
        animationFilePaths.reserve(arraySize);

        for (unsigned int i = 0; i < arraySize; i++)
        {
            std::filesystem::path pathToAdd(directoryPathArray[i]);
            animationFilePaths.push_back(pathToAdd);
            std::string nameHolder = i < scene->animationCount() ? scene->animation(i).animationTitle() : "NO ANIMATION FOUND IN SCENE";
            pathPrintOut += std::format("   ({})",nameHolder);
            pathPrintOut += pathToAdd.string();
            pathPrintOut += "\n";
        }
        animationFilePaths.shrink_to_fit();
        pathPrintOut += "   Using Rig From: ";
        pathPrintOut += rigPath.string() + "\n";

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.convertFromUniversalScene(*scene, sfbgsRig))
        {
            *errorMessageHolder += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }
        
        uint64_t minSize = std::min(translatedScene.animationCount(), (arraySize - 1));
        for (unsigned int i = 0; i < minSize; i++)
        {
            auto animResult = translatedScene.animation(i).writeToFile(animationFilePaths.at(i).c_str());
            if (animResult.hasError())
            {
                *errorMessageHolder += "[CALUMI.Animation API] ";
                *errorMessageHolder += animResult.toString().c_str();
                return false;
            }
        }
        
        *errorMessageHolder += "[CALUMIANIMATION_API] Successful Save To:";
        *errorMessageHolder += pathPrintOut.c_str();
        return true;
    }

    //bool SFBGS::SaveAnimationSceneToSFBGSFormatUsingRigReferenceC(UNIV::AnimationScene* scene, const wchar_t* directoryPath, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage)
    //{
    //    Utilities::StringContainer tempErrorMessage;
    //    Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
    //    errorMessageHolder->clear();
    //    
    //    std::filesystem::path rigPath(sfbgsRigPath);
    //    const char* afExt = ".af";

    //    SFBGS::SkeletonRig sfbgsRig;
    //    auto rigResult = sfbgsRig.ReadFromFile(rigPath.c_str());

    //    if (!rigResult.has_value())
    //    {
    //        *errorMessageHolder += "[CALUMI.Animation API] Error during rig import with error message:> ";
    //        *errorMessageHolder += rigResult.error().ToString().c_str();
    //        return false;
    //    }

    //    auto animPathResult = scene->GetFilePathsFromAnimationScene(directoryPath, afExt);
    //    if (!animPathResult.has_value())
    //    {
    //        *errorMessageHolder += animPathResult.error().c_str();
    //        return false;
    //    }
    //    Utilities::VectorContainer<Utilities::PathContainer> animationFilePaths = animPathResult.value();

    //    SFBGS::AnimationScene translatedScene;
    //    if (!translatedScene.ConvertFromUniversalScene(*scene, sfbgsRig))
    //    {
    //        *errorMessageHolder += "[CALUMI.Animation API] Could not convert animation scene";
    //        return false;
    //    }

    //    for (int i = 0; i < translatedScene.Animations().size(); i++)
    //    {
    //        auto animResult = translatedScene.Animations().at(i).WriteToFile(animationFilePaths.at(i).w_str());
    //        if (!animResult.has_value())
    //        {
    //            *errorMessageHolder += "[CALUMI.Animation API] ";
    //            *errorMessageHolder += animResult.error().ToString().c_str();
    //            return false;
    //        }
    //    }
    //    std::filesystem::path dirPath = directoryPath;
    //    *errorMessageHolder += "[CALUMI.Animation API] Successful Save To ";
    //    *errorMessageHolder += dirPath.string().c_str();
    //    return true;
    //}

    UNIV::AnimationScene* SFBGS::LoadAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray, int numberOfFiles, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (numberOfFiles < 1 || !filePathsArray)
        {
            *errorMessageHolder += "[CALUMI.Animation API] No Files To Read Into Animation Scene. Returning nullptr.";
            return nullptr;
        }

        SFBGS::AnimationScene sfbgsAnimationScene;
        //sfbgsAnimationScene.Animations().reserve(numberOfFiles);

        for (int i = 0; i < numberOfFiles; i++)
        {
            std::filesystem::path pathToAdd(filePathsArray[i]);
            if (pathToAdd.extension() == ".af")
            {
                SFBGS::Animation anim;
                auto animResult = anim.readFromFile(pathToAdd.c_str());
                if (animResult.hasError())
                {
                    *errorMessageHolder += "[CALUMI.Animation API] ";
                    *errorMessageHolder += animResult.toString().c_str();
                    return nullptr;
                }
                sfbgsAnimationScene.addAnimationToScene(anim);
            }
            if (pathToAdd.extension() == ".rig")
            {
                SFBGS::SkeletonRig rig;
                auto rigResult = rig.readFromFile(pathToAdd.c_str());
                if (rigResult.hasError())
                {
                    *errorMessageHolder += "[CALUMI.Animation API] ";
                    *errorMessageHolder += rigResult.toString().c_str();
                    return nullptr;
                }
                sfbgsAnimationScene.rig() = rig;
            }
        }

        if (sfbgsAnimationScene.rig().boneEntries().empty())
        {
            *errorMessageHolder += "[CALUMI.Animation API] No Rig Entry Found In File List. Cannot Convert!";
            return nullptr;
        }

        UNIV::AnimationScene* output = new UNIV::AnimationScene;
        *output = sfbgsAnimationScene.convertToUniversalScene();

        

        *errorMessageHolder += "[CALUMI.Animation API] AnimationScene Created. Please Remember To call DeleteAnimationSceneC(ptr) When Finished.";
        return output;
    }

    UNIV::AnimationScene* SFBGS::LoadAnimationSceneFromSFBGSFormatAndSaveToJsonC(const wchar_t** filePathsArray, int numberOfFiles, Utilities::StringContainer* errorMessage, const wchar_t* jsonOutputPath)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        //We do NOT want to clear our errorMessageHolder here as it is handled by the first called function

        UNIV::AnimationScene* output = LoadAnimationSceneFromSFBGSFormatC(filePathsArray, numberOfFiles, errorMessageHolder);
        if (output != nullptr)
        {
            if (jsonOutputPath != nullptr && jsonOutputPath[0] != L'\0') {
                Utilities::PathContainer jsonPath(jsonOutputPath);
                auto result = WriteToBinaryFile(jsonPath, output->toJSON(0).c_str());
                if (result.hasError())
                {
                    *errorMessageHolder += std::format("\n    {}", result.toString().c_str()).c_str();
                }
                else
                {
                    *errorMessageHolder += std::format("\n   [CALUMI.Animation API] JSON written to: {}", jsonPath.c_str()).c_str();
                }
            }
        }
        return output;

    }

    UNIV::SkeletonRig* SFBGS::LoadSFBGSSkeletonRigFromFileC(const wchar_t* filePath, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (!filePath)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No File Path Provided To Load Rig From. Returning Empty Rig.";
            return new UNIV::SkeletonRig();
        }

        std::filesystem::path pathToLoad(filePath);
        if (pathToLoad.extension() != ".rig")
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, File Path Provided Does Not Have .rig Extension. Returning Empty Rig.";
            return new UNIV::SkeletonRig();
        }

        SFBGS::SkeletonRig rig;
        auto rigResult = rig.readFromFile(pathToLoad.c_str());
        if (rigResult.hasError())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error during rig import with error message:> ";
            *errorMessageHolder += rigResult.toString().c_str();
            return new UNIV::SkeletonRig();
        }

        UNIV::SkeletonRig* output = new UNIV::SkeletonRig;
        *output = rig.convertToUniversalRig();
        *errorMessageHolder += "[CALUMI.Animation API] Rig Loaded Successfully From File Path Provided.";
        return output;
    }

    bool SFBGS::SaveAnimationToSFBGSFormatDirectC(UNIV::Animation* animation, const wchar_t* filePath, UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();
        
        if (!filePath)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No File Path Provided To Save File.";
            return false;
        }

        if (!rig || rig->boneEntries().size() <= 0)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Rig Present Or Rig Is Empty.";
            return false;
        }
        
        if (!animation)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Animation Present.";
            return false;
        }

        SFBGS::SkeletonRig sfbgsRig(*rig);
        SFBGS::Animation sfbgsAnimation = ConvertToSFBGSAnimation(*animation, sfbgsRig);

        auto result = sfbgsAnimation.writeToFile(filePath);

        if (!result.hasError())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Result: ";
            *errorMessageHolder += result.toString();
            return true;
        }
        
        *errorMessageHolder += "[CALUMI.Animation API] ";
        *errorMessageHolder += result.toString();
        return false;

    }

    bool SFBGS::SaveAnimationToSFBGSFormatWithExistingRigDirectC(UNIV::Animation* animation, const wchar_t* filePath, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (!filePath)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No File Path Provided To Save File.";
            return false;
        }

        if (!sfbgsRigPath)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Rig Path Provided.";
            return false;
        }

        SFBGS::SkeletonRig sfbgsRig;
        auto rigResult = sfbgsRig.readFromFile(sfbgsRigPath);

        if (rigResult.hasError() || sfbgsRig.boneEntries().size() <= 0)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Rig Error: ";
            *errorMessageHolder += rigResult.toString();
            return false;
        }

        if (!animation)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Animation Present.";
            return false;
        }

        SFBGS::Animation sfbgsAnimation = ConvertToSFBGSAnimation(*animation, sfbgsRig);

        auto result = sfbgsAnimation.writeToFile(filePath);

        if (!result.hasError())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Result: ";
            *errorMessageHolder += result.toString();
            return true;
        }

        *errorMessageHolder += "[CALUMI.Animation API] ";
        *errorMessageHolder += result.toString();
        return false;

    }

    bool SFBGS::SaveSkeletonRigToSFBGSFormatDirectC(UNIV::SkeletonRig* rig, const wchar_t* filePath, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (!filePath)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No File Path Provided To Save File.";
            return false;
        }

        if (!rig || rig->boneEntries().size() <= 0)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Rig Present Or Rig Is Empty.";
            return false;
        }

        SFBGS::SkeletonRig sfbgsRig(*rig);

        auto result = sfbgsRig.writeToFile(filePath);

        if (!result.hasError())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Result: ";
            *errorMessageHolder += result.toString();
            return true;
        }

        *errorMessageHolder += "[CALUMI.Animation API] ";
        *errorMessageHolder += result.toString();
        return false;
    }

    

}






