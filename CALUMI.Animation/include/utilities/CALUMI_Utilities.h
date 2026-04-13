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
		BufferObject();
		virtual ~BufferObject();

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

		[[nodiscard]] virtual const char& at(uint64_t idx) const;
		virtual char& at(uint64_t idx);
		virtual void insert(uint64_t pos, uint64_t size, char item);
		virtual void reserve(uint64_t size);
		virtual void resize(uint64_t size);
		virtual void push_back(const char& c);
		virtual void push_back(char&& c);

	private:
		struct PrivateBuffer;
		PrivateBuffer* pBuffer;
	};

	/**
	 * @brief A simple string container for convenience
	 */
	struct CALUMIANIMATION_API StringContainer
	{
		StringContainer(const char* cString);
		StringContainer(uint64_t count, char c);
		StringContainer(const StringContainer& source);
		StringContainer(StringContainer&& source) noexcept;
		StringContainer();
		~StringContainer();

		[[nodiscard]] const char* c_str() const;
		[[nodiscard]] const char* data() const;
		void clear() const;
		[[nodiscard]] bool empty() const;
		[[nodiscard]] uint64_t length(bool includeNull = false) const;

		uint64_t find(const char* s, uint64_t pos = 0) const;

		[[nodiscard]] int compare(const StringContainer& other, bool caseSensitive = true) const noexcept;
		[[nodiscard]] int compare(uint64_t pos, uint64_t len, const StringContainer& other) const;
		[[nodiscard]] int compare(uint64_t pos, uint64_t len, const StringContainer& other, uint64_t subPos, uint64_t subLngth) const;

		void assign(const char* str) const;

		[[nodiscard]] char at(uint64_t idx) const;
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


	protected:
		struct Impl;
		Impl* pImpl;
	};

	/**
	 *
	 * @param indents Amount of spaces
	 * @return A simple, empty string of only ' 's to assist with indentation
	 */
	StringContainer Indent(uint64_t indents);
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

		[[nodiscard]] uint64_t getOffset(uint64_t idx) const;
		[[nodiscard]] uint64_t getFinalOffset() const;
		[[nodiscard]] bool hasOffset(uint64_t idx) const;
		void setFinalOffset(uint64_t offset) const;
		void reserve(uint64_t size) const;
		[[nodiscard]] const char* c_str(uint64_t idx) const;
		[[nodiscard]] uint64_t stringLength(uint64_t idx, bool includeNull = false) const;
		[[nodiscard]] uint64_t size() const;

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
		void push_back(const T&& input);\
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

#pragma region EXTERN "C"
	extern "C" {
	CALUMIANIMATION_API StringContainer* CreateStringContainerC();
	CALUMIANIMATION_API const char* GetStringFromContainerC(const StringContainer* source);
	CALUMIANIMATION_API uint64_t GetStringContainerSizeC(const StringContainer* source);
	CALUMIANIMATION_API void DeleteStringContainerC(const StringContainer* ptr);
	}
#pragma endregion

}

