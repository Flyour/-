// KeyHook.cpp : 定义 DLL 应用程序的导出函数。

#include <stdio.h>
#include <windows.h>

// 只针对记事本程序截取键盘消息
#define  DEF_PROCESS_NAME L"notepad.exe"

HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
HWND g_hWnd = NULL;

// dllmain 是dll库加载与卸载时的默认入口函数
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		//MessageBox(NULL, TEXT("dll load"), TEXT("hello"), MB_OK);
		g_hInstance = hinstDLL; // 载入dll后返回的句柄
		break;

	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

// 钩子的回调函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//MessageBox(NULL, TEXT("once call callback proc"), TEXT("HELLO"), MB_OK);
	WCHAR szPath[MAX_PATH] = { 0, };
	WCHAR *p = NULL;

	char myt[10] = { 0, };
	sprintf_s(myt, "%d", nCode);

	//MessageBoxA(NULL, myt, "ncode", MB_OK); 

	if (nCode == 0)
	{
		//MessageBox(NULL, TEXT("first"), TEXT("HELLO"), MB_OK);

		//bit 31: 0 = key press, 1 = key release
		if (!(lParam & 0x80000000)) //按下键盘按键时
		{
			//MessageBox(NULL, TEXT("second"), TEXT("HELLO"), MB_OK);

			GetModuleFileNameW(NULL, szPath, MAX_PATH);

			p = wcsrchr(szPath, L'\\');
			//MessageBox(NULL, p, TEXT("processname"), MB_OK);

			//比较当前进程名称，若为notepad.exe ， 则消息不会传递给应用程序
			//（或下一个"钩子")
			if (!_wcsicmp(p+1, DEF_PROCESS_NAME))
			{
				MessageBox(NULL, TEXT("oh i find notepad"), TEXT("hello"), MB_OK);
				return 1;
			}
		}
	}
	//若非notepad.exe ，则调用CallNextHookEx()函数，将消息传递给应用程序（或下一个钩子)
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
	//MessageBox(NULL, TEXT("third"), TEXT("HELLO"), MB_OK);
}


#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) void HookStart()	//这里的函数调用方式是dll的导出函数特有的
	{
		g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hInstance, 0);

	}

	__declspec(dllexport) void HookStop()
	{
		if (g_hHook)
		{
			//卸载掉钩子
			UnhookWindowsHookEx(g_hHook);
			g_hHook = NULL;
		}

	}
#ifdef __cplusplus
}
#endif


