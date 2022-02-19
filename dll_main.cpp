#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <richedit.h>
#include <map>

#include "UnicodeOrAnsii.h"
#include "edit_string.h"
//#include "edit_map_properties.h"

DWORD ID_Proc;
DWORD ID_MainThread;

HINSTANCE DLL_Handle;

HHOOK WE_Hook;

LRESULT CALLBACK WndProc(int nCode, WPARAM wParam, LPARAM lParam);
void InstallHook();
bool GetConf();

extern "C" __declspec(dllexport) BOOL APIENTRY DllMain(
	IN HINSTANCE hDllHandle, 
	IN DWORD     nReason, 
	IN LPVOID    Reserved)
{
	switch (nReason)
	{
	case DLL_PROCESS_ATTACH:

		ID_Proc = GetCurrentProcessId();
		DLL_Handle = hDllHandle;
		ID_MainThread = GetCurrentThreadId();

		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InstallHook, (void*)NULL, 0, NULL);

		break;
	case DLL_PROCESS_DETACH:
		UnhookWindowsHookEx(WE_Hook);
		break;
	}
	return true;
}

void InstallHook()
{
	if (!GetConf())
		return;

	LoadLibrary(STR("Msftedit.dll"));

	MSG msg;
	WE_Hook = SetWindowsHookEx(WH_CALLWNDPROC, (HOOKPROC)WndProc, DLL_Handle, ID_MainThread);

	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{ 
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}
}


LRESULT CALLBACK WndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		LPCWPSTRUCT lp = (LPCWPSTRUCT)lParam;

		if (lp->message == WM_SHOWWINDOW && lp->wParam == TRUE)
		{
			int size = GetWindowTextLength(lp->hwnd) + 1;
			TChar* buffer = new TChar[size];
			SendMessage(lp->hwnd, WM_GETTEXT, (WPARAM)size, (LPARAM)buffer);
			TString wnd_text(buffer);
			delete [] buffer;

			if (std::find(EditString::Titles.begin(), EditString::Titles.end(), wnd_text) != EditString::Titles.end())
				EditString::editor = new EditString(lp->hwnd);
		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool GetConf()
{
	IFStream conf_file(STR("we extension\\we_extension.conf"));

	if (conf_file.is_open())
	{
		TString s;
		while (std::getline(conf_file, s))
			if (s.length() > 9 && s.substr(0, 9) == STR("strings+="))
				EditString::Titles.push_back(s.substr(9, s.length() - 9));
		conf_file.close();

		EditString::Titles.erase(std::unique(EditString::Titles.begin(), EditString::Titles.end()), EditString::Titles.end());
		MessageBox(NULL, STR("test"), STR("TEST"), MB_OK);
		return true;
	}

	return false;
}