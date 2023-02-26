#ifndef GPU_DEVICE_H
#define GPU_DEVICE_H

// External includes.
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_win32.h>

struct GLFWwindow;

namespace ebs
{
	struct gpu_device_config
	{
		GLFWwindow* window_handle; // #TODO - Make void* incase I ever switch from GLFW and don't want to keep returning here?
		u32 width;
		u32 height;
	}; // !struct gpu_device_config

	class gpu_device
	{
	public:
		int init(const gpu_device_config& cfg);
		void shutdown();
	
	private:
		int create_instance(const gpu_device_config& cfg);

		VkInstance m_instance;
		VkDebugUtilsMessengerEXT m_debug_utils_messenger;
		VkPhysicalDevice m_physical_device;
		VkPhysicalDeviceProperties m_physical_device_properties;
		VkQueue m_queue;
		u32 m_queue_family_index;
		VkDevice m_device;

		VkSurfaceKHR m_surface;
		VkSurfaceFormatKHR m_surface_format;
		VkPresentModeKHR m_present_mode;
		VkSwapchainKHR m_swapchain;
		u16 m_swapchain_width;
		u16 m_swapchain_height;
	}; // !class gpu_device

} // !namespace ebs

#endif // !GPU_DEVICE_H