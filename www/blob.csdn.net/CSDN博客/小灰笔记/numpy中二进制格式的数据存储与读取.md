# numpy中二进制格式的数据存储与读取 - 小灰笔记 - CSDN博客





2017年04月09日 20:20:34[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：5717








       使用save可以实现对numpy数据的磁盘存储，存储的方式是二进制。查看使用说明，说明专门提到了是未经压缩的二进制形式。存储后的数据可以进行加载或者读取，通过使用load方法。

In [81]:np.save('demo',data1)

       通过以上操作，数据data1被存储到了demo文件中，numpy会自动加上npy的文件后缀名。

In [82]: a =np.load('demo.npy')



In [83]: a

Out[83]: array([0,1, 2, 3, 4, 5, 6, 7, 8, 9])

       通过上面的结果可以看出加载成功，而a获取了原来data1的数据对象信息。进行相等判断可以看出，两者等价。

In [84]: a ==data1

Out[84]: array([True,  True,  True, True,  True,  True, True,  True,  True, True], dtype=bool)

       针对后缀名的自动追加，再次测试一下，如果参数中加上了npy的扩展名是否还会额外再增加一个？

In [85]:np.save('demo.npy',data1)

       通过以上命令操作得出结论：如果在参数中加上了扩展名，扩展名将不会再次扩充。使用简单的编辑器打开二进制文件看看是否可读，具体的信息如下：


![](https://img-blog.csdn.net/20170409202010648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


