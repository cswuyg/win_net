/**
If we use post data no post form, should no use curl_formadd function.
must set the file size to the curl, and set CURLOPT_READFUNCTION\CURLOPT_READDATA.

cswuyg
2013.7.25
*/

#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <string>
//
//
class CUserData
{
public:
	FILE* m_fp_read;
	LONG m_fileSize;
	LONG m_fileNeedRead;
};

/**
用hfs做测试，这个函数返回0，会停留在这里没动静，可能是在等服务器返回数据，这里测试的时候可以设置为返回CURL_READFUNC_ABORT。
*/
static size_t ReadFunctionCallback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t nReadBlock = 0;
	CUserData* pUserData = static_cast<CUserData*>(userdata);
	if (pUserData->m_fileNeedRead == 0)
	{
		return 0;  //return 0, the server should do something. 
	}
	if (size*nmemb < pUserData->m_fileNeedRead)
	{
		nReadBlock = fread_s(ptr, size*nmemb, nmemb, size, pUserData->m_fp_read);
		if (nReadBlock == size)
		{
			pUserData->m_fileNeedRead -= size*nmemb;
			return size*nmemb;
		}
	}
	else 
	{
		LONG nRead = pUserData->m_fileNeedRead;
		nReadBlock = fread_s(ptr, nRead, nRead, 1, pUserData->m_fp_read);
		if (nReadBlock == 1)
		{
			pUserData->m_fileNeedRead = 0;
			return nRead;
		}
	}
	return CURL_READFUNC_ABORT;
}

static int ProgressFunctionCallback( void *clientp, double dltotal, double dlnow, double ultotal, double ulnow )
{
	CURL* hCurl = (CURL*)clientp;
	LONG lResponseCode = 0;
	if(curl_easy_getinfo(hCurl, CURLINFO_RESPONSE_CODE , &lResponseCode) == CURLE_OK)
	{
		return 0;
	}
	return -1;
}

void CurlPostTest()
{
	curl_global_init(CURL_GLOBAL_ALL);
	CURL* hCurl = curl_easy_init();
	if(hCurl != NULL)
	{
		//也许有Expect: 100-continue，去掉它
		curl_slist* pOptionList = NULL;
		pOptionList = curl_slist_append(pOptionList, "Expect:");
		curl_easy_setopt(hCurl, CURLOPT_HTTPHEADER, pOptionList);

		curl_httppost* pFormPost = NULL;
		curl_httppost* pLastElem = NULL;
		curl_easy_setopt(hCurl, CURLOPT_FOLLOWLOCATION, 1);  //支持redirect
		curl_easy_setopt(hCurl, CURLOPT_CONNECTTIMEOUT, 15);
		curl_easy_setopt(hCurl, CURLOPT_URL, "http://172.17.195.156/hfs_up");
		curl_easy_setopt(hCurl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(hCurl, CURLOPT_PROGRESSFUNCTION, ProgressFunctionCallback);  
		curl_easy_setopt(hCurl, CURLOPT_PROGRESSDATA, hCurl);  

		FILE* fp_read = NULL;
		errno_t e_read_ret = _wfopen_s(&fp_read, L"c:\\temp\\a", L"rb"); 
		if (e_read_ret != 0)
		{
			std::cout << "_wfopen_s fail" << std::endl;
		}
		_fseeki64(fp_read, 0, SEEK_END);
		LONGLONG llFileSize = _ftelli64(fp_read);
		LONG lFileSize = llFileSize;  //这里不考虑大于4GB的文件
		CUserData user_data;
		user_data.m_fp_read = fp_read;
		user_data.m_fileSize = lFileSize;
		user_data.m_fileNeedRead = lFileSize;

		_fseeki64(fp_read, 0, SEEK_SET);
		curl_easy_setopt(hCurl, CURLOPT_POSTFIELDSIZE, lFileSize);

		curl_easy_setopt(hCurl, CURLOPT_READDATA, (void*)&user_data);
		curl_easy_setopt(hCurl, CURLOPT_READFUNCTION, ReadFunctionCallback);
		curl_easy_setopt(hCurl, CURLOPT_POST, TRUE);
		curl_easy_setopt(hCurl, CURLOPT_VERBOSE, 1);

		CURLcode res = curl_easy_perform(hCurl);
		if(res != CURLE_OK)
		{
			std::wcout << L"Error" << std::endl;
		}
		if(res == CURLE_ABORTED_BY_CALLBACK )
		{
			std::cout << "CURLE_ABORTED_BY_CALLBACK " << std::endl;
		}
		curl_formfree(pFormPost);
		curl_easy_cleanup(hCurl);
	}

	curl_global_cleanup();
}

int main(int argc, char **argv)
{
	CurlPostTest();
	system("pause");
	return 0;
}