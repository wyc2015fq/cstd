# 【editdistance】在Windows环境下配置python库editdistance - CD's Coding - CSDN博客





2017年05月05日 14:20:21[糖果天王](https://me.csdn.net/okcd00)阅读数：2046标签：[windows																[python																[编辑距离																[ed](https://so.csdn.net/so/search/s.do?q=ed&t=blog)
个人分类：[DIY																[开发备忘](https://blog.csdn.net/okcd00/article/category/2611293)](https://blog.csdn.net/okcd00/article/category/1775037)





编辑距离是文本处理中很常见的一种判别相似度的方法，

> 
**Wikipedia**

  In computer science, edit distance is a way of quantifying how dissimilar two strings (e.g., words) are to one another by counting the minimum number of operations required to transform one string into the other.

Given two strings a and b on an alphabet Σ (e.g. the set of ASCII characters, the set of bytes [0..255], etc.), the edit distance d(a, b) is the minimum-weight series of edit operations that transforms a into b. One of the simplest sets of edit operations is that defined by Levenshtein in 1966:[2]

- Insertion of a single symbol. If a = uv, then inserting the symbol x produces uxv. This can also be denoted ε→x, using ε to denote the empty string.
- Deletion of a single symbol changes uxv to uv (x→ε).
- Substitution of a single symbol x for a symbol y ≠ x changes uxv to uyv (x→y)

> 
**Baidu**

  编辑距离（Edit Distance），又称Levenshtein距离，是指两个字串之间，由一个转成另一个所需的最少编辑操作次数。许可的编辑操作包括将一个字符替换成另一个字符，插入一个字符，删除一个字符。一般来说，编辑距离越小，两个串的相似度越大。 

  例如将kitten转成sitting： 

  kitten->sitten （k→s） 

  sitten->sittin （e→i） 

  sittin->sitting （插入g） 

  俄罗斯科学家Vladimir Levenshtein在1965年提出这个概念。
在python中，我们通常使用`import editdistance`来直接调用，但是，在Windows下我们直接`pip install editdistance`  的时候会报错

```
C:\Users\Work>pip install editdistance
Collecting editdistance
  Using cached editdistance-0.3.1.tar.gz
Building wheels for collected packages: editdistance
  Running setup.py bdist_wheel for editdistance ... error
  Complete output from command C:\ProgramData\Anaconda3\envs\python2.7\python.exe -u -c "import setuptools, tokenize;__file__='c:\\users\\work\\appdata\\local\\temp\\pip-build-w4myie\\editdistance\\setup.py';f=getattr(tokenize, 'open', open)(__file__);code=f.read().replace('\r\n', '\n');f.close();exec(compile(code, __file__, 'exec'))" bdist_wheel -d c:\users\work\appdata\local\temp\tmpnrvewtpip-wheel- --python-tag cp27:
  running bdist_wheel
  running build
  running build_py
  creating build
  creating build\lib.win-amd64-2.7
  creating build\lib.win-amd64-2.7\editdistance
  copying editdistance\__init__.py -> build\lib.win-amd64-2.7\editdistance
  copying editdistance\_editdistance.h -> build\lib.win-amd64-2.7\editdistance
  copying editdistance\def.h -> build\lib.win-amd64-2.7\editdistance
  running build_ext
  building 'editdistance.bycython' extension
  creating build\temp.win-amd64-2.7
  creating build\temp.win-amd64-2.7\Release
  creating build\temp.win-amd64-2.7\Release\editdistance
  C:\Users\Work\AppData\Local\Programs\Common\Microsoft\Visual C++ for Python\9.0\VC\Bin\amd64\cl.exe /c /nologo /Ox /MD /W3 /GS- /DNDEBUG -I./editdistance -IC:\ProgramData\Anaconda3\envs\python2.7\include -IC:\ProgramData\Anaconda3\envs\python2.7\PC /Tpeditdistance/_editdistance.cpp /Fobuild\temp.win-amd64-2.7\Release\editdistance/_editdistance.obj
  _editdistance.cpp
  editdistance/_editdistance.cpp : warning C4819: The file contains a character that cannot be represented in the current code page (936). Save the file in Unicode format to prevent data loss
  C:\Users\Work\AppData\Local\Programs\Common\Microsoft\Visual C++ for Python\9.0\VC\Include\xlocale(342) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
  editdistance/_editdistance.cpp(117) : error C2059: syntax error : 'if'
  editdistance/_editdistance.cpp(118) : error C2059: syntax error : 'else'
  editdistance/_editdistance.cpp(119) : error C2059: syntax error : 'else'
  editdistance/_editdistance.cpp(120) : error C2059: syntax error : 'else'
  editdistance/_editdistance.cpp(121) : error C2059: syntax error : 'else'
  editdistance/_editdistance.cpp(122) : error C2059: syntax error : 'else'
  editdistance/_editdistance.cpp(123) : error C2059: syntax error : 'else'
  editdistance/_editdistance.cpp(124) : error C2059: syntax error : 'else'
  editdistance/_editdistance.cpp(125) : error C2059: syntax error : 'else'
  editdistance/_editdistance.cpp(126) : error C2059: syntax error : 'else'
  editdistance/_editdistance.cpp(127) : error C2059: syntax error : 'return'
  editdistance/_editdistance.cpp(128) : error C2059: syntax error : '}'
  editdistance/_editdistance.cpp(128) : error C2143: syntax error : missing ';' before '}'
  editdistance/_editdistance.cpp(128) : error C2059: syntax error : '}'
  error: command 'C:\\Users\\Work\\AppData\\Local\\Programs\\Common\\Microsoft\\Visual C++ for Python\\9.0\\VC\\Bin\\amd64\\cl.exe' failed with exit status 2

  ----------------------------------------
  Failed building wheel for editdistance
  Running setup.py clean for editdistance
Failed to build editdistance
Installing collected packages: editdistance
  Running setup.py install for editdistance ... error
    Complete output from command C:\ProgramData\Anaconda3\envs\python2.7\python.exe -u -c "import setuptools, tokenize;__file__='c:\\users\\work\\appdata\\local\\temp\\pip-build-w4myie\\editdistance\\setup.py';f=getattr(tokenize, 'open', open)(__file__);code=f.read().replace('\r\n', '\n');f.close();exec(compile(code, __file__, 'exec'))" install --record c:\users\work\appdata\local\temp\pip-xfiuou-record\install-record.txt --single-version-externally-managed --compile:
    running install
    running build
    running build_py
    creating build
    creating build\lib.win-amd64-2.7
    creating build\lib.win-amd64-2.7\editdistance
    copying editdistance\__init__.py -> build\lib.win-amd64-2.7\editdistance
    copying editdistance\_editdistance.h -> build\lib.win-amd64-2.7\editdistance
    copying editdistance\def.h -> build\lib.win-amd64-2.7\editdistance
    running build_ext
    building 'editdistance.bycython' extension
    creating build\temp.win-amd64-2.7
    creating build\temp.win-amd64-2.7\Release
    creating build\temp.win-amd64-2.7\Release\editdistance
    C:\Users\Work\AppData\Local\Programs\Common\Microsoft\Visual C++ for Python\9.0\VC\Bin\amd64\cl.exe /c /nologo /Ox /MD /W3 /GS- /DNDEBUG -I./editdistance -IC:\ProgramData\Anaconda3\envs\python2.7\include -IC:\ProgramData\Anaconda3\envs\python2.7\PC /Tpeditdistance/_editdistance.cpp /Fobuild\temp.win-amd64-2.7\Release\editdistance/_editdistance.obj
    _editdistance.cpp
    editdistance/_editdistance.cpp : warning C4819: The file contains a character that cannot be represented in the current code page (936). Save the file in Unicode format to prevent data loss
    C:\Users\Work\AppData\Local\Programs\Common\Microsoft\Visual C++ for Python\9.0\VC\Include\xlocale(342) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
    editdistance/_editdistance.cpp(117) : error C2059: syntax error : 'if'
    editdistance/_editdistance.cpp(118) : error C2059: syntax error : 'else'
    editdistance/_editdistance.cpp(119) : error C2059: syntax error : 'else'
    editdistance/_editdistance.cpp(120) : error C2059: syntax error : 'else'
    editdistance/_editdistance.cpp(121) : error C2059: syntax error : 'else'
    editdistance/_editdistance.cpp(122) : error C2059: syntax error : 'else'
    editdistance/_editdistance.cpp(123) : error C2059: syntax error : 'else'
    editdistance/_editdistance.cpp(124) : error C2059: syntax error : 'else'
    editdistance/_editdistance.cpp(125) : error C2059: syntax error : 'else'
    editdistance/_editdistance.cpp(126) : error C2059: syntax error : 'else'
    editdistance/_editdistance.cpp(127) : error C2059: syntax error : 'return'
    editdistance/_editdistance.cpp(128) : error C2059: syntax error : '}'
    editdistance/_editdistance.cpp(128) : error C2143: syntax error : missing ';' before '}'
    editdistance/_editdistance.cpp(128) : error C2059: syntax error : '}'
    error: command 'C:\\Users\\Work\\AppData\\Local\\Programs\\Common\\Microsoft\\Visual C++ for Python\\9.0\\VC\\Bin\\amd64\\cl.exe' failed with exit status 2

    ----------------------------------------
Command "C:\ProgramData\Anaconda3\envs\python2.7\python.exe -u -c "import setuptools, tokenize;__file__='c:\\users\\work\\appdata\\local\\temp\\pip-build-w4myie\\editdistance\\setup.py';f=getattr(tokenize, 'open', open)(__file__);code=f.read().replace('\r\n', '\n');f.close();exec(compile(code, __file__, 'exec'))" install --record c:\users\work\appdata\local\temp\pip-xfiuou-record\install-record.txt --single-version-externally-managed --compile" failed with error code 1 in c:\users\work\appdata\local\temp\pip-build-w4myie\editdistance\
```

于是，我就去 [PYPI](https://pypi.python.org/pypi/editdistance) 下载了一下 [editdistance-0.3.1.tar.gz](https://pypi.python.org/packages/a8/08/97ed5b60023ab4b67a46ee24bf1d3db93a9abc5b3be7e8f90de18160aaed/editdistance-0.3.1.tar.gz)

然后解压，在本地安装
`$ python setup.py install`
一样的报错，但是这一次我注意到了一点：

> 
editdistance/ **_editdistance.cpp** (117) : error C2059: syntax error


于是我在文件夹里找到这个cpp文件，打开：

![cpp](https://img-blog.csdn.net/20170505141738888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

好的我懂了…… 

我把文件里所有的日文注释全都删掉了，然后再回到setup.py的位置，

```
$ python setup.py install
$ python -c "import editdistance"
```

好的，搞定了~](https://so.csdn.net/so/search/s.do?q=编辑距离&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)




