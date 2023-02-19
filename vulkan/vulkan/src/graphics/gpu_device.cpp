// Internal includes.
#include "pch.h"
#include "gpu_device.h"

static const char* g_required_extensions[] =
{
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
};

static const char* g_requested_layers[] =
{
	"VK_LAYER_KHRONOS_validation",
};

namespace ebs
{

	// GPU Device ////////////////////////////////////////////////////
	int gpu_device::init()
	{
		std::cout << "Creating gpu device.\n";

		if (create_instance() != 0)
		{
			return -1;
		}

		return 0;
	}

	void gpu_device::shutdown()
	{
		vkDestroyInstance(instance, nullptr);
		std::cout << "Shutdown gpu device.\n";
	}

	int gpu_device::create_instance()
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
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = nullptr,
			.enabledExtensionCount = ebs_sizeofarray(g_required_extensions),
			.ppEnabledExtensionNames = g_required_extensions,
		};

		result = vkCreateInstance(&create_info, nullptr, &instance);
		if (result != VK_SUCCESS)
		{
			std::cout << "Failed to create Vulkan instance\n";
			return -1;
		}

		u32 extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		std::vector<VkExtensionProperties> extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
		for (u32 i = 0; i < extension_count; ++i)
		{
			std::cout << "\t" << extensions[i].extensionName << "\n";
		}


		return 0;
	}

} // namespace ebs