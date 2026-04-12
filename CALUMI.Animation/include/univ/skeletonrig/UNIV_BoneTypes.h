//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Utilities.h"

namespace CALUMI::UNIV {

	enum class BoneType : uint32_t
	{
		Default = 0,
		Twist = 1,
		UNDEFINED = 0xFFFFFFFF
	};

    inline static constexpr auto DefaultBoneTypeStr = "Default";
    inline static constexpr auto TwistBoneTypeStr = "Twist";


    /**
     * @brief Global function to assist in string to enum conversion
     * @param boneTypeStr c string of the type, likely #DefaultBoneTypeStr or #TwistBoneTypeStr
     * @return Will return BoneType::UNDEFINED if the bonetypeStr is not found
     */
    CALUMIANIMATION_API BoneType BoneTypeFromString(const char* boneTypeStr);

    /**
     * @brief Abstract for Bone specific, universally defined properties. Behaves similarly to a strategy pattern in concept
     */
    struct CALUMIANIMATION_API BoneTypeProperties
	{
        [[nodiscard]] virtual BoneType getType() const = 0;

        [[nodiscard]] virtual const char* getTypeString() const = 0;
		BoneTypeProperties() = default;
		virtual ~BoneTypeProperties() = default;
	};

    /**
     * @brief Default Bone Type, has no data assigned to it
     */
    struct CALUMIANIMATION_API DefaultBoneProperties : BoneTypeProperties
	{
		DefaultBoneProperties() = default;
		~DefaultBoneProperties() override = default;
        [[nodiscard]] const char* getTypeString() const override;

		// Inherited via BoneTypeProperties
        [[nodiscard]] BoneType getType() const override;
	};

	/**
	 * @brief Basic Twist Type, defines how the bone will behave at runtime
	 */
	struct CALUMIANIMATION_API TwistBoneProperties : BoneTypeProperties
	{
		/// @name Constructors
		/// @{
		TwistBoneProperties();
        explicit TwistBoneProperties(const TwistBoneProperties& input);
		~TwistBoneProperties() override;
		/// @}
		/// @name Twist Data
		/// @{
		/**
		 * @return Name of the bone driving this one
		 */
		[[nodiscard]] const char* twistDriver() const;
        void setTwistDriver(const char* boneName) const;
		/**
		 *
		 * @return Weight of this bone's twist influence
		 */
		[[nodiscard]] float twistDriverWeight() const;
        void setTwistDriverWeight(float wgt) const;
		/// @}
		/// @name Inherited
		/// @{
        [[nodiscard]] const char* getTypeString() const override;

        [[nodiscard]] BoneType getType() const override;
		/// @}
		/// @name Operators
		/// @{
		TwistBoneProperties& operator=(const TwistBoneProperties& input);
		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};

}