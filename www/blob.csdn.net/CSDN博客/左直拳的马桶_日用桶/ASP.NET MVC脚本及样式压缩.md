# ASP.NET MVC脚本及样式压缩 - 左直拳的马桶_日用桶 - CSDN博客
2014年02月14日 21:51:03[左直拳](https://me.csdn.net/leftfist)阅读数：2366
现在我用ASP.NET MVC4.0，发现它自带有脚本和样式压缩功能。不知道以前的版本有木有，没有深究。太棒了！以前我们还辛辛苦苦自己搞了一个压缩的东西。这再次说明，平庸程序员如我辈，应该把时间和精力花在学习新知识、了解业界新动向上，而不要埋头去搞什么通用的类库、模块之类的东西，还没搞出来已经落后了。
ASP.NET MVC4.0 自带的压缩功能是这样用的：
1、在app_start/bundleconfig.cs里设置键，如
```
bundles.Add(new ScriptBundle("~/js/jquery").Include(
                        "~/Scripts/jquery-{version}.js"));
```
2、在View页面
`@Scripts.Render("~/js/jquery")`
在 1 里面，有
`jquery-{version}.js`
这个{version}是什么意思呢？最开始的时候，我以为这是一个定义在配置文件里的KEY，但配置文件并没有。后来才知道，这是一个类似路由里通配符一类的东西。它的意思是，凡文件名符合这个规则的，都会包含在里面。比如下面这几个脚本文件：
![](https://img-blog.csdn.net/20140214214925921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
都会被压缩在一起，一并调用。很明显，这有时可能会引发错误或者意想不到的情况。

