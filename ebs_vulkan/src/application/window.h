#ifndef WINDOW_H
#define WINDOW_H

struct GLFWwindow;

namespace ebs
{

	struct window_config
	{
		u32 width;
		u32 height;
		const char* name;
	}; // !struct window_config

	class window
	{
	public:
		int init(const window_config& cfg);
		void shutdown();
		void run();

		bool should_window_close();
		GLFWwindow* get_window_handle() { return m_window_handle; }
	private:

		GLFWwindow* m_window_handle;

	}; // !class window

} // !namespace ebs

#endif // !WINDOW_H