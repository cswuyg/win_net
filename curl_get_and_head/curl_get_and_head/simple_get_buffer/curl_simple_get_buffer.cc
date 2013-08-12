#include "curl_simple_get_buffer.h"

#include <iostream>
#include <curl/curl.h>

namespace simple_get_buffer
{
	const char* const kUrl = "www.baidu.com";

	static int ProgressFunctionCallback( void *clientp, double dltotal, double dlnow, double ultotal, double ulnow )
	{
		if (dltotal == 0)
		{
			return 0;
		}
		std::cout << "percent : " << (dlnow / dltotal) * 100 << " % " << std::endl;
		return 0;
	}

	static size_t WriteCallBack(void* ptr, size_t size, size_t nmemb, void* stream)
	{
		if (ptr == NULL)
		{
			return 0;
		}
		std::string strNewData((char*)ptr, size*nmemb);
		std::string* data = static_cast<std::string*>(stream);
		(*data) += strNewData;
		return size*nmemb;
	}



	std::string simpleGetBuffer()
	{
		std::string strRet;
		CURL* hCurl = curl_easy_init();
		if (hCurl == NULL)
		{
			return strRet;
		}
		BOOL bError = FALSE;
		curl_easy_setopt(hCurl, CURLOPT_URL, kUrl);
		curl_easy_setopt(hCurl, CURLOPT_REFERER, "test_cswuyg.html");
		curl_easy_setopt(hCurl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(hCurl, CURLOPT_CONNECTTIMEOUT, 15);
		curl_easy_setopt(hCurl, CURLOPT_LOW_SPEED_LIMIT, 1);
		curl_easy_setopt(hCurl, CURLOPT_LOW_SPEED_TIME, 15);
		curl_easy_setopt(hCurl, CURLOPT_WRITEFUNCTION, WriteCallBack);
		curl_easy_setopt(hCurl, CURLOPT_WRITEDATA, (PVOID)&strRet);
		curl_easy_setopt(hCurl, CURLOPT_PROGRESSFUNCTION, ProgressFunctionCallback);  
		curl_easy_setopt(hCurl, CURLOPT_PROGRESSDATA, NULL);   
		curl_easy_setopt(hCurl, CURLOPT_NOPROGRESS, 0);

		LONG lRet = curl_easy_perform(hCurl);
		if (lRet != CURLE_OK)
		{
			strRet = "";
		}
		LONG lResponseCode = 0;
		if (curl_easy_getinfo(hCurl, CURLINFO_RESPONSE_CODE , &lResponseCode) !=  CURLE_OK)
		{
			strRet = "";
		}
		else if (lResponseCode != 200)
		{
			strRet = "";
		}
		curl_easy_cleanup(hCurl);
		return strRet;
	}
}