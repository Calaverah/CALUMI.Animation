//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"



namespace CALUMI { namespace Utilities {

	

	struct CALUMIANIMATION_API BufferObject
	{
		BufferObject();
		virtual ~BufferObject();

		virtual char* data() const;
		virtual uint64_t endPos() const;
		virtual uint64_t size() const;

		virtual const char& at(uint64_t idx) const;
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

	struct CALUMIANIMATION_API StringContainer
	{
		StringContainer(const char* cString);
		StringContainer(uint64_t count, char c);
		StringContainer(const StringContainer& source);
		StringContainer(StringContainer&& source) noexcept;
		StringContainer();
		~StringContainer();
		
		const char* c_str() const;
		const char* data() const;
		void clear();
		bool empty();
		uint64_t length(bool includeNull = false) const;

		uint64_t find(const char* s, uint64_t pos = 0) const;

		int compare(const StringContainer& other, bool caseSensitive = true) const noexcept;
		int compare(uint64_t pos, uint64_t len, const StringContainer& other) const;
		int compare(uint64_t pos, uint64_t len, const StringContainer& other, uint64_t subpos, uint64_t sublen) const;

		void assign(const char* str);

		char at(uint64_t idx) const;
		StringContainer& operator+=(const char* other);
		StringContainer& operator+=(const StringContainer& other);
		StringContainer& operator=(const StringContainer& other);
		StringContainer& operator=(const char* other);
		StringContainer operator+(const char* other);
		StringContainer operator+(const StringContainer& other);
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

	//String Stuff
	Utilities::StringContainer Indent(const uint64_t indents);
	bool IsNumeric(const Utilities::StringContainer& str);

	struct CALUMIANIMATION_API PathContainer
	{
		PathContainer();
		PathContainer(const char* cString);
		PathContainer(const wchar_t* wString);
		PathContainer(const PathContainer& source);
		PathContainer(const PathContainer&& source) noexcept;
		~PathContainer();

		StringContainer strContainer() const;
		const wchar_t* w_str() const;
		const char* c_str() const;
		void clear();
		bool empty();

		bool has_extension() const;
		bool has_filename() const;
		PathContainer filename() const;
		PathContainer stem() const;
		bool has_relativepath() const;
		bool has_parentpath() const;
		PathContainer extension() const;

		PathContainer& replace_extension(const PathContainer& replacement);

		PathContainer& operator=(const char* other);
		PathContainer& operator=(const wchar_t* other);
		PathContainer& operator=(const PathContainer& other);
		PathContainer& operator=(const Utilities::StringContainer& other);
		PathContainer& operator/=(const PathContainer& other);
		PathContainer& operator/=(const Utilities::StringContainer& other);
		PathContainer& operator/=(const char* other);
		PathContainer& operator/=(const wchar_t* other);
		bool operator==(const PathContainer& other) const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	struct CALUMIANIMATION_API StringList
	{
		StringList();
		StringList(const StringList& other);
		~StringList();

		void push_back(const char* string, uint64_t offset);
		void push_back(const char* string);

		uint64_t getOffset(uint64_t idx);
		uint64_t getFinalOffset();
		bool hasOffset(uint64_t idx);
		void setFinalOffset(uint64_t offset);
		void reserve(uint64_t size);
		const char* c_str(uint64_t idx) const;
		uint64_t stringLength(uint64_t idx, bool includeNull = false) const;
		uint64_t size() const;

		StringList& operator=(const StringList& other);

		bool empty() const;

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



	void AlignBuffer(BufferObject& buffer, unsigned long long& currentIndex, int alignmentSize);

	//Buff Stuff
	void AlignBufferAndRead(BufferObject& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Destination);
	void AlignFillBufferAndWrite(BufferObject& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, const void* Source);

#pragma region EXTERN "C"
	extern "C" {
		CALUMIANIMATION_API StringContainer* CreateStringContainerC();
		CALUMIANIMATION_API const char* GetStringFromContainerC(StringContainer* source);
		CALUMIANIMATION_API uint64_t GetStringContainerSizeC(StringContainer* source);
		CALUMIANIMATION_API void DeleteStringContainerC(StringContainer* ptr);
	}
#pragma endregion

}
}

