# 编译程序中出现“fatal error C1010: unexpected end of file” - ljx0305的专栏 - CSDN博客
2008年05月28日 14:40:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1087标签：[file																[c																[header](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[问题总结](https://blog.csdn.net/ljx0305/article/category/402025)
我在编译程序中老出现“fatal error C1010: unexpected end of file while looking for precompiled header directive”
这里肯定是一个新添加的类的.cpp文件开头没包含stdafx.h，在该文件最前面加上即可
有时可以使用右键点击项目工程中的该cpp文件，选择setting，在c/c++栏，选择PreCompiled headers，然后设置第一选项，选择不使用预编译头，解决这个问题。 
