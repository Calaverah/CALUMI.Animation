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

    struct AnimationScene::Impl
    {
        Utilities::StringContainer sceneName = "MyStarfieldAnimationScene";
        Utilities::VectorContainer<Animation> animations;
        SkeletonRig rig;
        Impl() = default;
    };
    Utilities::StringContainer& AnimationScene::SceneName() const { return pImpl->sceneName; }
    Utilities::VectorContainer<Animation>& AnimationScene::Animations() const { return pImpl->animations; }
    SkeletonRig& AnimationScene::Rig() const { return pImpl->rig; }

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
    bool AnimationScene::AddAnimationToScene(SFBGS::Animation& animation, bool overwrite)
    {
        for (unsigned int i = 0; i < pImpl->animations.size(); i++)
        {
            if (pImpl->animations.at(i).getAnimationFileName() == animation.getAnimationFileName())
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
        return RemoveAnimationFromScene(sceneToRemove.c_str());
    }

    bool AnimationScene::RemoveAnimationFromScene(const char* sceneToRemove)
    {
        for (unsigned int i = 0; i < pImpl->animations.size(); i++)
        {
            if (pImpl->animations.at(i).getAnimationFileName() == sceneToRemove)
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

    //Sequence Conversions Directly On Animation Blocks
    static void _ConvertScalarSq(const Utilities::VectorContainer<CALUMI::UNIV::Scalar>& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock.setScalarCount(static_cast<uint16_t>(input.size()));
        outputBlock.ScalarEntries().reserve(outputBlock.getScalarCount());
        outputBlock.ScalarKeyFrames().reserve(outputBlock.getScalarCount());
        for (int i = 0; i < input.size(); i++)
        {
            outputBlock.ScalarKeyFrames().push_back(input.at(i).Frame());
            outputBlock.ScalarEntries().push_back(static_cast<int16_t>(input.at(i).ScalarValue() * 5000));
        }
    }

    static Utilities::VectorContainer<CALUMI::UNIV::Scalar> _ConvertScalarSq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        Utilities::VectorContainer<CALUMI::UNIV::Scalar> output;
        output.reserve(inputBlock.getScalarCount());
        for (unsigned int i = 0; i < inputBlock.getScalarCount(); i++)
        {
            CALUMI::UNIV::Scalar toAdd(inputBlock.ScalarKeyFrames().at(i), inputBlock.ScalarEntries().at(i)/5000.0f);
            output.push_back(toAdd);
        }
        output.sort();
        return output;
    }

    static void _ConvertPrioritySq(const Utilities::VectorContainer<CALUMI::UNIV::Priority>& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock.setPriorityCount(static_cast<uint16_t>(input.size()));
        outputBlock.BonePriorityEntries().resize(outputBlock.getPriorityCount());
        outputBlock.BonePriorityKeyFrames().resize(outputBlock.getPriorityCount());
        for (unsigned int i = 0; i < outputBlock.getPriorityCount(); i++)
        {
            outputBlock.BonePriorityKeyFrames().at(i) = input.at(i).Frame();
            outputBlock.BonePriorityEntries().at(i) = input.at(i).PriorityValue();
        }
    }

    static Utilities::VectorContainer<CALUMI::UNIV::Priority> _ConvertPrioritySq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        Utilities::VectorContainer<CALUMI::UNIV::Priority> output;
        output.reserve(inputBlock.getPriorityCount());
        for (unsigned int i = 0; i < inputBlock.getPriorityCount(); i++)
        {
            CALUMI::UNIV::Priority toAdd(inputBlock.BonePriorityKeyFrames().at(i), inputBlock.BonePriorityEntries().at(i));
            output.push_back(toAdd);
        }
        output.sort();
        return output;
    }

    static void _ConvertRotationSq(const Utilities::VectorContainer<CALUMI::UNIV::Rotation>& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock.setRotationCount(static_cast<uint16_t>(input.size()));
        outputBlock.RotationKeyFrames().reserve(input.size());
        outputBlock.RotationEntries().reserve(input.size());
        outputBlock.RotationPrefixEntries().reserve(input.size());
        for (int i =0; i < input.size(); i++)
        {
            outputBlock.RotationKeyFrames().push_back(input.at(i).Frame());
            auto result = GetSFBGSRotationPair(input.at(i).RotationQuaternion());
            outputBlock.RotationPrefixEntries().push_back(result.getFirst());
            outputBlock.RotationEntries().push_back(result.getSecond());
        }

        outputBlock.RotationPrefixEntries ()= FoldRotationPrefixSequence(outputBlock.RotationPrefixEntries());
        outputBlock.setRotationPrefixCount(static_cast<uint16_t>(outputBlock.RotationPrefixEntries().size()));
    }

    static Utilities::VectorContainer<CALUMI::UNIV::Rotation> _ConvertRotationSq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        Utilities::VectorContainer<CALUMI::UNIV::Rotation> output;
        output.reserve(inputBlock.getRotationCount());
        Utilities::VectorContainer<CALUMI::SFBGS::RotationPrefix> unfoldedPrefixes = UnfoldRotationPrefixSequence(inputBlock.RotationPrefixEntries());
        for (unsigned int i = 0; i < inputBlock.getRotationCount(); i++)
        {
            CALUMI::UNIV::Rotation toAdd(inputBlock.RotationKeyFrames().at(i), GetUniversalRotation(unfoldedPrefixes.at(i), inputBlock.RotationEntries().at(i)));
            output.push_back(toAdd);
        }
        output.sort(false);
        return output;
    }

    static void _ConvertTranslationSq(const Utilities::VectorContainer<CALUMI::UNIV::Translation>& input, CALUMI::SFBGS::AnimationBlock& outputBlock, const float& highPrecision, const float& lowPrecision)
    {
        outputBlock.setTranslationCount(static_cast<uint16_t>(input.size()));
        outputBlock.TranslationKeyFrames().reserve(input.size());
        outputBlock.TranslationEntries().reserve(input.size());
        outputBlock.TranslationPrefixEntries().reserve(input.size());
        for (int i = 0; i < input.size(); i++)
        {
            outputBlock.TranslationKeyFrames().push_back(input.at(i).Frame());
            auto result = GetSFBGSTranslationPair(input.at(i).TranslationVector(), highPrecision, lowPrecision);
            outputBlock.TranslationPrefixEntries().push_back(result.getFirst());
            outputBlock.TranslationEntries().push_back(result.getSecond());
        }
        outputBlock.TranslationPrefixEntries ()= FoldTranslationPrefixSequence(outputBlock.TranslationPrefixEntries());
        outputBlock.setTranslationPrefixCount(static_cast<uint16_t>(outputBlock.TranslationPrefixEntries().size()));
    }

    static Utilities::VectorContainer<CALUMI::UNIV::Translation> _ConvertTranslationSq(const CALUMI::SFBGS::AnimationBlock& inputBlock, const float& highPrecision, const float& lowPrecision)
    {
        Utilities::VectorContainer<CALUMI::UNIV::Translation> output;
        output.reserve(inputBlock.getTranslationCount());
        Utilities::VectorContainer<CALUMI::SFBGS::TranslationPrefix> unfoldedPrefixes = UnfoldTranslationPrefixSequence(inputBlock.TranslationPrefixEntries());
        for (unsigned int i = 0; i < inputBlock.getTranslationCount(); i++)
        {
            CALUMI::UNIV::Translation toAdd(inputBlock.TranslationKeyFrames().at(i), GetUniversalTranslation(unfoldedPrefixes.at(i), inputBlock.TranslationEntries().at(i), highPrecision, lowPrecision));
            output.push_back(toAdd);
        }
        output.sort();
        return output;
    }


    //Animation Conversion
    static Animation ConvertToSFBGSAnimation(CALUMI::UNIV::Animation& anim, const CALUMI::SFBGS::SkeletonRig& rig)
    {
        Animation output;
        output.setAnimationFileName(anim.AnimationTitle());

        output.setBoneCount(rig.BoneCount()); //Just listing all bones for now. Will need compression algorithm eventually
        output.setFrameCount(anim.GetFrameCount());

        output.getIndexAtlas().reserve(output.getIndexAtlasCount());
        output.getIndexAtlas().push_back(0);
        output.getIndexAtlas().push_back(output.getBoneCount());


        output.getAnimationBlocks().resize(output.getBoneCount()); //Create empty blocks for all bones
        for (unsigned int i = 0; i< anim.AnimationBlocks().size(); i++)
        {
            AnimationBlock toAdd;
            _ConvertRotationSq(anim.AnimationBlocks().at(i).RotationSequence(), toAdd);
            _ConvertTranslationSq(anim.AnimationBlocks().at(i).TranslationSequence(), toAdd, rig.HighPrecision(), rig.LowPrecision());
            _ConvertScalarSq(anim.AnimationBlocks().at(i).ScalarSequence(), toAdd);
            _ConvertPrioritySq(anim.AnimationBlocks().at(i).PrioritySequence(), toAdd);
            if (rig.StringArray().at(anim.AnimationBlocks().at(i).BoneIndex()) != anim.AnimationBlocks().at(i).BoneName())
            {
                std::println("UNIV Bone Entry Name Does Not Match SFBGS RIG Bone Index Name");
                std::cin.get();
            }

            output.getAnimationBlocks().at(anim.AnimationBlocks().at(i).BoneIndex()) = toAdd; //Fill block for the correct index
        }
        output._evaluateHeaderFlags();
        return output;
    }

    static CALUMI::UNIV::Animation ConvertToUniversalAnimation(CALUMI::SFBGS::Animation& anim, const CALUMI::SFBGS::SkeletonRig& rig)
    {
        CALUMI::UNIV::Animation output;
        output.AnimationTitle(anim.getAnimationFileName());
        //output.boneCount = anim._boneCount;

        output.AnimationBlocks().reserve(anim.getAnimationBlocks().size());
        for (unsigned int i = 0; i < anim.getAnimationBlocks().size(); i++)
        {
            if (anim.getAnimationBlocks().at(i).getRotationCount() || anim.getAnimationBlocks().at(i).getTranslationCount() || anim.getAnimationBlocks().at(i).getScalarCount()|| anim.getAnimationBlocks().at(i).getPriorityCount())
            {
                CALUMI::UNIV::AnimationBlock toAdd;
                toAdd.BoneIndex(i);
                toAdd.BoneName(rig.StringArray().at(i).c_str());
                
                toAdd.ScalarSequence() = _ConvertScalarSq(anim.getAnimationBlocks().at(i));
                toAdd.PrioritySequence() = _ConvertPrioritySq(anim.getAnimationBlocks().at(i));
                toAdd.TranslationSequence() = _ConvertTranslationSq(anim.getAnimationBlocks().at(i), rig.HighPrecision(), rig.LowPrecision());
                toAdd.RotationSequence() = _ConvertRotationSq(anim.getAnimationBlocks().at(i));

                output.AnimationBlocks().push_back(toAdd);
            }
        }
        output.AnimationBlocks().shrink_to_fit();
        return output;
    }

    //CONVERSION PARENT FUNCTIONS
    bool AnimationScene::ConvertFromUniversalScene(CALUMI::UNIV::AnimationScene& input)
    {
        if (input.Rig().BoneEntries().empty())
        {
            std::println("No Rig Entry Found In UNIV Animation Scene. Cannot Convert.");
            //std::cin.get();
            return false;
        }

        pImpl->sceneName = input.SceneName();
        pImpl->rig = ConvertToSFBGSRig(input.Rig());

        pImpl->animations.reserve(input.Animations().size());
        for (unsigned int i = 0; i < input.Animations().size(); i++)
        {
            pImpl->animations.push_back(ConvertToSFBGSAnimation(input.Animations().at(i), pImpl->rig));
        }

        return true;
    }

    //Converting a scene overloaded so that an existing converted rig can be used
    bool AnimationScene::ConvertFromUniversalScene(CALUMI::UNIV::AnimationScene& input, SFBGS::SkeletonRig& rigReference)
    {
        if (rigReference.BoneEntries().empty())
        {
            std::println("No Rig Entry Found In SFBGS Reference Rig. Cannot Convert.");
            return false;
        }

        pImpl->sceneName = input.SceneName();
        pImpl->rig = rigReference;

        pImpl->animations.reserve(input.Animations().size());
        for (unsigned int i = 0; i < input.Animations().size(); i++)
        {
            pImpl->animations.push_back(ConvertToSFBGSAnimation(input.Animations().at(i), pImpl->rig));
        }

        return true;
    }

    CALUMI::UNIV::AnimationScene AnimationScene::ConvertToUniversalScene()
    {
        CALUMI::UNIV::AnimationScene outputScene;
        if (pImpl->rig.BoneEntries().empty())
        {
            std::println("No Rig Entry Found In SFBGS Animation Scene. Cannot Convert.");
            std::cin.get();
            return outputScene;
        }

        outputScene.SceneName(pImpl->sceneName);
        outputScene.Rig() = SFBGS::ConvertToUniversalRig(pImpl->rig);

        outputScene.Animations().reserve(pImpl->animations.size());
        for (int i = 0; i < pImpl->animations.size(); i++)
        {
            outputScene.Animations().push_back(ConvertToUniversalAnimation(pImpl->animations.at(i), pImpl->rig));
        }

        return outputScene;
    }

}

    //EXTERN C ===============================================================================================================================================================================
    bool SFBGS::SaveAnimationSceneToSFBGSFormatC(UNIV::AnimationScene* scene, const wchar_t* directoryPath, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();

        if (scene->Rig().BoneEntries().empty())
        {
            *errorMessageHolder += "[CALUMI.Animation API] No Rig Entry Found In Animation Scene. Cannot Convert";
            return false;
        }

        const char* rigExt = ".rig";
        const char* afExt  = ".af";
        if (scene->Rig().RigName().Empty())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Empty string found for Rig Name";
            return false;
        }
        Utilities::PathContainer rigFilePath(directoryPath);
        rigFilePath /= scene->Rig().RigName().c_str();
        //rigFilePath /= "characterassets";
        //rigFilePath /= "skeleton.rig";
        rigFilePath.replace_extension(rigExt);

        auto animPathResult = scene->GetFilePathsFromAnimationScene(directoryPath, afExt);
        if (!animPathResult.has_value())
        {
            *errorMessageHolder += animPathResult.error().c_str();
            return false;
        }
        Utilities::VectorContainer<Utilities::PathContainer> animationFilePaths = animPathResult.value();

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.ConvertFromUniversalScene(*scene))
        {
            *errorMessageHolder += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        auto rigResult = translatedScene.Rig().WriteToFile(rigFilePath);
        if (!rigResult.has_value())
        {
            *errorMessageHolder += "[CALUMI.Animation API] ";
            *errorMessageHolder += rigResult.error().ToString().c_str();
            return false;
        }
    
        for (int i = 0; i < translatedScene.Animations().size(); i++)
        {
            auto animResult = translatedScene.Animations().at(i).WriteToFile(animationFilePaths.at(i).w_str());
            if (!animResult.has_value())
            {
                *errorMessageHolder += "[CALUMI.Animation API] ";
                *errorMessageHolder += animResult.error().ToString().c_str();
                return false;
            }
        }

        std::filesystem::path dirPath = directoryPath;
        *errorMessageHolder += "Successful Save To ";
        *errorMessageHolder += dirPath.string().c_str();
        return true;
    }

    //Warning, desired directory path array must be in the same order as the animations in the scene. .rig file must be present and can be placed anywhere within the array. 
    bool SFBGS::SaveAnimationSceneToSFBGSFormatPathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, size_t arraySize, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();

        if (scene->Rig().BoneEntries().empty())
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
        if (!translatedScene.ConvertFromUniversalScene(*scene))
        {
            *errorMessageHolder += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        auto rigResult = translatedScene.Rig().WriteToFile(rigFilePath.c_str());
        if (!rigResult.has_value())
        {
            *errorMessageHolder += "[CALUMI.Animation API] ";
            *errorMessageHolder += rigResult.error().ToString().c_str();
            return false;
        }

        size_t minSize = min(translatedScene.Animations().size(), (arraySize - 1));

        for (int i = 0; i < minSize; i++)
        {
            auto animResult = translatedScene.Animations().at(i).WriteToFile(animationFilePaths.at(i).c_str());
            if (!animResult.has_value())
            {
                *errorMessageHolder = "[CALUMI.Animation API] ";
                *errorMessageHolder += animResult.error().ToString().c_str();
                return false;
            }
        }

        
        *errorMessageHolder += "Successful Save To ";
        *errorMessageHolder += pathPrintOut.c_str();
        return true;
    }

    //Warning, desired directory path array must be in the same order as the animations in the scene.
    bool SFBGS::SaveAnimationSceneToSFBGSFormatUsingRigReferencePathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, size_t arraySize, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();

        std::filesystem::path rigPath(sfbgsRigPath);
        //const char* afExt = ".af";

        SFBGS::SkeletonRig sfbgsRig;
        auto rigResult = sfbgsRig.ReadFromFile(rigPath.c_str());

        if (!rigResult.has_value())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error during rig import with error message:> ";
            *errorMessageHolder += rigResult.error().ToString().c_str();
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
            std::string nameHolder = i < scene->Animations().size() ? scene->Animations().at(i).AnimationTitle() : "NO ANIMATION FOUND IN SCENE";
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
            *errorMessageHolder += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }
        
        size_t minSize = min(translatedScene.Animations().size(), (arraySize - 1));
        for (unsigned int i = 0; i < minSize; i++)
        {
            auto animResult = translatedScene.Animations().at(i).WriteToFile(animationFilePaths.at(i).c_str());
            if (!animResult.has_value())
            {
                *errorMessageHolder += "[CALUMI.Animation API] ";
                *errorMessageHolder += animResult.error().ToString().c_str();
                return false;
            }
        }
        
        *errorMessageHolder += "[CALUMIANIMATION_API] Successful Save To:";
        *errorMessageHolder += pathPrintOut.c_str();
        return true;
    }

    bool SFBGS::SaveAnimationSceneToSFBGSFormatUsingRigReferenceC(UNIV::AnimationScene* scene, const wchar_t* directoryPath, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();
        
        std::filesystem::path rigPath(sfbgsRigPath);
        const char* afExt = ".af";

        SFBGS::SkeletonRig sfbgsRig;
        auto rigResult = sfbgsRig.ReadFromFile(rigPath.c_str());

        if (!rigResult.has_value())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error during rig import with error message:> ";
            *errorMessageHolder += rigResult.error().ToString().c_str();
            return false;
        }

        auto animPathResult = scene->GetFilePathsFromAnimationScene(directoryPath, afExt);
        if (!animPathResult.has_value())
        {
            *errorMessageHolder += animPathResult.error().c_str();
            return false;
        }
        Utilities::VectorContainer<Utilities::PathContainer> animationFilePaths = animPathResult.value();

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.ConvertFromUniversalScene(*scene, sfbgsRig))
        {
            *errorMessageHolder += "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        for (int i = 0; i < translatedScene.Animations().size(); i++)
        {
            auto animResult = translatedScene.Animations().at(i).WriteToFile(animationFilePaths.at(i).w_str());
            if (!animResult.has_value())
            {
                *errorMessageHolder += "[CALUMI.Animation API] ";
                *errorMessageHolder += animResult.error().ToString().c_str();
                return false;
            }
        }
        std::filesystem::path dirPath = directoryPath;
        *errorMessageHolder += "[CALUMI.Animation API] Successful Save To ";
        *errorMessageHolder += dirPath.string().c_str();
        return true;
    }

    UNIV::AnimationScene* SFBGS::LoadAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray, int numberOfFiles, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();

        if (numberOfFiles < 1 || !filePathsArray)
        {
            *errorMessageHolder += "[CALUMI.Animation API] No Files To Read Into Animation Scene. Returning nullptr.";
            return nullptr;
        }

        SFBGS::AnimationScene sfbgsAnimationScene;
        sfbgsAnimationScene.Animations().reserve(numberOfFiles);

        for (int i = 0; i < numberOfFiles; i++)
        {
            std::filesystem::path pathToAdd(filePathsArray[i]);
            if (pathToAdd.extension() == ".af")
            {
                SFBGS::Animation anim;
                auto animResult = anim.ReadFromFile(pathToAdd.c_str());
                if (!animResult.has_value())
                {
                    *errorMessageHolder += "[CALUMI.Animation API] ";
                    *errorMessageHolder += animResult.error().ToString().c_str();
                    return nullptr;
                }
                sfbgsAnimationScene.Animations().push_back(anim);
            }
            if (pathToAdd.extension() == ".rig")
            {
                SFBGS::SkeletonRig rig;
                auto rigResult = rig.ReadFromFile(pathToAdd.c_str());
                if (!rigResult.has_value())
                {
                    *errorMessageHolder += "[CALUMI.Animation API] ";
                    *errorMessageHolder += rigResult.error().ToString().c_str();
                    return nullptr;
                }
                sfbgsAnimationScene.Rig() = rig;
            }
        }

        if (sfbgsAnimationScene.Rig().BoneEntries().empty())
        {
            *errorMessageHolder += "[CALUMI.Animation API] No Rig Entry Found In File List. Cannot Convert!";
            return nullptr;
        }

        UNIV::AnimationScene* output = new UNIV::AnimationScene;
        *output = sfbgsAnimationScene.ConvertToUniversalScene();

        

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
                auto result = WriteToBinaryFile(jsonPath, output->ToJSON(0).c_str());
                if (!result.has_value())
                {
                    *errorMessageHolder += std::format("\n    {}", result.error().ToString().c_str()).c_str();
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
        errorMessageHolder->Clear();

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
        auto rigResult = rig.ReadFromFile(pathToLoad.c_str());
        if (!rigResult.has_value())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error during rig import with error message:> ";
            *errorMessageHolder += rigResult.error().ToString().c_str();
            return new UNIV::SkeletonRig();
        }

        UNIV::SkeletonRig* output = new UNIV::SkeletonRig;
        *output = SFBGS::ConvertToUniversalRig(rig);
        *errorMessageHolder += "[CALUMI.Animation API] Rig Loaded Successfully From File Path Provided.";
        return output;
    }

    bool SFBGS::SaveAnimationToSFBGSFormatDirectC(UNIV::Animation* animation, const wchar_t* filePath, UNIV::SkeletonRig* rig, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();
        
        if (!filePath)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No File Path Provided To Save File.";
            return false;
        }

        if (!rig || rig->BoneEntries().size() <= 0)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Rig Present Or Rig Is Empty.";
            return false;
        }
        
        if (!animation)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Animation Present.";
            return false;
        }

        SFBGS::SkeletonRig sfbgsRig = ConvertToSFBGSRig(*rig);
        SFBGS::Animation sfbgsAnimation = ConvertToSFBGSAnimation(*animation, sfbgsRig);

        auto result = sfbgsAnimation.WriteToFile(filePath);

        if (result.has_value())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Result: ";
            *errorMessageHolder += result.value();
            return true;
        }
        
        *errorMessageHolder += "[CALUMI.Animation API] ";
        *errorMessageHolder += result.error().ToString();
        return false;

    }

    bool SFBGS::SaveAnimationToSFBGSFormatWithExistingRigDirectC(UNIV::Animation* animation, const wchar_t* filePath, const wchar_t* sfbgsRigPath, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();

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
        auto rigResult = sfbgsRig.ReadFromFile(sfbgsRigPath);

        if (!rigResult.has_value() || sfbgsRig.BoneEntries().size() <= 0)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Rig Error: ";
            *errorMessageHolder += rigResult.error().ToString();
            return false;
        }

        if (!animation)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Animation Present.";
            return false;
        }

        SFBGS::Animation sfbgsAnimation = ConvertToSFBGSAnimation(*animation, sfbgsRig);

        auto result = sfbgsAnimation.WriteToFile(filePath);

        if (result.has_value())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Result: ";
            *errorMessageHolder += result.value();
            return true;
        }

        *errorMessageHolder += "[CALUMI.Animation API] ";
        *errorMessageHolder += result.error().ToString();
        return false;

    }

    bool SFBGS::SaveSkeletonRigToSFBGSFormatDirectC(UNIV::SkeletonRig* rig, const wchar_t* filePath, Utilities::StringContainer* errorMessage)
    {
        Utilities::StringContainer tempErrorMessage;
        Utilities::StringContainer* errorMessageHolder = errorMessage ? errorMessage : &tempErrorMessage;
        errorMessageHolder->Clear();

        if (!filePath)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No File Path Provided To Save File.";
            return false;
        }

        if (!rig || rig->BoneEntries().size() <= 0)
        {
            *errorMessageHolder += "[CALUMI.Animation API] Error, No Rig Present Or Rig Is Empty.";
            return false;
        }

        SFBGS::SkeletonRig sfbgsRig = ConvertToSFBGSRig(*rig);

        auto result = sfbgsRig.WriteToFile(filePath);

        if (result.has_value())
        {
            *errorMessageHolder += "[CALUMI.Animation API] Result: ";
            *errorMessageHolder += result.value();
            return true;
        }

        *errorMessageHolder += "[CALUMI.Animation API] ";
        *errorMessageHolder += result.error().ToString();
        return false;
    }

    

}






