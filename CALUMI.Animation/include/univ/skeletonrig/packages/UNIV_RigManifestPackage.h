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
        RigManifestPackage(const RigManifestPackage& other);
        ~RigManifestPackage() override;
        /// @}
        /// @name Operators
        /// @{
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
        [[nodiscard]] const char* getPackageType() const override;
        /**
         * @param indents
         * @return
         */
        [[deprecated]] [[nodiscard]] Utilities::StringContainer toJSON(uint64_t indents) const override;

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
         * @return A string list of the bone entries in the rig in a specific order
         */
        [[nodiscard]] Utilities::StringList processPackage(const SkeletonRig& rig) const;
        /// @}

    private:
        struct Impl;
        Impl* pImpl;

    };

    /// @}
}
