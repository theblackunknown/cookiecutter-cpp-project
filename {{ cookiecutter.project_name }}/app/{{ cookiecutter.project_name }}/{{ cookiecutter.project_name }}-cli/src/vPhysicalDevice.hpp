#pragma once

#include <vulkan/vulkan.h>

#include <iosfwd>
#include <iterator>
#include <vector>

struct vQueueFamilyRange;

struct vPhysicalDevice
{
	VkInstance mInstance			 = VK_NULL_HANDLE;
	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties mProperties;
	VkPhysicalDeviceMemoryProperties mMemoryProperties;

	vPhysicalDevice(VkInstance instance, VkPhysicalDevice device);
	~vPhysicalDevice();

	vQueueFamilyRange getQueueFamilies() const;
};

struct vPhysicalDeviceRange
{
	using PhysicalDevices = std::vector<vPhysicalDevice>;

	VkInstance mInstance;
	PhysicalDevices mPhysicalDevices;

	vPhysicalDeviceRange(VkInstance instance);

	PhysicalDevices::const_iterator begin() const
	{
		return std::begin(mPhysicalDevices);
	}

	PhysicalDevices::iterator begin()
	{
		return std::begin(mPhysicalDevices);
	}

	PhysicalDevices::const_iterator end() const
	{
		return std::end(mPhysicalDevices);
	}

	PhysicalDevices::iterator end()
	{
		return std::end(mPhysicalDevices);
	}
};

std::ostream& operator<<(std::ostream& stream, const vPhysicalDevice& device);
