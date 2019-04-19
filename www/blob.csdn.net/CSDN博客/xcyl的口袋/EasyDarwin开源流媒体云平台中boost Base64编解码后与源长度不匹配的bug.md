# EasyDarwin开源流媒体云平台中boost Base64编解码后与源长度不匹配的bug - xcyl的口袋 - CSDN博客
2016年07月22日 00:39:38[xcyl](https://me.csdn.net/cai6811376)阅读数：410
EasyDarwin云平台中部分协议使用了Base64编码昨晚报文通信的载体。比如在对摄像机进行对讲操作时，客户端将采集的音频数据经过Base64编码，通过POST的方式将组织的协议数据发送到EasyCMS，EasyCMS再对设备进行操作。
当设备接收到音频数据时，将Base64编码后的数据进行解码，得到音频数据，将音频数据播放出来。
这里，我们以前的Base64编解码是这样写的。（使用boost中的Base64封装）
```
//encode
string Base64Encode(const string &sInput)
{
    typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<string::const_iterator, 6, 8> > Base64EncodeIterator;
    stringstream result;
    copy(Base64EncodeIterator(sInput.begin()), Base64EncodeIterator(sInput.end()), ostream_iterator<char>(result));
    size_t equal_count = (3 - sInput.length() % 3) % 3;
    for (size_t i = 0; i < equal_count; i++)
    {
        result.put('=');
    }
    return result.str();
}
```
```
//decode
string Base64Decode(const string &sInput)
{
    typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<string::const_iterator>, 8, 6> Base64DecodeIterator;
    stringstream result;
    try
    {
        copy(Base64DecodeIterator(sInput.begin()), Base64DecodeIterator(sInput.end()), ostream_iterator<char>(result));
    }
    catch (...)
    {
        return string();
    }
    return result.str();
}
```
但是，我们在进行对讲功能开发的时候发现一个问题：在decode的时候，得出的解码后的string的size有的时候会比源多一个或两个长度，并且为‘\0’。这是因为
> 
Base64编码原理是把3字节的二进制数据编码为4字节的文本数据，长度增加33%。 
  如果要编码的二进制数据不是3的倍数，会在最后剩下1个或2个字节用\x00字节在末尾补足，然后在编码的末尾加上1个或2个=号。
boost中Base64解码的写法，将‘=’解码为‘\0’。这里我们做一个改进。
```cpp
//decode
string Base64Decode(const string &sInput)
{
    typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<string::const_iterator>, 8, 6> Base64DecodeIterator;
    stringstream result;
    try
    {
        string temp = sInput;
        int endIndex = temp.size() - 1;
        if (temp[endIndex] == '=')
        {
            temp.erase(endIndex);
        }
        endIndex = temp.size() - 1;
        if (temp[endIndex] == '=')
        {
            temp.erase(endIndex);
        }
        copy(Base64DecodeIterator(temp.begin()), Base64DecodeIterator(temp.end()), ostream_iterator<char>(result));
    }
    catch (...)
    {
        return string();
    }
    return result.str();
}
```
这里，解码后的数据长度就和源数据长度相同了。
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：288214068
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
