#pragma once

#include <glog/logging.h>

#include <vulkan/vulkan.h>

namespace logging
{
void vLoggingStart();
void vLoggingStop();

VKAPI_ATTR VkBool32 VKAPI_CALL vDebugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objectType,
	uint64_t object,
	size_t location,
	int32_t messageCode,
	const char* pLayerPrefix,
	const char* pMessage,
	void* pUserData);

struct ScopedLoggingService
{
	ScopedLoggingService()
	{
		vLoggingStart();
	}

	~ScopedLoggingService()
	{
		vLoggingStop();
	}
};
}  // namespace logging
