
#include "vLogger.hpp"

#include <vulkan/vulkan.h>

#include <iomanip>
#include <iostream>
#include <vector>

#include <ctime>

namespace
{
class FormattedLogSink : public ::google::LogSink
{
	void send(::google::LogSeverity severity, const char* /*full_filename*/,
		const char* base_filename, int line,
		const struct ::tm* tm_time,
		const char* message, size_t message_len) override
	{
		/* clang-format off */
		std::cout
			<< '['
                << std::setfill('0') << std::setw(2) << tm_time->tm_hour << ':'
                << std::setfill('0') << std::setw(2) << tm_time->tm_min  << ':'
                << std::setfill('0') << std::setw(2) << tm_time->tm_sec
            << ']'
			<< '['
                << std::setfill(' ') << std::setw(7) << std::right << google::GetLogSeverityName(severity)
            << ']'
			<< '['
                << std::setfill(' ') << std::setw(18) << std::right << base_filename << ':'
                << std::setfill('0') << std::setw(2) << std::right << line
            << ']'
            << " - "
			<< std::string(message, message_len)
            << std::endl;
		/* clang-format on */
	}
};
FormattedLogSink* sMainSink = nullptr;
}  // namespace

namespace logging
{
VkBool32 vDebugCallback(
	VkDebugReportFlagsEXT flags,
	[[maybe_unused]] VkDebugReportObjectTypeEXT objectType,
	[[maybe_unused]] uint64_t object,
	[[maybe_unused]] size_t location,
	[[maybe_unused]] int32_t messageCode,
	[[maybe_unused]] const char* pLayerPrefix,
	const char* pMessage,
	[[maybe_unused]] void* pUserData)
{
	switch (flags)
	{
		case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
		{
			LOG(INFO)
				<< '[' << std::setfill(' ') << std::setw(15) << std::right << pLayerPrefix << ']'
				<< pMessage;
		}
		break;
		case VK_DEBUG_REPORT_WARNING_BIT_EXT:
		{
			LOG(WARNING)
				<< '[' << std::setfill(' ') << std::setw(15) << std::right << pLayerPrefix << ']'
				<< pMessage;
		}
		break;
		case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
		{
			LOG(WARNING)
				<< '[' << std::setfill(' ') << std::setw(15) << std::right << pLayerPrefix << ']'
				<< pMessage;
		}
		break;
		case VK_DEBUG_REPORT_ERROR_BIT_EXT:
		{
			LOG(ERROR)
				<< '[' << std::setfill(' ') << std::setw(15) << std::right << pLayerPrefix << ']'
				<< pMessage;
		}
		break;
		case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
		{
			DLOG(INFO)
				<< '[' << std::setfill(' ') << std::setw(15) << std::right << pLayerPrefix << ']'
				<< pMessage;
		}
		break;
	}

	return VK_FALSE;
}

void vLoggingStart()
{
	FLAGS_minloglevel				= 0;
	FLAGS_alsologtostderr			= false;
	FLAGS_colorlogtostderr			= true;
	FLAGS_stop_logging_if_full_disk = true;
	google::InitGoogleLogging("vkOfflineRenderer");
	LOG(INFO) << "Google Logging service started";

	google::AddLogSink((sMainSink = new FormattedLogSink));

	if (VLOG_IS_ON(1))
	{
		uint32_t count = 0u;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

		std::vector<VkExtensionProperties> extensions(count);
		vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());

		VLOG(1) << "Available Vulkan extensions:";
		for (auto&& extension: extensions)
			VLOG(1) << '\t' << extension.extensionName;
	}

	if (VLOG_IS_ON(1))
	{
		uint32_t count = 0u;
		vkEnumerateInstanceLayerProperties(&count, nullptr);

		std::vector<VkLayerProperties> layers(count);
		vkEnumerateInstanceLayerProperties(&count, layers.data());

		VLOG(1) << "Available Vulkan layers:";
		for (auto&& layer: layers)
			VLOG(1) << '\t' << layer.layerName;
	}
}

void vLoggingStop()
{
	google::RemoveLogSink(sMainSink);
	delete sMainSink;
	LOG(INFO) << "Google Logging service stopped";
	google::ShutdownGoogleLogging();
}
}  // namespace logging
