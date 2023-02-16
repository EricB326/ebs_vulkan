// Internal includes.
#include "pch.h"
#include "window.h"

// External includes.
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace ebs
{
	// Callbacks /////////////////////////////////////////////////
	void window_close_callback(GLFWwindow* handle)
	{
		glfwSetWindowShouldClose(handle, GLFW_TRUE);
		std::cout << "window closing.\n";
	}

	// Window ////////////////////////////////////////////////////
	int window::init(const window_config& window_cfg)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window_handle = glfwCreateWindow(window_cfg.width, window_cfg.height, window_cfg.name, nullptr, nullptr);
		glfwSetWindowCloseCallback(window_handle, window_close_callback);

		std::cout << "window created successfully.\n";
		return 0;
	}

	void window::shutdown()
	{
		glfwDestroyWindow(window_handle);
		glfwTerminate();

		std::cout << "window shutdown successfully.\n";
	}

	void window::run()
	{
		glfwPollEvents();
	}

	bool window::should_window_close()
	{
		return glfwWindowShouldClose(window_handle);
	}

} // namespace ebs