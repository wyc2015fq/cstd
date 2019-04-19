# 调用线程必须为 STA,因为许多 UI 组件都需要 - 左直拳的马桶_日用桶 - CSDN博客
2014年12月05日 14:35:28[左直拳](https://me.csdn.net/leftfist)阅读数：10995
个人分类：[WPF																[.NET](https://blog.csdn.net/leftfist/article/category/94497)](https://blog.csdn.net/leftfist/article/category/2441177)
WPF中，代码中准备控制控件内容时，有时会报错：
调用线程必须为 STA,因为许多 UI 组件都需要
我知道，在winform下面，使用多线程时，控件的值读取是可以的，但如果要更改，那么就必须进行一些特别的处理。难道在WPF中，也一样吗？
首先，WPF中，当报上面那个错误时，代码可以改成类似以下：
```
void authOK(AuthorizationResult auth)
        {
            App.Current.Dispatcher.Invoke((Action)(() =>
            {
                labInfo.Content = "登录成功";
                btnLogin.IsEnabled = true;
                labUser.Content = auth.User.Name;
                labToken.Content = auth.AccessToken;
                loginWin.Close();
                client.Close();
            }));
        }
```
Dispatcher是一个线程控制器，反正你要控制线程里跑的东西，就要经过它。那么WPF里面，有个所谓UI线程，后台代码不能直接操作UI控件，需要的话，就要通过这个Dispatcher。
我的理解如此，未作深究，对WPF没有多大兴趣。人傻，就是这么任性！
