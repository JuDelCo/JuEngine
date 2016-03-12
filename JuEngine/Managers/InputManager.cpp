// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "InputManager.hpp"
#include "../OpenGL.hpp"
#include "../App.hpp"
#include "../Services/IWindowService.hpp"

namespace JuEngine
{
InputManager::InputManager()
{
	SetId("inputManager");
}

void InputManager::Update()
{
	if(! App::Window()->HasFocus())
	{
		return;
	}

	for(auto &key : mKeyBindings)
	{
		key.second.prevPressed = key.second.pressed;
		key.second.pressed = (glfwGetKey(App::Window()->GetWindow(), key.second.key) == GLFW_PRESS);
	}

	for(auto &button : mMouseBindings)
	{
		button.second.prevPressed = button.second.pressed;
		button.second.pressed = (glfwGetMouseButton(App::Window()->GetWindow(), button.second.button) == GLFW_PRESS);
	}

	double xPos, yPos;
	glfwGetCursorPos(App::Window()->GetWindow(), &xPos, &yPos);
	mMousePosition = vec2(xPos, yPos);

	mMouseWheelDeltaPrev = mMouseWheelDelta;
	mMouseWheelDelta = 0.f;
}

void InputManager::ResetBindings()
{
	mKeyBindings.clear();
}

void InputManager::BindKey(const Identifier& action, const KeyboardKey key)
{
	mKeyBindings[action] = KeyInfo{key, false, false};
}

void InputManager::BindMouse(const Identifier& action, const MouseButton button)
{
	mMouseBindings[action] = MouseInfo{button, false, false};
}

bool InputManager::IsPressed(const Identifier& action)
{
	try
	{
		auto key = mKeyBindings.at(action);
		return (key.pressed && ! key.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = mMouseBindings.at(action);
		return (button.pressed && ! button.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	return false;
}

bool InputManager::IsHeld(const Identifier& action)
{
	try
	{
		auto key = mKeyBindings.at(action);
		return key.pressed;
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = mMouseBindings.at(action);
		return button.pressed;
	}
	catch (const std::out_of_range& oor) {}

	return false;
}

bool InputManager::IsReleased(const Identifier& action)
{
	try
	{
		auto key = mKeyBindings.at(action);
		return (! key.pressed && key.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	try
	{
		auto button = mMouseBindings.at(action);
		return (! button.pressed && button.prevPressed);
	}
	catch (const std::out_of_range& oor) {}

	return false;
}

auto InputManager::MouseGetPosition() -> const vec2&
{
	return mMousePosition;
}

void InputManager::MouseSetPosition(const vec2 position)
{
	glfwSetCursorPos(App::Window()->GetWindow(), position.x, position.y);

	mMousePosition = position;
}

auto InputManager::MouseGetWheelDelta() -> const int&
{
	return mMouseWheelDeltaPrev;
}

bool InputManager::IsMouseOverWindow()
{
	if(mMousePosition.x >= 0 &&
	   mMousePosition.y >= 0 &&
	   mMousePosition.x <= App::Window()->GetSize().x &&
	   mMousePosition.y <= App::Window()->GetSize().y)
	{
		return true;
	}

	return false;
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
	mMouseWheelDelta = yOffset;
}
}
