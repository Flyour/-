// KeyHook.cpp : ���� DLL Ӧ�ó���ĵ���������

#include <stdio.h>
#include <windows.h>

// ֻ��Լ��±������ȡ������Ϣ
#define  DEF_PROCESS_NAME L"notepad.exe"

HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
HWND g_hWnd = NULL;

// dllmain ��dll�������ж��ʱ��Ĭ����ں���
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		//MessageBox(NULL, TEXT("dll load"), TEXT("hello"), MB_OK);
		g_hInstance = hinstDLL; // ����dll�󷵻صľ��
		break;

	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

// ���ӵĻص�����
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
		if (!(lParam & 0x80000000)) //���¼��̰���ʱ
		{
			//MessageBox(NULL, TEXT("second"), TEXT("HELLO"), MB_OK);

			GetModuleFileNameW(NULL, szPath, MAX_PATH);

			p = wcsrchr(szPath, L'\\');
			//MessageBox(NULL, p, TEXT("processname"), MB_OK);

			//�Ƚϵ�ǰ�������ƣ���Ϊnotepad.exe �� ����Ϣ���ᴫ�ݸ�Ӧ�ó���
			//������һ��"����")
			if (!_wcsicmp(p+1, DEF_PROCESS_NAME))
			{
				MessageBox(NULL, TEXT("oh i find notepad"), TEXT("hello"), MB_OK);
				return 1;
			}
		}
	}
	//����notepad.exe �������CallNextHookEx()����������Ϣ���ݸ�Ӧ�ó��򣨻���һ������)
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
	//MessageBox(NULL, TEXT("third"), TEXT("HELLO"), MB_OK);
}


#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) void HookStart()	//����ĺ������÷�ʽ��dll�ĵ����������е�
	{
		g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hInstance, 0);

	}

	__declspec(dllexport) void HookStop()
	{
		if (g_hHook)
		{
			//ж�ص�����
			UnhookWindowsHookEx(g_hHook);
			g_hHook = NULL;
		}

	}
#ifdef __cplusplus
}
#endif


