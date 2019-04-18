# Ubuntu下打开文件乱码及编码转换 - Orisun - 博客园







# [Ubuntu下打开文件乱码及编码转换](https://www.cnblogs.com/zhangchaoyang/articles/2140311.html)





txt文件在Windows下可以正常显示，Ubuntu下打开文件乱码。这是中文编码问题，Windows下用的是gb2312,而linux下用的是utf8。在此提供5种解决方案：

1. 在文档所在目录运行命令

     iconv -f gb2312 -t utf8 -c 1.txt > 2.txt
 选项-c的作用是忽略不能识别的字符，继续向后转换。否则iconv遇到不能识别的字符就终止转换。
 iconv不能实现批量转换，你需要写一段小程序来达到此目的。


2.  运行命令

ubuntu默认的打开.txt文件用的是gedit，可gedit默认编码是UTF8，打开windows下编辑的gb2312的文档都是乱码{Windows中默认的文件格式是GBK(gb2312)，（GBK编码是GB2312编码的超集，向下完全兼容GB2312。）}。解决方法就是把gedit的默认编码改为GB2312。

      gconf-editor

　　然后点开：apps->gedit-2->preferences->encodings

　　双击encodings中的auto-detected

　　在弹开的对话框中加入GB2312

　　再将GB2312移到最上，退出。

3. 用openoffice打开。文件──打开   打开你想要打开的.txt文件，在打开的时候会弹出窗口询问要选择何种字符。如图选择简体中文（GB18030），（GB18030编码向下兼容GBK和GB2312，兼容的含义是不仅字符兼容，而且相同字符的编码也相同。）则打开之后会正常显示了。只是每次都要询问

4. 在Windows上完成从gbk到uft-8的转换，相对于方法1效果要好一些。具体方法如下：

F:\语料库\tc-corpus-train\train>for /r C4-Literature %i in (*.txt) do iconv.exe -f gbk -t utf-8 %i > %~ni.txt


5. 使用enca。
enca可自动判别原文件的编码类型，并且支持通配符，就此能实现批量转换。
enca -L 原文件语言类型 -x 目标编码 原文件名
例如：enca -L zh_CN -x utf-8 file.txt
但是enca会经常遇到转换失败的情况，当然对于不能转换的enca会给出错误报告。












