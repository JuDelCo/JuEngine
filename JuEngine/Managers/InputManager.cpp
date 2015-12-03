// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "InputManager.hpp"
#include "WindowManager.hpp"
#include "../Resources/DebugLog.hpp"
#include "../OpenGL.hpp"

namespace JuEngine
{
InputManager* InputManager::mInstance = nullptr;

InputManager::InputManager() : IObject("inputManager")
{
	if(InputManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are an InputManager instance created already");
	}

	InputManager::mInstance = this;
}

InputManager::~InputManager()
{
	InputManager::mInstance = nullptr;
}

void InputManager::Update()
{
	if(! IsWindowActive())
	{
		return;
	}

	for(auto& key : mIdKeyBindings)
	{
		key.second.prevPressed = key.second.pressed;
		key.second.pressed = (glfwGetKey(WindowManager::GetWindow(), key.second.key) == GLFW_PRESS);
	}

	for(auto& key : mStringKeyBindings)
	{
		key.second.prevPressed = key.second.pressed;
		key.second.pressed = (glfwGetKey(WindowManager::GetWindow(), key.second.key) == GLFW_PRESS);
	}

	for(auto& button : mIdMouseBindings)
	{
		button.second.prevPressed = button.second.pressed;
		button.second.pressed = (glfwGetMouseButton(WindowManager::GetWindow(), button.second.button) == GLFW_PRESS);
	}

	for(auto& button : mStringMouseBindings)
	{
		button.second.prevPressed = button.second.pressed;
		button.second.pressed = (glfwGetMouseButton(WindowManager::GetWindow(), button.second.button) == GLFW_PRESS);
	}

	double xPos, yPos;
	glfwGetCursorPos(WindowManager::GetWindow(), &xPos, &yPos);
	mMousePosition = vec2(xPos, yPos);

	mMouseWheelDeltaPrev = mMouseWheelDelta;
	mMouseWheelDelta = 0.f;
}

void InputManager::ResetBindings()
{
	InputManager::mInstance->mIdKeyBindings.clear();
	InputManager::mInstance->mStringKeyBindings.clear();
}

void InputManager::BindKey(const size_t action, const KeyboardKey key)
{
	InputManager::mInstance->mIdKeyBindings[action] = KeyInfo{key, false, false};
}

void InputManager::BindKey(const string& action, const KeyboardKey key)
{
	InputManager::mInstance->mStringKeyBindings[action] = KeyInfo{key, false, false};
}

void InputManager::BindMouse(const size_t action, const MouseButton button)
{
	InputManager::mInstance->mIdMouseBindings[action] = MouseInfo{button, false, false};
}

void InputManager::BindMouse(const string& action, const MouseButton button)
{
	InputManager::mInstance->mStringMouseBindings[action] = MouseInfo{button, false, false};
}

bool InputManager::IsPressed(const size_t action)
{
	try
	{
		auto key = InputManager::mInstance->mIdKeyBindings.at(action);
		return (key.pressed && ! key.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = InputManager::mInstance->mIdMouseBindings.at(action);
		return (button.pressed && ! button.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	return false;
}

bool InputManager::IsPressed(const string& action)
{
	try
	{
		auto key = InputManager::mInstance->mStringKeyBindings.at(action);
		return (key.pressed && ! key.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = InputManager::mInstance->mStringMouseBindings.at(action);
		return (button.pressed && ! button.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	return false;
}

bool InputManager::IsHeld(const size_t action)
{
	try
	{
		auto key = InputManager::mInstance->mIdKeyBindings.at(action);
		return key.pressed;
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = InputManager::mInstance->mIdMouseBindings.at(action);
		return button.pressed;
	}
	catch (const std::out_of_range& oor) {}

	return false;
}

bool InputManager::IsHeld(const string& action)
{
	try
	{
		auto key = InputManager::mInstance->mStringKeyBindings.at(action);
		return key.pressed;
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = InputManager::mInstance->mStringMouseBindings.at(action);
		return button.pressed;
	}
	catch (const std::out_of_range& oor) {}

	return false;
}

bool InputManager::IsReleased(const size_t action)
{
	try
	{
		auto key = InputManager::mInstance->mIdKeyBindings.at(action);
		return (! key.pressed && key.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = InputManager::mInstance->mIdMouseBindings.at(action);
		return (! button.pressed && button.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	return false;
}

bool InputManager::IsReleased(const string& action)
{
	try
	{
		auto key = InputManager::mInstance->mStringKeyBindings.at(action);
		return (! key.pressed && key.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = InputManager::mInstance->mStringMouseBindings.at(action);
		return (! button.pressed && button.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	return false;
}

auto InputManager::MouseGetPosition() -> const vec2&
{
	return InputManager::mInstance->mMousePosition;
}

void InputManager::MouseSetPosition(const vec2 position)
{
	glfwSetCursorPos(WindowManager::GetWindow(), position.x, position.y);

	InputManager::mInstance->mMousePosition = position;
}

auto InputManager::MouseGetWheelDelta() -> const int&
{
	return InputManager::mInstance->mMouseWheelDeltaPrev;
}

bool InputManager::IsMouseOverWindow()
{
	if(InputManager::mInstance->mMousePosition.x >= 0 &&
	   InputManager::mInstance->mMousePosition.y >= 0 &&
	   InputManager::mInstance->mMousePosition.x <= WindowManager::GetSize().x &&
	   InputManager::mInstance->mMousePosition.y <= WindowManager::GetSize().y)
	{
		return true;
	}

	return false;
}

bool InputManager::IsWindowActive()
{
	return WindowManager::HasFocus();
}

void InputManager::CallbackKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
}

void InputManager::CallbackTextEvent(GLFWwindow* window, unsigned int codePoint)
{
}

void InputManager::CallbackMouseMoveEvent(GLFWwindow* window, double xPos, double yPos)
{
}

void InputManager::CallbackMouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
}

void InputManager::CallbackMouseScrollEvent(GLFWwindow* window, double xOffset, double yOffset)
{
	InputManager::mInstance->mMouseWheelDelta = yOffset;
}
}
