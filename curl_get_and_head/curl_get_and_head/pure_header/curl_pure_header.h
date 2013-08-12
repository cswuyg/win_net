#include <windows.h>
#include <string>


/**
HEAD parameter 

cswuyg@gmail.com
*/
namespace pure_header
{
	static size_t HeaderCallBack(void *ptr, size_t size, size_t nmemb, void *userdata);
	std::string getHeader();
}