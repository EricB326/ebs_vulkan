// Internal includes.
#include "pch.h"
#include "testbed_app.h"

static int g_frame_count = 0;

namespace ebs
{

	// Testbed App ////////////////////////////////////////////////////
	int testbed_app::init()
	{
		std::cout << "Creating testbed app.\n";
		window_config win_cfg =
		{
			.width = 800,
			.height = 600,
			.name = "ebs_vulkan"
		};

		if (init_window(win_cfg) != 0)
		{
			return -1;
		}

		if (init_graphics() != 0)
		{
			return -1;
		}

		return 0;
	}

	void testbed_app::shutdown()
	{
		app_gpu_device.shutdown();
		app_window.shutdown();
		std::cout << "Shutdown testbed app.\n";
	}

	void testbed_app::main_loop()
	{
		while (!app_window.should_window_close())
		{
			//std::cout << "frame: " << ++g_frame_count << "\n";
			app_window.run();
		}

		terminate_app = true;
	}

	bool testbed_app::should_terminate_app()
	{
		return terminate_app;
	}

	int testbed_app::init_window(const window_config& window_cfg)
	{
		if (app_window.init(window_cfg) != 0)
		{
			return -1;
		}

		return 0;
	}

	int testbed_app::init_graphics()
	{
		if (app_gpu_device.init() != 0)
		{
			return -1;
		}

		return 0;
	}

} // !namespace ebs
