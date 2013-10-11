/**
http get file 
cswuyg
2013.10.9
*/
#include "http_get.h"

const int kHeaderLength = 1024;

BOOL HttpGet::Get( const std::wstring& url, const std::wstring& path )
{
	url_ = url;
	save_path_ = path;
	BOOL crack_suc = CrackUrl();
	if (!crack_suc)
	{
		return FALSE;
	}
	HINTERNET session_handle = ::WinHttpOpen(L"cswuyg get test/1.0", WINHTTP_ACCESS_TYPE_NO_PROXY, NULL, NULL, NULL);
	HINTERNET connect_handle = ::WinHttpConnect(session_handle, url_hostname_.c_str(), 80, 0);
	if (connect_handle == NULL)
	{
		int i = ERROR_WINHTTP_INCORRECT_HANDLE_TYPE;
		return FALSE;
	}
	const wchar_t* lpszAcceptedType[] = {L"*/*", NULL};
	request_handle_ = ::WinHttpOpenRequest(connect_handle, L"GET", url_path_.c_str(), L"HTTP/1.1", WINHTTP_NO_REFERER, lpszAcceptedType, 0);
	if (request_handle_ == NULL)
	{
		return FALSE;
	}
	DWORD time_out = 5000;
	::WinHttpSetOption(request_handle_, WINHTTP_OPTION_CONNECT_TIMEOUT, &time_out, sizeof(DWORD));
	::WinHttpSendRequest(request_handle_, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	::WinHttpReceiveResponse(request_handle_, NULL);
	Recv();
	::WinHttpCloseHandle(request_handle_);
	::WinHttpCloseHandle(connect_handle);
	::WinHttpCloseHandle(session_handle);
	return FALSE;
}

BOOL HttpGet::Recv()
{
	if(request_handle_ == NULL)
	{
		return FALSE;
	}
	DWORD head_buf_len = kHeaderLength;
	wchar_t* header_buffer = (wchar_t*)new(std::nothrow)wchar_t[kHeaderLength];
	if (header_buffer == NULL)
	{
		return FALSE;
	}	
	::ZeroMemory(header_buffer, head_buf_len);
	if(!::WinHttpQueryHeaders(request_handle_, WINHTTP_QUERY_RAW_HEADERS_CRLF, WINHTTP_HEADER_NAME_BY_INDEX, header_buffer, &head_buf_len, WINHTTP_NO_HEADER_INDEX))
	{
		return FALSE;
	}
	DWORD status_code = 0;
	head_buf_len = sizeof(DWORD);
	if (!::WinHttpQueryHeaders(request_handle_, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &status_code, &head_buf_len, WINHTTP_NO_HEADER_INDEX)) 
	{
		return FALSE;
	}
	if (status_code != HTTP_STATUS_OK && status_code != HTTP_STATUS_PARTIAL_CONTENT) 
	{
		return FALSE;
	}

	head_buf_len = kHeaderLength;
	if (::WinHttpQueryHeaders (request_handle_, WINHTTP_QUERY_CONTENT_LENGTH, WINHTTP_HEADER_NAME_BY_INDEX, header_buffer, &head_buf_len, WINHTTP_NO_HEADER_INDEX)) 
	{
		remote_filesize_ = ::_wcstoui64(header_buffer, NULL, 10);
	}

	head_buf_len = kHeaderLength;
	::ZeroMemory(header_buffer, head_buf_len);
	if( ::WinHttpQueryHeaders(request_handle_, WINHTTP_QUERY_LAST_MODIFIED, WINHTTP_HEADER_NAME_BY_INDEX, header_buffer, &head_buf_len, WINHTTP_NO_HEADER_INDEX))
	{
		last_modify_time_ = std::wstring(header_buffer, head_buf_len);
	}

	write_file_ = ::CreateFile(save_path_.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	while(TRUE)
	{
		if (!RecvAndWriteToFile())
		{
			break;
		}
	}
	if (header_buffer != NULL)
	{
		delete header_buffer;
	}
	if (remote_filesize_ != has_write_size_)
	{
		return FALSE;
	}
	else 
	{
		return TRUE;
	}
}

BOOL HttpGet::CrackUrl()
{
	URL_COMPONENTS url_component;
	DWORD dwUrlLen = 0;
	::ZeroMemory(&url_component, sizeof(URL_COMPONENTS));
	url_component.dwStructSize = sizeof(URL_COMPONENTS);
	wchar_t lpszHostName[MAX_PATH] = { 0 };
	wchar_t lpszPath[MAX_PATH] = { 0 };
	url_component.lpszHostName = lpszHostName;
	url_component.dwHostNameLength  = MAX_PATH;
	url_component.lpszUrlPath = lpszPath;
	url_component.dwUrlPathLength = MAX_PATH;
	if (!::WinHttpCrackUrl(url_.c_str(), url_.length(), ICU_ESCAPE, &url_component))
	{
		return FALSE;
	}
	else
	{
		url_hostname_ = lpszHostName;
		url_path_ = lpszPath;
		return TRUE;
	}
}

BOOL HttpGet::RecvAndWriteToFile()
{
	if(request_handle_ == NULL)
	{
		return FALSE;
	}
	DWORD available_size = 0;
	::WinHttpQueryDataAvailable(request_handle_, &available_size);
	if (available_size == 0)
	{
		return FALSE;
	}
	BOOL ret = TRUE;
	DWORD readed = 0;
	wchar_t data_buf[8 * 1024];
	wchar_t* buf = data_buf;
	if (available_size > 8 * 1024)
	{
		buf = new(std::nothrow)wchar_t[available_size + 1];
	}
	if(!::WinHttpReadData(request_handle_, buf, available_size, &readed))
	{
		ret =  FALSE;
	}
	else
	{
		DWORD dwBytesWritten = 0;
		if (!::WriteFile(write_file_, buf, available_size, &dwBytesWritten, NULL))
		{
			if (dwBytesWritten != available_size)
			{
				ret = FALSE;
			}
		}
		has_write_size_ += dwBytesWritten;
	}
	double per = (double)has_write_size_ / remote_filesize_;
	per *= 10000;
	if ((per > (ten_thousand_per_ + 100)) || (per == 10000))
	{
		delegate_->GetPercent((int)per);
		ten_thousand_per_ = (int)per;
	}
	if (available_size > 8 * 1024)
	{
		delete [] buf;
	}
	return ret;
}
