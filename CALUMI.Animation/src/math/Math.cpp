//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include "math/Math.h"
#include <cmath>
#include "internalplatform.h"



namespace CALUMI
{
	namespace Math{

#pragma region VECTOR3

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

		struct Vector3::Impl
		{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;

			Impl() = default;
			Impl(float x, float y, float z) :x(x), y(y),z(z) {}
			Impl(double x, double y, double z) :x(static_cast<float>(x)), y(static_cast<float>(y)),z(static_cast<float>(z)) {}
			~Impl() = default;
		};

		float Vector3::getX() const
		{
			return pImpl->x;
		}
		float Vector3::getY() const
		{
			return pImpl->y;
		}
		float Vector3::getZ() const
		{
			return pImpl->z;
		}
		void Vector3::setX(float x)
		{
			pImpl->x = x;
		}
		void Vector3::setY(float y)
		{
			pImpl->y = y;
		}
		void Vector3::setZ(float z)
		{
			pImpl->z = z;
		}

		Vector3::Vector3()
		{
			pImpl = new Impl;
		}

		Vector3::~Vector3()
		{
			if (pImpl)
				delete pImpl;
		}

		Math::Vector3::Vector3(float input[3])
		{
			pImpl = new Impl(input[0], input[1], input[2]);
		}

		Vector3::Vector3(double input[3])
		{	
			pImpl = new Impl(input[0], input[1], input[2]);
		}

		Math::Vector3::Vector3(float input)
		{
			pImpl = new Impl(input, input, input);
		}

		Vector3::Vector3(double input)
		{
			pImpl = new Impl(input, input, input);
		}

		Vector3::Vector3(float x, float y, float z)
		{
			pImpl = new Impl(x, y, z);
		}

		Vector3::Vector3(double x, double y, double z)
		{
			pImpl = new Impl(x, y, z);
		}

		Vector3::Vector3(const Vector3& source)
		{
			pImpl = new Impl(source.pImpl->x, source.pImpl->y, source.pImpl->z);
		}

		Utilities::StringContainer Math::Vector3::ToString() const
		{
			Utilities::StringContainer output;
			output += std::format("x: {}, y: {}, z: {}", pImpl->x, pImpl->y, pImpl->z).c_str();
			return output;
		}

		bool Math::Vector3::operator==(const Math::Vector3& other) const
		{
			return pImpl->x == other.pImpl->x && pImpl->y == other.pImpl->y && pImpl->z == other.pImpl->z;
		}

		bool Math::Vector3::operator!=(const Math::Vector3& other) const
		{
			return !(pImpl->x == other.pImpl->x && pImpl->y == other.pImpl->y && pImpl->z == other.pImpl->z);
		}

		Math::Vector3& Math::Vector3::operator=(const Math::Vector3& other)
		{
			pImpl->x = other.pImpl->x;
			pImpl->y = other.pImpl->y;
			pImpl->z = other.pImpl->z;
			return *this;
		}

		Math::Vector3& Math::Vector3::operator+=(const Math::Vector3& other)
		{
			pImpl->x += other.pImpl->x;
			pImpl->y += other.pImpl->y;
			pImpl->z += other.pImpl->z;
			return *this;
		}

		Math::Vector3& Math::Vector3::operator-=(const Math::Vector3& other)
		{
			pImpl->x -= other.pImpl->x;
			pImpl->y -= other.pImpl->y;
			pImpl->z -= other.pImpl->z;
			return *this;
		}

		Math::Vector3& Math::Vector3::operator*=(const Math::Vector3& other)
		{
			pImpl->x *= other.pImpl->x;
			pImpl->y *= other.pImpl->y;
			pImpl->z *= other.pImpl->z;
			return *this;
		}

		Math::Vector3& Math::Vector3::operator*=(float other)
		{
			pImpl->x *= other;
			pImpl->y *= other;
			pImpl->z *= other;
			return *this;
		}

		Math::Vector3& Math::Vector3::operator/=(float other)
		{
			pImpl->x /= other;
			pImpl->y /= other;
			pImpl->z /= other;
			return *this;
		}

		Math::Vector3 Math::Vector3::operator+() const { return *this; }

		Math::Vector3 Math::Vector3::operator-() const { return Math::Vector3(-(pImpl->x), -(pImpl->y), -(pImpl->z)); }

		float Math::Vector3::Length() const
		{

			return sqrt((pImpl->x * pImpl->x) + (pImpl->y * pImpl->y) + (pImpl->z * pImpl->z));
		}
		float Math::Vector3::LengthSquared() const
		{
			return (pImpl->x * pImpl->x) + (pImpl->y * pImpl->y) + (pImpl->z * pImpl->z);
		}

		float Math::Vector3::Dot(const Math::Vector3& other) const
		{
			return (pImpl->x * other.pImpl->x) + (pImpl->y * other.pImpl->y) + (pImpl->z * other.pImpl->z);
		}

		void Math::Vector3::Cross(const Math::Vector3& other, Math::Vector3& result) const
		{
			result.pImpl->x = pImpl->y * other.pImpl->z - pImpl->z * other.pImpl->y;
			result.pImpl->y = pImpl->z * other.pImpl->x - pImpl->x * other.pImpl->z;
			result.pImpl->z = pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x;
		}

		Math::Vector3 Math::Vector3::Cross(const Math::Vector3& other) const
		{
			return Math::Vector3(pImpl->y * other.pImpl->z - pImpl->z * other.pImpl->y, pImpl->z * other.pImpl->x - pImpl->x * other.pImpl->z, pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x);
		}

		void Math::Vector3::Normalize()
		{
			*this /= this->Length();
		}

		double ToRadians(double degrees) noexcept { return degrees * (CLA_PI / 180.0); }

		double ToDegrees(double radians) noexcept { return radians * (180.0 / CLA_PI); }

		Math::Vector3 operator+ (const Math::Vector3& A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A.getX() + B.getX(), A.getY() + B.getY(), A.getZ() + B.getZ());
		}
		Math::Vector3 operator- (const Math::Vector3& A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A.getX() - B.getX(), A.getY() - B.getY(), A.getZ() - B.getZ());
		}
		Math::Vector3 operator* (const Math::Vector3& A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A.getX() * B.getX(), A.getY() * B.getY(), A.getZ() * B.getZ());
		}
		Math::Vector3 operator* (const Math::Vector3& A, float B) noexcept
		{
			return Math::Vector3(A.getX() * B, A.getY() * B, A.getZ() * B);
		}
		Math::Vector3 operator* (float A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A * B.getX(), A * B.getY(), A * B.getZ());
		}
		Math::Vector3 operator/ (const Math::Vector3& A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A.getX() / B.getX(), A.getY() / B.getY(), A.getZ() / B.getZ());
		}
		Math::Vector3 operator/ (const Math::Vector3& A, float B) noexcept
		{
			return Math::Vector3(A.getX() / B, A.getY() / B, A.getZ() / B);
		}
		Math::Vector3 CALUMI::Math::operator/(float A, const Math::Vector3& B) noexcept
		{
			return Math::Vector3(A / B.getX(), A / B.getY(), A / B.getZ());
		}

		float GetVector3X(Vector3* source)
		{
			return source->getX();
		}

		float GetVector3Y(Vector3* source)
		{
			return source->getY();
		}

		float GetVector3Z(Vector3* source)
		{
			return source->getZ();
		}

#pragma endregion

#pragma region VECTOR3D

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

		Utilities::StringContainer Math::Vector3D::ToString() const
		{
			Utilities::StringContainer output;
			output += std::format("x: {}, y: {}, z: {}",getX(), getY(), getZ()).c_str();
			return output;
		}

		struct Vector3D::Impl
		{
			double x = 0.0;
			double y = 0.0;
			double z = 0.0;
			Impl() = default;
			Impl(double x, double y, double z) : x(x), y(y), z(z) {}
		};

		double Vector3D::getX() const
		{
			return pImpl->x;
		}

		void Vector3D::setX(double x)
		{
			pImpl->x = x;
		}

		double Vector3D::getY() const
		{
			return pImpl->y;
		}

		void Vector3D::setY(double y)
		{
			pImpl->y = y;
		}

		double Vector3D::getZ() const
		{
			return pImpl->z;
		}

		void Vector3D::setZ(double z)
		{
			pImpl->z = z;
		}

		Vector3D::Vector3D()
		{
			pImpl = new Impl();
		}

		Vector3D::~Vector3D()
		{
			if (pImpl)
				delete pImpl;
		}

		Math::Vector3D::Vector3D(double input[3])
		{
			pImpl = new Impl(input[0], input[1], input[2]);
		}

		Math::Vector3D::Vector3D(double input)
		{
			pImpl = new Impl(input, input, input);
		}

		Math::Vector3D::Vector3D(Math::Vector3 input)
		{
			pImpl = new Impl(input.getX(), input.getY(), input.getZ());
		}

		Vector3D::Vector3D(const Vector3D& input)
		{
			pImpl = new Impl(input.pImpl->x, input.pImpl->y, input.pImpl->z);
		}

		Vector3D::Vector3D(double x, double y, double z)
		{
			pImpl = new Impl(x, y, z);
		}

		bool Math::Vector3D::operator==(const Math::Vector3D & other) const
		{
			return pImpl->x == other.pImpl->x && pImpl->y == other.pImpl->y && pImpl->z == other.pImpl->z;
		}

		bool Math::Vector3D::operator!=(const Math::Vector3D & other) const
		{
			return !(pImpl->x == other.pImpl->x && pImpl->y == other.pImpl->y && pImpl->z == other.pImpl->z);
		}

		Math::Vector3D& Math::Vector3D::operator=(const Math::Vector3D& other)
		{
			pImpl->x = other.pImpl->x;
			pImpl->y = other.pImpl->y;
			pImpl->z = other.pImpl->z;
			return *this;
		}

		Math::Vector3D& Math::Vector3D::operator+=(const Math::Vector3D & other)
		{
			pImpl->x += other.pImpl->x;
			pImpl->y += other.pImpl->y;
			pImpl->z += other.pImpl->z;
			return *this;
		}

		Math::Vector3D& Math::Vector3D::operator-=(const Math::Vector3D & other)
		{
			pImpl->x -= other.pImpl->x;
			pImpl->y -= other.pImpl->y;
			pImpl->z -= other.pImpl->z;
			return *this;
		}

		Math::Vector3D& Math::Vector3D::operator*=(const Math::Vector3D & other)
		{
			pImpl->x *= other.pImpl->x;
			pImpl->y *= other.pImpl->y;
			pImpl->z *= other.pImpl->z;
			return *this;
		}

		Math::Vector3D& Math::Vector3D::operator*=(double other)
		{
			pImpl->x *= other;
			pImpl->y *= other;
			pImpl->z *= other;
			return *this;
		}

		Math::Vector3D& Math::Vector3D::operator/=(double other)
		{
			pImpl->x /= other;
			pImpl->y /= other;
			pImpl->z /= other;
			return *this;
		}

		Math::Vector3D Math::Vector3D::operator+() const { return *this; };

		Math::Vector3D Math::Vector3D::operator-() const { return Math::Vector3D(-(pImpl->x), -(pImpl->y), -(pImpl->z)); };

		double Math::Vector3D::Length() const
		{

			return sqrt((pImpl->x * pImpl->x) + (pImpl->y * pImpl->y) + (pImpl->z * pImpl->z));
		}
		double Math::Vector3D::LengthSquared() const
		{
			return (pImpl->x * pImpl->x) + (pImpl->y * pImpl->y) + (pImpl->z * pImpl->z);
		}

		double Math::Vector3D::Dot(const Math::Vector3D & other) const
		{
			return (pImpl->x * other.pImpl->x) + (pImpl->y * other.pImpl->y) + (pImpl->z * other.pImpl->z);
		}

		void Math::Vector3D::Cross(const Math::Vector3D & other, Math::Vector3D & result) const
		{
			result.pImpl->x = pImpl->y * other.pImpl->z - pImpl->z * other.pImpl->y;
			result.pImpl->y = pImpl->z * other.pImpl->x - pImpl->x * other.pImpl->z;
			result.pImpl->z = pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x;
		}

		Math::Vector3D Math::Vector3D::Cross(const Math::Vector3D & other) const
		{
			return Math::Vector3D(pImpl->y * other.pImpl->z - pImpl->z * other.pImpl->y, pImpl->z * other.pImpl->x - pImpl->x * other.pImpl->z, pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x);
		}

		void Math::Vector3D::Normalize()
		{
			*this /= this->Length();
		}

		Vector3D Vector3D::Lerp(const Vector3D& input, double t) const
		{
			Vector3D output = (1.0-t)*(*this) + t * input;
			
			return output;
		}

		Math::Vector3D operator+ (const Math::Vector3D& A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A.getX() + B.getX(), A.getY() + B.getY(), A.getZ() + B.getZ());
		}
		Math::Vector3D operator- (const Math::Vector3D& A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A.getX() - B.getX(), A.getY() - B.getY(), A.getZ() - B.getZ());
		}
		Math::Vector3D operator* (const Math::Vector3D& A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A.getX() * B.getX(), A.getY() * B.getY(), A.getZ() * B.getZ());
		}
		Math::Vector3D operator* (const Math::Vector3D& A, double B) noexcept
		{
			return Math::Vector3D(A.getX() * B, A.getY() * B, A.getZ() * B);
		}
		Math::Vector3D operator* (double A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A * B.getX(), A * B.getY(), A * B.getZ());
		}
		Math::Vector3D operator/ (const Math::Vector3D& A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A.getX() / B.getX(), A.getY() / B.getY(), A.getZ() / B.getZ());
		}
		Math::Vector3D operator/ (const Math::Vector3D& A, double B) noexcept
		{
			return Math::Vector3D(A.getX() / B, A.getY() / B, A.getZ() / B);
		}
		Math::Vector3D CALUMI::Math::operator/(double A, const Math::Vector3D& B) noexcept
		{
			return Math::Vector3D(A / B.getX(), A / B.getY(), A / B.getZ());
		}

		double GetVector3DX(Vector3D* source)
		{
			return source->getX();
		}

		double GetVector3DY(Vector3D* source)
		{
			return source->getY();
		}

		double GetVector3DZ(Vector3D* source)
		{
			return source->getZ();
		}
#pragma endregion

#pragma region QUATERNION

		struct Quaternion::Impl
		{
			float x = 0.0;
			float y = 0.0;
			float z = 0.0;
			float w = 1.0;
			Impl() = default;
			Impl(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
			Impl(double x, double y, double z, double w) : x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z)), w(static_cast<float>(w)) {}
		};

		Quaternion::Quaternion()
		{
			pImpl = new Impl;
		}

		Quaternion::~Quaternion()
		{
			if (pImpl)
				delete pImpl;
		}

		//Math::Quaternions (float)
		Math::Quaternion::Quaternion(float input[4], bool normalize)
		{
			pImpl = new Impl(input[0], input[1], input[2], input[3]);
			if(normalize)
			Normalize();
		};

		Math::Quaternion::Quaternion(Vector3 direction, double radians, bool normalize)
		{
			direction.Normalize();
			float halfTheta = static_cast<float>(radians)/2.0f;
			float halfThetaSin = std::sin(halfTheta);
			float halfThetaCos = std::cos(halfTheta);
			
			pImpl = new Impl(	halfThetaSin * direction.getX(), 
								halfThetaSin * direction.getY(), 
								halfThetaSin * direction.getZ(), 
								halfThetaCos);
			
			if (normalize)
				Normalize();
		};

		Math::Quaternion::Quaternion(double x, double y, double z, double w, bool normalize)
		{ 
			pImpl = new Impl(x, y, z, w);
			if (normalize)
				Normalize();
		}

		Quaternion::Quaternion(float x, float y, float z, float w, bool normalize)
		{
			pImpl = new Impl(x, y, z, w);

			if (normalize)
				Normalize();
		}
		//**Warning**
		// https://www.andre-gaschler.com/rotationconverter/ is incorrect on euler conversion as of 3/24/2026
		// 
		// https://articulatedrobotics.xyz/tools/rotation-calculator/ has a more accurate conversion as of 3/24/2026
		//**Warning**
		Quaternion::Quaternion(float x, float y, float z, EulerOrder order)
		{
			const float cX = cosf(x * 0.5f), cY = cosf(y * 0.5f), cZ = cosf(z * 0.5f);
			const float sX = sinf(x * 0.5f), sY = sinf(y * 0.5f), sZ = sinf(z * 0.5f);
			float qx = 0.0f, qy = 0.0f, qz = 0.0f, qw = 1.0f;

			switch (order)
			{
				case CALUMI::Math::Quaternion::EulerOrder::XZY:
				{
					qx = sX * cY * cZ - sY * sZ * cX;
					qy = sZ * cX * cY - sX * sY * cZ;
					qz = sX * sZ * cY + sY * cX * cZ;
					qw = sX * sY * sZ + cX * cY * cZ;
					break;
				}
				case CALUMI::Math::Quaternion::EulerOrder::YXZ:
				{
					qx = sX * sZ * cY + sY * cX * cZ;
					qy = sX * cY * cZ - sY * sZ * cX;
					qz = sZ * cX * cY - sX * sY * cZ;
					qw = sX * sY * sZ + cX * cY * cZ;
					break;
				}
				case CALUMI::Math::Quaternion::EulerOrder::YZX:
				{
					qx = sX * sY * cZ + sZ * cX * cY;
					qy = sX * cY * cZ + sY * sZ * cX;
					qz = sY * cX * cZ - sX * sZ * cY;
					qw = cX * cY * cZ - sX * sY * sZ;
					break;
				}
				case CALUMI::Math::Quaternion::EulerOrder::ZXY:
				{
					qx = sY * cX * cZ - sX * sZ * cY;
					qy = sX * sY * cZ + sZ * cX * cY;
					qz = sX * cY * cZ + sY * sZ * cX;
					qw = cX * cY * cZ - sX * sY * sZ;
					break;
				}
				case CALUMI::Math::Quaternion::EulerOrder::ZYX:
				{
					qx = sZ * cX * cY - sX * sY * cZ;
					qy = sX * sZ * cY + sY * cX * cZ;
					qz = sX * cY * cZ - sY * sZ * cX;
					qw = sX * sY * sZ + cX * cY * cZ;
					break;
				}
				case CALUMI::Math::Quaternion::EulerOrder::XYX:
				{
					qx = sX* cY* cZ + sZ * cX * cY;
					qy = sX* sY* sZ + sY * cX * cZ;
					qz = sX* sY* cZ - sY * sZ * cX;
					qw = cX* cY* cZ - sX * sZ * cY;
					break;					
				}
				case CALUMI::Math::Quaternion::EulerOrder::XZX:
				{
					qx = sX* cY* cZ + sZ * cX * cY;
					qy = sY* sZ* cX - sX * sY * cZ;
					qz = sX* sY* sZ + sY * cX * cZ;
					qw = cX* cY* cZ - sX * sZ * cY;
					break;					
				}
				case CALUMI::Math::Quaternion::EulerOrder::YXY:
				{
					qx = sX* sY* sZ + sY * cX * cZ;
					qy = sX* cY* cZ + sZ * cX * cY;
					qz = sY* sZ* cX - sX * sY * cZ;
					qw = cX* cY* cZ - sX * sZ * cY;
					break;					
				}
				case CALUMI::Math::Quaternion::EulerOrder::YZY:
				{
					qx = sX* sY* cZ - sY * sZ * cX;
					qy = sX* cY* cZ + sZ * cX * cY;
					qz = sX* sY* sZ + sY * cX * cZ;
					qw = cX* cY* cZ - sX * sZ * cY;
					break;					
				}
				case CALUMI::Math::Quaternion::EulerOrder::ZXZ:
				{
					qx = sX * sY * sZ + sY * cX * cZ;
					qy = sX * sY * cZ - sY * sZ * cX;
					qz = sX * cY * cZ + sZ * cX * cY;
					qw = cX * cY * cZ - sX * sZ * cY;
					break;					
				}
				case CALUMI::Math::Quaternion::EulerOrder::ZYZ:
				{
					qx = sY * sZ * cX - sX * sY * cZ;
					qy = sX * sY * sZ + sY * cX * cZ;
					qz = sX * cY * cZ + sZ * cX * cY;
					qw = cX * cY * cZ - sX * sZ * cY;
					break;					
				}
				//XYZ
				default:
				{
					qx = sX* cY* cZ + sY * sZ * cX;
					qy = sY* cX* cZ - sX * sZ * cY;
					qz = sX* sY* cZ + sZ * cX * cY;
					qw = cX* cY* cZ - sX * sY * sZ;
					break;
				}
			}

			pImpl = new Impl(qx, qy, qz, qw);
		}

		Quaternion::Quaternion(const Quaternion& input)
		{
			pImpl = new Impl(input.pImpl->x, input.pImpl->y, input.pImpl->z, input.pImpl->w);
		}

		Math::Quaternion::Quaternion(Vector3D direction, double radians, bool normalize)
		{
			direction.Normalize();
			double halfTheta = radians / 2.0f;
			double halfThetaSin = std::sin(halfTheta);
			double halfThetaCos = std::cos(halfTheta);

			pImpl = new Impl(	halfThetaSin * direction.getX(),
								halfThetaSin * direction.getY(),
								halfThetaSin * direction.getZ(),
								halfThetaCos);
			
			if (normalize)
				Normalize();
		};

		float Quaternion::getX() const
		{
			return pImpl->x;
		}

		void Quaternion::setX(float x)
		{
			pImpl->x = x;
		}

		float Quaternion::getY() const
		{
			return pImpl->y;
		}

		void Quaternion::setY(float y)
		{
			pImpl->y = y;
		}

		float Quaternion::getZ() const
		{
			return pImpl->z;
		}

		void Quaternion::setZ(float z)
		{
			pImpl->z = z;
		}

		float Quaternion::getW() const
		{
			return pImpl->w;
		}

		void Quaternion::setW(float w)
		{
			pImpl->w = w;
		}

		Math::Quaternion Math::Quaternion::operator+() const noexcept
		{
			return *this;
		}

		Math::Quaternion Math::Quaternion::operator-() const noexcept
		{
			return Math::Quaternion(-(pImpl->x), -(pImpl->y), -(pImpl->z), -(pImpl->w));
		}

		bool Math::Quaternion::operator==(const Math::Quaternion& input) const noexcept
		{
			return (pImpl->x == input.pImpl->x && pImpl->y == input.pImpl->y && pImpl->z == input.pImpl->z && pImpl->w == input.pImpl->w);
		}

		bool Math::Quaternion::operator!=(const Math::Quaternion& input) const noexcept
		{
			return !(pImpl->x == input.pImpl->x && pImpl->y == input.pImpl->y && pImpl->z == input.pImpl->z && pImpl->w == input.pImpl->w);
		}

		bool Quaternion::AreSameRotation(const Quaternion& input, float tolerance) const noexcept
		{
			return (AreEqual(input,tolerance)|| AreEqual(-input, tolerance));
		}

		bool Quaternion::AreEqual(const Quaternion & input, float tolerance) const noexcept
		{
			float dx = abs(pImpl->x - input.pImpl->x);
			float dy = abs(pImpl->y - input.pImpl->y);
			float dz = abs(pImpl->z - input.pImpl->z);
			float dw = abs(pImpl->w - input.pImpl->w);
			return (dx < tolerance && dy < tolerance && dz < tolerance && dw < tolerance);
		}

		Math::Quaternion& Math::Quaternion::operator+= (const Math::Quaternion& other) noexcept
		{
			pImpl->x += other.pImpl->x;
			pImpl->y += other.pImpl->y;
			pImpl->z += other.pImpl->z;
			pImpl->w += other.pImpl->w;
			return *this;
		}

		Math::Quaternion& Math::Quaternion::operator-=(const Math::Quaternion& other) noexcept
		{
			pImpl->x -= other.pImpl->x;
			pImpl->y -= other.pImpl->y;
			pImpl->z -= other.pImpl->z;
			pImpl->w -= other.pImpl->w;
			return *this;
		}

		Math::Quaternion& Math::Quaternion::operator*=(const Math::Quaternion& other) noexcept
		{
			*this = *this * other;
			return *this;
		}

		Math::Quaternion& Math::Quaternion::operator*=(float other) noexcept
		{
			pImpl->x *= other;
			pImpl->y *= other;
			pImpl->z *= other;
			pImpl->w *= other;
			return *this;
		}

		Math::Quaternion& Math::Quaternion::operator=(const Math::Quaternion& other) noexcept
		{
			pImpl->x = other.pImpl->x;
			pImpl->y = other.pImpl->y;
			pImpl->z = other.pImpl->z;
			pImpl->w = other.pImpl->w;
			return *this;
		}
		Math::Quaternion& Math::Quaternion::operator/=(const Math::Quaternion& other) noexcept
		{
			pImpl->x /= other.pImpl->x;
			pImpl->y /= other.pImpl->y;
			pImpl->z /= other.pImpl->z;
			pImpl->w /= other.pImpl->w;
			return *this;
		}

		Quaternion Math::Quaternion::Conjugate() noexcept
		{
			return Quaternion(pImpl->x * -1, pImpl->y * -1, pImpl->z * -1, pImpl->w);
		}

		float Math::Quaternion::Dot(const Math::Quaternion& input) const noexcept
		{
			return (pImpl->x*input.pImpl->x + pImpl->y*input.pImpl->y + pImpl->z*input.pImpl->z + pImpl->w*input.pImpl->w);
		}

		void Math::Quaternion::Conjugate(Math::Quaternion& result) const noexcept
		{
			result.pImpl->x = -(pImpl->x);
			result.pImpl->y = -(pImpl->y);
			result.pImpl->z = -(pImpl->z);
			result.pImpl->w = pImpl->w;
		}

		float Math::Quaternion::LengthSquared() const noexcept
		{
			return (pImpl->x* pImpl->x + pImpl->y* pImpl->y + pImpl->z* pImpl->z + pImpl->w* pImpl->w);
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

			pImpl->x /= length;
			pImpl->y /= length;
			pImpl->z /= length;
			pImpl->w /= length;
		}

		void Math::Quaternion::Normalize(Math::Quaternion& result) const noexcept
		{
			float length = Length();
			if (length <= 0)
			{
				length = 1.0;
			}
			result.pImpl->x = pImpl->x / length;
			result.pImpl->y = pImpl->y / length;
			result.pImpl->z = pImpl->z / length;
			result.pImpl->w = pImpl->w / length;
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

			result.pImpl->x = conjugate.pImpl->x / length;
			result.pImpl->y = conjugate.pImpl->y / length;
			result.pImpl->z = conjugate.pImpl->z / length;
			result.pImpl->w = conjugate.pImpl->w / length;
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

			pImpl->x = conjugate.pImpl->x / length;
			pImpl->y = conjugate.pImpl->y / length;
			pImpl->z = conjugate.pImpl->z / length;
			pImpl->w = conjugate.pImpl->w / length;
		}

		float Quaternion::AngularDistance(const Quaternion& input) const
		{
			Quaternion inverse;
			Inverse(inverse);

			Quaternion diffQ = inverse * input;

			return 2 * acos(diffQ.getW());
		}

		Quaternion Quaternion::Slerp(const Quaternion& input, float t) const
		{
			float dot = Dot(input);

			if (abs(dot) >= 1.0f) return input;

			float hTheta = acos(dot);
			float sinhTheta = sqrtf(1.0f - dot * dot);

			float ratio1 = sin((1 - t) * hTheta) / sinhTheta;
			float ratio2 = sin(t * hTheta) / sinhTheta;

			Quaternion output(
				(getX() * ratio1 + input.getX() * ratio2),
				(getY() * ratio1 + input.getY() * ratio2),
				(getZ() * ratio1 + input.getZ() * ratio2),
				(getW() * ratio1 + input.getW() * ratio2)			
			);

			return output;
		}

		const Math::Quaternion Math::Quaternion::Identity = {0.0f,0.0f,0.0f,1.0f};

		Utilities::StringContainer Math::Quaternion::ToString()
		{
			Utilities::StringContainer output;
			output += std::format("x: {}, y: {}, z: {}, w: {}", pImpl->x, pImpl->y, pImpl->z, pImpl->w).c_str();
			return output;
		}

		Math::Quaternion operator+ (const Math::Quaternion& A, const Math::Quaternion& B) noexcept
		{
			return Math::Quaternion(A.getX() + B.getX(), A.getY() + B.getY(), A.getZ() + B.getZ(), A.getW() + B.getW() );
		}
		Math::Quaternion operator- (const Math::Quaternion& A, const Math::Quaternion& B) noexcept
		{
			return Math::Quaternion(A.getX() - B.getX(), A.getY() - B.getY(), A.getZ() - B.getZ(), A.getW() - B.getW());
		}
		Math::Quaternion operator* (const Math::Quaternion& A, const Math::Quaternion& B) noexcept
		{
			return Math::Quaternion(
				A.getW() * B.getX() + A.getX() * B.getW() + A.getY() * B.getZ() - A.getZ() * B.getY(),
				A.getW() * B.getY() - A.getX() * B.getZ() + A.getY() * B.getW() + A.getZ() * B.getX(),
				A.getW() * B.getZ() + A.getX() * B.getY() - A.getY() * B.getX() + A.getZ() * B.getW(),
				A.getW() * B.getW() - A.getX() * B.getX() - A.getY() * B.getY() - A.getZ() * B.getZ()
			);
		}
		Math::Quaternion operator* (const Math::Quaternion& A, float B) noexcept
		{
			return Math::Quaternion(A.getX() * B, A.getY() * B, A.getZ() * B, A.getW() * B);
		}
		Math::Quaternion operator* (float A, const Math::Quaternion& B) noexcept
		{
			return Math::Quaternion(B.getX() * A, B.getY() * A, B.getZ() * A, B.getW() * A);
		}
		Math::Quaternion operator/ (const Math::Quaternion& A, const Math::Quaternion& B) noexcept
		{
			Math::Quaternion nB;
			B.Inverse(nB);
			return A * nB;
		}
		float GetQuaternionX(Quaternion* source)
		{
			return source->getX();
		}
		float GetQuaternionY(Quaternion* source)
		{
			return source->getY();
		}
		float GetQuaternionZ(Quaternion* source)
		{
			return source->getZ();
		}
		float GetQuaternionW(Quaternion* source)
		{
			return source->getW();
		}
		bool RotateQuaternionByAxisAngleC(Quaternion* input, Quaternion* result, float x, float y, float z, float radians)
		{
			if (x == 0 && y == 0 && z == 0)
				return false;

			if (!input || !result)
				return false;

			Quaternion rotation(Vector3(x,y,z) , radians);
			*result = rotation * (*input);
			return true;
		}
#pragma endregion

#pragma region VECTOR2
		const Vector2 Vector2::Zero = 0.0;
		const Vector2 Vector2::One = 1.0;
		const Vector2 Vector2::UnitX = { 1.0,0.0 };
		const Vector2 Vector2::UnitY = { 0.0,1.0 };
		const Vector2 Vector2::Up = { 0.0,1.0 };
		const Vector2 Vector2::Down = { 0.0,-1.0 };
		const Vector2 Vector2::Right = { 1.0,0.0 };
		const Vector2 Vector2::Left = { -1.0,0.0 };

		struct Vector2::Impl
		{
			float x = 0.0f;
			float y = 0.0f;

			Impl() = default;
			Impl(float x, float y) :x(x), y(y) {}
			Impl(float value) :x(value), y(value) {}
		};

		Vector2::Vector2()
		{
			pImpl = new Impl;
		}
		Vector2::~Vector2()
		{
			if (pImpl)
				delete pImpl;
		}
		Vector2::Vector2(float input[2])
		{
			pImpl = new Impl(input[0],input[1]);
		}
		Vector2::Vector2(float input)
		{
			pImpl = new Impl(input);
		}
		Vector2::Vector2(float x, float y)
		{
			pImpl = new Impl(x, y);
		}
		Vector2::Vector2(const Vector2& input)
		{
			pImpl = new Impl(input.getX(), input.getY());
		}
		float Vector2::getX() const
		{
			return pImpl->x;
		}
		void Vector2::setX(float x)
		{
			pImpl->x = x;
		}
		float Vector2::getY() const
		{
			return pImpl->y;
		}
		void Vector2::setY(float y)
		{
			pImpl->y = y;
		}
		bool Vector2::operator==(const Vector2& other) const
		{
			return pImpl->x == other.pImpl->x && pImpl->y == other.pImpl->y;
		}
		bool Vector2::operator!=(const Vector2& other) const
		{
			return pImpl->x != other.pImpl->x || pImpl->y != other.pImpl->y;
		}
		Vector2& Vector2::operator=(const Vector2& other)
		{
			pImpl->x = other.pImpl->x;
			pImpl->y = other.pImpl->y;
			return *this;
		}
		Vector2& Vector2::operator+=(const Vector2& other)
		{
			pImpl->x += other.pImpl->x;
			pImpl->y += other.pImpl->y;
			return *this;
		}
		Vector2& Vector2::operator-=(const Vector2& other)
		{
			pImpl->x -= other.pImpl->x;
			pImpl->y -= other.pImpl->y;
			return *this;
		}
		Vector2& Vector2::operator*=(float other)
		{
			pImpl->x *= other;
			pImpl->y *= other;
			return *this;
		}
		Vector2& Vector2::operator*=(const Vector2& other)
		{
			pImpl->x *= other.pImpl->x;
			pImpl->y *= other.pImpl->y;
			return *this;
		}
		Vector2& Vector2::operator/=(float other)
		{
			pImpl->x /= other;
			pImpl->y /= other;
			return *this;
		}

		Vector2 Vector2::operator+() const
		{
			return *this;
		}
		Vector2 Vector2::operator-() const
		{
			return Math::Vector2(-(pImpl->x),-(pImpl->y));
		}

		float Vector2::Length() const 
		{
			return sqrt((pImpl->x * pImpl->x) + (pImpl->y * pImpl->y));
		}
		float Vector2::LengthSquared() const
		{
			return ((pImpl->x * pImpl->x) + (pImpl->y * pImpl->y));
		}

		float Vector2::Dot(const Vector2& other) const
		{
			return (pImpl->x * other.pImpl->x) + (pImpl->y * other.pImpl->y);
		}

		void Vector2::Cross(const Vector2& other, Vector2& result) const
		{
			result.pImpl->x = result.pImpl->y = (pImpl->x * other.pImpl->y) - (pImpl->y * other.pImpl->x);
		}
		Vector2 Vector2::Cross(const Vector2& other) const
		{
			float result = (pImpl->x * other.pImpl->y) - (pImpl->y * other.pImpl->x);
			return Vector2(result, result);
		}

		void Vector2::Normalize()
		{
			*this /= this->Length();
		}
		Utilities::StringContainer Vector2::ToString() const
		{
			Utilities::StringContainer output;
			output += std::format("x: {}, y: {}", pImpl->x, pImpl->y).c_str();
			return output;
		}
		//Non Member Functions
		Math::Vector2 operator+(const Vector2& A, const Vector2& B) noexcept
		{
			return Math::Vector2(A.getX() + B.getX(), A.getY() + B.getY());
		}
		Math::Vector2 operator-(const Vector2& A, const Vector2& B) noexcept
		{
			return Math::Vector2(A.getX() -B.getX(),A.getY() -B.getY());
		}
		Math::Vector2 operator*(const Vector2& A, const Vector2& B) noexcept
		{
			return Math::Vector2(A.getX() *B.getX(),A.getY() *B.getY());
		}
		Math::Vector2 operator*(const Vector2& A, float B) noexcept
		{
			return Math::Vector2(A.getX()*B,A.getY() *B);
		}
		Math::Vector2 operator*(float A, const Vector2& B) noexcept
		{
			return Math::Vector2(A*B.getX(),A*B.getY());
		}
		Math::Vector2 operator/(const Vector2& A, const Vector2& B) noexcept
		{
			return Math::Vector2(A.getX()/B.getX(),A.getY() /B.getY());
		}
		Math::Vector2 operator/(const Vector2& A, float B) noexcept
		{
			return Math::Vector2(A.getX()/B,A.getY() /B);
		}
		Math::Vector2 operator/(float A, const Vector2& B) noexcept
		{
			return Math::Vector2(A/B.getX(),A/B.getY());
		}
		float GetVector2X(Vector2* source)
		{
			return source->getX();
		}
		float GetVector2Y(Vector2* source)
		{
			return source->getY();
		}
#pragma endregion

#pragma region VECTOR2D
		const Vector2D Vector2D::Zero = 0.0;
		const Vector2D Vector2D::One = 1.0;
		const Vector2D Vector2D::UnitX = { 1.0,0.0 };
		const Vector2D Vector2D::UnitY = { 0.0,1.0 };
		const Vector2D Vector2D::Up = { 0.0,1.0 };
		const Vector2D Vector2D::Down = { 0.0,-1.0 };
		const Vector2D Vector2D::Right = { 1.0,0.0 };
		const Vector2D Vector2D::Left = { -1.0,0.0 };

		struct Vector2D::Impl
		{
			double x = 0.0;
			double y = 0.0;
			Impl() = default;
			Impl(double x, double y) :x(x), y(y) {}
			Impl(double value) :x(value), y(value) {}
		};

		Vector2D::Vector2D()
		{
			pImpl = new Impl;
		}

		Vector2D::~Vector2D()
		{
			if (pImpl)
				delete pImpl;
		}

		Vector2D::Vector2D(double input[2])
		{
			pImpl = new Impl(input[0],input[1]);
		}
		Vector2D::Vector2D(double input)
		{
			pImpl = new Impl(input);
		}
		Vector2D::Vector2D(Vector2 input)
		{
			pImpl = new Impl(static_cast<double>(input.getX()), static_cast<double>(input.getY()));
		}
		Vector2D::Vector2D(const Vector2D& input)
		{
			pImpl = new Impl(input.getX(), input.getY());
		}
		Vector2D::Vector2D(double x, double y)
		{
			pImpl = new Impl(x, y);
		}
		double Vector2D::getX() const
		{
			return pImpl->x;
		}
		void Vector2D::setX(double x)
		{
			pImpl->x = x;
		}
		double Vector2D::getY() const
		{
			return pImpl->y;
		}
		void Vector2D::setY(double y)
		{
			pImpl->y = y;
		}
		bool Vector2D::operator==(const Vector2D& other) const
		{
			return pImpl->x == other.pImpl->x && pImpl->y == other.pImpl->y;
		}
		bool Vector2D::operator!=(const Vector2D& other) const
		{
			return pImpl->x != other.pImpl->x || pImpl->y != other.pImpl->y;
		}

		Vector2D& Vector2D::operator=(const Vector2D& other)
		{
			pImpl->x = other.pImpl->x;
			pImpl->y = other.pImpl->y;
			return *this;
		}

		Vector2D& Vector2D::operator+=(const Vector2D& other)
		{
			pImpl->x += other.pImpl->x;
			pImpl->y += other.pImpl->y;
			return *this;
		}
		Vector2D& Vector2D::operator-=(const Vector2D& other)
		{
			pImpl->x -= other.pImpl->x;
			pImpl->y -= other.pImpl->y;
			return *this;
		}
		Vector2D& Vector2D::operator*=(double other)
		{
			pImpl->x *= other;
			pImpl->y *= other;
			return *this;
		}
		Vector2D& Vector2D::operator*=(const Vector2D& other)
		{
			pImpl->x *= other.pImpl->x;
			pImpl->y *= other.pImpl->y;
			return *this;
		}
		Vector2D& Vector2D::operator/=(double other)
		{
			pImpl->x /= other;
			pImpl->y /= other;
			return *this;
		}

		Vector2D Vector2D::operator+() const
		{
			return *this;
		}
		Vector2D Vector2D::operator-() const
		{
			return Math::Vector2D(-(pImpl->x), -(pImpl->y));
		}

		double Vector2D::Length() const
		{
			return sqrt((pImpl->x * pImpl->x) + (pImpl->y * pImpl->y));
		}
		double Vector2D::LengthSquared() const
		{
			return ((pImpl->x * pImpl->x) + (pImpl->y * pImpl->y));
		}

		double Vector2D::Dot(const Vector2D& other) const
		{
			return (pImpl->x * other.pImpl->x) + (pImpl->y * other.pImpl->y);
		}

		void Vector2D::Cross(const Vector2D& other, Vector2D& result) const
		{
			result.pImpl->x = result.pImpl->y = (pImpl->x * other.pImpl->y) - (pImpl->y * other.pImpl->x);
		}
		Vector2D Vector2D::Cross(const Vector2D& other) const
		{
			double result = (pImpl->x * other.pImpl->y) - (pImpl->y * other.pImpl->x);
			return Vector2D(result, result);
		}

		void Vector2D::Normalize()
		{
			*this /= this->Length();
		}
		Vector2D Vector2D::Lerp(const Vector2D& input, double t) const
		{
			Vector2D output = (1.0 - t) * (*this) + t * input;

			return output;
		}
		Utilities::StringContainer Vector2D::ToString() const
		{
			Utilities::StringContainer output;
			output += std::format("x: {}, y: {}", pImpl->x, pImpl->y).c_str();
			return output;
		}
		//Non member functions
		Math::Vector2D operator+(const Vector2D& A, const Vector2D& B) noexcept
		{
			return Math::Vector2D(A.getX() + B.getX(), A.getY() + B.getY());
		}
		Math::Vector2D operator-(const Vector2D& A, const Vector2D& B) noexcept
		{
			return Math::Vector2D(A.getX() - B.getX(), A.getY() - B.getY());
		}
		Math::Vector2D operator*(const Vector2D& A, const Vector2D& B) noexcept
		{
			return Math::Vector2D(A.getX() * B.getX(), A.getY() * B.getY());
		}
		Math::Vector2D operator*(const Vector2D& A, double B) noexcept
		{
			return Math::Vector2D(A.getX() * B, A.getY() * B);
		}
		Math::Vector2D operator*(double A, const Vector2D& B) noexcept
		{
			return Math::Vector2D(A * B.getX(), A * B.getY());
		}
		Math::Vector2D operator/(const Vector2D& A, const Vector2D& B) noexcept
		{
			return Math::Vector2D(A.getX() / B.getX(), A.getY() / B.getY());
		}
		Math::Vector2D operator/(const Vector2D& A, double B) noexcept
		{
			return Math::Vector2D(A.getX() / B, A.getY() / B);
		}
		Math::Vector2D operator/(double A, const Vector2D& B) noexcept
		{
			return Math::Vector2D(A / B.getX(), A / B.getY());
		}
		double GetVector2DX(Vector2D* source)
		{
			return source->getX();
		}
		double GetVector2DY(Vector2D* source)
		{
			return source->getY();
		}
#pragma endregion

}

}

