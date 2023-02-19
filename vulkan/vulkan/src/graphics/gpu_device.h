#ifndef GPU_DEVICE_H
#define GPU_DEVICE_H

// External includes.
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_win32.h>

namespace ebs
{

	class gpu_device
	{
	public:
		//gpu_device();
	
		int init();
		void shutdown();
	
	private:
		int create_instance();

		VkInstance m_instance;
		VkDebugUtilsMessengerEXT m_debug_utils_messenger;
		VkPhysicalDevice m_physical_device;
		VkPhysicalDeviceProperties m_physical_device_properties;
		VkQueue m_queue;
		u32 m_queue_family_index;
		VkDevice m_device;

	}; // !class gpu_device

} // !namespace ebs

#endif // !GPU_DEVICE_H