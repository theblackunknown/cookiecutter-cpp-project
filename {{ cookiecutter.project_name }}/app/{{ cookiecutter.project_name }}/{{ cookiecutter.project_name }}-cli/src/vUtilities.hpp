#pragma once

#include <array>
#include <cstddef>
#include <type_traits>

template<typename T>
constexpr auto make_array() -> std::array<T, 0>
{
	return {};
}

template<typename... Args>
constexpr auto make_array(Args&&... args) -> std::array<typename std::common_type_t<Args...>, sizeof...(Args)>
{
	return {std::forward<Args>(args)...};
}

template<typename T, std::size_t N>
constexpr auto get_buffer_size(T (&buffer [[maybe_unused]])[N]) -> decltype(N)
{
	return N;
}

template<typename T, std::size_t N>
constexpr auto get_buffer_size(const T (&buffer [[maybe_unused]])[N]) -> decltype(N)
{
	return N;
}
