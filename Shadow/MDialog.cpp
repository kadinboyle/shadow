#define DLGTITLE  L"Debug"
#define DLGFONT   L"MS Sans Serif"
#define DLGAPPLY  L"&Apply"
#define DLGCANCEL L"&Cancel"
#define NUMCHARS(aa) (sizeof(aa)/sizeof((aa)[0]))
#define IDC_BITMAP 99

#include "MDialog.h"

#pragma pack(push, 4)                 
static struct { // dltt 

	DWORD  style;
	DWORD  dwExtendedStyle;
	WORD   ccontrols;
	short  x;
	short  y;
	short  cx;
	short  cy;
	WORD   menu;         // name or ordinal of a menu resource
	WORD   windowClass;  // name or ordinal of a window class
	WCHAR  wszTitle[NUMCHARS(DLGTITLE)]; // title string of the dialog box
	short  pointsize;       // only if DS_SETFONT flag is set
	WCHAR  wszFont[NUMCHARS(DLGFONT)];   // typeface name, if DS_SETFONT is set

	// control info
	//
	struct {
		DWORD  style;
		DWORD  exStyle;
		short  x;
		short  y;
		short  cx;
		short  cy;
		WORD   id;
		WORD   sysClass;       // 0xFFFF identifies a system window class
		WORD   idClass;        // ordinal of a system window class
		WCHAR  wszTitle[NUMCHARS(DLGAPPLY)];
		WORD   cbCreationData; // bytes of following creation data
 //       WORD   wAlign;         // align next control to DWORD boundry.
	} apply;

	struct {
		DWORD  style;
		DWORD  exStyle;
		short  x;
		short  y;
		short  cx;
		short  cy;
		WORD   id;
		WORD   sysClass;       // 0xFFFF identifies a system window class
		WORD   idClass;        // ordinal of a system window class
		WCHAR  wszTitle[NUMCHARS(DLGCANCEL)];
		WORD   cbCreationData; // bytes of following creation data
	} cancel;

	struct {
		DWORD  style;
		DWORD  exStyle;
		short  x;
		short  y;
		short  cx;
		short  cy;
		WORD   id;
		WORD   sysClass;       // 0xFFFF identifies a system window class
		WORD   idClass;        // ordinal of a system window class
		WCHAR  wszTitle[1];    // title string or ordinal of a resource
		WORD   cbCreationData; // bytes of following creation data
	} bitmap;

} g_DebugDlgTemplate = {

WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU  // style  0x94c800c4
| DS_MODALFRAME | DS_3DLOOK
| DS_SETFONT,
0x0,        // exStyle;
3,          // ccontrols
0, 0, 300, 180,
0,                       // menu: none
0,                       // window class: none
DLGTITLE,                // Window caption
8,                       // font pointsize
DLGFONT,

   {
   WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | BS_DEFPUSHBUTTON,   // 0x50030001
   WS_EX_NOPARENTNOTIFY, // 0x4
   190,160,50,14,
   IDOK,
   0xFFFF, 0x0080, // button
   DLGAPPLY, 0,
   },

   {
   WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,    // 0x50010000
   WS_EX_NOPARENTNOTIFY, // 0x4
   244,160,50,14,
   IDCANCEL,
   0xFFFF, 0x0080, // button
   DLGCANCEL, 0,
   },

   {
   WS_CHILD | WS_VISIBLE | WS_GROUP | SS_LEFT,    // 0x50020000
   WS_EX_NOPARENTNOTIFY, // 0x4
   6,6,288,8,
   IDC_BITMAP,
   0xFFFF, 0x0082, // static
   L"", 0,
   },
};

#pragma pack(pop)

INT_PTR CALLBACK Debug_DlgProc(
	HWND   hwnd,
	UINT   uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
	}
	break;

	case WM_COMMAND:
	{
		UINT wId = LOWORD(wParam);
		if (wId == IDOK || wId == IDCANCEL)
		{
			EndDialog(hwnd, wId);
		}
	}
	break;

	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
		break;
	}

	return FALSE;
}


LRESULT DoDebugDialog(HWND hwndApp, LPVOID pvData)
{
	HINSTANCE hinst = hwndApp ? (HINSTANCE)(LONG_PTR)GetWindowLongPtr(hwndApp, GWLP_HINSTANCE)
		: (HINSTANCE)GetModuleHandle(NULL);

	return DialogBoxIndirectParamW(hinst, (LPCDLGTEMPLATEW)& g_DebugDlgTemplate, hwndApp,
		Debug_DlgProc, (LPARAM)pvData);
}