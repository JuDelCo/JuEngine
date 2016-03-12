// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Math.hpp"
#include <memory>

class GLFWwindow;

namespace JuEngine
{
class Renderer;

class JUENGINEAPI IWindowService : public IObject
{
	friend class AppController;
	friend class InputManager;

	public:
		virtual bool HasFocus() = 0;
		virtual void SetTitle(const std::string& title) = 0;
		virtual vec2 GetSize() = 0;
		virtual void SetSize(const unsigned int width, const unsigned int height) = 0;
		virtual vec2 GetPosition() = 0;
		virtual void SetPosition(const int left, const int top) = 0;
		virtual bool GetCloseState() = 0;
		virtual void SetCloseState(const bool close) = 0;
		virtual auto GetClipboardString() -> const std::string& = 0;
		virtual void SetClipboardString(const std::string& text) = 0;
		virtual auto GetRenderer() -> std::shared_ptr<Renderer> = 0;
		virtual void SetRenderer(std::shared_ptr<Renderer> renderer) = 0;

		virtual void CallbackWindowSize(GLFWwindow* window, int width, int height) = 0;
		virtual void CallbackFramebufferSize(GLFWwindow* window, int width, int height) = 0;
		virtual void CallbackWindowPosition(GLFWwindow* window, int xPos, int yPos) = 0;
		virtual void CallbackWindowFocus(GLFWwindow* window, int focused) = 0;
		virtual void CallbackDrop(GLFWwindow* window, int count, const char** paths) = 0;
		virtual void CallbackClose(GLFWwindow* window) = 0;

	protected:
		virtual void Load() = 0;
		virtual void Render() = 0;
		virtual void SwapBuffers() = 0;
		virtual void PollEvents() = 0;
		virtual void SetActiveInThisThread(const bool active = true) = 0;
		virtual auto GetWindow() -> GLFWwindow* = 0;
};
}
