# Python入门--番外--中文目录乱码问题 - 默槑 - 博客园







# [Python入门--番外--中文目录乱码问题](https://www.cnblogs.com/modaidai/p/6859700.html)





写[Python](http://lib.csdn.net/base/python)的程序，读取含有中文目录下的文件，结果发现根本读取不了该中文目录下的文件，

原因:通过调试发现：该文件的目录乱码,目录无法解析，自然导致无法读取文件内容

解决方法:

strPath = "E:\superBoard\pro_input\1.8有理数的减法\1.8有理数的减法.htm" #这里的strpath是你的中文路径名

strPath = unicode(strPath , "GB2312")

此时得到的中文路径就不会乱码了



延伸知识：

unicode编码系统的发明是为了统一各国的文字，因此也叫“万国码”。Unicode 为每种语言设置了唯一的二进制编码表示方式，也就是说无论哪个国家的语言都可以在 Unicode 上找到对应的代码。因此，当不同的编码系统进行相互转换的时候，可以利用 Unicode 做一个“中介”。

其他编码系统到 Unicode 的转换过程我们称为解码（decode），将 Unicode 转换为其他编码系统的过程称之为编码（encode）。例如 A 编码需要转换为 B 编码，过程如下：

A编码 -> decode(A) -> Unicode -> encode(B) -> B 编码



使用下面的方法得到你当前的默认编码

>>> import sys
>>> sys.getdefaultencoding()　　　　　　#一般显示的都是这个‘ascii’





下面有问题：

如何处理普通字符串和 Unicode 字符串进行拼接抛出 UnicodeDecodeError 异常？




- >>> string = "我爱" + u"Fish"
- Traceback (most recent call last):  File "<stdin>", line 1, in <module>UnicodeDecodeError:'ascii'codec can't decode byte 0xe6 in position 0: ordinal not in range(128)



分析：

使用 + 号进行字符串拼接，左边是普通字符串，右边是 Unicode 字符串。当两种类型的字符串拼接的时候，Python 会自动将左边的中文字符串转换为 Unicode 字符串，再进行拼接操作。但由于 "我爱" 的 ASCII 编码为 '\xe6\x88\x91\xe7\x88\xb1'，其中十六进制 '\xe6' 对应的值是 230。当编码值在 0 ~ 127 的时候，Unicode 和 ASCII 是兼容的，转换不会有什么问题。但当值大于 128 的时候，ASCII 编码便不能直接转换为 Unicode 了。因此，抛出 UnicodeDecodeError。

解决方案：

第一种：使用 Python3
第二种： 指定转换为 Unicode 的解码方式：


- >>> string = "我爱".decode('utf-8') + u"Fish"
- >>> print string
- 我爱Fish



第三种： 将 Unicode 字符串部分进行编码：


- >>> string = "我爱" + u"Fish".encode("utf-8")
- >>> print string
- 我爱Fish



再举个栗子：



问题：文件编码与 Python 编码不同如何解决？

test.txt 内容如下，并保存为 GB2312 编码：


- 我爱，真的！



test.py 内容如下：


- f1 = open("test.txt")
- print(f1.read())
- f1.close



但运行test.pl时会报错：


- >>> 
- Traceback (most recent call last):
-   File "/Users/FishC/Documents/Python/test.py", line 4, in <module>
-     print(f1.read())
-   File "/Library/Frameworks/Python.framework/Versions/3.4/lib/python3.4/encodings/ascii.py", line 26, in decode
-     return codecs.ascii_decode(input, self.errors)[0]
- UnicodeDecodeError: 'ascii' codec can't decode byte 0xce in position 0: ordinal not in range(128)



分析：

如果前边的内容都可以理解了，那么解决这样的编码问题就不再难得住你啦~~~

使用 open 打开文件的编码格式取决于系统（可以通过 locale.getpreferredencoding() 获得），认真看报错信息，这里系统使用 ASCII 对文件内容进行解码，遇到错误......因为我们知道文件的存放格式是 GB2312，因此我们只需要在打开文件的时候设置 encoding="gb2312" 即可解决问题：



- f1 = open("test.txt", encoding="gb2312")
- print(f1.read())
- f1.close

















