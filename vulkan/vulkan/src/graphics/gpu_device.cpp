// Internal includes.
#include "pch.h"
#include "gpu_device.h"

// External includes.
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace ebs
{

#ifndef NDEBUG
#define	VK_DEBUG_VALIDATION
#endif // !NDEBUG

	static const char* g_requested_extensions[] =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#if defined(VK_DEBUG_VALIDATION)
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME, // #TODO - Also support depreciated VK_EXT_debug_report? Are we in a time where not all new layer features are supported? https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VK_EXT_debug_utils.html#_issues
#endif // !VK_DEBUG_VALIDATION
	};

	static const char* g_requested_layers[] =
	{
#if defined(VK_DEBUG_VALIDATION)
		"VK_LAYER_KHRONOS_validation",
#endif // !VK_DEBUG_VALIDATION
	};

#if defined(VK_DEBUG_VALIDATION)
	static VkBool32 debug_utils_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT types, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) 
	{
		switch (types)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			std::cout << "(" << callback_data->pMessageIdName << " | " << callback_data->messageIdNumber << " | GENERAL) " << callback_data->pMessage << "\n";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			std::cout << "(" << callback_data->pMessageIdName << " | " << callback_data->messageIdNumber << " | VALIDATION) " << callback_data->pMessage << "\n";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			std::cout << "(" << callback_data->pMessageIdName << " | " << callback_data->messageIdNumber << " | PERFORMANCE) " << callback_data->pMessage << "\n";
			break;
		default:
			std::cout << "(" << callback_data->pMessageIdName << " | " << callback_data->messageIdNumber << "): " << callback_data->pMessage << "\n";
			break;
		}
		return VK_FALSE;
	}
	
	VkDebugUtilsMessengerCreateInfoEXT create_debug_utils_messager_info()
	{
		// #TODO - Drop verbose serverity and general type eventually.
		return
		{
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.pNext = nullptr,
			.flags = 0,
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
			.pfnUserCallback = debug_utils_callback,
			.pUserData = nullptr
		};
	}
#endif // !VK_DEBUG_VALIDATION

	// GPU Device ////////////////////////////////////////////////////
	int gpu_device::init(const gpu_device_config& cfg)
	{
		std::cout << "Creating gpu device.\n";

		if (create_instance(cfg) != 0)
		{
			return -1;
		}

		return 0;
	}

	void gpu_device::shutdown()
	{
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

		vkDestroyDevice(m_device, nullptr);

#if defined(VK_DEBUG_VALIDATION)
		PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
		vkDestroyDebugUtilsMessengerEXT(m_instance, m_debug_utils_messenger, nullptr);
#endif // !VK_DEBUG_VALIDATION

		vkDestroyInstance(m_instance, nullptr);

		std::cout << "Shutdown gpu device.\n";
	}

	int gpu_device::create_instance(const gpu_device_config& cfg)
	{
		VkResult result;

		VkApplicationInfo app_info
		{
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = "EBS Vulkan",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName = "EBS",
			.engineVersion = VK_MAKE_VERSION(1, 0 ,0),
			.apiVersion = VK_API_VERSION_1_2
		};

		VkInstanceCreateInfo create_info
		{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.pApplicationInfo = &app_info,
#if defined(VK_DEBUG_VALIDATION) // #TODO - Should very well occur after I check for existence..
			.enabledLayerCount = ebs_sizeofarray(g_requested_layers),
			.ppEnabledLayerNames = g_requested_layers,
#else
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = nullptr,
#endif // !VK_DEBUG_VALIDATION
			.enabledExtensionCount = ebs_sizeofarray(g_requested_extensions), // Not entirely safe, assuming that the system this code is being executed on in debug mode will already have the debug utils ext available.
			.ppEnabledExtensionNames = g_requested_extensions,
		};

#if defined(VK_DEBUG_VALIDATION)
		// Having debug messenger for create and destroy instance calls.
		const VkDebugUtilsMessengerCreateInfoEXT debug_utils_create_info = create_debug_utils_messager_info();
		create_info.pNext = &debug_utils_create_info;
#endif // !VK_DEBUG_VALIDATION

		result = vkCreateInstance(&create_info, nullptr, &m_instance);
		if (result != VK_SUCCESS)
		{
			std::cout << "Failed to create Vulkan instance.\n";
			return -1;
		}

		// #TODO - Verify existence of my "required" extensions? This app is designed to run purely on win32 as of writing this...
#if defined(VK_DEBUG_VALIDATION)
		u32 extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		std::vector<VkExtensionProperties> extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
		bool debug_utils_avaliable = false;
		for (u32 i = 0; i < extension_count; ++i)
		{
			if (!strcmp(extensions[i].extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
			{
				debug_utils_avaliable = true;
				break;
			}
		}

		if (!debug_utils_avaliable)
		{
			std::cout << "Debug utils extension requested, but are not available.\n";
		}
		else
		{
			VkDebugUtilsMessengerCreateInfoEXT debug_utils_create_info = create_debug_utils_messager_info();
			PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
			vkCreateDebugUtilsMessengerEXT(m_instance, &debug_utils_create_info, nullptr, &m_debug_utils_messenger);
		}

		// #TODO - Should I bother checking the layers for the validation layer if I already verified the debug extension?
		//u32 layer_count = 0;
		//vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
		//std::vector<VkLayerProperties> layers(layer_count);
		//vkEnumerateInstanceLayerProperties(&layer_count, layers.data());
		//for (u32 i = 0; i < layer_count; ++i)
		//{
		//	if (!strcmp(layers[i].layerName, g_requested_layers[i]))
		//	{
		//		debug_utils_avaliable = true;
		//		break;
		//	}
		//}
#endif // !VK_DEBUG_VALIDATION

		u32 device_count = 0;
		vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);
		std::vector<VkPhysicalDevice> devices(device_count);
		vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

		// #TODO - Improve. For now, use the first discrete device
		for (u32 i = 0; i < device_count; ++i)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(devices[i], &properties);
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				m_physical_device = devices[i];
				m_physical_device_properties = properties;
				break;
			}
		}

		u32 queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, nullptr);
		std::vector<VkQueueFamilyProperties> queue_familes(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, queue_familes.data());

		u32 family_index = 0;
		for (; family_index < queue_family_count; ++family_index)
		{
			if (queue_familes[family_index].queueCount > 0 && queue_familes[family_index].queueFlags & (VK_QUEUE_GRAPHICS_BIT))
			{
				break;
			}
		}

		float queue_priority = 1.0f;
		VkDeviceQueueCreateInfo queue_create_info
		{
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.queueFamilyIndex = family_index,
			.queueCount = 1,
			.pQueuePriorities = &queue_priority
		};

		VkPhysicalDeviceFeatures2 device_features
		{
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
			.pNext = nullptr
		};
		vkGetPhysicalDeviceFeatures2(m_physical_device, &device_features);

		const char* device_extensions[] =
		{
			"VK_KHR_swapchain",
		};

		VkDeviceCreateInfo device_create_info
		{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = &device_features, // Enable all available.
			.flags = 0,
			.queueCreateInfoCount = 1,
			.pQueueCreateInfos = &queue_create_info,
			.enabledExtensionCount = ebs_sizeofarray(device_extensions),
			.ppEnabledExtensionNames = device_extensions,
		};

		result = vkCreateDevice(m_physical_device, &device_create_info, nullptr, &m_device);
		if (result != VK_SUCCESS)
		{
			std::cout << "Failed to create logical device.\n";
			return -1;
		}

		vkGetDeviceQueue(m_device, family_index, 0, &m_queue);
		m_queue_family_index = family_index;

		if (glfwCreateWindowSurface(m_instance, cfg.window_handle, nullptr, &m_surface) != VK_SUCCESS)
		{
			std::cout << "Failed to create window surface.\n";
			return -1;
		}

		return 0;
	}

} // !namespace ebs