# string 中去除空白字符 - ljx0305的专栏 - CSDN博客
2012年08月23日 11:26:24[ljx0305](https://me.csdn.net/ljx0305)阅读数：659
                
str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
