/**
Example:
use tshark.exe to capture the packs. 
"pdml" means xml format and the value is hex string, should "decode" them.

cswuyg
2013.7.16
*/

#include <windows.h>
#include <iostream>
#include <string>

std::string decodeHex(const std::string& strHex)
{
	int nLen = strHex.length() / 2;
	std::string strRet(nLen, 0);
	for (int i = 0; i != nLen; ++i)
	{
		strRet[i] = ((strHex[2*i]>'a') ? (strHex[2*i]-'a'+10) : (strHex[2*i]-'0')) * 16;
		strRet[i] += (strHex[2*i+1]>='a') ? (strHex[2*i+1]-'a'+10) : (strHex[2*i+1]-'0');
	}
	return strRet;
}

void cswuyg_test_tshark()
{
	std::wstring strParam =  L"\"C:\\Program Files\\Wireshark\\tshark.exe\" -i 1 -p -l -T pdml -f \"dst port 80\" -R \"ip.addr==172.17.195.156\"";
	FILE* stream = NULL;
	errno_t err = _wfreopen_s(&stream, L"c:\\temp\\cswuyt_test.xml", L"w", stdout);
	if (err != 0)
	{
		std::cout << "error" << std::endl;
	}

	HANDLE hStd = ::GetStdHandle(STD_OUTPUT_HANDLE);
	//BOOL bSet = ::SetHandleInformation(hStd, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
	STARTUPINFO stStartInfo;
	ZeroMemory(&stStartInfo, sizeof(STARTUPINFO));
	stStartInfo.cb = sizeof(STARTUPINFO); 
	stStartInfo.hStdError = hStd;
	stStartInfo.hStdOutput = hStd;
	PROCESS_INFORMATION stProcInfo; 
	ZeroMemory(&stProcInfo, sizeof(PROCESS_INFORMATION));
	BOOL bSuccess = ::CreateProcess(NULL, const_cast<wchar_t*>(strParam.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &stStartInfo, &stProcInfo); 
	::CloseHandle(stProcInfo.hProcess);
	::CloseHandle(stProcInfo.hThread);
	::fclose(stream);
}


int main()
{
	std::cout << "hex decode test" << std::endl;
	std::cout << decodeHex("436f6e74656e742d446973706f736974696f6e3a20666f726d2d646174613b206e616d653d227566696c653031223b2066696c656e616d653d226373777579675f746573742e747874220d0a") << std::endl;
	std::cout << std::endl << std::endl;
	std::cout << "capture xml format pack...save at c:\\temp\\cswuyt_test.xml" << std::endl;
	cswuyg_test_tshark();

	system("pause");
	return 0;
}

