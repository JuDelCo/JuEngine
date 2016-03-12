// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "WindowManager.hpp"
#include "../Resources/Renderer.hpp"
#include "../OpenGL.hpp"
#include "../App.hpp"
#include "../Services/IInputService.hpp"
#include "../ImGui/imgui.hpp"
#include "../ImGui/impl/imgui_impl_glfw.h"
#include <thread>

namespace JuEngine
{
void ErrorCallbackGLFW(int error, const char* description)
{
	JuEngine::App::Log()->Error("GLFW Error (#%i): %s", error, description);
}

WindowManager::WindowManager()
{
	SetId("windowManager");
}

WindowManager::~WindowManager()
{
	ImGui_ImplGlfw_Shutdown();
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

bool WindowManager::HasFocus()
{
	// TODO
	// glfwGetWindowAttrib(mWindow, GLFW_FOCUSED);

	return mWindowHasFocus;
}

void WindowManager::SetTitle(const std::string& title)
{
	glfwSetWindowTitle(mWindow, title.c_str());
}

vec2 WindowManager::GetSize()
{
	// TODO
	// int width, height; glfwGetFramebufferSize(mWindow, &width, &height);

	return mWindowFramebufferSize;
}

void WindowManager::SetSize(const unsigned int width, const unsigned int height)
{
	glfwSetWindowSize(mWindow, width, height);
}

vec2 WindowManager::GetPosition()
{
	// TODO
	// int xPos, yPos; glfwGetWindowPos(mWindow, &xPos, &yPos);

	return mWindowPosition;
}

void WindowManager::SetPosition(const int left, const int top)
{
	glfwSetWindowPos(mWindow, left, top);
}

bool WindowManager::GetCloseState()
{
	// TODO
	//return glfwWindowShouldClose(mWindow);

	return mWindowCloseRequested;
}

void WindowManager::SetCloseState(const bool close)
{
	mWindowCloseRequested = close;

	// TODO
	// if(close) glfwSetWindowShouldClose(mWindow, GL_TRUE);
	// else glfwSetWindowShouldClose(mWindow, GL_FALSE);
}

auto WindowManager::GetClipboardString() -> const std::string&
{
	std::string* clipboardStr = new std::string();

	clipboardStr->assign(glfwGetClipboardString(mWindow));

	return std::move(*clipboardStr);
}

void WindowManager::SetClipboardString(const std::string& text)
{
	glfwSetClipboardString(mWindow, text.c_str());
}

auto WindowManager::GetRenderer() -> std::shared_ptr<Renderer>
{
	if(mRenderer.get() != nullptr)
	{
		return mRenderer;
	}

	App::Log()->Warning("Warning: WindowManager.GetRenderer: No renderer set (in %s)", GetId().GetStringRef().c_str());

	return nullptr;
}

void WindowManager::SetRenderer(std::shared_ptr<Renderer> renderer)
{
	mRenderer = renderer;
}

void WindowManager::CallbackWindowSize(GLFWwindow* window, int width, int height)
{
	mWindowSize = vec2(width, height);
}

void WindowManager::CallbackFramebufferSize(GLFWwindow* window, int width, int height)
{
	mWindowFramebufferSize = vec2(width, height);
}

void WindowManager::CallbackWindowPosition(GLFWwindow* window, int xPos, int yPos)
{
	mWindowPosition = vec2(xPos, yPos);
}

void WindowManager::CallbackWindowFocus(GLFWwindow* window, int focused)
{
	mWindowHasFocus = focused;
}

void WindowManager::CallbackDrop(GLFWwindow* window, int count, const char** paths)
{
	// TODO
	// for(int i = 0;  i < count;  ++i) handle_dropped_file(paths[i]);
}

void WindowManager::CallbackClose(GLFWwindow* window)
{
	mWindowCloseRequested = true;
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
	// TODO
	//glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

	// TODO: WindowManager: Resolution, Title, etc -> Make an interface (so I can edit them outside this class)
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
	ImGui_ImplGlfw_Init(mWindow, false);
	ImGui_ImplGlfw_NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = NULL;

	App::Log()->Debug("-------------------------------------------------");
	App::Log()->Debug("OpenGL context settings:");
	App::Log()->Debug("-------------------------------------------------");
	App::Log()->Debug("CPU Threads: %i", std::thread::hardware_concurrency());
	App::Log()->Debug("GL Version: %s", glGetString(GL_VERSION));
	App::Log()->Debug("GPU Vendor: %s", glGetString(GL_VENDOR));
	App::Log()->Debug("GPU Renderer: %s", glGetString(GL_RENDERER));
	App::Log()->Debug("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	GLint contextProfile; glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &contextProfile);
	App::Log()->Debug("GL Context profile mask: %i", contextProfile);
	//if (GLEW_ARB_compatibility) App::Log()->Debug("Compatibility profile detected"); else App::Log()->Debug("Core profile detected");
	GLint maxUBOSize; glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUBOSize);
	App::Log()->Debug("UBO max size: %i bytes", maxUBOSize);
	GLint maxUBOVertex; glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxUBOVertex);
	App::Log()->Debug("UBO max count: %i", maxUBOVertex);
	GLint offsetUBOAligment; glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offsetUBOAligment);
	App::Log()->Debug("UBO offset alignment: %i bytes", offsetUBOAligment); // alignment for multiple uniform blocks in one UBO - glBindBufferRange()
	App::Log()->Debug("GLFW version: %s", glfwGetVersionString());
	GLint forwardCompat = glfwGetWindowAttrib(mWindow, GLFW_OPENGL_FORWARD_COMPAT);
	App::Log()->Debug("GLFW GL Forward Compat mode: %s", (forwardCompat == GL_TRUE ? "True" : "False"));
	GLint debugMode = glfwGetWindowAttrib(mWindow, GLFW_OPENGL_DEBUG_CONTEXT);
	App::Log()->Debug("GLFW GL Debug mode: %s", (debugMode == GL_TRUE ? "True" : "False"));
	GLint glProfile = glfwGetWindowAttrib(mWindow, GLFW_OPENGL_PROFILE);
	App::Log()->Debug("GLFW GL Profile: %s", (glProfile == GLFW_OPENGL_CORE_PROFILE ? "Core" : (glProfile == GLFW_OPENGL_COMPAT_PROFILE ? "Compatibility" : "Unknown")));
	App::Log()->Debug("-------------------------------------------------");

	if(! GLEW_VERSION_3_3)
	{
		// TODO: WindowManager: Exit if < GL v3.3
		ThrowRuntimeError("Error, OpenGL 3.3 or superior is required to run this program");
	}

	// --------------------------------

	// TODO
	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
		App::Window()->CallbackWindowSize(window, width, height);
	});
	glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
		App::Window()->CallbackFramebufferSize(window, width, height);
	});
	glfwSetWindowPosCallback(mWindow, [](GLFWwindow* window, int xPos, int yPos) {
		App::Window()->CallbackWindowPosition(window, xPos, yPos);
	});
	glfwSetWindowFocusCallback(mWindow, [](GLFWwindow* window, int focused) {
		App::Window()->CallbackWindowFocus(window, focused);
	});
	glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window) {
		App::Window()->CallbackClose(window);
	});
	glfwSetDropCallback(mWindow, [](GLFWwindow* window, int count, const char** paths) {
		App::Window()->CallbackDrop(window, count, paths);
	});
	glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scanCode, int action, int mods) {
		App::Input()->CallbackKeyEvent(window, key, scanCode, action, mods);
		ImGui_ImplGlfw_KeyCallback(window, key, scanCode, action, mods);
	});
	glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int codePoint) {
		App::Input()->CallbackTextEvent(window, codePoint);
		ImGui_ImplGlfw_CharCallback(window, codePoint);
	});
	glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xPos, double yPos) {
		App::Input()->CallbackMouseMoveEvent(window, xPos, yPos);
	});
	glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods) {
		App::Input()->CallbackMouseButtonEvent(window, button, action, mods);
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	});
	glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xOffset, double yOffset) {
		App::Input()->CallbackMouseScrollEvent(window, xOffset, yOffset);
		ImGui_ImplGlfw_ScrollCallback(window, xOffset, yOffset);
	});
}

void WindowManager::Render()
{
	mRenderer->Render();
	ImGui::Render();

	SwapBuffers();
	ImGui_ImplGlfw_NewFrame();
}

void WindowManager::SwapBuffers()
{
	glfwSwapBuffers(mWindow);
}

void WindowManager::PollEvents()
{
	glfwPollEvents();
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
	if(mWindow != nullptr)
	{
		return mWindow;
	}

	App::Log()->Warning("Warning: WindowManager.GetWindow: No window set");

	return nullptr;
}
}
