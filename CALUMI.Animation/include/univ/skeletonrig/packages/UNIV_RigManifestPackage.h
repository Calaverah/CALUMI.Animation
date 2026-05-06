//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com
#pragma once

#include "../UNIV_IRigPackage.h"

namespace CALUMI::UNIV
{
    /**
     * @relates RigManifestPackage
     */
    inline constexpr auto MANIFEST_RIG_PACKAGE = "MANIFEST_RIG_PACKAGE";

    /// @addtogroup rig_packages
    /// @{
    /**
     * @brief Universal package that records the preferred index order of a rig
     * @details When importing or exporting, this package can assist in dictating the output order of a rig's bones
     */
    class CALUMIANIMATION_API RigManifestPackage : public IRigPackage
    {
    public:
        /// @name Constructors
        /// @{
        RigManifestPackage();
        /**
         * @param other
         */
        RigManifestPackage(const RigManifestPackage& other);
        ~RigManifestPackage() override;
        /// @}
        /// @name Operators
        /// @{
        /**
         * @param other
         * @return
         */
        RigManifestPackage& operator=(const RigManifestPackage& other);
        /// @}

        /// @name Bone Order
        /// @{

        /**
         * @brief Adds a bone to the end of the manifest
         * @param boneName
         * @return Whether the boneName was accepted into the manifest
         */
        bool addBone(const char* boneName) const;
        /**
         *
         * @param boneName
         * @param index The desired index of the manifest to insert this entry
         * @return Whether the bone was successfully added, if index is out of range will return false
         */
        bool insertBone(const char* boneName, unsigned int index) const;
        /**
         *
         * @param boneName
         * @return
         */
        bool removeBone(const char* boneName) const;
        /**
         *
         * @return The number of entries currently in the manifest
         */
        [[nodiscard]] unsigned int count() const;
        /**
         *
         * @param index
         * @return
         */
        [[nodiscard]] const char* bone(unsigned int index) const;
        /// @}
        /// @name Inherited
        /// @{
        /**
         *
         * @return "MANIFEST_RIG_PACKAGE"
         */
        [[nodiscard]] const char* packageType() const override;

    protected:
        /**
         * @return
         */
        [[nodiscard]] IPackage* clone() const override;
        /**
         * @param oldBone
         * @param newName
         * @return
         */
        bool handleBoneRename(const char* oldBone, const char* newName) override;
        /// @}

    public:
        /// @name Package Handling
        /// @{
        /**
         * @brief
         * @param rig
         * @param overwrite
         * @return Whether the operation was successful
         */
        static bool AddPackage(const SkeletonRig& rig, bool overwrite = true);
        /**
         *
         * @param rig
         * @return The Rig Manifest Package on this SkeletonRig. If none exists, one will be created.
         */
        static RigManifestPackage& GetPackage(const SkeletonRig& rig);
        /**
         *
         * @param rig
         * @return
         */
        static bool RemovePackage(const SkeletonRig& rig);
        /**
         * @details Prioritizing the manifest defined in the package, this function creates a list of the bones in the
         * desired index order. The only entries that would come before those listed in the manifest, would be
         * the parent/ancestors of those bones so that a child never comes before its parent.
         * @param rig
         * @param forceLineage
         * @return A string list of the bone entries in the rig in a specific order
         */
        [[nodiscard]] Utilities::StringList processPackage(const SkeletonRig& rig, bool forceLineage = false) const;
        /// @}
        /// @name Json
        /// @{
        Utilities::JsonObject toJson() const override;
        void fromJson(const Utilities::JsonObject& data) override;
        /// @}

    private:
        struct Impl;
        Impl* pImpl;

    };

    /// @}
}

/// @addtogroup c_univ_rig_packages
/// @{
/// @defgroup c_univ_manifest_rig_package Manifest Rig Package
/// @{

extern "C"
{
    /**
     *
     * @brief Adds a Manifest Package to the UNIV Skeleton Rig
     * @param rig
     * @param overwrite If this package already exists, will reset it to default
     * @return Error Code:\n -1 Invalid Ptr\n 0 = Successful Operation\n 1 = Package was not added to rig
     */
    CALUMIANIMATION_API int UNIVManifestRigPackage_AddPackageToSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig, bool overwrite);
    /**
     * @brief Removes the Manifest Package from a UNIV Skeleton Rig
     * @param rig
     * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Package was not removed
     */
    CALUMIANIMATION_API int UNIVManifestRigPackage_RemoveRigPackageFromSkeletonRigC(const CALUMI::UNIV::SkeletonRig* rig);
    /**
     * @brief
     * @param rig
     * @param boneName
     * @return Error Code:\n -1 = Invalid Ptr\n 0 = Bone successfully added\n 1 = Bone was not added to manifest
     */
    CALUMIANIMATION_API int UNIVManifestRigPackage_AddBoneC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName);
    /**
     *
     * @param rig
     * @param boneName
     * @param index
     * @return Error Code:\n -1 = Invalid Ptr\n 0 = Bone successfully added\n 1 = Bone was not added to manifest
     */
    CALUMIANIMATION_API int UNIVManifestRigPackage_InsertBoneC(const CALUMI::UNIV::SkeletonRig* rig,
                                                               const char* boneName, unsigned int index);
    /**
     *
     * @param rig
     * @param boneName
     * @return Error Code:\n -1 = Invalid Ptr\n 0 = Bone successfully added\n 1 = Bone was not added to manifest
     */
    CALUMIANIMATION_API int UNIVManifestRigPackage_RemoveBoneC(const CALUMI::UNIV::SkeletonRig* rig, const char* boneName);
    /**
     *
     * @param rig
     * @return Bone count or -1 if error occurs
     */
    CALUMIANIMATION_API int64_t UNIVManifestRigPackage_GetCountC(const CALUMI::UNIV::SkeletonRig* rig);
    /**
     *
     * @param rig
     * @param index
     * @return Bone name or nullptr if index is out of range or there is an error
     */
    CALUMIANIMATION_API const char* UNIVManifestRigPackage_GetBoneC(const CALUMI::UNIV::SkeletonRig* rig, unsigned int index);
}

/// @}
/// @}
