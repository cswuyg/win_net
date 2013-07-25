/**
code frome : \src\net\base\escape.cc
can encode url and path.

2013.7.25
cswuyg@gmail.com
*/

#include <iostream>
#include <string>
#include <assert.h>

const char kHexString[] = "0123456789ABCDEF";
inline char IntToHex(int i) 
{
	if (i >= 16)
	{
		assert(false);
	}
	return kHexString[i];
}

struct Charmap {
	bool Contains(unsigned char c) const {
		return ((map[c >> 5] & (1 << (c & 31))) != 0);
	}

	unsigned int map[8];
};

// Everything except alphanumerics and !'()*-._~
// See RFC 2396 for the list of reserved characters.
static const Charmap kQueryCharmap = {{
	0xffffffffL, 0xfc00987dL, 0x78000001L, 0xb8000001L,
		0xffffffffL, 0xffffffffL, 0xffffffffL, 0xffffffffL
}};

// non-printable, non-7bit, and (including space) ?>=<;+'&%$#"![\]^`{|}
static const Charmap kUrlEscape = {{
	0xffffffffL, 0xf80008fdL, 0x78000001L, 0xb8000001L,
		0xffffffffL, 0xffffffffL, 0xffffffffL, 0xffffffffL
}};

// non-printable, non-7bit, and (including space)  "#%:<>?[\]^`{|}
static const Charmap kPathCharmap = {{
	0xffffffffL, 0xd400002dL, 0x78000000L, 0xb8000001L,
		0xffffffffL, 0xffffffffL, 0xffffffffL, 0xffffffffL
}};

static const Charmap kExternalHandlerCharmap = {{
	0xffffffffL, 0x5000080dL, 0x68000000L, 0xb8000001L,
		0xffffffffL, 0xffffffffL, 0xffffffffL, 0xffffffffL
}};

// non-7bit
static const Charmap kNonASCIICharmap = {{
	0x00000000L, 0x00000000L, 0x00000000L, 0x00000000L,
		0xffffffffL, 0xffffffffL, 0xffffffffL, 0xffffffffL
}};

// Everything except alphanumerics, the reserved characters(;/?:@&=+$,) and
// !'()*-._~%
static const Charmap kExternalHandlerCharmap = {{
	0xffffffffL, 0x5000080dL, 0x68000000L, 0xb8000001L,
		0xffffffffL, 0xffffffffL, 0xffffffffL, 0xffffffffL
}};

std::string Escape(const std::string& text, const Charmap& charmap, bool use_plus)
{
	std::string escaped;
	escaped.reserve(text.length() * 3);
	for (unsigned int i = 0; i < text.length(); ++i)
	{
		unsigned char c = static_cast<unsigned char>(text[i]);
		if (use_plus && ' ' == c) 
		{
			escaped.push_back('+');
		}
		else if (charmap.Contains(c))
		{
			escaped.push_back('%');
			escaped.push_back(IntToHex(c >> 4));
			escaped.push_back(IntToHex(c & 0xf));
		}
		else 
		{
			escaped.push_back(c);
		}
	}
	return escaped;
}


int main()
{
	std::string strBeforeEncode = ":version = cswuyg ≤‚ ‘";
	std::string strAfterEncode = Escape(strBeforeEncode, kPathCharmap, false);  //encodeURI 
	std::cout << "after encodeURI:" << strAfterEncode << std::endl; 
	system("pause");
	return 0;
}

/**
another urlencode  example
*/

#define ToHex(n) ((unsigned char) (((n) > 9) ? ((n) - 10 + 'A') : ((n) + '0')))

std::string urlEncode(const std::string& strInput)
{
	std::string strRet;
	for (unsigned int k =0; k< strInput.length(); k++)
	{
		char ch = strInput[k];
		if ((ch >='a' && ch <= 'z') ||
			(ch >='A' && ch <= 'Z') ||
			(ch >='0' && ch <= '9'))
		{
			strRet += ch;
		}
		else
		{
			strRet += '%';
			strRet += (char)ToHex((ch & 0xf0) >> 4);
			strRet += (char)ToHex(ch & 0x0f);
		}
	}
	return strRet;
}
