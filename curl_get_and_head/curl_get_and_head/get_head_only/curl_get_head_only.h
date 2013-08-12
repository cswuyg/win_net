#include <windows.h>
#include <string>

/**
@note : NO use HEAD parameter, but use GET . It will receive some body data, but I ignore them.
base on libcurl 
cswuyg@gmail.com
*/
namespace get_head_only
{
	static size_t HeaderCallBack(void *ptr, size_t size, size_t nmemb, void *userdata);
	static size_t WriteCallBack(void* ptr, size_t size, size_t nmemb, void* stream);
	std::string getHeadOnly();
}