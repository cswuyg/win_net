#include <windows.h>
#include <string>

/**
GET buffer

base on libcurl 
cswuyg@gmail.com
*/
namespace simple_get_buffer
{
	static int ProgressFunctionCallback( void *clientp, double dltotal, double dlnow, double ultotal, double ulnow );
	static size_t WriteCallBack(void* ptr, size_t size, size_t nmemb, void* stream);
	std::string simpleGetBuffer();
}