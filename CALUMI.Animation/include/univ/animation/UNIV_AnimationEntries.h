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
        void setFrame(uint16_t frame) const;
        Math::Vector3D& translationVector() const;

		~Translation();
		Translation();
		Translation(const uint16_t& frame, const CALUMI::Math::Vector3D& translation);
		Translation(const Translation& input);

        bool isIdentical(const Translation& input) const;

		UNIV::Translation& operator=(const UNIV::Translation& other);

        /**
         * @deprecated
         * @param indents
         * @return
         */
        [[deprecated]] [[nodiscard]] Utilities::StringContainer toJSON(const uint64_t indents = 0) const;

	private:
		struct Impl;
		Impl* pImpl;
	};

    CALUMIANIMATION_API bool operator<(const UNIV::Translation& A, const UNIV::Translation& B);
    CALUMIANIMATION_API bool operator<=(const UNIV::Translation& A, const UNIV::Translation& B);
    CALUMIANIMATION_API bool operator>(const UNIV::Translation& A, const UNIV::Translation& B);
    CALUMIANIMATION_API bool operator>=(const UNIV::Translation& A, const UNIV::Translation& B);
    CALUMIANIMATION_API bool operator==(const UNIV::Translation& A, const UNIV::Translation& B);
    CALUMIANIMATION_API bool operator!=(const UNIV::Translation& A, const UNIV::Translation& B);

	struct CALUMIANIMATION_API Rotation
	{
        uint16_t frame() const;
        void setFrame(uint16_t frame) const;
        Math::Quaternion& rotationQuaternion() const;

		Rotation();
		Rotation(const uint16_t& frame, const CALUMI::Math::Quaternion& rotation);
		Rotation(const Rotation& input);
		~Rotation();

        bool isIdentical(const Rotation& input) const;

		UNIV::Rotation& operator=(const UNIV::Rotation& other);

        [[deprecated]] [[nodiscard]] Utilities::StringContainer toJSON(const uint64_t indents = 0) const;
	private:
		struct Impl;
		Impl* pImpl;
	};

    CALUMIANIMATION_API bool operator<(const UNIV::Rotation& A, const UNIV::Rotation& B);
    CALUMIANIMATION_API bool operator<=(const UNIV::Rotation& A, const UNIV::Rotation& B);
    CALUMIANIMATION_API bool operator>(const UNIV::Rotation& A, const UNIV::Rotation& B);
    CALUMIANIMATION_API bool operator>=(const UNIV::Rotation& A, const UNIV::Rotation& B);
    CALUMIANIMATION_API bool operator==(const UNIV::Rotation& A, const UNIV::Rotation& B);
    CALUMIANIMATION_API bool operator!=(const UNIV::Rotation& A, const UNIV::Rotation& B);


	struct CALUMIANIMATION_API Scalar
	{
        uint16_t frame() const;
        void setFrame(uint16_t frame) const;
        float scalarValue() const;
        void setScalarValue(float value) const;

		Scalar();
		~Scalar();
		Scalar(const uint16_t& frame, float scalar);
		Scalar(const Scalar& input);

        bool isIdentical(const Scalar& input) const;

		UNIV::Scalar& operator=(const UNIV::Scalar& other);

        [[deprecated]] [[nodiscard]] Utilities::StringContainer toJSON(const uint64_t indents = 0) const;
	private:
		struct Impl;
		Impl* pImpl;
	};

    CALUMIANIMATION_API bool operator<(const UNIV::Scalar& A, const UNIV::Scalar& B);
    CALUMIANIMATION_API bool operator<=(const UNIV::Scalar& A, const UNIV::Scalar& B);
    CALUMIANIMATION_API bool operator>(const UNIV::Scalar& A, const UNIV::Scalar& B);
    CALUMIANIMATION_API bool operator>=(const UNIV::Scalar& A, const UNIV::Scalar& B);
    CALUMIANIMATION_API bool operator==(const UNIV::Scalar& A, const UNIV::Scalar& B);
    CALUMIANIMATION_API bool operator!=(const UNIV::Scalar& A, const UNIV::Scalar& B);


	struct CALUMIANIMATION_API Priority
	{
        uint16_t frame() const;
        void setFrame(uint16_t frame) const;
        uint8_t priorityValue() const;
        void setPriorityValue(uint8_t value) const;

		Priority();
		~Priority();
		Priority(const uint16_t& frame, const uint8_t& priority);
		Priority(const Priority& input);

		Priority& operator=(const UNIV::Priority& other);

        [[deprecated]] [[nodiscard]] Utilities::StringContainer toJSON(const uint64_t indents = 0) const;

	private:
		struct Impl;
		Impl* pImpl;
	};

    CALUMIANIMATION_API bool operator<(const UNIV::Priority& A, const UNIV::Priority& B);
    CALUMIANIMATION_API bool operator<=(const UNIV::Priority& A, const UNIV::Priority& B);
    CALUMIANIMATION_API bool operator>(const UNIV::Priority& A, const UNIV::Priority& B);
    CALUMIANIMATION_API bool operator>=(const UNIV::Priority& A, const UNIV::Priority& B);
    CALUMIANIMATION_API bool operator==(const UNIV::Priority& A, const UNIV::Priority& B);
    CALUMIANIMATION_API bool operator!=(const UNIV::Priority& A, const UNIV::Priority& B);

	struct TranslationSequence;
	struct RotationSequence;
	struct ScalarSequence;
	struct PrioritySequence;

	CALUMIANIMATION_API void SortTranslationSequence(const TranslationSequence& sq, bool highToLow = false);
	CALUMIANIMATION_API void SortRotationSequence(const RotationSequence& sq, bool highToLow = false);
	CALUMIANIMATION_API void SortScalarSequence(const ScalarSequence& sq, bool highToLow = false);
	CALUMIANIMATION_API void SortPrioritySequence(const PrioritySequence& sq, bool highToLow = false);

    VECTORDECF(TranslationSequence, Translation, friend void SortTranslationSequence(const TranslationSequence& sq, bool highToLow);)
    VECTORDECF(RotationSequence, Rotation, friend void SortRotationSequence(const RotationSequence& sq, bool highToLow);)
    VECTORDECF(ScalarSequence, Scalar, friend void SortScalarSequence(const ScalarSequence& sq, bool highToLow);)
    VECTORDECF(PrioritySequence, Priority, friend void SortPrioritySequence(const PrioritySequence& sq, bool highToLow);)

	

	extern  "C" {
		CALUMIANIMATION_API Rotation* CreateRotationEntryC(uint16_t frame, float x, float y, float z, float w);

		CALUMIANIMATION_API Rotation* CreateRotationEntryFromEulerC(uint16_t frame, float x, float y, float z, uint8_t order);
		
		CALUMIANIMATION_API bool DeleteRotationEntryC(const Rotation* ptr);

		CALUMIANIMATION_API Translation* CreateTranslationEntryC(uint16_t frame, double x, double y, double z);
		
		CALUMIANIMATION_API bool DeleteTranslationEntryC(const Translation* ptr);
		
		CALUMIANIMATION_API Scalar* CreateScalarEntryC(uint16_t frame, float scalar);
		
		CALUMIANIMATION_API bool DeleteScalarEntryC(const Scalar* ptr);
		
		CALUMIANIMATION_API Priority* CreatePriorityEntryC(uint16_t frame, uint8_t priority);
		
		CALUMIANIMATION_API bool DeletePriorityEntryC(const Priority* ptr);
		
		CALUMIANIMATION_API uint16_t GetFrameFromRotationEntryC(const Rotation* source);
		
		CALUMIANIMATION_API uint16_t GetFrameFromTranslationEntryC(const Translation* source);
		
		CALUMIANIMATION_API uint16_t GetFrameFromScalarEntryC(const Scalar* source);
		
		CALUMIANIMATION_API uint16_t GetFrameFromPriorityEntryC(const Priority* source);

		CALUMIANIMATION_API Math::Quaternion* GetValueFromRotationEntryC(const Rotation* source);

		CALUMIANIMATION_API Math::Vector3D* GetValueFromTranslationEntryC(const Translation* source);

		CALUMIANIMATION_API float GetValueFromScalarEntryC(const Scalar* source);

		CALUMIANIMATION_API uint8_t GetValueFromPriorityEntryC(const Priority* source);
	}

}}