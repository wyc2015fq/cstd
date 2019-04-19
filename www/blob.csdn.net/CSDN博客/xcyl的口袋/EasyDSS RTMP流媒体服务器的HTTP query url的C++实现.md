# EasyDSS RTMP流媒体服务器的HTTP query url的C++实现 - xcyl的口袋 - CSDN博客
2017年09月22日 14:37:15[xcyl](https://me.csdn.net/cai6811376)阅读数：426
EasyDSS支持HTTP GET接口访问，我们需要获取url的各种参数信息
比如`http://ip:port/action?a=1&b=2&c=3`
我们需要知道对应的a、b、c的值
这里我们实现了简单的query url，能满足一般要求
后来回实现一个完善的URI解析类
to_lower辅助函数，C++中没有string的转换大小写函数
```
inline void to_lower(std::string& str)
{
    for (int i = 0; i <str.size(); i++)
        str[i] = tolower(str[i]);
}
```
query_url函数，简单实现
```cpp
std::map<std::string, std::string> query_url(const std::string& url)
{
    std::string keyval, key, val;
    //将参数以std::map的方式返回
    std::map<std::string, std::string> params;
    std::istringstream issUrl(url);
    //从issUrl中使用分隔符`&`提取
    while (std::getline(issUrl, keyval, '&'))
    {
        std::istringstream iss(keyval);
        //再以`=`分隔符获取key和value
        if (std::getline(std::getline(iss, key, '='), val))
        {
            to_lower(key);
            params[key] = val;
        }
    }
    return params;
}
```
当然，最应该使用的应该是std::map
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2017
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
