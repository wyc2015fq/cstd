# C++实现urlencode,urldecode - sandro_zhang的专栏 - CSDN博客
2012年05月30日 15:42:26[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：6838
```cpp
namespace HttpUtility
{
   
    typedef unsigned char BYTE;
 
    inline BYTE toHex(const BYTE &x)
    {
        return x > 9 ? x -10 + 'A': x + '0';
    }
    inline BYTE fromHex(const BYTE &x)
    {
        return isdigit(x) ? x-'0' : x-'A'+10;
    }
 
    inline string URLEncode(const string &sIn)
    {
        string sOut;
        for( size_t ix = 0; ix < sIn.size(); ix++ )
        {      
            BYTE buf[4];
            memset( buf, 0, 4 );
            if( isalnum( (BYTE)sIn[ix] ) )
            {      
                buf[0] = sIn[ix];
            }
            //else if ( isspace( (BYTE)sIn[ix] ) ) //貌似把空格编码成%20或者+都可以
            //{
            //    buf[0] = '+';
            //}
            else
            {
                buf[0] = '%';
                buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
                buf[2] = toHex( (BYTE)sIn[ix] % 16);
            }
            sOut += (char *)buf;
        }
        return sOut;
    };
    inline string URLDecode(const string &sIn)
    {
        string sOut;
        for( size_t ix = 0; ix < sIn.size(); ix++ )
        {
            BYTE ch = 0;
            if(sIn[ix]=='%')
            {
                ch = (fromHex(sIn[ix+1])<<4);
                ch |= fromHex(sIn[ix+2]);
                ix += 2;
            }
            else if(sIn[ix] == '+')
            {
                ch = ' ';
            }
            else
            {
                ch = sIn[ix];
            }
            sOut += (char)ch;
        }
        return sOut;
    }
}
```
感谢：[http://www.cnitblog.com/luckydmz/archive/2010/10/12/69959.html](http://www.cnitblog.com/luckydmz/archive/2010/10/12/69959.html)
