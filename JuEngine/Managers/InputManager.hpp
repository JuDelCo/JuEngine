// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Services/IInputService.hpp"
#include "InputDefines.hpp"
#include <unordered_map>

namespace JuEngine
{
class JUENGINEAPI InputManager : public IInputService
{
	public:
		InputManager();
		~InputManager();

		void Update();
		void ResetBindings();
		void BindKey(const Identifier& action, const KeyboardKey key);
		void BindMouse(const Identifier& action, const MouseButton button);

		bool IsPressed(const Identifier& action);
		bool IsHeld(const Identifier& action);
		bool IsReleased(const Identifier& action);

		auto MouseGetPosition() -> const vec2&;
		void MouseSetPosition(const vec2 position);
		auto MouseGetWheelDelta() -> const int&;
		bool IsMouseOverWindow();

	protected:
		void CallbackKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods);
		void CallbackTextEvent(GLFWwindow* window, unsigned int codePoint);
		void CallbackMouseMoveEvent(GLFWwindow* window, double xPos, double yPos);
		void CallbackMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
		void CallbackMouseScrollEvent(GLFWwindow* window, double xOffset, double yOffset);

	private:
		std::unordered_map<Identifier, KeyInfo> mKeyBindings;
		std::unordered_map<Identifier, MouseInfo> mMouseBindings;
		vec2 mMousePosition{0, 0};
		int mMouseWheelDeltaPrev{0};
		int mMouseWheelDelta{0};
};
}
