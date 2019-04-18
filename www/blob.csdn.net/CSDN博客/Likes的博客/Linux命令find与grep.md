# Linux命令find与grep - Likes的博客 - CSDN博客
2019年03月13日 11:52:59[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：7标签：[linux																[命令																[查找](https://so.csdn.net/so/search/s.do?q=查找&t=blog)](https://so.csdn.net/so/search/s.do?q=命令&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[其他](https://blog.csdn.net/songchuwang1868/article/category/8347161)
[https://www.cnblogs.com/zhangmo/p/3571735.html](https://www.cnblogs.com/zhangmo/p/3571735.html)
递归查找某个目录下相应的文件名：
```
#在根目录下查找所有名字带有hosts的文件
find / -name *hosts*
```
在一个特定文件中找特定的字符
`cat CMakeLists.txt |grep CXX`
在一个文件夹下递归查找特定字符
`grep -r CXX`

