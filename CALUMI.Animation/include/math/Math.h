//Copyright � 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Common.h"
#include "utilities/CALUMI_Utilities.h"

	/**
	 * @defgroup macros Useful Macros
	 * @{
	 * @section math Math Macros
	 * @{
	 * @name Math
	 * @{
	 */
	#define CLA_E        2.71828182845904523536   // e constant
	#define CLA_LOG2E    1.44269504088896340736   // log2(e)
	#define CLA_LOG10E   0.434294481903251827651  // log10(e)
	#define CLA_LN2      0.693147180559945309417  // ln(2)
	#define CLA_LN10     2.30258509299404568402   // ln(10)
	#define CLA_PI       3.14159265358979323846   // pi
	#define CLA_PI_2     1.57079632679489661923   // pi/2
	#define CLA_PI_4     0.785398163397448309616  // pi/4
	#define CLA_1_PI     0.318309886183790671538  // 1/pi
	#define CLA_2_PI     0.636619772367581343076  // 2/pi
	#define CLA_2_SQRTPI 1.12837916709551257390   // 2/sqrt(pi)
	#define CLA_SQRT2    1.41421356237309504880   // sqrt(2)
	#define CLA_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)
	/// @}
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
		/** @name Constructors */
		/// @{
		Vector2();
		~Vector2();
		explicit Vector2(float input[2]);
		explicit Vector2(float input);
		Vector2(float x, float y);
		Vector2(const Vector2& input);
		/// @}
		/** @name Data */
		/// @{

		float x() const;
		void setX(float x);
		float y() const;
		void setY(float y);

		/// @}

		/** @name Operators */
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
		/** @name Vector Operations */
		/// @{

		float Length() const;
		float LengthSquared() const;

		float Dot(const Vector2& other) const;

		void Cross(const Vector2& other, Vector2& result) const;
		Vector2 Cross(const Vector2& other) const;

		void Normalize();

		/// @}

		static const Vector2 Zero;
		static const Vector2 One;
		static const Vector2 UnitX;
		static const Vector2 UnitY;
		static const Vector2 Up;
		static const Vector2 Down;
		static const Vector2 Right;
		static const Vector2 Left;


		/** @name Print */
		/// @{

		Utilities::StringContainer ToString() const;

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


	extern "C" {
	/** @relates Vector2
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API float GetVector2X(Vector2* source);
	/** @relates Vector2
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API float GetVector2Y(Vector2* source);
	}

	struct CALUMIANIMATION_API Vector2D
	{
		/** @name Constructors */
		/// @{

		Vector2D();
		~Vector2D();
		explicit Vector2D(double input[2]);
		explicit Vector2D(double input);
		explicit Vector2D(Vector2 input);
		Vector2D(const Vector2D& input);
		Vector2D(double x, double y);

		/// @}
		/** @name Data */
		/// @{

		double x() const;
		void setX(double x);
		double y() const;
		void setY(double y);

		/// @}
		/** @name Operators */
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
		/** @name Vector Operations */
		/// @{

		double length() const;
		double lengthSquared() const;

		double dot(const Vector2D& other) const;
		void cross(const Vector2D& other, Vector2D& result) const;
		Vector2D cross(const Vector2D& other) const;

		void normalize();

		Vector2D lerp(const Vector2D& input, double t = 0.5) const;

		/// @}

		static const Vector2D Zero;
		static const Vector2D One;
		static const Vector2D UnitX;
		static const Vector2D UnitY;
		static const Vector2D Up;
		static const Vector2D Down;
		static const Vector2D Right;
		static const Vector2D Left;

		/** @name Print */
		/// @{
		Utilities::StringContainer toString() const;
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

	extern "C" {
	/** @relates Vector2D
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API double GetVector2DX(Vector2D* source);
	/** @relates Vector2D
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API double GetVector2DY(Vector2D* source);
	}

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

		float x() const;
		void setX(float x) const;
		float y() const;
		void setY(float y) const;
		float z() const;
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

		float length() const;
		float lengthSquared() const;

		float dot(const Vector3& other) const;

		void cross(const Vector3& other, Vector3& result) const;
		Vector3 cross(const Vector3& other) const;

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

		Utilities::StringContainer toString() const;

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

	extern "C" {
	/** @relates Vector3
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API float GetVector3X(Vector3* source);
	/** @relates Vector3
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API float GetVector3Y(Vector3* source);
	/** @relates Vector3
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API float GetVector3Z(Vector3* source);
	}

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

		double x() const;
		void setX(double x);
		double y() const;
		void setY(double y);
		double z() const;
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

		double length() const;
		double lengthSquared() const;

		double dot(const Vector3D& other) const;
		void cross(const Vector3D& other, Vector3D& result) const;
		Vector3D cross(const Vector3D& other) const;

		void normalize();

		Vector3D lerp(const Vector3D& input, double t = 0.5) const;

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

		Utilities::StringContainer toString() const;

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

	extern "C" {
	/** @relates Vector3D
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API double GetVector3DX(Vector3D* source);
	/** @relates Vector3D
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API double GetVector3DY(Vector3D* source);
	/** @relates Vector3D
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API double GetVector3DZ(Vector3D* source);
	}

	/**
		 * @brief Imaginary way to mathematically describe a rotation without risk of gimble lock
		 */
	struct CALUMIANIMATION_API Quaternion
	{
		/**
			 * @brief Order of operations for converting euler (rx,ry,rz) to quaternion
			 */
		enum class EulerOrder{
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
			Max = ZYZ
		};

		/** @name Constructors */
		/// @{

		Quaternion();
		~Quaternion();
		explicit Quaternion(float input[4], bool normalized = true);
		Quaternion(Vector3 direction, double radians, bool normalized = true);
		Quaternion(Vector3D direction, double radians, bool normalized = true);
		Quaternion(double x, double y, double z, double w, bool normalized = true);
		Quaternion(float x, float y, float z, float w, bool normalized = true);
		Quaternion(float x, float y, float z, EulerOrder order = EulerOrder::XYZ);
		Quaternion(const Quaternion& input);

		/// @}
		/** @name Data */
		/// @{

		float x() const;
		void setX(float x);
		float y() const;
		void setY(float y);
		float z() const;
		void setZ(float z);
		float w() const;
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

		bool areSameRotation(const Quaternion& input, float tolerance = 0.0) const noexcept;
		bool areEqual(const Quaternion& input, float tolerance = 0.0) const noexcept;

		/// @}
		/** @name Vector Operations */
		/// @{

		Quaternion conjugate() noexcept;
		void conjugate(Quaternion& result) const noexcept;

		float dot(const Quaternion& input) const noexcept;

		float lengthSquared() const noexcept;
		float length() const noexcept;

		void normalize() noexcept;
		void normalize(Quaternion& result) const noexcept;

		void inverse(Quaternion& result) const noexcept;
		void inverse() const noexcept;

		//Returns angle in radians
		float angularDistance(const Quaternion& input) const;

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

		Quaternion sLerp(const Quaternion& input, float t = 0.5f) const;

		static const Quaternion Identity;

		/// @}
		/** @name Print */
		/// @{

		Utilities::StringContainer toString() const;

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

	extern "C" {
	/** @relates Quaternion
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API float GetQuaternionX(Quaternion* source);
	/** @relates Quaternion
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API float GetQuaternionY(Quaternion* source);
	/** @relates Quaternion
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API float GetQuaternionZ(Quaternion* source);
	/** @relates Quaternion
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API float GetQuaternionW(Quaternion* source);
	/** @relates Quaternion
			 *  @name Extern "C"
			 */
	CALUMIANIMATION_API bool RotateQuaternionByAxisAngleC(Quaternion* input, Quaternion* result, float x, float y, float z, float radians);
	}
}
