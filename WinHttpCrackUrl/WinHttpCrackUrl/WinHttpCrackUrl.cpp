// 2012-9-9
// 测试、简单学习 WinHttpCrackUrl函数。
// 本代码，通过WinHttpCrackUrl函数获取域名。
// cswuyg

#include "stdafx.h"
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "Winhttp.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	URL_COMPONENTS stUrlComp;
	LPCWSTR pwszUrl1 = L"http://172.17.195.146/hfs_up/a.tmp";
	DWORD dwUrlLen = 0;
	ZeroMemory(&stUrlComp, sizeof(URL_COMPONENTS));
	stUrlComp.dwStructSize = sizeof(URL_COMPONENTS);

	wchar_t lpszHostName[MAX_PATH] = { 0 };
	wchar_t lpszPath[MAX_PATH] = { 0 };
	wchar_t lpszExtra[MAX_PATH] = { 0 };

	stUrlComp.lpszHostName = lpszHostName;
	stUrlComp.dwHostNameLength  = MAX_PATH;
	stUrlComp.lpszUrlPath = lpszPath;
	stUrlComp.dwUrlPathLength = MAX_PATH;
	stUrlComp.lpszExtraInfo = lpszExtra;
	stUrlComp.dwExtraInfoLength = MAX_PATH;

	if (!::WinHttpCrackUrl( pwszUrl1, (DWORD)(LONG_PTR)wcslen( pwszUrl1 ), ICU_ESCAPE, &stUrlComp ) )
	{
		::printf( "Error %u in WinHttpCrackUrl.\n", GetLastError() );
	}
	else
	{
		::printf("HostName : %S\n", lpszHostName);
		::printf("Path : %S\n", lpszPath);
		::printf("ExtraInfo : %S\n", lpszExtra);
	}
	::system("pause");
	return 0;
}
/*
HostName : 172.17.195.146
Path : /hfs_up/a.tmp
ExtraInfo :
请按任意键继续. . .
*/