// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Math.hpp"
#include "InputDefines.hpp"
#include <unordered_map>

class GLFWwindow;

namespace JuEngine
{
class JUENGINEAPI InputManager : public IObject
{
	friend class WindowManager;

	public:
		InputManager();
		~InputManager();

		void Update();
		static void ResetBindings();
		static void BindKey(const Identifier& action, const KeyboardKey key);
		static void BindMouse(const Identifier& action, const MouseButton button);

		static bool IsPressed(const Identifier& action);
		static bool IsHeld(const Identifier& action);
		static bool IsReleased(const Identifier& action);

		static auto MouseGetPosition() -> const vec2&;
		static void MouseSetPosition(const vec2 position);
		static auto MouseGetWheelDelta() -> const int&;
		static bool IsMouseOverWindow();
		static bool IsWindowActive();

	protected:
		static void CallbackKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods);
		static void CallbackTextEvent(GLFWwindow* window, unsigned int codePoint);
		static void CallbackMouseMoveEvent(GLFWwindow* window, double xPos, double yPos);
		static void CallbackMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
		static void CallbackMouseScrollEvent(GLFWwindow* window, double xOffset, double yOffset);

	private:
		std::unordered_map<Identifier, KeyInfo> mKeyBindings;
		std::unordered_map<Identifier, MouseInfo> mMouseBindings;
		vec2 mMousePosition{0, 0};
		int mMouseWheelDeltaPrev{0};
		int mMouseWheelDelta{0};

		// Singleton
		static InputManager* mInstance;
};
}
