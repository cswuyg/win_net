/**
An Example about upload file by libcurl.

cswuyg
2013.7.11
*/

#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <string>

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
		//上传文件，指定本地文件完整路径
		curl_formadd(&pFormPost, &pLastElem, CURLFORM_COPYNAME, "ufile01", CURLFORM_FILE, "C:\\temp\\upload_test_curl_cswuyg.txt", CURLFORM_CONTENTTYPE, "application/octet-stream", CURLFORM_END);

		//上传自定义文件内容的文件，CURLFORM_BUFFER指定服务端文件名
		//http://curl.haxx.se/libcurl/c/curl_formadd.html
		char* file_info = "file_info——cswuyg";
		int record_length = std::string(file_info).length();
		curl_formadd(&pFormPost, &pLastElem,
			CURLFORM_COPYNAME, "ufile01",
			CURLFORM_BUFFER, "cswuyg_test.txt",
			CURLFORM_BUFFERPTR, file_info,
			CURLFORM_BUFFERLENGTH, record_length,
			CURLFORM_END);
		//不加一个结束的标记hfs服务端无法写入文件，一般不存在这种问题，这里加入只是为了测试.
		//curl_formadd(&pFormPost, &pLastElem, CURLFORM_COPYNAME, "end", CURLFORM_COPYCONTENTS, "end", CURLFORM_END);
		curl_easy_setopt(hCurl, CURLOPT_HTTPPOST, pFormPost);
		curl_easy_setopt(hCurl, CURLOPT_URL, "http://127.0.0.1/hfs_up/");

		CURLcode res = curl_easy_perform(hCurl);
		if(res != CURLE_OK)
		{
			std::wcout << "Error" << std::endl;
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