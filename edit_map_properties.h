#ifndef EDIT_DESC
#define EDIT_DESC

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

#include "UnicodeOrAnsii.h"

#define EXTRA_SIZE 38

class EditProperties {
	HWND buttonOk;
	HWND buttonNo;
	HWND buttonDefault;
	HWND separator;

	class TabDescription {
		HWND editName;
		HWND staticName;
		
		HWND editPlayers;
		HWND staticPlayers;
		
		HWND editDescription;
		HWND staticDescription;
		
		HWND editAuthor;
		HWND staticAuthor;
		
		HWND staticExpansion1;
		HWND staticExpansion2;
		
		HWND editVersion;
		HWND staticVersion;
		
		HWND editVersionWE;
		HWND staticVersionWE;

		HWND editReasons;


	public:
		EditProperties& parent;
		TabDescription(EditProperties& parent);
	};
public:
	static EditProperties* editor;

	HWND window;

	TabDescription description;

	EditProperties(HWND hwnd);
	~EditProperties();

	LRESULT WndProc(HHOOK hook, int nCode, WPARAM wParam, LPARAM lParam);
	bool FindControls();
};

#endif