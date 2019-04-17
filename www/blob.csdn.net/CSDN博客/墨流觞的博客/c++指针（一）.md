# c++指针（一） - 墨流觞的博客 - CSDN博客





2017年02月21日 12:49:40[墨氲](https://me.csdn.net/dss_dssssd)阅读数：137标签：[c++ 指针](https://so.csdn.net/so/search/s.do?q=c++ 指针&t=blog)
个人分类：[c++](https://blog.csdn.net/dss_dssssd/article/category/6737792)









参考书籍：

  《c++程序设计》、《c++primeplus》、《c和指针》、《数据结构与算法——c++》

1.      计算机地址

1.计算机地址是以字节（byte）为单位的，通常用16进制显示，例如0x0012与0x0013差1个字节

注意若非地址而是普通的数字中位操作是以位（bit）为单位进行，一个字节包含8个比特位,

CPU存储数据的两种格式：

大端（也叫大印第安序）（Big endian）：低地址存放储存数据的高位字节

小端（也叫小印第安序）（Little endian）：低地址存放储存数据的低位字节

比如在CPU内存地址上存储4字节的整形变量0x12345678，假设从0x1000开始存储
|存储数据格式|0x1000|0x1001|0x1002|0x1003|
|----|----|----|----|----|
|大端|0x78|0x56|0x34|0x12|
|小端|0x12|0x34|0x56|0x78|

一般来说，x86系列的CPU是Little endian

代码验证：



```cpp
unsigned int num=0;//初始化num为0，防止num为混乱值
cout << "存储num的地址：" << &num << endl;
unsigned char *p = (unsigned char*)#//取出num地址中的低字节地址
*p = 0xff;
if (num == 0xff)
{
cout << "小端";
}
else //0xff被解析位高位字节
{
cout << "大端";
}
*(p++) = 0xff;//将num的第二个字节地址赋值
cout <<endl<< num;
```



输出：在Windows平台下验证




存储num的地址：009AF738

小端

255*/

模型解释：






