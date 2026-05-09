//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "animation/SFBGS_Animation.h"
#include "interfaces/IConvertible.h"
#include "skeletonrig/SFBGS_SkeletonRig.h"
#include "univ/UNIV_AnimationScene.h"
#include "utilities/CALUMI_Common.h"


namespace CALUMI::SFBGS
{
    class CALUMIANIMATION_API AnimationScene : UNIV::IConvertibleScene
    {
    public:
        [[nodiscard]] Utilities::StringContainer& sceneName() const;
        [[nodiscard]] Animation& animation(uint64_t idx) const;
        [[nodiscard]] uint64_t animationCount() const;
        [[nodiscard]] SkeletonRig& rig() const;
        void setRig(const SkeletonRig& rig) const;

        AnimationScene();
        ~AnimationScene() override;
        explicit AnimationScene(const char* sceneName);
        explicit AnimationScene(const Utilities::StringContainer& sceneName);
        AnimationScene(const AnimationScene& input);

        bool addAnimationToScene(const Animation& animation, bool overwrite = true) const;
        bool removeAnimationFromScene(const Utilities::StringContainer& sceneToRemove) const;
        bool removeAnimationFromScene(const char* sceneToRemove) const;
        bool removeAnimationFromScene(unsigned int idx) const;

        // Inherited via IConvertibleScene
        bool convertFromUniversalScene(UNIV::AnimationScene& input) override;
        bool convertFromUniversalScene(const UNIV::AnimationScene& input, const SkeletonRig& rigReference) const;
        UNIV::AnimationScene convertToUniversalScene() override;

        AnimationScene& operator=(const AnimationScene& input);

    private:
        struct Impl;
        Impl* pImpl;
    };
}

/// @addtogroup extern_c
/// @{
/// @addtogroup c_animation_scene
/// @{
/// @defgroup extern_c_starfield_scene Starfield
/// @{

extern "C" {
    /**
     *
     * @param scene
     * @param directoryPathArray
     * @param arraySize
     * @return Error Code:\n -1 = Invalid Data\n 0 = Successful Operation\n 1 = No Rig Filepath Found\n 2 = Scene Could
     * Not Be Converted\n-----\n 1X = Rig File Error\n 2X = Animation File Error\n-----\n X0 = FileNotFound\n X1 =
     * PermissionDenied\n
     * X2 = NotAFile\n X3 = ReadFailure\n X4 = IncorrectFileType\n X5 = FileTooLarge\n X6 = FileTooSmall\n X7 =
     * UnknownErrorCode\n X8 = WriteFailure\n X9 = File Could Not Be Written For Another Reason
     */
    CALUMIANIMATION_API int SaveAnimationSceneToSFBGSFormatPathOverrideC(CALUMI::UNIV::AnimationScene* scene,
                                                                         const wchar_t** directoryPathArray,
                                                                         uint64_t arraySize);
    /**
     *
     * @param scene
     * @param directoryPathArray
     * @param arraySize
     * @param sfbgsRigPath
     * @return Error Code:\n -1 = Invalid Data\n 0 = Successful Operation\n 1 = No Rig Filepath Found\n 2 = Scene Could
     * Not Be Converted\n-----\n 1X = Rig File Error\n 2X = Animation File Error\n-----\n X0 = FileNotFound\n X1 =
     * PermissionDenied\n
     * X2 = NotAFile\n X3 = ReadFailure\n X4 = IncorrectFileType\n X5 = FileTooLarge\n X6 = FileTooSmall\n X7 =
     * UnknownErrorCode\n X8 = WriteFailure\n X9 = File Could Not Be Written For Another Reason
     */
    CALUMIANIMATION_API int SaveAnimationSceneToSFBGSFormatUsingRigReferencePathOverrideC(
                                                                const CALUMI::UNIV::AnimationScene* scene,
                                                                const wchar_t**                     directoryPathArray,
                                                                uint64_t                            arraySize,
                                                                const wchar_t*                      sfbgsRigPath);
    /**
     *
     * @param filePathsArray
     * @param numberOfFiles
     * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteAnimationSceneC
     * @return nullptr if error occurs, otherwise ptr to heap allocated scene
     */
    CALUMIANIMATION_API CALUMI::UNIV::AnimationScene* LoadAnimationSceneFromSFBGSFormatC(const wchar_t** filePathsArray,
                                                                                         int             numberOfFiles);
    /**
     *
     * @param filePath File path to Starfield skeleton.rig
     * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteSkeletonRigC
     * @return nullptr if error occurs, other ptr to heap allocated converted UNIV rig
     */
    CALUMIANIMATION_API CALUMI::UNIV::SkeletonRig* LoadSFBGSSkeletonRigFromFileC(const wchar_t* filePath);
    /**
     *
     * @param animation
     * @param filePath
     * @param rig
     * @return Error Code:\n -1 = Invalid Data\n 0 = Successful Operation\n 1 = No Rig Filepath Found\n 2 = Scene Could
     * Not Be Converted\n-----\n 1X = Rig File Error\n 2X = Animation File Error\n-----\n X0 = FileNotFound\n X1 =
     * PermissionDenied\n
     * X2 = NotAFile\n X3 = ReadFailure\n X4 = IncorrectFileType\n X5 = FileTooLarge\n X6 = FileTooSmall, X7 =
     * UnknownErrorCode\n X8 = WriteFailure\n X9 = File Could Not Be Written For Another Reason
     */
    CALUMIANIMATION_API int SaveAnimationToSFBGSFormatDirectC(const CALUMI::UNIV::Animation*   animation,
                                                              const wchar_t*                   filePath,
                                                              const CALUMI::UNIV::SkeletonRig* rig);
    /**
     *
     * @param animation
     * @param filePath
     * @param sfbgsRigPath
     * @return Error Code:\n -1 = Invalid Data\n 0 = Successful Operation\n 1 = No Rig Filepath Found\n 2 = Scene Could
     * Not Be Converted\n-----\n 1X = Rig File Error\n 2X = Animation File Error\n-----\n X0 = FileNotFound\n X1 =
     * PermissionDenied\n
     * X2 = NotAFile\n X3 = ReadFailure\n X4 = IncorrectFileType\n X5 = FileTooLarge\n X6 = FileTooSmall\n X7 =
     * UnknownErrorCode\n X8 = WriteFailure\n X9 = File Could Not Be Written For Another Reason
     */
    CALUMIANIMATION_API int SaveAnimationToSFBGSFormatWithExistingRigDirectC(const CALUMI::UNIV::Animation* animation,
                                                                             const wchar_t* filePath,
                                                                             const wchar_t* sfbgsRigPath);
    /**
     *
     * @param rig
     * @param filePath
     * @return Error Code:\n -1 = Invalid Data\n 0 = Successful Operation\n 1 = No Rig Filepath Found\n 2 = Scene Could
     * Not Be Converted\n-----\n 1X = Rig File Error\n 2X = Animation File Error\n-----\n X0 = FileNotFound\n X1 =
     * PermissionDenied\n
     * X2 = NotAFile\n X3 = ReadFailure\n X4 = IncorrectFileType\n X5 = FileTooLarge\n X6 = FileTooSmall\n X7 =
     * UnknownErrorCode\n X8 = WriteFailure\n X9 = File Could Not Be Written For Another Reason
     */
    CALUMIANIMATION_API int SaveSkeletonRigToSFBGSFormatDirectC(const CALUMI::UNIV::SkeletonRig* rig,
                                                                const wchar_t*                   filePath);
}

/// @}
/// @}
/// @}
