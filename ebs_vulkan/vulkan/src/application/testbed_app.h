#ifndef TESTBED_APP_H
#define TESTBED_APP_H

// Internal includes.
#include "window.h"
#include "graphics/gpu_device.h"

namespace ebs
{

	class testbed_app
	{
	public:
		int init();
		void shutdown();
		void main_loop();
	
		bool should_terminate_app();

	private:
		int init_window(const window_config& win_cfg);
		int init_graphics(const gpu_device_config& win_cfg);

		window m_window;
		gpu_device m_gpu_device;
		bool m_terminate_app = false;
	}; // !class testbed_app

} // !namespace ebs

#endif // !TESTBED_APP_H