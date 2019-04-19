# WPF学习笔记：MVVM模式下，ViewModel如何关闭View？ - 左直拳的马桶_日用桶 - CSDN博客
2014年06月20日 22:29:19[左直拳](https://me.csdn.net/leftfist)阅读数：9257
矫枉过正，从一个极端走向另一个极端。MVVM模式，View只负责呈现，虽然也有后台代码，但基本上就是摆设，VM接管了一切的逻辑处理。
那么，现在，大能的VM已经完成了所有的事情，这个窗口V如何才能自动关闭呢？
据我目前少得可怜的WPF知识可知，有两种方案：
**一、利用View里的IsEnable属性**。
原理是这样的：
1、UI中的IsEnabled绑定VM中的属性
2、UI的后台代码中，注册IsEnableChange事件，在这个事件里，检测到传过来的值满足某个条件，即可触发Close()命令
如此，VM控制自己那个属性就能达到关闭V的目的了。
没代码我说个J8：
UI
```
<Window
    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
	……
    IsEnabled="{Binding IsLoginFailed, Mode=OneWay}">
```
UI后台代码
```
public LoginWindow()
        {
            InitializeComponent();
            this.IsEnabledChanged += LoginWindow_IsEnabledChanged;
        }
        private void LoginWindow_IsEnabledChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if ((bool)e.NewValue == false)
            {
                this.Close();
            }
        }
```
VM
```
private bool isLoginFailed = true;
        /// <summary>
        /// <para>获取或设置一个表示是否登录失败的值；true表示登录失败，否则为false。</para>
        /// <para>与LoginWindow的IsEnable属性绑定。当为False时，关闭Login Window。</para>
        /// 当该属性更改时通知客户端。 
        /// </summary>
        public bool IsLoginFailed
        {
            get
            {
                return this.isLoginFailed;
            }
            set
            {
                if (this.isLoginFailed != value)
                {
                    this.isLoginFailed = value;
                    this.RaisePropertyChanged("IsLoginFailed");
                }
            }
        }
```
二、用Invoke
1、UI后台代码：
```
public LoginWindow()
        {
            InitializeComponent();
            this.DataContext = new LoginWindowVM(this.Close);
        }
```
2、VM代码：
```
Action _closeAction;
        public LoginWindowVM(Action closeAction)
        {
            InitializeLoginInfo();
            this._closeAction = closeAction;
        }
        void CloseWin()
        {
            App.Current.Dispatcher.Invoke((Action)(() =>
            {
                this._closeAction.Invoke();
            }));
        }
```
两种方法，无疑第一种比较符合MVVM的思想。VM发出关闭的信号，触发V进行关闭，如何关闭，何时关闭，都由V自己控制；
而方案二，通过调用的方式来直接进行关闭，好像有点越俎代庖？
但是，IsEnable本意是用来控制失效和使能的，却被用于关闭UI，反而失去了控制失效使能的作用，有时会不方便。
方案二参考文章：
[http://bbs.csdn.net/topics/360085548](http://bbs.csdn.net/topics/360085548)
