#ifndef GPU_DEVICE_H
#define GPU_DEVICE_H

namespace ebs
{
	class gpu_device
	{
	public:
		gpu_device();
	
		int init();
		void shutdown();
	
	private:
	
	}; // class gpu_device
} // namespace ebs

#endif // !GPU_DEVICE_H