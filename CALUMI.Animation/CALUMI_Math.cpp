//Copyright © 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_Math.h"

namespace CALUMI
{
	namespace Math{

		const Vector3D Vector3D::Zero = 0.0; 
		const Vector3D Vector3D::One = 1.0;
		const Vector3D Vector3D::UnitX = { 1.0, 0.0, 0.0};
		const Vector3D Vector3D::UnitY = { 0.0, 1.0, 0.0};
		const Vector3D Vector3D::UnitZ = { 0.0, 0.0, 1.0};

		const Vector3D Vector3D::Up = { 0.0, 0.0, 1.0}; //In coordinate system found on Nifskope. May need modifying
		const Vector3D Vector3D::Down = { 0.0, 0.0, -1.0};
		const Vector3D Vector3D::Right = { -1.0, 0.0, 0.0};
		const Vector3D Vector3D::Left = { 1.0, 0.0, 0.0};
		const Vector3D Vector3D::Forward = { 0.0, 1.0, 0.0};
		const Vector3D Vector3D::Back = { 0.0, -1.0, 0.0};

		std::string Vector3D::ToString()
		{
			return std::format("x: {}, y: {}, z: {}",x,y,z);
		}

		Vector3D::Vector3D(double input[3])
		{
			x = input[0];
			y = input[1];
			z = input[2];
		}

		Vector3D::Vector3D(double input)
		{
			x = input;
			y = input;
			z = input;
		}

		bool Vector3D::operator==(const Vector3D & other) const
		{
			return x == other.x && y == other.y && z == other.z;
		}

		bool Vector3D::operator!=(const Vector3D & other) const
		{
			return !(x == other.x && y == other.y && z == other.z);
		}

		Vector3D Vector3D::operator+(const Vector3D & other) const
		{
			return Vector3D(x + other.x, y + other.y, z + other.z);
		}

		Vector3D& Vector3D::operator+=(const Vector3D & other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Vector3D Vector3D::operator-(const Vector3D & other) const
		{
			return Vector3D(x - other.x, y - other.y, z - other.z);
		}

		Vector3D& Vector3D::operator-=(const Vector3D & other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		Vector3D Vector3D::operator*(const Vector3D & other) const
		{
			return Vector3D(x * other.x, y * other.y, z * other.z);
		}
		Vector3D& Vector3D::operator*=(const Vector3D & other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		Vector3D& Vector3D::operator*=(const double& other)
		{
			x *= other;
			y *= other;
			z *= other;
			return *this;
		}

		Vector3D& Vector3D::operator/=(const double& other)
		{
			x /= other;
			y /= other;
			z /= other;
			return *this;
		}

		Vector3D Vector3D::operator+() const { return *this; };

		Vector3D Vector3D::operator-() const { return Vector3D(-x, -y, -z); };

		double Vector3D::Length() const
		{

			return sqrt((x * x) + (y * y) + (z * z));
		}
		double Vector3D::LengthSquared() const
		{
			return (x * x) + (y * y) + (z * z);
		}

		double Vector3D::Dot(const Vector3D & other) const
		{
			return (x * other.x) + (y * other.y) + (z * other.z);
		}

		void Vector3D::Cross(const Vector3D & other, Vector3D & result) const
		{
			result.x = y * other.z - z * other.y;
			result.y = z * other.x - x * other.z;
			result.z = x * other.y - y * other.x;
		}

		Vector3D Vector3D::Cross(const Vector3D & other) const
		{
			return Vector3D(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}

		void Vector3D::Normalize()
		{
			*this /= this->Length();
		}
	}

}

