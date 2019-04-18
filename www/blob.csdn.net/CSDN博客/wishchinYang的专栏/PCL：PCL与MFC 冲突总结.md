# PCL：PCL与MFC 冲突总结 - wishchinYang的专栏 - CSDN博客
2013年10月20日 13:26:53[wishchin](https://me.csdn.net/wishchin)阅读数：2414
#  (1):max,min问题
MFC程序过程中使用STL一些类编译出错，放到Console Application里一切正常。
比如：
```cpp
void CMyDialog::OnBnClickedButton1()
{
    double min=std::numeric_limits<double>::max();
 }
```
这一个问题在前一篇文章解决.
# (2):error C2661: “operator new” : 没有重载函数接受 3 个参数
对于mfc与pcl的各种冲突，例如其上。
解决方法为：[http://bbs.csdn.net/topics/330096920](http://bbs.csdn.net/topics/330096920)
    我的方法是直接注销了，dlg里面的#define new DEBUG_NEW
```cpp
#ifdef _DEBUG
//#define new DEBUG_NEW//wishchin 为消除pcl不能拿new三个参数！！！
#endif
```
(3):
.....................................................
