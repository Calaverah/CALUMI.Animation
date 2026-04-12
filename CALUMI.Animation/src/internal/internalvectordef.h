#pragma once

#include <algorithm>

#define VECTORDEF(CLASS, T) \
		struct CLASS::Impl \
		{ \
			std::vector<T> vector; \
			constexpr Impl() noexcept = default; \
			constexpr Impl(std::vector<T>&& source) noexcept { vector = source; } \
			constexpr Impl(const std::vector<T>& source) noexcept { vector = source; } \
		}; \
        CLASS::CLASS() noexcept : pImpl(new Impl()) {} \
        CLASS::~CLASS() \
		{ \
			if (pImpl) \
				{ \
				delete pImpl; \
				pImpl = nullptr; \
				} \
		} \
        CLASS::CLASS(const CLASS& source) : pImpl(new Impl(source.pImpl->vector)) {} \
		\
        CLASS::CLASS(CLASS&& source) noexcept : pImpl (new Impl(source.pImpl->vector)) {} \
		\
        CLASS& CLASS::operator=(const CLASS& other) { if(this != &other) { pImpl->vector = other.pImpl->vector; } return *this; } \
        CLASS& CLASS::operator=(CLASS&& other) noexcept { pImpl->vector = other.pImpl->vector; return *this; } \
		void CLASS::resize(uint64_t n) { pImpl->vector.resize(n); } \
		void CLASS::reserve(uint64_t n) { pImpl->vector.reserve(n); } \
		void CLASS::shrink_to_fit() { pImpl->vector.shrink_to_fit(); } \
		const T& CLASS::at(uint64_t i) const { return pImpl->vector.at(i); } \
		T& CLASS::at(uint64_t i) { return pImpl->vector.at(i); } \
		void CLASS::push_back(const T& input) { pImpl->vector.push_back(input); } \
		void CLASS::push_back(const T&& input) { pImpl->vector.push_back(input); } \
		void CLASS::clear() { pImpl->vector.clear(); } \
		uint64_t CLASS::size() const { return pImpl->vector.size(); } \
		bool CLASS::empty() const { return pImpl->vector.empty(); } \
		void CLASS::erase(uint64_t pos) \
		{ \
			if ((pImpl->vector.begin() + pos) >= pImpl->vector.begin() && (pImpl->vector.begin() + pos) < pImpl->vector.end()) \
			{ \
				pImpl->vector.erase((pImpl->vector.begin() + pos)); \
			} \
		} \
		void CLASS::insert_r(uint64_t pos, const T& item) \
		{ \
			if ((pImpl->vector.begin() + pos) >= pImpl->vector.begin() && (pImpl->vector.begin() + pos) <= pImpl->vector.end()) \
			{ \
				pImpl->vector.insert((pImpl->vector.begin() + pos), item); \
			} \
		} \
		void CLASS::insert(uint64_t pos, T&& item) \
		{ \
			if ((pImpl->vector.begin() + pos) >= pImpl->vector.begin() && (pImpl->vector.begin() + pos) <= pImpl->vector.end()) \
			{ \
				pImpl->vector.insert((pImpl->vector.begin() + pos), item); \
			} \
		} \
		void CLASS::insert(uint64_t pos, uint64_t count, const T& item) \
		{ \
			if ((pImpl->vector.begin() + pos) >= pImpl->vector.begin() && (pImpl->vector.begin() + pos) <= pImpl->vector.end()) \
			{ \
				pImpl->vector.insert((pImpl->vector.begin() + pos), count, item); \
			} \
		} \
		void CLASS::insert(uint64_t pos, uint64_t count, T&& item) \
		{ \
			if ((pImpl->vector.begin() + pos) >= pImpl->vector.begin() && (pImpl->vector.begin() + pos) <= pImpl->vector.end()) \
			{ \
				pImpl->vector.insert((pImpl->vector.begin() + pos), count, item); \
			} \
		} \
		uint64_t CLASS::end() const \
		{ \
			return (pImpl->vector.end() - pImpl->vector.begin()); \
		} \
		T* CLASS::data() noexcept \
		{ \
			return pImpl->vector.data(); \
		} \
		const T* CLASS::data() const noexcept \
		{ \
			return pImpl->vector.data(); \
		} \
		CLASS CLASS::range(uint64_t first, uint64_t last) const \
		{ \
			if (first > last) return CLASS(); \
			\
			CLASS output; \
			output.reserve(last + 1 - first); \
			\
			for (uint64_t i = first; i < last + 1 && i < size(); i++) \
			{ \
				output.push_back(at(i)); \
			} \
			\
			output.shrink_to_fit(); \
			\
			return output;	\
		}


