//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "SFBGS_AnimationScene.h"
#include "FileValidation.h"
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <filesystem>
#include <print>
#include <format>
#include <vector>




namespace CALUMI{ namespace SFBGS{

    //Interface Methods For Animation Vector
    bool AnimationScene::AddAnimationToScene(SFBGS::Animation& animation, bool overwrite)
    {
        for (unsigned int i = 0; i < animations.size(); i++)
        {
            if (animations.at(i).animationFileName == animation.animationFileName)
            {
                if (!overwrite)
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
        return RemoveAnimationFromScene(sceneToRemove.c_str());
    }

    bool AnimationScene::RemoveAnimationFromScene(const char* sceneToRemove)
    {
        for (unsigned int i = 0; i < animations.size(); i++)
        {
            if (animations.at(i).animationFileName == sceneToRemove)
            {
                animations.erase(i);
                return true;
            }
        }
        return false;
    }

    bool AnimationScene::RemoveAnimationFromScene(unsigned int idx)
    {
        if (idx >= animations.size() || idx < 0) return false;

        animations.erase(idx);
        return true;
    }

    //Sequence Conversions Directly On Animation Blocks
    static void _ConvertScalarSq(const Utilities::VectorContainer<CALUMI::UNIV::Scalar>& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock._scalarCount = static_cast<unsigned short>(input.size());
        outputBlock.scalarEntries.reserve(outputBlock._scalarCount);
        outputBlock.scalarKeyFrames.reserve(outputBlock._scalarCount);
        for (int i = 0; i < input.size(); i++)
        {
            outputBlock.scalarKeyFrames.push_back(input.at(i).frame);
            outputBlock.scalarEntries.push_back(static_cast<int16_t>(input.at(i).scalar * 5000));
        }
    }

    static Utilities::VectorContainer<CALUMI::UNIV::Scalar> _ConvertScalarSq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        Utilities::VectorContainer<CALUMI::UNIV::Scalar> output;
        output.reserve(inputBlock._scalarCount);
        for (unsigned int i = 0; i < inputBlock._scalarCount; i++)
        {
            CALUMI::UNIV::Scalar toAdd(inputBlock.scalarKeyFrames.at(i), inputBlock.scalarEntries.at(i)/5000.0f);
            output.push_back(toAdd);
        }
        output.sort();
        return output;
    }

    static void _ConvertPrioritySq(const Utilities::VectorContainer<CALUMI::UNIV::Priority>& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock._bonePriorityCount = static_cast<unsigned short>(input.size());
        outputBlock.bonePriorityEntries.resize(outputBlock._bonePriorityCount);
        outputBlock.bonePriorityKeyFrames.resize(outputBlock._bonePriorityCount);
        for (unsigned int i = 0; i < outputBlock._bonePriorityCount; i++)
        {
            outputBlock.bonePriorityKeyFrames.at(i) = input.at(i).frame;
            outputBlock.bonePriorityEntries.at(i) = input.at(i).priority;
        }
    }

    static Utilities::VectorContainer<CALUMI::UNIV::Priority> _ConvertPrioritySq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        Utilities::VectorContainer<CALUMI::UNIV::Priority> output;
        output.reserve(inputBlock._bonePriorityCount);
        for (unsigned int i = 0; i < inputBlock._bonePriorityCount; i++)
        {
            CALUMI::UNIV::Priority toAdd(inputBlock.bonePriorityKeyFrames.at(i), inputBlock.bonePriorityEntries.at(i));
            output.push_back(toAdd);
        }
        output.sort();
        return output;
    }

    static void _ConvertRotationSq(const Utilities::VectorContainer<CALUMI::UNIV::Rotation>& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock._rotationCount = static_cast<unsigned short>(input.size());
        outputBlock.rotationKeyFrames.reserve(input.size());
        outputBlock.rotationEntries.reserve(input.size());
        outputBlock.rotationPrefixEntries.reserve(input.size());
        for (int i =0; i < input.size(); i++)
        {
            outputBlock.rotationKeyFrames.push_back(input.at(i).frame);
            auto result = GetSFBGSRotationPair(input.at(i).rotation);
            outputBlock.rotationPrefixEntries.push_back(result.first);
            outputBlock.rotationEntries.push_back(result.second);
        }

        outputBlock.rotationPrefixEntries = FoldRotationPrefixSequence(outputBlock.rotationPrefixEntries);
        outputBlock._rotationPrefixCount = static_cast<unsigned short>(outputBlock.rotationPrefixEntries.size());
    }

    static Utilities::VectorContainer<CALUMI::UNIV::Rotation> _ConvertRotationSq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        Utilities::VectorContainer<CALUMI::UNIV::Rotation> output;
        output.reserve(inputBlock._rotationCount);
        Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix> unfoldedPrefixes = UnfoldRotationPrefixSequence(inputBlock.rotationPrefixEntries);
        for (unsigned int i = 0; i < inputBlock._rotationCount; i++)
        {
            CALUMI::UNIV::Rotation toAdd(inputBlock.rotationKeyFrames.at(i), GetUniversalRotation(unfoldedPrefixes.at(i), inputBlock.rotationEntries.at(i)));
            output.push_back(toAdd);
        }
        output.sort(false);
        return output;
    }

    static void _ConvertTranslationSq(const Utilities::VectorContainer<CALUMI::UNIV::Translation>& input, CALUMI::SFBGS::AnimationBlock& outputBlock, const float& highPrecision, const float& lowPrecision)
    {
        outputBlock._translationCount = static_cast<unsigned short>(input.size());
        outputBlock.translationKeyFrames.reserve(input.size());
        outputBlock.translationEntries.reserve(input.size());
        outputBlock.translationPrefixEntries.reserve(input.size());
        for (int i = 0; i < input.size(); i++)
        {
            outputBlock.translationKeyFrames.push_back(input.at(i).frame);
            auto result = GetSFBGSTranslationPair(input.at(i).translation, highPrecision, lowPrecision);
            outputBlock.translationPrefixEntries.push_back(result.first);
            outputBlock.translationEntries.push_back(result.second);
        }
        outputBlock.translationPrefixEntries = FoldTranslationPrefixSequence(outputBlock.translationPrefixEntries);
        outputBlock._translationPrefixCount = static_cast<unsigned short>(outputBlock.translationPrefixEntries.size());
    }

    static Utilities::VectorContainer<CALUMI::UNIV::Translation> _ConvertTranslationSq(const CALUMI::SFBGS::AnimationBlock& inputBlock, const float& highPrecision, const float& lowPrecision)
    {
        Utilities::VectorContainer<CALUMI::UNIV::Translation> output;
        output.reserve(inputBlock._translationCount);
        Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> unfoldedPrefixes = UnfoldTranslationPrefixSequence(inputBlock.translationPrefixEntries);
        for (unsigned int i = 0; i < inputBlock._translationCount; i++)
        {
            CALUMI::UNIV::Translation toAdd(inputBlock.translationKeyFrames.at(i), GetUniversalTranslation(unfoldedPrefixes.at(i), inputBlock.translationEntries.at(i), highPrecision, lowPrecision));
            output.push_back(toAdd);
        }
        output.sort();
        return output;
    }


    //Animation Conversion
    static Animation ConvertToSFBGSAnimation(CALUMI::UNIV::Animation& anim, const CALUMI::SFBGS::SkeletonRig& rig)
    {
        Animation output;
        output.animationFileName = anim.animationTitle.c_str();
        output.HeaderStart[2] = 1.0; //Setting header quaternion to w=1. Unknown what this does still

        output._boneCount = anim.boneCount; //Just listing all bones for now. Will need compression algorithm eventually
        output._frameCount = anim.GetFrameCount();

        output._indexAtlas.reserve(output._indexAtlasCounter);
        output._indexAtlas.push_back(0);
        output._indexAtlas.push_back(output._boneCount);


        output.animationBlocks.resize(output._boneCount); //Create empty blocks for all bones
        for (unsigned int i = 0; i< anim.animationBlocks.size();i++)
        {
            AnimationBlock toAdd;
            _ConvertRotationSq(anim.animationBlocks.at(i)._rotationSequence, toAdd);
            _ConvertTranslationSq(anim.animationBlocks.at(i)._translationSequence, toAdd, rig.highPrecision, rig.lowPrecision);
            _ConvertScalarSq(anim.animationBlocks.at(i)._scalarSequence, toAdd);
            _ConvertPrioritySq(anim.animationBlocks.at(i)._prioritySequence, toAdd);
            if (rig.stringArray.at(anim.animationBlocks.at(i).boneIndex) != anim.animationBlocks.at(i).boneName.c_str())
            {
                std::println("UNIV Bone Entry Name Does Not Match SFBGS RIG Bone Index Name");
                std::cin.get();
            }

            output.animationBlocks.at(anim.animationBlocks.at(i).boneIndex) = toAdd; //Fill block for the correct index
        }
        output._evaluateHeaderFlags();
        return output;
    }

    static CALUMI::UNIV::Animation ConvertToUniversalAnimation(CALUMI::SFBGS::Animation& anim, const CALUMI::SFBGS::SkeletonRig& rig)
    {
        CALUMI::UNIV::Animation output;
        output.animationTitle = anim.animationFileName.c_str();
        output.boneCount = anim._boneCount;

        output.animationBlocks.reserve(anim.animationBlocks.size());
        for (unsigned int i = 0; i < anim.animationBlocks.size(); i++)
        {
            if (anim.animationBlocks.at(i)._rotationCount || anim.animationBlocks.at(i)._translationCount || anim.animationBlocks.at(i)._scalarCount || anim.animationBlocks.at(i)._bonePriorityCount)
            {
                CALUMI::UNIV::AnimationBlock toAdd;
                toAdd.boneIndex = i;
                toAdd.boneName = rig.stringArray.at(i).c_str();
                
                toAdd._scalarSequence = _ConvertScalarSq(anim.animationBlocks.at(i));
                toAdd._prioritySequence = _ConvertPrioritySq(anim.animationBlocks.at(i));
                toAdd._translationSequence = _ConvertTranslationSq(anim.animationBlocks.at(i),rig.highPrecision,rig.lowPrecision);
                toAdd._rotationSequence = _ConvertRotationSq(anim.animationBlocks.at(i));

                output.animationBlocks.push_back(toAdd);
            }
        }
        output.animationBlocks.shrink_to_fit();
        return output;
    }

    //CONVERSION PARENT FUNCTIONS
    bool AnimationScene::ConvertFromUniversalScene(CALUMI::UNIV::AnimationScene& input)
    {
        if (input.rig.boneEntries.empty())
        {
            std::println("No Rig Entry Found In UNIV Animation Scene. Cannot Convert.");
            //std::cin.get();
            return false;
        }

        sceneName = input.sceneName;
        rig = ConvertToSFBGSRig(input.rig, highPrecisionValue, lowPrecisionValue);

        animations.reserve(input.animations.size());
        for (unsigned int i = 0; i < input.animations.size(); i++)
        {
            animations.push_back(ConvertToSFBGSAnimation(input.animations.at(i), rig));
        }

        return true;
    }

    //Converting a scene overloaded so that an existing converted rig can be used
    bool AnimationScene::ConvertFromUniversalScene(CALUMI::UNIV::AnimationScene& input, SFBGS::SkeletonRig& rigReference)
    {
        if (rigReference.boneEntries.empty())
        {
            std::println("No Rig Entry Found In SFBGS Reference Rig. Cannot Convert.");
            return false;
        }

        sceneName = input.sceneName;
        rig = rigReference;

        animations.reserve(input.animations.size());
        for (unsigned int i = 0; i < input.animations.size(); i++)
        {
            animations.push_back(ConvertToSFBGSAnimation(input.animations.at(i), rig));
        }

        return true;
    }

    CALUMI::UNIV::AnimationScene AnimationScene::ConvertToUniversalScene()
    {
        CALUMI::UNIV::AnimationScene outputScene;
        if (rig.boneEntries.empty())
        {
            std::println("No Rig Entry Found In SFBGS Animation Scene. Cannot Convert.");
            std::cin.get();
            return outputScene;
        }

        outputScene.sceneName = sceneName;
        outputScene.rig = SFBGS::ConvertToUniversalRig(rig);

        outputScene.animations.reserve(animations.size());
        for (int i = 0; i < animations.size(); i++)
        {
            outputScene.animations.push_back(ConvertToUniversalAnimation(animations.at(i), rig));
        }

        return outputScene;
    }

}

    //EXTERN C ===============================================================================================================================================================================
    bool SFBGS::SaveAnimationSceneToSFBGSFormatC(UNIV::AnimationScene* scene, float highPrecisionValue, float lowPrecisionValue, const wchar_t* directoryPath, Utilities::StringContainer* errorMessage)
    {
        errorMessage->Clear();

        if (scene->rig.boneEntries.empty())
        {
            *errorMessage += "[CALUMI.Animation API] No Rig Entry Found In Animation Scene. Cannot Convert";
            return false;
        }

        const char* rigExt = ".rig";
        const char* afExt  = ".af";
        if (scene->rig.rigName.Empty())
        {
            *errorMessage += "[CALUMI.Animation API] Empty string found for Rig Name";
            return false;
        }
        Utilities::PathContainer rigFilePath(directoryPath);
        //rigFilePath /= scene->rig.rigName;
        rigFilePath /= "characterassets";
        rigFilePath /= "skeleton.rig";
        rigFilePath.replace_extension(rigExt);

        auto animPathResult = scene->GetFilePathsFromAnimationScene(directoryPath, afExt);
        if (!animPathResult.has_value())
        {
            *errorMessage += animPathResult.error().c_str();
            return false;
        }
        Utilities::VectorContainer<Utilities::PathContainer> animationFilePaths = animPathResult.value();

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.ConvertFromUniversalScene(*scene))
        {
            *errorMessage += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        auto rigResult = translatedScene.rig.WriteToFile(rigFilePath);
        if (!rigResult.has_value())
        {
            *errorMessage += "[CALUMI.Animation API] ";
            *errorMessage += rigResult.error().ToString().c_str();
            return false;
        }
    
        for (int i = 0; i < translatedScene.animations.size(); i++)
        {
            auto animResult = translatedScene.animations.at(i).WriteToFile(animationFilePaths.at(i).w_str());
            if (!animResult.has_value())
            {
                *errorMessage += "[CALUMI.Animation API] ";
                *errorMessage += animResult.error().ToString().c_str();
                return false;
            }
        }

        std::filesystem::path dirPath = directoryPath;
        *errorMessage += "Successful Save To ";
        *errorMessage += dirPath.string().c_str();
        return true;
    }

    //Warning, desired directory path array must be in the same order as the animations in the scene. .rig file must be present and can be placed anywhere within the array. 
    bool SFBGS::SaveAnimationSceneToSFBGSFormatPathOverrideC(UNIV::AnimationScene* scene, float highPrecisionValue, float lowPrecisionValue, const wchar_t** directoryPathArray, size_t arraySize, Utilities::StringContainer* errorMessage)
    {
        errorMessage->Clear();

        if (scene->rig.boneEntries.empty())
        {
            *errorMessage += "[CALUMI.Animation API] No Rig Entry Found In Animation Scene. Cannot Convert";
            return false;
        }

        if (arraySize < 1 || !directoryPathArray || !directoryPathArray[0])
        {
            *errorMessage += ("[CALUMI.Animation API] Error, File Path Array Not Found");
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
            *errorMessage += "[CALUMI.Animation API] Error, No .rig File Path Found";
            return false;
        }

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.ConvertFromUniversalScene(*scene))
        {
            *errorMessage += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        auto rigResult = translatedScene.rig.WriteToFile(rigFilePath.c_str());
        if (!rigResult.has_value())
        {
            *errorMessage += "[CALUMI.Animation API] ";
            *errorMessage += rigResult.error().ToString().c_str();
            return false;
        }

        size_t minSize = min(translatedScene.animations.size(), (arraySize-1));

        for (int i = 0; i < minSize; i++)
        {
            auto animResult = translatedScene.animations.at(i).WriteToFile(animationFilePaths.at(i).c_str());
            if (!animResult.has_value())
            {
                *errorMessage = "[CALUMI.Animation API] ";
                *errorMessage += animResult.error().ToString().c_str();
                return false;
            }
        }

        
        *errorMessage += "Successful Save To ";
        *errorMessage += pathPrintOut.c_str();
        return true;
    }

    //Warning, desired directory path array must be in the same order as the animations in the scene.
    bool SFBGS::SaveAnimationSceneToSFBGSFormatUsingRigReferencePathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, size_t arraySize, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage)
    {
        errorMessage->Clear();

        std::filesystem::path rigPath(sfbgsRigPath);
        //const char* afExt = ".af";

        SFBGS::SkeletonRig sfbgsRig;
        auto rigResult = sfbgsRig.ReadFromFile(rigPath.c_str());

        if (!rigResult.has_value())
        {
            *errorMessage += "[CALUMI.Animation API] Error during rig import with error message:> ";
            *errorMessage += rigResult.error().ToString().c_str();
            return false;
        }
        if (arraySize < 1 || !directoryPathArray || !directoryPathArray[0])
        {
            *errorMessage += ("[CALUMI.Animation API] Error, File Path Array Not Found");
            return false;
        }

        std::string pathPrintOut;
        std::vector<std::filesystem::path> animationFilePaths;
        animationFilePaths.reserve(arraySize);

        for (unsigned int i = 0; i < arraySize; i++)
        {
            std::filesystem::path pathToAdd(directoryPathArray[i]);
            animationFilePaths.push_back(pathToAdd);
            std::string nameHolder = i < scene->animations.size() ? scene->animations.at(i).animationTitle.c_str() : "NO ANIMATION FOUND IN SCENE";
            pathPrintOut += std::format("   ({})",nameHolder);
            pathPrintOut += pathToAdd.string();
            pathPrintOut += "\n";
        }
        animationFilePaths.shrink_to_fit();
        pathPrintOut += "   Using Rig From: ";
        pathPrintOut += rigPath.string() + "\n";

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.ConvertFromUniversalScene(*scene, sfbgsRig))
        {
            *errorMessage += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }
        unsigned int j = 0;
        for (unsigned int i = 0; i < translatedScene.animations.size(); i++)
        {
            j = i < animationFilePaths.size() ? i : j;
            auto animResult = translatedScene.animations.at(i).WriteToFile(animationFilePaths.at(i).c_str());
            if (!animResult.has_value())
            {
                *errorMessage += "[CALUMI.Animation API] ";
                *errorMessage += animResult.error().ToString().c_str();
                return false;
            }
        }
        
        *errorMessage += "[CALUMIANIMATION_API] Successful Save To:";
        *errorMessage += pathPrintOut.c_str();
        return true;
    }

    bool SFBGS::SaveAnimationSceneToSFBGSFormatUsingRigReferenceC(UNIV::AnimationScene* scene, const wchar_t* directoryPath, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage)
    {
        errorMessage->Clear();
        
        std::filesystem::path rigPath(sfbgsRigPath);
        const char* afExt = ".af";

        SFBGS::SkeletonRig sfbgsRig;
        auto rigResult = sfbgsRig.ReadFromFile(rigPath.c_str());

        if (!rigResult.has_value())
        {
            *errorMessage += "[CALUMI.Animation API] Error during rig import with error message:> ";
            *errorMessage += rigResult.error().ToString().c_str();
            return false;
        }

        auto animPathResult = scene->GetFilePathsFromAnimationScene(directoryPath, afExt);
        if (!animPathResult.has_value())
        {
            *errorMessage += animPathResult.error().c_str();
            return false;
        }
        Utilities::VectorContainer<Utilities::PathContainer> animationFilePaths = animPathResult.value();

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.ConvertFromUniversalScene(*scene, sfbgsRig))
        {
            *errorMessage += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        for (int i = 0; i < translatedScene.animations.size(); i++)
        {
            auto animResult = translatedScene.animations.at(i).WriteToFile(animationFilePaths.at(i).w_str());
            if (!animResult.has_value())
            {
                *errorMessage += "[CALUMI.Animation API] ";
                *errorMessage += animResult.error().ToString().c_str();
                return false;
            }
        }
        std::filesystem::path dirPath = directoryPath;
        *errorMessage += "[CALUMI.Animation API] Successful Save To ";
        *errorMessage += dirPath.string().c_str();
        return true;
    }

    UNIV::AnimationScene* SFBGS::LoadAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray, int numberOfFiles, Utilities::StringContainer* errorMessage)
    {
        errorMessage->Clear();

        if (numberOfFiles < 1 || !filePathsArray)
        {
            *errorMessage += "[CALUMI.Animation API] No Files To Read Into Animation Scene. Returning nullptr.";
            return nullptr;
        }

        SFBGS::AnimationScene sfbgsAnimationScene;
        sfbgsAnimationScene.animations.reserve(numberOfFiles);

        for (int i = 0; i < numberOfFiles; i++)
        {
            std::filesystem::path pathToAdd(filePathsArray[i]);
            if (pathToAdd.extension() == ".af")
            {
                SFBGS::Animation anim;
                auto animResult = anim.ReadFromFile(pathToAdd.c_str());
                if (!animResult.has_value())
                {
                    *errorMessage += "[CALUMI.Animation API] ";
                    *errorMessage += animResult.error().ToString().c_str();
                    return nullptr;
                }
                sfbgsAnimationScene.animations.push_back(anim);
            }
            if (pathToAdd.extension() == ".rig")
            {
                SFBGS::SkeletonRig rig;
                auto rigResult = rig.ReadFromFile(pathToAdd.c_str());
                if (!rigResult.has_value())
                {
                    *errorMessage += "[CALUMI.Animation API] ";
                    *errorMessage += rigResult.error().ToString().c_str();
                    return nullptr;
                }
                sfbgsAnimationScene.rig = rig;
            }
        }

        if (sfbgsAnimationScene.rig.boneEntries.empty())
        {
            *errorMessage += "[CALUMI.Animation API] No Rig Entry Found In File List. Cannot Convert!";
            return nullptr;
        }

        UNIV::AnimationScene* output = new UNIV::AnimationScene;
        *output = sfbgsAnimationScene.ConvertToUniversalScene();

        

        *errorMessage += "[CALUMI.Animation API] AnimationScene Created. Please Remember To call DeleteAnimationSceneC(ptr) When Finished.";
        return output;
    }

    UNIV::AnimationScene* SFBGS::LoadJsonAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray, int numberOfFiles, Utilities::StringContainer* errorMessage, const wchar_t* jsonOutputPath)
    {
        //We do NOT want to clear our errorMessage here as it is handled by the first called function

        UNIV::AnimationScene* output = LoadAnimationSceneFromSFBGSFormatC(filePathsArray, numberOfFiles, errorMessage);
        if (output != nullptr)
        {
            if (jsonOutputPath != nullptr && jsonOutputPath[0] != L'\0') {
                Utilities::PathContainer jsonPath(jsonOutputPath);
                auto result = WriteToBinaryFile(jsonPath, output->ToJSON(0).c_str());
                if (!result.has_value())
                {
                    *errorMessage += std::format("\n    {}", result.error().ToString().c_str()).c_str();
                }
                else
                {
                    *errorMessage += std::format("\n   [CALUMI.Animation API] JSON written to: {}", jsonPath.c_str()).c_str();
                }
            }
        }
        return output;

    }

    UNIV::SkeletonRig* SFBGS::LoadSFBGSSkeletonRigFromFileC(const wchar_t* filePath, Utilities::StringContainer* errorMessage)
    {
        errorMessage->Clear();

        if (!filePath)
        {
            *errorMessage += "[CALUMI.Animation API] Error, No File Path Provided To Load Rig From. Returning Empty Rig.";
            return new UNIV::SkeletonRig();
        }

        std::filesystem::path pathToLoad(filePath);
        if (pathToLoad.extension() != ".rig")
        {
            *errorMessage += "[CALUMI.Animation API] Error, File Path Provided Does Not Have .rig Extension. Returning Empty Rig.";
            return new UNIV::SkeletonRig();
        }

        SFBGS::SkeletonRig rig;
        auto rigResult = rig.ReadFromFile(pathToLoad.c_str());
        if (!rigResult.has_value())
        {
            *errorMessage += "[CALUMI.Animation API] Error during rig import with error message:> ";
            *errorMessage += rigResult.error().ToString().c_str();
            return new UNIV::SkeletonRig();
        }

        UNIV::SkeletonRig* output = new UNIV::SkeletonRig;
        *output = SFBGS::ConvertToUniversalRig(rig);
        *errorMessage += "[CALUMI.Animation API] Rig Loaded Successfully From File Path Provided.";
        return output;
    }

    

}






