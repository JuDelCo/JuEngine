// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Services/IWindowService.hpp"

namespace JuEngine
{
class JUENGINEAPI WindowManager : public IWindowService
{
	public:
		WindowManager();
		~WindowManager();

		bool HasFocus();
		void SetTitle(const std::string& title);
		vec2 GetSize();
		void SetSize(const unsigned int width, const unsigned int height);
		vec2 GetPosition();
		void SetPosition(const int left, const int top);
		bool GetCloseState();
		void SetCloseState(const bool close);
		auto GetClipboardString() -> const std::string&;
		void SetClipboardString(const std::string& text);
		auto GetRenderer() -> std::shared_ptr<Renderer>;
		void SetRenderer(std::shared_ptr<Renderer> renderer);

		void CallbackWindowSize(GLFWwindow* window, int width, int height);
		void CallbackFramebufferSize(GLFWwindow* window, int width, int height);
		void CallbackWindowPosition(GLFWwindow* window, int xPos, int yPos);
		void CallbackWindowFocus(GLFWwindow* window, int focused);
		void CallbackDrop(GLFWwindow* window, int count, const char** paths);
		void CallbackClose(GLFWwindow* window);

	protected:
		void Load();
		void Render();
		void SwapBuffers();
		void PollEvents();
		void SetActiveInThisThread(const bool active = true);
		auto GetWindow() -> GLFWwindow*;

	private:
		GLFWwindow* mWindow{nullptr};
		bool mWindowCloseRequested{false};
		bool mWindowHasFocus{true};
		vec2 mWindowPosition{0,0};
		vec2 mWindowSize{0,0};
		vec2 mWindowFramebufferSize{0,0};
		std::shared_ptr<Renderer> mRenderer;
};
}
