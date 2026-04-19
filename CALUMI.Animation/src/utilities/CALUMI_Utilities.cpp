//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com


#include "internalplatform.h"
#include "utilities/CALUMI_Utilities.h"
#include <cstring>
#include <expected>
#include <string>
#include <vector>
#include <filesystem>
#include <cctype>
#include <cstdint>
#include "internal/internalvectordef.h"

namespace CALUMI::Utilities
{

	void AlignBuffer(unsigned long long& currentIndex, const int alignmentSize)
	{
		if (alignmentSize < 1) return; //if alignment size is for some reason 0, we skip this output and move on
		if (currentIndex % alignmentSize < 1) return; //we are aligned, move on

		const auto offset = alignmentSize - currentIndex % alignmentSize;

		currentIndex += offset;
	}

	static void s_AlignFillBuffer(BufferObject& buffer, unsigned long long& currentIndex, const int alignmentSize)
	{
		const unsigned long long startingIndex = currentIndex;
		AlignBuffer(currentIndex, alignmentSize);
		buffer.insert(buffer.endPos(), currentIndex - startingIndex, 0);
	}

	void AlignBufferAndRead(BufferObject& buffer, unsigned long long& currentIndex, const int alignmentSize, const int variableSize, void* Destination)
	{
		AlignBuffer(currentIndex, alignmentSize);
		std::memcpy(Destination, &buffer.at(currentIndex), variableSize);

		currentIndex += variableSize;
	}

	void AlignFillBufferAndWrite(BufferObject& buffer, unsigned long long& currentIndex, const int alignmentSize, const int variableSize, const void* Source)
	{
		s_AlignFillBuffer(buffer, currentIndex, alignmentSize);
		//if source is a short but needs to be a byte, the variableSize will take only the first byte which in Little Endian, is any value under 255
		buffer.insert(buffer.endPos(), static_cast<size_t>(variableSize), 0);
		std::memcpy(&buffer.at(currentIndex), Source, variableSize);

		currentIndex += variableSize;
	}

	StringContainer Indent(const uint64_t indents) {
		return {indents * 2, ' '};
	}

	bool IsNumeric(const StringContainer& str)
	{
		for (uint64_t i = 0; i < str.length(); i++)
		{
			if (const char _char = str.at(i); _char != '-' && _char != '.' && !std::isdigit(_char))
				return false;
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
	void PathContainer::clear() const
	{
		pImpl->path.clear();
	}
	bool PathContainer::empty() const
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
		return {pImpl->path.filename().c_str()};
	}
	PathContainer PathContainer::stem() const
	{
		return {pImpl->path.stem().c_str()};
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
		StringContainer output(pImpl->path.string().c_str());
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

	PathContainer::PathContainer(const char* cString) : PathContainer()
	{
		pImpl->path = cString;
	}

	PathContainer::PathContainer(const wchar_t* wString) : PathContainer()
	{
		pImpl->path = wString;
	}

	PathContainer::PathContainer(const PathContainer& source) : PathContainer()
	{
		*this = source;
	}

	PathContainer::PathContainer(const PathContainer&& source) noexcept : PathContainer()
	{
		*this = source;
	}

	PathContainer::~PathContainer()
	{
		if (pImpl)
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	bool PathContainer::operator==(const PathContainer& other) const
	{
		return pImpl->path == other.pImpl->path;
	}
	PathContainer& PathContainer::operator=(const PathContainer& other)
	{
		if (this != &other)
			pImpl->path = other.pImpl->path;

		return *this;
	}
	PathContainer& PathContainer::operator=(const StringContainer& other)
	{
		pImpl->path = other.c_str();
		return *this;
	}
	PathContainer& PathContainer::operator/=(const PathContainer& other)
	{
		pImpl->path /= other.pImpl->path;
		return *this;
	}
	PathContainer& PathContainer::operator/=(const StringContainer& other)
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
	StringContainer StringContainer::operator+(const char* other) const
	{
		StringContainer output(pImpl->string.c_str());
		output += other;
		return output;
	}
	StringContainer StringContainer::operator+(const StringContainer& other) const
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
	void StringContainer::clear() const
	{
		pImpl->string.clear();
	}
	bool StringContainer::empty() const
	{
		return pImpl->string.empty();
	}
	char StringContainer::at(const uint64_t idx) const
	{
		return pImpl->string.at(idx);
	}
	uint64_t StringContainer::length(const bool includeNull) const
	{
		uint64_t output = includeNull ? 1 : 0;
		output += pImpl->string.length();
		return output;
	}
	uint64_t StringContainer::find(const char* s, const uint64_t pos) const
	{
		return pImpl->string.find(s,pos);
	}
	int StringContainer::compare(const StringContainer& other, const bool caseSensitive) const noexcept
	{
		if (!caseSensitive)
		{
			return SCOMPARE(c_str(), other.c_str());
		}

		return pImpl->string.compare(other.pImpl->string);
	}
	int StringContainer::compare(const uint64_t pos, const uint64_t len, const StringContainer& other) const
	{
		return pImpl->string.compare(pos,len,other.pImpl->string);
	}
	int StringContainer::compare(const uint64_t pos, const uint64_t len, const StringContainer& other, const uint64_t subPos, const uint64_t subLngth) const
	{
		return pImpl->string.compare(pos,len,other.pImpl->string,subPos,subLngth);
	}
	void StringContainer::assign(const char* str) const
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
	StringContainer::StringContainer(const uint64_t count, const char c)
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
		{
			delete pImpl;
			pImpl = nullptr;
		}
	}
	const char* StringContainer::c_str() const
	{
		return pImpl->string.c_str();
	}
	const char* StringContainer::data() const
	{
		return pImpl->string.data();
	}
	StringContainer& StringContainer::operator+=(const char* other)
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
		if (this != &other)
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
		Entry(const char* cStr, const uint64_t ofs) noexcept
		{
			string = cStr;
			offset = ofs;
			hasOffset = true;
		}

		explicit Entry(const char* cStr) noexcept
		{
			string = cStr;
			offset = 0;
			hasOffset = false;
		}
		Entry& operator=(const Entry& other)
		{
			if (this != &other)
			{
				string = other.string;
				offset = other.offset;
				hasOffset = other.hasOffset;
			}
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
		if (this != &other)
		{
			pImpl->strings.reserve(other.pImpl->strings.size());

			for (const auto& entry : other.pImpl->strings)
			{
				pImpl->strings.push_back(entry);
			}

			pImpl->finalOffset = other.pImpl->finalOffset;
		}
		return *this;
	}
	void StringList::push_back(const char* string, const uint64_t offset) const
	{
		pImpl->strings.emplace_back(string, offset);
	}
	void StringList::push_back(const char* string) const
	{
		pImpl->strings.emplace_back(string);
	}
	uint64_t StringList::getOffset(const unsigned int idx) const
	{
		return pImpl->strings.at(idx).offset;
	}
	uint64_t StringList::getFinalOffset() const
	{
		return pImpl->finalOffset;
	}
	bool StringList::hasOffset(const unsigned int idx) const
	{
		return pImpl->strings.at(idx).hasOffset;
	}
	void StringList::setFinalOffset(const uint64_t offset) const
	{
		pImpl->finalOffset = offset;
	}
	void StringList::reserve(const unsigned int size) const
	{
		pImpl->strings.reserve(size);
	}
	const char* StringList::c_str(const unsigned int idx) const
	{
		return pImpl->strings.at(idx).string.c_str();
	}
	uint64_t StringList::stringLength(const unsigned int idx, const bool includeNull) const
	{
		uint64_t output = includeNull ? 1 : 0;
		output += pImpl->strings.at(idx).string.length();
		return output;
	}
	unsigned int StringList::size() const
	{
		return pImpl->strings.size();
	}

	int64_t StringList::find(const char* string, int64_t defValue) const
	{
		for (int i = 0; i < pImpl->strings.size(); i++)
		{
			if (SCOMPARE(string, pImpl->strings.at(i).string.c_str())==0)
			{
				return i;
			}
		}

		return defValue;
	}

	bool StringList::empty() const
	{
		return pImpl->strings.empty();
	}
#pragma endregion

#pragma region EXTERN"C"

	StringContainer* CreateStringContainerC()
	{
		return new StringContainer();
	}

	const char* GetStringFromContainerC(const StringContainer* source)
	{
		return source->c_str();
	}

	uint64_t GetStringContainerSizeC(const StringContainer* source)
	{
		return source->length();
	}

	void DeleteStringContainerC(const StringContainer* ptr)
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

	const char& BufferObject::at(const uint64_t idx) const
	{
		return pBuffer->_buffer.at(idx);
	}

	char& BufferObject::at(const uint64_t idx)
	{
		return pBuffer->_buffer.at(idx);
	}

	void BufferObject::insert(const uint64_t pos, const uint64_t size, const char item)
	{
		if (pBuffer->_buffer.begin() + static_cast<long long>(pos) >= pBuffer->_buffer.begin() && pBuffer->_buffer.begin() + static_cast<long long>(pos) <= pBuffer->_buffer.end())
		{
			pBuffer->_buffer.insert(pBuffer->_buffer.begin() + static_cast<long long>(pos), size, item);
		}
	}

	void BufferObject::reserve(const uint64_t size)
	{
		pBuffer->_buffer.reserve(size);
	}

	void BufferObject::resize(const uint64_t size)
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
