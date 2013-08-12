#include "curl_get_head_only.h"

#include <curl/curl.h>


namespace get_head_only
{
	const char* const kUrl = "www.baidu.com";


	size_t HeaderCallBack( void *ptr, size_t size, size_t nmemb, void *userdata )
	{
		if (ptr == NULL)
		{
			return 0;
		}
		std::string strNewData((char*)ptr, size*nmemb);
		std::string* data = static_cast<std::string*>(userdata);
		(*data) += strNewData;
		return size * nmemb; 
	}


	static size_t WriteCallBack(void* ptr, size_t size, size_t nmemb, void* stream)
	{
		//I don't need body
		return 0;
	}


	std::string getHeadOnly()
	{
		std::string strRet;
		CURL* hCurl = curl_easy_init();
		if (hCurl == NULL)
		{
			return strRet;
		}
		curl_easy_setopt(hCurl, CURLOPT_URL, kUrl);
		curl_easy_setopt(hCurl, CURLOPT_REFERER, "test_cswuyg.html");
		curl_easy_setopt(hCurl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(hCurl, CURLOPT_CONNECTTIMEOUT, 15);
		curl_easy_setopt(hCurl, CURLOPT_LOW_SPEED_LIMIT, 1);
		curl_easy_setopt(hCurl, CURLOPT_LOW_SPEED_TIME, 15);
		curl_easy_setopt(hCurl, CURLOPT_HEADERFUNCTION, HeaderCallBack);
		curl_easy_setopt(hCurl, CURLOPT_WRITEHEADER, (PVOID)&strRet);
		curl_easy_setopt(hCurl, CURLOPT_WRITEFUNCTION, WriteCallBack);

		LONG lRet = curl_easy_perform(hCurl);
		if (lRet != CURLE_WRITE_ERROR)  //abort at writecallback would cause CURL_WRITE_ERROR
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