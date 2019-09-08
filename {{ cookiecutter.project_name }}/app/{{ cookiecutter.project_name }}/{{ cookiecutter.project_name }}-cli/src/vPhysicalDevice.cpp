
#include "vPhysicalDevice.hpp"
#include "vQueueFamily.hpp"

#include <vulkan/vulkan.h>

#include <glog/logging.h>

#include <iomanip>

vPhysicalDevice::vPhysicalDevice(VkInstance instance, VkPhysicalDevice device)
	: mInstance(instance)
	, mPhysicalDevice(device)
{
	vkGetPhysicalDeviceProperties(mPhysicalDevice, &mProperties);
	vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &mMemoryProperties);
}

vPhysicalDevice::~vPhysicalDevice() = default;

vQueueFamilyRange vPhysicalDevice::getQueueFamilies() const
{
	return vQueueFamilyRange(mPhysicalDevice);
}

vPhysicalDeviceRange::vPhysicalDeviceRange(VkInstance instance)
	: mInstance(instance)
{
	uint32_t count  = 0u;
	VkResult result = vkEnumeratePhysicalDevices(mInstance, &count, nullptr);
	CHECK_EQ(VK_SUCCESS, result);

	std::vector<VkPhysicalDevice> devices(count);
	result = vkEnumeratePhysicalDevices(mInstance, &count, devices.data());
	CHECK_EQ(VK_SUCCESS, result);

	mPhysicalDevices.reserve(count);
	for (auto&& device: devices)
		mPhysicalDevices.emplace_back(mInstance, device);
}

std::ostream& operator<<(std::ostream& stream, const vPhysicalDevice& device)
{
	const VkPhysicalDeviceProperties& p = device.mProperties;
	/* clang-format off */
	stream
		<< "PhysicalDevice " << p.deviceName
		<< std::showbase << std::hex
		<< " (ID: " << p.deviceID << ") "
		<< " manufactured by " << p.vendorID
		<< std::resetiosflags(std::ios_base::hex)
		<< std::resetiosflags(std::ios_base::showbase)
		<< ", API "    << VK_VERSION_MAJOR(p.apiVersion   ) << '.' << VK_VERSION_MINOR(p.apiVersion   ) << '.' << VK_VERSION_PATCH(p.apiVersion   )
		// << ", API "    << std::showbase << std::hex << p.apiVersion
		<< ", Driver " << VK_VERSION_MAJOR(p.driverVersion) << '.' << VK_VERSION_MINOR(p.driverVersion) << '.' << VK_VERSION_PATCH(p.driverVersion)
		// << ", Driver " << std::showbase << std::hex << p.driverVersion
	;
	/* clang-format on */
	return stream;
}
