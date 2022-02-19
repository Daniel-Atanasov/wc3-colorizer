#include "edit_string.h"

std::vector<TString> EditString::Titles;
EditString* EditString::editor;

EditString::MessageParams::MessageParams(UINT msg, WPARAM wP, LPARAM lP)
	: message(msg),
	wParam(wP),
	lParam(lP)
{ }

LRESULT APIENTRY EditString::WndProc(HWND wnd, UINT msg, WPARAM wP, LPARAM lP)
{
	EditString* str_editor = reinterpret_cast<EditString*>(GetWindowLong(wnd, GWL_USERDATA));
	return str_editor->Procedure(MessageParams(msg, wP, lP));
}

EditString::EditString(HWND wnd)
	: window(wnd)
{
	SetupControls();
	SetupLayout();
}

LRESULT EditString::Procedure(MessageParams params)
{
	switch (params.message)
	{
	case WM_COMMAND:
		if (HIWORD(params.wParam) == EN_CHANGE){
			
			if ((HWND)params.lParam == this->fakebox)
			{
				colorizer->Update();

				TString s = ToString(SendMessage(textbox, WM_GETTEXTLENGTH, 0, 0)) + STR("/") + ToString(maxChars);
				SendMessage(counter, WM_SETTEXT, (WPARAM)s.size(), (LPARAM)s.c_str());

			}}
			break;
	case WM_CTLCOLORSTATIC:
		// Set the colour of the text for our URL
		if ((HWND)params.lParam == counter)
		{
			SetBkMode((HDC)params.wParam,TRANSPARENT);
			if (SendMessage(textbox, WM_GETTEXTLENGTH, 0, 0) == maxChars)
				SetTextColor((HDC)params.wParam, RGB(255,0,0));
			return (BOOL)GetSysColorBrush(COLOR_MENU);
		}
		break;
	}

	return CallWindowProc(oldProc, window, params.message, params.wParam, params.lParam);
}

void EditString::SetupControls()
{
	textbox = GetDlgItem(this->window, ID_EDITBOX);
	button_ok = GetDlgItem(this->window, ID_BUTTON_OK);
	button_no = GetDlgItem(this->window, ID_BUTTON_NO);
	label = GetDlgItem(this->window, ID_LABEL);

	maxChars = SendMessage(textbox, EM_GETLIMITTEXT, 0, 0);

	fakebox = CreateWindowEx(0, STR("RICHEDIT50W"), STR(""), FLAGS, SPACE, SPACE, TB_WIDTH, TB_HEIGHT, this->window, NULL, NULL, NULL);
	preview = CreateWindowEx(0, STR("RICHEDIT50W"), STR(""), FLAGS_RO, SPACE, TB_PREVIEW_Y, TB_WIDTH, TB_HEIGHT, this->window, NULL, NULL, NULL);
	counter = CreateWindowEx(0, STR("STATIC"), (ToString(SendMessage(textbox, WM_GETTEXTLENGTH, 0, 0)) + STR("/") + ToString(maxChars)).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER, SPACE, BT_Y, BT_WIDTH, BT_HEIGHT, this->window, NULL, NULL, NULL);

	SendMessage (fakebox, EM_SETEVENTMASK, 0, ENM_CHANGE);

	SetWindowLongPtr(this->window, GWL_USERDATA, reinterpret_cast<LONG_PTR>(this));
	SetWindowLongPtr(this->textbox, GWL_USERDATA, reinterpret_cast<LONG_PTR>(this));
	SetWindowLongPtr(this->fakebox, GWL_USERDATA, reinterpret_cast<LONG_PTR>(this));
	SetWindowLongPtr(this->preview, GWL_USERDATA, reinterpret_cast<LONG_PTR>(this));
	SetWindowLongPtr(this->button_ok, GWL_USERDATA, reinterpret_cast<LONG_PTR>(this));
	SetWindowLongPtr(this->button_no, GWL_USERDATA, reinterpret_cast<LONG_PTR>(this));
	SetWindowLongPtr(this->label, GWL_USERDATA, reinterpret_cast<LONG_PTR>(this));


	oldProc = (WNDPROC)SetWindowLongPtr(this->window, GWL_WNDPROC, (LONG_PTR)EditString::WndProc);

	colorizer = new PreviewBox(window, fakebox, preview, textbox, true);
}

void EditString::SetupLayout()
{
	SetWindowPos(window, HWND_TOP, 0, 0, WND_WIDTH, WND_HEIGHT, SWP_NOMOVE);
	SetWindowPos(button_ok, HWND_TOP, BT_X1, BT_Y, 0, 0, SWP_NOSIZE);
	SetWindowPos(button_no, HWND_TOP, BT_X2, BT_Y, 0, 0, SWP_NOSIZE);

	ShowWindow(label, SW_HIDE);
	ShowWindow(textbox, SW_HIDE);
}