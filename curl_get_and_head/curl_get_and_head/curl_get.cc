/**
libcurl example GET HEAD 
2013.8.12
cswuyg@gmail.com
*/

#include "simple_get_file\curl_simple_get_file.h"
#include "simple_get_buffer\curl_simple_get_buffer.h"
#include "get_head_only\curl_get_head_only.h"
#include "pure_header\curl_pure_header.h"

#include <curl\curl.h>
#include <iostream>

class curl_auto_init
{
public:
	curl_auto_init()
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}
	~curl_auto_init()
	{
		curl_global_cleanup();
	}
private:
	curl_auto_init(curl_auto_init&);
	curl_auto_init& operator=(const curl_auto_init&);
};



int main(int argc, wchar_t* argv[])
{
	curl_auto_init auto_init;
	//simple_get_file::simpleGetFile();
	//{
	//	std::string strData = simple_get_buffer::simpleGetBuffer();
	//	FILE* fp_write = NULL;
	//	errno_t e_write_ret = ::_wfopen_s(&fp_write, L"c:\\temp\\simple_get_buffer", L"wb");
	//	::_fseeki64(fp_write, 0, SEEK_SET);
	//	::fwrite(strData.c_str(), strData.length(), 1, fp_write);
	//	::fclose(fp_write);
	//}
	//{
	//	std::string strHeader = get_head_only::getHeadOnly();
	//}
	{
		std::string strHeader = pure_header::getHeader();
	}
	system("pause");
	return 0;
}

