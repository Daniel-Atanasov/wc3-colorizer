#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <Windows.h>
#include <Richedit.h>

#include "UnicodeOrAnsii.h"

#define DEFAULT_COLOR RGB(255, 255, 255)

#define FLAGS (WS_BORDER | WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL | ES_AUTOVSCROLL | ES_DISABLENOSCROLL)
#define FLAGS_RO FLAGS | ES_READONLY

struct PreviewBox {
	HWND original_edit;
	HWND preview_edit;
	HWND fake_edit;
	HWND parent;

	bool multiline;

	PreviewBox(HWND hwnd, HWND edit, HWND preview, HWND original, bool ml);
	void Update();
	void Append(TString str, COLORREF new_color);
};