# 加密三:base64 - weixin_33985507的博客 - CSDN博客
2018年01月23日 14:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
> 
简介
- 网络上最常见的用于传输8Bit(字节代码)的编码方式之一
> 
特点
- Base64编码具有不可读性
- 把3个字符变成4个字符(Base64要求把每三个8Bit的字节转换为四个6Bit的字节（3*8 = 4*6 = 24），然后把6Bit再添两位高位0，组成四个8Bit的字节，也就是说，转换后的字符串理论上将要比原来的长1/3)。
- 每76个字符加一个换行符。
- 最后的结束符也要处理。
> 
应用
- 在HTTP环境下传递较长的标识信息。
- 应用程序中，url地址进行base64编码
> 
改进方式
- AES与Base64的混合加密与解密
> 
终端指令加密
- $ cd 具体路径
- $ base64 abc.png -o abc.txt
- $ base64 abc.txt -o 123.png -D
> 
[base64在线编码工具](https://link.jianshu.com?t=http%3A%2F%2Fwww1.tc711.com%2Ftool%2FBASE64.htm)
> 
base64 编码表
![2007342-44792504bbda082e.png](https://upload-images.jianshu.io/upload_images/2007342-44792504bbda082e.png)
image.png
