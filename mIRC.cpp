#include <windows.h>
#include "LarrysmIRCIndexer.h"

typedef struct {
    DWORD  mVersion;
    HWND   mHwnd;
    BOOL   mKeep;
} LOADINFO;

bool ScriptUnloaded = false;

int __stdcall OnInitialLoad(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall OnFinalUnload(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
int __stdcall OnLoad(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause);
void __stdcall LoadDll(LOADINFO *LoadInfo);
int __stdcall UnloadDll(int Timeout);

void __stdcall LoadDll(LOADINFO *LoadInfo) {
	LoadInfo->mKeep = TRUE;						// Tell mIRC not to unload this DLL
}

int __stdcall UnloadDll(int Timeout) {
	// mIRC will automatically unload the DLL if inactive for 10 minutes. We therefore
	// need to make sure it won't unload.
	if (Timeout == 1) {
		return 0;								
    }

	// If the script is being uninstalled from mIRC, do not do a regular unload.
	if (ScriptUnloaded == true) {
		return 0;
	}

    return 1;
}

int __stdcall OnInitialLoad(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause) {
	RegisterComponentWithGDS();

	return 1;
}

int __stdcall OnFinalUnload(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause) {
	UnregisterComponentWithGDS();

	ScriptUnloaded = true;
	return 1;
}

int __stdcall OnLoad(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause) {
	return 1;
}

int __stdcall OnMessage(HWND mWnd, HWND aWnd, char *data, char *parms, BOOL show, BOOL nopause) {
	IndexMessage(data);
	return 1;
}
