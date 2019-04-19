# Qt中layout()->setSizeConstraint(QLayout::SetFixedSize);崩溃的问题 - fanyun的博客 - CSDN博客
2017年11月30日 20:55:28[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：456标签：[qt](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/fanyun_01/article/category/6429937)
                
使用调试器调试找到问题代码： layout()->setSizeConstraint(QLayout::SetFixedSize);
发现 layout()这里是0，也就是没有获取到布局。明明有布局怎么会没有获取到？
回到qtcreator ，意外发现有一项是设置整个对话框窗口的布局(选中对画框，从 Lay out菜单中选择 Lay Out in a Grid)
重新编译，问题解决。
            
