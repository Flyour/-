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

	// 使用 dwpid 获取目标进程的句柄
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
	{
		_tprintf(L"OpenProcess(%d) failed!!! [%d]\n", dwPID, GetLastError());
		return FALSE;
	}

	// 在目标进程内存中分配SZdLLnAME大小的内存
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);

	// 将myhack.dll 路径写入分配的内存
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllPath, dwBufSize, NULL);

	// 获取loadlibraryw() api 的地址
	// 不同进程中kernel32.dll 的加载地址相同。
	// dll中的函数位置是固定的。
	hMod = GetModuleHandle(L"kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");

	// 在notepae.exe进程中运行线程
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
		// tstol 将字符串转换为整型
		_tprintf(L"InjectDll(\"%s\")success!!!\n", argv[2]);
	else
		_tprintf(L"InjectDll(\"%s\")failed!!!\n", argv[2]);
	return 0;
}
