#include "edit_map_properties.h"

EditProperties* EditProperties::editor;

EditProperties::EditProperties(HWND hwnd):
	window(hwnd),
	buttonOk(GetDlgItem(this->window, 10)),
	buttonNo(GetDlgItem(this->window, 11)),
	buttonDefault(GetDlgItem(this->window, 13)),
	description(*this) {

		RECT r_window;

		GetWindowRect(this->buttonOk, &r_window);
		MessageBox(NULL, ToString(r_window.bottom - r_window.top).c_str(), "", MB_OK);



		SetWindowLongPtr(this->window, GWL_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

LRESULT EditProperties::WndProc(HHOOK hook, int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		LPCWPSTRUCT lp = (LPCWPSTRUCT)lParam;

	}

	return CallNextHookEx(hook, nCode, wParam, lParam);
}

EditProperties::TabDescription::TabDescription(EditProperties& parent): 
	parent(parent),
	editName(GetDlgItem(parent.window, 14)),
	staticName(GetNextWindow(editName, GW_HWNDPREV)),

	editPlayers(GetDlgItem(parent.window, 15)),
	staticPlayers(GetNextWindow(editPlayers, GW_HWNDPREV)),

	editDescription(GetDlgItem(parent.window, 16)),
	staticDescription(GetNextWindow(editDescription, GW_HWNDPREV)),

	editAuthor(GetDlgItem(parent.window, 17)),
	staticAuthor(GetNextWindow(editAuthor, GW_HWNDPREV)),

	staticExpansion1(GetNextWindow(editAuthor, GW_HWNDNEXT)),
	staticExpansion2(GetNextWindow(staticExpansion1, GW_HWNDNEXT)),

	editReasons(GetDlgItem(parent.window, 18)),

	editVersion(GetNextWindow(editReasons, GW_HWNDNEXT)),
	staticVersion(GetNextWindow(editVersion, GW_HWNDNEXT)),

	editVersionWE(GetNextWindow(staticVersion, GW_HWNDNEXT)),
	staticVersionWE(GetNextWindow(editVersionWE, GW_HWNDNEXT)) {

		RECT r_edesc;

		RECT r_eauthor;
		RECT r_sauthor;

		RECT r_exp1;
		RECT r_exp2;

		RECT r_reasons;

		RECT r_eversion;
		RECT r_sversion;

		RECT r_eversionWE;
		RECT r_sversionWE;

		GetClientRect(editDescription, &r_edesc);
		GetClientRect(staticAuthor, &r_sauthor);
		GetClientRect(editAuthor, &r_eauthor);
		GetClientRect(staticExpansion1, &r_exp1);
		GetClientRect(editReasons, &r_reasons);
		GetClientRect(editVersion, &r_eversion);

		MessageBox(NULL, ToString(r_edesc.top).c_str(), "", MB_OK);
		MessageBox(NULL, ToString(r_sauthor.bottom - r_sauthor.top).c_str(), "", MB_OK);
		MessageBox(NULL, ToString(r_eauthor.bottom - r_eauthor.top).c_str(), "", MB_OK);
		MessageBox(NULL, ToString(r_exp1.bottom - r_exp1.top).c_str(), "", MB_OK);
		MessageBox(NULL, ToString(r_reasons.bottom - r_reasons.top).c_str(), "", MB_OK);
		MessageBox(NULL, ToString(r_eversion.bottom - r_eversion.top).c_str(), "", MB_OK);
}