#pragma once
#include <string>

namespace apryx {

	class Window {
	protected:
		int m_Width;
		int m_Height;
		bool m_Fullscreen;
		bool m_VSync;
		std::string m_Title;

	public:
		Window(std::string title, int width = 640, int height = 480, bool full = false, bool vsync = true) : m_Title(title), m_Width(width), m_Height(height), m_Fullscreen(full), m_VSync(vsync) {};
		
		virtual void destroy() = 0;

		virtual bool isCloseRequested() const = 0;
		virtual bool isResized() const = 0;

		virtual float dpiScale() const { return 1; }

		virtual float getWidth() const = 0;
		virtual float getHeight() const = 0;

		virtual int getRawWidth() const = 0;
		virtual int getRawHeight() const = 0;

		virtual void setVisible(bool f) = 0;
		virtual void setFullscreen(bool f) = 0;
		virtual void setVSync(bool f) = 0;
		
		// Poll should return event queue
		virtual void poll() = 0;
		virtual void swap() = 0;

	};

}