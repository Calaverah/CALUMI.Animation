//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once
#include "utilities/CALUMI_Common.h"
#include "utilities/CALUMI_Utilities.h"

	/**
	 * @defgroup macros Useful Macros
	 * @{
	 * @name Math
	 * @{
	 */
	#define CLA_E        2.71828182845904523536   ///< e constant
	#define CLA_LOG2E    1.44269504088896340736   ///< log2(e)
	#define CLA_LOG10E   0.434294481903251827651  ///< log10(e)
	#define CLA_LN2      0.693147180559945309417  ///< ln(2)
	#define CLA_LN10     2.30258509299404568402   ///< ln(10)
	#define CLA_PI       3.14159265358979323846   ///< pi
	#define CLA_PI_2     1.57079632679489661923   ///< pi/2
	#define CLA_PI_4     0.785398163397448309616  ///< pi/4
	#define CLA_1_PI     0.318309886183790671538  ///< 1/pi
	#define CLA_2_PI     0.636619772367581343076  ///< 2/pi
	#define CLA_2_SQRTPI 1.12837916709551257390   ///< 2/sqrt(pi)
	#define CLA_SQRT2    1.41421356237309504880   ///< sqrt(2)
	#define CLA_SQRT1_2  0.707106781186547524401  ///< 1/sqrt(2)
	/// @}
	/// @}

namespace CALUMI::Math
{
	/**
	 * @brief Converts degrees to radians
	 * @param degrees
	 * @return radians
	 */
	CALUMIANIMATION_API double ToRadians(double degrees) noexcept;
	/**
	 * @brief Converts radians to degrees
	 * @param radians
	 * @return degrees
	 */
	CALUMIANIMATION_API double ToDegrees(double radians) noexcept;

	/**
	 * @brief Vector2 (float)
	 */
	struct CALUMIANIMATION_API Vector2
	{
		/// @name Constructors
		/// @{
		Vector2();
		~Vector2();
		explicit Vector2(float input[2]);
		explicit Vector2(float input);
		Vector2(float x, float y);
		Vector2(const Vector2& input);
		/// @}
		/// @name Data
		/// @{

		[[nodiscard]] float x() const;
		void setX(float x);
		[[nodiscard]] float y() const;
		void setY(float y);

		/// @}

		/// @name Operators
		/// @{

		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;

		Vector2& operator=(const Vector2& other);

		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		Vector2& operator*=(const Vector2& other);
		Vector2& operator*=(float other);
		Vector2& operator/=(float other);

		Vector2 operator+() const;
		Vector2 operator-() const;

		/// @}
		/// @name Vector Operations
		/// @{

		[[nodiscard]] float length() const;
		[[nodiscard]] float lengthSquared() const;

		[[nodiscard]] bool areEqual(const Vector2& input, float tolerance) const noexcept;

		[[nodiscard]] float dot(const Vector2& other) const;

		void cross(const Vector2& other, Vector2& result) const;
		[[nodiscard]] Vector2 cross(const Vector2& other) const;

		void normalize();

		/// @}

		static const Vector2 Zero;
		static const Vector2 One;
		static const Vector2 UnitX;
		static const Vector2 UnitY;
		static const Vector2 Up;
		static const Vector2 Down;
		static const Vector2 Right;
		static const Vector2 Left;


		/// @name Print
		/// @{

		[[nodiscard]] Utilities::StringContainer toString() const;

		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};

	/** @relates Vector2 */
	CALUMIANIMATION_API Vector2 operator+ (const Vector2& A, const Vector2& B) noexcept;
	/** @relates Vector2 */
	CALUMIANIMATION_API Vector2 operator- (const Vector2& A, const Vector2& B) noexcept;
	/** @relates Vector2 */
	CALUMIANIMATION_API Vector2 operator* (const Vector2& A, const Vector2& B) noexcept;
	/** @relates Vector2 */
	CALUMIANIMATION_API Vector2 operator* (const Vector2& A, float B) noexcept;
	/** @relates Vector2 */
	CALUMIANIMATION_API Vector2 operator* (float A, const Vector2& B) noexcept;
	/** @relates Vector2 */
	CALUMIANIMATION_API Vector2 operator/ (const Vector2& A, const Vector2& B) noexcept;
	/** @relates Vector2 */
	CALUMIANIMATION_API Vector2 operator/ (const Vector2& A, float B) noexcept;
	/** @relates Vector2 */
	CALUMIANIMATION_API Vector2 operator/ (float A, const Vector2& B) noexcept;


	struct CALUMIANIMATION_API Vector2D
	{
		/// @name Constructors
		/// @{

		Vector2D();
		~Vector2D();
		explicit Vector2D(double input[2]);
		explicit Vector2D(double input);
		explicit Vector2D(Vector2 input);
		Vector2D(const Vector2D& input);
		Vector2D(double x, double y);

		/// @}
		/// @name Data
		/// @{

		[[nodiscard]] double x() const;
		void setX(double x);
		[[nodiscard]] double y() const;
		void setY(double y);

		/// @}
		/// @name Operators
		/// @{

		bool operator==(const Vector2D& other) const;
		bool operator!=(const Vector2D& other) const;

		Vector2D& operator=(const Vector2D& other);

		Vector2D& operator+=(const Vector2D& other);
		Vector2D& operator-=(const Vector2D& other);
		Vector2D& operator*=(const Vector2D& other);
		Vector2D& operator*=(double other);
		Vector2D& operator/=(double other);

		Vector2D operator+() const;
		Vector2D operator-() const;

		/// @}
		/// @name Vector Operations
		/// @{

		[[nodiscard]] double length() const;
		[[nodiscard]] double lengthSquared() const;

		[[nodiscard]] bool areEqual(const Vector2D& input, float tolerance) const noexcept;

		[[nodiscard]] double dot(const Vector2D& other) const;
		void cross(const Vector2D& other, Vector2D& result) const;
		[[nodiscard]] Vector2D cross(const Vector2D& other) const;

		void normalize();

		[[nodiscard]] Vector2D lerp(const Vector2D& input, double t = 0.5) const;

		/// @}

		static const Vector2D Zero;
		static const Vector2D One;
		static const Vector2D UnitX;
		static const Vector2D UnitY;
		static const Vector2D Up;
		static const Vector2D Down;
		static const Vector2D Right;
		static const Vector2D Left;

		/// @name Print
		/// @{
		[[nodiscard]] Utilities::StringContainer toString() const;
		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};

	/** @relates Vector2D */
	CALUMIANIMATION_API Vector2D operator+ (const Vector2D& A, const Vector2D& B) noexcept;
	/** @relates Vector2D */
	CALUMIANIMATION_API Vector2D operator- (const Vector2D& A, const Vector2D& B) noexcept;
	/** @relates Vector2D */
	CALUMIANIMATION_API Vector2D operator* (const Vector2D& A, const Vector2D& B) noexcept;
	/** @relates Vector2D */
	CALUMIANIMATION_API Vector2D operator* (const Vector2D& A, double B) noexcept;
	/** @relates Vector2D */
	CALUMIANIMATION_API Vector2D operator* (double A, const Vector2D& B) noexcept;
	/** @relates Vector2D */
	CALUMIANIMATION_API Vector2D operator/ (const Vector2D& A, const Vector2D& B) noexcept;
	/** @relates Vector2D */
	CALUMIANIMATION_API Vector2D operator/ (const Vector2D& A, double B) noexcept;
	/** @relates Vector2D */
	CALUMIANIMATION_API Vector2D operator/ (double A, const Vector2D& B) noexcept;

	struct CALUMIANIMATION_API Vector3
	{
		/** @name Constructors */
		/// @{

		Vector3();
		~Vector3();
		explicit Vector3(float input[3]);
		explicit Vector3(double input[3]);
		explicit Vector3(float input);
		explicit Vector3(double input);
		Vector3(float x, float y, float z);
		Vector3(double x, double y, double z);
		Vector3(const Vector3& source);

		/// @}
		/** @name Data */
		/// @{

		[[nodiscard]] float x() const;
		void setX(float x) const;
		[[nodiscard]] float y() const;
		void setY(float y) const;
		[[nodiscard]] float z() const;
		void setZ(float z) const;

		/// @}
		/** @name Operators */
		/// @{

		bool operator==(const Vector3& other) const;
		bool operator!=(const Vector3& other) const;

		Vector3& operator=(const Vector3& other);

		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		Vector3& operator*=(const Vector3& other);
		Vector3& operator*=(float other);
		Vector3& operator/=(float other);

		Vector3 operator+() const;
		Vector3 operator-() const;

		/// @}
		/** @name Vector Operations */
		/// @{

		[[nodiscard]] float length() const;
		[[nodiscard]] float lengthSquared() const;

		[[nodiscard]] bool areEqual(const Vector3& input, float tolerance) const noexcept;

		[[nodiscard]] float dot(const Vector3& other) const;

		void cross(const Vector3& other, Vector3& result) const;
		[[nodiscard]] Vector3 cross(const Vector3& other) const;

		void normalize();

		/// @}

		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 UnitX;
		static const Vector3 UnitY;
		static const Vector3 UnitZ;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Right;
		static const Vector3 Left;
		static const Vector3 Forward;
		static const Vector3 Back;

		/** @name Print */
		/// @{

		[[nodiscard]] Utilities::StringContainer toString() const;

		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};

	/** @relates Vector3 */
	CALUMIANIMATION_API Vector3 operator+ (const Vector3& A, const Vector3& B) noexcept;
	/** @relates Vector3 */
	CALUMIANIMATION_API Vector3 operator- (const Vector3& A, const Vector3& B) noexcept;
	/** @relates Vector3 */
	CALUMIANIMATION_API Vector3 operator* (const Vector3& A, const Vector3& B) noexcept;
	/** @relates Vector3 */
	CALUMIANIMATION_API Vector3 operator* (const Vector3& A, float B) noexcept;
	/** @relates Vector3 */
	CALUMIANIMATION_API Vector3 operator* (float A, const Vector3& B) noexcept;
	/** @relates Vector3 */
	CALUMIANIMATION_API Vector3 operator/ (const Vector3& A, const Vector3& B) noexcept;
	/** @relates Vector3 */
	CALUMIANIMATION_API Vector3 operator/ (const Vector3& A, float B) noexcept;
	/** @relates Vector3 */
	CALUMIANIMATION_API Vector3 operator/ (float A, const Vector3& B) noexcept;

	struct CALUMIANIMATION_API Vector3D
	{
		/** @name Constructors */
		/// @{

		Vector3D();
		~Vector3D();
		explicit Vector3D(double input[3]);
		explicit Vector3D(double input);
		explicit Vector3D(Vector3 input);
		Vector3D(const Vector3D& input);
		Vector3D(double x, double y, double z);

		/// @}
		/** @name Data */
		/// @{

		[[nodiscard]] double x() const;
		void setX(double x);
		[[nodiscard]] double y() const;
		void setY(double y);
		[[nodiscard]] double z() const;
		void setZ(double z);

		/// @}
		/** @name Operators */
		/// @{

		bool operator==(const Vector3D& other) const;
		bool operator!=(const Vector3D& other) const;

		Vector3D& operator=(const Vector3D& other);

		Vector3D& operator+=(const Vector3D& other);
		Vector3D& operator-=(const Vector3D& other);
		Vector3D& operator*=(const Vector3D& other);
		Vector3D& operator*=(double other);
		Vector3D& operator/=(double other);

		Vector3D operator+() const;
		Vector3D operator-() const;

		/// @}
		/** @name Vector Operations */
		/// @{

		[[nodiscard]] double length() const;
		[[nodiscard]] double lengthSquared() const;

		[[nodiscard]] bool areEqual(const Vector3D& input, float tolerance = 0.0) const noexcept;

		[[nodiscard]] double dot(const Vector3D& other) const;
		void cross(const Vector3D& other, Vector3D& result) const;
		[[nodiscard]] Vector3D cross(const Vector3D& other) const;

		void normalize();

		[[nodiscard]] Vector3D lerp(const Vector3D& input, double t = 0.5) const;

		/// @}

		static const Vector3D Zero;
		static const Vector3D One;
		static const Vector3D UnitX;
		static const Vector3D UnitY;
		static const Vector3D UnitZ;
		static const Vector3D Up;
		static const Vector3D Down;
		static const Vector3D Right;
		static const Vector3D Left;
		static const Vector3D Forward;
		static const Vector3D Back;

		/** @name Print */
		/// @{

		[[nodiscard]] Utilities::StringContainer toString() const;

		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};


	/** @relates Vector3D */
	CALUMIANIMATION_API Vector3D operator+ (const Vector3D& A, const Vector3D& B) noexcept;
	/** @relates Vector3D */
	CALUMIANIMATION_API Vector3D operator- (const Vector3D& A, const Vector3D& B) noexcept;
	/** @relates Vector3D */
	CALUMIANIMATION_API Vector3D operator* (const Vector3D& A, const Vector3D& B) noexcept;
	/** @relates Vector3D */
	CALUMIANIMATION_API Vector3D operator* (const Vector3D& A, double B) noexcept;
	/** @relates Vector3D */
	CALUMIANIMATION_API Vector3D operator* (double A, const Vector3D& B) noexcept;
	/** @relates Vector3D */
	CALUMIANIMATION_API Vector3D operator/ (const Vector3D& A, const Vector3D& B) noexcept;
	/** @relates Vector3D */
	CALUMIANIMATION_API Vector3D operator/ (const Vector3D& A, double B) noexcept;
	/** @relates Vector3D */
	CALUMIANIMATION_API Vector3D operator/ (double A, const Vector3D& B) noexcept;

	/**
	 * @brief Convenient way to describe euler and Tait-Bryan operations
	 */
	struct CALUMIANIMATION_API EulerDefinition
	{
		/**
		 * @brief Order of operations for converting euler (rx,ry,rz) to quaternion
		 */
		enum class EulerOrder : uint8_t
		{
			XYZ =  0,
			XZY =  1,
			YXZ =  2,
			YZX =  3,
			ZXY =  4,
			ZYX =  5,
			XYX =  6,
			XZX =  7,
			YXY =  8,
			YZY =  9,
			ZXZ = 10,
			ZYZ = 11,

			RPY = XYZ,
			YPR = ZYX
		};

		static constexpr auto DefaultEulerOrder = EulerOrder::XYZ;
		static constexpr auto MaxEulerOrder = static_cast<uint8_t>(EulerOrder::ZYZ);

	public:
		/**
		 * @param first
		 * @param second
		 * @param third
		 * @param euler
		 */
		EulerDefinition(float first = 0.0f,
						float second = 0.0f,
						float third = 0.0f,
						EulerOrder euler = DefaultEulerOrder);

		EulerDefinition(const EulerDefinition& other);

		~EulerDefinition();

		EulerDefinition& operator=(const EulerDefinition& other);

		/**
		 * @return first of the operations, or alpha
		 */
		[[nodiscard]] float first() const;
		/**
		 * @return second of the operations, or beta
		 */
		[[nodiscard]] float second() const;
		/**
		 * @param safe Whether to consider singularity safety from the beta value,
		 * @return third of the operations, or gamma. If safety is on, will return 0.0f in the case that there is a
		 * singularity
		 */
		[[nodiscard]] float third(bool safe = true) const;
		/**
		 * @return order of the operations
		 */
		[[nodiscard]] EulerOrder order() const;
		/**
		 * @param first
		 */
		void setFirst(float first);
		/**
		 * @param second
		 */
		void setSecond(float second);
		/**
		 * @param third
		 */
		void setThird(float third);
		/**
		 * @param order
		 */
		void setOrder(EulerOrder order);

	public:
		/**
		 * @return Euler order of this order definition in the form of an int for convenient handling in extern "C"
		 */
		[[nodiscard]] static int ToInt(EulerOrder order);
		/**
		 *
		 * @param order
		 * @return EulerOrder from the given int, will default to EulerOrder::Default if not defined
		 */
		static EulerOrder GetEulerOrder(int order);

	private:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 * @brief Imaginary way to mathematically describe a rotation without risk of gimble lock
	 */
	struct CALUMIANIMATION_API Quaternion
	{


		/** @name Constructors */
		/// @{

		Quaternion();
		~Quaternion();
		explicit Quaternion(float input[4], bool normalized = true);
		Quaternion(Vector3 direction, double radians, bool normalized = true);
		Quaternion(Vector3D direction, double radians, bool normalized = true);
		Quaternion(double x, double y, double z, double w, bool normalized = true);
		Quaternion(float x, float y, float z, float w, bool normalized = true);
		Quaternion(EulerDefinition eulerInput);
		Quaternion(float x, float y, float z, EulerDefinition::EulerOrder order);
		Quaternion(const Quaternion& input);

		/// @}
		/** @name Data */
		/// @{

		[[nodiscard]] float x() const;
		void setX(float x);
		[[nodiscard]] float y() const;
		void setY(float y);
		[[nodiscard]] float z() const;
		void setZ(float z);
		[[nodiscard]] float w() const;
		void setW(float w);

		/// @}
		/** @name Operators */
		/// @{

		bool operator == (const Quaternion& input) const noexcept;
		bool operator != (const Quaternion& input) const noexcept;


		Quaternion& operator+=(const Quaternion& other) noexcept;
		Quaternion& operator-=(const Quaternion& other) noexcept;
		Quaternion& operator*=(const Quaternion& other) noexcept;
		Quaternion& operator*=(float other) noexcept;
		Quaternion& operator/=(const Quaternion& other) noexcept;

		Quaternion& operator=(const Quaternion& other) noexcept;

		Quaternion operator+() const noexcept;
		Quaternion operator-() const noexcept;

		/// @}
		/** @name Equality */
		/// @{

		[[nodiscard]] bool areSameRotation(const Quaternion& input, float tolerance = 0.0) const noexcept;
		[[nodiscard]] bool areEqual(const Quaternion& input, float tolerance = 0.0) const noexcept;

		/// @}
		/** @name Vector Operations */
		/// @{

		Quaternion conjugate() noexcept;
		void conjugate(Quaternion& result) const noexcept;

		[[nodiscard]] float dot(const Quaternion& input) const noexcept;

		[[nodiscard]] float lengthSquared() const noexcept;
		[[nodiscard]] float length() const noexcept;

		void normalize() noexcept;
		void normalize(Quaternion& result) const noexcept;

		void inverse(Quaternion& result) const noexcept;
		void inverse() const noexcept;

		//Returns angle in radians
		[[nodiscard]] float angularDistance(const Quaternion& input) const;

		/**
		 * @param start
		 * @param end
		 * @return The offset rotation needed for "start" to reach "end"
		 */
		static Quaternion rotationOffset(const Quaternion& start, const Quaternion& end);
		/**
		 * @param offset The amount to rotate this quaternion
		 */
		void rotateBy(const Quaternion& offset);

		[[nodiscard]] Quaternion sLerp(const Quaternion& input, float t = 0.5f) const;

		[[nodiscard]] EulerDefinition toEuler(EulerDefinition::EulerOrder order) const;

		static const Quaternion Identity;

		/// @}
		/** @name Print */
		/// @{

		[[nodiscard]] Utilities::StringContainer toString() const;

		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};

	/** @relates Quaternion */
	CALUMIANIMATION_API Quaternion operator+ (const Quaternion& A, const Quaternion& B) noexcept;
	/** @relates Quaternion */
	CALUMIANIMATION_API Quaternion operator- (const Quaternion& A, const Quaternion& B) noexcept;
	/** @relates Quaternion */
	CALUMIANIMATION_API Quaternion operator* (const Quaternion& A, const Quaternion& B) noexcept;
	/** @relates Quaternion */
	CALUMIANIMATION_API Quaternion operator* (const Quaternion& A, float B) noexcept;
	/** @relates Quaternion */
	CALUMIANIMATION_API Quaternion operator* (float A, const Quaternion& B) noexcept;
	/** @relates Quaternion */
	CALUMIANIMATION_API Quaternion operator/ (const Quaternion& A, const Quaternion& B) noexcept;

	struct CALUMIANIMATION_API Transform
	{
		Transform(Vector3 position = Vector3(), Quaternion rotation = Quaternion());
		Transform(const Transform& other);
		~Transform();

		Transform& operator=(const Transform& other);
		/**
		 *
		 * @param reference The origin transform to offset this transform by
		 * @return This transform in global coordinates
		 */
		[[nodiscard]] Transform global(const Transform& reference) const;
		/**
		 *
		 * @param reference The origin transform to find the offset from
		 * @return This transform relative to the given transform
		 */
		[[nodiscard]] Transform local(const Transform& reference) const;
		/**
		 * @return rotation of this transform
		 */
		[[nodiscard]] Quaternion rotation() const;
		/**
		 * @brief Sets the rotation
		 * @param rotation
		 */
		void setRotation(const Quaternion& rotation);
		/**
		 * @brief Sets the rotation
		 * @param rotation
		 */
		void setRotation(Quaternion&& rotation);
		/**
		 * @return position of this transform
		 */
		[[nodiscard]] Vector3 position() const;
		/**
		 * @brief Sets the position
		 * @param position
		 */
		void setPosition(const Vector3& position);
		/**
		 * @brief Sets the position
		 * @param position
		 */
		void setPosition(Vector3&& position);

	private:
		struct Impl;
		Impl* pImpl;
	};

}

/// @addtogroup extern_c
/// @{
/// @defgroup extern_c_math Math
/// @{
extern "C" {
/**
 * @param x
 * @param y
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteVector2C
 * @return Heap allocated Vector2 <float> of the given values
 */
CALUMIANIMATION_API CALUMI::Math::Vector2* CreateVector2C(float x, float y);
/**
 * @brief
 */
CALUMIANIMATION_API float GetVector2XC(CALUMI::Math::Vector2* source);
/**
 *  @brief
 */
CALUMIANIMATION_API float GetVector2YC(CALUMI::Math::Vector2* source);
/**
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Deletion
 */
CALUMIANIMATION_API int DeleteVector2C(CALUMI::Math::Vector2* ptr);

/**
 * @param x
 * @param y
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteVector2DC
 * @return Heap allocated vector2D <double> of the given values
 */
CALUMIANIMATION_API CALUMI::Math::Vector2D* CreateVector2DC(float x, float y);
/**
 * @brief
 */
CALUMIANIMATION_API double GetVector2DXC(CALUMI::Math::Vector2D* source);
/**
 * @brief
 */
CALUMIANIMATION_API double GetVector2DYC(CALUMI::Math::Vector2D* source);
/**
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Deletion
 */
CALUMIANIMATION_API int DeleteVector2DC(CALUMI::Math::Vector2D* ptr);

/**
 * @param x
 * @param y
 * @param z
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteVector3C
 * @return Heap allocated vector3 <float> of the given values
 */
CALUMIANIMATION_API CALUMI::Math::Vector3* CreateVector3C(float x, float y, float z);
/**
 *  @brief
 */
CALUMIANIMATION_API float GetVector3XC(CALUMI::Math::Vector3* source);
/**
 *  @brief
 */
CALUMIANIMATION_API float GetVector3YC(CALUMI::Math::Vector3* source);
/**
 *  @brief
 */
CALUMIANIMATION_API float GetVector3ZC(CALUMI::Math::Vector3* source);
/**
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Deletion
 */
CALUMIANIMATION_API int DeleteVector3C(CALUMI::Math::Vector3* ptr);

/**
 * @param x
 * @param y
 * @param z
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteVector3DC
 * @return Heap allocated vector3D <double> of the given values
 */
CALUMIANIMATION_API CALUMI::Math::Vector3D* CreateVector3DC(double x, double y, double z);
/**
 * @brief
 */
CALUMIANIMATION_API double GetVector3DXC(CALUMI::Math::Vector3D* source);
/**
 * @brief
 */
CALUMIANIMATION_API double GetVector3DYC(CALUMI::Math::Vector3D* source);
/**
 * @brief
 */
CALUMIANIMATION_API double GetVector3DZC(CALUMI::Math::Vector3D* source);
/**
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Deletion
 */
CALUMIANIMATION_API int DeleteVector3DC(CALUMI::Math::Vector3D* ptr);

/**
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteTransformC
 * @return Heap allocated transform at Position (0,0,0) and Rotation xyz = 0, w = 1
 */
CALUMIANIMATION_API CALUMI::Math::Transform* CreateTransformC();
/**
 * @param transform
 * @param reference Ptr to transform in which values will be copied
 * @return Error Code: \n -1 = Invalid Ptr\n 0 = Successful Copy
 */
CALUMIANIMATION_API int SetTransformFromReferenceC(CALUMI::Math::Transform* transform, CALUMI::Math::Transform* reference);
/**
 * @param global Transform to measure in a coordinate system
 * @param reference Reference Transform in the same coordinate system as the input transform
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteTransformC
 * @return Heap allocated transform representing the global input relative to the reference input
 */
CALUMIANIMATION_API CALUMI::Math::Transform* GetLocalTransformC(CALUMI::Math::Transform* global, CALUMI::Math::Transform* reference);
/**
 * @param local Transform to measure in a new coordinate system
 * @param reference Reference Transform in the same coordinate system as the output transform
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteTransformC
 * @return Heap allocated transform representing the local input in the same coordinate system as the reference
 */
CALUMIANIMATION_API CALUMI::Math::Transform* GetGlobalTransformC(CALUMI::Math::Transform* local, CALUMI::Math::Transform* reference);
/**
 * @param transform
 * @param x
 * @param y
 * @param z
 * @return Error code:\n -1 = Invalid Ptr\n 0 = Successful Operation
 */
CALUMIANIMATION_API int SetTransformPositionC(CALUMI::Math::Transform* transform, float x, float y, float z);
/**
 * @param transform
 * @param reference
 * @return Error code:\n -1 = Invalid Ptr\n 0 = Successful Operation
 */
CALUMIANIMATION_API int SetTransformPositionFromReferenceC(CALUMI::Math::Transform* transform, CALUMI::Math::Vector3* reference);
/**
 * @param transform
 * @param x
 * @param y
 * @param z
 * @param w
 * @return Error code:\n -1 = Invalid Ptr\n 0 = Successful Operation
 */
CALUMIANIMATION_API int SetTransformRotationC(CALUMI::Math::Transform* transform, float x, float y, float z, float w);
/**
 * @param transform
 * @param x
 * @param y
 * @param z
 * @param eulerOrder Int value can be found via EulerOrder
 * @return Error code:\n -1 = Invalid Ptr\n 0 = Successful Operation
 */
CALUMIANIMATION_API int SetTransformEulerRotationC(CALUMI::Math::Transform* transform, float x, float y, float z, int eulerOrder);
/**
 * @param transform
 * @param reference
 * @return Error code:\n -1 = Invalid Ptr\n 0 = Successful Operation
 */
CALUMIANIMATION_API int SetTransformRotationFromReferenceC(CALUMI::Math::Transform* transform, CALUMI::Math::Quaternion* reference);
/**
 * @param transform
 * @param first Ptr to float to receive the first operation
 * @param second Ptr to float to receive the second operation
 * @param third Ptr to float to receive the third operation
 * @param eulerOrder Int representation of a EulerDefinition::EulerOrder
 * @return Error code:\n -1 = Invalid Ptr\n 0 = Successful Operation
 */
CALUMIANIMATION_API int GetTransformEulerRotationC(CALUMI::Math::Transform* transform,
												   float* first,
												   float* second,
												   float* third,
												   int eulerOrder);
/**
 * @param transform
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteVector3C or use a direct
 * access function such as GetTransformPositionXC to avoid memory management
 * @return Heap allocated Vector3 representing the position of the given transform
 */
CALUMIANIMATION_API CALUMI::Math::Vector3* GetTransformPositionC(CALUMI::Math::Transform* transform);
/**
 * @param transform
 * @return Position's x value\n NaN will be returned if ptr is invalid
 */
CALUMIANIMATION_API float GetTransformPositionXC(CALUMI::Math::Transform* transform);
/**
 * @param transform
 * @return Position's y value\n NaN will be returned if ptr is invalid
 */
CALUMIANIMATION_API float GetTransformPositionYC(CALUMI::Math::Transform* transform);
/**
 * @param transform
 * @return Position's z value\n NaN will be returned if ptr is invalid
 */
CALUMIANIMATION_API float GetTransformPositionZC(CALUMI::Math::Transform* transform);
/**
 * @param transform
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteQuaternionC or use a direct
 * access function such as GetTransformRotationXC to avoid memory management
 * @return Heap allocated Quaternion representing the rotation of the given transform
 */
CALUMIANIMATION_API CALUMI::Math::Quaternion* GetTransformRotationC(CALUMI::Math::Transform* transform);
/**
 * @param transform
 * @return Rotation's x value\n NaN will be returned if ptr is invalid
 */
CALUMIANIMATION_API float GetTransformRotationXC(CALUMI::Math::Transform* transform);
/**
 * @param transform
 * @return Rotation's y value\n NaN will be returned if ptr is invalid
 */
CALUMIANIMATION_API float GetTransformRotationYC(CALUMI::Math::Transform* transform);
/**
 * @param transform
 * @return Rotation's z value\n NaN will be returned if ptr is invalid
 */
CALUMIANIMATION_API float GetTransformRotationZC(CALUMI::Math::Transform* transform);
/**
 * @param transform
 * @return Rotation's w value\n NaN will be returned if ptr is invalid
 */
CALUMIANIMATION_API float GetTransformRotationWC(CALUMI::Math::Transform* transform);
/**
 * @param ptr Dynamically allocated quaternion to delete
 * @return Error code: \n -1 = Invalid pointer \n 0 = Successful Deletion
 */
CALUMIANIMATION_API int DeleteTransformC(CALUMI::Math::Transform* ptr);
/**
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteQuaternionC
 * @note To get a basic quaternion with rotation, insert (0.0, 0.0, 0.0, 1.0)
 * @return A heap allocated, normalized, Quaternion <float> of the given values
 */
CALUMIANIMATION_API CALUMI::Math::Quaternion* CreateQuaternionC(float x, float y, float z, float w);
/**
 * @param x
 * @param y
 * @param z
 * @param eulerOrder
 * @warning Heap allocated return value, if not nullptr, must be deleted using #DeleteQuaternionC
 * @return
 */
CALUMI::Math::Quaternion* CreateQuaternionFromEulerC(float x, float y, float z, int eulerOrder);
/** @relates Quaternion
 *  @name Extern "C"
 */
CALUMIANIMATION_API float GetQuaternionXC(CALUMI::Math::Quaternion* source);
/** @relates Quaternion
 *  @name Extern "C"
 */
CALUMIANIMATION_API float GetQuaternionYC(CALUMI::Math::Quaternion* source);
/** @relates Quaternion
 *  @name Extern "C"
 */
CALUMIANIMATION_API float GetQuaternionZC(CALUMI::Math::Quaternion* source);
/** @relates Quaternion
 *  @name Extern "C"
 */
CALUMIANIMATION_API float GetQuaternionWC(CALUMI::Math::Quaternion* source);
/**
 * @param input
 * @param eulerOrder
 * @param first ptr to the first output float
 * @param second ptr to the second output float
 * @param third ptr to the third output float
 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation
 */
CALUMIANIMATION_API int GetQuaternionToEulerC(CALUMI::Math::Quaternion* input,
											  int eulerOrder,
											  float* first,
											  float* second,
											  float* third);
/** @relates Quaternion
 *  @name Extern "C"
 *  @return Error Code:\n -1 = Invalid Ptr\n 0 = Successful Operation\n 1 = Invalid Axis
 */
CALUMIANIMATION_API int RotateQuaternionByAxisAngleC(CALUMI::Math::Quaternion* input,
													 CALUMI::Math::Quaternion* result,
													 float x,
													 float y,
													 float z,
													 float radians);
/**
 * @param input Starting rotation
 * @param offset Rotation to apply to input
 * @param result Output rotation
 * @return Error code:\n -1 = Invalid Ptr\n 0 = Successful Operation
 */
CALUMIANIMATION_API int RotateQuaternionByQuaternionC(CALUMI::Math::Quaternion* input,
                                                      CALUMI::Math::Quaternion* offset,
                                                      CALUMI::Math::Quaternion* result);
/**
 * @param input Rotation of interest
 * @param reference Reference rotation
 * @param result Offset rotation to get from the reference to the input
 * @return Error code:\n -1 = Invalid Ptr\n 0 = Successful Operation
 */
CALUMIANIMATION_API int GetQuaternionOffsetC(CALUMI::Math::Quaternion* input,
                                             CALUMI::Math::Quaternion* reference,
                                             CALUMI::Math::Quaternion* result);
/**
 * @param ptr Dynamically allocated quaternion to delete
 * @return Error code: \n -1 = Invalid pointer \n 0 = Successful Deletion
 */
CALUMIANIMATION_API int DeleteQuaternionC(CALUMI::Math::Quaternion* ptr);
}

/// @}
/// @}
/// @}
