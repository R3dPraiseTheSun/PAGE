#pragma once


#ifdef WIN32

#include <Windows.h>

#endif

#ifdef BUILD_DLL
	#define PAGE_API __declspec(dllexport)
#else
	#define PAGE_API __declspec(dllimport)
#endif