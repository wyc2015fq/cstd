# Sybase：函数（对比sqlserver） - weixin_33985507的博客 - CSDN博客
2010年05月26日 10:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
**概述：**
- sybase的函数较sqlserver更为单纯，仅仅能用在表达式内
- sybase支持用户自定义函数：sql函数、Java函数；而sqlserver支持：sql函数和.net函数。老死不相往来啊！
- sybase有许多丰富、强大的系统管理方面的函数，而sqlserver更喜欢通过扩展存储过程来实现
- sybase有许多同功能的函数：目的是方面性、兼容性
**重点函数：**
- 集合函数 带有window窗口字句，这是sqlserver中没有的
- csconvert字符集转换函数
- encrypt、decrypt、base64_encode、base64_decode加密、解密函数
- errosmsg错误信息函数
- 事件处理函数
- hash散列函数
- http_encode、http_decode、html_encode、html_decode函数字符实体
- 统计函数
- xml函数

