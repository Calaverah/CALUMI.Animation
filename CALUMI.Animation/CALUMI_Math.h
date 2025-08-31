//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com
#define _USE_MATH_DEFINES
#pragma once
#include "CALUMI_Common.h"
#include <cmath>
#include <string>
#include <format>
#include <math.h>



namespace CALUMI
{
	namespace Math{

		double CALUMIANIMATION_API ToRadians(double degrees) noexcept;
		double CALUMIANIMATION_API ToDegrees(double radians) noexcept;

		struct CALUMIANIMATION_API Vector3
		{
			float x = 0.0;
			float y = 0.0;
			float z = 0.0;

			Vector3() = default;
			Vector3(float input[3]);
			Vector3(float input);
			Vector3(float x, float y, float z)
				: x(x), y(y), z(z)
			{
			}

			bool operator==(const Vector3& other) const;
			bool operator!=(const Vector3& other) const;

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

			std::string ToString();
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
			double x = 0.0;
			double y = 0.0;
			double z = 0.0;

			Vector3D() = default;
			Vector3D(double input[3]);
			Vector3D(double input);
			Vector3D(Vector3 input);
			Vector3D(double x, double y, double z)
				: x(x), y(y), z(z)
			{
			}

			bool operator==(const Vector3D& other) const;
			bool operator!=(const Vector3D& other) const;

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

			std::string ToString();
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
			float x = 0.0;
			float y = 0.0;
			float z = 0.0;
			float w = 1.0;

			Quaternion() = default;
			Quaternion(float input[4], bool normalize = true);
			Quaternion(Vector3 direction, float radians, bool normalize = true);
			Quaternion(Vector3D direction, float radians, bool normalize = true);
			Quaternion(float x, float y, float z, float w, bool normalize = true)
				: x(x), y(y), z(z), w(w)
			{
				if(normalize)
				Normalize();
			}

			bool operator == (const Quaternion& input) const noexcept;
			bool operator != (const Quaternion& input) const noexcept;
			bool AreSameRotation(const Quaternion& input, float tolerance = 0.0) const noexcept;
			bool AreEqual(const Quaternion& input, float tolerance = 0.0) const noexcept;

			Quaternion& operator+=(const Quaternion& other) noexcept;
			Quaternion& operator-=(const Quaternion& other) noexcept;
			Quaternion& operator*=(const Quaternion& other) noexcept;
			Quaternion& operator*=(float other) noexcept;
			Quaternion& operator/=(const Quaternion& other) noexcept;

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

			std::string ToString();

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
