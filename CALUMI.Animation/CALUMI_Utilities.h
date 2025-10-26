//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"

#define _VECTORTEMPLATE(T)	template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<T>; \
							template CALUMI::Utilities::StringContainer CALUMI::Utilities::VectorToJSON(const CALUMI::Utilities::VectorContainer<T>& vec, size_t indents);

namespace CALUMI { namespace Utilities {

	

	template<typename T, typename U>
	struct CALUMIANIMATION_API ExpectedContainer
	{
		ExpectedContainer(T& expectedValue);
		ExpectedContainer(T&& expectedValue) noexcept;
		ExpectedContainer();
		~ExpectedContainer();
		void SetErrorValue(U uValue);
		void SetValue(T tValue);

		bool has_value() const noexcept;
		U& error() const;
		T& value() const;

	private:
		struct Impl;
		Impl* pImpl;

	};
	
	struct CALUMIANIMATION_API StringContainer
	{
		StringContainer(const char* cString);
		StringContainer(size_t count, char c);
		StringContainer(const StringContainer& source);
		StringContainer(const StringContainer&& source) noexcept;
		StringContainer();
		~StringContainer();
		
		const char* c_str() const;
		const char* data() const;
		void Clear();
		bool Empty();
		size_t Length(bool includeNull = false) const;

		size_t find(const char* s, size_t pos = 0) const;

		int compare(const StringContainer& other, bool caseSensitive = true) const noexcept;
		int compare(size_t pos, size_t len, const StringContainer& other) const;
		int compare(size_t pos, size_t len, const StringContainer& other, size_t subpos, size_t sublen) const;

		char at(size_t idx) const;
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

	private:
		struct Impl;
		Impl* pImpl;
	};

	//String Stuff
	Utilities::StringContainer Indent(const size_t indents);

	struct CALUMIANIMATION_API PathContainer
	{
		PathContainer();
		PathContainer(const char* cString);
		PathContainer(const wchar_t* wString);
		PathContainer(const PathContainer& source);
		PathContainer(const PathContainer&& source) noexcept;
		~PathContainer();

		StringContainer StringContainer() const;
		const wchar_t* w_str() const;
		const char* c_str() const;
		void Clear();
		bool Empty();

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

	struct CALUMIANIMATION_API StringMap
	{
		StringMap();
		~StringMap();

		void push_back(const char* string, size_t offset);
		void push_back(const char* string);

		size_t GetOffset(size_t idx);
		size_t GetFinalOffset();
		bool HasOffset(size_t idx);
		void SetFinalOffset(size_t offset);
		void Reserve(size_t size);
		const char* GetString(size_t idx);
		size_t StringLength(size_t idx, bool includeNull = false);
		size_t Size();

	private:
		struct Impl;
		struct Entry;
		Impl* pImpl;
	};

	template<typename T>
	struct CALUMIANIMATION_API VectorContainer
	{
		VectorContainer() noexcept;
		~VectorContainer();
		VectorContainer(const size_t count);
		VectorContainer(const VectorContainer& source) noexcept;
		VectorContainer(const VectorContainer&& source) noexcept;
		VectorContainer(const T* dataBegin, const T* dataEnd);
		//VectorContainer(const StringContainer& source, bool inclundeNull = false) noexcept;
		VectorContainer& operator= (const VectorContainer& other);
		void push_back(const T& input);
		void push_back(const T&& input);
		void resize(size_t n);
		void reserve(size_t n);
		void shrink_to_fit();
		void fill(T fillValue);
		const T& at(size_t i) const;
		T& at(size_t i);
		void clear();
		size_t size() const;
		bool empty() const;
		
		void erase(size_t pos);
		void insert_r(size_t pos, T& item);
		void insert(size_t pos, T item);
		void insert(size_t pos, size_t count, T& item);
		void insert(size_t pos, size_t count, T item);
		T* data() noexcept;
		const T* data() const noexcept;
		void sort(bool highToLow = false);

		size_t end() const;
		

	private:
		struct Impl;
		Impl* pImpl;
	};

	template<typename A, typename B>
	struct CALUMIANIMATION_API PairContainer
	{
		A first;
		B second;
	};

	void AlignBuffer(Utilities::VectorContainer<char>& buffer, unsigned long long& currentIndex, int alignmentSize);

	//Buff Stuff
	void AlignBufferAndRead(Utilities::VectorContainer<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Destination);
	void AlignFillBufferAndWrite(Utilities::VectorContainer<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Source);

	//Json Stuff
	template <typename T>
	Utilities::StringContainer VectorToJSON(const Utilities::VectorContainer<T>& vec, const size_t indents = 0);


#pragma region TemplateExplicits
#pragma warning(disable: 4661)
	template struct CALUMIANIMATION_API VectorContainer<unsigned short>;
	template struct CALUMIANIMATION_API VectorContainer<short>;
	template struct CALUMIANIMATION_API VectorContainer<unsigned long>;
	template struct CALUMIANIMATION_API VectorContainer<long>;
	template struct CALUMIANIMATION_API VectorContainer<char>;
	template struct CALUMIANIMATION_API VectorContainer<uint8_t>;
	template struct CALUMIANIMATION_API VectorContainer<int8_t>;
	template struct CALUMIANIMATION_API VectorContainer<uint16_t>;
	template struct CALUMIANIMATION_API VectorContainer<int16_t>;
	template struct CALUMIANIMATION_API VectorContainer<uint32_t>;
	template struct CALUMIANIMATION_API VectorContainer<int32_t>;
	template struct CALUMIANIMATION_API VectorContainer<float>;
	template struct CALUMIANIMATION_API VectorContainer<double>;
	template struct CALUMIANIMATION_API VectorContainer<StringContainer>;
	template struct CALUMIANIMATION_API VectorContainer<PathContainer>;

	template struct CALUMIANIMATION_API ExpectedContainer<bool, StringContainer>;
	template struct CALUMIANIMATION_API ExpectedContainer<size_t, StringContainer>;
	template struct CALUMIANIMATION_API ExpectedContainer<bool, PathContainer>;
	template struct CALUMIANIMATION_API ExpectedContainer<PathContainer, bool>;
	template struct CALUMIANIMATION_API ExpectedContainer<PathContainer, StringContainer>;
	template struct CALUMIANIMATION_API ExpectedContainer<VectorContainer<PathContainer>, StringContainer>;
	template struct CALUMIANIMATION_API ExpectedContainer<StringContainer, StringContainer>;
	template struct CALUMIANIMATION_API ExpectedContainer<StringContainer, bool>;
#pragma warning(default: 4661)
#pragma endregion

	

}
}

