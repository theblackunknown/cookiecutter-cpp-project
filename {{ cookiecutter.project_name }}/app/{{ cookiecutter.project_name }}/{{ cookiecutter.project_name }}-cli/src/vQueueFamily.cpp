
#include "vQueueFamily.hpp"

#include <vulkan/vulkan.h>

#include <glog/logging.h>

vQueueFamily::vQueueFamily(VkPhysicalDevice device, uint32_t index, VkQueueFamilyProperties properties)
	: mPhysicalDevice(device)
	, mIndex(index)
	, mProperties(properties)
{
}

vQueueFamily::~vQueueFamily() = default;

vQueueFamilyRange::vQueueFamilyRange(VkPhysicalDevice device)
	: mPhysicalDevice(device)
{
	uint32_t count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &count, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(count);
	vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &count, queueFamilies.data());

	mQueueFamilies.reserve(count);
	for (uint32_t idx(0); idx < count; ++idx)
		mQueueFamilies.emplace_back(mPhysicalDevice, idx, queueFamilies.at(0));
}

std::ostream& operator<<(std::ostream& stream, const vQueueFamily& queueFamily)
{
	const VkQueueFamilyProperties& p = queueFamily.mProperties;
	/* clang-format off */
    stream
        << "QueueFamily " << p.queueCount << " queues with ";

    if (p.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        stream << "Graphics ";
    if (p.queueFlags & VK_QUEUE_COMPUTE_BIT)
        stream << "Compute ";
    if (p.queueFlags & VK_QUEUE_TRANSFER_BIT)
        stream << "Transfer ";
    if (p.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
        stream << "Sparse Binding ";

    stream << "capabilities";
	/* clang-format on */
	return stream;
}
