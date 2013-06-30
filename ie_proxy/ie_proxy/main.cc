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
	std::wstring strProxy = proxyConfig.GetIEProxy(L"www.google.com", proxyType);
	std::wcout << strProxy << std::endl;
	system("pause");
	return 0;
}