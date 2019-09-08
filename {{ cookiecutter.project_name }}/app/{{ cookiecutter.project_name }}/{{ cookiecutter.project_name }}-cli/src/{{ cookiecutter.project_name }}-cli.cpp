#include "vkToolset.hpp"
#include "shaders/vShaders.hpp"

#include <vUtilities.hpp>

#include <vulkan/vulkan.h>
// #include <ext_loader/vulkan_ext.h>

// #include <range/v3/all.hpp>

#include <glog/logging.h>

#include <array>
#include <memory>
#include <cstdint>

namespace
{
    struct GLOGService
    {
        GLOGService( )
        {
            google::InitGoogleLogging( "vkToolset" );
            LOG( INFO ) << "Google Logging service started";
        }

        ~GLOGService( )
        {
            LOG( INFO ) << "Googleeeeee Logging service stopped";
            google::ShutdownGoogleLogging( );
        }
    };
}

struct vkToolsetApplication::Implementation
{
    static
    VkBool32 VKAPI_PTR onDebugReportCallback(
        VkDebugReportFlagsEXT      flags,
        VkDebugReportObjectTypeEXT objectType UNUSED,
        uint64_t                   object UNUSED,
        size_t                     location UNUSED,
        int32_t                    messageCode,
        const char*                pLayerPrefix,
        const char*                pMessage,
        void*                      pUserData UNUSED)
    {
        switch( flags )
        {
            case static_cast<VkDebugReportFlagsEXT>( vk::DebugReportFlagBitsEXT::eInformation ):
                LOG( INFO ) << " [" << pLayerPrefix << "] Code " << messageCode << " : " << pMessage;
                break;
            case static_cast<VkDebugReportFlagsEXT>( vk::DebugReportFlagBitsEXT::eWarning ):
                LOG( WARNING ) << " [" << pLayerPrefix << "] Code " << messageCode << " : " << pMessage;
                break;
            case static_cast<VkDebugReportFlagsEXT>( vk::DebugReportFlagBitsEXT::ePerformanceWarning ):
                LOG( WARNING ) << " [" << pLayerPrefix << "] Code " << messageCode << " : " << pMessage;
                break;
            case static_cast<VkDebugReportFlagsEXT>( vk::DebugReportFlagBitsEXT::eError ):
                LOG( ERROR ) << " [" << pLayerPrefix << "] Code " << messageCode << " : " << pMessage;
                break;
            case static_cast<VkDebugReportFlagsEXT>( vk::DebugReportFlagBitsEXT::eDebug ):
                LOG( INFO ) << " [" << pLayerPrefix << "] Code " << messageCode << " : " << pMessage;
                break;
        }
        return VK_FALSE;
    }

    /// Services
    std::unique_ptr<GLOGService> mLoggingService = std::make_unique<GLOGService>( );

    /// Instance
    vk::UniqueInstance mInstance;

    // VkDebugReportCallback
    vk::UniqueDebugReportCallbackEXT mDebugReportCallback;

    /// Devices
    vk::PhysicalDevice mPhysicalDevice;
    vk::UniqueDevice mDevice;

    /// Queues
    vk::Queue mQueue;

    /// Shaders
    // vk::Queue mQueue;

    Implementation( )
    {
        initializeInstance( );
        initializeDevice( );

        initializeShader( );
        initializePipeline( );
    }

    void initializeInstance( )
    {
        vk::ApplicationInfo iApplication;
        iApplication.pApplicationName = "vkToolset";
        iApplication.pEngineName = "Pantherinae";
        iApplication.apiVersion = VK_MAKE_VERSION(1, 0, 0);

        auto extensions = make_array(
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME
        );

        auto layers = make_array(
            "VK_LAYER_LUNARG_standard_validation"
        );

        vk::InstanceCreateInfo iInstance;
        iInstance.pApplicationInfo = &iApplication;
        iInstance.enabledExtensionCount = extensions.size( );
        iInstance.ppEnabledExtensionNames = extensions.data( );
        iInstance.enabledLayerCount = layers.size( );
        iInstance.ppEnabledLayerNames = layers.data( );

        mInstance = vk::createInstanceUnique( iInstance );
        CHECK( mInstance );

        vkExtInitInstance( *mInstance );

        vk::DebugReportCallbackCreateInfoEXT iDebugReportCallback;

        #if defined( NDEBUG )
            iDebugReportCallback.flags =
                vk::DebugReportFlagBitsEXT::eWarning
                | vk::DebugReportFlagBitsEXT::ePerformanceWarning
                | vk::DebugReportFlagBitsEXT::eError;
        #else
            iDebugReportCallback.flags =
                vk::DebugReportFlagBitsEXT::eInformation
                | vk::DebugReportFlagBitsEXT::eWarning
                | vk::DebugReportFlagBitsEXT::ePerformanceWarning
                | vk::DebugReportFlagBitsEXT::eError
                | vk::DebugReportFlagBitsEXT::eDebug;
        #endif
        iDebugReportCallback.pfnCallback = &Implementation::onDebugReportCallback;
        iDebugReportCallback.pUserData = this;

        mDebugReportCallback = mInstance->createDebugReportCallbackEXTUnique(
            iDebugReportCallback
        );
        CHECK( mDebugReportCallback );
    }

    void initializeDevice( )
    {
        auto physicalDevices = mInstance->enumeratePhysicalDevices( );

        if( VLOG_IS_ON( 1 ) )
        {
            DLOG( INFO ) << "Physical Devices:";

            vk::PhysicalDeviceProperties properties;
            for( auto&& physicalDevice : physicalDevices )
            {
                physicalDevice.getProperties( &properties );
                DLOG( INFO ) << "> " << properties.deviceName;
                DLOG( INFO ) << "\t API "
                    << VK_VERSION_MAJOR( properties.apiVersion ) << '.'
                    << VK_VERSION_MINOR( properties.apiVersion ) << '.'
                    << VK_VERSION_PATCH( properties.apiVersion );
                DLOG( INFO ) << "\t Driver "
                    << VK_VERSION_MAJOR( properties.driverVersion ) << '.'
                    << VK_VERSION_MINOR( properties.driverVersion ) << '.'
                    << VK_VERSION_PATCH( properties.driverVersion );
            }
        }

        CHECK_GT( physicalDevices.size( ), 0 );
        mPhysicalDevice = physicalDevices.at( 0 );

        /// Select Queue Family
        auto queueFamilyProperties( mPhysicalDevice.getQueueFamilyProperties( ) );

        auto finderQueueFamilyIndex( std::find_if(
            queueFamilyProperties.begin( ),
            queueFamilyProperties.end( ),
            []( const vk::QueueFamilyProperties& properties ) {
                return ( properties.queueFlags & vk::QueueFlagBits::eCompute );
            }
        ) );
        CHECK( finderQueueFamilyIndex != queueFamilyProperties.end( ) );

        auto queueFamilyIndex( static_cast<std::uint32_t>(
            std::distance(
                queueFamilyProperties.begin( ),
                finderQueueFamilyIndex
            )
        ) );
        CHECK_LT( queueFamilyIndex, queueFamilyProperties.size( ) );

        /// Queue
        float priorities( 1.0f );
        vk::DeviceQueueCreateInfo iDeviceQueue;
        iDeviceQueue.queueFamilyIndex = queueFamilyIndex;
        iDeviceQueue.queueCount = 1;
        iDeviceQueue.pQueuePriorities = &priorities;

        vk::DeviceCreateInfo iDevice;
        iDevice.queueCreateInfoCount = 1;
        iDevice.pQueueCreateInfos = &iDeviceQueue;

        mDevice = mPhysicalDevice.createDeviceUnique( iDevice );
        CHECK( mDevice );

        mQueue = mDevice->getQueue( queueFamilyIndex, 0 );
    }

    void initializeShader( )
    {
        auto shaders = loadShaders( *mDevice );

        /*mShader =*/;
    }

    void initializePipeline( )
    {

    }
};

vkToolsetApplication::vkToolsetApplication( )
    : mImplementation( new Implementation )
{
}

vkToolsetApplication::~vkToolsetApplication( )
{
    delete mImplementation;
    mImplementation = nullptr;
}
