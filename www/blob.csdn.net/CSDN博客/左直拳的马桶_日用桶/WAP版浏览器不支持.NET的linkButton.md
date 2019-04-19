# WAP版浏览器不支持.NET的linkButton - 左直拳的马桶_日用桶 - CSDN博客
2012年11月06日 17:40:44[左直拳](https://me.csdn.net/leftfist)阅读数：1892
个人分类：[.NET																[Web](https://blog.csdn.net/leftfist/article/category/674421)](https://blog.csdn.net/leftfist/article/category/94497)

在我们网站FOR WAP版的某个页面上放了两个LinkButton，结果用手机自带的WAP浏览器看的时候，提示 通信协议不支持。
改成 Button 就可以了。
ImageButton的话，点击不会报错，页面虽然刷新，但没有执行相应的事件。不过页面确实是有提交的，只是没有跟ImageButton的事件挂钩。可以在Page_Load里查出是哪个控件触发了提交，如果发现是这个ImageButton的话，就执行它的事件。关键是怎么找出是谁触发了提交。
查找代码如下：
```
string GetPostBackControlName()
    {
        Control control = null;
        string ctrlname = Page.Request.Params["__EVENTTARGET"];
        if (ctrlname != null && ctrlname != String.Empty)
        {
            control = Page.FindControl(ctrlname);
        }
        else
        {
            Control c;
            foreach (string ctl in Page.Request.Form)
            {
                if (ctl.EndsWith(".x") || ctl.EndsWith(".y"))
                {
                    c = Page.FindControl(ctl.Substring(0, ctl.Length - 2));
                }
                else
                {
                    c = Page.FindControl(ctl);
                }
                if (c is System.Web.UI.WebControls.Button ||
                         c is System.Web.UI.WebControls.ImageButton)
                {
                    control = c;
                    break;
                }
            }
        }
        if (control != null)
            return control.ID;
        else
            return string.Empty;
    }
```
