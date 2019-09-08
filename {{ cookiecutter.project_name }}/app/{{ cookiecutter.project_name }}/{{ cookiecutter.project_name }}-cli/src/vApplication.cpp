
#include "vApplication.hpp"
#include "vPhysicalDevice.hpp"

#include "vLogger.hpp"
#include "vUtilities.hpp"

#include <vulkan/vulkan.h>

#include <glog/logging.h>

#include <iomanip>

namespace
{
PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallback   = nullptr;
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback = nullptr;
}  // namespace

vApplication::vApplication()
{
	VkApplicationInfo infoApplication  = {};
	infoApplication.sType			   = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	infoApplication.pApplicationName   = "vkOfflineRenderer";
	infoApplication.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	infoApplication.pEngineName		   = "vkToolkit";
	infoApplication.engineVersion	  = VK_MAKE_VERSION(0, 0, 0);
	infoApplication.apiVersion		   = VK_API_VERSION_1_0;

	auto layers = make_array(
		"VK_LAYER_LUNARG_standard_validation");
	auto extensions = make_array(
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	VkInstanceCreateInfo infoInstance	= {};
	infoInstance.sType					 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	infoInstance.pApplicationInfo		 = &infoApplication;
	infoInstance.enabledLayerCount		 = (uint32_t)(layers.size());
	infoInstance.ppEnabledLayerNames	 = layers.data();
	infoInstance.enabledExtensionCount   = (uint32_t)(extensions.size());
	infoInstance.ppEnabledExtensionNames = extensions.data();

	VkResult result = vkCreateInstance(&infoInstance, nullptr, &mInstance);
	CHECK_EQ(VK_SUCCESS, result);

	if (true)
	{  // Debug Callback
		// Extension function loading
		vkCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)
			vkGetInstanceProcAddr(mInstance, "vkCreateDebugReportCallbackEXT");
		CHECK_NOTNULL(vkCreateDebugReportCallback);
		vkDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)
			vkGetInstanceProcAddr(mInstance, "vkDestroyDebugReportCallbackEXT");
		CHECK_NOTNULL(vkDestroyDebugReportCallback);

		/* clang-format off */
		VkDebugReportCallbackCreateInfoEXT infoDebugCallback = {};
		infoDebugCallback.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		infoDebugCallback.flags =
			VK_DEBUG_REPORT_INFORMATION_BIT_EXT
			| VK_DEBUG_REPORT_WARNING_BIT_EXT
			| VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
			| VK_DEBUG_REPORT_ERROR_BIT_EXT
			| VK_DEBUG_REPORT_DEBUG_BIT_EXT
		;
		infoDebugCallback.pfnCallback = &logging::vDebugCallback;
		/* clang-format on */

		result = vkCreateDebugReportCallback(mInstance, &infoDebugCallback, nullptr, &mDebugCallback);
		CHECK_EQ(VK_SUCCESS, result);
	}
	{  // Physical devices
		vPhysicalDeviceRange rangePhysicalDevice(mInstance);

		[[maybe_unused]] const vPhysicalDevice& physicalDevice = *std::begin(rangePhysicalDevice);
	}
}

vApplication::~vApplication()
{
	if (mDebugCallback != VK_NULL_HANDLE)
		vkDestroyDebugReportCallback(mInstance, mDebugCallback, nullptr);
	if (mDebugCallback != VK_NULL_HANDLE)
		vkDestroyInstance(mInstance, nullptr);
}

vPhysicalDeviceRange vApplication::getPhysicalDevices() const
{
	return vPhysicalDeviceRange(mInstance);
}
