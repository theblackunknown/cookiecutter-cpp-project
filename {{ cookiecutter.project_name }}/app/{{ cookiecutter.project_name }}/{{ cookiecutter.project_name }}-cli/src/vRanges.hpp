#pragma once

template<
	typename T,
	template<typename> Iterator>
struct vRange
{
	Iterator<const T> begin() const;
	Iterator<const T> end() const;
	Iterator<T> begin();
	Iterator<T> end();
};
