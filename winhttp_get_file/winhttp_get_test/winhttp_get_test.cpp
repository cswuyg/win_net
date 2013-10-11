/**
An Example about download file by WinHttp.

cswuyg
2013.10.9
*/
#include "stdafx.h"
#include "http_get\http_get.h"

#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

#include <string>
#include <iostream>

const wchar_t* const kUrl = L"http://172.17.195.146/hfs_up/a.tmp";
const wchar_t* const kPath = L"c:\\abcd.rar";

class GetDelegateImpl : public GetDelegate
{
public:
	void GetPercent(int percent)
	{
		double per = (double)percent / 100;
		std::cout << "finish:" << per << "%" << std::endl; 
	}
};


int main()
{
	std::wcout << L"url:" << kUrl << std::endl;
	std::wcout << L"path:" << kPath << std::endl; 
	GetDelegateImpl call_back;
	HttpGet get_obj(&call_back);
	get_obj.Get(kUrl, kPath);
	system("pause");
}