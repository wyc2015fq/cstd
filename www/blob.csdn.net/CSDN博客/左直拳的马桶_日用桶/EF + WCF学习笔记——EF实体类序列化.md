# EF + WCF学习笔记——EF实体类序列化 - 左直拳的马桶_日用桶 - CSDN博客
2014年05月27日 11:47:45[左直拳](https://me.csdn.net/leftfist)阅读数：6154
项目中如果 EF + WCF 结合使用，模式应该是EF负责读取数据库，传递数据对象给WCF，WCF再将这些对象传送给客户端。因为WCF传送的对象需要序列化，而EF默认生成的对象并没有序列化，很可能会出现EF传过来的类对象，WCF里面要再定义一次，加上序列化特性。属性明明一样，但还是需要转换：遍历、赋值，太浪费了。
一种解决的方案是，让EF生成的序列化的实体类。
这样就需要修改EF的模板。
1、在VS2012中，首先我们要下载一个模板
# EF 5.x DbContext Generator with WCF Support
[http://visualstudiogallery.msdn.microsoft.com/79a5b6df-0ac9-49ac-82a0-413817bd97f4](http://visualstudiogallery.msdn.microsoft.com/79a5b6df-0ac9-49ac-82a0-413817bd97f4)
# EF 6.x DbContext Generator with WCF Support
http://visualstudiogallery.msdn.microsoft.com/fad56775-1738-407d-8d9c-73140474aa34
2、打开EF项目中的.edmx文件，在关系图空白的地方右键，选“添加代码生成项”
然后添加我们下载的模板
系统这时会让我们输入模板名称，默认是Module1.tt。注意要改成我们现有的*.tt，覆盖原来，不然的话，系统会顽固地采用原来的模板。
3、保存.edmx后，发现新生成的实体类，全部序列化矣。
-----------------------------------------------------------------------------------------------
有关WCF的序列化，只有需要传递给客户端的属性，才需要序列化。比如
```
public int Id { get; set; }
        [DataMember]
        public string Name { get; set; }
```
那么在客户端，只能看到属性Name，而看不到Id
