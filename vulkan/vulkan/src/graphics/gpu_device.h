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

		VkInstance instance;
	
	}; // class gpu_device

} // namespace ebs

#endif // !GPU_DEVICE_H