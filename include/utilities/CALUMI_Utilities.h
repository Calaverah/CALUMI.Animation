//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once
#include "CALUMI_Common.h"

namespace CALUMI::Utilities
{
	/**
	 * @brief A buffer object is a simple char vector with basic functionality meant for
	 * reading/parsing/aligning to a binary source during input
	 * @details This buffer object can be used in larger classes for data handling with alignment
	 */
	struct CALUMIANIMATION_API BufferObject
	{
		/// @name Initializer
		/// @{
		BufferObject();
		virtual ~BufferObject();
		/// @}
		/// @name Data
		/// @{

		/**
		 * @return Non-const ptr to the beginning of this vector
		 */
		[[nodiscard]] virtual char* data() const;
		/**
		 * @return The index of the vector.end, relative to its beginning
		 * @brief i.e. _buffer.end() - _buffer.begin()
		 */
		[[nodiscard]] virtual uint64_t endPos() const;
		/**
		 * @return The current buffer size
		 */
		[[nodiscard]] virtual uint64_t size() const;

		/**
		 * @param idx
		 * @return
		 */
		[[nodiscard]] virtual const char& at(uint64_t idx) const;
		/**
		 * @param idx
		 * @return
		 */
		virtual char& at(uint64_t idx);
		/**
		 * @param pos
		 * @param size
		 * @param item
		 */
		virtual void insert(uint64_t pos, uint64_t size, char item);
		/**
		 * @param size
		 */
		virtual void reserve(uint64_t size);
		/**
		 * @param size
		 */
		virtual void resize(uint64_t size);
		/**
		 * @param c
		 */
		virtual void push_back(const char& c);
		/**
		 * @param c
		 */
		virtual void push_back(char&& c);
		/// @}

	private:
		struct PrivateBuffer;
		PrivateBuffer* pBuffer;
	};

	/**
	 * @brief A simple string container for convenience
	 */
	struct CALUMIANIMATION_API StringContainer
	{
		/// @name Initialization
		/// @{

		/**
		 * @param cString
		 */
		StringContainer(const char* cString);
		/**
		 * @brief Mimics the std::string constructor
		 * @param count
		 * @param c
		 */
		StringContainer(uint64_t count, char c);
		/**
		 * @param source
		 */
		StringContainer(const StringContainer& source);
		/**
		 * @param source
		 */
		StringContainer(StringContainer&& source) noexcept;
		StringContainer();
		~StringContainer();

		/// @}
		/// @name Data
		/// @{

		/**
		 * @return
		 */
		[[nodiscard]] const char* c_str() const;
		/**
		 * @return
		 */
		[[nodiscard]] const char* data() const;
		/**
		 * @brief Clears the string
		 */
		void clear() const;
		/**
		 * @return
		 */
		[[nodiscard]] bool empty() const;
		/**
		 * @param includeNull
		 * @return
		 */
		[[nodiscard]] uint64_t length(bool includeNull = false) const;

		/**
		 * @param str
		 */
		void assign(const char* str) const;

		/**
		 * @param s
		 * @param pos
		 * @return
		 */
		uint64_t find(const char* s, uint64_t pos = 0) const;

		/**
		 * @param idx
		 * @return
		 */
		[[nodiscard]] char at(uint64_t idx) const;
		/// @}
		/// @name Comparisons
		/// @{

		[[nodiscard]] int compare(const StringContainer& other, bool caseSensitive = true) const noexcept;
		[[nodiscard]] int compare(uint64_t pos, uint64_t len, const StringContainer& other) const;
		[[nodiscard]] int compare(uint64_t pos, uint64_t len, const StringContainer& other, uint64_t subPos, uint64_t subLength) const;
		/// @}
		/// @name Operators
		/// @{


		StringContainer& operator+=(const char* other);
		StringContainer& operator+=(const StringContainer& other);
		StringContainer& operator=(const StringContainer& other);
		StringContainer& operator=(const char* other);
		StringContainer operator+(const char* other) const;
		StringContainer operator+(const StringContainer& other) const;
		bool operator==(const StringContainer& other) const;
		bool operator!=(const StringContainer& other) const;
		bool operator==(const char* other) const;
		bool operator!=(const char* other) const;
		bool operator<(const StringContainer& other) const;
		bool operator>(const StringContainer& other) const;

		/// @}

	protected:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 * @brief Extremely simple string check for numeric characters
	 * @param str
	 * @return
	 */
	bool IsNumeric(const StringContainer& str);

	/**
	 * @brief Simple wrapper for the std::filesystem::path class. Has options for wide or single space character output.
	 */
	struct CALUMIANIMATION_API PathContainer
	{
		PathContainer();
		PathContainer(const char* cString);
		PathContainer(const wchar_t* wString);
		PathContainer(const PathContainer& source);
		PathContainer(const PathContainer&& source) noexcept;
		~PathContainer();

		[[nodiscard]] StringContainer strContainer() const;
		[[nodiscard]] const wchar_t* w_str() const;
		[[nodiscard]] const char* c_str() const;
		void clear() const;
		[[nodiscard]] bool empty() const;

		[[nodiscard]] bool has_extension() const;
		[[nodiscard]] bool has_filename() const;
		[[nodiscard]] PathContainer filename() const;
		[[nodiscard]] PathContainer stem() const;
		[[nodiscard]] bool has_relativepath() const;
		[[nodiscard]] bool has_parentpath() const;
		[[nodiscard]] PathContainer extension() const;

		PathContainer& replace_extension(const PathContainer& replacement);

		PathContainer& operator=(const char* other);
		PathContainer& operator=(const wchar_t* other);
		PathContainer& operator=(const PathContainer& other);
		PathContainer& operator=(const StringContainer& other);
		PathContainer& operator/=(const PathContainer& other);
		PathContainer& operator/=(const StringContainer& other);
		PathContainer& operator/=(const char* other);
		PathContainer& operator/=(const wchar_t* other);
		bool operator==(const PathContainer& other) const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 * @brief String Vector wrapper with added data for serialization assistance
	 */
	struct CALUMIANIMATION_API StringList
	{
		StringList();
		StringList(const StringList& other);
		~StringList();

		void push_back(const char* string, uint64_t offset) const;
		void push_back(const char* string) const;

		[[nodiscard]] uint64_t offset(unsigned int idx) const;
		[[nodiscard]] uint64_t finalOffset() const;
		[[nodiscard]] bool hasOffset(unsigned int idx) const;
		void setFinalOffset(uint64_t offset) const;
		void reserve(unsigned int size) const;
		[[nodiscard]] const char* c_str(unsigned int idx) const;
		[[nodiscard]] uint64_t stringLength(unsigned int idx, bool includeNull = false) const;
		[[nodiscard]] unsigned int size() const;
		[[nodiscard]] int64_t find(const char* string, int64_t defValue = -1) const;

		StringList& operator=(const StringList& other);

		[[nodiscard]] bool empty() const;

	private:
		struct Impl;
		struct Entry;
		Impl* pImpl;
	};


#define VECTORDEC(CLASS, T) VECTORDECF(CLASS, T, )

#define VECTORDECF(CLASS, T, FRIENDDECL) \
struct CALUMIANIMATION_API CLASS \
	{ \
        CLASS() noexcept; \
        ~CLASS(); \
		explicit CLASS(uint64_t count);\
        CLASS(uint64_t count, const T& value);\
        CLASS(const CLASS& source);\
        CLASS(CLASS&& source) noexcept;\
		\
		\
        CLASS& operator= (const CLASS& other);\
        CLASS& operator= (CLASS&& other) noexcept;\
		\
		\
		void push_back(const T& input);\
		void push_back(T&& input) noexcept;\
		void resize(uint64_t n);\
		void reserve(uint64_t n);\
		void shrink_to_fit();\
		const T& at(uint64_t i) const;\
		T& at(uint64_t i);\
		void clear();\
		uint64_t size() const;\
		bool empty() const;\
		\
		void erase(uint64_t pos);\
		void insert_r(uint64_t pos, const T& item);\
		void insert(uint64_t pos, T&& item);\
		void insert(uint64_t pos, uint64_t count, const T& item);\
		void insert(uint64_t pos, uint64_t count, T&& item);\
		T* data() noexcept;\
		const T* data() const noexcept; \
		\
		CLASS range(uint64_t first, uint64_t last) const; \
		\
		uint64_t end() const;\
		\
	private:\
		struct Impl;\
		Impl* pImpl;\
    private:\
        FRIENDDECL\
    };

	VECTORDEC(U8Vector, uint8_t)
	VECTORDEC(S8Vector, int8_t)
	VECTORDEC(U16Vector, uint16_t)
	VECTORDEC(S16Vector, int16_t)
	VECTORDEC(U32Vector, uint32_t)
	VECTORDEC(S32Vector, int32_t)
	VECTORDEC(U64Vector, uint64_t)
	VECTORDEC(S64Vector, int64_t)
	VECTORDEC(FloatVector, float)
	VECTORDEC(DoubleVector, double)
	VECTORDEC(CharVector, char)




	void AlignBuffer(unsigned long long& currentIndex, int alignmentSize);
	void AlignBufferAndRead(BufferObject& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Destination);
	void AlignFillBufferAndWrite(BufferObject& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, const void* Source);
}

/// @addtogroup extern_c
/// @{
/// @defgroup extern_c_utilities Utilities
/// @{

	extern "C" {
	/**
	 * @brief Creates an empty, heap allocated, string container
	 * @warning Heap allocated return value, if not nullptr, must be deleted using DeleteStringContainerC
	 * @return Ptr to a heap allocated string container
	 */
	CALUMIANIMATION_API CALUMI::Utilities::StringContainer* CreateStringContainerC();
	/**
	 * @brief
	 * @param source
	 * @return nullptr if error or cString contained within the string container
	 */
	CALUMIANIMATION_API const char* GetStringFromContainerC(const CALUMI::Utilities::StringContainer* source);
	/**
	 *
	 * @param source
	 * @return length of the string or simply 0 if there is an error
	 */
	CALUMIANIMATION_API uint64_t GetStringContainerSizeC(const CALUMI::Utilities::StringContainer* source);
	/**
	 * @param ptr Reference to the ptr of the string container, the ptr will be set to nullptr upon completion if
	 * successful
	 */
	CALUMIANIMATION_API void DeleteStringContainerC(const CALUMI::Utilities::StringContainer** ptr);
	}

/// @}
/// @}
