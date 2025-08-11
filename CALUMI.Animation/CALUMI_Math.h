#pragma once
#ifdef CALUMIANIMATION_EXPORTS // This macro is typically defined by Visual Studio for DLL projects
#define CALUMIANIMATION_API __declspec(dllexport)
#else
#define CALUMIANIMATION_API __declspec(dllimport)
#endif
#include <cmath>
#include <string>
#include <format>

namespace CALUMI
{
	namespace Math{
		struct CALUMIANIMATION_API Vector3D
		{
			double x = 0.0;
			double y = 0.0;
			double z = 0.0;

			Vector3D() = default;
			Vector3D(double input[3]);
			Vector3D(double input);
			Vector3D(double x, double y, double z)
				: x(x), y(y), z(z)
			{
			}




			bool operator==(const Vector3D& other) const;
			bool operator!=(const Vector3D& other) const;

			Vector3D operator+(const Vector3D& other) const;
			Vector3D& operator+=(const Vector3D& other);
			Vector3D operator-(const Vector3D& other) const;
			Vector3D& operator-=(const Vector3D& other);
			Vector3D operator*(const Vector3D& other) const;
			Vector3D& operator*=(const Vector3D& other);
			Vector3D& operator*=(const double& other);
			Vector3D& operator/=(const double& other);

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
	}
}
