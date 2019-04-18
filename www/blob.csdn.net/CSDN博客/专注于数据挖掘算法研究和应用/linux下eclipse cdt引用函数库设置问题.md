# linux下eclipse cdt引用函数库设置问题 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年01月21日 10:38:34[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2354









ubuntu下eclipse cdt 编写C/C++代码时，如果使用openssl函数库，引用了ssl.h函数库：


    #include <openssl/ssl.h>


    但编译仍会提示你引用的openssl函数未定义，如使用SSL_get_peer_certificate()函数会提示：

   对‘SSL_get_peer_certificate’未定义的引用

   collect2: error: ld returned 1 exit status

   后来发现是因为eclipse cdt开发环境函数库引用设置问题。


    如果在命令行下编译c程序，gcc命令上要加“-lssl”，在eclipse CDT开发环境里要通过工程属性设置来解决，


    —右键工程，选择Properties，在“C/C++ Build——Settings”中，添加Library（-l），这里增加"ssl"库。



