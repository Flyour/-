// InjectDll.cpp

#include <Windows.h>
#include <tchar.h>

BOOL InjectDll(DWORD dwPID, LPCTSTR szDllPath)
{
	HANDLE hProcess = NULL, hThread = NULL;
	HMODULE hMod = NULL;
	LPVOID pRemoteBuf = NULL;
	DWORD dwBufSize = (DWORD)(_tcslen(szDllPath) * 2) + sizeof(TCHAR);
	LPTHREAD_START_ROUTINE pThreadProc;

	// ʹ�� dwpid ��ȡĿ����̵ľ��
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
	{
		_tprintf(L"OpenProcess(%d) failed!!! [%d]\n", dwPID, GetLastError());
		return FALSE;
	}

	// ��Ŀ������ڴ��з���SZdLLnAME��С���ڴ�
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);

	// ��myhack.dll ·��д�������ڴ�
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllPath, dwBufSize, NULL);

	// ��ȡloadlibraryw() api �ĵ�ַ
	// ��ͬ������kernel32.dll �ļ��ص�ַ��ͬ��
	// dll�еĺ���λ���ǹ̶��ġ�
	hMod = GetModuleHandle(L"kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");

	// ��notepae.exe�����������߳�
	hThread = CreateRemoteThread(hProcess,
		NULL,
		0,
		pThreadProc,
		pRemoteBuf,
		0,
		NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;
}

int _tmain(int argc, TCHAR *argv[])
{
	if (argc != 3)
	{
		_tprintf(L"USAGE : %s pid dll_path\n", argv[0]);
		return 1;
	}

	// inject dll
	if (InjectDll((DWORD)_tstol(argv[1]), argv[2]))
		// tstol ���ַ���ת��Ϊ����
		_tprintf(L"InjectDll(\"%s\")success!!!\n", argv[2]);
	else
		_tprintf(L"InjectDll(\"%s\")failed!!!\n", argv[2]);
	return 0;
}
