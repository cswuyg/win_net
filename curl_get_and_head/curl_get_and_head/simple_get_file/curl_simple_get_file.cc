#include "curl_simple_get_file.h"

#include <iostream>
#include <curl/curl.h>

/**
simple example
*/
void first_simple_get()
{
	CURL* hCurl = curl_easy_init();
	if(hCurl != NULL)
	{
		curl_easy_setopt(hCurl, CURLOPT_URL, "http://www.weather.com.cn/data/sk/101210102.html");
		curl_easy_setopt(hCurl, CURLOPT_VERBOSE, 1);
		CURLcode ret = curl_easy_perform(hCurl);
		if(ret != CURLE_OK)
		{
			std::wcout << "Error" << std::endl;
		}

		curl_easy_cleanup(hCurl);
	}
}


namespace simple_get_file
{

	const char* const kUrl = "http://dldir1.qq.com/qqfile/qq/QQ2013/2013Beta3/6565/QQ2013Beta3.exe";

	static size_t WriteCallBack( void* ptr, size_t size, size_t nmemb, void* stream )
	{
		size_t stLength = size * nmemb;
		if (NULL == ptr || stLength <= 0)
		{
			return 0;
		}
		FILE* fp_write = static_cast<FILE*>(stream);
		size_t nWriteSize = fwrite(ptr, nmemb, size, fp_write);
		if (stLength != nWriteSize*nmemb)
		{
			int i = 0;
		}
		return nWriteSize*nmemb;
	}

	static int ProgressFunctionCallback( void *clientp, double dltotal, double dlnow, double ultotal, double ulnow )
	{
		double dPercent = dlnow / dltotal;
		std::cout << "percent : " << dPercent * 100 << " %" << std::endl;
		return 0;
	}


	BOOL simpleGetFile()
	{
		FILE* fp_write = NULL;
		errno_t e_write_ret = ::_wfopen_s(&fp_write, L"c:\\temp\\first_file_get", L"wb");
		_fseeki64(fp_write, 0, SEEK_SET);
		CURL* hCurl = curl_easy_init();
		if (hCurl == NULL)
		{
			return 0;
		}
		BOOL bRet = TRUE;

		curl_easy_setopt(hCurl, CURLOPT_URL, kUrl);
		curl_easy_setopt(hCurl, CURLOPT_REFERER, "test_cswuyg.html");
		curl_easy_setopt(hCurl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(hCurl, CURLOPT_CONNECTTIMEOUT, 15);
		curl_easy_setopt(hCurl, CURLOPT_LOW_SPEED_LIMIT, 1);
		curl_easy_setopt(hCurl, CURLOPT_LOW_SPEED_TIME, 15);
		curl_easy_setopt(hCurl, CURLOPT_WRITEFUNCTION, WriteCallBack);
		curl_easy_setopt(hCurl, CURLOPT_WRITEDATA, (PVOID)fp_write);
		curl_easy_setopt(hCurl, CURLOPT_PROGRESSFUNCTION, ProgressFunctionCallback);  
		curl_easy_setopt(hCurl, CURLOPT_PROGRESSDATA, NULL);   
		curl_easy_setopt(hCurl, CURLOPT_NOPROGRESS, 0);

		LONG lRet = curl_easy_perform(hCurl);
		if (lRet != CURLE_OK)
		{
			bRet = FALSE;
		}
		::fclose(fp_write);

		LONG lResponseCode = 0;
		if (curl_easy_getinfo(hCurl, CURLINFO_RESPONSE_CODE , &lResponseCode) !=  CURLE_OK)
		{
			bRet = FALSE;
		}
		else if (lResponseCode != 200)
		{
			bRet = FALSE;
		}

		curl_easy_cleanup(hCurl);
		return bRet;
	}
}