#pragma once

#include <vulkan/vulkan.h>

#include <vector>

struct vPhysicalDeviceRange;

class vApplication
{
private:
	VkInstance mInstance					= VK_NULL_HANDLE;
	VkDebugReportCallbackEXT mDebugCallback = VK_NULL_HANDLE;

public:
	vApplication();
	~vApplication();

	vPhysicalDeviceRange getPhysicalDevices() const;
};
