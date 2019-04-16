# MFC的图片按钮 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月16日 19:31:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：28
个人分类：[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)










利用CBitmapButton类实现位图按钮的两种方法 利用CBitmapButton类实现位图按钮的两种方法：
 方法一：
 1、在资源编辑的时候选中按钮的 Owner draw 属性，不需要选择 Bitmap 属性.
 按钮右键-->Styles-->Owner draw

 2、在程序中定义一个CBitmapButton成员变量。
 注意：不能使用ClassWizard为按钮映射一个CButton变量，然后改为CBitmapButton，
 这么做并不能将按钮直接映射为CBitmapButton类的对象，反而会出现初始化错误

 3、使用CBitmapButton::LoadBitmaps装载各种状态的图片,
 使用CBitmapButton::SubclassDlgItem关联到想要的按钮，
 使用CBitmapButton::SizeToContent函数使按钮适合图片大小。
 注意Loadbitmaps一定要在关联到按钮之前进行！

 附：添加资源Bitmap的方法：Insert菜单-->Resource -->Resource type:Bitmap-->Import,
 之后选择所需的图片就可以了

 方法一的例子：
 首先，选中按钮的owner Draw属性，在资源中加载两副*.bmp格式的位图，
 然后在对话框类中加入：



```cpp
CBitmapButton m_btnX1;//必须放在函数外面才能正常实现
BOOL CXXXXXXX::OnInitDialog()
{
CDialog::OnInitDialog();
m_btnX1.LoadBitmaps(IDB_XXXX_UP,IDB_XXXX_DOWN);//这里是两副位图，鼠标按下和弹起，
 // 也可以只加一个ID资源；
m_btnX1.SubclassDlgItem(IDC_DASEN_X1,this);
m_btnX1.SizeToContent();
}
```





方法二：使用CBitmapButton::AutoLoad函数关联到想要的按钮
 需要注意：
 A、使用CBitmapButton::AutoLoad函数之前不能使用CBitmapButton::LoadBitmaps装载
 各种状态的图片，否则会出错。
 B、AutoLoad函数完成关联和改变按钮大小的功能。类似于上面CBitmapButton::SubclassDlgItem
 和CBitmapButton::SizeToContent函数的功能。
 C、CBitmapButton::AutoLoad使用的位图是默认资源ID的，即它会自动装载相关资源位图。
 位图的资源ID格式为：
 "按钮Caption+U"、"按钮Caption+D"、"按钮Caption+F"、"按钮Caption+X"，
 分别代表Up、Down、Focus、Disable状态。
 如资源编辑时，希望关联的按钮的Caption为Test，
 那么其默认装载的位图资源的ID为："TestU"/"TestD"/"TestF"/"TestX"，
 注意分号""也是其ID的一部分。
 方法二例子：
 首先，选中Caption为："myBitmapButton"按钮的owner Draw属性
 其次，添加两个位图资源，并修改其ID分别为："myBitmapButtonU"和"myBitmapButtonD"，这里只添加两个位图
 然后在对话框类中加入：

```cpp
CBitmapButton m_btnX1;//必须放在函数外面才能正常实现
 BOOL CXXXXXXX::OnInitDialog()
 {
 CDialog::OnInitDialog();
 
 m_btnX1.AutoLoad(IDC_BUTTON1,this);//ID为IDC_BUTTON1的按钮的Caption为：myBitmapButton
 }
```



至此两种方法就都结束了。



原文地址：[http://yq1518602434.blog.163.com/blog/static/202937084201252011141376/](http://yq1518602434.blog.163.com/blog/static/202937084201252011141376/)




