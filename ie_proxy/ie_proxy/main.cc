/**
an example about how to get the ie proxy
cswuyg@gmail.com
2013.5.15
*/
#include <iostream>
#include "ie_proxy/ie_proxy.h"


int main()
{
	net::CIEProxy proxyConfig;
	//net::E_proxy_type proxyType = net::eProxyType_http;
	net::E_proxy_type proxyType = net::eProxyType_all;

	URL_COMPONENTS url_component = { 0 };
	std::wstring strUrl = L"http://msdn.microsoft.com/en-us/library/windows/desktop/aa384376(v=vs.85).aspx";
	url_component.dwStructSize = sizeof(URL_COMPONENTS);
	url_component.dwHostNameLength = (DWORD)-1;
	::WinHttpCrackUrl(strUrl.c_str(), (DWORD)strUrl.length(), 0, &url_component);
	std::wstring strHostName = std::wstring(url_component.lpszHostName, url_component.dwHostNameLength);
	//strHostName = L"127.0.0.1";
	std::wstring strProxy = proxyConfig.GetIEProxy(strHostName.c_str(), proxyType);
	std::wcout << strProxy << std::endl;
	system("pause");
	return 0;
}