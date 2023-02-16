// Internal includes.
#include "pch.h"
#include "testbed_app.h"

static int g_frame_count = 0;

namespace ebs
{
	int testbed_app::init(const app_config& app_cfg)
	{
		if (init_window(app_cfg.window_cfg) != 0)
		{
			return -1;
		}

		std::cout << "testbed app created successfully.\n";
		return 0;
	}

	void testbed_app::shutdown()
	{
		app_window.shutdown();
		std::cout << "testbed app shutdown successfully.\n";
	}

	void testbed_app::main_loop()
	{
		while (!app_window.should_window_close())
		{
			std::cout << "frame: " << ++g_frame_count << "\n";
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
		return 0;
	}
} // namespace ebs
