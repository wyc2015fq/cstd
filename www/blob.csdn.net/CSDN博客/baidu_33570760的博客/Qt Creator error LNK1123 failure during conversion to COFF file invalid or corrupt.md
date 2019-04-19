# Qt Creator error: LNK1123: failure during conversion to COFF: file invalid or corrupt - baidu_33570760的博客 - CSDN博客
2016年06月17日 17:33:06[carman_风](https://me.csdn.net/baidu_33570760)阅读数：410标签：[qt																[lnk1123](https://so.csdn.net/so/search/s.do?q=lnk1123&t=blog)](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[qt](https://blog.csdn.net/baidu_33570760/article/category/6898155)
Qt Creator error: LNK1123: 转换到 COFF 期间失败: 文件无效或损坏
治标又治本的解决方法：
找到在 { C:\Windows\Microsoft.NET\Framework\v4.0.30319 } 和 { C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin } 文件夹下的 cvtres.exe 文件。
对比两个 cvtres.exe 的时间，删除旧的文件即可。
**技巧：**在删除之前可以备份 cvtres.exe 为 cvtres.exe.backup，以免造成不必要的误删。
``
