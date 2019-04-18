# 读取txt文件的简易算法 - weixin_33985507的博客 - CSDN博客
2013年08月13日 11:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
网友在问，从一个文本文件(txt)读取数据，并做简易算法。网友的原问题大约如下，
![](https://images0.cnblogs.com/blog/28036/201308/13095208-7bb30ffbeac44eb48b42101c183abb41.png)
网友的问题，虽然说是全部是数字，但没有说明是否只有一行。因此Insus.NET在实现算法时，处理文本文件是否多行，是否为全部数字，是否带在空行，某一行是否只有一个数字，分隔符是否在前面，或是在后面。
因此Insus.NET提供上面所提及条件因素的文本文件作为算法对象：
![](https://images0.cnblogs.com/blog/28036/201308/13101605-53dc3b5394454e6d80b409f762c2b045.png)
OK，Insus.NET还是较喜欢创建对象来处理这些问题，由于文本文件是以行来存储，因此就把行作为对象来处理。逻辑处理：
![](https://images0.cnblogs.com/blog/28036/201308/13103513-6ec3649caf4a43429558f8d13c5defce.png)
接下来，Insus.NET写一个数字的类别，就是用来处理每行中每个分隔符(,)之间的数字。
![](https://images0.cnblogs.com/blog/28036/201308/13103804-d6ea46785a3d47e2b16c041222e8ab43.png)
还要创建一个类别，是获取所有符合条件的数字。
![](https://images0.cnblogs.com/blog/28036/201308/13105200-d0c6b05b6b464b93a83af24fcb55200d.png)
把结果输出网页看看，在输出结果之前，还是写程序，读取文本文件，获取每一行，处理每行中的分隔符之间的数。最后得到List<Number>集合之后，转换为int[] arr也是方便之极。
![](https://images0.cnblogs.com/blog/28036/201308/13105735-2062fe0853c746d2b062f60d481ef107.png)
结果：
![](https://images0.cnblogs.com/blog/28036/201308/13110011-fa32e375876848fb818e8d6e6b38e1bb.png)
