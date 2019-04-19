# android错误修正集|| Connection reset by peer - 三少GG - CSDN博客
2012年03月12日 11:51:10[三少GG](https://me.csdn.net/scut1135)阅读数：2313
                
android error: Apostrophe not preceded 错误解决办法
eclipse引入之前项目中的android项目，资源文件values中报特殊字符没有被转义错误，错误信息如下：
error: Apostrophe not preceded by \ (in Shanghai is China’s largest cities, the most important
economic and financial center, transportation hub and foreign trade port, is also China\’s
science and technology, culture and education as an important base. The city\’s land area is
6340.5 square kilometers.)
绝决办法是，把<string name=“shanghai”>Shanghai is China’s largest cities 中的 China’s 的’进行转义既**China\’s**。
问题定位：
Apostrophe not preceded by \-->原来**撇号需要加\',避免编译歧异.**
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**W/System.err(23882): java.net.SocketException: Connection reset by peer**
出现Connection reset by peer的原因是**对端服务器异常**退出了此次连接。即要从服务器端思考解决办法。
客户端遇到这种情况，只有重新和服务器端建立连接。
