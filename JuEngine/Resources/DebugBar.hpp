// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../Math.hpp"

class GLFWwindow;

namespace JuEngine
{
enum class VarType
{
	BOOL, CHAR, STRING,
	INT8, INT16, INT32,
	UINT8, UINT16, UINT32,
	FLOAT, DOUBLE,
	COLOR32, COLOR3, COLOR4,
	QUAT4, DIR3
};

typedef void (*DebugBarButtonCallback)(void* clientData);
typedef void (*DebugBarSetCallback)(const void* value, void* clientData);
typedef void (*DebugBarGetCallback)(void* value, void* clientData);

class JUENGINEAPI DebugBar : public IObject
{
friend class WindowManager;

public:
	DebugBar(const string& name);
	~DebugBar();

	auto GetBarName() -> const string&;
	void ForceRefresh();
	void Show();
	void Hide();
	void SetIconified(bool iconified);
	void BringToFront();
	void SendToBack();
	void SetParams(const string& params);
	void SetRefreshRate(float seconds);
	void SetSize(vec2 size);
	void SetWidthVar(int size);
	void SetPosition(vec2 position);
	void SetLabel(const string& label);
	void SetHelp(const string& help);
	void SetColor(vec3 color);
	void SetAlpha(int alpha);
	void SetMovable(bool movable);
	void SetResizable(bool resizable);
	void SetIconifiable(bool iconifiable);
	void SetAlwaysTop(bool alwaysTop);
	void SetAlwaysBottom(bool alwaysBottom);
	void AddSeparator(const string& name, const string& params = "");
	void AddButton(const string& name, DebugBarButtonCallback callback, void* clientData = NULL, const string& params = "");
	void AddVarReadOnly(const string& name, VarType type, const void *var, const string& params = "");
	void AddVarReadWrite(const string& name, VarType type, void *var, const string& params = "");
	void AddVarCallback(const string& name, VarType type, DebugBarSetCallback setCallback, DebugBarGetCallback getCallback, void* clientData = NULL, const string& params = "");
	void AddVarReadOnly(const string& name, const string& enumName, const void *var, const string& params = "");
	void AddVarReadWrite(const string& name, const string& enumName, void *var, const string& params = "");
	void AddVarCallback(const string& name, const string& enumName, DebugBarSetCallback setCallback, DebugBarGetCallback getCallback, void* clientData = NULL, const string& params = "");
	void SetGroupOpened(const string& name, bool opened);
	void SetVarParams(const string& name, const string& params = "");
	void SetVarGroup(const string& name, const string& group);
	void SetVarLabel(const string& name, const string& label);
	void SetVarHelp(const string& name, const string& help);
	void SetVarVisible(const string& name, bool visible);
	void SetVarReadOnly(const string& name, bool readOnly);
	void SetVarKey(const string& name, const string& key);
	void SetVarKeys(const string& name, const string& keyIncr, const string& keyDecr);
	void SetVarNumClamp(const string& name, int min, int max);
	void SetVarNumStep(const string& name, float step);
	void SetVarNumPrecision(const string& name, int precision);
	void SetVarHexadecimal(const string& name, bool hexadecimal);
	void SetVarBoolStrings(const string& name, const string& trueStr, const string& falseStr);
	void SetVarArrowColor(const string& name, vec3 color);
	void SetVarArrowShowValues(const string& name, bool showValues);
	void RemoveVar(const string& name);
	void RemoveAllVars();
	static void SetGlobalParameters(const string& params);
	static void DefineEnum(const string& name, const string& enumDef);
	static int GetCountTotalBars();

protected:
	static void Init();
	static void Draw();
	static void Terminate();
	static void CallbackWindowSize(vec2 windowSize);
	static void CallbackKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods);
	static void CallbackTextEvent(GLFWwindow* window, unsigned int codePoint);
	static void CallbackMouseMoveEvent(GLFWwindow* window, double xPos, double yPos);
	static void CallbackMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	static void CallbackMouseScrollEvent(GLFWwindow* window, double xOffset, double yOffset);

private:
	void* mTwBar;
	string mBarName;
};
}
