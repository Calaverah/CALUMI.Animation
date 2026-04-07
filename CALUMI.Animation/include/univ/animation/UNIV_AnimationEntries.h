//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Common.h"
#include "math/Math.h"
#include "utilities/CALUMI_Utilities.h"

namespace CALUMI {namespace UNIV {

	

	struct CALUMIANIMATION_API Translation
	{
        uint16_t frame() const;
        void setFrame(uint16_t frame);
        Math::Vector3D& translationVector() const;

		~Translation();
		Translation();
		Translation(const uint16_t& frame, const CALUMI::Math::Vector3D& translation);
		Translation(const Translation& input);

        bool isIdentical(const Translation& input) const;

		UNIV::Translation& operator=(const UNIV::Translation& other);

        Utilities::StringContainer toJSON(const uint64_t indents = 0) const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	bool operator<(const UNIV::Translation& A, const UNIV::Translation& B);
	bool operator<=(const UNIV::Translation& A, const UNIV::Translation& B);
	bool operator>(const UNIV::Translation& A, const UNIV::Translation& B);
	bool operator>=(const UNIV::Translation& A, const UNIV::Translation& B);
	bool operator==(const UNIV::Translation& A, const UNIV::Translation& B);
	bool operator!=(const UNIV::Translation& A, const UNIV::Translation& B);

	struct CALUMIANIMATION_API Rotation
	{
        uint16_t frame() const;
        void setFrame(uint16_t frame);
        Math::Quaternion& rotationQuaternion() const;

		Rotation();
		Rotation(const uint16_t& frame, const CALUMI::Math::Quaternion& rotation);
		Rotation(const Rotation& input);
		~Rotation();

        bool isIdentical(const Rotation& input) const;

		UNIV::Rotation& operator=(const UNIV::Rotation& other);

        Utilities::StringContainer toJSON(const uint64_t indents = 0) const;
	private:
		struct Impl;
		Impl* pImpl;
	};

	bool operator<(const UNIV::Rotation& A, const UNIV::Rotation& B);
	bool operator<=(const UNIV::Rotation& A, const UNIV::Rotation& B);
	bool operator>(const UNIV::Rotation& A, const UNIV::Rotation& B);
	bool operator>=(const UNIV::Rotation& A, const UNIV::Rotation& B);
	bool operator==(const UNIV::Rotation& A, const UNIV::Rotation& B);
	bool operator!=(const UNIV::Rotation& A, const UNIV::Rotation& B);


	struct CALUMIANIMATION_API Scalar
	{
        uint16_t frame() const;
        void setFrame(uint16_t frame);
        float scalarValue() const;
        void setScalarValue(float value);

		Scalar();
		~Scalar();
		Scalar(const uint16_t& frame, float scalar);
		Scalar(const Scalar& input);

        bool isIdentical(const Scalar& input) const;

		UNIV::Scalar& operator=(const UNIV::Scalar& other);

        Utilities::StringContainer toJSON(const uint64_t indents = 0) const;
	private:
		struct Impl;
		Impl* pImpl;
	};

	bool operator<(const UNIV::Scalar& A, const UNIV::Scalar& B);
	bool operator<=(const UNIV::Scalar& A, const UNIV::Scalar& B);
	bool operator>(const UNIV::Scalar& A, const UNIV::Scalar& B);
	bool operator>=(const UNIV::Scalar& A, const UNIV::Scalar& B);
	bool operator==(const UNIV::Scalar& A, const UNIV::Scalar& B);
	bool operator!=(const UNIV::Scalar& A, const UNIV::Scalar& B);


	struct CALUMIANIMATION_API Priority
	{
        uint16_t frame() const;
        void setFrame(uint16_t frame);
        uint8_t priorityValue() const;
        void setPriorityValue(uint8_t value);

		Priority();
		~Priority();
		Priority(const uint16_t& frame, const uint8_t& priority);
		Priority(const Priority& input);

		UNIV::Priority& operator=(const UNIV::Priority& other);

        Utilities::StringContainer toJSON(const uint64_t indents = 0) const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	bool operator<(const UNIV::Priority& A, const UNIV::Priority& B);
	bool operator<=(const UNIV::Priority& A, const UNIV::Priority& B);
	bool operator>(const UNIV::Priority& A, const UNIV::Priority& B);
	bool operator>=(const UNIV::Priority& A, const UNIV::Priority& B);
	bool operator==(const UNIV::Priority& A, const UNIV::Priority& B);
	bool operator!=(const UNIV::Priority& A, const UNIV::Priority& B);

	VECTORDEC(TranslationSequence, Translation)
	VECTORDEC(RotationSequence, Rotation)
	VECTORDEC(ScalarSequence, Scalar)
	VECTORDEC(PrioritySequence, Priority)

    void SortTranslationSequence(TranslationSequence& sq, bool highToLow = false);
    void SortRotationSequence(RotationSequence& sq, bool highToLow = false);
    void SortScalarSequence(ScalarSequence& sq, bool highToLow = false);
    void SortPrioritySequence(PrioritySequence& sq, bool highToLow = false);

	extern  "C" {
		CALUMIANIMATION_API Rotation* CreateRotationEntryC(uint16_t frame, float x, float y, float z, float w);

		/// <summary>
		/// Creates a quaternion using euler operations.
		/// </summary>
		/// <param name="frame">Frame of the entry</param>
		/// <param name="x">In Radians</param>
		/// <param name="y">In Radians</param>
		/// <param name="z">In Radians</param>
		/// <param name="order">Enum found in the Math::Quaternion struct</param>
		/// <returns></returns>
		CALUMIANIMATION_API Rotation* CreateRotationEntryFromEulerC(uint16_t frame, float x, float y, float z, uint8_t order);
		
		CALUMIANIMATION_API bool DeleteRotationEntryC(Rotation* ptr);

		CALUMIANIMATION_API Translation* CreateTranslationEntryC(uint16_t frame, double x, double y, double z);
		
		CALUMIANIMATION_API bool DeleteTranslationEntryC(Translation* ptr);
		
		CALUMIANIMATION_API Scalar* CreateScalarEntryC(uint16_t frame, float scalar);
		
		CALUMIANIMATION_API bool DeleteScalarEntryC(Scalar* ptr);
		
		CALUMIANIMATION_API Priority* CreatePriorityEntryC(uint16_t frame, uint8_t priority);
		
		CALUMIANIMATION_API bool DeletePriorityEntryC(Priority* ptr);
		
		CALUMIANIMATION_API uint16_t GetFrameFromRotationEntryC(Rotation* source);
		
		CALUMIANIMATION_API uint16_t GetFrameFromTranslationEntryC(Translation* source);
		
		CALUMIANIMATION_API uint16_t GetFrameFromScalarEntryC(Scalar* source);
		
		CALUMIANIMATION_API uint16_t GetFrameFromPriorityEntryC(Priority* source);
		/// <summary>
		/// Returns a pointer to a rotation entry value (Quaternion). An array of 4 floats (4 bytes each)
		/// </summary>
		/// <param name="source"></param>
		/// <returns></returns>
		CALUMIANIMATION_API Math::Quaternion* GetValueFromRotationEntryC(Rotation* source);
		/// <summary>
		/// Returns a pointer to the translation entry value (Vector3D). An array of 3 doubles  (8 bytes each)
		/// </summary>
		/// <param name="source"></param>
		/// <returns></returns>
		CALUMIANIMATION_API Math::Vector3D* GetValueFromTranslationEntryC(Translation* source);
		/// <summary>
		/// Returns the scalar entry value (float)
		/// </summary>
		/// <param name="source"></param>
		/// <returns></returns>
		CALUMIANIMATION_API float GetValueFromScalarEntryC(Scalar* source);
		/// <summary>
		/// Returns the priority entry value (uint8_t)
		/// </summary>
		/// <param name="source"></param>
		/// <returns></returns>
		CALUMIANIMATION_API uint8_t GetValueFromPriorityEntryC(Priority* source);
	}

}}

//#pragma warning(disable: 4661)
//_VECTORTEMPLATE(CALUMI::UNIV::Rotation);
//_VECTORTEMPLATE(CALUMI::UNIV::Translation);
//_VECTORTEMPLATE(CALUMI::UNIV::Scalar);
//_VECTORTEMPLATE(CALUMI::UNIV::Priority);
//#pragma warning(default: 4661)