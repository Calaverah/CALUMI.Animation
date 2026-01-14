//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"
#include "CALUMI_Utilities.h"



namespace CALUMI
{
	namespace Math{

		CALUMIANIMATION_API double ToRadians(double degrees) noexcept;
		CALUMIANIMATION_API double ToDegrees(double radians) noexcept;

		struct CALUMIANIMATION_API Vector2
		{

			Vector2();
			~Vector2();
			Vector2(float input[2]);
			Vector2(float input);
			Vector2(float x, float y);
			Vector2(const Vector2& input);

			float getX() const;
			void setX(float x);
			float getY() const;
			void setY(float y);

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

			float Length() const;
			float LengthSquared() const;

			float Dot(const Vector2& other) const;

			void Cross(const Vector2& other, Vector2& result) const;
			Vector2 Cross(const Vector2& other) const;

			void Normalize();

			static const Vector2 Zero;
			static const Vector2 One;
			static const Vector2 UnitX;
			static const Vector2 UnitY;
			static const Vector2 Up;
			static const Vector2 Down;
			static const Vector2 Right;
			static const Vector2 Left;

			Utilities::StringContainer ToString() const;

		private:
			struct Impl;
			Impl* pImpl;
		};

		CALUMIANIMATION_API Math::Vector2 operator+ (const Vector2& A, const Vector2& B) noexcept;
		CALUMIANIMATION_API Math::Vector2 operator- (const Vector2& A, const Vector2& B) noexcept;
		CALUMIANIMATION_API Math::Vector2 operator* (const Vector2& A, const Vector2& B) noexcept;
		CALUMIANIMATION_API Math::Vector2 operator* (const Vector2& A, float B) noexcept;
		CALUMIANIMATION_API Math::Vector2 operator* (float A, const Vector2& B) noexcept;
		CALUMIANIMATION_API Math::Vector2 operator/ (const Vector2& A, const Vector2& B) noexcept;
		CALUMIANIMATION_API Math::Vector2 operator/ (const Vector2& A, float B) noexcept;
		CALUMIANIMATION_API Math::Vector2 operator/ (float A, const Vector2& B) noexcept;

		extern "C" {
			CALUMIANIMATION_API float GetVector2X(Vector2* source);
			CALUMIANIMATION_API float GetVector2Y(Vector2* source);
		}

		struct CALUMIANIMATION_API Vector2D
		{
			double getX() const;
			void setX(double x);
			double getY() const;
			void setY(double y);

			Vector2D();
			~Vector2D();
			Vector2D(double input[2]);
			Vector2D(double input);
			Vector2D(Vector2 input);
			Vector2D(const Vector2D& input);
			Vector2D(double x, double y);

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

			double Length() const;
			double LengthSquared() const;

			double Dot(const Vector2D& other) const;
			void Cross(const Vector2D& other, Vector2D& result) const;
			Vector2D Cross(const Vector2D& other) const;

			void Normalize();

			static const Vector2D Zero;
			static const Vector2D One;
			static const Vector2D UnitX;
			static const Vector2D UnitY;
			static const Vector2D Up;
			static const Vector2D Down;
			static const Vector2D Right;
			static const Vector2D Left;

			Utilities::StringContainer ToString() const;

		private:
			struct Impl;
			Impl* pImpl;
		};

		CALUMIANIMATION_API Math::Vector2D operator+ (const Vector2D& A, const Vector2D& B) noexcept;
		CALUMIANIMATION_API Math::Vector2D operator- (const Vector2D& A, const Vector2D& B) noexcept;
		CALUMIANIMATION_API Math::Vector2D operator* (const Vector2D& A, const Vector2D& B) noexcept;
		CALUMIANIMATION_API Math::Vector2D operator* (const Vector2D& A, double B) noexcept;
		CALUMIANIMATION_API Math::Vector2D operator* (double A, const Vector2D& B) noexcept;
		CALUMIANIMATION_API Math::Vector2D operator/ (const Vector2D& A, const Vector2D& B) noexcept;
		CALUMIANIMATION_API Math::Vector2D operator/ (const Vector2D& A, double B) noexcept;
		CALUMIANIMATION_API Math::Vector2D operator/ (double A, const Vector2D& B) noexcept;

		extern "C" {
			CALUMIANIMATION_API double GetVector2DX(Vector2D* source);
			CALUMIANIMATION_API double GetVector2DY(Vector2D* source);
		}

		struct CALUMIANIMATION_API Vector3
		{
			float getX() const;
			void setX(float x);
			float getY() const;
			void setY(float y);
			float getZ() const;
			void setZ(float z);

			Vector3();
			~Vector3();
			Vector3(float input[3]);
			Vector3(double input[3]);
			Vector3(float input);
			Vector3(double input);
			Vector3(float x, float y, float z);
			Vector3(double x, double y, double z);
			Vector3(const Vector3& source);

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

			float Length() const;
			float LengthSquared() const;

			float Dot(const Vector3& other) const;

			void Cross(const Vector3& other, Vector3& result) const;
			Vector3 Cross(const Vector3& other) const;

			void Normalize();

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

			Utilities::StringContainer ToString() const;

		private:
			struct Impl;
			Impl* pImpl;
		};

		CALUMIANIMATION_API Math::Vector3 operator+ (const Vector3& A, const Vector3& B) noexcept;
		CALUMIANIMATION_API Math::Vector3 operator- (const Vector3& A, const Vector3& B) noexcept;
		CALUMIANIMATION_API Math::Vector3 operator* (const Vector3& A, const Vector3& B) noexcept;
		CALUMIANIMATION_API Math::Vector3 operator* (const Vector3& A, float B) noexcept;
		CALUMIANIMATION_API Math::Vector3 operator* (float A, const Vector3& B) noexcept;
		CALUMIANIMATION_API Math::Vector3 operator/ (const Vector3& A, const Vector3& B) noexcept;
		CALUMIANIMATION_API Math::Vector3 operator/ (const Vector3& A, float B) noexcept;
		CALUMIANIMATION_API Math::Vector3 operator/ (float A, const Vector3& B) noexcept;

		extern "C" {
			CALUMIANIMATION_API float GetVector3X(Vector3* source);
			CALUMIANIMATION_API float GetVector3Y(Vector3* source);
			CALUMIANIMATION_API float GetVector3Z(Vector3* source);
		}

		struct CALUMIANIMATION_API Vector3D
		{
			double getX() const;
			void setX(double x);
			double getY() const;
			void setY(double y);
			double getZ() const;
			void setZ(double z);

			Vector3D();
			~Vector3D();
			Vector3D(double input[3]);
			Vector3D(double input);
			Vector3D(Vector3 input);
			Vector3D(const Vector3D& input);
			Vector3D(double x, double y, double z);

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

			double Length() const;
			double LengthSquared() const;

			double Dot(const Vector3D& other) const;
			void Cross(const Vector3D& other, Vector3D& result) const;
			Vector3D Cross(const Vector3D& other) const;

			void Normalize();

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

			Utilities::StringContainer ToString() const;

		private:
			struct Impl;
			Impl* pImpl;
		};

		CALUMIANIMATION_API Math::Vector3D operator+ (const Vector3D& A, const Vector3D& B) noexcept;
		CALUMIANIMATION_API Math::Vector3D operator- (const Vector3D& A, const Vector3D& B) noexcept;
		CALUMIANIMATION_API Math::Vector3D operator* (const Vector3D& A, const Vector3D& B) noexcept;
		CALUMIANIMATION_API Math::Vector3D operator* (const Vector3D& A, double B) noexcept;
		CALUMIANIMATION_API Math::Vector3D operator* (double A, const Vector3D& B) noexcept;
		CALUMIANIMATION_API Math::Vector3D operator/ (const Vector3D& A, const Vector3D& B) noexcept;
		CALUMIANIMATION_API Math::Vector3D operator/ (const Vector3D& A, double B) noexcept;
		CALUMIANIMATION_API Math::Vector3D operator/ (double A, const Vector3D& B) noexcept;

		extern "C" {
			CALUMIANIMATION_API double GetVector3DX(Vector3D* source);
			CALUMIANIMATION_API double GetVector3DY(Vector3D* source);
			CALUMIANIMATION_API double GetVector3DZ(Vector3D* source);
		}

		/// <summary>
		/// CALUMI::MATH::Quaternions are normalized by default
		/// </summary>
		struct CALUMIANIMATION_API Quaternion
		{

			Quaternion();
			~Quaternion();
			Quaternion(float input[4], bool normalize = true);
			Quaternion(Vector3 direction, double radians, bool normalize = true);
			Quaternion(Vector3D direction, double radians, bool normalize = true);
			Quaternion(double x, double y, double z, double w, bool normalize = true);
			Quaternion(float x, float y, float z, float w, bool normalize = true);
			Quaternion(const Quaternion& input);

			float getX() const;
			void setX(float x);
			float getY() const;
			void setY(float y);
			float getZ() const;
			void setZ(float z);
			float getW() const;
			void setW(float w);

			bool operator == (const Quaternion& input) const noexcept;
			bool operator != (const Quaternion& input) const noexcept;
			bool AreSameRotation(const Quaternion& input, float tolerance = 0.0) const noexcept;
			bool AreEqual(const Quaternion& input, float tolerance = 0.0) const noexcept;

			Quaternion& operator+=(const Quaternion& other) noexcept;
			Quaternion& operator-=(const Quaternion& other) noexcept;
			Quaternion& operator*=(const Quaternion& other) noexcept;
			Quaternion& operator*=(float other) noexcept;
			Quaternion& operator/=(const Quaternion& other) noexcept;

			Quaternion& operator=(const Quaternion& other) noexcept;

			Quaternion operator+() const noexcept;
			Quaternion operator-() const noexcept;

			Quaternion Conjugate() noexcept;
			void Conjugate(Quaternion& result) const noexcept;

			float Dot(const Quaternion& input) const noexcept;

			float LengthSquared() const noexcept;
			float Length() const noexcept;

			void Normalize() noexcept;
			void Normalize(Quaternion& result) const noexcept;

			void Inverse(Quaternion& result) const noexcept;
			void Inverse() noexcept;

			static const Quaternion Identity;

			Utilities::StringContainer ToString();

		private:
			struct Impl;
			Impl* pImpl;
		};

		CALUMIANIMATION_API Math::Quaternion operator+ (const Quaternion& A, const Quaternion& B) noexcept;
		CALUMIANIMATION_API Math::Quaternion operator- (const Quaternion& A, const Quaternion& B) noexcept;
		CALUMIANIMATION_API Math::Quaternion operator* (const Quaternion& A, const Quaternion& B) noexcept;
		CALUMIANIMATION_API Math::Quaternion operator* (const Quaternion& A, float B) noexcept;
		CALUMIANIMATION_API Math::Quaternion operator* (float A, const Quaternion& B) noexcept;
		CALUMIANIMATION_API Math::Quaternion operator/ (const Quaternion& A, const Quaternion& B) noexcept;

		extern "C" {
			CALUMIANIMATION_API float GetQuaternionX(Quaternion* source);
			CALUMIANIMATION_API float GetQuaternionY(Quaternion* source);
			CALUMIANIMATION_API float GetQuaternionZ(Quaternion* source);
			CALUMIANIMATION_API float GetQuaternionW(Quaternion* source);
			CALUMIANIMATION_API bool RotateQuaternionByAxisAngleC(Quaternion* input, Quaternion* result, float x, float y, float z, float radians);
		}
	}
	
}
