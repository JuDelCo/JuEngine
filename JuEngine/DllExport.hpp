// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#ifdef _WIN32
	#define DLLEXPORT __declspec(dllexport)
	#define DLLIMPORT __declspec(dllimport)
#elif __GNUC__
	#define DLLEXPORT __attribute__((visibility("default")))
	#define DLLIMPORT
#endif

#ifdef JUENGINE_COMPILE_DLL
	#define JUENGINEAPI DLLEXPORT
#else
	#define JUENGINEAPI DLLIMPORT
#endif
