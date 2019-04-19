# 参数化查询***需要参数@start，但为提供该参数 - Jun5203 - CSDN博客
2019年03月29日 19:16:46[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：116
所属专栏：[C#](https://blog.csdn.net/column/details/29824.html)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190329191352321.png)
出现这个错误的原因：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190329191409557.png)
因为参数的值定义为null，这样传入数据库是不识别的。null是C#的，null传入数据库中必须变成NbNull.Value，NbNull.Value才是数据库中的null。
如果给参数@start赋值为null，相当于没赋。
解决方法：
```
private string start="";
private string end="";
```
