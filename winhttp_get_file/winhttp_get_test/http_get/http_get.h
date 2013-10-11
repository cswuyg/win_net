/**
http get file 
cswuyg
2013.10.9
*/
#pragma once

#include <string>
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

class GetDelegate
{
public:
	virtual void GetPercent(int percent) = 0;
};

class HttpGet
{
public:
	HttpGet(GetDelegate* delegate)
		: request_handle_(NULL)
		, remote_filesize_(0)
		, has_write_size_(0)
		, write_file_(INVALID_HANDLE_VALUE)
		, delegate_(delegate)
		, ten_thousand_per_(0)
	{};
	~HttpGet(){};

	//synchronous http get 
	BOOL Get(const std::wstring& url, const std::wstring& path);

private:
	BOOL CrackUrl();
	BOOL RecvAndWriteToFile();
	BOOL Recv();

private:
	std::wstring url_;
	std::wstring save_path_;
	std::wstring url_path_;
	std::wstring url_hostname_;
	std::wstring last_modify_time_;
	HINTERNET request_handle_;
	LONGLONG remote_filesize_;
	LONGLONG has_write_size_;
	HANDLE write_file_;
	GetDelegate* delegate_;
	int ten_thousand_per_;
};