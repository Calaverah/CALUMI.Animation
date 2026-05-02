//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Json.h"
#include "utilities/CALUMI_Utilities.h"

namespace CALUMI::UNIV {

    inline static constexpr auto DefaultBoneTypeStr = "Default";
    inline static constexpr auto TwistBoneTypeStr = "Twist";

    /**
     * @brief Abstract for Bone specific, universally defined properties. Behaves similarly to a strategy pattern in concept
     */
    struct CALUMIANIMATION_API BoneTypeProperty
	{
		/**
		 * @brief Bone Types Available
		 */
		enum class BoneType : uint32_t
    	{
    		Default = 0, ///< Default behavior at runtime
			Twist = 1, ///< Motion is determined during runtime instead of by animation directly

			UNDEFINED = 0xFFFFFFFF
		};

    	static constexpr auto BoneTypeMax = static_cast<uint32_t>(BoneType::Twist);

        [[nodiscard]] virtual BoneType type() const = 0;

        [[nodiscard]] virtual const char* typeAsString() const = 0;
		BoneTypeProperty() = default;
		virtual ~BoneTypeProperty() = default;

    	virtual Utilities::JsonObject toJson() const = 0;

    public:
    	/**
		 * @param boneTypeStr c string of the type, likely #DefaultBoneTypeStr or #TwistBoneTypeStr
		 * @return Will return BoneType::UNDEFINED if the bonetypeStr is not found
		 */
    	static BoneType BoneTypeFromString(const char* boneTypeStr);

	    /**
	     * @param typeAsInteger
	     * @return Safe enum conversion from integer
	     */
	    static BoneType GetBoneType(uint32_t typeAsInteger);
	};

    /**
     * @brief Default Bone Type, has no data assigned to it
     */
    struct CALUMIANIMATION_API DefaultBoneProperty : BoneTypeProperty
	{
    	/// @name Initialization
    	/// @{
		DefaultBoneProperty() = default;
		~DefaultBoneProperty() override = default;
    	/// @}
    	/// @name Inherited
    	/// @{
        [[nodiscard]] const char* typeAsString() const override;

		// Inherited via BoneTypeProperties
        [[nodiscard]] BoneType type() const override;

    	/// @}
    	/// @name Json
    	/// @{
    	Utilities::JsonObject toJson() const override;
    	/// @}
	};

	/**
	 * @brief Basic Twist Type, defines how the bone will behave at runtime
	 */
	struct CALUMIANIMATION_API TwistBoneProperty : BoneTypeProperty
	{
		/// @name Initialization
		/// @{
		TwistBoneProperty();
        explicit TwistBoneProperty(const TwistBoneProperty& input);
		~TwistBoneProperty() override;
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
        [[nodiscard]] const char* typeAsString() const override;

        [[nodiscard]] BoneType type() const override;
		/// @}
		/// @name Operators
		/// @{
		TwistBoneProperty& operator=(const TwistBoneProperty& input);
		/// @}
		/// @name Json
		/// @{
		Utilities::JsonObject toJson() const override;
		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};

}