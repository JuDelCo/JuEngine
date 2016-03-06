// Copyright (c) 2016 Juan Delgado (JuDelCo)
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

	for(auto &key : mKeyBindings)
	{
		key.second.prevPressed = key.second.pressed;
		key.second.pressed = (glfwGetKey(WindowManager::GetWindow(), key.second.key) == GLFW_PRESS);
	}

	for(auto &button : mMouseBindings)
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
	InputManager::mInstance->mKeyBindings.clear();
}

void InputManager::BindKey(const Identifier& action, const KeyboardKey key)
{
	InputManager::mInstance->mKeyBindings[action] = KeyInfo{key, false, false};
}

void InputManager::BindMouse(const Identifier& action, const MouseButton button)
{
	InputManager::mInstance->mMouseBindings[action] = MouseInfo{button, false, false};
}

bool InputManager::IsPressed(const Identifier& action)
{
	try
	{
		auto key = InputManager::mInstance->mKeyBindings.at(action);
		return (key.pressed && ! key.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = InputManager::mInstance->mMouseBindings.at(action);
		return (button.pressed && ! button.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	return false;
}

bool InputManager::IsHeld(const Identifier& action)
{
	try
	{
		auto key = InputManager::mInstance->mKeyBindings.at(action);
		return key.pressed;
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = InputManager::mInstance->mMouseBindings.at(action);
		return button.pressed;
	}
	catch (const std::out_of_range& oor) {}

	return false;
}

bool InputManager::IsReleased(const Identifier& action)
{
	try
	{
		auto key = InputManager::mInstance->mKeyBindings.at(action);
		return (! key.pressed && key.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = InputManager::mInstance->mMouseBindings.at(action);
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
