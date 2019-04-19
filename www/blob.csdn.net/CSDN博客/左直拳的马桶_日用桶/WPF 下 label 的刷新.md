# WPF 下 label 的刷新 - 左直拳的马桶_日用桶 - CSDN博客
2014年12月05日 14:00:47[左直拳](https://me.csdn.net/leftfist)阅读数：3923
WPF下，label控件并没有什么 Refresh() 的方法。那么现在问题就来了。
假设有这么个场景：WPF窗体上有一个按钮，一个Label，按下按钮，触发一些耗时的操作；在操作之前，Label显示一个信息，说现在开始要做什么什么东西了。
结果，按钮按下去以后，系统会处于忙碌状态，而Label该先显示的内容，却没有显示，一直等到耗时操作完毕，才突然显示。
这不科学呀，完全失去了提示的意义。明明是 label.Content = "努力与服务器连接中，请稍候。。。"; 先执行的，怎么结果姗姗来迟？
原因不去深究了。我对WPF也不感兴趣。WPF就是这么任性。解决办法是将耗时操作放到一个新线程里执行：
```
LoginWindow loginWin;
        SocketIOClient client;
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            labInfo.Content = "努力与服务器连接中，请稍候。。。";
            btnLogin.IsEnabled = false;
            Thread thr = null;
            thr = new Thread(new ThreadStart(BuildConnection));
            thr.Priority = ThreadPriority.Lowest;
            thr.IsBackground = true;
            thr.Start();
        }
        void BuildConnection()
        {
            client = new SocketIOClient();
            client.OnConnected += login;
            client.OnAuthOK += authOK;
            client.Execute();
        }
```
