// Internal includes.
#include "pch.h"
#include "testbed_app.h"

static int g_frame_count = 0;
static u32 g_app_width = 800;
static u32 g_app_height = 600;

namespace ebs
{

	// Testbed App ////////////////////////////////////////////////////
	int testbed_app::init()
	{
		std::cout << "Creating testbed app.\n";
		window_config win_cfg =
		{
			.width = g_app_width,
			.height = g_app_height,
			.name = "ebs_vulkan"
		};


		if (init_window(win_cfg) != 0)
		{
			return -1;
		}

		gpu_device_config gpu_device_cfg =
		{
			.window_handle = m_window.get_window_handle(),
			.width = g_app_width,
			.height = g_app_height,
		};

		if (init_graphics(gpu_device_cfg) != 0)
		{
			return -1;
		}

		return 0;
	}

	void testbed_app::shutdown()
	{
		m_gpu_device.shutdown();
		m_window.shutdown();
		std::cout << "Shutdown testbed app.\n";
	}

	void testbed_app::main_loop()
	{
		while (!m_window.should_window_close())
		{
			//std::cout << "frame: " << ++g_frame_count << "\n";
			m_window.run();
		}

		m_terminate_app = true;
	}

	bool testbed_app::should_terminate_app()
	{
		return m_terminate_app;
	}

	int testbed_app::init_window(const window_config& win_cfg)
	{
		if (m_window.init(win_cfg) != 0)
		{
			return -1;
		}

		return 0;
	}

	int testbed_app::init_graphics(const gpu_device_config& gpu_device_cfg)
	{
		if (m_gpu_device.init(gpu_device_cfg) != 0)
		{
			return -1;
		}

		return 0;
	}

} // !namespace ebs
