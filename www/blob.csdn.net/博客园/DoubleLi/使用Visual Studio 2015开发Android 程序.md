# 使用Visual Studio 2015开发Android 程序 - DoubleLi - 博客园






**环境配置：**

操作系统：win 7 64位

IDE：Visual Studio 2015

SDK：installer_r24.3.3-windows

**安装前提：**

编辑hosts文件（在附件可下载）因为安装过程中要联网更新和注册

安装完成VS之后直接新建android程序会提示：
|---------------------------Microsoft Visual Studio---------------------------值不能为 null。参数名: path1---------------------------确定   ---------------------------|
|----|

那是因为VS没有配置android的SDK，接下来我们就设置。

## 第一步：更新android SDK

自行百度并安装installer_r24.3.3-windows.exe，然后打开安装路径下的SDK Manager选择一个安卓版本更新，比如4.1.2,可以根据需要将其他版本对勾去掉。

然后等待更新完毕：

![wps52D1.tmp](https://images0.cnblogs.com/blog/16765/201507/232023229597513.jpg)

然后打开AVD Manager创建一个虚拟机：

![wps52E2.tmp](https://images0.cnblogs.com/blog/16765/201507/232023238492842.jpg)

点击右边的Start启动看看能不能起起来。

## 第二步：新建android项目：

![wps52F2.tmp](https://images0.cnblogs.com/blog/16765/201507/232023246312969.jpg)

然后会要求你登陆：

![wps5303.tmp](https://images0.cnblogs.com/blog/16765/201507/232023256931311.jpg)

需要先注册，然后登陆。

然后依次点开资源管理器，找到布局文件：

![wps5304.tmp](https://images0.cnblogs.com/blog/16765/201507/232023265379652.jpg)

双击打开设计界面：

工具箱上面已经内置了很多控件：

![wps5314.tmp](https://images0.cnblogs.com/blog/16765/201507/232023277093195.jpg)

这里无所谓了，喜欢拖就拖，不喜欢就自己写布局代码，咱们完成一个登陆界面：

![wps5325.tmp](https://images0.cnblogs.com/blog/16765/201507/232023285378308.jpg)

完整代码如：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:orientation="vertical"
    android:layout_margin="5dip">
    <TextView
        android:id="@+id/form_title"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="初始用户名和密码都是123" />
    <LinearLayout
        android:id="@+id/layout_login_name"
        android:layout_width="fill_parent"
        android:layout_height="wrap_content"
        android:layout_margin="5.0dip"
        android:layout_marginTop="10.0dip"
        android:orientation="horizontal">
        <TextView
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="登录名：" />
        <EditText
            android:id="@+id/txt_login_name"
            android:layout_width="fill_parent"
            android:layout_height="wrap_content"
            android:textSize="15.0sp" />
    </LinearLayout>
    <LinearLayout
        android:id="@+id/login_pwd_layout"
        android:layout_width="fill_parent"
        android:layout_height="wrap_content"
        android:layout_below="@id/layout_login_name"
        android:layout_centerHorizontal="true"
        android:layout_margin="5.0dip"
        android:orientation="horizontal">
        <TextView
            android:id="@+id/login_pass_edit"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="密  码："
            android:textSize="15.0sp" />
        <EditText
            android:id="@+id/txt_login_pwd"
            android:layout_width="fill_parent"
            android:layout_height="wrap_content"
            android:password="true"
            android:textSize="15.0sp" />
    </LinearLayout>
    <Button
        android:id="@+id/btn_login"
        android:layout_width="fill_parent"
        android:layout_height="wrap_content"
        android:layout_gravity="center"
        android:gravity="center"
        android:onClick="btn_click"
        android:text="登陆" />
</LinearLayout>

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

这些代码稍微一用力就能看明白。

打开MainActivity 编辑代码如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

protected override void OnCreate(Bundle bundle)
  {
     base.OnCreate(bundle);
    // Set our view from the "main" layout resource
    SetContentView(Resource.Layout.Main);
    // Get our button from the layout resource,  form_title
    // and attach an event to it
            Button button = FindViewById<Button>(Resource.Id.btn_login);
            EditText txtLoginName = FindViewById<EditText>(Resource.Id.txt_login_name);
            EditText txtLoginPwd = FindViewById<EditText>(Resource.Id.txt_login_pwd);
            TextView txtMsg = FindViewById<TextView>(Resource.Id.form_title);
            button.Click += delegate {
                                        string loginName = txtLoginName.Text;
                                        string loginPwd = txtLoginPwd.Text;
                                        if (loginName == loginPwd&& loginName == "123")
                                           {
                                              txtMsg.Text = "登陆成功！";
                                           }
                                    };
  }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

含义很简单，就是找到控件，取值，赋值，控件的ID在布局中定义@+id/后面的就是。

智能提示不灵光，暂时忍忍吧。

然后启动，按F5，如果想查看详细信息或者运行中异常，请依次打开logcat：

![wps5326.tmp](https://images0.cnblogs.com/blog/16765/201507/232023296625865.jpg)

将输出控制台拉大：

![wps5337.tmp](https://images0.cnblogs.com/blog/16765/201507/232023306466463.jpg)

以后在运行中如果奔溃，可以在这里找到详细信息。

在虚拟机中进入控制面板：

![wps5347.tmp](https://images0.cnblogs.com/blog/16765/201507/232023318031479.jpg)

启动它，输入信息：

![wps5348.tmp](https://images0.cnblogs.com/blog/16765/201507/232023328183834.jpg)

点击登录：

![wps5359.tmp](https://images0.cnblogs.com/blog/16765/201507/232023338039133.jpg)

## 第三步：部署app

经过第二步大家可以在debug目录下找到apk安装文件：

![wps5369.tmp](https://images0.cnblogs.com/blog/16765/201507/232023348498947.jpg)

然后一激动就复制到手机中，结果发现根本用不了。

原因是VS中开发的apk需要发布才能安装使用，发布按钮就在

![wps536A.tmp](https://images0.cnblogs.com/blog/16765/201507/232023355997318.jpg)

目前是灰的，需要将调试模式改为release才可用：

![wps537B.tmp](https://images0.cnblogs.com/blog/16765/201507/232023363654217.jpg)

然后会出现发布向导：

![wps537C.tmp](https://images0.cnblogs.com/blog/16765/201507/232023374124031.jpg)

这里您请随意！

![wps538D.tmp](https://images0.cnblogs.com/blog/16765/201507/232023382875130.jpg)

然后继续：

![wps539D.tmp](https://images0.cnblogs.com/blog/16765/201507/232023391626229.jpg)

记住上面的路径，一会就在这里找安装用APK文件。

然后等黑屏闪2下，就出现了这个期待的文件：

![wps53AE.tmp](https://images0.cnblogs.com/blog/16765/201507/232023399902343.jpg)

复制到手机中，安装后，开始得瑟吧！

![wps53BE.tmp](https://images0.cnblogs.com/blog/16765/201507/232023425377898.png)

host[下载](http://files.cnblogs.com/files/madyina/20150722hosts.rar)

源码[下载](http://pan.baidu.com/s/1kTELbSj)

from:https://www.cnblogs.com/madyina/archive/2015/07/23/4671708.html









