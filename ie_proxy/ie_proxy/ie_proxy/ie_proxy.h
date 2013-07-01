/**
an example about how to get the ie proxy
cswuyg@gmail.com
2013.5.15
*/
#pragma once

#include <string>
#include <windows.h>
#include <Winhttp.h>
#pragma comment(lib, "Winhttp.lib")


namespace net
{
	typedef enum E_proxy_type 
	{
		eProxyType_all = 0,
		eProxyType_http,
		eProxyType_https,
		eProxyType_ftp,
		eProxyType_socks,
	};

	class CIEProxy
	{
	public:
		std::wstring GetIEProxy(const std::wstring& strHostName, const E_proxy_type& eProxyType);

	private:
		/**
		@brief 检查白名单确认是否使用proxy
		*/
		BOOL CheckByPass(const std::wstring& strHostName, const std::wstring& strPassBy);  
		/**
		@brief 从多个proxy串中获取某个proxy串
		*/
		std::wstring GetProxyFromString(const E_proxy_type& eProxyType, const std::wstring& strProxys);
	};
}