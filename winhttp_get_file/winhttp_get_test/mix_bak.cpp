/**
An Example about download file by WinHttp.

cswuyg
2013.10.9
*/
#include "stdafx.h"
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

#include <string>
#include <iostream>

const int kHeaderLength = 1024;

BOOL ReadData(HINTERNET hRequest, LPSTR pBuffer, DWORD dwSize)
{
	if( hRequest == NULL)
	{
		return FALSE;
	}
	DWORD readed = 0;
	if(!::WinHttpReadData(hRequest, (LPVOID)pBuffer, dwSize, &readed))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL RecvData(HINTERNET hRequest)
{
	if( hRequest == NULL)
	{
		return FALSE;
	}
	DWORD dwHeaderBufferLength = kHeaderLength;
	wchar_t* pHeaderBuffer = (wchar_t*)new wchar_t[kHeaderLength];
	if (pHeaderBuffer == NULL)
	{
		return FALSE;
	}	
	::ZeroMemory(pHeaderBuffer, dwHeaderBufferLength);
	if(!::WinHttpQueryHeaders( hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, pHeaderBuffer, &dwHeaderBufferLength, WINHTTP_NO_HEADER_INDEX))
	{
		return FALSE;
	}
	DWORD status_code = 0;
	dwHeaderBufferLength = sizeof(status_code);
	if (!::WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &status_code, &dwHeaderBufferLength, WINHTTP_NO_HEADER_INDEX)) 
	{
		return FALSE;
	}
	if (status_code != HTTP_STATUS_OK && status_code != HTTP_STATUS_PARTIAL_CONTENT) 
	{
		return FALSE;
	}

	wchar_t szETag[256] = L"";
	dwHeaderBufferLength = kHeaderLength;
	::ZeroMemory(pHeaderBuffer, dwHeaderBufferLength);
	if(::WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_ETAG, WINHTTP_HEADER_NAME_BY_INDEX, pHeaderBuffer, &dwHeaderBufferLength, WINHTTP_NO_HEADER_INDEX)) 
	{
		::wcsncpy_s(szETag, 256, pHeaderBuffer, 255);
	}

	unsigned __int64 nSize = 0;
	dwHeaderBufferLength = kHeaderLength;
	if (::WinHttpQueryHeaders ( hRequest, WINHTTP_QUERY_CONTENT_LENGTH, WINHTTP_HEADER_NAME_BY_INDEX, pHeaderBuffer, &dwHeaderBufferLength, WINHTTP_NO_HEADER_INDEX)) 
	{
		nSize = ::_tcstoui64((PCTSTR)pHeaderBuffer, NULL, 10);
	}

	wchar_t szLastModified[256] = L"";
	dwHeaderBufferLength = kHeaderLength;
	::ZeroMemory(pHeaderBuffer, dwHeaderBufferLength);
	if( ::WinHttpQueryHeaders( hRequest, WINHTTP_QUERY_LAST_MODIFIED, WINHTTP_HEADER_NAME_BY_INDEX, pHeaderBuffer, &dwHeaderBufferLength, WINHTTP_NO_HEADER_INDEX))
	{
		::wcsncpy_s(szLastModified, 256, pHeaderBuffer, 254);
	}

	wchar_t szTransferEncoding[256] = L"";
	dwHeaderBufferLength = kHeaderLength;
	::ZeroMemory(pHeaderBuffer, dwHeaderBufferLength);
	if( ::WinHttpQueryHeaders( hRequest, WINHTTP_QUERY_TRANSFER_ENCODING, WINHTTP_HEADER_NAME_BY_INDEX, pHeaderBuffer, &dwHeaderBufferLength, WINHTTP_NO_HEADER_INDEX))
	{
		::wcsncpy_s(szTransferEncoding, 256, pHeaderBuffer, 254);
	}
	LONGLONG total_size = 0;
	std::wstring last_modify_time;
	std::wstring etag;
	if (::_tcsicmp(szTransferEncoding, L"chunked") == 0)
	{
		total_size = 0;
	}
	else
	{
		if (total_size == 0)
		{
			total_size = nSize;
			last_modify_time = szLastModified;
			etag = szETag;
		}
	}
	HANDLE hFile = ::CreateFile(L"C:\\temp_test", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	BOOL bResult = TRUE;
	DWORD dwNumOfBytesRead = 0;
	while(TRUE)
	{
		DWORD dAvaiSize = 0;
		::WinHttpQueryDataAvailable(hRequest, &dAvaiSize);
		if (dAvaiSize == 0)
		{
			break;
		}
		LPSTR pResponseBuffer = new char[dAvaiSize+1];
		::ZeroMemory(pResponseBuffer, dAvaiSize + 1);
		if(!ReadData(hRequest, pResponseBuffer, dAvaiSize))
		{
			break;
		}
		DWORD dwBytesWritten = 0;
		dwNumOfBytesRead = dAvaiSize;
		if (!::WriteFile(hFile, pResponseBuffer, dAvaiSize, &dwBytesWritten, NULL))
		{
			delete [] pResponseBuffer;
			bResult = FALSE;
			break;
		}
		delete [] pResponseBuffer;
		if (dwBytesWritten != dwNumOfBytesRead)
		{
			break;
		}
	}
	if (pHeaderBuffer != NULL)
	{
		delete pHeaderBuffer;
	}
	return bResult;
}

void WinHttpGetTest()
{
	HINTERNET hSession = ::WinHttpOpen(L"cswuyg get test/1.0", WINHTTP_ACCESS_TYPE_NO_PROXY, NULL, NULL, NULL);
	HINTERNET hConnect = ::WinHttpConnect(hSession, L"172.17.195.146", 80, 0);
	if (hConnect == NULL)
	{
		int i = ERROR_WINHTTP_INCORRECT_HANDLE_TYPE;
		return;
	}
	const wchar_t* lpszAcceptedType[] = {L"*/*", NULL};
	HINTERNET hRequest = ::WinHttpOpenRequest(hConnect, L"GET", L"/hfs_up/a.tmp", L"HTTP/1.1", WINHTTP_NO_REFERER, lpszAcceptedType, 0);
	if (hRequest == NULL)
	{
		return;
	}
	DWORD dwTime = 5000;
	::WinHttpSetOption(hRequest, WINHTTP_OPTION_CONNECT_TIMEOUT, &dwTime, sizeof(DWORD));
	::WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	::WinHttpReceiveResponse(hRequest, NULL);
	RecvData(hRequest);
	::WinHttpCloseHandle(hRequest);
	::WinHttpCloseHandle(hConnect);
	::WinHttpCloseHandle(hSession);
}

int main()
{
	WinHttpGetTest();
	system("pause");
}