# 抛弃MFC, 转向.Net - 逍遥剑客 - CSDN博客
2008年11月02日 00:49:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2705标签：[mfc																[c#																[.net																[object																[qt](https://so.csdn.net/so/search/s.do?q=qt&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=.net&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[工具开发](https://blog.csdn.net/xoyojank/article/category/1858383)
 现在我们的工具全是MFC的, 维护起来那叫一个郁闷
一直在找一个C++中好用的UI库, 看过Qt(看起来到不错, 懒得学), wxWidgets(跟MFC一个样), 剩下的就不说了......
前段时间发现了Xtreme ToolkitPro, 里面那些Samples很强大, 也有过用它来做界面的冲动
但是想想, 这东西还是基于MFC, 一样还是要手动写........
很怀念大学时在C#里整得那些东西, 做界面超级方便
但是, 要在C#里用C++的东西, 还要中间再加一层, 隔着衣服摸起来怎么会爽~~~
后来跟朋友讨论一番, 发现C++/CLI可以一用
语法已经比托管C++看起来爽多了, 至少那一堆下划线已经消失了
出去溜冰回来就直接写了个小程序试了试, 嗯嗯, 终于找到当年写Java和C#的感觉了!
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081102/CLR_FileExplorer.JPG)
看起来还不错吧? 第一次用CLR, 花了几个小时就弄出来了, .Net的库还是很好用的, 基本上你需要什么功能一查就能找到, MSDN里也都有示例. 只是为了体验一下新的编程习惯, 所以还没有嵌入NativeC++的代码. 
总结一下新的东西:
1. CLR里的指针不叫指针, 叫"句柄", 使用gcnew而不是new创建. 如:
- DirectoryInfo^ disk = gcnew DirectoryInfo(drive->Name);
2. 句柄相当于一个智能指针, 所以我们不用太关心它们什么时候释放(.Net的公有特征)
3. 对象的"属性"不是通过get/set方法来读写的, 而是通过访问器来的(跟C#一样)
4. for each很好用.....
- for each(FileInfo^ file in curDir->GetFiles())
-                  {
-                      ListViewItem^ item = gcnew ListViewItem(file->Name, 3);
-                      item->SubItems->Add(Convert::ToString(file->Length));
- this->listViewFile->Items->Add(item);
-                  }
- 
5. 多了个委托(还是跟C#一个样)
6. class在定义的时候多了个ref关键字
7. 发现static_cast用来做类型转换没啥用, 因为所有的对象都是Object的子类(囧)
