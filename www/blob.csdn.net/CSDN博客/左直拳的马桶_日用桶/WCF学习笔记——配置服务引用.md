# WCF学习笔记——配置服务引用 - 左直拳的马桶_日用桶 - CSDN博客
2014年05月23日 14:59:03[左直拳](https://me.csdn.net/leftfist)阅读数：2214
个人分类：[.NET																[WCF](https://blog.csdn.net/leftfist/article/category/2441179)](https://blog.csdn.net/leftfist/article/category/94497)
WCF传过来的东西要序列化。
比如，在WCF服务中，象这么个方法
`public IEnumerable<UserItem> GetUserList()`
默认情况下，在客户端会调用，是这样：
```
//ServiceReference1，服务引用            
	using (ServiceReference1.UserListClient client = new ServiceReference1.UserListClient())
        {
            ServiceReference1.UserItem[] list = client.GetUserList();
        }
```
但是我们在WPF中，多用 ObservableCollection，那么如何转换？
当然可以在代码中加以转换，但更省事的是配置服务引用：
在VS中，右击这个服务引用，选配置服务引用，然后
集合类型：System.Collections.ObjectModel.ObservableCollection
字典集合类型：System.Collections.Generic.Dictionary
如此，WCF服务中的IEnumerable，在客户端，即可自动换成 ObservableCollection 矣
