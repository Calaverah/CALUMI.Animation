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
    Animation& AnimationScene::animation(const uint64_t idx) const { return pImpl->animations.at(idx); }
    uint64_t AnimationScene::animationCount() const { return pImpl->animations.size(); }
    SkeletonRig& AnimationScene::rig() const { return pImpl->rig; }
    void AnimationScene::setRig(const SkeletonRig& rig) const { pImpl->rig = rig; }

    AnimationScene::AnimationScene() : pImpl(new Impl) {}
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
            pImpl->animations = input.pImpl->animations; pImpl->rig = input.pImpl->rig; pImpl->sceneName = input.pImpl->sceneName;

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
    static Animation ConvertToSFBGSAnimation(UNIV::Animation& anim, const SkeletonRig& rig)
    {
        Animation output;
        output.setAnimationFileName(anim.animationTitle());

        output.setBoneCount(rig.boneCount()); //Just listing all bones for now. Will need compression algorithm eventually
        output.setFrameCount(anim.frameCount());

        output.indexAtlas().reserve(output.indexAtlasCount());
        output.indexAtlas().push_back(0);
        output.indexAtlas().push_back(output.boneCount());


        output.animationBlocks().resize(output.boneCount()); //Create empty blocks for all bones
        for (unsigned int i = 0; i< anim.animationBlocks().size(); i++)
        {
            AnimationBlock toAdd;
            toAdd.setRotationSequenceFromUNIV(anim.animationBlocks().at(i).rotationSequence());
            toAdd.setTranslationSequenceFromUNIV(anim.animationBlocks().at(i).translationSequence(), rig.highPrecision(), rig.lowPrecision());
            toAdd.setScalarSequenceFromUNIV(anim.animationBlocks().at(i).scalarSequence());
            toAdd.setPrioritySequenceFromUNIV(anim.animationBlocks().at(i).prioritySequence());

            if(const int boneIdx = rig.findBoneIndex(anim.animationBlocks().at(i).boneName()); boneIdx >= 0 && boneIdx < output.animationBlocks().size())
                output.animationBlocks().at(boneIdx) = toAdd; //Fill block for the correct index
        }

        if (const auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(anim.getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
        {
            for (int i = 0; i < pkg->getAmendedBlockCount() && i < std::numeric_limits<int>::max(); i++)
            {
                AnimationBlock toAdd;
                toAdd.setRotationSequenceFromUNIV(pkg->getAmendedBlock(i)->rotationSequence());

                float hiVal = pkg->usesRigForPrecision() ? rig.highPrecision() : pkg->getOverridePrecisionSet().high();
                float loVal = pkg->usesRigForPrecision() ? rig.lowPrecision() : pkg->getOverridePrecisionSet().low();

                toAdd.setTranslationSequenceFromUNIV(pkg->getAmendedBlock(i)->translationSequence(), hiVal, loVal);

                toAdd.setScalarSequenceFromUNIV(pkg->getAmendedBlock(i)->scalarSequence());
                toAdd.setPrioritySequenceFromUNIV(pkg->getAmendedBlock(i)->prioritySequence());
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
            if (anim.animationBlocks().at(i).rotationCount() || anim.animationBlocks().at(i).translationCount() || anim.animationBlocks().at(i).scalarCount()|| anim.animationBlocks().at(i).priorityCount())
            {
                UNIV::AnimationBlock toAdd;
                const AnimationBlock& refBlock = anim.animationBlocks().at(i);
                //toAdd.setBoneIndex(i);
                toAdd.setBoneName(rig.stringArray().c_str(i));
                
                toAdd.scalarSequence() = refBlock.scalarSequenceAsUNIV();
                toAdd.prioritySequence() = refBlock.prioritySequenceAsUNIV();
                toAdd.translationSequence() = refBlock.translationSequenceAsUNIV(rig.highPrecision(), rig.lowPrecision());
                toAdd.rotationSequence() = refBlock.rotationSequenceAsUNIV();

                output.animationBlocks().push_back(toAdd);
            }
        }
        output.animationBlocks().shrink_to_fit();

        //Amended animation blocks (NiNode bones)
        CreateNewSFBGSAnimationPackage(output, true);

        if(anim.verifyAmendedBlocks())
        {
            const auto& amendedBlocks = anim.amendedAnimationBlocks();
            auto& hashSet = anim.amendedHashSet();

            if(const auto pkg = dynamic_cast<SFBGS_AnimationPackage*>(output.getPackageManager().getPackage(SFBGS_ANIM_PACKAGE)))
            {
                for (uint64_t i = 0; i < amendedBlocks.size(); i++)
                {
                    const uint32_t hash = hashSet.at(i);
                    UNIV::AnimationBlock toAdd;
                    AnimationBlock refBlock = amendedBlocks.at(i);

                    std::string tempName = Utilities::HashRegistry::getInstance().isKnownHash(hash) ? Utilities::HashRegistry::getInstance().getKnownHashString(hash) : "Unknown_Hash_" + std::to_string(i);
                    toAdd.setBoneName(tempName.c_str());

                    toAdd.scalarSequence() = refBlock.scalarSequenceAsUNIV();
                    toAdd.prioritySequence() = refBlock.prioritySequenceAsUNIV();
                    toAdd.translationSequence() = refBlock.translationSequenceAsUNIV(rig.highPrecision(), rig.lowPrecision());
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
        if (input.rig().boneEntries().empty())
            return false;


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
    bool AnimationScene::convertFromUniversalScene(const UNIV::AnimationScene& input, const SkeletonRig& rigReference) const
    {
        if (rigReference.boneEntries().empty())
            return false;


        pImpl->sceneName = input.sceneName();
        pImpl->rig = rigReference;

        pImpl->animations.reserve(input.animationCount());
        for (unsigned int i = 0; i < input.animationCount(); i++)
        {
            pImpl->animations.push_back(ConvertToSFBGSAnimation(input.animation(i), pImpl->rig));
        }

        return true;
    }

    UNIV::AnimationScene AnimationScene::convertToUniversalScene()
    {
        UNIV::AnimationScene outputScene;
        if (pImpl->rig.boneEntries().empty())
            return outputScene;

        outputScene.setSceneName(pImpl->sceneName);
        outputScene.rig() = pImpl->rig.convertToUniversalRig();

        //outputScene.Animations().reserve(pImpl->animations.size());
        for (const auto& animation : pImpl->animations)
        {
            auto newAnim = ConvertToUniversalAnimation(animation, pImpl->rig);
            outputScene.addAnimationToScene(newAnim);
        }

        return outputScene;
    }

}

    //EXTERN C ===============================================================================================================================================================================


    bool SFBGS::SaveAnimationSceneToSFBGSFormatPathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, const uint64_t arraySize, Utilities::StringContainer* errorMessage)
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
            *errorMessageHolder += "[CALUMI.Animation API] Error, File Path Array Not Found";
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

        AnimationScene translatedScene;
        if (!translatedScene.convertFromUniversalScene(*scene))
        {
            *errorMessageHolder += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        if (auto rigResult = translatedScene.rig().writeToFile(rigFilePath.c_str()); rigResult.hasError())
        {
            *errorMessageHolder += "[CALUMI.Animation API] ";
            *errorMessageHolder += rigResult.toString().c_str();
            return false;
        }

        const uint64_t minSize = std::min(translatedScene.animationCount(), arraySize - 1);

        for (int i = 0; i < minSize; i++)
        {
            if (auto animResult = translatedScene.animation(i).writeToFile(animationFilePaths.at(i).c_str()); animResult.hasError())
            {
                *errorMessageHolder = "[CALUMI.Animation API] ";
                *errorMessageHolder += animResult.toString();
                return false;
            }
        }

        
        *errorMessageHolder += "Successful Save To ";
        *errorMessageHolder += pathPrintOut.c_str();
        return true;
    }

    //Warning, desired directory _path array must be in the same order as the animations in the scene.
    bool SFBGS::SaveAnimationSceneToSFBGSFormatUsingRigReferencePathOverrideC(const UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, const uint64_t arraySize, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        const std::filesystem::path rigPath(sfbgsRigPath);
        //const char* afExt = ".af";

        SkeletonRig sfbgsRig;

        if (auto rigResult = sfbgsRig.readFromFile(rigPath.c_str()); rigResult.hasError())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error during rig import with error message:> ";
            *errorMessageHolder += rigResult.toString().c_str();
            return false;
        }
        if (arraySize < 1 || !directoryPathArray || !directoryPathArray[0])
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, File Path Array Not Found";
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

        const AnimationScene translatedScene;
        if (!translatedScene.convertFromUniversalScene(*scene, sfbgsRig))
        {
            *errorMessageHolder += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        const uint64_t minSize = std::min(translatedScene.animationCount(), arraySize - 1);
        for (unsigned int i = 0; i < minSize; i++)
        {
            if (auto animResult = translatedScene.animation(i).writeToFile(animationFilePaths.at(i).c_str()); animResult.hasError())
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

    UNIV::AnimationScene* SFBGS::LoadAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray, const int numberOfFiles, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (numberOfFiles < 1 || !filePathsArray)
        {
            *errorMessageHolder += "[CALUMI.Animation API] No Files To Read Into Animation Scene. Returning nullptr.";
            return nullptr;
        }

        AnimationScene sfbgsAnimationScene;
        //sfbgsAnimationScene.Animations().reserve(numberOfFiles);

        for (int i = 0; i < numberOfFiles; i++)
        {
            std::filesystem::path pathToAdd(filePathsArray[i]);
            if (pathToAdd.extension() == ".af")
            {
                Animation anim;
                if (auto animResult = anim.readFromFile(pathToAdd.c_str()); animResult.hasError())
                {
                    *errorMessageHolder += "[CALUMI.Animation API] ";
                    *errorMessageHolder += animResult.toString().c_str();
                    return nullptr;
                }

                // ReSharper disable once CppExpressionWithoutSideEffects
                sfbgsAnimationScene.addAnimationToScene(anim);
            }
            if (pathToAdd.extension() == ".rig")
            {
                SkeletonRig rig;
                if (auto rigResult = rig.readFromFile(pathToAdd.c_str()); rigResult.hasError())
                {
                    *errorMessageHolder += "[CALUMI.Animation API]";
                    *errorMessageHolder += " ";
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

        const auto output = new UNIV::AnimationScene;
        *output = sfbgsAnimationScene.convertToUniversalScene();

        

        *errorMessageHolder += "[CALUMI.Animation API] AnimationScene Created. Please Remember To call DeleteAnimationSceneC(ptr) When Finished.";
        return output;
    }

    UNIV::AnimationScene* SFBGS::LoadAnimationSceneFromSFBGSFormatAndSaveToJsonC(const wchar_t** filePathsArray, const int numberOfFiles, Utilities::StringContainer* errorMessage, const wchar_t* jsonOutputPath)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        //We do NOT want to clear our errorMessageHolder here as it is handled by the first called function

        UNIV::AnimationScene* output = LoadAnimationSceneFromSFBGSFormatC(filePathsArray, numberOfFiles, errorMessageHolder);
        if (output != nullptr)
        {
            if (jsonOutputPath != nullptr && jsonOutputPath[0] != L'\0') {
                const Utilities::PathContainer jsonPath(jsonOutputPath);
                if (auto result = WriteToBinaryFile(jsonPath, output->toJSON(0).c_str()); result.hasError())
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

        const std::filesystem::path pathToLoad(filePath);
        if (pathToLoad.extension() != ".rig")
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, File Path Provided Does Not Have .rig Extension. Returning Empty Rig.";
            return new UNIV::SkeletonRig();
        }

        SkeletonRig rig;
        if (auto rigResult = rig.readFromFile(pathToLoad.c_str()); rigResult.hasError())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error during rig import with error message:> ";
            *errorMessageHolder += rigResult.toString().c_str();
            return new UNIV::SkeletonRig();
        }

        const auto output = new UNIV::SkeletonRig;
        *output = rig.convertToUniversalRig();
        *errorMessageHolder += "[CALUMI.Animation API] Rig Loaded Successfully From File Path Provided.";
        return output;
    }

    bool SFBGS::SaveAnimationToSFBGSFormatDirectC(UNIV::Animation* animation, const wchar_t* filePath, const UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();
        
        if (!filePath)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No File Path Provided To Save File.";
            return false;
        }

        if (!rig || rig->boneEntries().empty())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Rig Present Or Rig Is Empty.";
            return false;
        }
        
        if (!animation)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Animation Present.";
            return false;
        }

        const SkeletonRig sfbgsRig(*rig);
        Animation sfbgsAnimation = ConvertToSFBGSAnimation(*animation, sfbgsRig);

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

        SkeletonRig sfbgsRig;

        if (auto rigResult = sfbgsRig.readFromFile(sfbgsRigPath); rigResult.hasError() || sfbgsRig.boneEntries().empty())
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

        Animation sfbgsAnimation = ConvertToSFBGSAnimation(*animation, sfbgsRig);

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

    bool SFBGS::SaveSkeletonRigToSFBGSFormatDirectC(const UNIV::SkeletonRig* rig, const wchar_t* filePath, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->clear();

        if (!filePath)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No File Path Provided To Save File.";
            return false;
        }

        if (!rig || rig->boneEntries().empty())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Rig Present Or Rig Is Empty.";
            return false;
        }

        SkeletonRig sfbgsRig(*rig);

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






