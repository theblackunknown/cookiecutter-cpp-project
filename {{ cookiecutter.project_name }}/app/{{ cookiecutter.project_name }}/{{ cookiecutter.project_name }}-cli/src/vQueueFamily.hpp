#pragma once

#include <vulkan/vulkan.h>

#include <iosfwd>
#include <iterator>
#include <vector>

struct vQueueFamily
{
	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	uint32_t mIndex;
	uint32_t _padding;
	VkQueueFamilyProperties mProperties;

	vQueueFamily(VkPhysicalDevice instance, uint32_t index, VkQueueFamilyProperties properties);
	~vQueueFamily();
};

struct vQueueFamilyRange
{
	using QueueFamilies = std::vector<vQueueFamily>;

	VkPhysicalDevice mPhysicalDevice;
	QueueFamilies mQueueFamilies;

	vQueueFamilyRange(VkPhysicalDevice device);

	QueueFamilies::const_iterator begin() const
	{
		return std::begin(mQueueFamilies);
	}

	QueueFamilies::iterator begin()
	{
		return std::begin(mQueueFamilies);
	}

	QueueFamilies::const_iterator end() const
	{
		return std::end(mQueueFamilies);
	}

	QueueFamilies::iterator end()
	{
		return std::end(mQueueFamilies);
	}
};

std::ostream& operator<<(std::ostream& stream, const vQueueFamily& device);
