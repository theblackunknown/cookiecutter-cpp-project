#pragma once

#if 0
struct vPhysicalDeviceIterator
{
    using difference_type   = std::ptrdiff_t;
    using value_type        = vPhysicalDevice;
    using pointer           = const vPhysicalDevice*;
    using reference         = const vPhysicalDevice&;
    using iterator_category = std::random_access_iterator_tag;

    const vPhysicalDeviceRange* mRange = nullptr;
    uint64_t mIndex                    = 0u;

    vPhysicalDeviceIterator(const vPhysicalDeviceRange* range, uint64_t index)
        : mRange(range)
        , mIndex(index)
    {
    }

    vPhysicalDeviceIterator(vPhysicalDeviceIterator&&)      = default;
    vPhysicalDeviceIterator(const vPhysicalDeviceIterator&) = default;
    vPhysicalDeviceIterator& operator=(vPhysicalDeviceIterator&&) = default;
    vPhysicalDeviceIterator& operator=(const vPhysicalDeviceIterator&) = default;

    vPhysicalDeviceIterator& operator++()
    {
        ++mIndex;
        return *this;
    }

    vPhysicalDeviceIterator operator++(int)
    {
        vPhysicalDeviceIterator retval = *this;
        ++(*this);
        return retval;
    }

    vPhysicalDeviceIterator& operator+=(int n)
    {
        mIndex += n;
        return *this;
    }

    vPhysicalDeviceIterator& operator--()
    {
        --mIndex;
        return *this;
    }

    vPhysicalDeviceIterator operator--(int)
    {
        vPhysicalDeviceIterator retval = *this;
        --(*this);
        return retval;
    }

    vPhysicalDeviceIterator& operator-=(int n)
    {
        mIndex -= n;
        return *this;
    }

    bool operator==(vPhysicalDeviceIterator rhs) const
    {
        return (mRange == rhs.mRange) && (mIndex == rhs.mIndex);
    }

    bool operator!=(vPhysicalDeviceIterator other) const
    {
        return !(*this == other);
    }

    reference operator*() const
    {
        assert(mIndex < mRange->mPhysicalDevices.size());
        return mRange->mPhysicalDevices.at(mIndex);
    }

    pointer operator->() const
    {
        assert(mIndex < mRange->mPhysicalDevices.size());
        return std::addressof(mRange->mPhysicalDevices.at(mIndex));
    }
};

inline vPhysicalDeviceIterator operator+(const vPhysicalDeviceIterator& lhs, const vPhysicalDeviceIterator& rhs)
{
    return vPhysicalDeviceIterator(lhs.mRange, lhs.mIndex + rhs.mIndex);
}

inline vPhysicalDeviceIterator operator-(const vPhysicalDeviceIterator& lhs, const vPhysicalDeviceIterator& rhs)
{
    return vPhysicalDeviceIterator(lhs.mRange, lhs.mIndex - rhs.mIndex);
}

#endif
