#ifndef EDIT_STRING
#define EDIT_STRING

#include <exception>
#include <vector>
#include <memory>

#include <strsafe.h>

#include "UnicodeOrAnsii.h"
#include "preview_box.h"

#define SPACE 4

#define ID_EDITBOX		17
#define ID_BUTTON_OK	10
#define ID_BUTTON_NO	11
#define ID_LABEL		-1

#define TB_WIDTH		382
#define TB_HEIGHT		128
#define TB_PREVIEW_Y	TB_HEIGHT + 2*SPACE

#define BT_WIDTH		70
#define BT_HEIGHT		20
#define BT_Y			2*TB_HEIGHT + 3*SPACE
#define BT_X1			242
#define BT_X2			BT_X1 + SPACE + BT_WIDTH

#define WND_WIDTH		396
#define WND_HEIGHT		320

struct EditString
{
	struct MessageParams
	{
		UINT message;
		WPARAM wParam;
		LPARAM lParam;

		MessageParams(UINT msg, WPARAM wP, LPARAM lP);
	};

	static std::vector<TString> Titles;
	static EditString* editor;
	static LRESULT APIENTRY WndProc(HWND wnd, UINT msg, WPARAM wP, LPARAM lP);
	
	PreviewBox* colorizer;
	
	HWND window;
	HWND textbox;
	HWND fakebox;
	HWND preview;
	HWND label;
	HWND button_ok;
	HWND button_no;
	HWND counter;

	int maxChars;

	WNDPROC oldProc;

	bool multiline;

	EditString(HWND hwnd);
	~EditString();
	
	LRESULT Procedure(MessageParams p);
	void SetupControls();
	void SetupLayout();
};
#endif