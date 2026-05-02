//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com
#pragma once

#include "../UNIV_IRigPackage.h"

namespace CALUMI::UNIV
{
    /**
     * @relates RigMirrorPackage
     */
    inline constexpr auto MIRROR_RIG_PACKAGE = "MIRROR_RIG_PACKAGE";

    /// @addtogroup rig_packages
    /// @{
    class CALUMIANIMATION_API RigMirrorPackage : public IRigPackage
    {
    public:
        RigMirrorPackage();
        ~RigMirrorPackage() override;

        /**
         * @brief Creates a pairing definition, if package is set to exclusive, any previous pairings for each of the
         * bones will be erased. If one entry is left blank, it will simply remove all uses of the non-blank entry.
         * If both entries are blank, nothing will happen.
         * @param bone1
         * @param bone2
         */
        void addPair(const char* bone1, const char* bone2) const;
        /**
         *
         * @param bone The bone whose mirror should be found
         * @return The mirroring bone to the input, or an empty string if none were found
         */
        [[nodiscard]] const char* pairedBone(const char* bone) const;
        /**
         *
         * @return The amount of pairs that exist in the package
         */
        [[nodiscard]] unsigned int pairCount() const;
        /**
         *
         * @param index
         * @return The name of the first of the pairing at the given index
         */
        [[nodiscard]] const char* firstOfPair(unsigned int index) const;
        /**
         *
         * @param index
         * @return The name of the second of the pairing at the given index
         */
        [[nodiscard]] const char* secondOfPair(unsigned int index) const;
        /**
         *
         * @return "RIG_MIRROR_PACKAGE"
         */
        [[nodiscard]] const char* packageType() const override;

    protected:
        /**
         *
         * @return Heap allocated cloned package of this one
         */
        [[nodiscard]] IPackage* clone() const override;
        /**
         *
         * @param oldBone
         * @param newName
         * @return
         */
        bool handleBoneRename(const char* oldBone, const char* newName) override;

    public:
        RigMirrorPackage& operator=(const RigMirrorPackage& other);

        /// @name Json
        /// @{
        Utilities::JsonObject toJson() const override;
        /// @}

    private:
        struct Impl;
        Impl* pImpl;

    public:
        /// @name Package Handling
        /// @{
        /**
         * @brief Method for adding a Mirror Rig Package to the UNIV Skeleton Rig
         * @param rig
         * @param overwrite Will overwrite with default package if existing package is found
         * @return
         */
        static bool AddPackage(const SkeletonRig& rig, bool overwrite = true);
        /**
         * @brief Method for removing a Mirror Rig Package from a UNIV Skeleton Rig
         * @param rig
         * @return
         */
        static bool RemovePackage(const SkeletonRig& rig);
        /**
         *
         * @param rig
         * @return The Rig Mirror Package on this SkeletonRig. If none exists, one will be created.
         */
        static RigMirrorPackage& GetPackage(const SkeletonRig& rig);
        /// @}
    };
    /// @}
}




    /// @addtogroup c_univ_rig_packages
    /// @{
    /// @defgroup c_univ_mirror_rig_package Mirror Rig Package
    /// @{

    extern "C"
    {
    /**
     * @brief Adds a Mirror Rig Package to the UNIV Skeleton Rig
     * @param rig UNIV Rig
     * @param overwrite If set to true, will replace the Mirror Rig package, if one exists
     * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Existing package was found and not
     * overwritten
     */
    CALUMIANIMATION_API int UNIVMirrorRigPackage_AddPackageToSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig, bool
        overwrite);
    /**
     * @brief Removes the Mirror Package from a UNIV Skeleton Rig
     * @param rig
     * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Package was not removed
     */
    CALUMIANIMATION_API int UNIVMirrorRigPackage_RemoveRigPackageFromSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig);
    /**
     * @brief Adds a mirror pair to the Mirror Package from a UNIV Skeleton Rig. If one of the two entries is blank,
     * then it will clear the other non-empty entry from the list. If both are blank, nothing will happen.
     * @param rig
     * @param bone1
     * @param bone2
     * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Package not found
     */
    CALUMIANIMATION_API int UNIVMirrorRigPackage_AddPairToRigPackageFromSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig,
                                                                                     const char* bone1,
                                                                                     const char* bone2);
    /**
     * @param rig
     * @return Error Code:\n -1 = Invalid Ptr\n 0 = No entries or no package found\n n = number of entries
     */
    CALUMIANIMATION_API int64_t UNIVMirrorRigPackage_GetPairCountC(const CALUMI::UNIV::SkeletonRig* rig);
    /**
     * @param rig
     * @param index
     * @return The name of the first entry at the given mirror entry index, empty c string if none are found, or a
     * nullptr if there was an invalid ptr error
     */
    CALUMIANIMATION_API const char* UNIVMirrorRigPackage_GetFirstOfPairEntryC(const CALUMI::UNIV::SkeletonRig* rig,
                                                                             unsigned int index);
    /**
     * @param rig
     * @param index
     * @return The name of the second entry at the given mirror entry index, empty c string if none are found, or a
     * nullptr if there was an invalid ptr error
     */
    CALUMIANIMATION_API const char* UNIVMirrorRigPackage_GetSecondOfPairEntryC(const CALUMI::UNIV::SkeletonRig* rig,
                                                                             unsigned int index);
    }
    /// @}
    /// @}

