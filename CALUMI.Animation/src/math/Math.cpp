//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


// ReSharper disable CppPassValueParameterByConstReference
// ReSharper disable CppMemberFunctionMayBeConst
// ReSharper disable CppParameterMayBeConstPtrOrRef
// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppDFANullDereference
#include "math/Math.h"
#include <cmath>
#include "internalplatform.h"



namespace CALUMI::Math
{

#pragma region VECTOR3

	const Vector3 Vector3::Zero = Vector3(0.0f);
	const Vector3 Vector3::One = Vector3(1.0f);
	const Vector3 Vector3::UnitX = { 1.0, 0.0, 0.0 };
	const Vector3 Vector3::UnitY = { 0.0, 1.0, 0.0 };
	const Vector3 Vector3::UnitZ = { 0.0, 0.0, 1.0 };

	const Vector3 Vector3::Up = { 0.0, 0.0, 1.0 };
	const Vector3 Vector3::Down = { 0.0, 0.0, -1.0 };
	const Vector3 Vector3::Right = { -1.0, 0.0, 0.0 };
	const Vector3 Vector3::Left = { 1.0, 0.0, 0.0 };
	const Vector3 Vector3::Forward = { 0.0, 1.0, 0.0 };
	const Vector3 Vector3::Back = { 0.0, -1.0, 0.0 };

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

	float Vector3::x() const
	{
		return pImpl->x;
	}
	float Vector3::y() const
	{
		return pImpl->y;
	}
	float Vector3::z() const
	{
		return pImpl->z;
	}
	void Vector3::setX(const float x) const
	{
		pImpl->x = x;
	}
	void Vector3::setY(const float y) const
	{
		pImpl->y = y;
	}
	void Vector3::setZ(const float z) const
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
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}

	Vector3::Vector3(float input[3])
	{
		pImpl = new Impl(input[0], input[1], input[2]);
	}

	Vector3::Vector3(double input[3])
	{
		pImpl = new Impl(input[0], input[1], input[2]);
	}

	Vector3::Vector3(float input)
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

	Utilities::StringContainer Vector3::toString() const
	{
		Utilities::StringContainer output;
		output += std::format("x: {}, y: {}, z: {}", pImpl->x, pImpl->y, pImpl->z).c_str();
		return output;
	}

	bool Vector3::operator==(const Vector3& other) const
	{
		return pImpl->x == other.pImpl->x && pImpl->y == other.pImpl->y && pImpl->z == other.pImpl->z;
	}

	bool Vector3::operator!=(const Vector3& other) const
	{
		return !(pImpl->x == other.pImpl->x && pImpl->y == other.pImpl->y && pImpl->z == other.pImpl->z);
	}

	Vector3& Vector3::operator=(const Vector3& other)
	{
		if (this != &other)
		{
			pImpl->x = other.pImpl->x;
			pImpl->y = other.pImpl->y;
			pImpl->z = other.pImpl->z;
		}
		return *this;
	}

	Vector3& Vector3::operator+=(const Vector3& other)
	{
		pImpl->x += other.pImpl->x;
		pImpl->y += other.pImpl->y;
		pImpl->z += other.pImpl->z;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& other)
	{
		pImpl->x -= other.pImpl->x;
		pImpl->y -= other.pImpl->y;
		pImpl->z -= other.pImpl->z;
		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& other)
	{
		pImpl->x *= other.pImpl->x;
		pImpl->y *= other.pImpl->y;
		pImpl->z *= other.pImpl->z;
		return *this;
	}

	Vector3& Vector3::operator*=(float other)
	{
		pImpl->x *= other;
		pImpl->y *= other;
		pImpl->z *= other;
		return *this;
	}

	Vector3& Vector3::operator/=(float other)
	{
		pImpl->x /= other;
		pImpl->y /= other;
		pImpl->z /= other;
		return *this;
	}

	Vector3 Vector3::operator+() const { return *this; }

	Vector3 Vector3::operator-() const { return {-pImpl->x, -pImpl->y, -pImpl->z}; }

	float Vector3::length() const
	{

		return sqrt(pImpl->x * pImpl->x + pImpl->y * pImpl->y + pImpl->z * pImpl->z);
	}
	float Vector3::lengthSquared() const
	{
		return pImpl->x * pImpl->x + pImpl->y * pImpl->y + pImpl->z * pImpl->z;
	}
	bool Vector3::areEqual(const Vector3& input, float tolerance) const noexcept
	{
		return std::abs(pImpl->x - input.pImpl->x) < tolerance &&
			   std::abs(pImpl->y - input.pImpl->y) < tolerance &&
			   std::abs(pImpl->z - input.pImpl->z) < tolerance;
	}

	float Vector3::dot(const Vector3& other) const
	{
		return pImpl->x * other.pImpl->x + pImpl->y * other.pImpl->y + pImpl->z * other.pImpl->z;
	}

	void Vector3::cross(const Vector3& other, Vector3& result) const
	{
		result.pImpl->x = pImpl->y * other.pImpl->z - pImpl->z * other.pImpl->y;
		result.pImpl->y = pImpl->z * other.pImpl->x - pImpl->x * other.pImpl->z;
		result.pImpl->z = pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x;
	}

	Vector3 Vector3::cross(const Vector3& other) const
	{
		return {pImpl->y * other.pImpl->z - pImpl->z * other.pImpl->y, pImpl->z * other.pImpl->x - pImpl->x * other.pImpl->z, pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x};
	}

	void Vector3::normalize()
	{
		*this /= this->length();
	}

	double ToRadians(double degrees) noexcept { return degrees * (CLA_PI / 180.0); }

	double ToDegrees(double radians) noexcept { return radians * (180.0 / CLA_PI); }

	Vector3 operator+ (const Vector3& A, const Vector3& B) noexcept
	{
		return {A.x() + B.x(), A.y() + B.y(), A.z() + B.z()};
	}
	Vector3 operator- (const Vector3& A, const Vector3& B) noexcept
	{
		return {A.x() - B.x(), A.y() - B.y(), A.z() - B.z()};
	}
	Vector3 operator* (const Vector3& A, const Vector3& B) noexcept
	{
		return {A.x() * B.x(), A.y() * B.y(), A.z() * B.z()};
	}
	Vector3 operator* (const Vector3& A, float B) noexcept
	{
		return {A.x() * B, A.y() * B, A.z() * B};
	}
	Vector3 operator* (float A, const Vector3& B) noexcept
	{
		return {A * B.x(), A * B.y(), A * B.z()};
	}
	Vector3 operator/ (const Vector3& A, const Vector3& B) noexcept
	{
		return {A.x() / B.x(), A.y() / B.y(), A.z() / B.z()};
	}
	Vector3 operator/ (const Vector3& A, float B) noexcept
	{
		return {A.x() / B, A.y() / B, A.z() / B};
	}
	Vector3 operator/(float A, const Vector3& B) noexcept
	{
		return {A / B.x(), A / B.y(), A / B.z()};
	}

	Vector3* CreateVector3C(float x, float y, float z)
	{
		return new Vector3(x, y, z);
	}
	float GetVector3XC(Vector3* source)
	{
		return source->x();
	}

	float GetVector3YC(Vector3* source)
	{
		return source->y();
	}

	float GetVector3ZC(Vector3* source)
	{
		return source->z();
	}
	int DeleteVector3C(Vector3* ptr)
	{
		if (!ptr)
			return -1;

		try
		{
			delete ptr;
			return 0;
		}
		catch (std::bad_alloc&){}

		return -1;
	}

#pragma endregion

#pragma region VECTOR3D

	//Math::Vector3D (double)
	const Vector3D Vector3D::Zero = Vector3D(0.0);
	const Vector3D Vector3D::One = Vector3D(1.0);
	const Vector3D Vector3D::UnitX = { 1.0, 0.0, 0.0};
	const Vector3D Vector3D::UnitY = { 0.0, 1.0, 0.0};
	const Vector3D Vector3D::UnitZ = { 0.0, 0.0, 1.0};

	const Vector3D Vector3D::Up = { 0.0, 0.0, 1.0}; //In coordinate system found on Nifskope. May need modifying
	const Vector3D Vector3D::Down = { 0.0, 0.0, -1.0};
	const Vector3D Vector3D::Right = { -1.0, 0.0, 0.0};
	const Vector3D Vector3D::Left = { 1.0, 0.0, 0.0};
	const Vector3D Vector3D::Forward = { 0.0, 1.0, 0.0};
	const Vector3D Vector3D::Back = { 0.0, -1.0, 0.0};

	Utilities::StringContainer Vector3D::toString() const
	{
		Utilities::StringContainer output;
		output += std::format("x: {}, y: {}, z: {}",x(), y(), z()).c_str();
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

	double Vector3D::x() const
	{
		return pImpl->x;
	}

	void Vector3D::setX(double x)
	{
		pImpl->x = x;
	}

	double Vector3D::y() const
	{
		return pImpl->y;
	}

	void Vector3D::setY(double y)
	{
		pImpl->y = y;
	}

	double Vector3D::z() const
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
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}

	Vector3D::Vector3D(double input[3])
	{
		pImpl = new Impl(input[0], input[1], input[2]);
	}

	Vector3D::Vector3D(double input)
	{
		pImpl = new Impl(input, input, input);
	}

	Vector3D::Vector3D(Vector3 input)
	{
		pImpl = new Impl(input.x(), input.y(), input.z());
	}

	Vector3D::Vector3D(const Vector3D& input)
	{
		pImpl = new Impl(input.pImpl->x, input.pImpl->y, input.pImpl->z);
	}

	Vector3D::Vector3D(double x, double y, double z)
	{
		pImpl = new Impl(x, y, z);
	}

	bool Vector3D::operator==(const Vector3D & other) const
	{
		return pImpl->x == other.pImpl->x && pImpl->y == other.pImpl->y && pImpl->z == other.pImpl->z;
	}

	bool Vector3D::operator!=(const Vector3D & other) const
	{
		return !(pImpl->x == other.pImpl->x && pImpl->y == other.pImpl->y && pImpl->z == other.pImpl->z);
	}

	Vector3D& Vector3D::operator=(const Vector3D& other)
	{
		if (this != &other)
		{
			pImpl->x = other.pImpl->x;
			pImpl->y = other.pImpl->y;
			pImpl->z = other.pImpl->z;
		}
		return *this;
	}

	Vector3D& Vector3D::operator+=(const Vector3D & other)
	{
		pImpl->x += other.pImpl->x;
		pImpl->y += other.pImpl->y;
		pImpl->z += other.pImpl->z;
		return *this;
	}

	Vector3D& Vector3D::operator-=(const Vector3D & other)
	{
		pImpl->x -= other.pImpl->x;
		pImpl->y -= other.pImpl->y;
		pImpl->z -= other.pImpl->z;
		return *this;
	}

	Vector3D& Vector3D::operator*=(const Vector3D & other)
	{
		pImpl->x *= other.pImpl->x;
		pImpl->y *= other.pImpl->y;
		pImpl->z *= other.pImpl->z;
		return *this;
	}

	Vector3D& Vector3D::operator*=(double other)
	{
		pImpl->x *= other;
		pImpl->y *= other;
		pImpl->z *= other;
		return *this;
	}

	Vector3D& Vector3D::operator/=(double other)
	{
		pImpl->x /= other;
		pImpl->y /= other;
		pImpl->z /= other;
		return *this;
	}

	Vector3D Vector3D::operator+() const { return *this; }

	Vector3D Vector3D::operator-() const { return {-pImpl->x, -pImpl->y, -pImpl->z}; }

	double Vector3D::length() const
	{

		return sqrt(pImpl->x * pImpl->x + pImpl->y * pImpl->y + pImpl->z * pImpl->z);
	}
	double Vector3D::lengthSquared() const
	{
		return pImpl->x * pImpl->x + pImpl->y * pImpl->y + pImpl->z * pImpl->z;
	}

	bool Vector3D::areEqual(const Vector3D& input, float tolerance) const noexcept
	{
		return std::abs(pImpl->x - input.pImpl->x) < tolerance &&
			   std::abs(pImpl->y - input.pImpl->y) < tolerance &&
			   std::abs(pImpl->z - input.pImpl->z) < tolerance;
	}

	double Vector3D::dot(const Vector3D & other) const
	{
		return pImpl->x * other.pImpl->x + pImpl->y * other.pImpl->y + pImpl->z * other.pImpl->z;
	}

	void Vector3D::cross(const Vector3D & other, Vector3D & result) const
	{
		result.pImpl->x = pImpl->y * other.pImpl->z - pImpl->z * other.pImpl->y;
		result.pImpl->y = pImpl->z * other.pImpl->x - pImpl->x * other.pImpl->z;
		result.pImpl->z = pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x;
	}

	Vector3D Vector3D::cross(const Vector3D & other) const
	{
		return {pImpl->y * other.pImpl->z - pImpl->z * other.pImpl->y, pImpl->z * other.pImpl->x - pImpl->x * other.pImpl->z, pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x};
	}

	void Vector3D::normalize()
	{
		*this /= this->length();
	}

	Vector3D Vector3D::lerp(const Vector3D& input, double t) const
	{
		Vector3D output = (1.0-t) * *this + t * input;

		return output;
	}

	Vector3D operator+ (const Vector3D& A, const Vector3D& B) noexcept
	{
		return {A.x() + B.x(), A.y() + B.y(), A.z() + B.z()};
	}
	Vector3D operator- (const Vector3D& A, const Vector3D& B) noexcept
	{
		return {A.x() - B.x(), A.y() - B.y(), A.z() - B.z()};
	}
	Vector3D operator* (const Vector3D& A, const Vector3D& B) noexcept
	{
		return {A.x() * B.x(), A.y() * B.y(), A.z() * B.z()};
	}
	Vector3D operator* (const Vector3D& A, double B) noexcept
	{
		return {A.x() * B, A.y() * B, A.z() * B};
	}
	Vector3D operator* (double A, const Vector3D& B) noexcept
	{
		return {A * B.x(), A * B.y(), A * B.z()};
	}
	Vector3D operator/ (const Vector3D& A, const Vector3D& B) noexcept
	{
		return {A.x() / B.x(), A.y() / B.y(), A.z() / B.z()};
	}
	Vector3D operator/ (const Vector3D& A, double B) noexcept
	{
		return {A.x() / B, A.y() / B, A.z() / B};
	}
	Vector3D operator/(double A, const Vector3D& B) noexcept
	{
		return {A / B.x(), A / B.y(), A / B.z()};
	}

	Vector3D* CreateVector3DC(double x, double y, double z)
	{
		return new Vector3D(x, y, z);
	}
	double GetVector3DXC(Vector3D* source)
	{
		return source->x();
	}

	double GetVector3DYC(Vector3D* source)
	{
		return source->y();
	}

	double GetVector3DZC(Vector3D* source)
	{
		return source->z();
	}
	int DeleteVector3DC(Vector3D* ptr)
	{
		if (!ptr)
			return -1;

		try
		{
			delete ptr;
			return 0;
		}
		catch (std::bad_alloc&){}

		return -1;
	}
#pragma endregion

#pragma region EULER_DEFINITIONS
	struct EulerDefinition::Impl
	{
		float first{}, second{}, third{};
		EulerOrder order = EulerOrder::Default;
	};

	EulerDefinition::EulerDefinition(float first, float second, float third, EulerOrder euler) : pImpl(new Impl)
	{
		pImpl->first = first;
		pImpl->second = second;
		pImpl->third = third;
		pImpl->order = euler;
	}

	EulerDefinition::EulerDefinition(const EulerDefinition& other) : pImpl(new Impl)
	{
		*this = other;
	}

	EulerDefinition::~EulerDefinition()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}

	EulerDefinition& EulerDefinition::operator=(const EulerDefinition& other)
	{
		if (this != &other)
		{
			pImpl->first = other.pImpl->first;
			pImpl->second = other.pImpl->second;
			pImpl->third = other.pImpl->third;
			pImpl->order = other.pImpl->order;
		}
		return *this;
	}

	float EulerDefinition::first() const
	{
		return pImpl->first;
	}

	float EulerDefinition::second() const
	{
		return pImpl->second;
	}

	float EulerDefinition::third(bool safe) const
	{
		if (!safe)
			return pImpl->third;

		auto singularity = 0.0f;

		switch (pImpl->order)
		{
		case EulerOrder::XYZ:
		case EulerOrder::XZY:
		case EulerOrder::YXZ:
		case EulerOrder::YZX:
		case EulerOrder::ZXY:
		case EulerOrder::ZYX:
			singularity = static_cast<float>(ToRadians(90.0));
			break;
		case EulerOrder::XYX:
		case EulerOrder::XZX:
		case EulerOrder::YXY:
		case EulerOrder::YZY:
		case EulerOrder::ZXZ:
		case EulerOrder::ZYZ:
			singularity = static_cast<float>(ToRadians(180.0));
			break;
		}

		if (std::abs(std::abs(pImpl->second) - singularity) < 0.000001f)
			return 0.0f;

		return pImpl->third;
	}

	EulerDefinition::EulerOrder EulerDefinition::order() const
	{
		return pImpl->order;
	}

	void EulerDefinition::setFirst(float first)
	{
		pImpl->first = first;
	}

	void EulerDefinition::setSecond(float second)
	{
		pImpl->second = second;
	}

	void EulerDefinition::setThird(float third)
	{
		pImpl->third = third;
	}

	void EulerDefinition::setOrder(EulerOrder order)
	{
		pImpl->order = order;
	}

	int EulerDefinition::toInt(EulerOrder order)
	{
		return static_cast<int>(order);
	}

	EulerDefinition::EulerOrder EulerDefinition::GetEulerOrder(int order)
	{
		if (order < 0 || order >= static_cast<int>(EulerOrder::Max))
			order = 0;

		return static_cast<EulerOrder>(order);
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
		Impl(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
		{
			//Prevent -0.0f inputs
			if (this->x == -0.0f)
				this->x = 0.0f;
			if (this->y == -0.0f)
				this->y = 0.0f;
			if (this->z == -0.0f)
				this->z = 0.0f;
			if (this->w == -0.0f)
				this->w = 0.0f;

			//Ensure total non-zero
			if (x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f)
				w = 1.0f;
		}
		Impl(double x, double y, double z, double w) : Impl(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w)) {}
	};

	Quaternion::Quaternion()
	{
		pImpl = new Impl;
	}

	Quaternion::~Quaternion()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}

	//Math::Quaternions (float)
	Quaternion::Quaternion(float input[4], bool normalized)
	{
		pImpl = new Impl(input[0], input[1], input[2], input[3]);
		if(normalized)
			normalize();
	}

	Quaternion::Quaternion(Vector3 direction, double radians, bool normalized)
	{
		direction.normalize();
		const float halfTheta = static_cast<float>(radians)/2.0f;
		const float halfThetaSin = std::sin(halfTheta);
		const float halfThetaCos = std::cos(halfTheta);

		pImpl = new Impl(	halfThetaSin * direction.x(),
			                 halfThetaSin * direction.y(),
			                 halfThetaSin * direction.z(),
			                 halfThetaCos);

		if (normalized)
			normalize();
	}

	Quaternion::Quaternion(double x, double y, double z, double w, bool normalized)
	{
		pImpl = new Impl(x, y, z, w);
		if (normalized)
			normalize();
	}

	Quaternion::Quaternion(float x, float y, float z, float w, bool normalized)
	{
		pImpl = new Impl(x, y, z, w);

		if (normalized)
			normalize();
	}

	Quaternion::Quaternion(EulerDefinition eulerInput) : Quaternion(eulerInput.first(),
																	eulerInput.second(),
																	eulerInput.third(true),
																	eulerInput.order())	{}

	//**Warning**
	// https://www.andre-gaschler.com/rotationconverter/ is incorrect on euler conversion as of 3/24/2026
	//
	// https://articulatedrobotics.xyz/tools/rotation-calculator/ has a more accurate conversion as of 3/24/2026
	//**Warning**
	Quaternion::Quaternion(float x, float y, float z, EulerDefinition::EulerOrder order)
	{
		const float cX = cosf(x * 0.5f), cY = cosf(y * 0.5f), cZ = cosf(z * 0.5f);
		const float sX = sinf(x * 0.5f), sY = sinf(y * 0.5f), sZ = sinf(z * 0.5f);
		float qx = 0.0f, qy = 0.0f, qz = 0.0f, qw = 1.0f;

		switch (order)
		{
		case EulerDefinition::EulerOrder::XZY:
			{
				qx = sX * cY * cZ - sY * sZ * cX;
				qy = sZ * cX * cY - sX * sY * cZ;
				qz = sX * sZ * cY + sY * cX * cZ;
				qw = sX * sY * sZ + cX * cY * cZ;
				break;
			}
		case EulerDefinition::EulerOrder::YXZ:
			{
				qx = sX * sZ * cY + sY * cX * cZ;
				qy = sX * cY * cZ - sY * sZ * cX;
				qz = sZ * cX * cY - sX * sY * cZ;
				qw = sX * sY * sZ + cX * cY * cZ;
				break;
			}
		case EulerDefinition::EulerOrder::YZX:
			{
				qx = sX * sY * cZ + sZ * cX * cY;
				qy = sX * cY * cZ + sY * sZ * cX;
				qz = sY * cX * cZ - sX * sZ * cY;
				qw = cX * cY * cZ - sX * sY * sZ;
				break;
			}
		case EulerDefinition::EulerOrder::ZXY:
			{
				qx = sY * cX * cZ - sX * sZ * cY;
				qy = sX * sY * cZ + sZ * cX * cY;
				qz = sX * cY * cZ + sY * sZ * cX;
				qw = cX * cY * cZ - sX * sY * sZ;
				break;
			}
		case EulerDefinition::EulerOrder::ZYX:
			{
				qx = sZ * cX * cY - sX * sY * cZ;
				qy = sX * sZ * cY + sY * cX * cZ;
				qz = sX * cY * cZ - sY * sZ * cX;
				qw = sX * sY * sZ + cX * cY * cZ;
				break;
			}
		case EulerDefinition::EulerOrder::XYX:
			{
				qx = sX* cY* cZ + sZ * cX * cY;
				qy = sX* sY* sZ + sY * cX * cZ;
				qz = sX* sY* cZ - sY * sZ * cX;
				qw = cX* cY* cZ - sX * sZ * cY;
				break;
			}
		case EulerDefinition::EulerOrder::XZX:
			{
				qx = sX* cY* cZ + sZ * cX * cY;
				qy = sY* sZ* cX - sX * sY * cZ;
				qz = sX* sY* sZ + sY * cX * cZ;
				qw = cX* cY* cZ - sX * sZ * cY;
				break;
			}
		case EulerDefinition::EulerOrder::YXY:
			{
				qx = sX* sY* sZ + sY * cX * cZ;
				qy = sX* cY* cZ + sZ * cX * cY;
				qz = sY* sZ* cX - sX * sY * cZ;
				qw = cX* cY* cZ - sX * sZ * cY;
				break;
			}
		case EulerDefinition::EulerOrder::YZY:
			{
				qx = sX* sY* cZ - sY * sZ * cX;
				qy = sX* cY* cZ + sZ * cX * cY;
				qz = sX* sY* sZ + sY * cX * cZ;
				qw = cX* cY* cZ - sX * sZ * cY;
				break;
			}
		case EulerDefinition::EulerOrder::ZXZ:
			{
				qx = sX * sY * sZ + sY * cX * cZ;
				qy = sX * sY * cZ - sY * sZ * cX;
				qz = sX * cY * cZ + sZ * cX * cY;
				qw = cX * cY * cZ - sX * sZ * cY;
				break;
			}
		case EulerDefinition::EulerOrder::ZYZ:
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

	Quaternion::Quaternion(const Quaternion& input) : pImpl(new Impl())
	{
		*this = input;
	}

	Quaternion::Quaternion(Vector3D direction, double radians, bool normalized)
	{
		direction.normalize();
		const double halfTheta = radians / 2.0f;
		const double halfThetaSin = std::sin(halfTheta);
		const double halfThetaCos = std::cos(halfTheta);

		pImpl = new Impl(	halfThetaSin * direction.x(),
			                 halfThetaSin * direction.y(),
			                 halfThetaSin * direction.z(),
			                 halfThetaCos);

		if (normalized)
			normalize();
	}

	float Quaternion::x() const
	{
		return pImpl->x;
	}

	void Quaternion::setX(float x)
	{
		pImpl->x = x;
	}

	float Quaternion::y() const
	{
		return pImpl->y;
	}

	void Quaternion::setY(float y)
	{
		pImpl->y = y;
	}

	float Quaternion::z() const
	{
		return pImpl->z;
	}

	void Quaternion::setZ(float z)
	{
		pImpl->z = z;
	}

	float Quaternion::w() const
	{
		return pImpl->w;
	}

	void Quaternion::setW(float w)
	{
		pImpl->w = w;
	}

	Quaternion Quaternion::operator+() const noexcept
	{
		return *this;
	}

	Quaternion Quaternion::operator-() const noexcept
	{
		//Avoid -0.0f which is possible somehow
		float x = pImpl->x == 0.0f ? 0.0f : -pImpl->x;
		float y = pImpl->y == 0.0f ? 0.0f : -pImpl->y;
		float z = pImpl->z == 0.0f ? 0.0f : -pImpl->z;
		float w = pImpl->w == 0.0f ? 0.0f : -pImpl->w;

		return {x, y, z, w};
	}

	bool Quaternion::operator==(const Quaternion& input) const noexcept
	{
		return pImpl->x == input.pImpl->x && pImpl->y == input.pImpl->y && pImpl->z == input.pImpl->z && pImpl->w == input.pImpl->w;
	}

	bool Quaternion::operator!=(const Quaternion& input) const noexcept
	{
		return !(pImpl->x == input.pImpl->x && pImpl->y == input.pImpl->y && pImpl->z == input.pImpl->z && pImpl->w == input.pImpl->w);
	}

	bool Quaternion::areSameRotation(const Quaternion& input, float tolerance) const noexcept
	{
		return areEqual(input,tolerance)|| areEqual(-input, tolerance);
	}

	bool Quaternion::areEqual(const Quaternion & input, float tolerance) const noexcept
	{
		const float dx = std::abs(pImpl->x - input.pImpl->x);
		const float dy = std::abs(pImpl->y - input.pImpl->y);
		const float dz = std::abs(pImpl->z - input.pImpl->z);
		const float dw = std::abs(pImpl->w - input.pImpl->w);
		return dx < tolerance && dy < tolerance && dz < tolerance && dw < tolerance;
	}

	Quaternion& Quaternion::operator+= (const Quaternion& other) noexcept
	{
		pImpl->x += other.pImpl->x;
		pImpl->y += other.pImpl->y;
		pImpl->z += other.pImpl->z;
		pImpl->w += other.pImpl->w;
		return *this;
	}

	Quaternion& Quaternion::operator-=(const Quaternion& other) noexcept
	{
		pImpl->x -= other.pImpl->x;
		pImpl->y -= other.pImpl->y;
		pImpl->z -= other.pImpl->z;
		pImpl->w -= other.pImpl->w;
		return *this;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& other) noexcept
	{
		*this = *this * other;
		return *this;
	}

	Quaternion& Quaternion::operator*=(float other) noexcept
	{
		pImpl->x *= other;
		pImpl->y *= other;
		pImpl->z *= other;
		pImpl->w *= other;
		return *this;
	}

	Quaternion& Quaternion::operator=(const Quaternion& other) noexcept
	{
		if (this != &other)
		{
			pImpl->x = other.pImpl->x;
			pImpl->y = other.pImpl->y;
			pImpl->z = other.pImpl->z;
			pImpl->w = other.pImpl->w;
		}
		return *this;
	}
	Quaternion& Quaternion::operator/=(const Quaternion& other) noexcept
	{
		pImpl->x /= other.pImpl->x;
		pImpl->y /= other.pImpl->y;
		pImpl->z /= other.pImpl->z;
		pImpl->w /= other.pImpl->w;
		return *this;
	}

	Quaternion Quaternion::conjugate() noexcept
	{
		return {pImpl->x * -1, pImpl->y * -1, pImpl->z * -1, pImpl->w};
	}

	float Quaternion::dot(const Quaternion& input) const noexcept
	{
		return pImpl->x*input.pImpl->x + pImpl->y*input.pImpl->y + pImpl->z*input.pImpl->z + pImpl->w*input.pImpl->w;
	}

	void Quaternion::conjugate(Quaternion& result) const noexcept
	{
		result.pImpl->x = -pImpl->x;
		result.pImpl->y = -pImpl->y;
		result.pImpl->z = -pImpl->z;
		result.pImpl->w = pImpl->w;
	}

	float Quaternion::lengthSquared() const noexcept
	{
		return pImpl->x* pImpl->x + pImpl->y* pImpl->y + pImpl->z* pImpl->z + pImpl->w* pImpl->w;
	}

	float Quaternion::length() const noexcept
	{
		return sqrt(lengthSquared());
	}

	void Quaternion::normalize() noexcept
	{
		float pLength = length();
		if (pLength <= 0)
		{
			pLength = 1.0;
		}

		pImpl->x /= pLength;
		pImpl->y /= pLength;
		pImpl->z /= pLength;
		pImpl->w /= pLength;
	}

	void Quaternion::normalize(Quaternion& result) const noexcept
	{
		float pLength = length();
		if (pLength <= 0)
		{
			pLength = 1.0;
		}
		result.pImpl->x = pImpl->x / pLength;
		result.pImpl->y = pImpl->y / pLength;
		result.pImpl->z = pImpl->z / pLength;
		result.pImpl->w = pImpl->w / pLength;
	}

	void Quaternion::inverse(Quaternion& result) const noexcept
	{
		float pLength = length();
		if (pLength <= 0)
		{
			pLength = 1;
		}
		Quaternion qConjugate;
		conjugate(qConjugate);

		result.pImpl->x = qConjugate.pImpl->x / pLength;
		result.pImpl->y = qConjugate.pImpl->y / pLength;
		result.pImpl->z = qConjugate.pImpl->z / pLength;
		result.pImpl->w = qConjugate.pImpl->w / pLength;
	}

	void Quaternion::inverse() const noexcept
	{
		float pLength = length();
		if (pLength <= 0)
		{
			pLength = 1;
		}
		Quaternion qConjugate;
		conjugate(qConjugate);

		pImpl->x = qConjugate.pImpl->x / pLength;
		pImpl->y = qConjugate.pImpl->y / pLength;
		pImpl->z = qConjugate.pImpl->z / pLength;
		pImpl->w = qConjugate.pImpl->w / pLength;
	}

	float Quaternion::angularDistance(const Quaternion& input) const
	{
		const auto diffQ = rotationOffset(input, *this);

		return 2 * acos(diffQ.w());
	}

	Quaternion Quaternion::rotationOffset(const Quaternion& start, const Quaternion& end)
	{
		Quaternion inv;
		start.inverse(inv);
		return end * inv;
	}

	void Quaternion::rotateBy(const Quaternion& offset)
	{
		*this = offset * *this;
	}

	Quaternion Quaternion::sLerp(const Quaternion& input, float t) const
	{
		const float pDot = dot(input);

		if (std::abs(pDot) >= 1.0f) return input;

		const float hTheta = acos(pDot);
		const float sinhTheta = sqrtf(1.0f - pDot * pDot);

		const float ratio1 = sin((1 - t) * hTheta) / sinhTheta;
		const float ratio2 = sin(t * hTheta) / sinhTheta;

		Quaternion output(
			x() * ratio1 + input.x() * ratio2,
			y() * ratio1 + input.y() * ratio2,
			z() * ratio1 + input.z() * ratio2,
			w() * ratio1 + input.w() * ratio2
		);

		return output;
	}

	EulerDefinition Quaternion::toEuler(EulerDefinition::EulerOrder order) const
	{
		Quaternion norm = *this;
		norm.normalize();

		EulerDefinition output;
		float matrix[3][3];
		//top row
		matrix[0][0] = 1 - 2*(norm.pImpl->y * norm.pImpl->y + norm.pImpl->z * norm.pImpl->z);
		matrix[0][1] = 2 * (norm.pImpl->x * norm.pImpl->y - norm.pImpl->w * norm.pImpl->z);
		matrix[0][2] = 2 * (norm.pImpl->x * norm.pImpl->z + norm.pImpl->w * norm.pImpl->y);

		//middle row
		matrix[1][0] = 2 * (norm.pImpl->x * norm.pImpl->y + norm.pImpl->w * norm.pImpl->z);
		matrix[1][1] = 1 - 2 * (norm.pImpl->x * norm.pImpl->x + norm.pImpl->z * norm.pImpl->z);
		matrix[1][2] = 2 * (norm.pImpl->y * norm.pImpl->z - norm.pImpl->w * norm.pImpl->x);

		//bottom row
		matrix[2][0] = 2 * (norm.pImpl->x * norm.pImpl->z - norm.pImpl->w * norm.pImpl->y);
		matrix[2][1] = 2 * (norm.pImpl->y * norm.pImpl->z + norm.pImpl->w * norm.pImpl->x);
		matrix[2][2] = 1 - 2 * (norm.pImpl->x * norm.pImpl->x + norm.pImpl->y * norm.pImpl->y);

		output.setOrder(order);

		switch (order)
		{
		case EulerDefinition::EulerOrder::XYZ:
			{
				output.setFirst(atan2(-matrix[1][2], matrix[2][2]));
				output.setSecond(asin(matrix[0][2]));
				output.setThird(atan2(-matrix[0][1], matrix[0][0]));
				break;
			}
		case EulerDefinition::EulerOrder::XZY:
			{
				output.setFirst(atan2(matrix[2][1], matrix[1][1]));
				output.setSecond(asin(-matrix[0][1]));
				output.setThird(atan2(matrix[0][2],matrix[0][0]));
				break;
			}
		case EulerDefinition::EulerOrder::YXZ:
			{
				output.setFirst(atan2(matrix[0][2], matrix[2][2]));
				output.setSecond(asin(-matrix[1][2]));
				output.setThird(atan2(matrix[1][0], matrix[1][1]));
				break;
			}
		case EulerDefinition::EulerOrder::YZX:
			{
				output.setFirst(atan2(-matrix[2][0], matrix[0][0]));
				output.setSecond(asin(matrix[1][0]));
				output.setThird(atan2(-matrix[1][2], matrix[1][1]));
				break;
			}
		case EulerDefinition::EulerOrder::ZXY:
			{
				output.setFirst(atan2(-matrix[0][1], matrix[1][1]));
				output.setSecond(asin(matrix[2][1]));
				output.setThird(atan2(-matrix[2][0], matrix[2][2]));
				break;
			}
		case EulerDefinition::EulerOrder::ZYX:
			{
				output.setFirst(atan2(matrix[1][0], matrix[0][0]));
				output.setSecond(asin(-matrix[2][0]));
				output.setThird(atan2(matrix[2][1], matrix[2][2]));
				break;
			}
		case EulerDefinition::EulerOrder::XYX:
			{
				output.setFirst(atan2(matrix[1][0], -matrix[2][0]));
				output.setSecond(acos(matrix[0][0]));
				output.setThird(atan2(matrix[0][1], matrix[0][2]));
				break;
			}
		case EulerDefinition::EulerOrder::XZX:
			{
				output.setFirst(atan2(matrix[2][0], matrix[1][0]));
				output.setSecond(acos(matrix[0][0]));
				output.setThird(atan2(matrix[0][2], -matrix[0][1]));
				break;
			}
		case EulerDefinition::EulerOrder::YXY:
			{
				output.setFirst(atan2(matrix[0][1], matrix[2][1]));
				output.setSecond(acos(matrix[1][1]));
				output.setThird(atan2(matrix[1][0], -matrix[1][2]));
				break;
			}
		case EulerDefinition::EulerOrder::YZY:
			{
				output.setFirst(atan2(matrix[2][1], -matrix[0][1]));
				output.setSecond(acos(matrix[1][1]));
				output.setThird(atan2(matrix[1][2], matrix[1][0]));
				break;
			}
		case EulerDefinition::EulerOrder::ZXZ:
			{
				output.setFirst(atan2(matrix[0][2], -matrix[1][2]));
				output.setSecond(acos(matrix[2][2]));
				output.setThird(atan2(matrix[2][0], matrix[2][1]));
				break;
			}
		case EulerDefinition::EulerOrder::ZYZ:
			{
				output.setFirst(atan2(matrix[1][2], matrix[0][2]));
				output.setSecond(acos(matrix[2][2]));
				output.setThird(atan2(matrix[2][1], -matrix[2][0]));
				break;
			}
		}

		return output;
	}

	const Quaternion Quaternion::Identity = {0.0f,0.0f,0.0f,1.0f};

	Utilities::StringContainer Quaternion::toString() const
	{
		Utilities::StringContainer output;
		output += std::format("x: {}, y: {}, z: {}, w: {}", pImpl->x, pImpl->y, pImpl->z, pImpl->w).c_str();
		return output;
	}

	Quaternion operator+ (const Quaternion& A, const Quaternion& B) noexcept
	{
		return {A.x() + B.x(), A.y() + B.y(), A.z() + B.z(), A.w() + B.w() };
	}
	Quaternion operator- (const Quaternion& A, const Quaternion& B) noexcept
	{
		return {A.x() - B.x(), A.y() - B.y(), A.z() - B.z(), A.w() - B.w()};
	}
	Quaternion operator* (const Quaternion& A, const Quaternion& B) noexcept
	{
		return {
			A.w() * B.x() + A.x() * B.w() + A.y() * B.z() - A.z() * B.y(),
			A.w() * B.y() - A.x() * B.z() + A.y() * B.w() + A.z() * B.x(),
			A.w() * B.z() + A.x() * B.y() - A.y() * B.x() + A.z() * B.w(),
			A.w() * B.w() - A.x() * B.x() - A.y() * B.y() - A.z() * B.z()
		};
	}
	Quaternion operator* (const Quaternion& A, float B) noexcept
	{
		return {A.x() * B, A.y() * B, A.z() * B, A.w() * B};
	}
	Quaternion operator* (float A, const Quaternion& B) noexcept
	{
		return {B.x() * A, B.y() * A, B.z() * A, B.w() * A};
	}
	Quaternion operator/ (const Quaternion& A, const Quaternion& B) noexcept
	{
		Quaternion nB;
		B.inverse(nB);
		return A * nB;
	}
	Quaternion* CreateQuaternionC(float x, float y, float z, float w)
	{
		return new Quaternion(x, y, z, w);
	}
	Quaternion* CreateQuaternionFromEulerC(float x, float y, float z, int eulerOrder)
	{
		const auto euler = EulerDefinition(x,y,z,EulerDefinition::GetEulerOrder(eulerOrder));

		return new Quaternion(euler);
	}
	float GetQuaternionXC(Quaternion* source)
	{
		return source->x();
	}
	float GetQuaternionYC(Quaternion* source)
	{
		return source->y();
	}
	float GetQuaternionZC(Quaternion* source)
	{
		return source->z();
	}
	float GetQuaternionWC(Quaternion* source)
	{
		return source->w();
	}
	int RotateQuaternionByAxisAngleC(Quaternion* input, Quaternion* result, float x, float y, float z, float radians)
	{
		if (!input || !result)
			return -1;

		if (x == 0 && y == 0 && z == 0)
			return 1;

		try
		{
			const Quaternion rotation(Vector3(x,y,z) , radians);
			*result = rotation * *input;
			return 0;
		}
		catch ( std::bad_alloc& ){}

		return -1;
	}
	int GetQuaternionToEulerC(Quaternion* input, int eulerOrder, float* first, float* second, float* third)
	{
		if (!input || !first || !second || !third)
			return -1;

		const auto euler = EulerDefinition::GetEulerOrder(eulerOrder);

		try
		{
			const auto result = input->toEuler(euler);
			*first = result.first();
			*second = result.second();
			*third = result.third();
			return 0;
		}
		catch ( std::bad_alloc& ){}
		return -1;
	}
	int RotateQuaternionByQuaternionC(Quaternion* input, Quaternion* offset, Quaternion* result)
	{
		if (!input || !offset || !result)
			return -1;

		try
		{
			*result = *input;
			result->rotateBy(*offset);
			return 0;
		}
		catch (std::bad_alloc&){}
		return -1;
	}
	int GetQuaternionOffsetC(Quaternion* input, Quaternion* reference, Quaternion* result)
	{
		if (!input || !reference || !result)
			return -1;

		try
		{
			*result = Quaternion::rotationOffset(*reference, *input);
			return 0;
		}
		catch (std::bad_alloc&){}
		return -1;
	}
	int DeleteQuaternionC(Quaternion* ptr)
	{
		if (!ptr)
			return -1;

		try
		{
			delete ptr;
			return 0;
		}
		catch ( std::bad_alloc&) { }

		return -1;
	}
#pragma endregion

#pragma region VECTOR2
	const Vector2 Vector2::Zero = Vector2(0.0);
	const Vector2 Vector2::One = Vector2(1.0);
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
		explicit Impl(float value) :x(value), y(value) {}
	};

	Vector2::Vector2()
	{
		pImpl = new Impl;
	}
	Vector2::~Vector2()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
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
		pImpl = new Impl(input.x(), input.y());
	}
	float Vector2::x() const
	{
		return pImpl->x;
	}
	void Vector2::setX(float x)
	{
		pImpl->x = x;
	}
	float Vector2::y() const
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
		if (this != &other)
		{
			pImpl->x = other.pImpl->x;
			pImpl->y = other.pImpl->y;
		}
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
		return {-pImpl->x,-pImpl->y};
	}

	float Vector2::Length() const
	{
		return sqrt(pImpl->x * pImpl->x + pImpl->y * pImpl->y);
	}
	float Vector2::LengthSquared() const
	{
		return pImpl->x * pImpl->x + pImpl->y * pImpl->y;
	}
	bool Vector2::areEqual(const Vector2& input, float tolerance) const noexcept
	{
		return std::abs(pImpl->x - input.pImpl->x) < tolerance &&
			   std::abs(pImpl->y - input.pImpl->y) < tolerance;
	}
	float Vector2::Dot(const Vector2& other) const
	{
		return pImpl->x * other.pImpl->x + pImpl->y * other.pImpl->y;
	}

	void Vector2::Cross(const Vector2& other, Vector2& result) const
	{
		result.pImpl->x = result.pImpl->y = pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x;
	}
	Vector2 Vector2::Cross(const Vector2& other) const
	{
		float result = pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x;
		return {result, result};
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
	Vector2 operator+(const Vector2& A, const Vector2& B) noexcept
	{
		return {A.x() + B.x(), A.y() + B.y()};
	}
	Vector2 operator-(const Vector2& A, const Vector2& B) noexcept
	{
		return {A.x() -B.x(),A.y() -B.y()};
	}
	Vector2 operator*(const Vector2& A, const Vector2& B) noexcept
	{
		return {A.x() *B.x(),A.y() *B.y()};
	}
	Vector2 operator*(const Vector2& A, float B) noexcept
	{
		return {A.x()*B,A.y() *B};
	}
	Vector2 operator*(float A, const Vector2& B) noexcept
	{
		return {A*B.x(),A*B.y()};
	}
	Vector2 operator/(const Vector2& A, const Vector2& B) noexcept
	{
		return {A.x()/B.x(),A.y() /B.y()};
	}
	Vector2 operator/(const Vector2& A, float B) noexcept
	{
		return {A.x()/B,A.y() /B};
	}
	Vector2 operator/(float A, const Vector2& B) noexcept
	{
		return {A/B.x(),A/B.y()};
	}
	float GetVector2XC(Vector2* source)
	{
		return source->x();
	}
	float GetVector2YC(Vector2* source)
	{
		return source->y();
	}
	int DeleteVector2C(Vector2* ptr)
	{
		if (!ptr)
			return -1;

		try
		{
			delete ptr;
			return 0;
		}
		catch (std::bad_alloc&){}

		return -1;
	}
#pragma endregion

#pragma region VECTOR2D
	const Vector2D Vector2D::Zero = Vector2D(0.0);
	const Vector2D Vector2D::One = Vector2D(1.0);
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
		explicit Impl(double value) :x(value), y(value) {}
	};

	Vector2D::Vector2D()
	{
		pImpl = new Impl;
	}

	Vector2D::~Vector2D()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
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
		pImpl = new Impl(static_cast<double>(input.x()), static_cast<double>(input.y()));
	}
	Vector2D::Vector2D(const Vector2D& input)
	{
		pImpl = new Impl(input.x(), input.y());
	}
	Vector2D::Vector2D(double x, double y)
	{
		pImpl = new Impl(x, y);
	}
	double Vector2D::x() const
	{
		return pImpl->x;
	}
	void Vector2D::setX(double x)
	{
		pImpl->x = x;
	}
	double Vector2D::y() const
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
		if (this != &other)
		{
			pImpl->x = other.pImpl->x;
			pImpl->y = other.pImpl->y;
		}
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
		return {-pImpl->x, -pImpl->y};
	}

	double Vector2D::length() const
	{
		return sqrt(pImpl->x * pImpl->x + pImpl->y * pImpl->y);
	}
	double Vector2D::lengthSquared() const
	{
		return pImpl->x * pImpl->x + pImpl->y * pImpl->y;
	}
	bool Vector2D::areEqual(const Vector2D& input, float tolerance) const noexcept
	{
		return std::abs(pImpl->x - input.pImpl->x) < tolerance &&
			   std::abs(pImpl->y - input.pImpl->y) < tolerance;
	}
	double Vector2D::dot(const Vector2D& other) const
	{
		return pImpl->x * other.pImpl->x + pImpl->y * other.pImpl->y;
	}

	void Vector2D::cross(const Vector2D& other, Vector2D& result) const
	{
		result.pImpl->x = result.pImpl->y = pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x;
	}
	Vector2D Vector2D::cross(const Vector2D& other) const
	{
		double result = pImpl->x * other.pImpl->y - pImpl->y * other.pImpl->x;
		return {result, result};
	}

	void Vector2D::normalize()
	{
		*this /= this->length();
	}
	Vector2D Vector2D::lerp(const Vector2D& input, double t) const
	{
		Vector2D output = (1.0 - t) * *this + t * input;

		return output;
	}
	Utilities::StringContainer Vector2D::toString() const
	{
		Utilities::StringContainer output;
		output += std::format("x: {}, y: {}", pImpl->x, pImpl->y).c_str();
		return output;
	}
	//Non-member functions
	Vector2D operator+(const Vector2D& A, const Vector2D& B) noexcept
	{
		return {A.x() + B.x(), A.y() + B.y()};
	}
	Vector2D operator-(const Vector2D& A, const Vector2D& B) noexcept
	{
		return {A.x() - B.x(), A.y() - B.y()};
	}
	Vector2D operator*(const Vector2D& A, const Vector2D& B) noexcept
	{
		return {A.x() * B.x(), A.y() * B.y()};
	}
	Vector2D operator*(const Vector2D& A, double B) noexcept
	{
		return {A.x() * B, A.y() * B};
	}
	Vector2D operator*(double A, const Vector2D& B) noexcept
	{
		return {A * B.x(), A * B.y()};
	}
	Vector2D operator/(const Vector2D& A, const Vector2D& B) noexcept
	{
		return {A.x() / B.x(), A.y() / B.y()};
	}
	Vector2D operator/(const Vector2D& A, double B) noexcept
	{
		return {A.x() / B, A.y() / B};
	}
	Vector2D operator/(double A, const Vector2D& B) noexcept
	{
		return {A / B.x(), A / B.y()};
	}
	Vector2D* CreateVector2DC(float x, float y)
	{
		return new Vector2D(x, y);
	}
	double GetVector2DXC(Vector2D* source)
	{
		return source->x();
	}
	double GetVector2DYC(Vector2D* source)
	{
		return source->y();
	}
	int DeleteVector2DC(Vector2D* ptr)
	{
		if (!ptr)
			return -1;

		try
		{
			delete ptr;
			return 0;
		}
		catch (std::bad_alloc&){}

		return -1;
	}
#pragma endregion

#pragma region Tranform
	struct Transform::Impl
	{
		Vector3 m_position;
		Quaternion m_rotation;
	};

	Transform::Transform(Vector3 position, Quaternion rotation) : pImpl(new Impl())
	{
		pImpl->m_position = position;
		pImpl->m_rotation = rotation;
	}

	Transform::Transform(const Transform& other) : pImpl(new Impl())
	{
		*this = other;
	}

	Transform::~Transform()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}

	Transform& Transform::operator=(const Transform& other)
	{
		if (this != &other)
		{
			pImpl->m_position = other.pImpl->m_position;
			pImpl->m_rotation = other.pImpl->m_rotation;
		}
		return *this;
	}

	Transform Transform::global(const Transform& reference) const
	{
		Quaternion qGlobal = pImpl->m_rotation;
		qGlobal.rotateBy(reference.pImpl->m_rotation);

		Quaternion qPosition(pImpl->m_position.x(), pImpl->m_position.y(), pImpl->m_position.z(), 0.0f);

		qPosition = reference.pImpl->m_rotation * qPosition * reference.pImpl->m_rotation.conjugate();
		const auto scalar = pImpl->m_position.length();

		return { {
					qPosition.x() * scalar + reference.pImpl->m_position.x(),
					qPosition.y() * scalar + reference.pImpl->m_position.y(),
					qPosition.z() * scalar + reference.pImpl->m_position.z()
				 },
					qGlobal
			   };
	}

	Transform Transform::local(const Transform& reference) const
	{
		Quaternion qLocal = pImpl->m_rotation;
		const Quaternion qInverse = reference.pImpl->m_rotation;
		qInverse.inverse();
		qLocal.rotateBy(qInverse);

		const Vector3 vPosition = pImpl->m_position - reference.pImpl->m_position;
		Quaternion qPosition(vPosition.x(), vPosition.y(), vPosition.z(), 0.0f);
		qPosition = qInverse * qPosition * reference.pImpl->m_rotation;

		const auto scalar = vPosition.length();

		return { {
			qPosition.x() * scalar,
			qPosition.y() * scalar,
			qPosition.z() * scalar
		 },
			qLocal
	   };
	}

	Quaternion Transform::rotation() const
	{
		return pImpl->m_rotation;
	}

	void Transform::setRotation(const Quaternion& rotation)
	{
		pImpl->m_rotation = rotation;
	}

	void Transform::setRotation(Quaternion&& rotation)
	{
		pImpl->m_rotation = rotation;
	}

	Vector3 Transform::position() const
	{
		return pImpl->m_position;
	}

	void Transform::setPosition(const Vector3& position)
	{
		pImpl->m_position = position;
	}

	void Transform::setPosition(Vector3&& position)
	{
		pImpl->m_position = position;
	}
	Transform* CreateTransformC()
	{
		return new Transform;
	}
	int DeleteTransformC(Transform* ptr)
	{
		if (!ptr)
			return -1;

		try
		{
			delete ptr;
			return 0;
		}
		catch (std::bad_alloc&){}

		return -1;
	}

	int SetTransformFromReferenceC(Transform* transform, Transform* reference)
	{
		if (!transform || !reference)
			return -1;

		try
		{
			*transform = *reference;
			return 0;
		}
		catch (std::bad_alloc&){}

		return -1;
	}

	Transform* GetLocalTransformC(Transform* global, Transform* reference)
	{
		if (!global || !reference)
			return nullptr;

		const auto output = new Transform;

		try
		{
			*output = global->local(*reference);
			return output;
		}
		catch (std::bad_alloc&)
		{
			delete output;
		}

		return nullptr;
	}
	Transform* GetGlobalTransformC(Transform* local, Transform* reference)
	{
		if (!local || !reference)
			return nullptr;

		const auto output = new Transform;

		try
		{
			*output = local->global(*reference);
			return output;
		}
		catch (std::bad_alloc&)
		{
			delete output;
		}

		return nullptr;
	}
	int SetTransformPositionC(Transform* transform, float x, float y, float z)
	{
		if (!transform)
			return -1;

		try
		{
			transform->setPosition({x,y,z});
			return 0;
		}
		catch (std::bad_alloc&){}
		return -1;
	}
	int SetTransformPositionFromReferenceC(Transform* transform, Vector3* reference)
	{
		if (!transform || !reference)
			return -1;

		try
		{
			transform->setPosition(*reference);
			return 0;
		}
		catch (std::bad_alloc&){}

		return -1;
	}
	int SetTransformRotationC(Transform* transform, float x, float y, float z, float w)
	{
		if (!transform)
			return -1;

		try
		{
			transform->setRotation({x,y,z,w});
			return 0;
		}
		catch (std::bad_alloc&){}
		return -1;
	}
	int SetTransformEulerRotationC(Transform* transform, float x, float y, float z, int eulerOrder)
	{
		if (!transform)
			return -1;

		const auto euler = EulerDefinition(x,y,z, EulerDefinition::GetEulerOrder(eulerOrder));

		try
		{
			transform->setRotation(euler);
			return 0;
		}
		catch (std::bad_alloc&){}
		return -1;
	}
#pragma endregion
	int SetTransformRotationFromReferenceC(Transform* transform, Quaternion* reference)
	{
		if (!transform || !reference)
			return -1;

		try
		{
			transform->setRotation(*reference);
			return 0;
		}
		catch (std::bad_alloc&){}
		return -1;
	}

	Vector3* GetTransformPositionC(Transform* transform)
	{
		if (!transform)
			return nullptr;

		const auto output = new Vector3;
		try
		{
			*output = transform->position();
			return output;
		}
		catch (std::bad_alloc&)
		{
			delete output;
		}
		return nullptr;
	}

	float GetTransformPositionXC(Transform* transform)
	{
		constexpr float nanOutput = std::numeric_limits<float>::quiet_NaN();

		if (!transform)
			return nanOutput;

		try
		{
			return transform->position().x();
		}
		catch (std::bad_alloc&){}
		return nanOutput;
	}

	float GetTransformPositionYC(Transform* transform)
	{
		constexpr float nanOutput = std::numeric_limits<float>::quiet_NaN();

		if (!transform)
			return nanOutput;

		try
		{
			return transform->position().y();
		}
		catch (std::bad_alloc&){}
		return nanOutput;
	}

	float GetTransformPositionZC(Transform* transform)
	{
		constexpr float nanOutput = std::numeric_limits<float>::quiet_NaN();

		if (!transform)
			return nanOutput;

		try
		{
			return transform->position().z();
		}
		catch (std::bad_alloc&){}
		return nanOutput;
	}

	Quaternion* GetTransformRotationC(Transform* transform)
	{
		if (!transform)
			return nullptr;

		const auto output = new Quaternion;

		try
		{
			*output = transform->rotation();
			return output;
		}
		catch (std::bad_alloc&){}
		return nullptr;
	}

	float GetTransformRotationXC(Transform* transform)
	{
		constexpr float nanOutput = std::numeric_limits<float>::quiet_NaN();

		if (!transform)
			return nanOutput;

		try
		{
			return transform->rotation().x();
		}
		catch (std::bad_alloc&){}
		return nanOutput;
	}
	float GetTransformRotationYC(Transform* transform)
	{
		constexpr float nanOutput = std::numeric_limits<float>::quiet_NaN();

		if (!transform)
			return nanOutput;

		try
		{
			return transform->rotation().y();
		}
		catch (std::bad_alloc&){}
		return nanOutput;
	}
	float GetTransformRotationZC(Transform* transform)
	{
		constexpr float nanOutput = std::numeric_limits<float>::quiet_NaN();

		if (!transform)
			return nanOutput;

		try
		{
			return transform->rotation().z();
		}
		catch (std::bad_alloc&){}
		return nanOutput;
	}
	float GetTransformRotationWC(Transform* transform)
	{
		constexpr float nanOutput = std::numeric_limits<float>::quiet_NaN();

		if (!transform)
			return nanOutput;

		try
		{
			return transform->rotation().w();
		}
		catch (std::bad_alloc&){}
		return nanOutput;
	}

	int GetTransformEulerRotationC(Transform* transform, float* first, float* second, float* third, int eulerOrder)
	{
		if (!transform || !first || !second || !third)
			return -1;

		const auto euler = EulerDefinition::GetEulerOrder(eulerOrder);

		try
		{
			const auto result = transform->rotation().toEuler(euler);
			*first = result.first();
			*second = result.second();
			*third = result.third();
			return 0;
		}
		catch (std::bad_alloc&){}
		return -1;
	}
}

