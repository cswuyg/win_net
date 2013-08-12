#include "curl_pure_header.h"

#include <curl/curl.h>



namespace pure_header
{
	const char* const kUrl = "http://dldir1.qq.com/qqfile/qq/QQ2013/2013Beta3/6565/QQ2013Beta3.exe";

	static size_t HeaderCallBack(void *ptr, size_t size, size_t nmemb, void *userdata)
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


	std::string getHeader()
	{
		std::string strRetHeader;
		CURL* hCurl = curl_easy_init();
		if (hCurl == NULL)
		{
			return strRetHeader;
		}
		curl_easy_setopt(hCurl, CURLOPT_URL, kUrl);
		curl_easy_setopt(hCurl, CURLOPT_REFERER, "test_cswuyg.html");
		curl_easy_setopt(hCurl, CURLOPT_NOBODY, 1L);
		curl_easy_setopt(hCurl, CURLOPT_FOLLOWLOCATION, 1);   
		curl_easy_setopt(hCurl, CURLOPT_HEADERFUNCTION, HeaderCallBack);
		curl_easy_setopt(hCurl, CURLOPT_WRITEHEADER, (PVOID)&strRetHeader);
		curl_easy_setopt(hCurl, CURLOPT_CONNECTTIMEOUT, 15);
		long lRet =	curl_easy_perform(hCurl);
		LONG lResponseCode = 0;
		if (curl_easy_getinfo(hCurl, CURLINFO_RESPONSE_CODE , &lResponseCode) != CURLE_OK)
		{
			strRetHeader = "";
		}
		if (lResponseCode != 200)
		{
			strRetHeader = "";
		}
		//double file_size = 0;
		//if((curl_easy_getinfo(hCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &file_size) != CURLE_OK) || (filesize < 0))
		//{
		//	file_size = 0;
		//}
		curl_easy_cleanup(hCurl);
		return strRetHeader;
	}
}
