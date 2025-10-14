//Copyright � 2025 aka Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "pch.h"
#include "CALUMI_Utilities.h"
#include <cstring>
#include <filesystem>
#include "CALUMI_AnimationScene.h"
#include "SFBGS_AnimationScene.h"
#include <expected>
#include <string>
#include <vector>

namespace CALUMI {
	namespace Utilities {

		static void _AlignBuffer(Utilities::VectorContainer<char>& buffer, unsigned long long& currentIndex, int alignmentSize)
		{
			if (alignmentSize < 1) return; //if alignment size is for some reason 0, we skip this output and move on
			if (currentIndex % alignmentSize < 1) return; //we are aligned, move on

			int offset = (alignmentSize - (currentIndex % alignmentSize));

			currentIndex += offset;
		}

		static void _AlignFillBuffer(Utilities::VectorContainer<char>& buffer, unsigned long long& currentIndex, int alignmentSize)
		{
			unsigned long long startingIndex = currentIndex;
			_AlignBuffer(buffer, currentIndex, alignmentSize);
			buffer.insert(buffer.end(), (currentIndex - startingIndex), 0);
		}

		template<typename T>
		Utilities::StringContainer VectorToJSON(const Utilities::VectorContainer<T>& vec, const size_t indents)
		{
			if (vec.empty()) {
				return " []";
			}

			Utilities::StringContainer output = "[\n";
			for (size_t i = 0; i < vec.size(); i++) {
				output += vec.at(i).ToJSON(indents + 1).c_str();
				if (i < vec.size() - 1) {
					output += ",\n";
				}
			}
			output += std::format("\n{}]", Indent(indents).c_str()).c_str();
			return output;
		}


		/// <summary>
		/// This method will assign a value from a buffer with memcpy, however it will consider padding from the byte array buffer first.
		/// When finished it will advance the currentPosition index by the appropriate amount of spaces
		/// </summary>
		/// <param name="currentPosition"></param>
		/// <param name="currentIndex"></param>
		/// <param name="alignmentSize"></param>
		/// <param name="Destination"></param>
		void AlignBufferAndRead(Utilities::VectorContainer<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Destination)
		{
			_AlignBuffer(buffer, currentIndex, alignmentSize);
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
		void AlignFillBufferAndWrite(Utilities::VectorContainer<char>& buffer, unsigned long long& currentIndex, int alignmentSize, int variableSize, void* Source)
		{
			_AlignFillBuffer(buffer, currentIndex, alignmentSize);
			//if source is a short but needs to be a byte, the variableSize will take only the first byte which in Little Endian, is any value under 255
			buffer.insert(buffer.end(), static_cast<size_t>(variableSize), 0);
			std::memcpy(&buffer.at(currentIndex), Source, variableSize);

			currentIndex += variableSize;
		}

		Utilities::StringContainer Indent(const size_t indents) {
			return Utilities::StringContainer(indents * 2, ' ');
		}

#pragma region PathContainer
		struct PathContainer::Impl
		{
			std::filesystem::path path;
			std::string cStringHolder;
		};
		void PathContainer::Clear()
		{
			pImpl->path.clear();
		}
		bool PathContainer::Empty()
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

		StringContainer PathContainer::StringContainer() const
		{
			Utilities::StringContainer output(pImpl->path.string().c_str());
			return output;
		}

		const wchar_t* PathContainer::w_str() const
		{
			return pImpl->path.c_str();
		}

		const char* PathContainer::c_str() const
		{
			pImpl->cStringHolder = pImpl->path.string();
			return pImpl->cStringHolder.c_str();
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
		void StringContainer::Clear()
		{
			pImpl->string.clear();
		}
		bool StringContainer::Empty()
		{
			return pImpl->string.empty();
		}
		char StringContainer::at(size_t idx) const
		{
			return pImpl->string.at(idx);
		}
		size_t StringContainer::Length(bool includeNull) const
		{
			size_t output = includeNull ? 1 : 0;
			output += pImpl->string.length();
			return output;
		}
		int StringContainer::compare(const StringContainer& other) const noexcept
		{
			return pImpl->string.compare(other.pImpl->string);
		}
		int StringContainer::compare(size_t pos, size_t len, const StringContainer& other) const
		{
			return pImpl->string.compare(pos,len,other.pImpl->string);
		}
		int StringContainer::compare(size_t pos, size_t len, const StringContainer& other, size_t subpos, size_t sublen) const
		{
			return pImpl->string.compare(pos,len,other.pImpl->string,subpos,sublen);
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
		StringContainer::StringContainer(size_t count, char c)
		{
			pImpl = new Impl;
			pImpl->string = std::string(count, c);
		}
		StringContainer::StringContainer(const StringContainer& source)
		{
			pImpl = new Impl;
			pImpl->string = source.pImpl->string;
		}
		StringContainer::StringContainer(const StringContainer&& source) noexcept
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

		template<typename T>
		struct VectorContainer<T>::Impl
		{
			std::vector<T> vector;
			Impl() = default;
			Impl(const size_t count)
			{
				std::vector<T> temp(count);
				vector = temp;
			}
		};
		template<typename T>
		VectorContainer<T>::VectorContainer() noexcept
		{
			pImpl = new Impl;
		}
		template<typename T>
		VectorContainer<T>::~VectorContainer()
		{
			if (pImpl)
				delete pImpl;
		}
		template<typename T>
		VectorContainer<T>::VectorContainer(const size_t count)
		{
			pImpl = new Impl(count);
		}
		template<typename T>
		VectorContainer<T>::VectorContainer(const VectorContainer<T>& source) noexcept
		{
			pImpl = new Impl;
			pImpl->vector = source.pImpl->vector;
		}
		template<typename T>
		VectorContainer<T>::VectorContainer(const VectorContainer&& source) noexcept
		{
			pImpl = new Impl;
			pImpl->vector = source.pImpl->vector;
		}
		template<typename T>
		VectorContainer<T>::VectorContainer(const StringContainer& source, bool includeNull) noexcept
		{
			pImpl = new Impl;
			pImpl->vector.resize(source.Length(includeNull));
			std::memcpy(pImpl->vector.data(), source.data(), source.Length());
		}
		template<typename T>
		VectorContainer<T>& VectorContainer<T>::operator=(const VectorContainer<T>& other)
		{
			pImpl->vector = other.pImpl->vector;
			return *this;
		}
		template<typename T>
		void VectorContainer<T>::resize(size_t n)
		{
			pImpl->vector.resize(n);
		}
		template<typename T>
		void VectorContainer<T>::reserve(size_t n)
		{
			pImpl->vector.reserve(n);
		}
		template<typename T>
		void VectorContainer<T>::shrink_to_fit()
		{
			pImpl->vector.shrink_to_fit();
		}
		template<typename T>
		void VectorContainer<T>::fill(T fillValue)
		{
			std::fill(pImpl->vector.begin(), pImpl->vector.end(), fillValue);
		}
		template<typename T>
		const T& VectorContainer<T>::at(size_t i) const
		{
			return pImpl->vector.at(i);
		}
		template<typename T>
		T& VectorContainer<T>::at(size_t i)
		{
			return pImpl->vector.at(i);
		}
		template<typename T>
		void VectorContainer<T>::push_back(const T& input)
		{
			pImpl->vector.push_back(input);
		}
		template<typename T>
		void VectorContainer<T>::push_back(const T&& input)
		{
			pImpl->vector.push_back(input);
		}
		template<typename T>
		void VectorContainer<T>::clear()
		{
			pImpl->vector.clear();
		}
		template<typename T>
		size_t VectorContainer<T>::size() const
		{
			return pImpl->vector.size();
		}
		template<typename T>
		bool VectorContainer<T>::empty() const
		{
			return pImpl->vector.empty();
		}

		template<typename T>
		void VectorContainer<T>::erase(size_t pos)
		{
			if ((pImpl->vector.begin() + pos) >= pImpl->vector.begin() && (pImpl->vector.begin() + pos) < pImpl->vector.end())
			{
				pImpl->vector.erase((pImpl->vector.begin() + pos));
			}
		}

		template<typename T>
		void VectorContainer<T>::insert_r(size_t pos, T& item)
		{
			if ((pImpl->vector.begin()+pos) >= pImpl->vector.begin() && (pImpl->vector.begin() + pos) <= pImpl->vector.end())
			{
				pImpl->vector.insert((pImpl->vector.begin() + pos), item);
			}
		}

		template<typename T>
		void VectorContainer<T>::insert(size_t pos, T item)
		{
			if ((pImpl->vector.begin() + pos) >= pImpl->vector.begin() && (pImpl->vector.begin() + pos) <= pImpl->vector.end())
			{
				pImpl->vector.insert((pImpl->vector.begin() + pos), item);
			}
		}

		template<typename T>
		void VectorContainer<T>::insert(size_t pos, size_t count, T& item)
		{
			if ((pImpl->vector.begin() + pos) >= pImpl->vector.begin() && (pImpl->vector.begin() + pos) <= pImpl->vector.end())
			{
				pImpl->vector.insert((pImpl->vector.begin() + pos), count, item);
			}
		}

		template<typename T>
		void VectorContainer<T>::insert(size_t pos, size_t count, T item)
		{
			if ((pImpl->vector.begin() + pos) >= pImpl->vector.begin() && (pImpl->vector.begin() + pos) <= pImpl->vector.end())
			{
				pImpl->vector.insert((pImpl->vector.begin() + pos), count, item);
			}
		}

		template<typename T>
		size_t VectorContainer<T>::end() const
		{
			return (pImpl->vector.end() - pImpl->vector.begin());
		}

		template<typename T>
		T* VectorContainer<T>::data() noexcept
		{
			return pImpl->vector.data();
		}

		template<typename T>
		const T* VectorContainer<T>::data() const noexcept
		{
			return pImpl->vector.data();
		}

		template<typename T>
		void VectorContainer<T>::sort(bool highToLow)
		{
			if constexpr (HasLessThan<T>)
			{
				if (highToLow)
				{
					std::sort(pImpl->vector.end(), pImpl->vector.begin());
				}
				else
				{
					std::sort(pImpl->vector.begin(), pImpl->vector.end());
				}
			}
		}


#pragma endregion

#pragma region StringMap
		struct StringMap::Entry
		{
			std::string string;
			size_t offset = 0;
			bool hasOffset = false;
			Entry() = default;
			Entry(Entry& source) noexcept
			{
				string = source.string;
				offset = source.offset;
				hasOffset = source.hasOffset;
			}
			Entry(Entry&& source) noexcept
			{
				string = source.string;
				offset = source.offset;
				hasOffset = source.hasOffset;
			}
			Entry(const char* cStr, size_t ofs) noexcept
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
		};

		struct StringMap::Impl
		{
			std::vector<Entry> strings;
			size_t finalOffset = 0;
		};
		
		StringMap::StringMap()
		{
			pImpl = new Impl;
		}
		StringMap::~StringMap()
		{
			if (pImpl)
				delete pImpl;
		}
		void StringMap::push_back(const char* string, size_t offset)
		{
			pImpl->strings.push_back(Entry(string, offset));
		}
		void StringMap::push_back(const char* string)
		{
			pImpl->strings.push_back(Entry(string));
		}
		size_t StringMap::GetOffset(size_t idx)
		{
			return pImpl->strings.at(idx).offset;
		}
		size_t StringMap::GetFinalOffset()
		{
			return pImpl->finalOffset;
		}
		bool StringMap::HasOffset(size_t idx)
		{
			return pImpl->strings.at(idx).hasOffset;
		}
		void StringMap::SetFinalOffset(size_t offset)
		{
			pImpl->finalOffset = offset;
		}
		void StringMap::Reserve(size_t size)
		{
			pImpl->strings.reserve(size);
		}
		const char* StringMap::GetString(size_t idx)
		{
			return pImpl->strings.at(idx).string.c_str();
		}
		size_t StringMap::StringLength(size_t idx, bool includeNull)
		{
			size_t output = includeNull ? 1 : 0;
			output += pImpl->strings.at(idx).string.length();
			return output;
		}
		size_t StringMap::Size()
		{
			return pImpl->strings.size();
		}
#pragma endregion

#pragma region ExpectedContainer

		template<typename T, typename U>
		struct ExpectedConatiner<T, U>::Impl
		{
			std::expected<T, U> expected;
			Impl(T& tValue)
			{
				expected = tValue;
			}
		};


		template<typename T, typename U>
		ExpectedConatiner<T, U>::ExpectedConatiner(T& expectedValue)
		{
			pImpl = new Impl(expectedValue);
		}

		template<typename T, typename U>
		ExpectedConatiner<T, U>::ExpectedConatiner(T&& expectedValue) noexcept
		{
			pImpl = new Impl(expectedValue);
		}

		template<typename T, typename U>
		bool ExpectedConatiner<T, U>::has_value() const noexcept
		{
			return pImpl->expected.has_value();
		}
		template<typename T, typename U>
		U& ExpectedConatiner<T, U>::error() const
		{
			return pImpl->expected.error();
		}
		template<typename T, typename U>
		T& ExpectedConatiner<T, U>::value() const
		{
			return pImpl->expected.value();
		}

		template<typename T, typename U>
		ExpectedConatiner<T, U>::~ExpectedConatiner()
		{
			if (pImpl)
				delete pImpl;
		}
		/*template<typename T, typename U>
		void ExpectedConatiner<T, U>::SetErrorValue_R(const U& uValue)
		{
			pImpl->expected = std::unexpected(uValue);
		}*/
		template<typename T, typename U>
		void ExpectedConatiner<T, U>::SetErrorValue(U uValue)
		{
			pImpl->expected = std::unexpected(uValue);
		}
#pragma endregion

		
}

}
