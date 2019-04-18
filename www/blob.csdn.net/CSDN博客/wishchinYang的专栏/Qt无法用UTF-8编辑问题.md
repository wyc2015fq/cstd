# Qt无法用UTF-8编辑问题 - wishchinYang的专栏 - CSDN博客
2017年10月30日 10:17:00[wishchin](https://me.csdn.net/wishchin)阅读数：5083
原因：
Windows默认编码格式是GBK. 而QT-各默认版本的编码格式是UTF-8.
解决方法“：
Windows环境下,Qt Creator,菜单->工具->选项->文本编辑器->行为->文件编码:
默认编码:System(简体中文windows系统默认指的是GBK编码, 即下拉框选项里的GBK/windows-936-2000/CP936/MS936/windows-936)
            
