# WCF序列化 - 左直拳的马桶_日用桶 - CSDN博客
2014年07月07日 21:41:18[左直拳](https://me.csdn.net/leftfist)阅读数：1656
在WCF里面，不声明数据成员特性"[DataMember]"其实也是可以的，比如：
在WCF里面：
```
[DataContract]
    public class User
    {
        public int Id { get; set; }
        public string Name { get; set; }
        [DataMember]
        public string Description;
    }
    public class User2
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public string Description;
    }
```
客户端：
```
using (ServiceReference1.Service1Client client = new ServiceReference1.Service1Client())
            {
                ServiceReference1.User user = client.GetUser();
                ServiceReference1.User2 user2 = client.GetUser2();
                MessageBox.Show(user.Description);
                MessageBox.Show(user2.Name);
            }
```
代码没什么问题，可以正常运行。
那么，这个数据成员特性究竟有什么意义呢？难道只控制某些属性的是否可见？不懂。
参考文章：
[http://www.tuicool.com/articles/NZFr6vu](http://www.tuicool.com/articles/NZFr6vu)
