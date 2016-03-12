// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Math.hpp"

class GLFWwindow;

namespace JuEngine
{
typedef int KeyboardKey;
typedef int MouseButton;

class JUENGINEAPI IInputService : public IObject
{
	friend class WindowManager;

	public:
		virtual void Update() = 0;
		virtual void ResetBindings() = 0;
		virtual void BindKey(const Identifier& action, const KeyboardKey key) = 0;
		virtual void BindMouse(const Identifier& action, const MouseButton button) = 0;

		virtual bool IsPressed(const Identifier& action) = 0;
		virtual bool IsHeld(const Identifier& action) = 0;
		virtual bool IsReleased(const Identifier& action) = 0;

		virtual auto MouseGetPosition() -> const vec2& = 0;
		virtual void MouseSetPosition(const vec2 position) = 0;
		virtual auto MouseGetWheelDelta() -> const int& = 0;
		virtual bool IsMouseOverWindow() = 0;

	protected:
		virtual void CallbackKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods) = 0;
		virtual void CallbackTextEvent(GLFWwindow* window, unsigned int codePoint) = 0;
		virtual void CallbackMouseMoveEvent(GLFWwindow* window, double xPos, double yPos) = 0;
		virtual void CallbackMouseButtonEvent(GLFWwindow* window, int button, int action, int mods) = 0;
		virtual void CallbackMouseScrollEvent(GLFWwindow* window, double xOffset, double yOffset) = 0;
};
}
