
# mac 安装pynlp , 解决了各种安装问题 - 追求卓越,做到专业 - CSDN博客


2019年03月02日 17:08:45[Waldenz](https://me.csdn.net/enter89)阅读数：35标签：[mac pyltp																](https://so.csdn.net/so/search/s.do?q=mac pyltp&t=blog)[mac 																](https://so.csdn.net/so/search/s.do?q=mac &t=blog)[pyltp 																](https://so.csdn.net/so/search/s.do?q=pyltp &t=blog)[ltp																](https://so.csdn.net/so/search/s.do?q=ltp&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=pyltp &t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=mac &t=blog)个人分类：[LTP																](https://blog.csdn.net/enter89/article/category/8594498)
[
																								](https://so.csdn.net/so/search/s.do?q=mac &t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=mac pyltp&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=mac pyltp&t=blog)

首先放弃 pip 直接安装，使用源码安装，下载源码：
```python
$ git clone https://github.com/HIT-SCIR/pyltp
```
重点来了，下载下pyltp的源码后，进入到pyltp目录下， 然后执行
```python
$ python setup.py install
```
会直接产生如下错误，找不到.h的文件。 查看pyltp目录下的ltp文件夹会发现，这个文件夹是空的。
![](https://img-blog.csdnimg.cn/20190302164942154.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
因为 pyltp/ltp这个目录是空的，ltp 是用c++写的，.h的文件都在ltp文件夹中。所以需要重新下载ltp。
进入pyltp目录，把ltp文件夹删除，然后再重新下载ltp的源码。
```python
$ cd pyltp
$ ls 
$ rm -rf ltp
$ git clone https://github.com/HIT-SCIR/ltp.git  //重新下载ltp的源码
```
然后再重新执行
```python
$ python setup.py install
```
执行了好久后，完成！
![](https://img-blog.csdnimg.cn/20190302170729806.png)


