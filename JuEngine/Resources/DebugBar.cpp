// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "DebugBar.hpp"
#include "DebugLog.hpp"
#include <unordered_map>
#include <sstream>
#include <AntTweakBar.h>

namespace JuEngine
{
std::unordered_map<string, int> mEnumDefines;

void ErrorCallbackATB(const char* message)
{
	JuEngine::DebugLog::Write("AntTweakBar Error: %s", message);
}

void CopyStdStringToClient(string& destinationClientString, const string& sourceLibraryString)
{
	destinationClientString = sourceLibraryString;
}

TwType ConvertTypeToTw(VarType type)
{
	TwType twType = TW_TYPE_UNDEF;

	switch(type)
	{
		case VarType::BOOL:
			twType = TW_TYPE_BOOLCPP; break;
		case VarType::CHAR:
			twType = TW_TYPE_CHAR; break;
		case VarType::STRING:
			twType = TW_TYPE_STDSTRING; break;
		case VarType::INT8:
			twType = TW_TYPE_INT8; break;
		case VarType::INT16:
			twType = TW_TYPE_INT16; break;
		case VarType::INT32:
			twType = TW_TYPE_INT32; break;
		case VarType::UINT8:
			twType = TW_TYPE_UINT8; break;
		case VarType::UINT16:
			twType = TW_TYPE_UINT16; break;
		case VarType::UINT32:
			twType = TW_TYPE_UINT32; break;
		case VarType::FLOAT:
			twType = TW_TYPE_FLOAT; break;
		case VarType::DOUBLE:
			twType = TW_TYPE_DOUBLE; break;
		case VarType::COLOR32:
			twType = TW_TYPE_COLOR32; break;
		case VarType::COLOR3:
			twType = TW_TYPE_COLOR3F; break;
		case VarType::COLOR4:
			twType = TW_TYPE_COLOR4F; break;
		case VarType::QUAT4:
			twType = TW_TYPE_QUAT4F; break;
		case VarType::DIR3:
			twType = TW_TYPE_DIR3F; break;
	}

	return twType;
}

DebugBar::DebugBar(const string& name) : IObject("debugBar")
{
	mBarName = name;
	mTwBar = TwNewBar(mBarName.c_str());
}

DebugBar::~DebugBar()
{
	TwDeleteBar((TwBar*)mTwBar);
}

auto DebugBar::GetBarName() -> const string&
{
	// const char * TwGetBarName(mTwBar);

	return mBarName;
}

void DebugBar::ForceRefresh()
{
	TwRefreshBar((TwBar*)mTwBar); // Force refresh
}

void DebugBar::Show()
{
	SetParams("visible=true");
}

void DebugBar::Hide()
{
	SetParams("visible=false");
}

void DebugBar::SetIconified(bool iconified)
{
	SetParams("iconified=" + (iconified ? string("true") : string("false")));
}

void DebugBar::BringToFront()
{
	TwSetTopBar((TwBar*)mTwBar);
}

void DebugBar::SendToBack()
{
	TwSetBottomBar((TwBar*)mTwBar);
}

void DebugBar::SetParams(const string& params)
{
	string temp(mBarName + " " + params);

	TwDefine(temp.c_str());
}

void DebugBar::SetRefreshRate(float seconds)
{
	std::stringstream ss;
	ss << seconds;

	SetParams("refresh=" + ss.str());
}

void DebugBar::SetSize(vec2 size)
{
	std::stringstream ss;
	ss << (int)size.x << " " << (int)size.y;

	SetParams("size='" + ss.str() + "'");
}

void DebugBar::SetWidthVar(int size)
{
	std::stringstream ss;
	if(size <= 0.f) ss << "fit";
	else ss << (size > 255.f ? 255 : (unsigned int)size);

	SetParams("valueswidth=" + ss.str());
}

void DebugBar::SetPosition(vec2 position)
{
	std::stringstream ss;
	ss << (int)position.x << " " << (int)position.y;

	SetParams("position='" + ss.str() + "'");
}

void DebugBar::SetLabel(const string& label)
{
	SetParams("label=`" + label + "`");
}

void DebugBar::SetHelp(const string& help)
{
	SetParams("help=`" + help + "`");
}

void DebugBar::SetColor(vec3 color)
{
	std::stringstream ss;

	ss << (color.x <= 0.f ? 0 : (color.x > 255.f ? 255 : (unsigned int)color.x));
	ss << " ";
	ss << (color.y <= 0.f ? 0 : (color.y > 255.f ? 255 : (unsigned int)color.y));
	ss << " ";
	ss << (color.z <= 0.f ? 0 : (color.z > 255.f ? 255 : (unsigned int)color.z));

	SetParams("color='" + ss.str() + "'");
}

void DebugBar::SetAlpha(int alpha)
{
	std::stringstream ss;
	ss << (alpha <= 0.f ? 0 : (alpha > 255.f ? 255 : (unsigned int)alpha));

	SetParams("alpha=" + ss.str());
}

void DebugBar::SetMovable(bool movable)
{
	SetParams("movable=" + (movable ? string("true") : string("false")));
}

void DebugBar::SetResizable(bool resizable)
{
	SetParams("resizable=" + (resizable ? string("true") : string("false")));
}

void DebugBar::SetIconifiable(bool iconifiable)
{
	SetParams("iconifiable=" + (iconifiable ? string("true") : string("false")));
}

void DebugBar::SetAlwaysTop(bool alwaysTop)
{
	SetParams("alwaystop=" + (alwaysTop ? string("true") : string("false")));
}

void DebugBar::SetAlwaysBottom(bool alwaysBottom)
{
	SetParams("alwaysbottom=" + (alwaysBottom ? string("true") : string("false")));
}

void DebugBar::AddSeparator(const string& name, const string& params)
{
	TwAddSeparator((TwBar*)mTwBar, name.c_str(), params.c_str());
}

void DebugBar::AddButton(const string& name, DebugBarButtonCallback callback, void* clientData, const string& params)
{
	TwAddButton((TwBar*)mTwBar, name.c_str(), callback, clientData, params.c_str());
}

void DebugBar::AddVarReadOnly(const string& name, VarType type, const void *var, const string& params)
{
	TwAddVarRO((TwBar*)mTwBar, name.c_str(), ConvertTypeToTw(type), var, params.c_str());
}

void DebugBar::AddVarReadWrite(const string& name, VarType type, void *var, const string& params)
{
	TwAddVarRW((TwBar*)mTwBar, name.c_str(), ConvertTypeToTw(type), var, params.c_str());
}

void DebugBar::AddVarCallback(const string& name, VarType type, DebugBarSetCallback setCallback, DebugBarGetCallback getCallback, void* clientData, const string& params)
{
	TwAddVarCB((TwBar*)mTwBar, name.c_str(), ConvertTypeToTw(type), setCallback, getCallback, clientData, params.c_str());
}

void DebugBar::AddVarReadOnly(const string& name, const string& enumName, const void *var, const string& params)
{
	TwAddVarRO((TwBar*)mTwBar, name.c_str(), (TwType)mEnumDefines[enumName], var, params.c_str());
}

void DebugBar::AddVarReadWrite(const string& name, const string& enumName, void *var, const string& params)
{
	TwAddVarRW((TwBar*)mTwBar, name.c_str(), (TwType)mEnumDefines[enumName], var, params.c_str());
}

void DebugBar::AddVarCallback(const string& name, const string& enumName, DebugBarSetCallback setCallback, DebugBarGetCallback getCallback, void* clientData, const string& params)
{
	TwAddVarCB((TwBar*)mTwBar, name.c_str(), (TwType)mEnumDefines[enumName], setCallback, getCallback, clientData, params.c_str());
}

void DebugBar::SetGroupOpened(const string& name, bool opened)
{
	SetVarParams(name, "opened=" + (opened ? string("true") : string("false")));
}

void DebugBar::SetVarParams(const string& name, const string& params)
{
	string temp(mBarName + "/" + name + " " + params);

	TwDefine(temp.c_str());
}

void DebugBar::SetVarGroup(const string& name, const string& group)
{
	SetVarParams(name, "group=`" + group + "`");
}

void DebugBar::SetVarLabel(const string& name, const string& label)
{
	SetVarParams(name, "label=`" + label + "`");
}

void DebugBar::SetVarHelp(const string& name, const string& help)
{
	SetVarParams(name, "help=`" + help + "`");
}

void DebugBar::SetVarVisible(const string& name, bool visible)
{
	SetVarParams(name, "visible=" + (visible ? string("true") : string("false")));
}

void DebugBar::SetVarReadOnly(const string& name, bool readOnly)
{
	SetVarParams(name, "readonly=" + (readOnly ? string("true") : string("false")));
}

void DebugBar::SetVarKey(const string& name, const string& key)
{
	// NOTE: Use the key events message displayed at the bottom of the help bar
	SetVarParams(name, "key=" + key);
}

void DebugBar::SetVarKeys(const string& name, const string& keyIncr, const string& keyDecr)
{
	SetVarParams(name, "keyincr=" + keyIncr);
	SetVarParams(name, "keydecr=" + keyDecr);
}

void DebugBar::SetVarNumClamp(const string& name, int min, int max)
{
	std::stringstream ss_min, ss_max;
	ss_min << min;
	ss_max << max;

	SetVarParams(name, "min=" + ss_min.str());
	SetVarParams(name, "min=" + ss_max.str());
}

void DebugBar::SetVarNumStep(const string& name, float step)
{
	std::stringstream ss;
	ss << step;

	SetVarParams(name, "step=" + ss.str());
}

void DebugBar::SetVarNumPrecision(const string& name, int precision)
{
	std::stringstream ss;
	ss << (precision < 0 ? -1 : (precision > 12 ? 12 : precision));

	SetVarParams(name, "precision=" + ss.str());
}

void DebugBar::SetVarHexadecimal(const string& name, bool hexadecimal)
{
	SetVarParams(name, "hexa=" + (hexadecimal ? string("true") : string("false")));
}

void DebugBar::SetVarBoolStrings(const string& name, const string& trueStr, const string& falseStr)
{
	SetVarParams(name, "true=`" + trueStr + "`");
	SetVarParams(name, "false=`" + falseStr + "`");
}

void DebugBar::SetVarArrowColor(const string& name, vec3 color)
{
	std::stringstream ss;

	ss << (color.x <= 0.f ? 0 : (color.x > 255.f ? 255 : (unsigned int)color.x));
	ss << " ";
	ss << (color.y <= 0.f ? 0 : (color.y > 255.f ? 255 : (unsigned int)color.y));
	ss << " ";
	ss << (color.z <= 0.f ? 0 : (color.z > 255.f ? 255 : (unsigned int)color.z));

	SetVarParams(name, "arrowcolor='" + ss.str() + "'");
}

void DebugBar::SetVarArrowShowValues(const string& name, bool showValues)
{
	SetVarParams(name, "showval=" + (showValues ? string("true") : string("false")));
}

void DebugBar::RemoveVar(const string& name)
{
	TwRemoveVar((TwBar*)mTwBar, name.c_str());
}

void DebugBar::RemoveAllVars()
{
	TwRemoveAllVars((TwBar*)mTwBar);
}

void DebugBar::SetGlobalParameters(const string& params)
{
	string temp("GLOBAL " + params);

	TwDefine(temp.c_str());
}

void DebugBar::DefineEnum(const string& name, const string& enumDef)
{
	mEnumDefines[name] = (int)TwDefineEnumFromString(name.c_str(), enumDef.c_str());
}

int DebugBar::GetCountTotalBars()
{
	return TwGetBarCount();
}

void DebugBar::Init()
{
	TwInit(TW_OPENGL_CORE, NULL);
	TwHandleErrors(ErrorCallbackATB);
	TwCopyStdStringToClientFunc(CopyStdStringToClient);

	TwWindowSize(1280, 720);
	TwDefine("GLOBAL help='JuEngine \nCompiled on " __DATE__ " at " __TIME__ "\nby JuDelCo'");
	TwDefine("GLOBAL fontSize=3 fontresizable=false"); // contained=true
	TwDefine("TW_HELP label='Help'");
}

void DebugBar::Draw()
{
	TwDraw();
}

void DebugBar::Terminate()
{
	TwTerminate();
}

void DebugBar::CallbackWindowSize(vec2 windowSize)
{
	TwWindowSize(windowSize.x, windowSize.y);
}

void DebugBar::CallbackKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	TwEventKeyGLFW(key, action);
}

void DebugBar::CallbackTextEvent(GLFWwindow* window, unsigned int codePoint)
{
	#define GLFW_PRESS 1
	TwEventCharGLFW(codePoint, GLFW_PRESS);
}

void DebugBar::CallbackMouseMoveEvent(GLFWwindow* window, double xPos, double yPos)
{
	#ifdef _WIN32
		TwEventMousePosGLFW((int)window, xPos, yPos);
	#else
		TwEventMousePosGLFW(xPos, yPos);
	#endif
}

void DebugBar::CallbackMouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	#ifdef _WIN32
		TwEventMouseButtonGLFW(window, button, action, mods);
	#else
		TwEventMouseButtonGLFW(button, action);
	#endif
}

void DebugBar::CallbackMouseScrollEvent(GLFWwindow* window, double xOffset, double yOffset)
{
	#ifdef _WIN32
		TwEventMouseWheelGLFW((int)window, xOffset, yOffset);
	#else
		TwEventMouseWheelGLFW(yOffset);
	#endif
}
}
