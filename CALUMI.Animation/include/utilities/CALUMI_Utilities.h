//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include "CALUMI_Common.h"

#define _VECTORTEMPLATE(T)	template struct CALUMIANIMATION_API CALUMI::Utilities::VectorContainer<T>; \
                            // template CALUMI::Utilities::StringContainer CALUMI::Utilities::VectorToJSON(const CALUMI::Utilities::VectorContainer<T>& vec, std::size_t indents);



namespace CALUMI { namespace Utilities {

	

	template<typename T, typename U>
	struct CALUMIANIMATION_API ExpectedContainer
	{
		ExpectedContainer(T& expectedValue);
		ExpectedContainer(T&& expectedValue) noexcept;
		ExpectedContainer();
		~ExpectedContainer();
		void setErrorValue(U uValue);
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
		StringContainer(std::size_t count, char c);
		StringContainer(const StringContainer& source);
		StringContainer(StringContainer&& source) noexcept;
		StringContainer();
		~StringContainer();
		
		const char* c_str() const;
		const char* data() const;
		void clear();
		bool empty();
		std::size_t length(bool includeNull = false) const;

		std::size_t find(const char* s, std::size_t pos = 0) const;

		int compare(const StringContainer& other, bool caseSensitive = true) const noexcept;
		int compare(std::size_t pos, std::size_t len, const StringContainer& other) const;
		int compare(std::size_t pos, std::size_t len, const StringContainer& other, std::size_t subpos, std::size_t sublen) const;

		void assign(const char* str);

		char at(std::size_t idx) const;
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
	Utilities::StringContainer Indent(const std::size_t indents);
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

	struct CALUMIANIMATION_API StringMap
	{
		StringMap();
		~StringMap();

		void push_back(const char* string, std::size_t offset);
		void push_back(const char* string);

		std::size_t getOffset(std::size_t idx);
		std::size_t getFinalOffset();
		bool hasOffset(std::size_t idx);
		void setFinalOffset(std::size_t offset);
		void reserve(std::size_t size);
		const char* c_str(std::size_t idx);
		std::size_t stringLength(std::size_t idx, bool includeNull = false);
		std::size_t size();

	private:
		struct Impl;
		struct Entry;
		Impl* pImpl;
	};


	template<class T>
	struct CALUMIANIMATION_API VectorContainer
	{
		constexpr VectorContainer() noexcept;
		constexpr ~VectorContainer();
		explicit VectorContainer(std::size_t count);
		constexpr VectorContainer(std::size_t count, const T& value);
		constexpr VectorContainer(const VectorContainer& source);
		constexpr VectorContainer(VectorContainer&& source) noexcept;


		constexpr VectorContainer& operator= (const VectorContainer& other);
		constexpr VectorContainer& operator= (VectorContainer&& other) noexcept;


		void push_back(const T& input);
		void push_back(const T&& input);
		void resize(std::size_t n);
		void reserve(std::size_t n);
		void shrink_to_fit();
		const T& at(std::size_t i) const;
		T& at(std::size_t i);
		void clear();
		std::size_t size() const;
		bool empty() const;
		
		void erase(std::size_t pos);
		void insert_r(std::size_t pos, T& item);
		void insert(std::size_t pos, T item);
		void insert(std::size_t pos, std::size_t count, T& item);
		void insert(std::size_t pos, std::size_t count, T item);
		T* data() noexcept;
		const T* data() const noexcept;
		void sort(bool highToLow = false);

		VectorContainer<T> range(std::size_t first, std::size_t last) const;

		std::size_t end() const;

	private:
		struct Impl;
		Impl* pImpl;
	};

	template<typename A, typename B>
	struct CALUMIANIMATION_API PairContainer
	{
		PairContainer();
		PairContainer(A a, B b);
		PairContainer(const PairContainer& input);
		PairContainer(PairContainer&& input) noexcept;
		~PairContainer();

		PairContainer& operator=(const PairContainer& input);

		A getFirst() const;
		void setFirst(const A& input);
		B getSecond() const;
		void setSecond(const B& input);

	private:
		struct Impl;
		Impl* pImpl;
	};

	void AlignBuffer(Utilities::VectorContainer<char>& buffer, unsigned long long& currentIndex, int alignmentSize);

	//Buff Stuff
	void AlignBufferAndRead(Utilities::VectorContainer<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Destination);
	void AlignFillBufferAndWrite(Utilities::VectorContainer<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, const void* Source);

	//Json Stuff
    //template <typename T>
    //Utilities::StringContainer VectorToJSON(const Utilities::VectorContainer<T>& vec, const std::size_t indents = 0);

#pragma region TemplateExplicits
#pragma warning(disable: 4661)
	template struct CALUMIANIMATION_API PairContainer<float, float>;
	template struct CALUMIANIMATION_API VectorContainer<unsigned short>;
    template struct CALUMIANIMATION_API VectorContainer<short>;
    template struct CALUMIANIMATION_API VectorContainer<unsigned long>;
    template struct CALUMIANIMATION_API VectorContainer<long>;
	template struct CALUMIANIMATION_API VectorContainer<char>;
	template struct CALUMIANIMATION_API VectorContainer<const char*>;
	template struct CALUMIANIMATION_API VectorContainer<uint8_t>;
	template struct CALUMIANIMATION_API VectorContainer<int8_t>;
#ifdef _WIN32
    template struct CALUMIANIMATION_API VectorContainer<uint16_t>;
    template struct CALUMIANIMATION_API VectorContainer<int16_t>;
    template struct CALUMIANIMATION_API VectorContainer<int64_t>;
    template struct CALUMIANIMATION_API VectorContainer<uint64_t>;
#endif
	template struct CALUMIANIMATION_API VectorContainer<uint32_t>;
	template struct CALUMIANIMATION_API VectorContainer<int32_t>;
	template struct CALUMIANIMATION_API VectorContainer<float>;
	template struct CALUMIANIMATION_API VectorContainer<double>;
	template struct CALUMIANIMATION_API VectorContainer<StringContainer>;
	template struct CALUMIANIMATION_API VectorContainer<PathContainer>;

	template struct CALUMIANIMATION_API ExpectedContainer<bool, StringContainer>;
    template struct CALUMIANIMATION_API ExpectedContainer<std::size_t, StringContainer>;
	template struct CALUMIANIMATION_API ExpectedContainer<bool, PathContainer>;
	template struct CALUMIANIMATION_API ExpectedContainer<PathContainer, bool>;
	template struct CALUMIANIMATION_API ExpectedContainer<PathContainer, StringContainer>;
	template struct CALUMIANIMATION_API ExpectedContainer<VectorContainer<PathContainer>, StringContainer>;
	template struct CALUMIANIMATION_API ExpectedContainer<StringContainer, StringContainer>;
	template struct CALUMIANIMATION_API ExpectedContainer<StringContainer, bool>;

#pragma warning(default: 4661)
#pragma endregion

#pragma region EXTERN "C"
	extern "C" {
		CALUMIANIMATION_API StringContainer* CreateStringContainerC();
		CALUMIANIMATION_API const char* GetStringFromContainerC(StringContainer* source);
		CALUMIANIMATION_API std::size_t GetStringContainerSizeC(StringContainer* source);
		CALUMIANIMATION_API void DeleteStringContainerC(StringContainer* ptr);
	}
#pragma endregion

}
}

