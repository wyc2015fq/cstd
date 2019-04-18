# python3：语法变动 及新特性 - wishchinYang的专栏 - CSDN博客
2013年08月06日 20:12:17[wishchin](https://me.csdn.net/wishchin)阅读数：612
****python3.0 对python2.x 升级后重大语法变动**，幸好留下2.7.6及后续2版本，保持一些语法兼容。**
**原始地址：[http://hi.baidu.com/jxq61/item/3a24883189f31f84c3cf298d](http://hi.baidu.com/jxq61/item/3a24883189f31f84c3cf298d)**
**python3.0 对python2.x 升级后重大语法变动**
python 2.4 与 python 3.0 的比较
一、 print 从语句变为函数
原:     print   1, 2+3
改为: print ( 1, 2+3 )
在python3.0中，print成为了一个函数，将传入的参数一输出。
Python3.0：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/1.PNG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/1.PNG)
Python 2.5：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/2.PNG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/2.PNG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/2.PNG)
二、range 与 xrange
现在，range的实现和xrange一样了，xrange似乎被去掉了。
Python3.0：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/13.PNG)
Python2.5：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/14.PNG)
原 : **range( 0, 4 )**   结果 是 列表 [0,1,2,3 ]
改为：**list( range(0,4) )**
原 : **xrange( 0, 4 )**    适用于 for 循环的变量控制
改为：**range(0,4) **
三.字符串
原: **字符串以 8-bit 字符串**存储
改为: **字符串以 16-bit Unicode 字符串**存储
使用比较操作符时，如果被比较的对象没有有意义的自然序，比较操作会抛出TypeError异常。比如：
Python3.0：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/3.PNG)
Python2.5：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/4.PNG)
四.  cmp（）函数不再存在，__cmp__()已经不支持了，被一些比较函数替代，比如__lt__(),__eq__()等。如果需要使用cmp（），可以使用（a>b）-（a<b）替代。
注意下图中的变化
Python3.0：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/5.PNG)
Python2.5
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/6.PNG)
五． long不存在了，被int代替，但是效果与long一样
Python3.0：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/7.PNG)
Python2.5：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/8.PNG)
六、try except 语句的变化
原: **try:**
**          ......**
**     except    Exception, e :**
**         ......**
改为
**try:**
**          ......**
**     except    Exception as e :**
**         ......**
**七、打开文件  键盘输入**
原： **file( ..... )**
**    或 open(.....)**
改为：
**只能用 open(.....)**
从键盘录入一个字符串
原: **raw_input( "提示信息" )**
改为: **input( "提示信息" )**
**八、bytes 数据类型 **
八进制数的表示方式改变了：
Python3.0:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/11.PNG)
Python 2.5:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/12.PNG)

A bytes object is an immutable array. The items are 8-bit bytes, represented by integers in the range 0 <= x < 256.
bytes 可以看成是“字节数组”对象，每个元素是 8-bit 的字节，取值范围 0~255。
由于在 python 3.0中字符串以 unicode 编码存储，当写入二进制文件时，字符串无法直接写入（或读取），必须以某种方式的编码为字节序列后，方可写入。
（一）字符串编码(encode) 为 bytes
例:  ** s = "张三abc12"**
**       b = s.encode( 编码方式)**
       # b 就是 bytes 类型的数据
      # 常用的编码方式为 ： "uft-16"    , "utf-8", "gbk", "gb2312", "ascii" , "latin1" 等
      # 注 ： 当字符串不能编码为指定的“编码方式”时，会引发异常
(二) bytes 解码(decode)为字符串
**s = "张三abc12"**
**       b = s.encode( "gbk")    # 字符串 s 编码为 gbk 格式的字节序列**
** s1 = b.decode("gbk")   # 将字节序列 b以gbk格式 解码为字符串 **
       # 说明，当字节序列不能以指定的编码格式解码时会引发异常
(三）使用方法举例
**************************************
**#coding=gbk **
**f = open("c:\\1234.txt", "wb")s = "张三李四abcd1234"# -------------------------------# 在 python2.4 中我们可以这样写：# f.write( s )# 但在 python 3.0中会引发异常# -------------------------------b = s.encode("gbk")f.write( b )f.close()**
**input("?")**
读取该文件的例子:
**#coding=gbk **
**f = open("c:\\1234.txt", "rb")f.seek(0,2) #定位至文件尾n = f.tell() #读取文件的字节数f.seek(0,0) #重新定位至文件开始处b = f.read( n )# ------------------------------# 在 python 2.4 中 b 是字符串类型# 要 python 3.0 中 b 是 bytes 类型# 因此需要按指定的编码方式确码# ------------------------------ s = b.decode("gbk")print ( s )# ------------------------------# 在 python 2.4 中 可以写作 print s 或 print ( s )# 要 python 3.0 中 必须写作 print ( s )# ------------------------------ f.close()input("?")**
运行后应显示:
**张三李四abcd1234**
(四) bytes序列，一但形成，其内容是不可变的
例：
**s="ABCD"**
**b=s.encode("gbk")**
**print b[0]       # 显示   65**
**b[0] = 66    **
# **执行该句，出现异常: 'bytes' object does not support item assignment**
**九、 chr( K ) 与 ord( c )**
python 2.4.2以前
**chr( K )   将编码K 转为字符，K的范围是 0 ~ 255**
**   ord( c )   取单个字符的编码, 返回值的范围: 0 ~ 255**
python 3.0
**chr( K )   将编码K 转为字符，K的范围是 0 ~ 65535**
**   ord( c )   取单个字符的编码, 返回值的范围: 0 ~ 65535**
十、 除法运算符(是一个相当大的改进呢！！！)
python 2.4.2以前
**10/3      结果为 3      **
python 3.0
**10 / 3 结果为 3.3333333333333335**
**   10 // 3 结果为 3**
**十一：dict的函数，keys（），items（），values（），返回的值不再是list，而是view。所以dict.iterkeys(),dict.iteritems()和dict.itervalues()被去掉了。**
十二、字节数组对象 --- 新增
(一） 初始化
**    a = bytearray(   10 )**
     # a 是一个由十个字节组成的数组，其每个元素是一个字节，类型借用 int
     # 此时，每个元素初始值为 0
（二） 字节数组 是可变的
**    a = bytearray(   10 )**
**a[0] = 25**
     # 可以用赋值语句更改其元素，但所赋的值必须在 0 ~ 255 之间
(三)   字节数组的切片仍是字节数组
(四)   字符串转化为字节数组
**#coding=gbk**
**     s ="你好"**
**     b = s.encode( "gbk")     # 先将字符串按某种“GBK”编码方式转化为 bytes**
**     c = bytearray( b )          #再将 bytes 转化为 字节数组**
**     也可以写作**
**     c = bytearray( "你好", "gbk")**
(五)   字节数组转化为字符串
**c = bytearray( 4 )**
**       c[0] = 65 ; c[1]=66; c[2]= 67; c[3]= 68**
**      s = c.decode( "gbk" )**
**       print ( s )**
      # 应显示： ABCD           
**(六) 字节数组可用于写入文本文件**
**#coding=gbk **
**f = open("c:\\1234.txt", "wb")s = "张三李四abcd1234"# -------------------------------# 在 python2.4 中我们可以这样写：# f.write( s )# 但在 python 3.0中会引发异常# -------------------------------b = s.encode("gbk")**
**f.write( b )c=bytearray( "王五","gbk")f.write( c )f.close()**
**input("?")**
[#Python](http://hi.baidu.com/tag/Python/feeds)
tips.  在python3.0中，sys.maxint不存在了，因为int的大小不再受到限制，但是sys.maxsize还存在。
Python3.0：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/9.PNG)
Python2.5：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/10.PNG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/lokibalder/EntryImages/20081205/10.PNG)
