//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "SFBGS_AnimationScene.h"



namespace CALUMI{ namespace SFBGS{


    //Sequence Conversions Directly On Animation Blocks
    void _ConvertScalarSq(const std::vector<CALUMI::UNIV::Scalar>& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock._scalarCount = input.size();
        outputBlock.scalarEntries.reserve(outputBlock._scalarCount);
        outputBlock.scalarKeyFrames.reserve(outputBlock._scalarCount);
        for (const CALUMI::UNIV::Scalar& scalar : input)
        {
            outputBlock.scalarKeyFrames.push_back(scalar.frame);
            outputBlock.scalarEntries.push_back((uint16_t)(scalar.scalar * 5000));
        }
    }

    std::vector<CALUMI::UNIV::Scalar> _ConvertScalarSq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        std::vector<CALUMI::UNIV::Scalar> output;
        output.reserve(inputBlock._scalarCount);
        for (unsigned int i = 0; i < inputBlock._scalarCount; i++)
        {
            CALUMI::UNIV::Scalar toAdd(inputBlock.scalarKeyFrames.at(i),(inputBlock.scalarEntries.at(i)/5000.0));
            output.push_back(toAdd);
        }
        return output;
    }

    void _ConvertPrioritySq(const std::vector<CALUMI::UNIV::Priority>& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock._bonePriorityCount = input.size();
        outputBlock.bonePriorityEntries.resize(outputBlock._bonePriorityCount);
        outputBlock.bonePriorityKeyFrames.resize(outputBlock._bonePriorityCount);
        for (unsigned int i = 0; i < outputBlock._bonePriorityCount; i++)
        {
            outputBlock.bonePriorityKeyFrames.at(i) = input.at(i).frame;
            outputBlock.bonePriorityEntries.at(i) = input.at(i).priority;
        }
    }

    std::vector<CALUMI::UNIV::Priority> _ConvertPrioritySq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        std::vector<CALUMI::UNIV::Priority> output;
        output.reserve(inputBlock._bonePriorityCount);
        for (unsigned int i = 0; i < inputBlock._bonePriorityCount; i++)
        {
            CALUMI::UNIV::Priority toAdd(inputBlock.bonePriorityKeyFrames.at(i), inputBlock.bonePriorityEntries.at(i));
            output.push_back(toAdd);
        }
        return output;
    }

    void _ConvertRotationSq(const std::vector<CALUMI::UNIV::Rotation>& input, CALUMI::SFBGS::AnimationBlock& outputBlock)
    {
        outputBlock._rotationCount = input.size();
        outputBlock.rotationKeyFrames.reserve(input.size());
        outputBlock.rotationEntries.reserve(input.size());
        outputBlock.rotationPrefixEntries.reserve(input.size());
        for (const CALUMI::UNIV::Rotation& rotation : input)
        {
            outputBlock.rotationKeyFrames.push_back(rotation.frame);
            auto result = GetSFBGSRotationPair(rotation.rotation);
            outputBlock.rotationPrefixEntries.push_back(result.first);
            outputBlock.rotationEntries.push_back(result.second);
        }

        outputBlock.rotationPrefixEntries = FoldRotationPrefixSequence(outputBlock.rotationPrefixEntries);
        outputBlock._rotationPrefixCount = outputBlock.rotationPrefixEntries.size();
    }

    std::vector<CALUMI::UNIV::Rotation> _ConvertRotationSq(const CALUMI::SFBGS::AnimationBlock& inputBlock)
    {
        std::vector<CALUMI::UNIV::Rotation> output;
        output.reserve(inputBlock._rotationCount);
        std::vector<CALUMI::SFBGS::RotationPrefix> unfoldedPrefixes = UnfoldRotationPrefixSequence(inputBlock.rotationPrefixEntries);
        for (unsigned int i = 0; i < inputBlock._rotationCount; i++)
        {
            CALUMI::UNIV::Rotation toAdd(inputBlock.rotationKeyFrames.at(i), GetUniversalRotation(unfoldedPrefixes.at(i), inputBlock.rotationEntries.at(i)));
            output.push_back(toAdd);
        }
        return output;
    }

    void _ConvertTranslationSq(const std::vector<CALUMI::UNIV::Translation>& input, CALUMI::SFBGS::AnimationBlock& outputBlock, const float& highPrecision, const float& lowPrecision)
    {
        outputBlock._translationCount = input.size();
        outputBlock.translationKeyFrames.reserve(input.size());
        outputBlock.translationEntries.reserve(input.size());
        outputBlock.translationPrefixEntries.reserve(input.size());
        for (const CALUMI::UNIV::Translation& translation : input)
        {
            outputBlock.translationKeyFrames.push_back(translation.frame);
            auto result = GetSFBGSTranslationPair(translation.translation, highPrecision, lowPrecision);
            outputBlock.translationPrefixEntries.push_back(result.first);
            outputBlock.translationEntries.push_back(result.second);
        }
        outputBlock.translationPrefixEntries = FoldTranslationPrefixSequence(outputBlock.translationPrefixEntries);
        outputBlock._translationPrefixCount = outputBlock.translationPrefixEntries.size();
    }

    std::vector<CALUMI::UNIV::Translation> _ConvertTranslationSq(const CALUMI::SFBGS::AnimationBlock& inputBlock, const float& highPrecision, const float& lowPrecision)
    {
        std::vector<CALUMI::UNIV::Translation> output;
        output.reserve(inputBlock._translationCount);
        std::vector<CALUMI::SFBGS::TranslationPrefix> unfoldedPrefixes = UnfoldTranslationPrefixSequence(inputBlock.translationPrefixEntries);
        for (unsigned int i = 0; i < inputBlock._translationCount; i++)
        {
            CALUMI::UNIV::Translation toAdd(inputBlock.translationKeyFrames.at(i), GetUniversalTranslation(unfoldedPrefixes.at(i), inputBlock.translationEntries.at(i), highPrecision, lowPrecision));
            output.push_back(toAdd);
        }
        return output;
    }


    //Animation Conversion
    Animation ConvertToSFBGSAnimation(CALUMI::UNIV::Animation& anim, const CALUMI::SFBGS::SkeletonRig& rig)
    {
        Animation output;
        output.animationFileName = anim.animationTitle;
        output.HeaderStart[2] = 1.0; //Setting header quaternion to w=1. Unknown what this does still

        output._boneCount = anim.boneCount; //Just listing all bones for now. Will need compression algorithm eventually
        output._frameCount = anim.GetFrameCount();

        output._indexAtlas.reserve(output._indexAtlasCounter);
        output._indexAtlas.push_back(0);
        output._indexAtlas.push_back(output._boneCount);


        output.animationBlocks.resize(output._boneCount); //Create empty blocks for all bones
        for (CALUMI::UNIV::AnimationBlock entry : anim.animationBlocks)
        {
            AnimationBlock toAdd;
            _ConvertRotationSq(entry._rotationSequence, toAdd);
            _ConvertTranslationSq(entry._translationSequence, toAdd, rig.highPrecision, rig.lowPrecision);
            _ConvertScalarSq(entry._scalarSequence, toAdd);
            _ConvertPrioritySq(entry._prioritySequence, toAdd);
            if (entry.boneName != rig.stringArray.at(entry.boneIndex))
            {
                std::println("UNIV Bone Entry Name Does Not Match SFBGS RIG Bone Index Name");
                std::cin.get();
            }

            output.animationBlocks.at(entry.boneIndex) = toAdd; //Fill block for the correct index
        }
        output._evaluateHeaderFlags();
        return output;
    }

    CALUMI::UNIV::Animation ConvertToUniversalAnimation(CALUMI::SFBGS::Animation& anim, const CALUMI::SFBGS::SkeletonRig& rig)
    {
        CALUMI::UNIV::Animation output;
        output.animationTitle = anim.animationFileName;
        output.boneCount = anim._boneCount;

        output.animationBlocks.reserve(anim.animationBlocks.size());
        for (unsigned int i = 0; i < anim.animationBlocks.size(); i++)
        {
            if (anim.animationBlocks.at(i)._rotationCount || anim.animationBlocks.at(i)._translationCount || anim.animationBlocks.at(i)._scalarCount || anim.animationBlocks.at(i)._bonePriorityCount)
            {
                CALUMI::UNIV::AnimationBlock toAdd;
                toAdd.boneIndex = i;
                toAdd.boneName = rig.stringArray.at(i);
                
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
        for (CALUMI::UNIV::Animation anim : input.animations)
        {
            animations.push_back(ConvertToSFBGSAnimation(anim,rig));
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
        for (CALUMI::UNIV::Animation anim : input.animations)
        {
            animations.push_back(ConvertToSFBGSAnimation(anim, rig));
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
        outputScene.rig = ConvertToUniversalRig(rig);

        outputScene.animations.reserve(animations.size());
        for (CALUMI::SFBGS::Animation anim : animations)
        {
            outputScene.animations.push_back(ConvertToUniversalAnimation(anim, rig));
        }

        return outputScene;
    }

}

    //EXTERN C ===============================================================================================================================================================================
    bool SFBGS::SaveAnimationSceneToSFBGSFormatC(UNIV::AnimationScene* scene, float highPrecisionValue, float lowPrecisionValue, const wchar_t* directoryPath, const char* errorMessage)
    {
        if (scene->rig.boneEntries.empty())
        {
            errorMessage = "[CALUMI.Animation API] No Rig Entry Found In Animation Scene. Cannot Convert";
            return false;
        }

        const char* rigExt = ".rig";
        const char* afExt  = ".af";
        if (scene->rig.rigName.empty())
        {
            errorMessage = "[CALUMI.Animation API] Empty string found for Rig Name";
            return false;
        }
        std::filesystem::path rigFilePath(directoryPath);
        //rigFilePath /= scene->rig.rigName;
        rigFilePath /= "characterassets";
        rigFilePath /= "skeleton.rig";
        rigFilePath.replace_extension(rigExt);

        auto animPathResult = scene->GetFilePathsFromAnimationScene(directoryPath, afExt);
        if (!animPathResult.has_value())
        {
            errorMessage = animPathResult.error().c_str();
            return false;
        }
        std::vector<std::filesystem::path> animationFilePaths = animPathResult.value();

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.ConvertFromUniversalScene(*scene))
        {
            errorMessage = "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        auto rigResult = translatedScene.rig.WriteToFile(rigFilePath);
        if (!rigResult.has_value())
        {
            std::string temp = "[CALUMI.Animation API] " + rigResult.error().ToString();
            errorMessage = temp.c_str();
            return false;
        }
    
        for (int i = 0; i < translatedScene.animations.size(); i++)
        {
            auto animResult = translatedScene.animations.at(i).WriteToFile(animationFilePaths.at(i));
            if (!animResult.has_value())
            {
                std::string temp = "[CALUMI.Animation API] " + animResult.error().ToString();
                errorMessage = temp.c_str();
                return false;
            }
        }

        std::filesystem::path dirPath = directoryPath;
        char buffer[] = "Successful Save To ";
        strcat_s(buffer, 240, dirPath.string().c_str());
        errorMessage = buffer;
        return true;
    }

    //Warning, desired directory path array must be in the same order as the animations in the scene. .rig file must be present and can be placed anywhere within the array. 
    bool SFBGS::SaveAnimationSceneToSFBGSFormatPathOverrideC(UNIV::AnimationScene* scene, float highPrecisionValue, float lowPrecisionValue, const wchar_t** directoryPathArray, size_t arraySize, const char* errorMessage)
    {
        if (scene->rig.boneEntries.empty())
        {
            errorMessage = "[CALUMI.Animation API] No Rig Entry Found In Animation Scene. Cannot Convert";
            return false;
        }

        if (arraySize < 1 || !directoryPathArray || !directoryPathArray[0])
        {
            errorMessage = ("[CALUMI.Animation API] Error, File Path Array Not Found");
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
            errorMessage = "[CALUMI.Animation API] Error, No .rig File Path Found";
            return false;
        }

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.ConvertFromUniversalScene(*scene))
        {
            errorMessage = "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        auto rigResult = translatedScene.rig.WriteToFile(rigFilePath);
        if (!rigResult.has_value())
        {
            std::string temp = "[CALUMI.Animation API] " + rigResult.error().ToString();
            errorMessage = temp.c_str();
            return false;
        }

        unsigned int minSize = min(translatedScene.animations.size(), (arraySize-1));

        for (int i = 0; i < minSize; i++)
        {
            auto animResult = translatedScene.animations.at(i).WriteToFile(animationFilePaths.at(i));
            if (!animResult.has_value())
            {
                std::string temp = "[CALUMI.Animation API] " + animResult.error().ToString();
                errorMessage = temp.c_str();
                return false;
            }
        }

        
        char buffer[] = "Successful Save To ";
        strcat_s(buffer,240, pathPrintOut.c_str());
        errorMessage = buffer;
        return true;
    }

    //Warning, desired directory path array must be in the same order as the animations in the scene.
    bool SFBGS::SaveAnimationSceneToSFBGSFormatUsingRigReferencePathOverrideC(UNIV::AnimationScene* scene, const wchar_t** directoryPathArray, size_t arraySize, const wchar_t* sfbgsRigPath, const char* errorMessage)
    {
        std::filesystem::path rigPath(sfbgsRigPath);
        //const char* afExt = ".af";

        SFBGS::SkeletonRig sfbgsRig;
        auto rigResult = sfbgsRig.ReadFromFile(rigPath);

        if (!rigResult.has_value())
        {
            char buffer[] = "[CALUMI.Animation API] Error during rig import with error message:> ";
            strcat_s(buffer, 240, rigResult.error().ToString().c_str());

            errorMessage = buffer;
            return false;
        }
        if (arraySize < 1 || !directoryPathArray || !directoryPathArray[0])
        {
            errorMessage = ("[CALUMI.Animation API] Error, File Path Array Not Found");
            return false;
        }

        std::string pathPrintOut;
        std::vector<std::filesystem::path> animationFilePaths;
        animationFilePaths.reserve(arraySize);

        for (unsigned int i = 0; i < arraySize; i++)
        {
            std::filesystem::path pathToAdd(directoryPathArray[i]);
            animationFilePaths.push_back(pathToAdd);
            pathPrintOut += pathToAdd.string();
            pathPrintOut += "\n";
        }
        animationFilePaths.shrink_to_fit();
        pathPrintOut += "Using Rig From: ";
        pathPrintOut += rigPath.string() + "\n";

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.ConvertFromUniversalScene(*scene, sfbgsRig))
        {
            errorMessage = "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        for (int i = 0; i < translatedScene.animations.size(); i++)
        {
            auto animResult = translatedScene.animations.at(i).WriteToFile(animationFilePaths.at(i));
            if (!animResult.has_value())
            {
                std::string temp = "[CALUMI.Animation API] " + animResult.error().ToString();
                errorMessage = temp.c_str();
                return false;
            }
        }
        
        errorMessage = pathPrintOut.c_str();
        return true;
    }

    bool SFBGS::SaveAnimationSceneToSFBGSFormatUsingRigReferenceC(UNIV::AnimationScene* scene, const wchar_t* directoryPath, const wchar_t* sfbgsRigPath, const char* errorMessage)
    {
        std::filesystem::path rigPath(sfbgsRigPath);
        const char* afExt = ".af";

        SFBGS::SkeletonRig sfbgsRig;
        auto rigResult = sfbgsRig.ReadFromFile(rigPath);

        if (!rigResult.has_value())
        {
            char buffer[] = "[CALUMI.Animation API] Error during rig import with error message:> ";
            strcat_s(buffer, 240, rigResult.error().ToString().c_str());

            errorMessage = buffer;
            return false;
        }

        auto animPathResult = scene->GetFilePathsFromAnimationScene(directoryPath, afExt);
        if (!animPathResult.has_value())
        {
            errorMessage = animPathResult.error().c_str();
            return false;
        }
        std::vector<std::filesystem::path> animationFilePaths = animPathResult.value();

        SFBGS::AnimationScene translatedScene;
        if (!translatedScene.ConvertFromUniversalScene(*scene, sfbgsRig))
        {
            errorMessage = "[CALUMI.Animation API] Could not convert animation scene";
            return false;
        }

        for (int i = 0; i < translatedScene.animations.size(); i++)
        {
            auto animResult = translatedScene.animations.at(i).WriteToFile(animationFilePaths.at(i));
            if (!animResult.has_value())
            {
                std::string temp = "[CALUMI.Animation API] " + animResult.error().ToString();
                errorMessage = temp.c_str();
                return false;
            }
        }
        std::filesystem::path dirPath = directoryPath;
        char buffer[] = "Successful Save To ";
        strcat_s(buffer, 240, dirPath.string().c_str());
        errorMessage = buffer;
        return true;
    }

    UNIV::AnimationScene* SFBGS::LoadAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray, int numberOfFiles, const char* errorMessage)
    {
        if (numberOfFiles < 1 || !filePathsArray)
        {
            errorMessage = "[CALUMI.Animation API] No Files To Read Into Animation Scene. Returning nullptr.";
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
                auto animResult = anim.ReadFromFile(pathToAdd);
                if (!animResult.has_value())
                {
                    std::string temp = "[CALUMI.Animation API] " + animResult.error().ToString();
                    errorMessage = temp.c_str();
                    return nullptr;
                }
                sfbgsAnimationScene.animations.push_back(anim);
            }
            if (pathToAdd.extension() == ".rig")
            {
                SFBGS::SkeletonRig rig;
                auto rigResult = rig.ReadFromFile(pathToAdd);
                if (!rigResult.has_value())
                {
                    std::string temp = "[CALUMI.Animation API] " + rigResult.error().ToString();
                    errorMessage = temp.c_str();
                    return nullptr;
                }
                sfbgsAnimationScene.rig = rig;
            }
        }

        if (sfbgsAnimationScene.rig.boneEntries.empty())
        {
            errorMessage = "[CALUMI.Animation API] No Rig Entry Found In File List. Cannot Convert!";
            return nullptr;
        }

        UNIV::AnimationScene* output = new UNIV::AnimationScene;
        *output = sfbgsAnimationScene.ConvertToUniversalScene();

        Utilities::WriteJSONToFile("C:/General Files/Git/AnimScript/AnimStarfieldMeshConverter_repo/scripts/tool_export_mesh/ExportedJSONs/LastFile.json", output->ToJSON());

        errorMessage = "[CALUMI.Animation API] AnimationScene Created. Please Remember To call DeleteAnimationSceneC(ptr) When Finished.";
        return output;
    }

}






