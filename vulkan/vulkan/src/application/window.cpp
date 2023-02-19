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
	}

	// Window ////////////////////////////////////////////////////
	int window::init(const window_config& window_cfg)
	{
		std::cout << "Creating window.\n";
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_window_handle = glfwCreateWindow(window_cfg.width, window_cfg.height, window_cfg.name, nullptr, nullptr);
		glfwSetWindowCloseCallback(m_window_handle, window_close_callback);

		return 0;
	}

	void window::shutdown()
	{
		glfwDestroyWindow(m_window_handle);
		glfwTerminate();
		std::cout << "Shutdown window.\n";
	}

	void window::run()
	{
		glfwPollEvents();
	}

	bool window::should_window_close()
	{
		return glfwWindowShouldClose(m_window_handle);
	}

} // !namespace ebs