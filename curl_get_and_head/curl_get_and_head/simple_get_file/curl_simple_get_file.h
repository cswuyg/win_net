#include <windows.h>


/**
GET file

base on libcurl 
cswuyg@gmail.com
*/
namespace simple_get_file
{
	static size_t WriteCallBack(void* ptr, size_t size, size_t nmemb, void* stream);
	static int ProgressFunctionCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	BOOL simpleGetFile();
}










