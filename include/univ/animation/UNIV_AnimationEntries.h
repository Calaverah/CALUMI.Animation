//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "utilities/CALUMI_Common.h"
#include "math/Math.h"
#include "utilities/CALUMI_Utilities.h"

namespace CALUMI::UNIV
{
	/**
	 * @brief Translation Sequence Entry With Frame
	 */
	struct CALUMIANIMATION_API TranslationFrame
	{
		/// @name Data
		/// @{

		/**
		 * @return The frame of this entry
		 */
		[[nodiscard]] uint16_t frame() const;
		/**
		 * @param frame 0 based frame
		 */
		void setFrame(uint16_t frame) const;
		/**
		 *
		 * @return
		 */
		[[nodiscard]] Math::Vector3D& translationVector() const;

		/// @}
		/// @name Initialization
		/// @{

		~TranslationFrame();
		TranslationFrame();
		/**
		 * @param frame 0 based frame
		 * @param translation high precision vector3
		 */
		TranslationFrame(const uint16_t& frame, const Math::Vector3D& translation);
		/**
		 * @param data
		 */
		explicit TranslationFrame(const Utilities::JsonObject& data);
		/**
		 * @param input
		 */
		TranslationFrame(const TranslationFrame& input);

		/// @}
		/// @name Operators
		/// @{

		/**
		 * @param other
		 * @return
		 */
		TranslationFrame& operator=(const TranslationFrame& other);

		/// @}
		/// @name True Comparison
		/// @{

		/**
		 * @return Whether the two entries are equal for all struct members
		 */
		[[nodiscard]] bool isIdentical(const TranslationFrame& input) const;

		/// @}
		/// @name Json
		/// @{
		[[nodiscard]] Utilities::JsonObject toJson() const;
		/// @}
	private:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 * @relates TranslationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator<(const TranslationFrame& A, const TranslationFrame& B);
	/**
	 * @relates TranslationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator<=(const TranslationFrame& A, const TranslationFrame& B);
	/**
	 * @relates TranslationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator>(const TranslationFrame& A, const TranslationFrame& B);
	/**
	 * @relates TranslationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator>=(const TranslationFrame& A, const TranslationFrame& B);
	/**
	 * @relates TranslationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator==(const TranslationFrame& A, const TranslationFrame& B);
	/**
	 * @relates TranslationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator!=(const TranslationFrame& A, const TranslationFrame& B);

	/**
	 * @brief Rotation Sequence Entry With Frame
	 */
	struct CALUMIANIMATION_API RotationFrame
	{
		/// @name Data
		/// @{

		/**
		 * @return The frame of this entry
		 */
		[[nodiscard]] uint16_t frame() const;
		/**
		 * @param frame 0 based frame
		 */
		void setFrame(uint16_t frame) const;
		/**
		 * @return
		 */
		[[nodiscard]] Math::Quaternion& rotationQuaternion() const;

		/// @}
		/// @name Initialization
		/// @{

		RotationFrame();
		/**
		 * @param frame 0 based frame
		 * @param rotation Quaternion based rotation
		 */
		RotationFrame(const uint16_t& frame, const Math::Quaternion& rotation);
		/**
		 * @param data
		 */
		explicit RotationFrame(const Utilities::JsonObject& data);
		/**
		 * @param input
		 */
		RotationFrame(const RotationFrame& input);
		~RotationFrame();

		/// @}
		/// @name Operators
		/// @{

		/**
		 * @param other
		 * @return
		 */
		RotationFrame& operator=(const RotationFrame& other);

		/// @}
		/// @name True Comparison
		/// @{

		/**
		 * @return Whether the two entries are equal for all struct members
		 */
		[[nodiscard]] bool isIdentical(const RotationFrame& input) const;

		/// @}
		/// @name Json
		/// @{
		[[nodiscard]] Utilities::JsonObject toJson() const;
		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 * @relates RotationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator<(const RotationFrame& A, const RotationFrame& B);
	/**
	 * @relates RotationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator<=(const RotationFrame& A, const RotationFrame& B);
	/**
	 * @relates RotationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator>(const RotationFrame& A, const RotationFrame& B);
	/**
	 * @relates RotationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator>=(const RotationFrame& A, const RotationFrame& B);
	/**
	 * @relates RotationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator==(const RotationFrame& A, const RotationFrame& B);
	/**
	 * @relates RotationFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator!=(const RotationFrame& A, const RotationFrame& B);


	/**
	 * @brief Scalar Sequence Entry With Frame
	 */
	struct CALUMIANIMATION_API ScalarFrame
	{
		/// @name Data
		/// @{

		/**
		 * @return The frame of this entry
		 */
		[[nodiscard]] uint16_t frame() const;
		/**
		 *
		 * @param frame 0 based frame
		 */
		void setFrame(uint16_t frame) const;
		/**
		 * @return
		 */
		[[nodiscard]] float scalarValue() const;
		/**
		 * @param value
		 */
		void setScalarValue(float value) const;

		/// @}
		/// @name Initialization
		/// @{

		ScalarFrame();
		~ScalarFrame();
		/**
		 * @param frame 0 based frame
		 * @param scalar
		 */
		ScalarFrame(const uint16_t& frame, float scalar);
		/**
		 * @param data
		 */
		explicit ScalarFrame(const Utilities::JsonObject& data);
		/**
		 * @param input
		 */
		ScalarFrame(const ScalarFrame& input);

		/// @}
		/// @name Operators
		/// @{

		/**
		 * @param other
		 * @return
		 */
		ScalarFrame& operator=(const ScalarFrame& other);

		/// @}
		/// @name True Comparison
		/// @{

		/**
		 * @return Whether the two entries are equal for all struct members
		 */
		[[nodiscard]] bool isIdentical(const ScalarFrame& input) const;

		/// @}
		/// @name Json
		/// @{
		[[nodiscard]] Utilities::JsonObject toJson() const;
		/// @}
	private:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 * @relates ScalarFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator<(const ScalarFrame& A, const ScalarFrame& B);
	/**
	 * @relates ScalarFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator<=(const ScalarFrame& A, const ScalarFrame& B);
	/**
	 * @relates ScalarFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator>(const ScalarFrame& A, const ScalarFrame& B);
	/**
	 * @relates ScalarFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator>=(const ScalarFrame& A, const ScalarFrame& B);
	/**
	 * @relates ScalarFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator==(const ScalarFrame& A, const ScalarFrame& B);
	/**
	 * @relates ScalarFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator!=(const ScalarFrame& A, const ScalarFrame& B);


	/**
	 * @brief Priority Sequence Entry With Frame
	 */
	struct CALUMIANIMATION_API PriorityFrame
	{
		/// @name Data
		/// @{

		/**
		 * @return The frame of this entry
		 */
		[[nodiscard]] uint16_t frame() const;
		/**
		 * @param frame 0 based frame
		 */
		void setFrame(uint16_t frame) const;
		/**
		 * @return
		 */
		[[nodiscard]] uint8_t priorityValue() const;
		/**
		 * @param value
		 */
		void setPriorityValue(uint8_t value) const;

		/// @}
		/// @name Initialization
		/// @{

		PriorityFrame();
		~PriorityFrame();
		/**
		 * @param frame 0 based frame
		 * @param priority
		 */
		PriorityFrame(const uint16_t& frame, const uint8_t& priority);
		/**
		 * @param data
		 */
		explicit PriorityFrame(const Utilities::JsonObject& data);
		/**
		 * @param input
		 */
		PriorityFrame(const PriorityFrame& input);

		/// @}
		/// @name Operators
		/// @{

		/**
		 * @param other
		 * @return
		 */
		PriorityFrame& operator=(const PriorityFrame& other);
		///@}
		///@name
		///@{
		[[nodiscard]] Utilities::JsonObject toJson() const;
		///@}
		/// @name True Comparison
		/// @{

		/**
		 * @return Whether the two entries are equal for all struct members
		 */
		[[nodiscard]] bool isIdentical(const PriorityFrame& input) const;

		/// @}

	private:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 * @relates PriorityFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator<(const PriorityFrame& A, const PriorityFrame& B);
	/**
	 * @relates PriorityFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator<=(const PriorityFrame& A, const PriorityFrame& B);
	/**
	 * @relates PriorityFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator>(const PriorityFrame& A, const PriorityFrame& B);
	/**
	 * @relates PriorityFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator>=(const PriorityFrame& A, const PriorityFrame& B);
	/**
	 * @relates PriorityFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator==(const PriorityFrame& A, const PriorityFrame& B);
	/**
	 * @relates PriorityFrame
	 * @param A
	 * @param B
	 * @return Frame Comparison
	 */
	CALUMIANIMATION_API bool operator!=(const PriorityFrame& A, const PriorityFrame& B);

	struct TranslationSequence;
	struct RotationSequence;
	struct ScalarSequence;
	struct PrioritySequence;

	/**
	 * @relates TranslationFrame
	 * @brief Sorts the sequence by frame
	 * @param sq
	 * @param highToLow
	 */
	CALUMIANIMATION_API void SortTranslationSequence(const TranslationSequence& sq, bool highToLow = false);
	/**
	 * @relates TranslationFrame
	 * @param sq1
	 * @param sq2
	 * @return
	 */
	CALUMIANIMATION_API bool CompareTranslationSequence(const TranslationSequence& sq1, const TranslationSequence& sq2);
	/**
	 * @relates RotationFrame
	 * @brief Sorts the sequence by frame
	 * @param sq
	 * @param highToLow
	 */
	CALUMIANIMATION_API void SortRotationSequence(const RotationSequence& sq, bool highToLow = false);
	/**
	 * @relates RotationFrame
	 * @param sq1
	 * @param sq2
	 * @return
	 */
	CALUMIANIMATION_API bool CompareRotationSequence(const RotationSequence& sq1, const RotationSequence& sq2);
	/**
	 * @relates ScalarFrame
	 * @brief Sorts the sequence by frame
	 * @param sq
	 * @param highToLow
	 */
	CALUMIANIMATION_API void SortScalarSequence(const ScalarSequence& sq, bool highToLow = false);
	/**
	 * @relates ScalarFrame
	 * @param sq1
	 * @param sq2
	 * @return
	 */
	CALUMIANIMATION_API bool CompareScalarSequence(const ScalarSequence& sq1, const ScalarSequence& sq2);
	/**
	 * @relates PriorityFrame
	 * @brief Sorts the sequence by frame
	 * @param sq
	 * @param highToLow
	 */
	CALUMIANIMATION_API void SortPrioritySequence(const PrioritySequence& sq, bool highToLow = false);
	/**
	 * @relates PriorityFrame
	 * @param sq1
	 * @param sq2
	 * @return
	 */
	CALUMIANIMATION_API bool ComparePrioritySequence(const PrioritySequence& sq1, const PrioritySequence& sq2);

	VECTORDECF(TranslationSequence, TranslationFrame,
		friend void CALUMIANIMATION_API SortTranslationSequence(const TranslationSequence& sq, bool highToLow);
		friend bool CALUMIANIMATION_API CompareTranslationSequence(const TranslationSequence& sq1, const TranslationSequence& sq2);)
	VECTORDECF(RotationSequence, RotationFrame,
		friend void CALUMIANIMATION_API SortRotationSequence(const RotationSequence& sq, bool highToLow);
		friend bool CALUMIANIMATION_API CompareRotationSequence(const RotationSequence& sq1, const RotationSequence& sq2);)
	VECTORDECF(ScalarSequence, ScalarFrame,
		friend void CALUMIANIMATION_API SortScalarSequence(const ScalarSequence& sq, bool highToLow);
		friend bool CALUMIANIMATION_API CompareScalarSequence(const ScalarSequence& sq1, const ScalarSequence& sq2);)
	VECTORDECF(PrioritySequence, PriorityFrame,
		friend void CALUMIANIMATION_API SortPrioritySequence(const PrioritySequence& sq, bool highToLow);
		friend bool CALUMIANIMATION_API ComparePrioritySequence(const PrioritySequence& sq1, const PrioritySequence& sq2);)

}

	/// @addtogroup extern_c
	/// @{
	/// @addtogroup c_univ_anim
	/// @{

	extern "C" {
		/**
		 * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteRotationFrameC
		 * @param frame
		 * @param x
		 * @param y
		 * @param z
		 * @param w
		 * @return Heap allocated RotationFrame
		 */
		CALUMIANIMATION_API CALUMI::UNIV::RotationFrame* CreateRotationFrameC(uint16_t frame, float x, float y, float z, float w);
		/**
		 * @brief Quaternion creation with @ref CALUMI::Math::EulerDefinition::EulerOrder "Euler Order"
		 * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteRotationFrameC
		 * @param frame
		 * @param x
		 * @param y
		 * @param z
		 * @param order See @ref CALUMI::Math::EulerDefinition::EulerOrder "Euler Order"
		 * @return Heap allocated RotationFrame
		 */
		CALUMIANIMATION_API CALUMI::UNIV::RotationFrame* CreateRotationFrameFromEulerC(uint16_t frame, float x, float y, float z, uint8_t order);
		/**
		 * @param ptr Pointer Reference that is set to nullptr on completion
		 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successfully Deleted
		 */
		CALUMIANIMATION_API int DeleteRotationFrameC(const CALUMI::UNIV::RotationFrame** ptr);
		/**
		 * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteTranslationFrameC
		 * @param frame
		 * @param x
		 * @param y
		 * @param z
		 * @return Heap allocated TranslationFrame
		 */
		CALUMIANIMATION_API CALUMI::UNIV::TranslationFrame* CreateTranslationFrameC(uint16_t frame, double x, double y, double z);
		/**
		 * @param ptr Pointer Reference that is set to nullptr on completion
		 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successfully Deleted
		 */
		CALUMIANIMATION_API int DeleteTranslationFrameC(const CALUMI::UNIV::TranslationFrame** ptr);
		/**
		 * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteScalarFrameC
		 * @param frame
		 * @param scalar
		 * @return Heap allocated ScalarFrame
		 */
		CALUMIANIMATION_API CALUMI::UNIV::ScalarFrame* CreateScalarFrameC(uint16_t frame, float scalar);
		/**
		 * @param ptr Pointer Reference that is set to nullptr on completion
		 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successfully Deleted
		 */
		CALUMIANIMATION_API int DeleteScalarFrameC(const CALUMI::UNIV::ScalarFrame** ptr);
		/**
		 * @warning Heap allocated return value, if not nullptr, must be deleted using DeletePriorityFrameC
		 * @param frame
		 * @param priority
		 * @return Heap allocated PriorityFrame
		 */
		CALUMIANIMATION_API CALUMI::UNIV::PriorityFrame* CreatePriorityFrameC(uint16_t frame, uint8_t priority);
		/**
		 * @param ptr Pointer Reference that is set to nullptr on completion
		 * @return Error Code:\n -1 = Invalid Ptr\n 0 = Successfully Deleted
		 */
		CALUMIANIMATION_API int DeletePriorityFrameC(const CALUMI::UNIV::PriorityFrame** ptr);
		/**
		 *
		 * @param source
		 * @return Frame or 0 if error
		 */
		CALUMIANIMATION_API uint16_t GetFrameFromRotationFrameC(const CALUMI::UNIV::RotationFrame* source);
		/**
		 *
		 * @param source
		 * @return Frame or 0 if error
		 */
		CALUMIANIMATION_API uint16_t GetFrameFromTranslationFrameC(const CALUMI::UNIV::TranslationFrame* source);
		/**
		 *
		 * @param source
		 * @return Frame or 0 if error
		 */
		CALUMIANIMATION_API uint16_t GetFrameFromScalarFrameC(const CALUMI::UNIV::ScalarFrame* source);
		/**
		 *
		 * @param source
		 * @return Frame or 0 if error
		 */
		CALUMIANIMATION_API uint16_t GetFrameFromPriorityFrameC(const CALUMI::UNIV::PriorityFrame* source);
		/**
		 *
		 * @param source
		 * @return Quaternion reference or nullptr if error
		 */
		CALUMIANIMATION_API CALUMI::Math::Quaternion* GetValueFromRotationFrameC(const CALUMI::UNIV::RotationFrame* source);
		/**
		 *
		 * @param source
		 * @return Vector reference or nullptr if error
		 */
		CALUMIANIMATION_API CALUMI::Math::Vector3D* GetValueFromTranslationFrameC(const CALUMI::UNIV::TranslationFrame* source);
		/**
		 *
		 * @param source
		 * @return Scale value or NaN if error
		 */
		CALUMIANIMATION_API float GetValueFromScalarFrameC(const CALUMI::UNIV::ScalarFrame* source);
		/**
		 *
		 * @param source
		 * @return Priority value or 0 if error
		 */
		CALUMIANIMATION_API uint8_t GetValueFromPriorityFrameC(const CALUMI::UNIV::PriorityFrame* source);
	}


	/// @}
	/// @}
