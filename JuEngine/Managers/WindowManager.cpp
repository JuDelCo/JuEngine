// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "WindowManager.hpp"
#include "InputManager.hpp"
#include "../OpenGL.hpp"
#include "../Resources/Renderer.hpp"
#include "../Resources/DebugLog.hpp"
#include <thread>

namespace JuEngine
{
void ErrorCallbackGLFW(int error, const char* description)
{
	JuEngine::DebugLog::Write("GLFW Error (#%i): %s", error, description);
}

WindowManager* WindowManager::mInstance = nullptr;

WindowManager::WindowManager() : IObject("windowManager")
{
	if(WindowManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are a WindowManager instance created already");
	}

	WindowManager::mInstance = this;
}

WindowManager::~WindowManager()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();

	WindowManager::mInstance = nullptr;
}

bool WindowManager::HasFocus()
{
	// glfwGetWindowAttrib(WindowManager::mInstance->mWindow, GLFW_FOCUSED);

	return WindowManager::mInstance->mWindowHasFocus;
}

void WindowManager::SetTitle(const string& title)
{
	glfwSetWindowTitle(WindowManager::mInstance->mWindow, title.c_str());
}

vec2 WindowManager::GetSize()
{
	// int width, height; glfwGetFramebufferSize(WindowManager::mInstance->mWindow, &width, &height);

	return WindowManager::mInstance->mWindowFramebufferSize;
}

void WindowManager::SetSize(const unsigned int width, const unsigned int height)
{
	glfwSetWindowSize(WindowManager::mInstance->mWindow, width, height);
}

vec2 WindowManager::GetPosition()
{
	// int xPos, yPos; glfwGetWindowPos(WindowManager::mInstance->mWindow, &xPos, &yPos);

	return WindowManager::mInstance->mWindowPosition;
}

void WindowManager::SetPosition(const int left, const int top)
{
	glfwSetWindowPos(WindowManager::mInstance->mWindow, left, top);
}

bool WindowManager::GetCloseState()
{
	//return glfwWindowShouldClose(WindowManager::mInstance->mWindow);

	return WindowManager::mInstance->mWindowCloseRequested;
}

void WindowManager::SetCloseState(const bool close)
{
	WindowManager::mInstance->mWindowCloseRequested = close;

	/*if(close) glfwSetWindowShouldClose(WindowManager::mInstance->mWindow, GL_TRUE);
	else glfwSetWindowShouldClose(WindowManager::mInstance->mWindow, GL_FALSE);*/
}

auto WindowManager::GetClipboardString() -> const string&
{
	string* clipboardStr = new string();

	clipboardStr->assign(glfwGetClipboardString(WindowManager::mInstance->mWindow));

	return std::move(*clipboardStr);
}

void WindowManager::SetClipboardString(const string& text)
{
	glfwSetClipboardString(WindowManager::mInstance->mWindow, text.c_str());
}

void WindowManager::Load()
{
	if(! glfwInit())
	{
		ThrowRuntimeError("Error, failed to init GLFW");
	}

	glfwSetErrorCallback(ErrorCallbackGLFW);

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, GL_TRUE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60); // For fullscreen
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

	// TODO: WindowManager: Resolución, Título, etc configurables
	mWindow = glfwCreateWindow(1280, 720, "JuEngine Test", NULL, NULL);
	mWindowSize = vec2(1280, 720);
	mWindowFramebufferSize = vec2(1280, 720);

	if (! mWindow)
	{
		glfwTerminate();
		ThrowRuntimeError("Error, failed to create a GLFW window");
	}

	SetActiveInThisThread(true);
	glfwSwapInterval(0);

	glewExperimental = GL_TRUE;
	glewInit();

	DebugLog::Write("-------------------------------------------------");
	DebugLog::Write("OpenGL context settings:");
	DebugLog::Write("-------------------------------------------------");
	DebugLog::Write("CPU Threads: %i", std::thread::hardware_concurrency());
	DebugLog::Write("GL Version: %s", glGetString(GL_VERSION));
	DebugLog::Write("GPU Vendor: %s", glGetString(GL_VENDOR));
	DebugLog::Write("GPU Renderer: %s", glGetString(GL_RENDERER));
	DebugLog::Write("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	GLint contextProfile; glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &contextProfile);
	DebugLog::Write("GL Context profile mask: %i", contextProfile);
	//if (GLEW_ARB_compatibility) DebugLog::Write("Compatibility profile detected"); else DebugLog::Write("Core profile detected");
	GLint maxUBOSize; glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUBOSize);
	DebugLog::Write("UBO max size: %i bytes", maxUBOSize);
	GLint maxUBOVertex; glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxUBOVertex);
	DebugLog::Write("UBO max count: %i", maxUBOVertex);
	GLint offsetUBOAligment; glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offsetUBOAligment);
	DebugLog::Write("UBO offset alignment: %i bytes", offsetUBOAligment); // alignment for multiple uniform blocks in one UBO - glBindBufferRange()
	DebugLog::Write("GLFW version: %s", glfwGetVersionString());
	GLint forwardCompat = glfwGetWindowAttrib(mWindow, GLFW_OPENGL_FORWARD_COMPAT);
	DebugLog::Write("GLFW GL Forward Compat mode: %s", (forwardCompat == GL_TRUE ? "True" : "False"));
	GLint debugMode = glfwGetWindowAttrib(mWindow, GLFW_OPENGL_DEBUG_CONTEXT);
	DebugLog::Write("GLFW GL Debug mode: %s", (debugMode == GL_TRUE ? "True" : "False"));
	GLint glProfile = glfwGetWindowAttrib(mWindow, GLFW_OPENGL_PROFILE);
	DebugLog::Write("GLFW GL Profile: %s", (glProfile == GLFW_OPENGL_CORE_PROFILE ? "Core" : (glProfile == GLFW_OPENGL_COMPAT_PROFILE ? "Compatibility" : "Unknown")));
	DebugLog::Write("-------------------------------------------------");

	// TODO: WindowManager: Exit if < GL v3.3

	// --------------------------------

	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetWindowSizeCallback(mWindow, WindowManager::CallbackWindowSize);
	glfwSetFramebufferSizeCallback(mWindow, WindowManager::CallbackFramebufferSize);
	glfwSetWindowPosCallback(mWindow, WindowManager::CallbackWindowPosition);
	glfwSetWindowFocusCallback(mWindow, WindowManager::CallbackWindowFocus);
	glfwSetWindowCloseCallback(mWindow, WindowManager::CallbackClose);
	glfwSetDropCallback(mWindow, WindowManager::CallbackDrop);
	glfwSetKeyCallback(mWindow, InputManager::CallbackKeyEvent);
	glfwSetCharCallback(mWindow, InputManager::CallbackTextEvent);
	glfwSetCursorPosCallback(mWindow, InputManager::CallbackMouseMoveEvent);
	glfwSetMouseButtonCallback(mWindow, InputManager::CallbackMouseButtonEvent);
	glfwSetScrollCallback(mWindow, InputManager::CallbackMouseScrollEvent);
}

void WindowManager::Render()
{
	mRenderer->Render();

	SwapBuffers();
}

void WindowManager::SwapBuffers()
{
	glfwSwapBuffers(mWindow);
}

void WindowManager::PollEvents()
{
	glfwPollEvents();
}

auto WindowManager::GetRenderer() -> Renderer*
{
	if(mRenderer)
	{
		return mRenderer.get();
	}

	DebugLog::Write("Warning: WindowManager.GetRenderer: No renderer set (in %s)", this->GetName().c_str());

	return nullptr;
}

void WindowManager::SetRenderer(Renderer* renderer)
{
	mRenderer = shared_ptr<Renderer>(renderer);
}

void WindowManager::SetActiveInThisThread(const bool active)
{
	if(mWindow)
	{
		if(active) glfwMakeContextCurrent(mWindow);
		else glfwMakeContextCurrent(NULL);
	}
}

auto WindowManager::GetWindow() -> GLFWwindow*
{
	if(WindowManager::mInstance->mWindow)
	{
		return WindowManager::mInstance->mWindow;
	}

	DebugLog::Write("Warning: WindowManager.GetWindow: No window set");

	return nullptr;
}

void WindowManager::CallbackWindowSize(GLFWwindow* window, int width, int height)
{
	WindowManager::mInstance->mWindowSize = vec2(width, height);
}

void WindowManager::CallbackFramebufferSize(GLFWwindow* window, int width, int height)
{
	WindowManager::mInstance->mWindowFramebufferSize = vec2(width, height);
}

void WindowManager::CallbackWindowPosition(GLFWwindow* window, int xPos, int yPos)
{
	WindowManager::mInstance->mWindowPosition = vec2(xPos, yPos);
}

void WindowManager::CallbackWindowFocus(GLFWwindow* window, int focused)
{
	WindowManager::mInstance->mWindowHasFocus = focused;
}

void WindowManager::CallbackDrop(GLFWwindow* window, int count, const char** paths)
{
	// for(int i = 0;  i < count;  ++i) handle_dropped_file(paths[i]);
}

void WindowManager::CallbackClose(GLFWwindow* window)
{
	WindowManager::mInstance->mWindowCloseRequested = true;
}
}
