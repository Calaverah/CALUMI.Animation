//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "interfaces/IReadWritable.h"
#include "SFBGS_AnimationEntries.h"
#include "univ/animation/UNIV_AnimationEntries.h"
#include <io/FileResult.h>


namespace CALUMI::SFBGS
{
    class AnimationScene;

    /**
     * @brief Flags defined in the SFBGS animation file's header
     */
    struct CALUMIANIMATION_API HeaderFlags
    {
        /**
         * @name Constructors
         * @{
         */
        HeaderFlags();
        ~HeaderFlags();
        HeaderFlags(char c0, char c1, char c2, char c3);
        explicit HeaderFlags(char input[4]);
        HeaderFlags(const HeaderFlags& input);
        /// @}

        /**
         * @name Operators
         * @{
         */

        /**
         *
         * @param input
         * @return
         */
        HeaderFlags& operator=(const HeaderFlags& input);
        /// @}

        /**
         * @name Known Data
         * @{
         */

        /**
         * @brief Unknown flag that appears unused
         * @return
         */
        [[nodiscard]] bool firstEntry() const;
        /**
         * @brief Unknown flag that appears unused
         * @param input
         */
        void setFirstEntry(bool input) const;
        /**
         * @brief Counter size flag
         * @return Whether animation block counters are 1 or 2 bytes in size
         */
        [[nodiscard]] bool areKeyCounters2Byte() const;
        /**
         * @brief Counter size flag
         * @param input Whether animation block counters are 1 or 2 bytes in size
         */
        void setKeyCountersFlag(bool input) const;
        /**
         * @brief Key Frame size flag
         * @return Whether animation block key frames are 1 or 2 bytes in size
         */
        [[nodiscard]] bool areKeyFrameEntries2Byte() const;
        /**
         * @brief Key Frame size flag
         * @param input Whether animation block key frames are 1 or 2 bytes in size
         */
        void setKeyFrameEntriesFlag(bool input) const;
        /**
         * @brief Scalars present flag
         * @return Whether scalars are present
         */
        [[nodiscard]] bool hasScalarSequence() const;
        /**
         * @brief Scalars present flag
         * @param input Whether scalars are present
         */
        void setScalarSequenceFlag(bool input) const;
        /**
         * @brief Main byte of flag data
         * @return Byte containing the 3 used flags
         */
        [[nodiscard]] unsigned char data() const;

        /// @}

        /**
         * @name Unknown or Unused
         * @{
         */
        [[nodiscard]] bool u1() const;
        [[nodiscard]] bool u2() const;
        [[nodiscard]] bool u3() const;
        [[nodiscard]] bool u4() const;
        void setU1(bool input) const;
        void setU2(bool input) const;
        void setU3(bool input) const;
        void setU4(bool input) const;

        [[nodiscard]] unsigned char c1() const;
        [[nodiscard]] unsigned char c2() const;
        [[nodiscard]] unsigned char c3() const;
        void setC1(unsigned char input) const;
        void setC2(unsigned char input) const;
        void setC3(unsigned char input) const;

        /// @}

    private:
        struct Impl;
        Impl* pImpl;
    };

    /**
     * @brief Section of data that describes transform sequences
     */
    struct CALUMIANIMATION_API AnimationBlock
    {
        /**
         * @name Constructors
         * @{
         */


        AnimationBlock();
        /**
         *
         * @param input
         */
        AnimationBlock(const AnimationBlock& input);
        /**
         *
         * @param buffer
         * @param addressIndex
         * @param flags
         */
        AnimationBlock(Utilities::BufferObject& buffer, unsigned long long& addressIndex, const HeaderFlags& flags);
        ~AnimationBlock();

        /// @}
        /// @name Operators
        /// @{

        /**
         *
         * @param input
         * @return
         */
        AnimationBlock& operator=(const AnimationBlock& input);

        /// @}
        /**
         * @name Counts
         */

        /**
         * @brief
         * @return The number of rotation entries (suffix)
         */
        [[nodiscard]] uint16_t rotationCount() const;
        /**
         * @brief
         * @return The number of rotation entries (prefix)
         */
        [[nodiscard]] uint16_t rotationPrefixCount() const;
        /**
         * @brief
         * @return The number of translation entries (suffix)
         */
        [[nodiscard]] uint16_t translationCount() const;
        /**
         * @brief
         * @return The number of translation entries (prefix)
         */
        [[nodiscard]] uint16_t translationPrefixCount() const;
        /**
         * @brief
         * @return The number of scalar entries
         */
        [[nodiscard]] uint16_t scalarCount() const;
        /**
         * @brief
         * @return The number of "priority" entries
         */
        [[nodiscard]] uint16_t priorityCount() const;

        /**
         *
         * @param input
         */
        void setRotationCount(uint16_t input) const;
        /**
         *
         * @param input
         */
        void setRotationPrefixCount(uint16_t input) const;
        /**
         *
         * @param input
         */
        void setTranslationCount(uint16_t input) const;
        /**
         *
         * @param input
         */
        void setTranslationPrefixCount(uint16_t input) const;
        /**
         *
         * @param input
         */
        void setScalarCount(uint16_t input) const;
        /**
         *
         * @param input
         */
        void setPriorityCount(uint16_t input) const;

        /// @}

        /**
         * @name Key Frames
         * @{
         */

        /**
         * @brief Vector of Keyframes of size [RotationCount]
         * @return
         */
        [[nodiscard]] Utilities::U16Vector& rotationKeyFrames() const;
        /**
         * @brief Vector of Keyframes of size [TranslationCount]
         * @return
         */
        [[nodiscard]] Utilities::U16Vector& translationKeyFrames() const;
        /**
         * @brief Vector of Keyframes of size [ScalarCount]
         * @return
         */
        [[nodiscard]] Utilities::U16Vector& scalarKeyFrames() const;
        /**
         * @brief Vector of Keyframes of size [PriorityCount]
         * @return
         */
        [[nodiscard]] Utilities::U16Vector& priorityKeyFrames() const;

        /// @}
        /**
        * @name Compressed Entries
        */

        /**
         * @brief "Suffix" of the compressed Rotation entries, one for each frame
         * @return
         */
        [[nodiscard]] RotationEntrySequence& rotationEntries() const;
        /**
         * @brief "Prefix" of the compress Rotation entries, RLE sequence
         * @details The "prefix" entries are folded into an RLE sequence where there is a counter on the bitfield that describes how many frames these entries apply to using the programmatic counting method (0,1,2...)
         * @return
         */
        [[nodiscard]] RotationPrefixSequence& rotationPrefixEntries() const;
        /**
         * @brief "Suffix" of the compressed Translation entries, one for each frame
         * @return
         */
        [[nodiscard]] TranslationEntrySequence& translationEntries() const;
        /**
         * @brief "Prefix" of the compress Translation entries, RLE sequence
         * @details The "prefix" entries are folded into an RLE sequence where there is a counter on the bitfield that describes how many frames these entries apply to using the common counting method (1,2, 3...)
         * @return
         */
        [[nodiscard]] TranslationPrefixSequence& translationPrefixEntries() const;
        /**
         * @brief Scalar entries
         * @details Value described with a signed short (2 Bytes) where 5000 is the base scale of 1.0f
         * @return
         */
        [[nodiscard]] Utilities::S16Vector& scalarEntries() const;
        /**
         * @brief "Priority" entries
         * @details Not much is known about these. It is believed that they are used by additive animations to
         * determine which animation take precedence when transforming this bone
         * @return
         */
        [[nodiscard]] Utilities::U8Vector& priorityEntries() const;

        /// @}
        /// @name Serialization
        /// @{
        /**
         * @brief Method for serializing data into the expected file format
         * @param buffer
         * @param addressIndex
         * @param flags
         */
        void serializeIntoBuffer(Utilities::BufferObject& buffer, unsigned long long& addressIndex, const HeaderFlags& flags) const;
        /// @}
        /// @name Conversion
        /// @{

        /**
         *
         * @param input
         */
        void setRotationSequenceFromUNIV(UNIV::RotationSequence& input) const;
        /**
         *
         * @return
         */
        [[nodiscard]] UNIV::RotationSequence rotationSequenceAsUNIV() const;
        /**
         *
         * @param input
         * @param highPrecision
         * @param lowPrecision
         */
        void setTranslationSequenceFromUNIV(UNIV::TranslationSequence& input, const float& highPrecision, const float& lowPrecision) const;
        /**
         *
         * @param highPrecision
         * @param lowPrecision
         * @return
         */
        [[nodiscard]] UNIV::TranslationSequence translationSequenceAsUNIV(const float& highPrecision, const float& lowPrecision) const;
        /**
         *
         * @param input
         */
        void setScalarSequenceFromUNIV(UNIV::ScalarSequence& input) const;
        /**
         *
         * @return
         */
        [[nodiscard]] UNIV::ScalarSequence scalarSequenceAsUNIV() const;
        /**
         *
         * @param input
         */
        void setPrioritySequenceFromUNIV(UNIV::PrioritySequence& input) const;
        /**
         *
         * @return
         */
        [[nodiscard]] UNIV::PrioritySequence prioritySequenceAsUNIV() const;

    private:
        struct Impl;
        Impl* pImpl;
    };

    VECTORDEC(AnimationBlockVector, AnimationBlock)

    /**
     * @brief Not much is known about this just yet. Nothing that has been proven that is
     */
    struct CALUMIANIMATION_API Preamble
    {
        [[nodiscard]] uint16_t count() const;
        void setCount(uint16_t sz) const;

        [[nodiscard]] Utilities::FloatVector& preSet() const;
        void setPreSet(const Utilities::FloatVector& input) const;
        [[nodiscard]] Utilities::FloatVector& mainSet() const;
        void setMainSet(const Utilities::FloatVector& input) const;
        [[nodiscard]] Utilities::S16Vector& footer1() const;
        void setFooter1(const Utilities::S16Vector& input) const;
        [[nodiscard]] Utilities::S8Vector& footer2() const;
        void setFooter2(const Utilities::S8Vector& input) const;


        Preamble();
        ~Preamble();
        Preamble(Utilities::BufferObject& buffer, unsigned long long& addressIndex, uint64_t frameCount);
        Preamble& operator=(const Preamble& other);
        Preamble(const Preamble& other);

    private:
        struct Impl;
        Impl* pImpl;
    };

    VECTORDEC(PreambleVector, Preamble)

    /**
     * @brief Starfield Animation
     */
    class CALUMIANIMATION_API Animation : IReadWritable
    {
    public:
        /**
         * @brief Index Counting Solution for the Index Atlas
         */
        enum class IndexCountingSolution : uint8_t
        {
            odd = 0, ///< 1,3,5,etc
            even = 1, ///< 0,2,4,etc
            all = 2 ///< all indices
        };

    public:
        /**
         * @name Constructors
         * @{
         */

        Animation();
        /**
         *
         * @param input
         */
        Animation(const Animation& input);
        ~Animation() override;

        /// @}
        /// @name Operators
        /// @{

        /**
         *
         * @param input
         * @return
         */
        Animation& operator=(const Animation& input);

        /// @}
        /// @name Meta
        /// @{
        ///
        [[nodiscard]] uint64_t sourceFileSize() const;

        /**
         *
         * @return
         */
        [[nodiscard]] Utilities::StringContainer& animationFileName() const;
        /**
         *
         * @param input
         */
        void setAnimationFileName(const Utilities::StringContainer& input) const;

        /// @}
        /// @name Header Data
        /// @{

        /**
         * @return
         */
        [[nodiscard]] uint64_t magicNumber() const;
        /**
         *
         * @param input
         */
        void setMagicNumber(uint64_t input) const;

        /**
         *
         * @return
         */
        [[nodiscard]] Math::Quaternion& headerRotation() const;
        /**
         *
         * @param input
         */
        void setHeaderRotation(const Math::Quaternion& input) const;

        /**
         *
         * @return
         */
        [[nodiscard]] Math::Vector3& headerTranslation() const;
        /**
         *
         * @param input
         */
        void setHeaderTranslation(const Math::Vector3& input) const;

        /**
         *
         * @return
         */
        [[nodiscard]] HeaderFlags& headerFlags() const;
        /**
         *
         * @param input
         */
        void setHeaderFlags(const HeaderFlags& input) const;
        /**
         * @brief Will assign proper flag values based on the current Animation data
         */
        void evaluateHeaderFlags() const;

        /**
         *
         * @return
         */
        [[nodiscard]] short versionNumber() const;
        /**
         *
         * @param v
         */
        void setVersionNumber(short v) const;


        /**
         *
         * @return
         */
        [[nodiscard]] uint16_t boneCount() const;
        /**
         *
         * @param input
         */
        void setBoneCount(uint16_t input) const;
        /**
         *
         * @return
         */
        [[nodiscard]] uint16_t frameCount() const;
        /**
         *
         * @param input
         */
        void setFrameCount(uint16_t input) const;
        /**
         *
         * @return
         */
        [[nodiscard]] uint16_t indexAtlasCount() const;
        /**
         *
         * @param input
         */
        void setIndexAtlasCount(uint16_t input) const;

        /**
         * @brief Count of Non-Rig based animation blocks
         * @return
         */
        [[nodiscard]] uint16_t amendedBlockCount() const;
        /**
         *
         * @param input
         */
        void setAmendedBlockCount(uint16_t input) const;
        /**
         *
         * @return
         */
        [[nodiscard]] uint16_t preambleOffset() const;
        /**
         *
         * @param input
         */
        void setPreambleOffset(uint16_t input) const;

        /**
         *
         * @return
         */
        [[nodiscard]] Utilities::FloatVector nZeroFloats() const;
        /**
         *
         * @param input
         */
        void setNZeroFloats(const float input[3]) const;

        /**
         * @brief Hash values to apply the amended animation blocks. The original string is lost during export and must be retraced
         * @return
         */
        [[nodiscard]] Utilities::U32Vector& amendedHashSet() const;
        /**
         *
         * @param input
         */
        void setAmendedHashSet(const Utilities::U32Vector& input) const;

        /**
         *
         * @return
         */
        [[nodiscard]] uint32_t preambleCount() const;
        /**
         *
         * @param input
         */
        void setPreambleCount(uint32_t input) const;
        /**
         *
         * @return
         */
        [[nodiscard]] PreambleVector& preamble() const;
        /**
         *
         * @param input
         */
        void setPreamble(const PreambleVector& input) const;


        /// @}
        /// @name Animation Block Entries
        /// @{

        /**
         * @brief Index Atlas describes how the following data is assigned
         * @details The atlas consists of an array of unsigned values, the first value (and following odd index values) being the amount of bones to skip during processing
         * and the second value (and following even index values) being the amount of bones to apply the data to. \n
         * For example an array {1, 9, 11, 1} will skip the SkeletonRig's root bone, apply the first 11 animation blocks to bone indices 1-9, skip bone indices 10-20,
         * and then finally apply the final blocks to the bone at index 21
         * @return
         */
        [[nodiscard]] Utilities::U16Vector& indexAtlas() const;
        /**
         * @brief
         * @param input An evenly sized array of unsigned values
         */
        void setIndexAtlast(const Utilities::U16Vector& input) const;

        /**
         * @brief The array of animation blocks to apply to the given Starfield skeleton rig
         * @return
         */
        [[nodiscard]] AnimationBlockVector& animationBlocks() const;
        /**
         *
         * @param input
         */
        void setAnimationBlocks(const AnimationBlockVector& input) const;
        /**
         * @brief The array of animation blocks that are applied by hash value in game, to some form of AnimObject
         * @return
         */
        [[nodiscard]] AnimationBlockVector& amendedAnimationBlocks() const;
        /**
         *
         * @param input
         */
        void setAmendedAnimationBlocks(const AnimationBlockVector& input) const;

        /// @}

    public:
        /// @name IReadWritable
        /// @{

        Utilities::FileResult readFromFile(Utilities::PathContainer& inputFilePath) override;
        Utilities::FileResult readFromFile(Utilities::PathContainer&& inputFilePath) override;
        Utilities::FileResult writeToFile(Utilities::PathContainer& outputFilePath) override;
        Utilities::FileResult writeToFile(Utilities::PathContainer&& outputFilePath) override;

        /// @}

    public:
        /// @name Validation
            /// @{

        /**
         * @brief Confirms the sizes of various amended block data to be equal
         * @return
         */
        [[nodiscard]] bool verifyAmendedBlocks() const;

        /// @}

    private:
        struct Impl;
        Impl* pImpl;

    private:
        friend class AnimationScene;
    };
}
