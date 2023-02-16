// Internal includes.
#include "pch.h"
#include "application/testbed_app.h"

// Standard includes.
#include <iostream>

int main()
{
	ebs::testbed_app app;
	ebs::app_config config
	{
		ebs::window_config
		{
			800,
			600,
			"ebs_vulkan"
		}
	};

	if (app.init(config) != 0)
	{
		std::cout << "Failed to initialise app.\n";
		return -1;
	}

	while (!app.should_terminate_app())
	{
		app.main_loop();
	}

	app.shutdown();

	return 0;
}