# 针对UCWEB的一些开发 - 左直拳的马桶_日用桶 - CSDN博客
2012年11月08日 15:04:02[左直拳](https://me.csdn.net/leftfist)阅读数：1179
个人分类：[.NET																[Web](https://blog.csdn.net/leftfist/article/category/674421)](https://blog.csdn.net/leftfist/article/category/94497)
UCWEB浏览器，支持ViewState，但里面的内容不能太长，否则会报错。
ImageButton的话，提交同样也对应不上它的OnClick事件。并且似乎也不能确定是不是它触发的提交15:08
```
protected void Page_Load(object sender, EventArgs e)
    {
        if (IsPostBack)
        {
            if (GetPostBackControlName().CompareTo("imgPhoto") == 0)
            {
                NextPhoto();
            }
            else
            {
                string name = GetPostBackControlName();
                if(String.IsNullOrEmpty(labNote2.Text))
                labNote.Text = "无名";
            }
        }
    }
```
将会得到一个“无名”的结果。
有关查找哪个控件触发提交事件
`GetPostBackControlName()`
及相关情况，请参考
[http://blog.csdn.net/leftfist/article/details/8153950](http://blog.csdn.net/leftfist/article/details/8153950)
