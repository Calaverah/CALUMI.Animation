//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com
#include "pch.h"
#include "CALUMI_Math.h"
#include <cmath>
#include <math.h>
#include <format>
#include <corecrt_math_defines.h>

namespace CALUMI
{
	namespace Math{

		//Math::Vector3 (float)
		const Math::Vector3 Math::Vector3::Zero = 0.0;
		const Math::Vector3 Math::Vector3::One = 1.0;
		const Math::Vector3 Math::Vector3::UnitX = { 1.0, 0.0, 0.0 };
		const Math::Vector3 Math::Vector3::UnitY = { 0.0, 1.0, 0.0 };
		const Math::Vector3 Math::Vector3::UnitZ = { 0.0, 0.0, 1.0 };

		const Math::Vector3 Math::Vector3::Up = { 0.0, 0.0, 1.0 }; //In coordinate system found on Nifskope. May need modifying
		const Math::Vector3 Math::Vector3::Down = { 0.0, 0.0, -1.0 };
		const Math::Vector3 Math::Vector3::Right = { -1.0, 0.0, 0.0 };
		const Math::Vector3 Math::Vector3::Left = { 1.0, 0.0, 0.0 };
		const Math::Vector3 Math::Vector3::Forward = { 0.0, 1.0, 0.0 };
		const Math::Vector3 Math::Vector3::Back = { 0.0, -1.0, 0.0 };

		Utilities::StringContainer Math::Vector3::ToString()
		{
			Utilities::StringContainer output;
			output += std::format("x: {}, y: {}, z: {}", x, y, z).c_str();
			return output;
		}

		Math::Vector3::Vector3(float input[3])
		{
			x = input[0];
			y = input[1];
			z = input[2];
		}

		Math::Vector3::Vector3(float input)
		{
			x = input;
			y = input;
			z = input;
		}

		bool Math::Vector3::operator==(const Math::Vector3& other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		bool Math::Vector3::operator!=(const Math::Vector3& other) const
		{
			return !(x == other.x && y == other.y && z == other.z);
		}

		Math::Vector3& Math::Vector3::operator+=(const Math::Vector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Math::Vector3& Math::Vector3::operator-=(const Math::Vector3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		Math::Vector3& Math::Vector3::operator*=(const Math::Vector3& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		Math::Vector3& Math::Vector3::operator*=(float other)
		{
			x *= other;
			y *= other;
			z *= other;
			return *this;
		}

		Math::Vector3& Math::Vector3::operator/=(float other)
		{
			x /= other;
			y /= other;
			z /= other;
			return *this;
		}

		Math::Vector3 Math::Vector3::operator+() const { return *this; };

		Math::Vector3 Math::Vector3::operator-() const { return Math::Vector3(-x, -y, -z); };

		float Math::Vector3::Length() const
		{

			return sqrt((x * x) + (y * y) + (z * z));
		}
		float Math::Vector3::LengthSquared() const
		{
			return (x * x) + (y * y) + (z * z);
		}

		float Math::Vector3::Dot(const Math::Vector3& other) const
		{
			return (x * other.x) + (y * other.y) + (z * other.z);
		}

		void Math::Vector3::Cross(const Math::Vector3& other, Math::Vector3& result) const
		{
			result.x = y * other.z - z * other.y;
			result.y = z * other.x - x * other.z;
			result.z = x * other.y - y * other.x;
		}

		Math::Vector3 Math::Vector3::Cross(const Math::Vector3& other) const
		{
			return Math::Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}

		void Math::Vector3::Normalize()
		{
			*this /= this->Length();
		}

		double ToRadians(double degrees) noexcept { return degrees * (M_PI / 180.0); }

		double ToDegrees(double radians) noexcept { return radians * (180.0 / M_PI); }

		Math::Vector3 operator+ (const Math::Vector3& A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		}
		Math::Vector3 operator- (const Math::Vector3& A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A.x - B.x, A.y - B.y, A.z - B.z);
		}
		Math::Vector3 operator* (const Math::Vector3& A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A.x * B.x, A.y * B.y, A.z * B.z);
		}
		Math::Vector3 operator* (const Math::Vector3& A, float B) noexcept
		{
			return Math::Vector3(A.x * B, A.y * B, A.z * B);
		}
		Math::Vector3 operator* (float A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A * B.x, A * B.y, A * B.z);
		}
		Math::Vector3 operator/ (const Math::Vector3& A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A.x / B.x, A.y / B.y, A.z / B.z);
		}
		Math::Vector3 operator/ (const Math::Vector3& A, float B) noexcept
		{
			return Math::Vector3(A.x / B, A.y / B, A.z / B);
		}
		Math::Vector3 CALUMI::Math::operator/(float A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A / B.x, A / B.y, A / B.z);
		}

		float GetVector3X(Vector3* source)
		{
			return source->x;
		}

		float GetVector3Y(Vector3* source)
		{
			return source->y;
		}

		float GetVector3Z(Vector3* source)
		{
			return source->z;
		}


		//Math::Vector3D (double)
		const Math::Vector3D Math::Vector3D::Zero = 0.0; 
		const Math::Vector3D Math::Vector3D::One = 1.0;
		const Math::Vector3D Math::Vector3D::UnitX = { 1.0, 0.0, 0.0};
		const Math::Vector3D Math::Vector3D::UnitY = { 0.0, 1.0, 0.0};
		const Math::Vector3D Math::Vector3D::UnitZ = { 0.0, 0.0, 1.0};

		const Math::Vector3D Math::Vector3D::Up = { 0.0, 0.0, 1.0}; //In coordinate system found on Nifskope. May need modifying
		const Math::Vector3D Math::Vector3D::Down = { 0.0, 0.0, -1.0};
		const Math::Vector3D Math::Vector3D::Right = { -1.0, 0.0, 0.0};
		const Math::Vector3D Math::Vector3D::Left = { 1.0, 0.0, 0.0};
		const Math::Vector3D Math::Vector3D::Forward = { 0.0, 1.0, 0.0};
		const Math::Vector3D Math::Vector3D::Back = { 0.0, -1.0, 0.0};

		Utilities::StringContainer Math::Vector3D::ToString()
		{
			Utilities::StringContainer output;
			output += std::format("x: {}, y: {}, z: {}",x,y,z).c_str();
			return output;
		}

		Math::Vector3D::Vector3D(double input[3])
		{
			x = input[0];
			y = input[1];
			z = input[2];
		}

		Math::Vector3D::Vector3D(double input)
		{
			x = input;
			y = input;
			z = input;
		}

		Math::Vector3D::Vector3D(Math::Vector3 input)
		{
			x = input.x;
			y = input.y;
			z = input.z;
		}

		bool Math::Vector3D::operator==(const Math::Vector3D & other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		bool Math::Vector3D::operator!=(const Math::Vector3D & other) const
		{
			return !(x == other.x && y == other.y && z == other.z);
		}

		Math::Vector3D& Math::Vector3D::operator+=(const Math::Vector3D & other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Math::Vector3D& Math::Vector3D::operator-=(const Math::Vector3D & other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		Math::Vector3D& Math::Vector3D::operator*=(const Math::Vector3D & other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		Math::Vector3D& Math::Vector3D::operator*=(double other)
		{
			x *= other;
			y *= other;
			z *= other;
			return *this;
		}

		Math::Vector3D& Math::Vector3D::operator/=(double other)
		{
			x /= other;
			y /= other;
			z /= other;
			return *this;
		}

		Math::Vector3D Math::Vector3D::operator+() const { return *this; };

		Math::Vector3D Math::Vector3D::operator-() const { return Math::Vector3D(-x, -y, -z); };

		double Math::Vector3D::Length() const
		{

			return sqrt((x * x) + (y * y) + (z * z));
		}
		double Math::Vector3D::LengthSquared() const
		{
			return (x * x) + (y * y) + (z * z);
		}

		double Math::Vector3D::Dot(const Math::Vector3D & other) const
		{
			return (x * other.x) + (y * other.y) + (z * other.z);
		}

		void Math::Vector3D::Cross(const Math::Vector3D & other, Math::Vector3D & result) const
		{
			result.x = y * other.z - z * other.y;
			result.y = z * other.x - x * other.z;
			result.z = x * other.y - y * other.x;
		}

		Math::Vector3D Math::Vector3D::Cross(const Math::Vector3D & other) const
		{
			return Math::Vector3D(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}

		void Math::Vector3D::Normalize()
		{
			*this /= this->Length();
		}

		Math::Vector3D operator+ (const Math::Vector3D& A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A.x + B.x, A.y + B.y, A.z + B.z);
		}
		Math::Vector3D operator- (const Math::Vector3D& A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A.x - B.x, A.y - B.y, A.z - B.z);
		}
		Math::Vector3D operator* (const Math::Vector3D& A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A.x * B.x, A.y * B.y, A.z * B.z);
		}
		Math::Vector3D operator* (const Math::Vector3D& A, double B) noexcept
		{
			return Math::Vector3D(A.x * B, A.y * B, A.z * B);
		}
		Math::Vector3D operator* (double A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A * B.x, A * B.y, A * B.z);
		}
		Math::Vector3D operator/ (const Math::Vector3D& A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A.x / B.x, A.y / B.y, A.z / B.z);
		}
		Math::Vector3D operator/ (const Math::Vector3D& A, double B) noexcept
		{
			return Math::Vector3D(A.x / B, A.y / B, A.z / B);
		}
		Math::Vector3D CALUMI::Math::operator/(double A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A / B.x, A / B.y, A / B.z);
		}

		double GetVector3DX(Vector3D* source)
		{
			return source->x;
		}

		double GetVector3DY(Vector3D* source)
		{
			return source->y;
		}

		double GetVector3DZ(Vector3D* source)
		{
			return source->z;
		}

		//Math::Quaternions (float)
		Math::Quaternion::Quaternion(float input[4], bool normalize)
		{
			x = input[0];
			y = input[1];
			z = input[2];
			w = input[3];

			if(normalize)
			Normalize();
		};

		Math::Quaternion::Quaternion(Vector3 direction, double radians, bool normalize)
		{
			direction.Normalize();
			float halfTheta = static_cast<float>(radians)/2.0f;
			float halfThetaSin = std::sin(halfTheta);
			float halfThetaCos = std::cos(halfTheta);

			x = halfThetaSin * direction.x;
			y = halfThetaSin * direction.y;
			z = halfThetaSin * direction.z;
			w = halfThetaCos;
			
			if (normalize)
				Normalize();
		};

		Math::Quaternion::Quaternion(Vector3D direction, double radians, bool normalize)
		{
			direction.Normalize();
			double halfTheta = radians / 2.0f;
			double halfThetaSin = std::sin(halfTheta);
			double halfThetaCos = std::cos(halfTheta);

			x = static_cast<float>(halfThetaSin * direction.x);
			y = static_cast<float>(halfThetaSin * direction.y);
			z = static_cast<float>(halfThetaSin * direction.z);
			w = static_cast<float>(halfThetaCos);
			
			if (normalize)
				Normalize();
		};



		Math::Quaternion Math::Quaternion::operator+() const noexcept
		{
			return *this;
		}

		Math::Quaternion Math::Quaternion::operator-() const noexcept
		{
			return Math::Quaternion(-x, -y, -z, -w);
		}

		bool Math::Quaternion::operator==(const Math::Quaternion& input) const noexcept
		{
			return (x == input.x && y == input.y && z == input.z && w == input.w);
		}

		bool Math::Quaternion::operator!=(const Math::Quaternion& input) const noexcept
		{
			return !(x == input.x && y == input.y && z == input.z && w == input.w);
		}

		bool Quaternion::AreSameRotation(const Quaternion& input, float tolerance) const noexcept
		{
			return (AreEqual(input,tolerance)|| AreEqual(-input, tolerance));
		}

		bool Quaternion::AreEqual(const Quaternion & input, float tolerance) const noexcept
		{
			float dx = abs(x - input.x);
			float dy = abs(y - input.y);
			float dz = abs(z - input.z);
			float dw = abs(w - input.w);
			return (dx < tolerance && dy < tolerance && dz < tolerance && dw < tolerance);
		}

		Math::Quaternion& Math::Quaternion::operator+= (const Math::Quaternion& other) noexcept
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}

		Math::Quaternion& Math::Quaternion::operator-=(const Math::Quaternion& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}

		Math::Quaternion& Math::Quaternion::operator*=(const Math::Quaternion& other) noexcept
		{
			*this = *this * other;
			return *this;
		}

		Math::Quaternion& Math::Quaternion::operator*=(float other) noexcept
		{
			x *= other;
			y *= other;
			z *= other;
			w *= other;
			return *this;
		}

		Math::Quaternion& Math::Quaternion::operator/=(const Math::Quaternion& other) noexcept
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
			return *this;
		}

		Quaternion Math::Quaternion::Conjugate() noexcept
		{
			return Quaternion(x * -1, y * -1, z * -1, w);
		}

		float Math::Quaternion::Dot(const Math::Quaternion& input) const noexcept
		{
			return (x*input.x + y*input.y + z*input.z + w*input.w);
		}

		void Math::Quaternion::Conjugate(Math::Quaternion& result) const noexcept
		{
			result.x = -x;
			result.y = -y;
			result.z = -z;
			result.w = w;
		}

		float Math::Quaternion::LengthSquared() const noexcept
		{
			return (x*x + y*y + z*z + w*w);
		}

		float Math::Quaternion::Length() const noexcept
		{
			return sqrt(LengthSquared());
		}

		void Math::Quaternion::Normalize() noexcept
		{
			float length = Length();
			if (length <= 0)
			{
				length = 1.0;
			}

			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}

		void Math::Quaternion::Normalize(Math::Quaternion& result) const noexcept
		{
			float length = Length();
			if (length <= 0)
			{
				length = 1.0;
			}
			result.x = x / length;
			result.y = y / length;
			result.z = z / length;
			result.w = w / length;
		}

		void Math::Quaternion::Inverse(Math::Quaternion& result) const noexcept
		{
			float length = Length();
			if (length <= 0)
			{
				length = 1;
			}
			Math::Quaternion conjugate;
			Conjugate(conjugate);

			result.x = conjugate.x / length; 
			result.y = conjugate.y / length; 
			result.z = conjugate.z / length;
			result.w = conjugate.w / length;
		}

		void Math::Quaternion::Inverse() noexcept
		{
			float length = Length();
			if (length <= 0)
			{
				length = 1;
			}
			Math::Quaternion conjugate;
			Conjugate(conjugate);

			x = conjugate.x / length;
			y = conjugate.y / length;
			z = conjugate.z / length;
			w = conjugate.w / length;
		}

		const Math::Quaternion Math::Quaternion::Identity = {0.0f,0.0f,0.0f,1.0f};

		Utilities::StringContainer Math::Quaternion::ToString()
		{
			Utilities::StringContainer output;
			output += std::format("x: {}, y: {}, z: {}, w: {}", x, y, z, w).c_str();
			return output;
		}

		Math::Quaternion operator+ (const Math::Quaternion& A, const Math::Quaternion& B) noexcept
		{
			return Math::Quaternion(A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w );
		}
		Math::Quaternion operator- (const Math::Quaternion& A, const Math::Quaternion& B) noexcept
		{
			return Math::Quaternion(A.x - B.x, A.y - B.y, A.z - B.z, A.w - B.w);
		}
		Math::Quaternion operator* (const Math::Quaternion& A, const Math::Quaternion& B) noexcept
		{
			return Math::Quaternion(
				A.w * B.x + A.x * B.w + A.y * B.z - A.z * B.y,
				A.w * B.y - A.x * B.z + A.y * B.w + A.z * B.x,
				A.w * B.z + A.x * B.y - A.y * B.x + A.z * B.w,
				A.w * B.w - A.x * B.x - A.y * B.y - A.z * B.z
			);
		}
		Math::Quaternion operator* (const Math::Quaternion& A, float B) noexcept
		{
			return Math::Quaternion(A.x * B, A.y * B, A.z * B, A.w * B);
		}
		Math::Quaternion operator* (float A, const Math::Quaternion& B) noexcept
		{
			return Math::Quaternion(B.x * A, B.y * A, B.z * A, B.w * A);
		}
		Math::Quaternion operator/ (const Math::Quaternion& A, const Math::Quaternion& B) noexcept
		{
			Math::Quaternion nB;
			B.Inverse(nB);
			return A * nB;
		}
		float GetQuaternionX(Quaternion* source)
		{
			return source->x;
		}
		float GetQuaternionY(Quaternion* source)
		{
			return source->y;
		}
		float GetQuaternionZ(Quaternion* source)
		{
			return source->z;
		}
		float GetQuaternionW(Quaternion* source)
		{
			return source->w;
		}
		bool RotateQuaternionByAxisAngleC(Quaternion* input, Quaternion* result, float x, float y, float z, float radians)
		{
			if (x == 0 && y == 0 && z == 0)
			{
				return false;
			}

			Quaternion rotation(Vector3(x,y,z) , radians);
			*result = rotation * (*input);
			return true;
		}
	}

}

