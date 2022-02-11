#include <windows.h>
#include <iostream>
using namespace std;

//远程线程注入
bool RemoteThreadInject(DWORD dwPid)
{
    //1.使用PID打开进程获取权限
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, dwPid);

    if (!hProcess)
    {
        printf("PID FALSE\n");
        return false;
    }

    //2.申请内存,写入DLL路径
    int nLen = sizeof(WCHAR)*(wcslen(L"D:\\MakeDLL.dll") + 1);
    LPVOID pBuf = VirtualAllocEx(hProcess, NULL, nLen, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!pBuf)
    {
        printf("GET FALSE\n");
        return false;
    }
    //3.写入内存
    SIZE_T dwWrite = 0;
    if (!WriteProcessMemory(hProcess, pBuf, L"D:\\MakeDLL.dll", nLen, &dwWrite))
    {   
        printf("WRITE FALSE\n");
        return false;
    }

    //4.创建远程线程，让对方调用LoadLibrary
    HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL,
        (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW"), pBuf, 0, 0);
    if (!hRemoteThread)
    {
        printf("Create FALSE\n");
        return false;
    }

    //5.等待线程结束返回,释放资源
    WaitForSingleObject(hRemoteThread, -1);
    CloseHandle(hRemoteThread);
    VirtualFreeEx(hProcess, pBuf, 0, MEM_FREE);

    return true;
}


int main()
{
    DWORD dwPid=7592;//填入进程id
    RemoteThreadInject(dwPid);
    cout<<GetLastError()<<endl;

    return 0;
}
