// myhack.cpp

#include <windows.h>
#include <tchar.h>

#pragma comment(lib, "urlmon.lib")

#define DEF_URL  (L"http://meizitu.com/")
#define DEF_FILE_NAME (L"index.html")

HMODULE g_hMod = NULL;

DWORD WINAPI ThreadProc(LPVOID lParam)
{
	WCHAR szPath[_MAX_PATH] = { 0, };
	if (!GetModuleFileNameW(g_hMod, szPath, MAX_PATH))	//�ҵ�dll����·��
		return FALSE;
	//MessageBoxW(NULL, szPath, L"hello", MB_OK);
	WCHAR *p = wcsrchr(szPath, L'\\');	
	if (!p)
		return FALSE;
	wcscpy_s(p + 1, 30, DEF_FILE_NAME);	//ƴ����index.html
	//OutputDebugString(p+1);
	//MessageBoxW(NULL, szPath, L"hello", MB_OK);
	HRESULT	res  = URLDownloadToFile(NULL, DEF_URL, szPath, 0, NULL);	//����ҳ��

	switch (res) {
	case S_OK:
		MessageBox(NULL, TEXT("S_OK"), L"HELLO", MB_OK);
	case E_OUTOFMEMORY:
		MessageBox(NULL, TEXT("E_OUTOFMEMORY"), L"HELLO", MB_OK);
	case INET_E_DOWNLOAD_FAILURE:
		MessageBox(NULL, TEXT("INET_E_DOWNLOAD_FAILURE"), L"HELLO", MB_OK);
	}

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	HANDLE hThread = NULL;
	g_hMod = (HMODULE)hinstDLL;	// ��ʼ��ȫ�ֱ���
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString(L"myhack.dlll Injection!!!");	//��ӡ������Ϣ
		hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);	//ִ���������
		CloseHandle(hThread);
		break;
	}
	return TRUE;
}
