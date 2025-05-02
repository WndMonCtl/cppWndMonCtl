#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include <windows.h>

#endif // SYSTEM_H

#ifndef TSTRING_H
#define TSTRING_H

#include <string>

#if defined(UNICODE) || defined(_UNICODE)
using wmcString = std::wstring;
#define _tcout std::wcout
#define _tcin  std::wcin
#define wmcUnicode;
#else
using wmcString = std::string;
#define _tcout std::cout
#define _tcin  std::cin
#define wmcMBCS
#endif // UNICODE || _UNICODE

#include <tchar.h>
#include <iostream>

#endif // TSTRING_H

#ifndef STL_H
#define STL_H

#include<vector>

#endif // STL_H

namespace
{
	////////////****** 窗体 ******////////////
	class WindowStrong
	{
	public:
		WindowStrong() {}
		WindowStrong(HWND hwndTarget)
		{
			hwnd = hwndTarget;
		}

	public:
		bool IsValid()
		{
			return hwnd != nullptr;
		}

	public:
		HWND hwnd;
	};

	////////////****** 窗体信息 ******////////////

	class WindowInfo
	{
	public:
		WindowInfo() : hwnd(nullptr),
			title(_T("")),
			className(_T("")),
			style(0),
			exStyle(0),
			rect{ 0, 0, 0, 0 },
			width(0),
			height(0),
			isVisible(false)
		{
		}
		~WindowInfo() {}

	public:
		HWND hwnd;
		wmcString title;
		wmcString className;
		LONG_PTR style;
		LONG_PTR exStyle;
		RECT rect;
		int width;
		int height;

		bool isVisible;
	};
	class WindowInfoExtra : public WindowInfo
	{
	public:
		WindowInfoExtra() : isEnabled(false),
			isIconic(false),
			isZoomed(false),
			instance(nullptr)
		{
		}

	public:
		bool isEnabled;
		bool isIconic;
		bool isZoomed;

		HINSTANCE instance;
	};

	bool priGetWindowInfoExtra(WindowInfo& windowInfo, HWND hwnd)
	{
		windowInfo.hwnd = hwnd;

		// 1. 获取窗口标题 (Window Text)
#ifdef wmcUnicode
		int titleLen = GetWindowTextLengthW(hwnd);
		windowInfo.title.resize(titleLen);
		GetWindowTextW(hwnd, &windowInfo.title[0], titleLen + 1);
#else
		int titleLen = GetWindowTextLengthA(hwnd);
		windowInfo.title.resize(titleLen);
		GetWindowTextA(hwnd, &windowInfo.title[0], titleLen + 1);
#endif

		// 2. 获取窗口类名
#ifdef wmcUnicode
		{
			std::vector<wchar_t> classNameBuf;
			size_t currentBufSize = 256;
			int classNameLen = 0;
			while (true)
			{
				classNameBuf.resize(currentBufSize);

				classNameLen = GetClassNameW(hwnd, classNameBuf.data(), static_cast<int>(classNameBuf.size()));
				if (classNameLen == 0)
				{
					windowInfo.className = L"";
					break;
				}
				if (static_cast<size_t>(classNameLen) < classNameBuf.size() - 1)
				{
					windowInfo.className.assign(classNameBuf.data(), classNameLen);
					break;
				}

				if (currentBufSize >= 32768)
				{
					windowInfo.className.assign(classNameBuf.data(), classNameLen);
					break;
				}
				currentBufSize *= 2;
				if (currentBufSize > 32768) currentBufSize = 32768;
			}
		}
#else
		{
			std::vector<char> classNameBuf;
			size_t currentBufSize = 256;
			int classNameLen = 0;
			while (true)
			{
				classNameBuf.resize(currentBufSize);

				classNameLen = GetClassNameA(hwnd, classNameBuf.data(), static_cast<int>(classNameBuf.size()));
				if (classNameLen == 0)
				{
					windowInfo.className = "";
					break;
				}
				if (static_cast<size_t>(classNameLen) < classNameBuf.size() - 1)
				{
					windowInfo.className.assign(classNameBuf.data(), classNameLen);
					break;
				}

				if (currentBufSize >= 32768)
				{
					windowInfo.className.assign(classNameBuf.data(), classNameLen);
					break;
				}
				currentBufSize *= 2;
				if (currentBufSize > 32768) currentBufSize = 32768;
			}
		}
#endif

		// 3. 获取标准窗口样式
		windowInfo.style = GetWindowLongPtrW(hwnd, GWL_STYLE);

		// 4. 获取扩展窗口样式
		windowInfo.exStyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);

		// 5. 获取窗口 RECT (屏幕坐标)
		GetWindowRect(hwnd, &windowInfo.rect);
		windowInfo.width = windowInfo.rect.right - windowInfo.rect.left;
		windowInfo.height = windowInfo.rect.bottom - windowInfo.rect.top;

		// 6. 获取窗口显示状态
		windowInfo.isVisible = IsWindowVisible(hwnd);
	}
}
namespace WndMC
{
	////////////****** 窗体 ******////////////
	extern class WindowStrong;

	////////////****** 窗体信息 ******////////////

	extern class WindowInfo;
	//extern class WindowInfoExtra;

	class Snapshot
	{
	};
}