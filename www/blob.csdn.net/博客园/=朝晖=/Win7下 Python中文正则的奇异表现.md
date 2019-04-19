# Win7下 Python中文正则的奇异表现 - =朝晖= - 博客园
# [Win7下 Python中文正则的奇异表现](https://www.cnblogs.com/dhcn/p/7106487.html)
首先贴Win32下的直接[Python](http://lib.csdn.net/base/python) shell下的[测试](http://lib.csdn.net/base/softwaretest)结果：
```
Python 2.7.6 (default, Nov 10 2013, 19:24:18) [MSC v.1500 32 bit (Intel)] on win32  
Type "copyright", "credits" or "license()" for more information.  
>>> import re  
>>> print re.match(ur'[\u4e00-\u9fa5]+',u"中国")  
None  
>>>
```
```
>>> import sys  
>>> sys.getdefaultencoding()  
'ascii'
```
```
>>> str1=u"中国"  
>>> str1  
u'\xd6\xd0\xb9\xfa'
```
接着来先cmd，再python命令进入python交互环境下的结果：
```
Microsoft Windows [版本 6.1.7601]  
版权所有 (c) 2009 Microsoft Corporation。保留所有权利。  
  
C:\Users\user>python  
Python 2.7.6 (default, Nov 10 2013, 19:24:18) [MSC v.1500 32 bit (Intel)] on win32  
Type "help", "copyright", "credits" or "license" for more information.  
>>> import re  
>>> print re.match(ur'[\u4e00-\u9fa5]+',u"中国")  
<_sre.SRE_Match object at 0x00522C28>  
>>> import sys  
>>> sys.getdefaultencoding()  
'ascii'
```
```
>>> str1=u"中国"  
>>> str1  
u'\u4e2d\u56fd'
```
生产应用中的问题好像是因为codeing设置的添加而解决，和上面这个问题有一定关系，以上形式是交互环境下测试所得的结果。
参考资料：http://www.limijd.me/2013/02/[python](http://lib.csdn.net/base/python)%E9%87%8Cunicode%E5%92%8Cutf-8%E7%9A%84%E5%8C%BA%E5%88%AB/

