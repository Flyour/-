#include <stdio.h>
#include <conio.h>
#include <Windows.h>

#define DEF_DLL_NAME "KeyHook.dll"
#define DEF_HOOKSTART "HookStart"
#define DEF_HOOKSTOP "HookStop"

// ʹ��typedef ������������������
typedef void(*PFN_HOOKSTART)();
typedef void(*PFN_HOOKSTOP)();

int main()
{
	HMODULE hDll = NULL;
	PFN_HOOKSTART HookStart = NULL;
	PFN_HOOKSTOP HookStop = NULL;
	char ch = 0;

	//����KeyHook.dll
	hDll = LoadLibraryA(DEF_DLL_NAME);

	//��ȡ����������ַ
	HookStart = (PFN_HOOKSTART)GetProcAddress(hDll, DEF_HOOKSTART);
	HookStop = (PFN_HOOKSTOP)GetProcAddress(hDll, DEF_HOOKSTOP);

	// ��ʼ��ȡ
	HookStart();

	// �ȴ�ֱ���û����� "q"
	printf("please input 'q' to quit!\n");
	//MessageBox(NULL, TEXT("start"), TEXT("SHIT"), MB_OK);
	while (_getch() != 'q');

	//��ֹ��ȡ
	HookStop();

	//ж��KeyHook.dll
	FreeLibrary(hDll);
	return 0;

}