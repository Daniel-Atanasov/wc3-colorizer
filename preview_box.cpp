#include "preview_box.h"

PreviewBox::PreviewBox(HWND hwnd, HWND fake, HWND preview, HWND original, bool ml)
	:original_edit(original),
	preview_edit(preview), 
	fake_edit(fake),
	parent(hwnd),
	multiline(ml)
{
	SetWindowLong(this->fake_edit, GWL_EXSTYLE, 0);
	SetWindowLong(this->preview_edit, GWL_EXSTYLE, 0);

	SendMessage(this->preview_edit, EM_SETBKGNDCOLOR, (WPARAM)0, (LPARAM)RGB(31, 21, 59));

	HFONT hFont = (HFONT)SendMessage(this->original_edit, WM_GETFONT, NULL, NULL);
	SendMessage(this->fake_edit, WM_SETFONT, (WPARAM)hFont, 0);
	SendMessage(this->preview_edit, WM_SETFONT, (WPARAM)hFont, 0);
	DeleteObject(hFont);

	int limit = SendMessage(this->original_edit, EM_GETLIMITTEXT, 0, 0);
	SendMessage(this->fake_edit, EM_SETLIMITTEXT, (WPARAM)limit, 0);
	SendMessage(this->preview_edit, EM_SETLIMITTEXT, (WPARAM)limit, 0);

	int size = GetWindowTextLength(this->original_edit) + 1;
	TChar* buffer = new TChar[size];
	SendMessage(this->original_edit, WM_GETTEXT, (WPARAM)size, (LPARAM)buffer);
	SendMessage(this->fake_edit, WM_SETTEXT, NULL, (LPARAM)buffer);
	delete [] buffer;
	SetWindowPos(this->fake_edit,
		HWND_TOP,
		0, 0,
		0, 0,
		SWP_NOSIZE | SWP_NOMOVE);

	SetWindowPos(this->preview_edit,
		HWND_TOP,
		0, 0,
		0, 0,
		SWP_NOSIZE | SWP_NOMOVE);
	this->Update();
}

void PreviewBox::Update() {
	int size = GetWindowTextLength(this->fake_edit) + 1;
	TChar* buffer = new TChar[size];
	SendMessage(this->fake_edit, WM_GETTEXT, (WPARAM)size, (LPARAM)buffer);
	TString new_string(buffer);
	delete [] buffer;

	new_string.erase(std::remove(new_string.begin(), new_string.end(), '\r'), new_string.end());
	SendMessage(this->preview_edit, WM_SETTEXT, (WPARAM)0, '\0');
	SendMessage(this->original_edit, WM_SETTEXT, NULL, (LPARAM)new_string.c_str());

	COLORREF color = DEFAULT_COLOR;
	int i = 0;
	for  (int j = 0; j < new_string.size(); ++j) {
		TCHAR c = new_string[j];
		if (c == '|')
			switch (std::tolower(new_string[j + 1])) {
			case 'n':
				new_string.replace(j, 2, STR("\n"));
				break;
			case 'r':
				this->Append(new_string.substr(i, j - i), color);
				color = RGB(255, 255, 255);
				j += 1;
				i = j + 1;
				break;
			case 'c':
				if (new_string.length() > j + 9) {
					TString hex = new_string.substr(j + 2, 8);
					if (hex.find_last_not_of(STR("0123456789abcdefABCDEF"), 0) == TString::npos) {
						this->Append(new_string.substr(i, j - i), color);
						int R, G, B;

						IStringStream(hex.substr(2, 2)) >> std::hex >> R;
						IStringStream(hex.substr(4, 2)) >> std::hex >> G;
						IStringStream(hex.substr(6, 2)) >> std::hex >> B;
						color = RGB(R, G, B);
						j += 9;

						i = j + 1;
					}
				}
				break;
		}
	}

	this->Append(new_string.substr(i, new_string.length() - i), color);
}

void PreviewBox::Append(TString str, COLORREF new_color) {
	//Move cursor to the very end
	SendMessage(this->preview_edit, EM_SETSEL, 0, -1);
	SendMessage(this->preview_edit, EM_SETSEL, -1, -1);

	CHARRANGE cr;
	//WM_GETTEXTLENGTH returns an incorrect length
	//This returns the current selection position
	//in Characters
	SendMessage(this->preview_edit, EM_EXGETSEL , (WPARAM)0, (LPARAM)&cr);
	//Replace the 'selection' with the string
	SendMessage(this->preview_edit, EM_REPLACESEL, 0, (LPARAM)str.c_str());
	//Select the new text
	SendMessage(this->preview_edit, EM_SETSEL, (WPARAM)cr.cpMin, (LPARAM)cr.cpMin + str.length());
	CHARFORMAT cf;
	cf.cbSize = sizeof(cf);
	//Get the current CharFORMAT - only color needs to be changed
	SendMessage(this->preview_edit, EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	//Fill in the details
	cf.dwEffects   = NULL;
	cf.crTextColor = new_color;
	cf.dwMask      = CFM_COLOR;
	//Set it to the new one
	SendMessage(this->preview_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}