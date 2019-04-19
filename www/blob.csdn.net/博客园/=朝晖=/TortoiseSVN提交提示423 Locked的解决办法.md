# TortoiseSVN提交提示423 Locked的解决办法 - =朝晖= - 博客园
# [TortoiseSVN提交提示423 Locked的解决办法](https://www.cnblogs.com/dhcn/p/7100271.html)
    此办法是阅读官方文档(TortoiseSVN-1.6.16-zh_CN.pdf) 4.21 锁部分提供的办法：
     首先选择选择要提交的文件右键菜单：“TortoiseSVN->get locks”在对话框的左下角有个steal the locks选项，勾选这个选项，你就可以窃取别人的锁。然后你就成为锁的拥有者，TortoiseSVN->release locks,释放锁，然后commit提交即可。

