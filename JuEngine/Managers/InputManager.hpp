// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../Math.hpp"
#include "InputDefines.hpp"
#include <unordered_map>

class GLFWwindow;

namespace JuEngine
{
using std::size_t;
using std::unordered_map;

class JUENGINEAPI InputManager : public IObject
{
friend class WindowManager;

public:
	InputManager();
	virtual ~InputManager();

	void Update();
	static void ResetBindings();
	static void BindKey(const size_t action, const KeyboardKey key);
	static void BindKey(const string& action, const KeyboardKey key);
	static void BindMouse(const size_t action, const MouseButton button);
	static void BindMouse(const string& action, const MouseButton button);

	static bool IsPressed(const size_t action);
	static bool IsPressed(const string& action);
	static bool IsHeld(const size_t action);
	static bool IsHeld(const string& action);
	static bool IsReleased(const size_t action);
	static bool IsReleased(const string& action);

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
	unordered_map<size_t, KeyInfo> mIdKeyBindings;
	unordered_map<string, KeyInfo> mStringKeyBindings;
	unordered_map<size_t, MouseInfo> mIdMouseBindings;
	unordered_map<string, MouseInfo> mStringMouseBindings;
	vec2 mMousePosition{0, 0};
	int mMouseWheelDeltaPrev{0};
	int mMouseWheelDelta{0};

	// Singleton
	static InputManager* mInstance;
};
}
