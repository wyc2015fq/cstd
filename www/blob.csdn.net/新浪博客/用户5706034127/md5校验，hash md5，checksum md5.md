# md5校验，hash md5，checksum md5_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
			转载：http://blog.csdn.net/linda1000/article/details/17581035

[Linux](http://lib.csdn.net/base/linux)下校验文件MD5值，最简单的方法就是执行md5sum命令
md5sum filename
原本打算用subprocess调用系统命令来获取md5值，
**[python]**[view
plain](http://blog.csdn.net/linda1000/article/details/17581035#)[copy](http://blog.csdn.net/linda1000/article/details/17581035#)
[print](http://blog.csdn.net/linda1000/article/details/17581035#)[?](http://blog.csdn.net/linda1000/article/details/17581035#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/128693/fork)
- 
importsubprocess,shlex 
- 
cmd = "md5sum filename"
- 
p = subprocess(shlex.split(cmd), stdout=subprocess.PIPE) 
- 
printp.stdout.read() 
不过[Python](http://lib.csdn.net/base/python)有自带的MD5模块hashlib，用起来简单很多，
Python Hashlib模块的使用说明 [http://docs.python.org/2/library/hashlib.html](http://docs.python.org/2/library/hashlib.html)
fd = hashlib.md5() #获取一个MD5加密[算法](http://lib.csdn.net/base/datastructure)对象
fd.update("string") #指定需要加密的字符串
fd.hexdigest() #获取加密后的16进制字符串
实例
**[python]**[view
plain](http://blog.csdn.net/linda1000/article/details/17581035#)[copy](http://blog.csdn.net/linda1000/article/details/17581035#)
[print](http://blog.csdn.net/linda1000/article/details/17581035#)[?](http://blog.csdn.net/linda1000/article/details/17581035#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/128693/fork)
- 
#!/usr/bin/env python 
- 
#coding : utf-8 3 4 import sys 
- 
importhashlib 
- 
- 
defmd5sum(filename): 
- fd = open(filename,"r") 
- fcont = fd.r 
- fd.close() 
- fmd5 = hashlib.md5(fcont) 
- returnfmd5 
- 
- 
if__name__ == "__main__": 
- fmd5 = md5sum(sys.argv[1]) 
- printfmd5.hexdigest() 
其中fmd5 = hashlib.md5(fcont)等同于
fmd5 = hashlib.md5(fcont)
fmd5.update(fcont)
需要注意的是，传入 hashlib.md5() 的应该是 文件内容而不是文件名 ，这样才是对文件内容产生md5校验码；
另外，调用了 hashlib.md5() 后返回的是一个对象，想要获得linux下 md5sum 同样的效果，还要调用一下
hexdigest() 方法。
但是，这个方法有点过于粗暴，当检验大文件时，一次将所有文件内容读入内存，实在耗费较大，
网上给出实例[http://blog.csdn.net/shanliangliuxing/article/details/10115397](http://blog.csdn.net/shanliangliuxing/article/details/10115397)，
根据文件块长度，依次获取文件内容读入内存，通过update()逐次更新校验值，
**[python]**[view
plain](http://blog.csdn.net/linda1000/article/details/17581035#)[copy](http://blog.csdn.net/linda1000/article/details/17581035#)
[print](http://blog.csdn.net/linda1000/article/details/17581035#)[?](http://blog.csdn.net/linda1000/article/details/17581035#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/128693/fork)
- 
#!/usr/bin/env python 2
- 
- #coding : utf-8 3 import hashlib
- 
- defmd5hex(word): 
- """ MD5加密算法，返回32位小写16进制符号
- """
- ifisinstance(word, unicode): 
- word = word.encode("utf-8") 
- elifnotisinstance(word, str): 
- word = str(word) 
- m = hashlib.md5() 
- m.update(word) 
- returnm.hexdigest() 
- 
- defmd5sum(fname): 
- """ 计算文件的MD5值
- """
- defread_chunks(fh): 
- fh.seek(0) 
- chunk = fh.read(8096) 
- whilechunk: 
- yieldchunk 
- chunk = fh.read(8096) 
- else: #最后要将游标放回文件开头
- fh.seek(0) 
- m = hashlib.md5() 
- ifisinstance(fname, basestring) \ 
- andos.path.exists(fname): 
- with open(fname, "rb") as fh: 
- forchunk inread_chunks(fh): 
- m.update(chunk) 
- #上传的文件缓存 或 已打开的文件流
- eliffname.__class__.__name__ in["StringIO", "StringO"] \ 
- orisinstance(fname, file): 
- forchunk inread_chunks(fname): 
- m.update(chunk) 
- else: 
- return""40returnm.hexdigest()
 

