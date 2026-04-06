//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include "internalplatform.h"

#include "utilities/CALUMI_Utilities.h"
//It is important to include the scene headers directly for explicit instantiation of the templates
#include "univ/UNIV_AnimationScene.h"
#include "sfbgs/SFBGS_AnimationScene.h"

//Rest of the headers can go here
#include <cstring>
#include <expected>
#include <string>
#include <vector>
#include <filesystem>
#include <cctype>
#include <cstdint>
#include "internal/internalvectordef.h"

namespace CALUMI {
	namespace Utilities {

		void AlignBuffer(BufferObject& buffer, unsigned long long& currentIndex, int alignmentSize)
		{
			if (alignmentSize < 1) return; //if alignment size is for some reason 0, we skip this output and move on
			if (currentIndex % alignmentSize < 1) return; //we are aligned, move on

			int offset = (alignmentSize - (currentIndex % alignmentSize));

			currentIndex += offset;
		}

		static void _AlignFillBuffer(BufferObject& buffer, unsigned long long& currentIndex, int alignmentSize)
		{
			unsigned long long startingIndex = currentIndex;
			AlignBuffer(buffer, currentIndex, alignmentSize);
			buffer.insert(buffer.endPos(), (currentIndex - startingIndex), 0);
		}

        // template<typename T>
        // CALUMI::Utilities::StringContainer VectorToJSON(const CALUMI::Utilities::VectorContainer<T>& vec, const uint64_t indents)
        // {
        // 	if (vec.empty()) {
        // 		return " []";
        // 	}

        // 	Utilities::StringContainer output = "[\n";
        // 	for (uint64_t i = 0; i < vec.size(); i++) {

        // 		if constexpr (std::is_pointer_v<T>)
        // 		{ output += vec.at(i)->ToJSON(indents + 1).c_str(); }
        // 		else
        // 		{ output += vec.at(i).ToJSON(indents + 1).c_str(); }
				
        // 		if (i < vec.size() - 1) {
        // 			output += ",\n";
        // 		}
        // 	}
        // 	output += std::format("\n{}]", Indent(indents).c_str()).c_str();
        // 	return output;
        // }


		/// <summary>
		/// This method will assign a value from a buffer with memcpy, however it will consider padding from the byte array buffer first.
		/// When finished it will advance the currentPosition index by the appropriate amount of spaces
		/// </summary>
		/// <param name="currentPosition"></param>
		/// <param name="currentIndex"></param>
		/// <param name="alignmentSize"></param>
		/// <param name="Destination"></param>
		void AlignBufferAndRead(BufferObject& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Destination)
		{
			AlignBuffer(buffer, currentIndex, alignmentSize);
			std::memcpy(Destination, &buffer.at(currentIndex), variableSize);

			currentIndex += variableSize;
		}

		/// <summary>
		/// This method will assign a value to a buffer with memcpy, however it will consider padding
		/// This method is to be used when a buffer is declared but sizing is unknown. Blank values will be inserted
		/// at the end of the vector to be overwritten
		/// </summary>
		/// <param name="currentPosition"></param>
		/// <param name="currentIndex"></param>
		/// <param name="alignmentSize"></param>
		/// <param name="Destination"></param>
		void AlignFillBufferAndWrite(BufferObject& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, const void* Source)
		{
			_AlignFillBuffer(buffer, currentIndex, alignmentSize);
			//if source is a short but needs to be a byte, the variableSize will take only the first byte which in Little Endian, is any value under 255
			buffer.insert(buffer.endPos(), static_cast<size_t>(variableSize), 0);
			std::memcpy(&buffer.at(currentIndex), Source, variableSize);

			currentIndex += variableSize;
		}

		Utilities::StringContainer Indent(const uint64_t indents) {
			return Utilities::StringContainer(indents * 2, ' ');
		}

		bool IsNumeric(const Utilities::StringContainer& str)
		{
			for (uint64_t i = 0; i < str.length(); i++)
			{
				char _char = str.at(i);
				if (_char != '-' && _char != '.' && !std::isdigit(_char)) return false;
			}
			return true;
		}

#pragma region PathContainer
		struct PathContainer::Impl
		{
			std::filesystem::path path;
#ifdef _WIN32
			std::string cStringHolder;
#else
			std::wstring wStringHolder;
#endif
		};
		void PathContainer::clear()
		{
			pImpl->path.clear();
		}
		bool PathContainer::empty()
		{
			return pImpl->path.empty();
		}

		bool PathContainer::has_extension() const
		{
			return pImpl->path.has_extension();
		}
		bool PathContainer::has_filename() const
		{
			return pImpl->path.has_filename();
		}
		PathContainer PathContainer::filename() const
		{
			return pImpl->path.filename().c_str();
		}
		PathContainer PathContainer::stem() const
		{
			return pImpl->path.stem().c_str();
		}
		bool PathContainer::has_relativepath() const
		{
			return pImpl->path.has_relative_path();
		}
		bool PathContainer::has_parentpath() const
		{
			return pImpl->path.has_parent_path();
		}
		PathContainer PathContainer::extension() const
		{
			PathContainer output;
			output.pImpl->path = pImpl->path.extension();
			return output;
		}

		PathContainer& PathContainer::replace_extension(const PathContainer& replacement)
		{
			pImpl->path.replace_extension(replacement.pImpl->path);
			return *this;
		}

		StringContainer PathContainer::strContainer() const
		{
			Utilities::StringContainer output(pImpl->path.string().c_str());
			return output;
		}

		const wchar_t* PathContainer::w_str() const
		{
#ifdef _WIN32
			return pImpl->path.c_str();
#else
            pImpl->wStringHolder = pImpl->path.wstring();
			return pImpl->wStringHolder.c_str();
#endif
		}

		const char* PathContainer::c_str() const
		{
#ifdef _WIN32
			pImpl->cStringHolder = pImpl->path.string();
			return pImpl->cStringHolder.c_str();
#else
			return pImpl->path.c_str();
#endif
		}

		PathContainer::PathContainer()
		{
			pImpl = new Impl;
		}

		PathContainer::PathContainer(const char* cString)
		{
			pImpl = new Impl;
			pImpl->path = cString;
		}

		PathContainer::PathContainer(const wchar_t* wString)
		{
			pImpl = new Impl;
			pImpl->path = wString;
		}

		PathContainer::PathContainer(const PathContainer& source)
		{
			pImpl = new Impl;
			pImpl->path = source.pImpl->path;
		}

		PathContainer::PathContainer(const PathContainer&& source) noexcept
		{
			pImpl = new Impl;
			pImpl->path = source.pImpl->path;
		}

		PathContainer::~PathContainer()
		{
			if (pImpl)
				delete pImpl;
		}
		bool PathContainer::operator==(const PathContainer& other) const
		{
			return pImpl->path == other.pImpl->path;
		}
		PathContainer& PathContainer::operator=(const PathContainer& other)
		{
			pImpl->path = other.pImpl->path;
			return *this;
		}
		PathContainer& PathContainer::operator=(const Utilities::StringContainer& other)
		{
			pImpl->path = other.c_str();
			return *this;
		}
		PathContainer& PathContainer::operator/=(const PathContainer& other)
		{
			pImpl->path /= other.pImpl->path;
			return *this;
		}
		PathContainer& PathContainer::operator/=(const Utilities::StringContainer& other)
		{
			pImpl->path /= other.c_str();
			return *this;
		}
		PathContainer& PathContainer::operator/=(const char* other)
		{
			pImpl->path /= other;
			return *this;
		}
		PathContainer& PathContainer::operator/=(const wchar_t* other)
		{
			pImpl->path /= other;
			return *this;
		}
		PathContainer& PathContainer::operator=(const char* other)
		{
			pImpl->path = other;
			return *this;
		}
		PathContainer& PathContainer::operator=(const wchar_t* other)
		{
			pImpl->path = other;
			return *this;
		}
#pragma endregion
		
#pragma region StringContainer
		struct StringContainer::Impl
		{
			std::string string;
		};
		StringContainer StringContainer::operator+(const char* other)
		{
			StringContainer output(pImpl->string.c_str());
			output += other;
			return output;
		}
		StringContainer StringContainer::operator+(const StringContainer& other)
		{
			StringContainer output(pImpl->string.c_str());
			output += other;
			return output;
		}
		bool StringContainer::operator==(const StringContainer& other) const
		{
			return pImpl->string == other.pImpl->string;
		}
		bool StringContainer::operator!=(const StringContainer& other) const
		{
			return pImpl->string != other.pImpl->string;
		}
		bool StringContainer::operator==(const char* other) const
		{
			return pImpl->string == other;
		}
		bool StringContainer::operator!=(const char* other) const
		{
			return pImpl->string != other;
		}
		bool StringContainer::operator<(const StringContainer& other) const
		{
			return pImpl->string < other.pImpl->string;
		}
		bool StringContainer::operator>(const StringContainer& other) const
		{
			return pImpl->string > other.pImpl->string;
		}
		void StringContainer::clear()
		{
			pImpl->string.clear();
		}
		bool StringContainer::empty()
		{
			return pImpl->string.empty();
		}
		char StringContainer::at(uint64_t idx) const
		{
			return pImpl->string.at(idx);
		}
		uint64_t StringContainer::length(bool includeNull) const
		{
			uint64_t output = includeNull ? 1 : 0;
			output += pImpl->string.length();
			return output;
		}
		uint64_t StringContainer::find(const char* s, uint64_t pos) const
		{
			return pImpl->string.find(s,pos);
		}
		int StringContainer::compare(const StringContainer& other, bool caseSensitive) const noexcept
		{
			if (!caseSensitive)
			{
				return SCOMPARE(c_str(), other.c_str());
			}

			return pImpl->string.compare(other.pImpl->string);
		}
		int StringContainer::compare(uint64_t pos, uint64_t len, const StringContainer& other) const
		{
			return pImpl->string.compare(pos,len,other.pImpl->string);
		}
		int StringContainer::compare(uint64_t pos, uint64_t len, const StringContainer& other, uint64_t subpos, uint64_t sublen) const
		{
			return pImpl->string.compare(pos,len,other.pImpl->string,subpos,sublen);
		}
		void StringContainer::assign(const char* str)
		{
			pImpl->string.assign(str);
		}
		StringContainer& StringContainer::operator=(const char* other)
		{
			pImpl->string = other;
			return *this;
		}
		StringContainer::StringContainer(const char* cString)
		{
			pImpl = new Impl;
			pImpl->string = cString;
		}
		StringContainer::StringContainer(uint64_t count, char c)
		{
			pImpl = new Impl;
			pImpl->string = std::string(count, c);
		}
		StringContainer::StringContainer(const StringContainer& source)
		{
			pImpl = new Impl;
			pImpl->string = source.pImpl->string;
		}
		StringContainer::StringContainer(StringContainer&& source) noexcept
		{
			pImpl = new Impl;
			pImpl->string = source.pImpl->string;
		}
		StringContainer::StringContainer()
		{
			pImpl = new Impl;
		}
		StringContainer::~StringContainer()
		{
			if (pImpl)
				delete pImpl;
		}
		const char* StringContainer::c_str() const
		{
			return pImpl->string.c_str();
		}
		const char* StringContainer::data() const
		{
			return pImpl->string.data();
		}
		StringContainer& Utilities::StringContainer::operator+=(const char* other)
		{
			pImpl->string += other;
			return *this;
		}
		StringContainer& StringContainer::operator+=(const StringContainer& other)
		{
			pImpl->string += other.pImpl->string;
			return *this;
		}
		StringContainer& StringContainer::operator=(const StringContainer& other)
		{
			pImpl->string = other.pImpl->string;
			return *this;
		}
#pragma endregion


		

#pragma region Vectors


		VECTORDEF(U8Vector, uint8_t)
		VECTORDEF(S8Vector, int8_t)
		VECTORDEF(U16Vector, uint16_t)
		VECTORDEF(S16Vector, int16_t)
		VECTORDEF(U32Vector, uint32_t)
		VECTORDEF(S32Vector, int32_t)
		VECTORDEF(U64Vector, uint64_t)
		VECTORDEF(S64Vector, int64_t)
		VECTORDEF(FloatVector, float)
		VECTORDEF(DoubleVector, double)
		VECTORDEF(CharVector, char)

		


#pragma endregion

#pragma region StringList
		struct StringList::Entry
		{
			std::string string;
			uint64_t offset = 0;
			bool hasOffset = false;
			Entry() = default;
			Entry(const Entry& source)
			{
				*this = source;
			}
			Entry(Entry&& source) noexcept
			{
				*this = source;
			}
			Entry(const char* cStr, uint64_t ofs) noexcept
			{
				string = cStr;
				offset = ofs;
				hasOffset = true;
			}
			Entry(const char* cStr) noexcept
			{
				string = cStr;
				offset = 0;
				hasOffset = false;
			}
			Entry& operator=(const Entry& other)
			{
				string = other.string;
				offset = other.offset;
				hasOffset = other.hasOffset;
				return *this;
			}
		};

		struct StringList::Impl
		{
			std::vector<Entry> strings;
			uint64_t finalOffset = 0;
		};
		
		StringList::StringList()
		{
			pImpl = new Impl;
		}
		StringList::StringList(const StringList& other) : StringList()
		{
			*this = other;
		}
		StringList::~StringList()
		{
			if (pImpl)
			{
				delete pImpl;
				pImpl = nullptr;
			}
		}
		StringList& StringList::operator=(const StringList& other)
		{
			pImpl->strings.reserve(other.pImpl->strings.size());

			for (const auto& entry : other.pImpl->strings)
			{
				pImpl->strings.push_back(entry);
			}

			pImpl->finalOffset = other.pImpl->finalOffset;
			return *this;
		}
		void StringList::push_back(const char* string, uint64_t offset)
		{
			pImpl->strings.push_back(Entry(string, offset));
		}
		void StringList::push_back(const char* string)
		{
			pImpl->strings.push_back(Entry(string));
		}
		uint64_t StringList::getOffset(uint64_t idx)
		{
			return pImpl->strings.at(idx).offset;
		}
		uint64_t StringList::getFinalOffset()
		{
			return pImpl->finalOffset;
		}
		bool StringList::hasOffset(uint64_t idx)
		{
			return pImpl->strings.at(idx).hasOffset;
		}
		void StringList::setFinalOffset(uint64_t offset)
		{
			pImpl->finalOffset = offset;
		}
		void StringList::reserve(uint64_t size)
		{
			pImpl->strings.reserve(size);
		}
		const char* StringList::c_str(uint64_t idx) const
		{
			return pImpl->strings.at(idx).string.c_str();
		}
		uint64_t StringList::stringLength(uint64_t idx, bool includeNull) const
		{
			uint64_t output = includeNull ? 1 : 0;
			output += pImpl->strings.at(idx).string.length();
			return output;
		}
		uint64_t StringList::size() const
		{
			return pImpl->strings.size();
		}
		bool StringList::empty() const
		{
			return pImpl->strings.empty();
		}
#pragma endregion

#pragma region ExpectedContainer
//
//		template<typename T, typename U>
//		struct ExpectedContainer<T, U>::Impl
//		{
//			std::expected<T, U> expected;
//			Impl() = default;
//			Impl(T& tValue)
//			{
//				expected = std::expected<T,U>(tValue);
//			}
//		};
//
//
//		template<typename T, typename U>
//		ExpectedContainer<T, U>::ExpectedContainer(T& expectedValue)
//		{
//			pImpl = new Impl(expectedValue);
//		}
//
//		template<typename T, typename U>
//		ExpectedContainer<T, U>::ExpectedContainer(T&& expectedValue) noexcept
//		{
//			pImpl = new Impl(expectedValue);
//		}
//
//		template<typename T, typename U>
//		ExpectedContainer<T, U>::ExpectedContainer()
//		{
//			pImpl = new Impl;
//		}
//
//		template<typename T, typename U>
//		bool ExpectedContainer<T, U>::has_value() const noexcept
//		{
//			return pImpl->expected.has_value();
//		}
//		template<typename T, typename U>
//		U& ExpectedContainer<T, U>::error() const
//		{
//			return pImpl->expected.error();
//		}
//		template<typename T, typename U>
//		T& ExpectedContainer<T, U>::value() const
//		{
//			return pImpl->expected.value();
//		}
//
//		template<typename T, typename U>
//		ExpectedContainer<T, U>::~ExpectedContainer()
//		{
//			if (pImpl)
//				delete pImpl;
//		}
//		/*template<typename T, typename U>
//		void ExpectedContainer<T, U>::SetErrorValue_R(const U& uValue)
//		{
//			pImpl->expected = std::unexpected(uValue);
//		}*/
//		template<typename T, typename U>
//		void ExpectedContainer<T, U>::setErrorValue(U uValue)
//		{
//			pImpl->expected = std::unexpected(uValue);
//		}
//		template<typename T, typename U>
//		void ExpectedContainer<T, U>::SetValue(T tValue)
//		{
//			pImpl->expected = tValue;
//		}
#pragma endregion

#pragma region PAIR
	//template<typename A, typename B>
	//struct PairContainer<A, B>::Impl
	//{
	//	A first;
	//	B second;
	//};

	//template<typename A, typename B>
	//PairContainer<A, B>::PairContainer()
	//{
	//	pImpl = new Impl;
	//}

	//template<typename A, typename B>
	//PairContainer<A, B>::PairContainer(A a, B b)
	//{
	//	pImpl = new Impl;
	//	pImpl->first = a;
	//	pImpl->second = b;
	//}

	//template<typename A, typename B>
	//PairContainer<A, B>::PairContainer(const PairContainer& input)
	//{
	//	pImpl = new Impl;
	//	pImpl->first = input.pImpl->first;
	//	pImpl->second = input.pImpl->second;
	//}

	//template<typename A, typename B>
	//PairContainer<A, B>::PairContainer(PairContainer&& input) noexcept
	//{
	//	pImpl = new Impl;
	//	pImpl->first = input.pImpl->first;
	//	pImpl->second = input.pImpl->second;
	//}

	//template<typename A, typename B>
	//PairContainer<A, B>::~PairContainer()
	//{
	//	if (pImpl)
	//		delete pImpl;
	//}

	//template<typename A, typename B>
	//PairContainer<A, B>& PairContainer<A, B>::operator=(const PairContainer& input)
	//{
	//	pImpl->first = input.pImpl->first;
	//	pImpl->second = input.pImpl->second;
	//	return *this;
	//}

	//template<typename A, typename B>
	//A PairContainer<A, B>::getFirst() const
	//{
	//	return pImpl->first;
	//}

	//template<typename A, typename B>
	//B PairContainer<A, B>::getSecond() const
	//{
	//	return pImpl->second;
	//}

	//template<typename A, typename B>
	//void PairContainer<A, B>::setFirst(const A& a)
	//{
	//	pImpl->first = a;
	//}

	//template<typename A, typename B>
	//void PairContainer<A, B>::setSecond(const B& b)
	//{
	//	pImpl->second = b;
	//}

#pragma endregion


#pragma region EXTERN"C"

	StringContainer* CreateStringContainerC()
	{
		return new StringContainer();
	}

	const char* GetStringFromContainerC(StringContainer* source)
	{
		return source->c_str();
	}

	uint64_t GetStringContainerSizeC(StringContainer* source)
	{
		return source->length();
	}

	void DeleteStringContainerC(StringContainer* ptr)
	{
		if (ptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}
#pragma endregion

	struct BufferObject::PrivateBuffer
	{
		std::vector<char> _buffer;
	};

	BufferObject::BufferObject() : pBuffer(new PrivateBuffer()) { }

	BufferObject::~BufferObject()
	{
		if (pBuffer)
		{
			delete pBuffer;
			pBuffer = nullptr;
		}
	}

	char* BufferObject::data() const
	{
		return pBuffer->_buffer.data();
	}

	uint64_t BufferObject::endPos() const
	{
		return pBuffer->_buffer.end() - pBuffer->_buffer.begin();
	}

	uint64_t BufferObject::size() const
	{
		return pBuffer->_buffer.size();
	}

	const char& BufferObject::at(uint64_t idx) const
	{
		return pBuffer->_buffer.at(idx);
	}

	char& BufferObject::at(uint64_t idx)
	{
		return pBuffer->_buffer.at(idx);
	}

	void BufferObject::insert(uint64_t pos, uint64_t size, char item)
	{
		if ((pBuffer->_buffer.begin() + pos) >= pBuffer->_buffer.begin() && (pBuffer->_buffer.begin() + pos) <= pBuffer->_buffer.end())
		{
			pBuffer->_buffer.insert((pBuffer->_buffer.begin() + pos), size, item);
		}
	}

	void BufferObject::reserve(uint64_t size)
	{
		pBuffer->_buffer.reserve(size);
	}

	void BufferObject::resize(uint64_t size)
	{
		pBuffer->_buffer.resize(size);
	}

	void BufferObject::push_back(const char& c)
	{
		pBuffer->_buffer.push_back(c);
	}

	void BufferObject::push_back(char&& c)
	{
		pBuffer->_buffer.push_back(c);
	}

}

}
