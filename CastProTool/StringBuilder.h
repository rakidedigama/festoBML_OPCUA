// from: http://stackoverflow.com/questions/492475/inline-ostringstream-macro-reloaded/492494#492494

#ifndef STRINGBUILDER_H__
#define STRINGBUILDER_H__

#include <sstream>
#include <windows.h>


class StringBuilder {
public:
    template<typename T>
    StringBuilder& operator<<(T const& obj) 
    {
        buffer << obj;
        return *this;
    }
    operator std::string() 
    {
        return buffer.str();
    }
	static inline bool Convert(wchar_t* wszConvertTo, char* szText)
	{
			size_t iLen = strlen(szText)+1;
			if ( wszConvertTo == 0 )
				return false;
			memset(wszConvertTo,0,iLen);
            ::MultiByteToWideChar(  CP_ACP, NULL,szText, -1, wszConvertTo,(int)iLen );

			//now pass wText
			return true;

	}

private:
    std::ostringstream buffer;
};

#endif // STRINGBUILDER_H__
