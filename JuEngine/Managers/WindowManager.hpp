// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../Math.hpp"

class GLFWwindow;

namespace JuEngine
{
class Renderer;

class JUENGINEAPI WindowManager : public IObject
{
friend class Application;
friend class InputManager;

public:
	WindowManager();
	virtual ~WindowManager();

	static bool HasFocus();
	static void SetTitle(const string& title);
	static vec2 GetSize();
	static void SetSize(const unsigned int width, const unsigned int height);
	static vec2 GetPosition();
	static void SetPosition(const int left, const int top);
	static bool GetCloseState();
	static void SetCloseState(const bool close);
	static auto GetClipboardString() -> const string&;
	static void SetClipboardString(const string& text);

protected:
	void Load();
	void Render();
	void SwapBuffers();
	void PollEvents();
	auto GetRenderer() -> Renderer*;
	void SetRenderer(Renderer* renderer);
	void SetActiveInThisThread(const bool active = true);
	static auto GetWindow() -> GLFWwindow*;

private:
	static void CallbackWindowSize(GLFWwindow* window, int width, int height);
	static void CallbackFramebufferSize(GLFWwindow* window, int width, int height);
	static void CallbackWindowPosition(GLFWwindow* window, int xPos, int yPos);
	static void CallbackWindowFocus(GLFWwindow* window, int focused);
	static void CallbackDrop(GLFWwindow* window, int count, const char** paths);
	static void CallbackClose(GLFWwindow* window);

	GLFWwindow* mWindow{nullptr};
	bool mWindowCloseRequested{false};
	bool mWindowHasFocus{true};
	vec2 mWindowPosition{0,0};
	vec2 mWindowSize{0,0};
	vec2 mWindowFramebufferSize{0,0};
	shared_ptr<Renderer> mRenderer;

	// Singleton
	static WindowManager* mInstance;
};
}
